/*
 * Copyright (c) 2025 Karoly Molnar
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#define LOG_LEVEL CONFIG_DISPLAY_LOG_LEVEL
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(gu_vfd);

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/spi.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/drivers/display.h>
#include <string.h>
#include "display_gu_vfd.h"

/* --- Device tree instance --- */
#define DT_DRV_COMPAT noritake_gu_vfd

/**
 * @brief Complete UART DT information
 * In contrary to I2C and SPI, this dt_spec is not available in the respective driver header.
 */
struct uart_dt_spec {
    /** UART */
    const struct device *dev;
    /** UART specific configuration */
    struct uart_config config;
};

union gu_vfd_bus {
    struct i2c_dt_spec i2c;
    struct spi_dt_spec spi;
    struct uart_dt_spec uart;
};

typedef bool (*gu_vfd_connection_ready_fn)(const struct device *dev);

typedef int (*gu_vfd_write_fn)(const struct device *dev, uint8_t data);

typedef int (*gu_vfd_read_fn)(const struct device *dev, uint8_t *data);

typedef const char *(*gu_vfd_connection_name_fn)(const struct device *dev);

static int gu_vfd_write_buffer(const struct device *dev, uint8_t *buf, size_t len);

static int gu_vfd_binary_mode(const struct device *dev);

static int gu_vfd_graphic_write(const struct device *dev, uint8_t data[], uint8_t len);

static int gu_vfd_vertical_mode(const struct device *dev);

static int gu_vfd_set_brightness(const struct device *dev, uint8_t brightness);

static int gu_vfd_set_pos(const struct device *dev, uint8_t x, uint8_t y);

struct gu_vfd_data {
    uint8_t *dummy_buffer;
};

struct gu_vfd_config {
    union gu_vfd_bus connection;
    struct gpio_dt_spec mb_pin;
    struct gpio_dt_spec hb_pin;
    struct gpio_dt_spec reset_pin;
    gu_vfd_connection_ready_fn connection_ready;
    gu_vfd_write_fn write;
    gu_vfd_read_fn read;
    gu_vfd_connection_name_fn connection_name;
    uint16_t height;
    uint16_t width;
};


static int gu_vfd_wait_for_ready(const struct device *dev) {
    int mb_state = 0;
    const struct gu_vfd_config *config = dev->config;
    do {
        mb_state = gpio_pin_get_dt(&config->mb_pin);
    } while (mb_state == 1);
    return 0;
}

static int gu_vfd_write(const struct device *dev, const uint16_t x,
                        const uint16_t y,
                        const struct display_buffer_descriptor *desc,
                        const void *buf) {
    int ret = 0;
    if (x > desc->width || y > desc->height) {
        return -EINVAL;
    }

    ret |= gu_vfd_set_pos(dev, (uint8_t)x, (uint8_t)y);

    uint8_t gfx[] = {0x1c, 0x5c, 0x48, 0x3e, 0x1d, 0x1d, 0x14, 0x36};

    ret |= gu_vfd_graphic_write(dev, gfx, sizeof(gfx));

    return ret;
}

static int gu_vfd_blanking_on(const struct device *dev) {
    uint8_t cmd[] = {GU_VFD_CMD_POWER_OFF >> 8, GU_VFD_CMD_POWER_OFF & 0xFFu};
    return gu_vfd_write_buffer(dev, cmd, sizeof(cmd));
}

static int gu_vfd_blanking_off(const struct device *dev) {
    uint8_t cmd[] = {GU_VFD_CMD_POWER_ON >> 8, GU_VFD_CMD_POWER_ON & 0xFFu};
    return gu_vfd_write_buffer(dev, cmd, sizeof(cmd));
}


static void gu_vfd_get_capabilities(const struct device *dev,
                                    struct display_capabilities *cap) {
    memset(cap, 0, sizeof(struct display_capabilities));
    const struct gu_vfd_config *config = dev->config;
    cap->x_resolution = config->width;
    cap->y_resolution = config->height;
    cap->supported_pixel_formats = PIXEL_FORMAT_MONO10;
    cap->current_pixel_format = PIXEL_FORMAT_MONO10;
    cap->screen_info = SCREEN_INFO_MONO_VTILED;
}

/* --- Init --- */
static int gu_vfd_init(const struct device *dev) {
    int ret = 0;
    const struct gu_vfd_config *config = dev->config;
    if (!config->connection_ready(dev)) {
        LOG_ERR("Serial connection device is not ready");
        return -ENODEV;
    }


    /* Reset if pin connected then perform physical Reset */
    if (config->reset_pin.port) {
        if (!gpio_is_ready_dt(&config->reset_pin)) {
            LOG_ERR("RESET GPIO device not ready");
            return -ENODEV;
        }

        ret = gpio_pin_configure_dt(&config->reset_pin, GPIO_OUTPUT_INACTIVE);
        if (ret < 0) {
            LOG_ERR("Unable to set RESET pin to OUTPUT");
            return ret;
        }

        /* asserting the reset pin for a short time */
        gpio_pin_set_dt(&config->reset_pin, 1);
        //k_busy_wait(GU_VFD_DELAY_RESET_ASSERT_US);
        k_sleep(K_USEC(GU_VFD_DELAY_RESET_ASSERT_US));
        gpio_pin_set_dt(&config->reset_pin, 0);
    }


    ret = gpio_pin_configure_dt(&config->mb_pin, GPIO_INPUT);
    if (ret < 0) {
        LOG_ERR("Unable to set MB pin to INPUT");
        return ret;
    }

    if (!gpio_is_ready_dt(&config->mb_pin)) {
        LOG_ERR("MB GPIO device not ready");
        return -ENODEV;
    }

    /* The device needs several 10 milliseconds after reset to be ready */
    k_sleep(K_MSEC(GU_VFD_DELAY_AFTER_RESET_MS));

    gu_vfd_wait_for_ready(dev);
    const int mb_state = gpio_pin_get_dt(&config->mb_pin);
    if (mb_state != 0) {
        LOG_ERR("MB GPIO pin shall be LOW after init");
        return -ENODEV;
    }

    ret = gu_vfd_binary_mode(dev);
    ret |= gu_vfd_vertical_mode(dev);
    ret |= gu_vfd_set_brightness(dev,CONFIG_GU_VFD_DEFAULT_BRIGHTNESS);

    return ret;
}

static int gu_vfd_deinit(const struct device *dev) {
    /* wait for finalizing current operation */
    gu_vfd_wait_for_ready(dev);
    return 0;
}

static int gu_vfd_vertical_mode(const struct device *dev) {
    uint8_t cmd[2] = {GU_VFD_CMD_WRITE_MODE, GU_VFD_WRITE_MODE_GFX_VERTICAL};
    return gu_vfd_write_buffer(dev, cmd, sizeof(cmd));
}

static int gu_vfd_graphic_write(const struct device *dev, uint8_t data[], uint8_t len) {
    int ret = 0;
    uint8_t cmd[] = {GU_VFD_CMD_GRAPHIC_WRITE, len};
    ret = gu_vfd_write_buffer(dev, cmd, sizeof(cmd));
    ret |= gu_vfd_write_buffer(dev, data, len);
    return ret;
}

static int gu_vfd_set_brightness(const struct device *dev, const uint8_t brightness) {
    int ret = 0;
    if (brightness > 8) {
        ret = -EINVAL;
    } else {
        uint8_t cmd[] = {GU_VFD_CMD_BRIGHTNESS, GU_VFD_BRIGHTNESS_0 + brightness};
        ret = gu_vfd_write_buffer(dev, cmd, sizeof(cmd));
    }

    return ret;
}

static int gu_vfd_clear(const struct device *dev) {
    const struct gu_vfd_config *config = dev->config;
    uint8_t cmd[] = {GU_VFD_CMD_CLEAR_AREA, 0, 0, config->width, config->height};
    return gu_vfd_write_buffer(dev, cmd, sizeof(cmd));
}

static int gu_vfd_binary_mode(const struct device *dev) {
    uint8_t cmd[2] = {GU_VFD_CMD_BINARY_MODE >> 8,GU_VFD_CMD_BINARY_MODE & 0xFF};
    return gu_vfd_write_buffer(dev, cmd, sizeof(cmd));
}


static int gu_vfd_set_pos(const struct device *dev, uint8_t x, uint8_t y) {
    uint8_t cmd[] = {GU_VFD_CMD_CURSOR_POS, x, y};
    return gu_vfd_write_buffer(dev, cmd, sizeof(cmd));
}

#if (DT_HAS_COMPAT_ON_BUS_STATUS_OKAY(DT_DRV_COMPAT, spi))
static bool gu_vfd_connection_ready_spi(const struct device *dev) {
    const struct gu_vfd_config *config = dev->config;

    return spi_is_ready_dt(&config->connection.spi);
}

static int gu_vfd_write_buffer(const struct device *dev, uint8_t *const buf, const size_t len) {
    const struct gu_vfd_config *config = dev->config;
    for (size_t i = 0; i < len; i++) {
        uint8_t data = buf[i];
        config->write(dev, data);
        gu_vfd_wait_for_ready(dev);
    }
    return 0;
}

static int gu_vfd_write_spi(const struct device *dev, uint8_t const data) {
    const struct gu_vfd_config *config = dev->config;

    uint8_t dummy_rx = 0;

    struct spi_buf tx_buf = {
        .buf = (uint8_t *) &data,
        .len = 1
    };

    const struct spi_buf_set tx_bufs = {
        .buffers = &tx_buf,
        .count = 1
    };

    struct spi_buf rx_buf = {
        .buf = &dummy_rx,
        .len = 0
    };

    const struct spi_buf_set rx_bufs = {
        .buffers = &rx_buf,
        .count = 0
    };
    return spi_transceive_dt(&config->connection.spi, &tx_bufs, &rx_bufs);
}

static int gu_vfd_read_spi(const struct device *dev, uint8_t *data) {
    const struct gu_vfd_config *config = dev->config;
    uint8_t dummy_tx = GU_VFD_CMD_DUMMY;


    const struct spi_buf rx_buf = {
        .buf = data,
        .len = 1
    };

    const struct spi_buf_set rx_bufs = {
        .buffers = &rx_buf,
        .count = 1
    };

    const struct spi_buf tx_buf = {
        .buf = &dummy_tx,
        .len = 1
    };

    const struct spi_buf_set tx_bufs = {
        .buffers = &tx_buf,
        .count = 1
    };

    return spi_transceive_dt(&config->connection.spi, &tx_bufs, &rx_bufs);
}

static const char *gu_vfd_connection_name_spi(const struct device *dev) {
    const struct gu_vfd_config *config = dev->config;

    return config->connection.spi.bus->name;
}
#endif


static DEVICE_API(display, gu_vfd_driver_api) = {
    .blanking_on = gu_vfd_blanking_on,
    .blanking_off = gu_vfd_blanking_off,
    .write = gu_vfd_write,
    .clear = gu_vfd_clear,
    .set_brightness = gu_vfd_set_brightness,
    .get_capabilities = gu_vfd_get_capabilities,
};


#define GU_VFD_CONFIG_SPI(node_id)                                          \
    .connection = {.spi = SPI_DT_SPEC_GET(                                  \
    node_id, SPI_OP_MODE_MASTER | SPI_TRANSFER_MSB | SPI_WORD_SET(8), 0)},  \
    .connection_ready = gu_vfd_connection_ready_spi,                        \
    .write = gu_vfd_write_spi,                                              \
    .read = gu_vfd_read_spi,                                                \
    .connection_name = gu_vfd_connection_name_spi,


#define GU_VFD_CONFIG_I2C(node_id)                                  \
    .bus = {.i2c = I2C_DT_SPEC_GET(node_id)},                       \
    .connection_ready = gu_vfd_connection_ready_spi,                \
    .write = gu_vfd_write_spi,                                      \
    .read = gu_vfd_read_spi,                                      \
    .connection_name = gu_vfd_connection_name_spi,

#define GU_VFD_DEFINE(node_id)                                                      \
    static struct gu_vfd_data data##node_id;                                        \
	static const struct gu_vfd_config config##node_id = {                           \
        .height = DT_PROP(node_id, height),                                         \
        .width = DT_PROP(node_id, width),                                           \
        .mb_pin = GPIO_DT_SPEC_GET(node_id, mb_gpios),                              \
        .reset_pin = GPIO_DT_SPEC_GET(node_id, reset_gpios),                        \
        COND_CODE_1(DT_ON_BUS(node_id, spi), (GU_VFD_CONFIG_SPI(node_id)),          \
            (GU_VFD_CONFIG_I2C(node_id)))                                           \
	};                                                                              \
                                                                                    \
	DEVICE_DT_DEINIT_DEFINE(node_id, gu_vfd_init, gu_vfd_deinit, NULL, &data##node_id, &config##node_id,  \
			 POST_KERNEL, CONFIG_DISPLAY_INIT_PRIORITY, &gu_vfd_driver_api);

DT_FOREACH_STATUS_OKAY(DT_DRV_COMPAT, GU_VFD_DEFINE)

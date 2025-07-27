/*
 * Copyright (c) 2025 Karoly Molnar
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <string.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/display.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(vfd_display_sample, LOG_LEVEL_DBG);

int main(void)
{
	const struct device *const dev = DEVICE_DT_GET(DT_NODELABEL(vfd_display));

	if (!device_is_ready(dev)) {
		LOG_ERR("display device is not ready.");
		return 0;
	}

	return 0;
}

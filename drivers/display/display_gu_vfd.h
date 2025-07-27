/*
 * Copyright (c) 2025 Karoly Molnar
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _DISPLAY_GU_VFD_H_
#define _DISPLAY_GU_VFD_H_

#define GU_VFD_DELAY_AFTER_RESET_MS    60u
#define GU_VFD_DELAY_RESET_ASSERT_US   2u

/*! Dummy Byte - Code: 00H (50µs) */
#define GU_VFD_CMD_DUMMY    0x0u

/*! Macro Start - Code: 01H – 07H (BUSY time depends on contents) */
#define GU_VFD_CMD_MACRO(x)    (0x1u + (x))

/*! Back Space - Code: 08H (50µs) */
#define GU_VFD_CMD_BSP    0x8u

/*! Horizontal Tab - Code: 09H (50µs) */
#define GU_VFD_CMD_HT    0x9u

/*! Line Feed - Code: 0AH (50µs) */
#define GU_VFD_CMD_LF    0xAu

/*! Home - Code: 0BH (50µs) */
#define GU_VFD_CMD_HOME    0xBu

/*! Vertical Tab - Code: 0CH (50µs) */
#define GU_VFD_CMD_VT    0xCu

/*! Carriage Return - Code: 0DH (50µs) */
#define GU_VFD_CMD_CR    0xDu

/*! Clear EOL - Code: 0EH (2.5ms) */
#define GU_VFD_CMD_CLEAR_EOL    0xEu

/*! Test - Code: 0FH (50µs) */
#define GU_VFD_CMD_TEST    0xFu

/*! Cursor Position - Code: 10H + X pos + Y pos (50µs) */
#define GU_VFD_CMD_CURSOR_POS    0x10u

/*! Set Area - Code: 11H + X left + Y top + X right + Y bottom (50µs + 1ms [Last Byte]) */
#define GU_VFD_CMD_SET_AREA    0x11u

/*! Clear Area - Code: 12H + X left + Y top + X right + Y bottom (50µs + 1ms [Last Byte]) */
#define GU_VFD_CMD_CLEAR_AREA    0x12u

/*! Invert Area - Code: 13H + X left + Y top + X right + Y bottom (50µs + 1ms [Last Byte]) */
#define GU_VFD_CMD_INVERT_AREA    0x13u

/*! Set Outline - Code: 14H + X left + Y top + X right + Y bottom (50µs + 1ms [Last Byte]) */
#define GU_VFD_CMD_SET_OUTLINE    0x14u

/*! Clear Outline - Code: 15H + X left + Y top + X right + Y bottom (50µs + 1ms [Last Byte]) */
#define GU_VFD_CMD_CLEAR_OUTLINE    0x15u

/*! Set Pixel - Code: 16H (50µs) */
#define GU_VFD_CMD_SET_PIXEL    0x16u

/*! Clear Pixel - Code: 17H (50µs) */
#define GU_VFD_CMD_CLEAR_PIXEL    0x17u

/*! Graphic Write - Code: 18H + Length + Data (50µs + 250µs [Each Data Byte]) */
#define GU_VFD_CMD_GRAPHIC_WRITE    0x18u

/*! Reset - Code: 19H (500µs) */
#define GU_VFD_CMD_RESET    0x19u

/*! Write Mode - Code: 1AH + Data (50µs) */
#define GU_VFD_CMD_WRITE_MODE    0x1Au

#define GU_VFD_WRITE_MODE_GFX_HORIZONTAL    (0u<<7)
#define GU_VFD_WRITE_MODE_GFX_VERTICAL      (1u<<7)
#define GU_VFD_WRITE_MODE_CURSOR_HORIZONTAL (0u<<6)
#define GU_VFD_WRITE_MODE_CURSOR_VERTICAL   (1u<<6)
#define GU_VFD_WRITE_MODE_CURSOR_FORWARD    (0u<<5)
#define GU_VFD_WRITE_MODE_CURSOR_BACKWARD   (1u<<5)
#define GU_VFD_WRITE_MODE_UNDERSCORE_OFF    (0u<<4)
#define GU_VFD_WRITE_MODE_UNDERSCORE_ON     (1u<<4)
#define GU_VFD_WRITE_MODE_UNDERSCORE_STATIC (0u<<3)
#define GU_VFD_WRITE_MODE_UNDERSCORE_FLASH  (1u<<3)
#define GU_VFD_WRITE_MODE_PEN_OVERWRITE     (0u<<0)
#define GU_VFD_WRITE_MODE_PEN_AND           (1u<<0)
#define GU_VFD_WRITE_MODE_PEN_OR            (2u<<0)
#define GU_VFD_WRITE_MODE_PEN_XOR           (3u<<0)

/*! Set Macro - Code: 1BH + Macro Number + Length + Data (50µs + 250µs [Each Data Byte]) */
#define GU_VFD_CMD_SET_MACRO    0x1Bu

/*! Brightness - Code: 1BH + Level (50µs) */
#define GU_VFD_CMD_BRIGHTNESS    0x1Bu

#define GU_VFD_BRIGHTNESS_0      0xF8u

/*! Erase Macros - Code: 1BH + 4DH (250ms) */
#define GU_VFD_CMD_ERASE_MACROS    0x1B4Du

/*! Lock/Unlock EEPROM - Code: 1BH + 4CH + 55H (50µs + 40ms [Last Byte]) */
#define GU_VFD_CMD_LOCK_EEPROM    0x1B4Cu
#define GU_VFD_CMD_UNLOCK_EEPROM  0x1B55u


/*! Checksum - Code: 1BH + 43H (50µs) */
#define GU_VFD_CMD_CHECKSUM    0x1B43u

/*! Power On/Off - Code: 1BH + 50H / 46H (50µs) */
#define GU_VFD_CMD_POWER_ON    0x1B50u
#define GU_VFD_CMD_POWER_OFF   0x1B46u

/*! Hex/Binary Mode - Code: 1BH + 48H / 42H (50µs) */
#define GU_VFD_CMD_HEX_MODE    0x1B48u
#define GU_VFD_CMD_BINARY_MODE 0x1B42u

/*! Set Serial Communications - Code: 1BH + 49H + Data */
#define GU_VFD_SERIAL_AUTO_IO_SEND_ON   (1u<<7)
#define GU_VFD_SERIAL_AUTO_IO_SEND_OFF  (0u<<7)
#define GU_VFD_SERIAL_PACKET_MODE_ON    (1u<<6)
#define GU_VFD_SERIAL_PACKET_MODE_OFF   (0u<<6)
#define GU_VFD_SERIAL_COMM_BUFFER_ON    (1u<<5)
#define GU_VFD_SERIAL_COMM_BUFFER_OFF   (0u<<5)
#define GU_VFD_SERIAL_PARITY_EVEN       (1u<<2)
#define GU_VFD_SERIAL_PARITY_NONE       (0u<<2)
#define GU_VFD_SERIAL_BAUD_4800         (0u<<0)
#define GU_VFD_SERIAL_BAUD_9600         (1u<<0)
#define GU_VFD_SERIAL_BAUD_19200        (2u<<0)
#define GU_VFD_SERIAL_BAUD_38400        (3u<<0)
#define GU_VFD_SERIAL_BAUD_57600        (4u<<0)
#define GU_VFD_SERIAL_BAUD_76800        (5u<<0)
#define GU_VFD_SERIAL_BAUD_1200         (6u<<0)
#define GU_VFD_SERIAL_BAUD_2400         (7u<<0)

/*! Enable I/O Port - Code: 1BH + 44H + Data (50µs + 80ms[Last Byte]) */
#define GU_VFD_CMD_ENABLE_IO_PORT       0x1B44u

/*! Read Port - Code: 1BH + 52H (50µs) */
#define GU_VFD_CMD_READ_PORT            0x1B52u

/*! Set Port Lines - Code: 1BH + 4FH + Data (50µs) */
#define GU_VFD_CMD_SET_PORT_LINES      0x1B4Fu

/*! Enable Key Scanning - Code: 1BH + 4BH (50µs + 40ms [Last Byte]) */
#define GU_VFD_CMD_ENABLE_KEY_SCAN      0x1B4Bu


/*! Select Font - Code: 1CH / 1DH / 1EH (50µs) */
#define GU_VFD_CMD_SELECT_FONT_PROP    0x1Cu
#define GU_VFD_CMD_SELECT_FONT_5X7     0x1Du
#define GU_VFD_CMD_SELECT_FONT_10X14   0x1Eu

/*! Graphic Area Write - Code: 1FH + X Left + Y Top + X Right + Y Bottom + Data (50µs + 250µs [Each Data Byte]) */
#define GU_VFD_CMD_GRAPHIC_AREA_WRITE    0x1Fu

/*! Window 1 Select - Code: 1BH + 80H (50µs) */
#define GU_VFD_CMD_WINDOW_1_SELECT    0x1B80u

/*! Window 2 Select - Code: 1BH + 81H (50µs) */
#define GU_VFD_CMD_WINDOW_2_SELECT    0x1B81u

/*! Window Define - Code: 1BH + 82H + X Left + Y Top + X Right + Y Bottom (50µs + 60µs [Last Byte]) */
#define GU_VFD_CMD_WINDOW_DEFINE    0x1B82u

/*! Window Mode - Code: 1BH + 83H + Mode (50µs) */
#define GU_VFD_CMD_WINDOW_MODE    0x1B83u

#define GU_VFD_WINDOW_MODE_INVERT   0u
#define GU_VFD_WINDOW_MODE_CLEAR    1u
#define GU_VFD_WINDOW_MODE_FILL     2u
#define GU_VFD_WINDOW_MODE_PATTERN  3u

/*! Window Show - Code: 1BH + 84H (50µs) */
#define GU_VFD_CMD_WINDOW_SHOW   0x1B84u

/*! Window Kill - Code: 1BH + 85H (50µs) */
#define GU_VFD_CMD_WINDOW_KILL   0x1B85u

/*! Window Flash - Code: 1BH + 86H + Number (50µs) */
#define GU_VFD_CMD_WINDOW_FLASH 0x1B86u

#define GU_VFD_FLASH_INFINITE   0xFFu
#define GU_VFD_FLASH_STOP       0u

/* Window Flash Speed - Code: 1BH + 87H + Speed (50µs) */
#define GU_VFD_CMD_WINDOW_FLASH_SPEED 0x1B87u

#define GU_VFD_FLASH_ON_SPEED_15MS    (0u<<4)
#define GU_VFD_FLASH_ON_SPEED_30MS    (1u<<4)
#define GU_VFD_FLASH_ON_SPEED_45MS    (2u<<4)
#define GU_VFD_FLASH_ON_SPEED_100MS   (3u<<4)
#define GU_VFD_FLASH_ON_SPEED_150MS   (4u<<4)
#define GU_VFD_FLASH_ON_SPEED_200MS   (5u<<4)
#define GU_VFD_FLASH_ON_SPEED_250MS   (6u<<4)
#define GU_VFD_FLASH_ON_SPEED_350MS   (7u<<4)
#define GU_VFD_FLASH_ON_SPEED_500MS   (8u<<4)
#define GU_VFD_FLASH_ON_SPEED_750MS   (9u<<4)
#define GU_VFD_FLASH_ON_SPEED_1000MS  (10u<<4)
#define GU_VFD_FLASH_ON_SPEED_1500MS  (11u<<4)
#define GU_VFD_FLASH_ON_SPEED_2000MS  (12u<<4)
#define GU_VFD_FLASH_ON_SPEED_2500MS  (13u<<4)
#define GU_VFD_FLASH_ON_SPEED_3000MS  (14u<<4)
#define GU_VFD_FLASH_ON_SPEED_3500MS  (15u<<4)

#define GU_VFD_FLASH_OFF_SPEED_15MS   (0u<<0)
#define GU_VFD_FLASH_OFF_SPEED_30MS   (1u<<0)
#define GU_VFD_FLASH_OFF_SPEED_45MS   (2u<<0)
#define GU_VFD_FLASH_OFF_SPEED_100MS  (3u<<0)
#define GU_VFD_FLASH_OFF_SPEED_150MS  (4u<<0)
#define GU_VFD_FLASH_OFF_SPEED_200MS  (5u<<0)
#define GU_VFD_FLASH_OFF_SPEED_250MS  (6u<<0)
#define GU_VFD_FLASH_OFF_SPEED_350MS  (7u<<0)
#define GU_VFD_FLASH_OFF_SPEED_500MS  (8u<<0)
#define GU_VFD_FLASH_OFF_SPEED_750MS  (9u<<0)
#define GU_VFD_FLASH_OFF_SPEED_1000MS (10u<<0)
#define GU_VFD_FLASH_OFF_SPEED_1500MS (11u<<0)
#define GU_VFD_FLASH_OFF_SPEED_2000MS (12u<<0)
#define GU_VFD_FLASH_OFF_SPEED_2500MS (13u<<0)
#define GU_VFD_FLASH_OFF_SPEED_3000MS (14u<<0)
#define GU_VFD_FLASH_OFF_SPEED_3500MS (15u<<0)

/*! Window Wipe Effect - Code: 1BH + 88H + Wipe (50µs) */
#define GU_VFD_CMD_WINDOW_WIPE  0x1B88u

#define GU_VFD_WIPE_LEFT_TO_RIGHT_COVER       0u
#define GU_VFD_WIPE_RIGHT_TO_LEFT_COVER       1u
#define GU_VFD_WIPE_TOP_TO_BOTTOM_COVER       2u
#define GU_VFD_WIPE_BOTTOM_TO_TOP_COVER       3u
#define GU_VFD_WIPE_LEFT_TO_RIGHT_UNCOVER     4u
#define GU_VFD_WIPE_RIGHT_TO_LEFT_UNCOVER     5u
#define GU_VFD_WIPE_TOP_TO_BOTTOM_UNCOVER     6u
#define GU_VFD_WIPE_BOTTOM_TO_TOP_UNCOVER     7u
#define GU_VFD_WIPE_HORIZONTAL_CENTER_TO_EDGE_COVER    8u
#define GU_VFD_WIPE_HORIZONTAL_EDGE_TO_CENTER_UNCOVER  9u
#define GU_VFD_WIPE_VERTICAL_CENTER_TO_EDGE_COVER      10u
#define GU_VFD_WIPE_VERTICAL_EDGE_TO_CENTER_UNCOVER    11u


/*! Window Wipe Speed - Code: 1BH + 89H + Speed (50µs) */
#define GU_VFD_CMD_WINDOW_WIPE_SPEED  0x1B89u

#define GU_VFD_WIPE_SPEED_HALT    0x00u
#define GU_VFD_WIPE_SPEED_17HZ    0x01u
#define GU_VFD_WIPE_SPEED_35HZ    0x02u
#define GU_VFD_WIPE_SPEED_52HZ    0x03u
#define GU_VFD_WIPE_SPEED_70HZ    0x04u
#define GU_VFD_WIPE_SPEED_87HZ    0x05u
#define GU_VFD_WIPE_SPEED_105HZ   0x06u
#define GU_VFD_WIPE_SPEED_122HZ   0x07u
#define GU_VFD_WIPE_SPEED_140HZ   0x08u
#define GU_VFD_WIPE_SPEED_157HZ   0x09u
#define GU_VFD_WIPE_SPEED_315HZ   0x10u
#define GU_VFD_WIPE_SPEED_175HZ   0x0Au
#define GU_VFD_WIPE_SPEED_192HZ   0x0Bu
#define GU_VFD_WIPE_SPEED_210HZ   0x0Cu
#define GU_VFD_WIPE_SPEED_227HZ   0x0Du
#define GU_VFD_WIPE_SPEED_245HZ   0x0Eu
#define GU_VFD_WIPE_SPEED_262HZ   0x0Fu

/*! Window Pattern Select - Code: 1BH + 8DH + Pattern (50µs) */
#define GU_VFD_CMD_WINDOW_PATTERN_SELECT    0x1B8Du

/*! Window Pattern Data - Code: 1BH + 8EH + Data (50µs) */
#define GU_VFD_CMD_WINDOW_PATTERN_DATA    0x1B8Eu

/*! Window Pattern Option - Code: 1BH + 8FH + Option (50µs) */
#define GU_VFD_CMD_WINDOW_PATTERN_OPTION    0x1B8Fu

#define GU_VFD_PATTERN_OPTION_INVERT_ON     (1u<<3)
#define GU_VFD_PATTERN_OPTION_INVERT_OFF    (0u<<3)
#define GU_VFD_PATTERN_OPTION_ALIGNMENT_ON  (1u<<2)
#define GU_VFD_PATTERN_OPTION_ALIGNMENT_OFF (0u<<2)
#define GU_VFD_PATTERN_OPTION_ALIGNMENT_BOTTOM  (0u<<1)
#define GU_VFD_PATTERN_OPTION_ALIGNMENT_TOP     (1u<<1)
#define GU_VFD_PATTERN_OPTION_ALIGNMENT_RIGHT   (0u<<0)
#define GU_VFD_PATTERN_OPTION_ALIGNMENT_LEFT    (1u<<0)


/*! Scroll Text In Window - Code: 1BH + 90H + Mode + No + Data (50µs + no of data bytes * 50µs [last byte]) */
#define GU_VFD_CMD_SCROLL_TEXT_IN_WINDOW    0x1B90u

#define GU_VFD_SCROLL_TEXT_PAD_ON       (1u<<5)
#define GU_VFD_SCROLL_TEXT_PAD_OFF      (0u<<5)
#define GU_VFD_SCROLL_TEXT_WINDOW_ON    (1u<<4)
#define GU_VFD_SCROLL_TEXT_WINDOW_OFF   (0u<<4)
#define GU_VFD_SCROLL_TEXT_WINDOW_UP    0u
#define GU_VFD_SCROLL_TEXT_WINDOW_DOWN  1u
#define GU_VFD_SCROLL_TEXT_WINDOW_LEFT  2u
#define GU_VFD_SCROLL_TEXT_WINDOW_RIGHT 3u


/*! Scroll Speed - Code: 1BH + 91H + Speed (50µs) */
#define GU_VFD_CMD_SCROLL_SPEED    0x1B91u

#define GU_VFD_SCROLL_SPEED_HALT    0x00u
#define GU_VFD_SCROLL_SPEED_35HZ    0x01u
#define GU_VFD_SCROLL_SPEED_70HZ    0x02u
#define GU_VFD_SCROLL_SPEED_105HZ    0x03u
#define GU_VFD_SCROLL_SPEED_140HZ    0x04u
#define GU_VFD_SCROLL_SPEED_175HZ    0x05u
#define GU_VFD_SCROLL_SPEED_210HZ    0x06u
#define GU_VFD_SCROLL_SPEED_245HZ    0x07u
#define GU_VFD_SCROLL_SPEED_315HZ    0x08u

/*! Select Extended Font - Code: 1BH + 98H + Font (50µs) */
#define GU_VFD_CMD_SELECT_EXTENDED_FONT    0x1B98u

#define GU_VFD_EXTENDED_FONT_5X5        0u
#define GU_VFD_EXTENDED_FONT_5X7        1u
#define GU_VFD_EXTENDED_FONT_10X14      2u
#define GU_VFD_EXTENDED_FONT_7X15       3u
#define GU_VFD_EXTENDED_FONT_5X7_CYR    4u
#define GU_VFD_EXTENDED_FONT_10X14_CYR  5u
#define GU_VFD_EXTENDED_FONT_SPACING_FIXED  (0u<<3)
#define GU_VFD_EXTENDED_FONT_SPACING_PROP   (1u<<3)
#define GU_VFD_EXTENDED_FONT_SPACING_H1     (0u<<4)
#define GU_VFD_EXTENDED_FONT_SPACING_H2     (1u<<4)
#define GU_VFD_EXTENDED_FONT_SPACING_H3     (2u<<4)
#define GU_VFD_EXTENDED_FONT_SPACING_H4     (3u<<4)
#define GU_VFD_EXTENDED_FONT_SPACING_H5     (4u<<4)
#define GU_VFD_EXTENDED_FONT_SPACING_H6     (5u<<4)
#define GU_VFD_EXTENDED_FONT_SPACING_H7     (6u<<4)
#define GU_VFD_EXTENDED_FONT_SPACING_H8     (7u<<4)

/*! Draw Line - Code: 1BH + 9AH + X + Y (50µs) */
#define GU_VFD_CMD_DRAW_LINE    0x1B9Au

/*! Auto Fade - Code: 1BH + 9CH + Level (50µs) */
#define GU_VFD_CMD_AUTO_FADE    0x1B9Cu

#define GU_VFD_FADE_0PCT    0u
#define GU_VFD_FADE_14PCT   1u
#define GU_VFD_FADE_28PCT   2u
#define GU_VFD_FADE_43PCT   3u
#define GU_VFD_FADE_57PCT   4u
#define GU_VFD_FADE_71PCT   5u
#define GU_VFD_FADE_86PCT   6u
#define GU_VFD_FADE_100PCT  7u
#define GU_VFD_FADE_SPEED_1 (0u<<4)
#define GU_VFD_FADE_SPEED_2 (1u<<4)
#define GU_VFD_FADE_SPEED_3 (2u<<4)
#define GU_VFD_FADE_SPEED_4 (3u<<4)

/*! Command Delay - Code: 1BH + 9FH + Delay (50µs + delay [Last Byte]) */
#define GU_VFD_CMD_COMMAND_DELAY    0x1B9Fu

#define GU_VFD_COMMAND_DELAY_SCAN       0u
#define GU_VFD_COMMAND_DELAY_10MS(x)    (1u*(x))
#define GU_VFD_COMMAND_DELAY_SCROLL     0xF8u
#define GU_VFD_COMMAND_DELAY_W1_FLASH   0xFAu
#define GU_VFD_COMMAND_DELAY_W2_FLASH   0xFBu
#define GU_VFD_COMMAND_DELAY_W1_WIPE    0xFCu
#define GU_VFD_COMMAND_DELAY_W2_WIPE    0xFDu

#endif

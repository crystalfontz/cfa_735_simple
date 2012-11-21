#ifndef _PROCESS_CMD_H_INCLUDED_
#define _PROCESS_CMD_H_INCLUDED_

#define PAYLOAD_OFFSET       2
#define PAYLOAD_SIZE_OFFSET  1
#define COMMAND_ID_OFFSET    0
#define HEADER_SIZE          PAYLOAD_OFFSET
#define CS_SIZE              2
#define RAW_CMD_SIZE         (HEADER_SIZE + CS_SIZE)

#define CMD_FULL_SIZE(payload_size)   (RAW_CMD_SIZE + payload_size)


typedef struct __attribute__ ((__packed__))
{
	
	unsigned char cmd;
	unsigned char size;

}cmd_hdr_t;

#define CMD_EXP_SIZE(func)         (sizeof(func ## _rq_t) - HEADER_SIZE)

#define ADD_COMMAND(func, cmd_id)  { CMD_EXP_SIZE(func), CMD_EXP_SIZE(func), func, 0, cmd_id }
#define ADD_COMMAND_VARIADIC_SIZE(size_min, size_max, func, cmd_id)  { size_min, size_max, func, 0, cmd_id }
//#define ADD_COMMAND_FIX(size, func, cmd_id)  { CMD_EXP_SIZE(func), CMD_EXP_SIZE(func), func, 0, cmd_id }


/**
 * List of commands
 */
enum
{
	CMD_PING							= 0x00, // Ping
	CMD_WRITE_USER_FLASH_DATA			= 0x02,
	CMD_READ_USER_FLASH_DATA			= 0x03,
	CMD_RESET							= 0x05,
	CMD_CLEAR_LCD_SCREEN				= 0x06, // Clear screen
	CMD_UNDOCUMENTED_0x08				= 0x08,
	CMD_SET_SPECIAL_CHAR				= 0x09, // Set special character
	CMD_SET_LCD_CURSOR_POSITION			= 0x0B, // Send LCD cursor position
	CMD_SET_LCD_CURSOR_STYLE			= 0x0C,
	CMD_SET_LCD_CONTRAST				= 0x0D,
	CMD_SET_BACK_LIGHT					= 0x0E,
	CMD_SEND_DATA_TO_LCD				= 0x1F, // Send data to LCD
	CMD_SET_FAN							= 0x11,
	CMD_READ_DOW						= 0x12,
	CMD_CONF_KEY_REPORT					= 0x17,
	CMD_READ_KEYPAD						= 0x18,
	CMD_CONF_GPIO						= 0x22,
};

/**
 * This is how cmd looks like in the array of supported commands
 */
typedef struct
{
	/**
	 * minimum number of bytes in the message
	 */
	int size_min;

	/**
	 * maximum number of bytes in the message
	 */
	int size_max;

	/**
	 * callback which processes the command and sends response
	 */
	int (*handler)(uint8_t *buffer, int size);

	/**
	 * number of times the command called
	 */
	int calls;

	/**
	 * command identifier
	 */
	int command_id;
} cmd_t;

/**
 * Size of the array which contains all supported commands
 * Usually something like (sizeof(commands)/sizeof(commands[0])) will do
 * The variable should be declared in the cmd.c
 */
extern int commands_total;

/**
 * Array of commands handlers
 */
extern const cmd_t commands[];

/**
 * This macro will be helpful if I decide to modify prototype of
 * the call-backs in the future
 */
#define CMD_DECLARE_FUNCTION(name) int name(uint8_t *buffer, int size)

/**
 * Complete FSM which handles incoming commands
 * Returns non-zero (size of the command) if a
 * complete command was handled in this call
 */
extern int process_command(uint8_t *rx_buffer, unsigned int *size);

/**
 * Send response (from peripheral card) of the specified size.
 * The data is in the rx_buffer
 */
int cmd_send_slave(uint8_t *buffer, int size);

/**
 * Send asynchronous report to the host
 */
int cmd_send_report(uint8_t *buffer, int size);

/**
 * Debug counters
 */
typedef struct __attribute__ ((__packed__))
{
	uint16_t process_command;
	uint16_t process_command_bad_id;
	uint16_t process_command_bad_size_min;
	uint16_t process_command_bad_size_max;
	uint16_t process_command_bad_cs;
	uint16_t process_command_handler;
	uint16_t process_command_handler_done;
	uint16_t process_command_am_read;
	uint16_t process_command_am_write;
	uint16_t process_command_not_full;
} cmd_stat_t;

/**
 * System statistics. It is placed here by accident
 * File sys.h would be a better place but it does not
 * exist
 */
extern cmd_stat_t cmd_stat;


typedef struct __attribute__ ((__packed__))
{
	cmd_hdr_t hdr;

}cmd_ping_rq_t;

typedef struct __attribute__ ((__packed__))
{
	cmd_hdr_t hdr;

}cmd_ping_rs_t;


typedef struct __attribute__ ((__packed__))
{
	cmd_hdr_t hdr;

}cmd_clear_lcd_screen_rq_t;

typedef struct __attribute__ ((__packed__))
{
	cmd_hdr_t hdr;

}cmd_clear_lcd_screen_rs_t;


typedef struct __attribute__ ((__packed__))
{
	cmd_hdr_t hdr;
	unsigned char column;
	unsigned char row;
	unsigned char text[20];
}cmd_send_data_to_lcd_rq_t;

typedef struct __attribute__ ((__packed__))
{
	cmd_hdr_t hdr;

}cmd_send_data_to_lcd_rs_t;


typedef struct __attribute__ ((__packed__))
{
	cmd_hdr_t hdr;
	unsigned char column;
	unsigned char row;
}cmd_set_lcd_cursor_position_rq_t;

typedef struct __attribute__ ((__packed__))
{
	cmd_hdr_t hdr;

}cmd_set_lcd_cursor_position_rs_t;

typedef struct __attribute__ ((__packed__))
{
	cmd_hdr_t hdr;
	unsigned char style;
}cmd_set_lcd_cursor_style_rq_t;

typedef struct __attribute__ ((__packed__))
{
	cmd_hdr_t hdr;

}cmd_set_lcd_cursor_style_rs_t;

typedef struct __attribute__ ((__packed__))
{
	cmd_hdr_t hdr;
	unsigned char contrast;
}cmd_set_lcd_contrast_rq_t;

typedef struct __attribute__ ((__packed__))
{
	cmd_hdr_t hdr;

}cmd_set_lcd_contrast_rs_t;

typedef struct __attribute__ ((__packed__))
{
	cmd_hdr_t hdr;
	unsigned char brightness;
}cmd_set_back_light_rq_t;

typedef struct __attribute__ ((__packed__))
{
	cmd_hdr_t hdr;

}cmd_set_back_light_rs_t;

typedef struct __attribute__ ((__packed__))
{
	cmd_hdr_t hdr;
	unsigned char press_mask;
	unsigned char release_mask;
}cmd_conf_key_report_rq_t;

typedef struct __attribute__ ((__packed__))
{
	cmd_hdr_t hdr;

}cmd_conf_key_report_rs_t;

typedef struct __attribute__ ((__packed__))
{
	cmd_hdr_t hdr;
}cmd_read_keypad_rq_t;

typedef struct __attribute__ ((__packed__))
{
	cmd_hdr_t hdr;
	unsigned char pressed;
	unsigned char pressed_from_last_poll;
	unsigned char released_from_last_poll;
}cmd_read_keypad_rs_t;

typedef struct __attribute__ ((__packed__))
{
	cmd_hdr_t hdr;
	unsigned char device_index;
}cmd_read_dow_rq_t;

typedef struct __attribute__ ((__packed__))
{
	cmd_hdr_t hdr;
	unsigned char device_index;
	unsigned char rom_id[8];
}cmd_read_dow_rs_t;

extern CMD_DECLARE_FUNCTION (cmd_ping);
extern CMD_DECLARE_FUNCTION (cmd_clear_lcd_screen);
extern CMD_DECLARE_FUNCTION (cmd_send_data_to_lcd);
extern CMD_DECLARE_FUNCTION(cmd_set_lcd_cursor_position);
extern CMD_DECLARE_FUNCTION(cmd_set_lcd_cursor_style);
extern CMD_DECLARE_FUNCTION(cmd_set_lcd_contrast);
extern CMD_DECLARE_FUNCTION(cmd_set_back_light);
extern CMD_DECLARE_FUNCTION(cmd_return_ok);
extern CMD_DECLARE_FUNCTION(cmd_conf_key_report);
extern CMD_DECLARE_FUNCTION(cmd_read_keypad);
extern CMD_DECLARE_FUNCTION(cmd_set_special_character);
extern CMD_DECLARE_FUNCTION(cmd_read_dow);
extern CMD_DECLARE_FUNCTION(cmd_udocumented_0x08);
extern CMD_DECLARE_FUNCTION(cmd_read_user_flash_data);
extern CMD_DECLARE_FUNCTION(cmd_reset);


#endif // _PROCESS_CMD_H_INCLUDED_

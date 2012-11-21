#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "uart.h"
#include "process_cmd.h"
#include "keys.h"
#include "stm32f10x.h"
#include "lcd.h"
#include "usb_vcom.h"
#include "08x08fnt.h"


#define PRINT_TRACE 0
#define PIXELS_IN_COLUMN 8
#define PIXELS_IN_ROM 8

const cmd_t gen_commands[] = {
	ADD_COMMAND_VARIADIC_SIZE(0, 255, cmd_ping, CMD_PING),
	ADD_COMMAND(cmd_clear_lcd_screen, CMD_CLEAR_LCD_SCREEN),
	ADD_COMMAND_VARIADIC_SIZE(3, 22, cmd_send_data_to_lcd, CMD_SEND_DATA_TO_LCD),
	ADD_COMMAND(cmd_set_lcd_cursor_position, CMD_SET_LCD_CURSOR_POSITION),
	ADD_COMMAND(cmd_set_lcd_cursor_style, CMD_SET_LCD_CURSOR_STYLE),
	ADD_COMMAND(cmd_set_lcd_contrast, CMD_SET_LCD_CONTRAST),
	ADD_COMMAND(cmd_set_back_light, CMD_SET_BACK_LIGHT),
	ADD_COMMAND(cmd_read_keypad, CMD_READ_KEYPAD),
	ADD_COMMAND(cmd_conf_key_report, CMD_CONF_KEY_REPORT),
	ADD_COMMAND(cmd_read_dow, CMD_READ_DOW),

	ADD_COMMAND_VARIADIC_SIZE(9,   9, cmd_set_special_character, CMD_SET_SPECIAL_CHAR),
	ADD_COMMAND_VARIADIC_SIZE(0,   255, cmd_udocumented_0x08, CMD_UNDOCUMENTED_0x08),
	ADD_COMMAND_VARIADIC_SIZE(16,  16, cmd_read_user_flash_data, CMD_READ_USER_FLASH_DATA),
	ADD_COMMAND_VARIADIC_SIZE(3,   3, cmd_reset, CMD_RESET),
	ADD_COMMAND_VARIADIC_SIZE(4,   4, cmd_return_ok, CMD_SET_FAN),
	ADD_COMMAND_VARIADIC_SIZE(16,  16, cmd_return_ok, CMD_WRITE_USER_FLASH_DATA),
	ADD_COMMAND_VARIADIC_SIZE(0,  255, cmd_return_ok, CMD_CONF_GPIO),

};

int gen_commands_total = sizeof(gen_commands)/sizeof(gen_commands[0]);

/**
 * System statistics
 */
cmd_stat_t cmd_stat;


static int shift_rx_buffer(uint8_t *rx_buffer, int size)
{
	size--;
	memmove(&rx_buffer[0], &rx_buffer[1], size);

	return size;
}

//This code is from the IRDA LAP documentation, which appears to
//have been copied from PPP:
//
// http://irda.affiniscape.com/associations/2494/files/Specifications/
// IrLAP11_Plus_Errata.zip
//
//I doubt that there are any worries about the legality of this code,
//searching for the first line of the table below, it appears that
//the code is already included in the linux 2.6 kernel "Driver for
//ST5481 USB ISDN modem". This is an "industry standard" algorithm
//and I do not think there are ANY issues with it at all.

//CRC lookup table to avoid bit-shifting loops.
static const uint16_t crcLookupTable[256] = {
		0x00000,0x01189,0x02312,0x0329B,0x04624,0x057AD,0x06536,0x074BF,
		0x08C48,0x09DC1,0x0AF5A,0x0BED3,0x0CA6C,0x0DBE5,0x0E97E,0x0F8F7,
		0x01081,0x00108,0x03393,0x0221A,0x056A5,0x0472C,0x075B7,0x0643E,
		0x09CC9,0x08D40,0x0BFDB,0x0AE52,0x0DAED,0x0CB64,0x0F9FF,0x0E876,
		0x02102,0x0308B,0x00210,0x01399,0x06726,0x076AF,0x04434,0x055BD,
		0x0AD4A,0x0BCC3,0x08E58,0x09FD1,0x0EB6E,0x0FAE7,0x0C87C,0x0D9F5,
		0x03183,0x0200A,0x01291,0x00318,0x077A7,0x0662E,0x054B5,0x0453C,
		0x0BDCB,0x0AC42,0x09ED9,0x08F50,0x0FBEF,0x0EA66,0x0D8FD,0x0C974,
		0x04204,0x0538D,0x06116,0x0709F,0x00420,0x015A9,0x02732,0x036BB,
		0x0CE4C,0x0DFC5,0x0ED5E,0x0FCD7,0x08868,0x099E1,0x0AB7A,0x0BAF3,
		0x05285,0x0430C,0x07197,0x0601E,0x014A1,0x00528,0x037B3,0x0263A,
		0x0DECD,0x0CF44,0x0FDDF,0x0EC56,0x098E9,0x08960,0x0BBFB,0x0AA72,
		0x06306,0x0728F,0x04014,0x0519D,0x02522,0x034AB,0x00630,0x017B9,
		0x0EF4E,0x0FEC7,0x0CC5C,0x0DDD5,0x0A96A,0x0B8E3,0x08A78,0x09BF1,
		0x07387,0x0620E,0x05095,0x0411C,0x035A3,0x0242A,0x016B1,0x00738,
		0x0FFCF,0x0EE46,0x0DCDD,0x0CD54,0x0B9EB,0x0A862,0x09AF9,0x08B70,
		0x08408,0x09581,0x0A71A,0x0B693,0x0C22C,0x0D3A5,0x0E13E,0x0F0B7,
		0x00840,0x019C9,0x02B52,0x03ADB,0x04E64,0x05FED,0x06D76,0x07CFF,
		0x09489,0x08500,0x0B79B,0x0A612,0x0D2AD,0x0C324,0x0F1BF,0x0E036,
		0x018C1,0x00948,0x03BD3,0x02A5A,0x05EE5,0x04F6C,0x07DF7,0x06C7E,
		0x0A50A,0x0B483,0x08618,0x09791,0x0E32E,0x0F2A7,0x0C03C,0x0D1B5,
		0x02942,0x038CB,0x00A50,0x01BD9,0x06F66,0x07EEF,0x04C74,0x05DFD,
		0x0B58B,0x0A402,0x09699,0x08710,0x0F3AF,0x0E226,0x0D0BD,0x0C134,
		0x039C3,0x0284A,0x01AD1,0x00B58,0x07FE7,0x06E6E,0x05CF5,0x04D7C,
		0x0C60C,0x0D785,0x0E51E,0x0F497,0x08028,0x091A1,0x0A33A,0x0B2B3,
		0x04A44,0x05BCD,0x06956,0x078DF,0x00C60,0x01DE9,0x02F72,0x03EFB,
		0x0D68D,0x0C704,0x0F59F,0x0E416,0x090A9,0x08120,0x0B3BB,0x0A232,
		0x05AC5,0x04B4C,0x079D7,0x0685E,0x01CE1,0x00D68,0x03FF3,0x02E7A,
		0x0E70E,0x0F687,0x0C41C,0x0D595,0x0A12A,0x0B0A3,0x08238,0x093B1,
		0x06B46,0x07ACF,0x04854,0x059DD,0x02D62,0x03CEB,0x00E70,0x01FF9,
		0x0F78F,0x0E606,0x0D49D,0x0C514,0x0B1AB,0x0A022,0x092B9,0x08330,
		0x07BC7,0x06A4E,0x058D5,0x0495C,0x03DE3,0x02C6A,0x01EF1,0x00F78
};

static uint16_t calculate_checksum(uint8_t *bufptr,uint16_t len)
{
	register uint16_t newCrc;
	newCrc=0xFFFF;

	//This algorithm is based on the IrDA LAP example.
	while(len--)
	newCrc = (newCrc >> 8) ^ crcLookupTable[(newCrc ^ *bufptr++) & 0xff];

	//Make this crc match the one’s complement that is sent in the packet.
	return(~newCrc);
}

static unsigned char checksum(uint8_t *buffer, int size)
{
	uint16_t sum;
	uint16_t cs;


	sum = calculate_checksum(buffer, size);
	cs = ((uint16_t)buffer[size] << 0) |
			((uint16_t)buffer[size + 1] << 8);

	return (sum == cs);
}

/**
 * I am going to reset this variable after I get first command
 */
static char first_command = 1;

int cmd_send_report(uint8_t *buffer, int size)
{
	uint16_t cs;

	buffer[PAYLOAD_SIZE_OFFSET] = size - HEADER_SIZE;

	cs = calculate_checksum(buffer, size);
	buffer[size] = (cs >> 0) & 0xFF;
	buffer[size + 1] = (cs >> 8) & 0xFF;

	size += CS_SIZE;

	USB_VCOMwrite(size, buffer);

	return 0;
}


int cmd_send_slave(uint8_t *buffer, int size)
{
	uint16_t cs;
	
	buffer[PAYLOAD_SIZE_OFFSET] = size - HEADER_SIZE;

	// in all responses set normal response
	buffer[COMMAND_ID_OFFSET] |= 0x40;;

	cs = calculate_checksum(buffer, size);
	buffer[size] = (cs >> 0) & 0xFF;
	buffer[size + 1] = (cs >> 8) & 0xFF;

	size += CS_SIZE;

	USB_VCOMwrite(size, buffer);

	return 0;
}

/**
 * Find command with specified command id
 * Return reference to command if found, 0 if fails
 */
static const cmd_t *find_command(unsigned char command_id)
{
	const cmd_t *cmd;
	int i;

	for (i = 0; i < gen_commands_total; i++)
	{
		cmd = &gen_commands[i];
		if (cmd->command_id == command_id)
		{
			return cmd;
		}
	}

	return 0;
}

#define PRINT_COMMANDS 0
static inline void cmd_print_commands(int command_id)
{
#if (PRINT_COMMANDS == 1)
	static int x = 0;
	static int y = 0;
	if ((x < 200) && (y < 40))
		RenderNumber(x, y, command_id);
	x += 20;
	if (x > 200)
	{
		x = 0;
		y += 20;
	}
#endif
}

int process_command(uint8_t *rx_buffer, unsigned int *rx_buffer_size)
{
	unsigned char ret = 0;
	unsigned char res;
	unsigned int size;
	const cmd_t *cmd;
	int expected_size, command_size;
	unsigned char command_id;


	cmd_stat.process_command++;

	size = *rx_buffer_size;
	while (1)
	{
		// check that the command is well formed

		if (size < RAW_CMD_SIZE) // I did not get all bytes yet
			break;

		command_id = rx_buffer[COMMAND_ID_OFFSET];
		cmd = find_command(command_id);
		cmd_print_commands(command_id);
		if (cmd == 0)
		{
			cmd_stat.process_command_bad_id++;
			size = shift_rx_buffer(rx_buffer, size);
			continue;
		}

		command_size = rx_buffer[PAYLOAD_SIZE_OFFSET];
		if (cmd->size_max < command_size)
		{
			cmd_stat.process_command_bad_size_min++;
			size = shift_rx_buffer(rx_buffer, size);
			continue;
		}

		if (cmd->size_min > command_size)
		{
			cmd_stat.process_command_bad_size_max++;
			size = shift_rx_buffer(rx_buffer, size);
			continue;
		}
		expected_size = command_size;

		if (size < expected_size+RAW_CMD_SIZE)
		{
			cmd_stat.process_command_not_full++;
			break;
		}

		res = checksum(rx_buffer, command_size+HEADER_SIZE);
		if (!res)
		{
			cmd_stat.process_command_bad_cs++;
			size = shift_rx_buffer(rx_buffer, size);
			continue;
		}

		cmd_stat.process_command_handler++;
		// This is a legal command
		cmd->handler(rx_buffer, expected_size+HEADER_SIZE);
		cmd_stat.process_command_handler_done++;

		size = 0;
		first_command = 0;
		ret = 1;

		break;
	}

	*rx_buffer_size = size;
	return ret;
}

CMD_DECLARE_FUNCTION(cmd_ping)
{
	cmd_send_slave(buffer, size);

	return 0;
}

CMD_DECLARE_FUNCTION(cmd_set_special_character)
{
	unsigned char *cmd    = &buffer[PAYLOAD_OFFSET];
	int char_idx = cmd[0];
	uint8_t * bitmap = &cmd[1];
	int i;

	cmd_send_slave(buffer, size - 9);

	for ( i = 0 ; i < 8 ; i++)
	{
		Font_Cyrillic_08x08[char_idx][i] = bitmap[i];
	}


	return 0;
}


CMD_DECLARE_FUNCTION(cmd_set_lcd_cursor_position)
{
	extern void main_move_cursor(int x, int y);
	cmd_set_lcd_cursor_position_rq_t *rq = (cmd_set_lcd_cursor_position_rq_t *)buffer;

	main_move_cursor(rq->column * PIXELS_IN_COLUMN, rq->row * PIXELS_IN_ROM);
	size = HEADER_SIZE;
	cmd_send_slave(buffer, size);

	return 0;
}

CMD_DECLARE_FUNCTION(cmd_set_lcd_cursor_style)
{
	extern void main_set_cursor_mode(int mode);
	cmd_set_lcd_cursor_style_rq_t *rq = (cmd_set_lcd_cursor_style_rq_t *)buffer;

	if (rq->style == 0)
	{
		main_set_cursor_mode(0);
	}
	else
	{
		main_set_cursor_mode(1);
	}
	size = HEADER_SIZE;
	cmd_send_slave(buffer, size);
	return 0;
}

CMD_DECLARE_FUNCTION(cmd_set_lcd_contrast)
{
	size = HEADER_SIZE;
	cmd_send_slave(buffer, size);
	return 0;
}

CMD_DECLARE_FUNCTION(cmd_set_back_light)
{
	size = HEADER_SIZE;
	cmd_send_slave(buffer, size);
	return 0;
}


CMD_DECLARE_FUNCTION(cmd_clear_lcd_screen)
{
	size = HEADER_SIZE;
	cmd_send_slave(buffer, size);
#if (PRINT_COMMANDS != 1)
	LCDClear();
#endif
	return 0;
}


CMD_DECLARE_FUNCTION(cmd_send_data_to_lcd)
{
	char text[32];
	int len;
	int col, row;
	cmd_send_data_to_lcd_rq_t *rq = (cmd_send_data_to_lcd_rq_t *)buffer;

	len = rq->hdr.size - 2;
	memcpy(text, rq->text, len);

	col = rq->column * PIXELS_IN_COLUMN;
	row = rq->row * PIXELS_IN_ROM;
	size = HEADER_SIZE;
	cmd_send_slave(buffer, size);

	text[len] = 0;
#if (PRINT_COMMANDS != 1)
	RenderString90(col, row, text);
#endif

	return 0;
}

CMD_DECLARE_FUNCTION(cmd_return_ok)
{
	size = HEADER_SIZE;
	cmd_send_slave(buffer, size);
	return 0;
}

#define KP_UP		0x01
#define KP_ENTER	0x02
#define KP_CANCEL	0x04
#define KP_LEFT		0x08
#define KP_RIGHT	0x10
#define KP_DOWN		0x20

CMD_DECLARE_FUNCTION(cmd_conf_key_report)
{
	size = HEADER_SIZE;
	cmd_send_slave(buffer, size);
	return 0;
}

inline uint16_t TRANSLATE_BIT(uint16_t val, uint16_t b1, uint16_t b2)
{
	if (val & b1)
		return b2;
	return 0;
}

#define TB(b1, b2) (TRANSLATE_BIT(main_key_state_current, b1, b2))

CMD_DECLARE_FUNCTION(cmd_read_keypad)
{
	cmd_read_keypad_rs_t *rs = (cmd_read_keypad_rs_t *)buffer;
	extern uint16_t main_key_state_current;
	uint16_t key_state_current =
			TB(KEY_UP_PIN, KP_UP) |
			TB(KEY_DOWN_PIN, KP_DOWN) |
			TB(KEY_LEFT_PIN, KP_LEFT) |
			TB(KEY_RIGHT_PIN, KP_RIGHT) |
			TB(KEY_ENTER_PIN, KP_ENTER) |
			TB(KEY_CANCEL_PIN, KP_CANCEL);
	static uint16_t key_state_last;

	rs->pressed = key_state_current;
	rs->pressed_from_last_poll = (~key_state_last) & key_state_current;
	rs->released_from_last_poll = key_state_last & (~key_state_current);
	key_state_last = key_state_current;
	size = sizeof(cmd_read_keypad_rs_t);
	cmd_send_slave(buffer, size);

	return 0;
}

CMD_DECLARE_FUNCTION(cmd_read_dow)
{
	cmd_read_dow_rs_t *rs = (cmd_read_dow_rs_t *)buffer;
	cmd_read_dow_rq_t *rq = (cmd_read_dow_rq_t *)buffer;
	unsigned char device_index;

	device_index = rq->device_index;
	memset(rs->rom_id, 0, sizeof(rs->rom_id));
	rs->device_index = device_index;

	size = sizeof(cmd_read_keypad_rs_t);
	cmd_send_slave(buffer, size);

	return 0;
}

CMD_DECLARE_FUNCTION(cmd_udocumented_0x08)
{
	size = HEADER_SIZE;
	cmd_send_slave(buffer, size);
	return 0;
}

CMD_DECLARE_FUNCTION(cmd_read_user_flash_data)
{
	size = HEADER_SIZE+16;
	memset(&buffer[PAYLOAD_OFFSET], 0, 16);
	cmd_send_slave(buffer, size);

	return 0;
}

CMD_DECLARE_FUNCTION(cmd_reset)
{
	size = HEADER_SIZE;
	cmd_send_slave(buffer, size);
	return 0;
}


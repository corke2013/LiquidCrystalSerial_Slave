#ifndef LiquidCrystalSerial_Slave_h
#define LiquidCrystalSerial_Slave_h

#if (ARDUINO <  100) && !defined(MPIDE)
#error LiquidCrystalSerial library requires Arduino 1.0 or later
#endif

#include <hd44780.h>
#include <SoftwareSerial.h>

//commands
#define LCD_PAYLOAD_START 0x01
#define LCD_BEGIN 0x80
#define LCD_CLEAR 0x81
#define LCD_HOME 0x82
#define LCD_SET_CURSOR 0x83
#define LCD_WRITE 0x84
#define LCD_PRINT 0x85
#define LCD_CURSOR 0x86
#define LCD_NO_CURSOR 0x87
#define LCD_BLINK 0x88
#define LCD_NO_BLINK 0x89
#define LCD_DISPLAY 0x8A
#define LCD_NO_DISPLAY 0x8B
#define LCD_SCROLL_DISPLAY_LEFT 0x8C
#define LCD_SCROLL_DISPLAY_RIGHT 0x8D
#define LCD_AUTOSCROLL 0x8E
#define LCD_NO_AUTOSCROLL 0x8F
#define LCD_LEFT_TO_RIGHT 0x90
#define LCD_RIGHT_TO_LEFT 0x91
#define LCD_CREATE_CHAR 0x92
#define LCD_SET_ROW_OFFSETS 0x93
#define LCD_COMMAND 0x94
#define LCD_BACKLIGHT 0x95
#define LCD_NO_BACKLIGHT 0x96
#define LCD_LINE_WRAP 0x97
#define LCD_NO_LINE_WRAP 0x98
#define LCD_MOVE_CURSOR_LEFT 0x99
#define LCD_MOVE_CURSOR_RIGHT 0x9A
#define LCD_SET_EXEC_TIMES 0x9B
#define LCD_ON 0x9C
#define LCD_OFF 0x9D

class LiquidCrystalSerial_Slave
{
  public:
    LiquidCrystalSerial_Slave(SoftwareSerial* serial, hd44780* lcd);
	void begin(unsigned long speed);
	void run();
  private:
	SoftwareSerial* lcdSerial;
	hd44780* lcd;
	byte runCmd(byte cmd, byte* buff, byte nBytes);
	struct lcdColRowStruct {
		byte col;
		byte row;
	};
	struct lcdCreateCharStruct {
		byte charval;
		byte charmap[8];
	};
	struct lcdSet2RowOffsetsStruct {
		byte row1;
		byte row2;
	};
	struct lcdSet3RowOffsetsStruct {
		byte row1;
		byte row2;
		byte row3;
	};
	struct lcdSet4RowOffsetsStruct {
		byte row1;
		byte row2;
		byte row3;
		byte row4;
	};
	struct lcdSetExecTimesStruct {
		uint16_t chUs;
		uint16_t insU;
	};
};

#endif

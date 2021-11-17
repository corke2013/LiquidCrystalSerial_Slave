//standard headers
#include <Wire.h>
#include <SoftwareSerial.h>

//custom headers
#include <hd44780.h>
#include <hd44780ioClass/hd44780_I2Cexp.h>

//buffers
#define COMMAND_SIZE 1
#define BUFFER_SIZE 80

//commands
#define LCD_PAYLOAD_START 0x02
#define LCD_PAYLOAD_END 0x03
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
#define LCD_ON 0x9B
#define LCD_OFF 0x9C

//strucs
struct lcdColRow {
  uint8_t col;
  uint8_t row;
};

struct lcdCreateChar {
  uint8_t charval;
  uint8_t charmap[8];
};

struct lcdSet2RowOffsets {
  uint8_t row1;
  uint8_t row2;
};

struct lcdSet3RowOffsets {
  uint8_t row1;
  uint8_t row2;
  uint8_t row3;
};

struct lcdSet4RowOffsets {
  uint8_t row1;
  uint8_t row2;
  uint8_t row3;
  uint8_t row4;
};

SoftwareSerial lcdSerial (3, 4);
hd44780_I2Cexp lcd;

void setup() {
  lcdSerial.begin(9600);
}

void loop() {
  if (!lcdSerial.isListening()) {
    lcdSerial.listen();
  }
  if (lcdSerial.find(LCD_PAYLOAD_START)) {
    byte cmd[COMMAND_SIZE];
    if (!lcdSerial.readBytes(cmd, COMMAND_SIZE)) {
      return;
    }
    byte buff[BUFFER_SIZE];
    uint8_t nBytes = lcdSerial.readBytesUntil(LCD_PAYLOAD_END, buff, BUFFER_SIZE);
    if (nBytes < 0) {
      return;
    }
    lcdRunCommand(*cmd, buff, nBytes);
  }
}

void lcdRunCommand(byte cmd, byte* buff, uint8_t nBytes) {
  uint8_t result;
  lcdColRow lcr;
  lcdCreateChar lcc;
  lcdSet2RowOffsets ls2ro;
  lcdSet3RowOffsets ls3ro;
  lcdSet4RowOffsets ls4ro;
  switch (cmd) {
    case LCD_BEGIN:
      lcr = *(lcdColRow*) buff;
      result = lcd.begin(lcr.col, lcr.row);
      break;
    case LCD_CLEAR:
      result = lcd.clear();
      break;
    case LCD_HOME:
      result = lcd.home();
      break;
    case LCD_SET_CURSOR:
      lcr = *(lcdColRow*) buff;
      result = lcd.setCursor(lcr.col, lcr.row);
      break;
    case LCD_WRITE:
      if (nBytes == sizeof(byte)) {
        lcd.write(*buff);
      }
      else if (buff[nBytes - 1] == 0x00) {
        lcd.write((char*)buff);
      }
      else {
        lcd.write(buff, nBytes);
      }
      break;
    case LCD_PRINT:
      lcd.print((char*)buff);
      break;
    case LCD_CURSOR:
      result = lcd.cursor();
      break;
    case LCD_NO_CURSOR:
      result = lcd.noCursor();
      break;
    case LCD_BLINK:
      result = lcd.blink();
      break;
    case LCD_NO_BLINK:
      result = lcd.noBlink();
      break;
    case LCD_DISPLAY:
      result = lcd.display();
      break;
    case LCD_NO_DISPLAY:
      result = lcd.noDisplay();
      break;
    case LCD_SCROLL_DISPLAY_LEFT:
      result = lcd.scrollDisplayLeft();
      break;
    case LCD_SCROLL_DISPLAY_RIGHT:
      result = lcd.scrollDisplayRight();
      break;
    case LCD_AUTOSCROLL:
      result = lcd.autoscroll();
      break;
    case LCD_NO_AUTOSCROLL:
      result = lcd.noAutoscroll();
      break;
    case LCD_LEFT_TO_RIGHT:
      result = lcd.leftToRight();
      break;
    case LCD_RIGHT_TO_LEFT:
      result = lcd.rightToLeft();
      break;
    case LCD_CREATE_CHAR:
      lcc = *(lcdCreateChar*) buff;
      result = lcd.createChar(lcc.charval, lcc.charmap);
      break;
    case LCD_SET_ROW_OFFSETS:
      if (nBytes == sizeof(byte)) {
        result = lcd.setRowOffsets(*buff);
      }
      else if (nBytes == sizeof(ls2ro)) {
        ls2ro = *(lcdSet2RowOffsets*) buff;
        result = lcd.setRowOffsets(ls2ro.row1, ls2ro.row2);
      }
      else if (nBytes == sizeof(ls3ro)) {
        ls3ro = *(lcdSet3RowOffsets*) buff;
        result = lcd.setRowOffsets(ls3ro.row1, ls3ro.row2, ls3ro.row3);
      }
      else if (nBytes == sizeof(ls4ro)) {
        ls4ro = *(lcdSet4RowOffsets*) buff;
        result = lcd.setRowOffsets(ls4ro.row1, ls4ro.row2, ls4ro.row3, ls4ro.row4);
      }
      break;
    case LCD_COMMAND:
      if (nBytes == sizeof(byte)) {
        result = lcd.command(*buff);
      }
      break;
    case LCD_BACKLIGHT:
      result = lcd.backlight();
      break;
    case LCD_NO_BACKLIGHT:
      result = lcd.noBacklight();
      break;
    case LCD_LINE_WRAP:
      result = lcd.lineWrap();
      break;
    case LCD_NO_LINE_WRAP:
      result = lcd.noLineWrap();
      break;
    case LCD_MOVE_CURSOR_LEFT:
      result = lcd.moveCursorLeft();
      break;
    case LCD_MOVE_CURSOR_RIGHT:
      result = lcd.moveCursorRight();
      break;
    case LCD_ON:
      result = lcd.on();
      break;
    case LCD_OFF:
      result = lcd.off();
      break;
    default:
      break;
  }
  lcdSerial.print(result);
}

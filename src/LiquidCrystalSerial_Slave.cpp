#include <LiquidCrystalSerial_Slave.h>

LiquidCrystalSerial_Slave::LiquidCrystalSerial_Slave(SoftwareSerial* lcdSerial, hd44780* lcd){
	LiquidCrystalSerial_Slave::lcdSerial = lcdSerial;
	LiquidCrystalSerial_Slave::lcd = lcd;
}

void LiquidCrystalSerial_Slave::begin(unsigned long speed){
	LiquidCrystalSerial_Slave::lcdSerial->begin(speed);
}

void LiquidCrystalSerial_Slave::run(){
	if (!LiquidCrystalSerial_Slave::lcdSerial->isListening()) {
		LiquidCrystalSerial_Slave::lcdSerial->listen();
	}
	if (LiquidCrystalSerial_Slave::lcdSerial->find(LCD_PAYLOAD_START)) {
		byte cmd[COMMAND_SIZE];
		if (!LiquidCrystalSerial_Slave::lcdSerial->readBytes(cmd, COMMAND_SIZE)) {
			return;
		}
		byte buff[BUFFER_SIZE];
		uint8_t nBytes = LiquidCrystalSerial_Slave::lcdSerial->readBytesUntil(LCD_PAYLOAD_END, buff, BUFFER_SIZE);
		if (nBytes < 0) {
			return;
		}
		LiquidCrystalSerial_Slave::lcdSerial->write(runCmd(*cmd, buff, nBytes));
	}
}

uint8_t LiquidCrystalSerial_Slave::runCmd(byte cmd, byte* buff, uint8_t nBytes)
{
	uint8_t result = 0;
	lcdColRow lcr;
	lcdCreateChar lcc;
	lcdSet2RowOffsets ls2ro;
	lcdSet3RowOffsets ls3ro;
	lcdSet4RowOffsets ls4ro;
	switch (cmd) {
		case LCD_BEGIN:
			lcr = *(lcdColRow*) buff;
			result = LiquidCrystalSerial_Slave::lcd->begin(lcr.col, lcr.row);
			break;
		case LCD_CLEAR:
			result = LiquidCrystalSerial_Slave::lcd->clear();
			break;
		case LCD_HOME:
			result = LiquidCrystalSerial_Slave::lcd->home();
			break;
		case LCD_SET_CURSOR:
			lcr = *(lcdColRow*) buff;
			result = LiquidCrystalSerial_Slave::lcd->setCursor(lcr.col, lcr.row);
			break;
		case LCD_WRITE:
			if (nBytes == sizeof(byte)) {
				LiquidCrystalSerial_Slave::lcd->write(*buff);
			}
			else if (buff[nBytes - 1] == 0x00) {
				LiquidCrystalSerial_Slave::lcd->write((char*)buff);
			}
			else {
				LiquidCrystalSerial_Slave::lcd->write(buff, nBytes);
			}
			break;
		case LCD_PRINT:
			LiquidCrystalSerial_Slave::lcd->print((char*)buff);
			break;
		case LCD_CURSOR:
			result = LiquidCrystalSerial_Slave::lcd->cursor();
			break;
		case LCD_NO_CURSOR:
			result = LiquidCrystalSerial_Slave::lcd->noCursor();
			break;
		case LCD_BLINK:
			result = LiquidCrystalSerial_Slave::lcd->blink();
			break;
		case LCD_NO_BLINK:
			result = LiquidCrystalSerial_Slave::lcd->noBlink();
			break;
		case LCD_DISPLAY:
			result = LiquidCrystalSerial_Slave::lcd->display();
			break;
		case LCD_NO_DISPLAY:
			result = LiquidCrystalSerial_Slave::lcd->noDisplay();
			break;
		case LCD_SCROLL_DISPLAY_LEFT:
			result = LiquidCrystalSerial_Slave::lcd->scrollDisplayLeft();
			break;
		case LCD_SCROLL_DISPLAY_RIGHT:
			result = LiquidCrystalSerial_Slave::lcd->scrollDisplayRight();
			break;
		case LCD_AUTOSCROLL:
			result = LiquidCrystalSerial_Slave::lcd->autoscroll();
			break;
		case LCD_NO_AUTOSCROLL:
			result = LiquidCrystalSerial_Slave::lcd->noAutoscroll();
			break;
		case LCD_LEFT_TO_RIGHT:
			result = LiquidCrystalSerial_Slave::lcd->leftToRight();
			break;
		case LCD_RIGHT_TO_LEFT:
			result = LiquidCrystalSerial_Slave::lcd->rightToLeft();
			break;
		case LCD_CREATE_CHAR:
			lcc = *(lcdCreateChar*) buff;
			result = LiquidCrystalSerial_Slave::lcd->createChar(lcc.charval, lcc.charmap);
			break;
		case LCD_SET_ROW_OFFSETS:
			if (nBytes == sizeof(byte)) {
				result = LiquidCrystalSerial_Slave::lcd->setRowOffsets(*buff);
			}
			else if (nBytes == sizeof(ls2ro)) {
				ls2ro = *(lcdSet2RowOffsets*) buff;
				result = LiquidCrystalSerial_Slave::lcd->setRowOffsets(ls2ro.row1, ls2ro.row2);
			}
			else if (nBytes == sizeof(ls3ro)) {
				ls3ro = *(lcdSet3RowOffsets*) buff;
				result = LiquidCrystalSerial_Slave::lcd->setRowOffsets(ls3ro.row1, ls3ro.row2, ls3ro.row3);
			}
			else if (nBytes == sizeof(ls4ro)) {
				ls4ro = *(lcdSet4RowOffsets*) buff;
				result = LiquidCrystalSerial_Slave::lcd->setRowOffsets(ls4ro.row1, ls4ro.row2, ls4ro.row3, ls4ro.row4);
			}
			break;
		case LCD_COMMAND:
			if (nBytes == sizeof(byte)) {
				result = LiquidCrystalSerial_Slave::lcd->command(*buff);
			}
			break;
		case LCD_BACKLIGHT:
			result = LiquidCrystalSerial_Slave::lcd->backlight();
			break;
		case LCD_NO_BACKLIGHT:
			result = LiquidCrystalSerial_Slave::lcd->noBacklight();
			break;
		case LCD_LINE_WRAP:
			result = LiquidCrystalSerial_Slave::lcd->lineWrap();
			break;
		case LCD_NO_LINE_WRAP:
			result = LiquidCrystalSerial_Slave::lcd->noLineWrap();
			break;
		case LCD_MOVE_CURSOR_LEFT:
			result = LiquidCrystalSerial_Slave::lcd->moveCursorLeft();
			break;
		case LCD_MOVE_CURSOR_RIGHT:
			result = LiquidCrystalSerial_Slave::lcd->moveCursorRight();
			break;
		case LCD_ON:
			result = LiquidCrystalSerial_Slave::lcd->on();
			break;
		case LCD_OFF:
			result = LiquidCrystalSerial_Slave::lcd->off();
			break;
		default:
			break;
	}
	return result;
}

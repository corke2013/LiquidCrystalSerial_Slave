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
		byte command;
		if (sizeof(command) != LiquidCrystalSerial_Slave::lcdSerial->readBytes(&command, sizeof(command))) return;
		byte nBytesPayload;
		if (sizeof(nBytesPayload) != LiquidCrystalSerial_Slave::lcdSerial->readBytes(&nBytesPayload, sizeof(nBytesPayload))) return;
		byte payload[nBytesPayload];
		if (nBytesPayload != LiquidCrystalSerial_Slave::lcdSerial->readBytes(payload, nBytesPayload)) return;
		LiquidCrystalSerial_Slave::lcdSerial->write(runCmd(command, payload, nBytesPayload));
	}
}

byte LiquidCrystalSerial_Slave::runCmd(byte command, byte* payload, byte nBytesPayload)
{
	byte result = 0;
	lcdColRowStruct lcr;
	lcdCreateCharStruct lcc;
	lcdSet2RowOffsetsStruct ls2ro;
	lcdSet3RowOffsetsStruct ls3ro;
	lcdSet4RowOffsetsStruct ls4ro;
	lcdSetExecTimesStruct let;
	switch (command) {
		case LCD_BEGIN:
			lcr = *(lcdColRowStruct*) payload;
			result = LiquidCrystalSerial_Slave::lcd->begin(lcr.col, lcr.row);
			break;
		case LCD_CLEAR:
			result = LiquidCrystalSerial_Slave::lcd->clear();
			break;
		case LCD_HOME:
			result = LiquidCrystalSerial_Slave::lcd->home();
			break;
		case LCD_SET_CURSOR:
			lcr = *(lcdColRowStruct*) payload;
			result = LiquidCrystalSerial_Slave::lcd->setCursor(lcr.col, lcr.row);
			break;
		case LCD_WRITE:
			if (nBytesPayload == sizeof(byte)) {
				LiquidCrystalSerial_Slave::lcd->write(*payload);
			}
			else if (payload[nBytesPayload - 1] == 0x00) {
				LiquidCrystalSerial_Slave::lcd->write((char*)payload);
			}
			else {
				LiquidCrystalSerial_Slave::lcd->write(payload, nBytesPayload);
			}
			break;
		case LCD_PRINT:
			LiquidCrystalSerial_Slave::lcd->print((char*)payload);
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
			lcc = *(lcdCreateCharStruct*) payload;
			result = LiquidCrystalSerial_Slave::lcd->createChar(lcc.charval, lcc.charmap);
			break;
		case LCD_SET_ROW_OFFSETS:
			if (nBytesPayload == sizeof(byte)) {
				result = LiquidCrystalSerial_Slave::lcd->setRowOffsets(*payload);
			}
			else if (nBytesPayload == sizeof(ls2ro)) {
				ls2ro = *(lcdSet2RowOffsetsStruct*) payload;
				result = LiquidCrystalSerial_Slave::lcd->setRowOffsets(ls2ro.row1, ls2ro.row2);
			}
			else if (nBytesPayload == sizeof(ls3ro)) {
				ls3ro = *(lcdSet3RowOffsetsStruct*) payload;
				result = LiquidCrystalSerial_Slave::lcd->setRowOffsets(ls3ro.row1, ls3ro.row2, ls3ro.row3);
			}
			else if (nBytesPayload == sizeof(ls4ro)) {
				ls4ro = *(lcdSet4RowOffsetsStruct*) payload;
				result = LiquidCrystalSerial_Slave::lcd->setRowOffsets(ls4ro.row1, ls4ro.row2, ls4ro.row3, ls4ro.row4);
			}
			break;
		case LCD_COMMAND:
			if (nBytesPayload == sizeof(byte)) {
				result = LiquidCrystalSerial_Slave::lcd->command(*payload);
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
		case LCD_SET_EXEC_TIMES:
			let = *(lcdSetExecTimesStruct*) payload;
			LiquidCrystalSerial_Slave::lcd->setExecTimes(let.chUs, let.insU);
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

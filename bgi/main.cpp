#include "graphics.h"
#include "z80_cpu.hpp"


int main(int argc, char *argv[])
{	
	
	initwindow(LCD_WIDTH * 2 + 2 * SCREEN_OFFSET + DEBUG_WINDOW_WIDTH, LCD_HEIGHT * 2 + 2 * SCREEN_OFFSET, "gameboy emulator");
	DMGZ80CPU cpu;
	cpu.readROM("tetris.gb");
	setbkcolor(WHITE);
	cleardevice();
	cpu.run();
	return 0;
	


}

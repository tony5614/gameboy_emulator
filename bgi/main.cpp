#include "graphics.h"
#include "z80_cpu.hpp"


int main(int argc, char *argv[])
{	
	
	initwindow(560, 256, "gameboy_emulator", 10 ,20);
	DMGZ80CPU cpu;
	setbkcolor(WHITE);
	cleardevice();
	cpu.readROM("mario.gb");
	cpu.run();
	return 0;
	


}

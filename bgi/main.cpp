#include "graphics.h"
#include "z80_cpu.hpp"


int main(int argc, char *argv[])
{


	initwindow(LCD_WIDTH * 2 + 2 * SCREEN_OFFSET + DEBUG_WINDOW_WIDTH, LCD_HEIGHT * 2 + 2 * SCREEN_OFFSET, "gameboy emulator");


	//int color;
	//color = getmaxcolor() / 2;
	//for(int i = 0 ; i < 30 ; i++)
	//	putpixel(i, i, COLOR(255, 0, 0));



	DMGZ80CPU cpu;
	cpu.readROM("04-op r,imm.gb");

	cpu.run();

	/*
	std::ifstream fin;
	fin.open("videoram.bin", std::ios::ate | std::ios::binary);
	int bin_size = fin.tellg();
	fin.seekg(0, fin.beg);
	U8 one_byte;
	for (int i = 0; i < bin_size; i++) 
	{
		fin.read((char*)&one_byte, sizeof(one_byte));
		cpu.memory[VIDEO_RAM_BASE + i] = one_byte;
	}
	cpu.memory[0xFF40] = 0xD3;
	cpu.debugPPU();
	getchar();
	*/
	/*







	*/



	return 0;
}

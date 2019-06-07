#include "graphics.h"
#include "z80_cpu.hpp"


int main(int argc, char *argv[])
{	
	
	
	initwindow(560 * SCALE_SIZE, 256 * SCALE_SIZE, "gameboy_emulator", 20 ,400);
	DMGZ80CPU cpu;
	//setbkcolor(WHITE);
	cleardevice();
	cpu.readROM("mario.gb");
	cpu.run();
	return 0;
	

	
	/*
	initwindow(560, 256, "gameboy_emulator", 10, 20);

	setbkcolor(COLOR(100, 100, 100));
	cleardevice();

	bar(0, 0, 20, 20);
	setfillstyle(1, YELLOW);


	void *tile_buf_ptr = malloc(imagesize(0, 0, 0, 0));
	putpixel(0, 0, RED);
	getimage(0, 0, 0, 0, tile_buf_ptr);
	printf("size = %d\n", imagesize(0, 0, 0, 0));
	printf("size = %d\n", imagesize(0, 0, 1, 1));
	printf("size = %d\n", imagesize(0, 0, 2, 2));
	printf("size = %d\n", imagesize(0, 0, 3, 3));

	putimage(10, 10, tile_buf_ptr, COPY_PUT);
	*/

	getchar();

}



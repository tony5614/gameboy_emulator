#pragma once
#include <stdio.h>
#include <windows.h>
#include <stdlib.h>d
#include <iostream>
#include <fstream>
#include <iomanip>


typedef unsigned char  U8;
typedef unsigned short U16;

#define BIT(x)                 (0x1 << (x))

#define CARTRIDGE_ROM_BASE     (0x0000)
#define VIDEO_RAM_BASE         (0x8000)
#define BG_MAP_ADDR            (0x9800)
#define EXTERNAL_RAM_BASE      (0xA000)
#define WORK_RAM_BASE          (0xC000)
#define OAM_BASE               (0xFE00)
#define PERIPHERAL_BASE        (0xFF00)
#define JOY_PAD                (0xFF00)
#define 	JOY_PAD_RESET          (BIT(4)|BIT(5))
#define 	JOY_PAD_SEL_BUTTON     BIT(4)
#define 	JOY_PAD_SEL_DIRECT     BIT(5)
#define TIMER_ADDR             (0xFF06)	//TMA timer
#define TIMER_CTRL_ADDR        (0xFF07) //TAC timer control
#define LCD_CTRL_REG           (0xFF40) //LCDC
#define 	BG_CODE_SEL_FALG       BIT(3)   //BG Code Area Selection Flag
#define LCD_Y_COORD_REG        (0xFF44) //LY lcd control y coordinate
#define BG_PALETTE_DATA        (0xFF47) //BGP
#define INT_FLAGS_ADDR         (0xFF0F)
#define HIGH_WORK_RAM_BASE     (0xFF80)
#define STACK_BEGIN_ADDR       (0xFFFE)
#define INT_SWITCH_BASE        (0xFFFF)

#define MEMORY_SIZE            (65536)

#define SCREEN_OFFSET          (10)
#define RESOLUTION_X           (160)
#define RESOLUTION_Y           (144)
#define DEBUG_WINDOW_WIDTH     (200)
#define BG_TILE_BUFFER_X       (256)
#define BG_TILE_BUFFER_Y       (0)

union AF
{
	U16 all;
	struct
	{
		U8 rsv : 4;
		U8 f_c : 1;     //carry flag
		U8 f_h : 1;     //half caffy flag
		U8 f_n : 1;     //substract flag
		U8 f_z : 1;     //zero flag
		U8 a;
	};
};

union BC
{
	U16 all;
	struct
	{
		U8 c;
		U8 b;
	};
};

union DE
{
	U16 all;
	struct
	{
		U8 e;
		U8 d;
	};
};

union HL
{
	U16 all;
	struct
	{
		U8 l;
		U8 h;
	};
};


class U8DATA
{
public:
	U8  value;
	U16 *access_addr;
	U8 _hang;
	U8DATA()
	{
		value = 0;
		_hang = true;
	}
	U8DATA(U8 &val)
	{
		this->value = val;
		_hang = true;

	}
	operator U8()
	{
		if ((0xFF00 <= *this->access_addr) && (*this->access_addr < 0xFF80))
		{
			//printf("cast operator_ @ %04X\n", *access_addr);
			//printf("value = %04X\n", this->value);
		}
		////printf("cast operator @ %04X\n", *access_addr);
		if (*access_addr == 0xFF44)
		{
			//while (_hang);
			//this->value = 0x94;
		}
		else if (*access_addr == 0xFF00)
		{
			if ((this->value & JOY_PAD_RESET) == JOY_PAD_RESET)
			{
				this->value = 0xFF;
			}
			else if (this->value & JOY_PAD_SEL_DIRECT)
			{
				if (GetKeyState(VK_RIGHT) < 0)
					this->value &= (~BIT(0));
				if (GetKeyState(VK_LEFT) < 0)
					this->value &= (~BIT(1));
				if (GetKeyState(VK_UP) < 0)
					this->value &= (~BIT(2));
				if (GetKeyState(VK_DOWN) < 0)
					this->value &= (~BIT(3));
			}
			else if (this->value & JOY_PAD_SEL_BUTTON)
			{
				if (GetKeyState('A') < 0)
					this->value &= (~BIT(0));
				if (GetKeyState('S') < 0)
					this->value &= (~BIT(1));
				if (GetKeyState(VK_SHIFT) < 0)
					this->value &= (~BIT(2));
				if (GetKeyState(VK_RETURN) < 0)
					this->value &= (~BIT(3));
			}
			else
			{
				//printf("unknown joy sel %04X\n", this->value);
				while (_hang);
			}
		}

		return this->value;
	}
	U8* operator &()
	{
		//printf("get address operator @ %04X\n" ,*access_addr);
		if ((0xFF00 <= *this->access_addr) && (*this->access_addr < 0xFF80))
		{
			//printf("get address operator_ @ %04X\n", *access_addr);
			//printf("value = %04X\n", this->value);
		}
		return &this->value;
	}
	U8DATA &operator=(U8 val)
	{
		U8 _hang = true;
		if((0xFF00 <= *this->access_addr) && (*this->access_addr < 0xFF80))
		{
			//printf("assign operator_ @ %04X\n", *access_addr);
			//printf("value = %04X\n", this->value);
		}
		if (*access_addr == 0xFF00)
		{
			//reset joy pad
			this->value = val | 0xCF;
		}
		//normal assignment
		else
		{
			this->value = val;
		}
		return (*this);
	}
};



class DEBUG_MEM
{	
public:
	
	U16 access_addr;
	U8DATA  data[MEMORY_SIZE];	
	//U8  data[MEMORY_SIZE];
	U8  _hang;
	DEBUG_MEM()
	{
		_hang = true;
		access_addr = 0;
	}
	U8DATA &operator[](U16 addr)
	{
		access_addr = addr;
		this->data[addr].access_addr = &this->access_addr;
		return this->data[addr];
	}
};

class DMGZ80CPU
{
private:

	//U8 memory[MEMORY_SIZE];
	AF af;
	BC bc;
	DE de;
	HL hl;
	U16 pc, sp;
	U16 stack[16];
	U8 ime;                            //interrupt master enable
	U8 halt_state;
	void* tile_buf_ptr;
	SYSTEMTIME begin_time;
	SYSTEMTIME end_time;
	unsigned int cpu_cycles;

public:
	DEBUG_MEM memory;
	inline	void printREG() 
	{
		//printf("A:%02X F:%02X\n", af.a, af.all & 0xFF);
		//printf("B:%02X C:%02X\n", bc.b, bc.c);
		//printf("D:%02X E:%02X\n", de.d, de.e);
		//printf("H:%02X L:%02X\n", hl.h, hl.l);
		//printf("PC:%04X SP:%04X\n", pc, sp);
	}
	void printMEM(int base, int length)
	{
		for (int i = base; i < (base + length); i++)
		{
			if ((i & 0xF) == 0)
			{	
				//printf("\n%04X : ", base + i);
			}
			//printf("%02X ", memory[base + i]);
		}
		//printf("\n");
	}
	void readROM(std::string filename)
	{
		std::ifstream fin;
		int rom_size;
		U8  one_byte;
	

		fin.open(filename.c_str(), std::ios::ate | std::ios::binary);
		//get rom size
		rom_size = fin.tellg();
		//move pointer to beginning of rom
		fin.seekg(0, fin.beg);
		for(int i = 0; i < rom_size; i++)
		{
			//fin.read((char *)(&memory[0] + CARTRIDGE_ROM_BASE + sizeof(U8DATA) * i), sizeof(char));
			
			fin.read((char *)(&one_byte), sizeof(one_byte));
			memory[i] = one_byte;
		}
		fin.close();

		printMEM(0, 64);

	}

	inline U16 *idxToRegss(U8 idx) //00b ~ 11b -> BC DE HL SP
	{
		if (idx == 0x0)
			return &bc.all;
		else if (idx == 0x1)
			return &de.all;
		else if (idx == 0x2)
			return &hl.all;
		else if (idx == 0x3)
			return &sp;
	}

	inline U16 *idxToRegdd(U8 idx) //00b ~ 11b -> BC DE HL SP
	{
		if (idx == 0x0)
			return &bc.all;
		else if (idx == 0x1)
			return &de.all;
		else if (idx == 0x2)
			return &hl.all;
		else if (idx == 0x3)
			return &sp;
	}


	inline U8 *idxToRegr_HL(U8 idx) //000b ~ 110b -> A B .. L (HL)
	{
		if (idx == 0x0)
			return &bc.b;
		else if (idx == 0x1)
			return &bc.c;
		else if (idx == 0x2)
			return &de.d;
		else if (idx == 0x3)
			return &de.e;
		else if (idx == 0x4)
			return &hl.h;
		else if (idx == 0x5)
			return &hl.l;
		else if (idx == 0x6)
			return &(memory[hl.all]);
		else if (idx == 0x7)
			return &af.a;
	}

	inline U16 *idxToRegqq(U8 idx) //00b ~ 11b -> BC DE HL SP
	{
		if (idx == 0x0)
			return &bc.all;
		else if (idx == 0x1)
			return &de.all;
		else if (idx == 0x2)
			return &hl.all;
		else if (idx == 0x3)
			return &af.all;
	}


	inline U16 idxToP(U8 idx) //for RST
	{
		if (idx == 0x0)
			return 0x00;
		else if (idx == 0x1)
			return 0x08;
		else if (idx == 0x2)
			return 0x10;
		else if (idx == 0x3)
			return 0x18;
		else if (idx == 0x4)
			return 0x20;
		else if (idx == 0x5)
			return 0x28;
		else if (idx == 0x6)
			return 0x30;
		else if (idx == 0x7)
			return 0x38;

	}

	void displayControl()
	{
		buildBackground();
	}
	DMGZ80CPU()
	{
		//boot rom
		sp = STACK_BEGIN_ADDR;
		pc = 0x0;
		af.all = 0x0000;
		bc.all = 0x0000;
		de.all = 0x0000;
		hl.all = 0x0000;
		
		tile_buf_ptr = malloc(imagesize(0,0,8,8));
		halt_state = FALSE;
		cpu_cycles = 0;

		//load video ram		0x8000 ~ 0x8270
		//sp = 0xCFF9;
		//pc = 0x27C3;
		//af.all = 0x09A0;
		//bc.all = 0x0000;
		//de.all = 0x0369;
		//hl.all = 0x0369;

		//main program start
		sp = STACK_BEGIN_ADDR;
		pc = 0x150;
		af.all = 0x01B0;
		bc.all = 0x0013;
		de.all = 0x00D8;
		hl.all = 0x014D;

		//quick debug from
		//sp = 0xCFFF;
		//pc = 0x28A;
		//af.all = 0x00C0;
		//bc.all = 0x0000;
		//de.all = 0x00D8;
		//hl.all = 0xFDFF;

		//check point
		//sp = 0xCFFF;
		//pc = 0x2B8;
		//af.all = 0x8080;
		//bc.all = 0x0000;
		//de.all = 0x00D8;
		//hl.all = 0x97FF;
		
		//clear memory
		memset((void*)(&memory[0]), 0x00, 0x10000 * sizeof(U8DATA));
	}
	inline int paletteCodeToColor(U8 palette_color_code)
	{
		U8  color_code;
		int color;
		U8  bg_palette_color = 0xE4;//memory[BG_PALETTE_DATA];
		U8  palette_code_to_color_code[4] = {(bg_palette_color >> 0) & 0x3,(bg_palette_color >> 2) & 0x3,(bg_palette_color >> 4) & 0x3,(bg_palette_color >> 6) & 0x3};
		
		//palette color code -> color code
		color_code = palette_code_to_color_code[palette_color_code];
		
		//color code -> color
		
			
		switch(color_code)
		{
		case 0x0:
			color = COLOR(0, 0, 0);
			break;

		case 0x1:
			color = COLOR(85, 85, 85);
			break;

		case 0x2:
			color = COLOR(170, 170, 170);
			break;

		case 0x3:
			color = COLOR(255, 255, 255);
			break;

		default:
			//printf("unknown color code\n");
			break;
		}
		return color;
	}
	//         x_pos
	//          v
	//  y_pos ->-------------
	//          |  .(x,y)
	//          |
	//          |
	//          |
	//
	//build each 8x8 tile
	void buildTile(int y_pos,int x_pos, U16 dot_data_ofst)
	{
		int data_idx = 0;
		U8 upper_dot_byte, lower_dot_byte;
		int color;
		U8 color_code = 0;
		for (int i = 0; i < 16; i += 2) 
		{
			upper_dot_byte = memory[dot_data_ofst + i];
			lower_dot_byte = memory[dot_data_ofst + i + 1];
			//process per row
			for (int x = x_pos; x < (x_pos + 8); x++)
			{
				color_code = 0;
				color_code += (upper_dot_byte & BIT(7 - (x & 0x7))) ? 2 : 0;
				color_code += (lower_dot_byte & BIT(7 - (x & 0x7))) ? 1 : 0;
				//color = COLOR(color_code * 85, color_code * 85, color_code * 85);
				color = paletteCodeToColor(color_code);
				putpixel(x, y_pos, color);
				//putpixel(x, y_pos, COLOR(255, 255, 255));
			}
			y_pos++;
		}
	}
	//build all tiles
	void buildAllTileData()
	{
		int x_pos = 0, y_pos = 0;
		int video_ram_offset = 0;
		U8 _break = false;
		U8 sixteen_byte[16];
		U16 dot_data_ofst;
		for (y_pos = BG_TILE_BUFFER_Y + 0; y_pos < (256 + BG_TILE_BUFFER_Y); y_pos += 8) 
		{
			for (x_pos = BG_TILE_BUFFER_X + 0; x_pos < (BG_TILE_BUFFER_X + 256); x_pos += 8)
			{				
				rectangle(x_pos, y_pos, x_pos + 8, y_pos+8);
				//for (int i = 0; i < 16; i++) 
				//{
				//	sixteen_byte[i] = memory[VIDEO_RAM_BASE + video_ram_offset + i];
				//}
				buildTile(y_pos, x_pos, VIDEO_RAM_BASE + video_ram_offset);


				video_ram_offset += 16;
				if ((VIDEO_RAM_BASE + video_ram_offset) >= BG_MAP_ADDR) 
				{
					_break = true;
				}
			}
			if (_break)
				break;
		}
	}
	//get tile from built tile data
	void getBackgroundTile(U8 chr_code, void *tile_ptr)
	{
		U8 tile_idx_x = chr_code & 0x1F; // *8 % 256
		U8 tile_idx_y = chr_code >> 5;   // *8 / 256
		getimage(BG_TILE_BUFFER_X + tile_idx_x * 8, BG_TILE_BUFFER_Y + tile_idx_y * 8, BG_TILE_BUFFER_X + (tile_idx_x + 1) * 8, BG_TILE_BUFFER_Y + (tile_idx_y + 1) * 8, tile_ptr);
	}
	//render 160 * 144 takes about 40ms 
	//takes about 79800 cpu cycles
	void buildBackground()
	{
		U8    chr_code; //BG MAP
		U16   bg_disp_data_addr = ((memory[LCD_CTRL_REG] & BG_CODE_SEL_FALG) ? 0x9C00 : 0x9800);

		for(int y = 0; y < 256; y += 8)
		{
			for(int x = 0; x < 256; x += 8)
			{
				chr_code = memory[bg_disp_data_addr++];
				//out of viewport
				if ((x >= 160) || (y >= 144))
					continue;
				getBackgroundTile(chr_code, tile_buf_ptr);
				putimage(x, y, tile_buf_ptr, COPY_PUT);
			}
		}
	}
	void debugPPU() 
	{
		rectangle(0, 0, 257, 257);
		buildAllTileData();
		buildBackground();
	}
	void update_lcd_y_coord() 
	{
		if ((cpu_cycles & 0x1FF) == 0)
		{
			//each horizontal line takes 512 cpu cycles
			memory[LCD_Y_COORD_REG] = (U8)(memory[LCD_Y_COORD_REG] + 1);
			if (((U8)memory[LCD_Y_COORD_REG]) == 154)
				memory[LCD_Y_COORD_REG] = (U8)0x0;
		}
	}
	void check_interrupt() 
	{
		//
		//memory[INT_FLAGS_ADDR];
		//memory[INT_SWITCH_BASE];

		//vertical blanking interupt
		if(memory[LCD_Y_COORD_REG] > )
	}
	void run()
	{
		U8    opcode, xx;
		U16   aabb;
		U8    b, reg_value;
		U16   prev_value;
		U8    carry;
		U8    bhere = 1;
		U8    ly = memory[LCD_Y_COORD_REG];
		U8    _debug = true;
		GetSystemTime(&begin_time);

		//each loop takes about 0.0005 ms
		while (TRUE)
		{
			cpu_cycles++;
			update_lcd_y_coord();

			if (halt_state == TRUE) 
			{
				//check interrupt flag
				//isr();
			}
			else
			{
				opcode = memory[pc];
				xx = memory[pc + 1];
				aabb = (memory[pc + 2] << 8) | memory[pc + 1];
				b = (memory[pc + 1] >> 3) & 0x7;


				ly = memory[LCD_Y_COORD_REG];
				//volatile bool _hang = true;
				//if(((PERIPHERAL_BASE <= hl.all) && (hl.all <= 0xFF80)) || ((PERIPHERAL_BASE <= xx) && (xx <= 0xFF80)))
				//{	
				//	//printf("PERIPHERAL\n");
				//	while(_hang);
				//}
				
			
				//if (pc == 0x2F3) 
				//{
				//	buildAllTileData();
				//	buildBackground();
				//	getchar();
				//}

				printREG();
				//printf("ROM\n");
				//printf("F\n");
				//printf("00:%04X : %04X \n", pc,opcode);

				//if(this->pc == 0x40)
				//{
				//	buildAllTileData();
				//	buildBackground();
				//}

			
				switch (opcode)
				{
				case 0xC3:
					pc = aabb;
					//printf("jp %04X\n", pc);
					break;

				case 0xE9:
					pc = hl.all;
					//printf("jp %04X\n", pc);
					break;

				case 0xDA:
					pc += 3;
					pc = (af.f_c) ? (aabb) : (pc);
					//printf("jp c %04X\n", pc);
					break;

				case 0xD2:
					pc += 3;
					pc = (!af.f_c) ? (aabb) : (pc);
					//printf("jp nc %04X\n", pc);
					break;

				case 0xC2:
					pc += 3;
					pc = (!af.f_z) ? (aabb) : (pc);
					//printf("jp nz %04X\n", pc);
					break;

				case 0xCA:
					pc += 3;
					pc = (af.f_z) ? (aabb) : (pc);
					//printf("jp z %04X\n", pc);
					break;

				case 0x18:
					pc += 2;
					//printf("jr %04X\n", (char)xx + pc);
					pc += (char)xx;
					break;

				case 0x38:
					pc += 2;
					//printf("jr c %04X\n", pc + (char)xx);
					pc += (af.f_c) ? (xx) : (0);
					break;

				case 0x30:
					pc += 2;
					//printf("jr nc %04X\n", pc + (char)xx);
					pc += (!af.f_c) ? (xx) : (0);
					break;

				case 0x20:
					pc += 2;
					//printf("jr nz %04X\n", pc + (char)xx);
					pc += (!af.f_z) ? ((char)xx) : (0);
					break;

				case 0x28:
					pc += 2;
					//printf("jr z %04X\n", pc + (char)xx);
					pc += (af.f_z) ? (xx) : (0);
					break;

				//CALL $aabb
				case 0xCD: 
					pc += 3;				
					memory[--sp] = pc >> 8;
					memory[--sp] = pc & 0xFF;
					pc = aabb;
					break;
			
				//CALL C,$aabb
				case 0xDC:
					pc += 3;	
					if(af.f_c)
					{
						memory[--sp] = pc >> 8;
						memory[--sp] = pc & 0xFF;
						pc = aabb;
					}				
					break;
			
				//CALL NC,$aabb
				case 0xD4:
					pc += 3;			
					if(!af.f_c)
					{
						memory[--sp] = pc >> 8;
						memory[--sp] = pc & 0xFF;
						pc = aabb;
					}	
					break;
			
				//CALL NZ,$aabb
				case 0xC4:
					pc += 3;		
					if(!af.f_z)
					{
						memory[--sp] = pc >> 8;
						memory[--sp] = pc & 0xFF;
						pc = aabb;
					}	
					break;
			
				//CALL Z,$aabb
				case 0xCC:
					pc += 3;	
					if(af.f_z)
					{
						memory[--sp] = pc >> 8;
						memory[--sp] = pc & 0xFF;
						pc = aabb;
					}	
					break;

				case 0x00:
					pc += 1;
					//printf("nop");
					break;

				//RST
				case 0xC7:
				case 0xCF:
				case 0xD7:
				case 0xDF:
				case 0xE7:
				case 0xEF:
				case 0xF7:
				case 0xFF:
					pc += 1;
					memory[--sp] = (pc >> 8) ;
					memory[--sp] = (pc & 0xFF);
					pc = idxToP((opcode >> 3) & 7);
					//printf("rst %04X , sp = %04X\n", pc, sp);
					break;

					//add A.B..(HL)  to A
				case 0x80:
				case 0x81:
				case 0x82:
				case 0x83:
				case 0x84:
				case 0x85:
				case 0x86:
				case 0x87:
					//printf("add register to A\n", pc, sp);
					prev_value = af.a;
					af.a += (*idxToRegr_HL(opcode & 0x7));
					af.f_c = (prev_value > af.a) ? 1 : 0;
					af.f_h = ((prev_value & 0xF) > (af.a & 0xF)) ? 1 : 0;
					af.f_h = 0;
					af.f_z = (af.a == 0) ? 1 : 0;
					pc += 1;
					break;


					//add xx to A
				case 0xC6:
					//printf("add xx to A\n", pc, sp);
					prev_value = af.a;
					af.a += xx;
					af.f_z = (af.a == 0) ? 1 : 0;
					af.f_c = (prev_value > af.a) ? 1 : 0;
					af.f_h = ((prev_value & 0xF) > (af.a & 0xF)) ? 1 : 0;
					pc += 2;
					break;

					//add BC DE HL SP to HL
				case 0x09:
				case 0x19:
				case 0x29:
				case 0x39:
					//printf("add BC DE HL SP to HL\n", pc, sp);
					prev_value = af.a;
					hl.all += (*idxToRegss((opcode & 0x30) >> 4));
					af.f_z = (hl.all == 0) ? 1 : 0;
					af.f_c = (prev_value > hl.all) ? 1 : 0;
					af.f_h = ((prev_value & 0xFFF) > (hl.all & 0xFFF)) ? 1 : 0;
					pc += 1;
					break;

					//add xx to sp    
				case 0xE8:
					//printf("add xx to sp\n", pc, sp);
					prev_value = sp;
					sp += (char)xx;
					af.f_n = 0;
					af.f_z = 0;
					if (((char)xx) > 0)
					{
						af.f_h = ((prev_value & 0xFFF) < (sp & 0xFFF)) ? 1 : 0;
						af.f_c = (prev_value < sp) ? 1 : 0;
					}
					else if (((char)xx) < 0)
					{
						af.f_h = ((prev_value & 0xFFF) > (sp & 0xFFF)) ? 1 : 0;
						af.f_c = (prev_value > sp) ? 1 : 0;
					}
					pc += 2;
					break;


					//load reg with xx
				case 0x06:
				case 0x0E:
				case 0x16:
				case 0x1E:
				case 0x26:
				case 0x2E:
				case 0x36:
				case 0x3E:
					pc += 2;
					(*idxToRegr_HL((opcode >> 3) & 0x7)) = xx;
					//printf("LD  %04X\n", xx);
					break;

					//load reg with (HL)
				case 0x7E:
				case 0x46:
				case 0x4E:
				case 0x56:
				case 0x5E:
				case 0x66:
				case 0x6E:
					pc += 1;
					(*idxToRegr_HL((opcode >> 3) & 0x7)) = memory[hl.all];
					//printf("LD  %04X\n", memory[hl.all]);
					break;

					//load reg with reg
				case 0x40:
				case 0x41:
				case 0x42:
				case 0x43:
				case 0x44:
				case 0x45:
				case 0x47:
				case 0x48:
				case 0x49:
				case 0x4A:
				case 0x4B:
				case 0x4C:
				case 0x4D:
				case 0x4F:
				case 0x50:
				case 0x51:
				case 0x52:
				case 0x53:
				case 0x54:
				case 0x55:
				case 0x57:
				case 0x58:
				case 0x59:
				case 0x5A:
				case 0x5B:
				case 0x5C:
				case 0x5D:
				case 0x5F:
				case 0x60:
				case 0x61:
				case 0x62:
				case 0x63:
				case 0x64:
				case 0x65:
				case 0x67:
				case 0x68:
				case 0x69:
				case 0x6A:
				case 0x6B:
				case 0x6C:
				case 0x6D:
				case 0x6F:
				case 0x78:
				case 0x79:
				case 0x7A:
				case 0x7B:
				case 0x7C:
				case 0x7D:
				case 0x7F:
					pc += 1;
					(*idxToRegr_HL((opcode >> 3) & 0x7)) = (*idxToRegr_HL(opcode & 0x7));
					//printf("LD  %04X\n", (*idxToRegr_HL((opcode >> 3) & 0x7)));
					break;

					//load 16bit_reg with aabb
				case 0x01:
				case 0x11:
				case 0x21:
				case 0x31:
					pc += 3;
					(*idxToRegdd((opcode >> 4) & 0x3)) = aabb;
					//printf("LD  \n");
					break;


					//load A with (BC)
				case 0x0A:
					pc += 1;
					af.a = memory[bc.c];
					//printf("LD  \n");
					break;

					//load A with (DE)
				case 0x1A:
					pc += 1;
					af.a = memory[de.all];
					//printf("LD  \n");
					break;

					//LD HLI load A with (HL++)
				case 0x2A:
					pc += 1;
					af.a = memory[hl.all++];
					//printf("LDHLI  \n");
					break;

					//LD HLD load A with (HL--)
				case 0x3A:
					pc += 1;
					af.a = memory[hl.all--];
					//printf("LDHLD  \n");
					break;

			
					//LD HLI load (HL++) with A 
				case 0x22:
					pc += 1;
					memory[hl.all++] = af.a;
					//printf("LD HLI load (HL++) with A  \n");
					break;
			
					//LD HLD load(HL--) with A 
				case 0x32:
					pc += 1;
					memory[hl.all--] = af.a;
					//printf("LD HLD load(HL--) with A   \n");
					break;


					//load A with (aabb)
				case 0xFA:
					af.a = memory[aabb];
					//printf("LD mempry[aabb] = %04X  \n",memory[aabb]);
					pc += 3;
					break;

					//load HL with SP
				case 0xF8:
					prev_value = hl.all;
					hl.all = sp + (char)xx;
					af.f_n = 0;
					af.f_z = 0;
					if (((char)xx) > 0)
					{
						af.f_h = ((prev_value & 0xFFF) < (hl.all & 0xFFF)) ? 1 : 0;
						af.f_c = (prev_value < hl.all ) ? 1 : 0;
					}
					else if (((char)xx) < 0) 
					{
						af.f_h = ((prev_value & 0xFFF) > (hl.all & 0xFFF)) ? 1 : 0;
						af.f_c = (prev_value > hl.all) ? 1 : 0;
					}
					pc += 2;
					//printf("load HL with SP  \n");
					break;

					//load SP with HL
				case 0xF9:
					sp = hl.all;
					pc += 1;
					//printf("load SP with HL  \n");
					break;

					//load (HL) with reg				
				case 0x77:
				case 0x70:
				case 0x71:
				case 0x72:
				case 0x73:
				case 0x74:
				case 0x75:
					//printf("load (HL) with reg  \n");
					pc += 1;
					memory[hl.all] = (*idxToRegr_HL(opcode & 0x7));
					break;

					//load (aabb) with A
				case 0xEA:
					memory[aabb] = af.a;
					pc += 3;
					//printf("load (aabb) with A  \n");
					break;

					//load (aabb) with sp
				case 0x08:
					//printf("load (aabb) with sp  \n");
					memory[aabb] = sp & 0xFF;
					memory[aabb + 1] = (sp >> 8) & 0xFF;
					pc += 3;
					break;

					//load (0xFF00 + xx) with A
				case 0xE0:
					//printf("load (xx) with A  \n");
					memory[PERIPHERAL_BASE + xx] = af.a;
				
					pc += 2;
					//debug ,simulate interrupt
				
					/*if((pc == 0x2BE))
					{					
						if(bhere--)
						{
							memory[--sp] = (pc-2) >> 8;
							memory[--sp] = (pc-2) & 0xFF;
							memory[0xFFCE] = 0;
							memory[0xFF98] = 0;
							memory[0xFFE3] = 0;

							memory[0xFFB6] = 0x3E;
							memory[0xFFB7] = 0xC0;
							memory[0xFFB8] = 0xE0;
							memory[0xFFB9] = 0x46;
							memory[0xFFBA] = 0x3E;
							memory[0xFFBB] = 0x28;
							memory[0xFFBC] = 0x3D;
							memory[0xFFBD] = 0x20;
							memory[0xFFBE] = 0xFD;
							memory[0xFFBF] = 0xC9;
							memory[0xFFC0] = 0x37;
							memory[0xFFC1] = 0x1C;
							memory[0xFFE1] = 0x24;


							memory[0xFF00] = 0xEF;

							pc = 0x17E;
						}
					}*/

					break;
				
					//load (0xFF00 + C) with A
				case 0xE2:
					//printf("load (C) with A  \n");
					memory[PERIPHERAL_BASE + bc.c] = af.a;
					pc += 1;
					break;
				
					//load A with (0xFF00 + xx)
				case 0xF0:
					pc += 2;
					af.a = memory[PERIPHERAL_BASE + xx];
					//printf("LD	\n");
					break;
			
					//load A with (0xFF00 + C)
				case 0xF2:
					pc += 1;
					af.a = memory[PERIPHERAL_BASE + bc.c];
					//printf("LD	\n");
					break;

					//load (BC) with A
				case 0x02:
					//printf("load (BC) with A  \n");
					memory[bc.all] = af.a;
					pc += 1;
					break;

					//load (DE) with A
				case 0x12:
					//printf("load (DE) with A  \n");
					memory[de.all] = af.a;
					pc += 1;
					break;



					//POP AF,BC,DE,HL
				case 0xF1:
				case 0xC1:
				case 0xD1:
				case 0xE1:
					(*idxToRegqq((opcode >> 4) & 0x3)) = (memory[sp + 1] << 8) | memory[sp];
					pc += 1;
					sp += 2;
					//printf("pop  %04X\n", sp);
					break;

					//PUSH AF,BC,DE,HL
				case 0xF5:
				case 0xC5:
				case 0xD5:
				case 0xE5:
					memory[--sp] = (*idxToRegqq((opcode >> 4) & 0x3)) >> 8;
					memory[--sp] = (*idxToRegqq((opcode >> 4) & 0x3)) & 0xFF;
					pc += 1;
					//printf("push  %04X\n", sp);
					break;

					//ADC  add reg..(HL) + flag_carry to A
				case 0x88:
				case 0x89:
				case 0x8A:
				case 0x8B:
				case 0x8C:
				case 0x8D:
				case 0x8E:
				case 0x8F:
					prev_value = af.a;
					af.a = (*idxToRegr_HL(opcode & 0x7)) + af.f_c;
					af.f_c = (prev_value > af.a) ? 1 : 0;
					af.f_h = ((prev_value & 0xF) > (af.a & 0xF)) ? 1 : 0;
					af.f_n = 0;
					af.f_z = (af.a == 0) ? 1 : 0;
					pc += 1;
					//printf("ADC  %04X\n", af.a);
					break;

					//ADC add xx + flag_carry to A
				case 0xCE:
					prev_value = af.a;
					af.a = xx + af.f_c;
					pc += 2;				
					af.f_c = (prev_value > af.a) ? 1 : 0;
					af.f_h = ((prev_value & 0xF) > (af.a & 0xF)) ? 1 : 0;
					af.f_n = 0;
					af.f_z = (af.a == 0) ? 1 : 0;
					//printf("ADC  %04X\n", af.a);
					break;

					//CCF clear carry flag
				case 0x3F:
					af.f_c = 0;
					pc += 1;
					//printf("CCF  %04X\n", af.all);
					break;

					//SCF set carry flag
				case 0x37:
					af.f_c = 1;
					pc += 1;
					//printf("SCF  %04X\n", af.all);
					break;


					//AND xx
				case 0xE6:
					pc += 2;				
					af.a &= xx;
					af.f_c = 0;
					af.f_h = 1;
					af.f_n = 0;
					af.f_z = (af.a == 0) ? 1 : 0;
					break;
				
					//AND A,B..(HL)
				case 0xA0:
				case 0xA1:
				case 0xA2:
				case 0xA3:
				case 0xA4:
				case 0xA5:
				case 0xA6:
				case 0xA7:
					af.a &= (*idxToRegr_HL(opcode & 0x7) > af.a);
					af.f_c = 0;
					af.f_h = 1;
					af.f_n = 0;
					af.f_z = (af.a == 0) ? 1 : 0;
					pc += 1;
					break;


					//CP compare A with reg,(HL)				
				case 0xBE:
				case 0xBF:
				case 0xB8:
				case 0xB9:
				case 0xBA:
				case 0xBB:
				case 0xBC:
				case 0xBD:
					pc += 1;
					af.f_c = (*idxToRegr_HL(opcode & 0x7) > af.a) ? 1 : 0;
					af.f_h = ((*idxToRegr_HL(opcode & 0x7) & 0xF) > (af.a & 0xF)) ? 1 : 0;
					af.f_n = 1;
					af.f_z = (*idxToRegr_HL(opcode & 0x7) == af.a) ? 1 : 0;
					//printf("CP  %04X\n", af.a);
					break;

					//compare A xx	
				case 0xFE:
					af.f_c = (xx > af.a) ? 1 : 0;
					af.f_h = ((xx & 0xF) > (af.a & 0xF)) ? 1 : 0;
					af.f_n = 1;
					af.f_z = (af.a == xx) ? 1 : 0;
					pc += 2;
					//printf("CP  %04X\n", xx);
					break;

					//CPL complement A	
				case 0x2F:
					af.f_h = 1;
					af.f_n = 1;
					af.a = ~af.a;
					pc += 1;
					//printf("CPL  %04X\n", af.a);
					break;

					//DAA decimal adjust A
				case 0x27:
					if (af.f_n)//substract
					{
						if (((af.a & 0xF0) > 0x90) || (af.f_c))
						{
							af.f_c = 1;
							af.a -= 0x60;
						}
						if (((af.a & 0xF) > 0x9) || (af.f_h))
						{
							af.f_h = 1;
							af.a -= 0x60;
						}
					}
					else//addition
					{
						if (((af.a & 0xF0) > 0x90) || (af.f_c))
						{
							af.f_c = 1;
							af.a += 0x60;
						}
						if (((af.a & 0xF) > 0x9) || (af.f_h))
						{
							af.f_h = 1;
							af.a += 0x60;
						}
					}
					//printf("DAA  %04X\n", af.a);
					pc += 1;
					break;

					//decrement	A,B..L (HL) by 1			
				case 0x05:
				case 0x0D:
				case 0x15:
				case 0x1D:
				case 0x25:
				case 0x2D:
				case 0x35:
				case 0x3D:
					prev_value = (*idxToRegr_HL((opcode >> 3) & 0x7));
					(*idxToRegr_HL((opcode >> 3) & 0x7))--;
					reg_value = (*idxToRegr_HL((opcode >> 3) & 0x7));
					af.f_n = 1;
					pc += 1;
					af.f_z = (reg_value == 0) ? 1 : 0;
					af.f_h = ((reg_value & 0xF) > (prev_value & 0xF)) ? 1 : 0;
					//printf("DEC  %04X\n", (*idxToRegr_HL((opcode >> 3) & 0x7)));
					break;

					//DEC decrement BC..HL by 1 	
				case 0x0B:
				case 0x1B:
				case 0x2B:
				case 0x3B:
					(*idxToRegss((opcode >> 4) & 0x3))--;
					pc += 1;
					//printf("DEC  %04X\n", (*idxToRegss((opcode >> 4) & 0x3)));
					break;

					//DI disable interrupt
				case 0xF3:
					pc += 1;
					ime = false;
					//printf("DI\n");
					break;

					//EI enable interrupt
				case 0xFB:
					pc += 1;
					ime = true;
					//printf("EI\n");
					break;

					//halt
					//if IME is true -> go to isr upon interrupt occuring
					//if IME is false -> starts from pc
				case 0x76:
					halt_state = TRUE;
					pc += 1;
					//printf("halt\n");

					GetSystemTime(&end_time);

					printf("cpu cycle : %d\n", cpu_cycles);
					printf("%d %d\n", begin_time.wSecond, begin_time.wMilliseconds);
					printf("%d %d\n", end_time.wSecond, end_time.wMilliseconds);

					buildAllTileData();
					GetSystemTime(&begin_time);
					buildBackground();
					GetSystemTime(&end_time);
					printf("buildBackground begin %d %d\n", begin_time.wSecond, begin_time.wMilliseconds);
					printf("buildBackground end %d %d\n", end_time.wSecond, end_time.wMilliseconds);
					//while (_debug);
					break;

					//increment A B..L (HL) by 1 		
				case 0x04:
				case 0x0C:
				case 0x14:
				case 0x1C:
				case 0x24:
				case 0x2C:
				case 0x34:
				case 0x3C:
					prev_value = (*idxToRegr_HL((opcode >> 3) & 0x7));
					(*idxToRegr_HL((opcode >> 3) & 0x7))++;
					reg_value = (*idxToRegr_HL((opcode >> 3) & 0x7));
					af.f_h = ((prev_value & 0xF) > (reg_value & 0xF)) ? 1 : 0;
					af.f_n = 0;
					af.f_z = (reg_value == 0) ? 1 : 0;
					pc += 1;
					//printf("INC  %04X\n", (*idxToRegr_HL((opcode >> 3) & 0x7)));
					break;

					//INC increment BC..HL by 1 	
				case 0x03:
				case 0x13:
				case 0x23:
				case 0x33:
					(*idxToRegss((opcode >> 4) & 0x3))++;
					pc += 1;
					//printf("INC  %04X\n", (*idxToRegss((opcode >> 4) & 0x3)));
					break;

					//OR A with xx
				case 0xF6:
					af.a |= xx;
					af.f_c = 0;
					af.f_h = 0;
					af.f_n = 0;
					af.f_z = (af.a == 0) ? 1 : 0;
					pc += 2;
					//printf("OR A  %04X\n", (*idxToRegr_HL(opcode & 0x7)));
					break;

					//OR A with reg (HL)
				case 0xB0:
				case 0xB1:
				case 0xB2:
				case 0xB3:
				case 0xB4:
				case 0xB5:
				case 0xB6:
				case 0xB7:
					af.a |= (*idxToRegr_HL(opcode & 0x7));
					af.f_c = 0;
					af.f_h = 0;
					af.f_n = 0;
					af.f_z = (af.a == 0) ? 1 : 0;
					pc += 1;
					//printf("OR = %04X\n", (*idxToRegr_HL(opcode & 0x7)));
					break;

				case 0xCB:
					//RLC
					if (xx <= 0x07)
					{
						af.f_c = (*idxToRegr_HL(xx & 0x7)) & 0x80 ? 1 : 0;
						(*idxToRegr_HL(xx & 0x7)) <<= 1;
						reg_value = (*idxToRegr_HL(xx & 0x7));
						af.f_h = 0;
						af.f_n = 0;
						af.f_z = (reg_value == 0) ? 1 : 0;
						//printf("RLC = %04X\n", (*idxToRegr_HL(xx & 0x7)));
					}
					//RRC
					else if ((0x08 <= xx) && (xx <= 0x0F))
					{
						af.f_c = (*idxToRegr_HL(xx & 0x7)) & 0x01 ? 1 : 0;
						(*idxToRegr_HL(xx & 0x7)) >>= 1;
						reg_value = (*idxToRegr_HL(xx & 0x7));
						af.f_h = 0;
						af.f_n = 0;
						af.f_z = (reg_value == 0) ? 1 : 0;
						//printf("RRC = %04X\n", (*idxToRegr_HL(xx & 0x7)));
					}
					//RL
					else if ((0x10 <= xx) && (xx <= 0x17))
					{
						prev_value = af.f_c;
						af.f_c = (*idxToRegr_HL(xx & 0x7)) & 0x80 ? 1 : 0;
						(*idxToRegr_HL(xx & 0x7)) <<= 1;
						(*idxToRegr_HL(xx & 0x7)) |= prev_value;
						reg_value = (*idxToRegr_HL(xx & 0x7));
						af.f_h = 0;
						af.f_n = 0;
						af.f_z = (reg_value == 0) ? 1 : 0;
						//printf("RL = %04X\n", (*idxToRegr_HL(xx & 0x7)));
					}
					//RR
					else if ((0x18 <= xx) && (xx <= 0x1F))
					{
						prev_value = af.f_c;
						af.f_c = (*idxToRegr_HL(xx & 0x7)) & 0x01 ? 1 : 0;
						(*idxToRegr_HL(xx & 0x7)) >>= 1;
						(*idxToRegr_HL(xx & 0x7)) |= (prev_value << 8);
						reg_value = (*idxToRegr_HL(xx & 0x7));
						af.f_h = 0;
						af.f_n = 0;
						af.f_z = (reg_value == 0) ? 1 : 0;
						//printf("RR = %04X\n", (*idxToRegr_HL(xx & 0x7)));
					}
					//SET
					else if ((0xC0 <= xx) && (xx <= 0xFF))
					{
						(*idxToRegr_HL(xx & 0x7)) &= BIT(b);
						//printf("SET = %04X\n", (*idxToRegr_HL(xx & 0x7)));
					}
					//SLA
					else if ((0x20 <= xx) && (xx <= 0x27))
					{
						af.f_c = ((*idxToRegr_HL(xx & 0x7)) & 0x80) ? 1 : 0;
						(*idxToRegr_HL(xx & 0x7)) <<= 1;
						reg_value = (*idxToRegr_HL(xx & 0x7));
						af.f_h = 0;
						af.f_n = 0;
						af.f_z = (reg_value == 0) ? 1 : 0;
						//printf("SLA = %04X\n", (*idxToRegr_HL(xx & 0x7)));
					}
					//SRA
					else if ((0x28 <= xx) && (xx <= 0x2F))
					{
						prev_value = ((*idxToRegr_HL(xx & 0x7)) & 0x80);
						af.f_c = ((*idxToRegr_HL(xx & 0x7)) & 0x01) ? 1 : 0;
						(*idxToRegr_HL(xx & 0x7)) >>= 1;
						(*idxToRegr_HL(xx & 0x7)) |= prev_value;
						reg_value = (*idxToRegr_HL(xx & 0x7));
						af.f_h = 0;
						af.f_n = 0;
						af.f_z = (reg_value == 0) ? 1 : 0;
						//printf("SRA = %04X\n", (*idxToRegr_HL(xx & 0x7)));
					}
					//SWAP
					else if ((0x30 <= xx) && (xx <= 0x37))
					{
						prev_value = (*idxToRegr_HL(xx & 0x7));
						(*idxToRegr_HL(xx & 0x7)) <<= 4;
						(*idxToRegr_HL(xx & 0x7)) &= 0xF0;
						(*idxToRegr_HL(xx & 0x7)) |= (prev_value >> 4);
						reg_value = (*idxToRegr_HL(xx & 0x7));
						af.f_c = 0;
						af.f_h = 0;
						af.f_n = 0;
						af.f_z = (reg_value == 0) ? 1 : 0;
						//printf("SWAP = %04X\n", (*idxToRegr_HL(xx & 0x7)));
					}
					//SRL
					else if ((0x38 <= xx) && (xx <= 0x3F))
					{
						af.f_c = ((*idxToRegr_HL(xx & 0x7)) & 0x01) ? 1 : 0;
						(*idxToRegr_HL(xx & 0x7)) >>= 1;
						reg_value = (*idxToRegr_HL(xx & 0x7));
						af.f_h = 0;
						af.f_n = 0;
						af.f_z = (reg_value == 0) ? 1 : 0;
						//printf("SRL = %04X\n", (*idxToRegr_HL(xx & 0x7)));
					}
					//BIT bit
					else if ((0x40 <= xx) && (xx <= 0x7F))
					{
						af.f_z = (*idxToRegr_HL(xx & 0x7)) & BIT(b) ? 0 : 1;
						af.f_h = 1;
						af.f_n = 0;
						//printf("BIT = %04X\n", (*idxToRegr_HL(xx & 0x7)));
					}
					//RES
					else if ((0x80 <= xx) && (xx <= 0xBF))
					{
						(*idxToRegr_HL(xx & 0x7)) &= (~BIT(b));
						//printf("RES = %04X\n", (*idxToRegr_HL(xx & 0x7)));
					}
					else
					{
						//printf("unknown opcode xx = %02X\n",xx);
						while(1);
					}
					pc += 2;
					break;

					//RET NZ  
				case 0xC0:
					if (!af.f_z)
						pc = (memory[sp + 1] << 8) | memory[sp];
					else
						pc += 1;
					sp += 2;
					//printf("RET NZ = %04X\n", pc);
					break;

					//RET Z   
				case 0xC8:
					if (af.f_z)
						pc = (memory[sp + 1] << 8) | memory[sp];
					else
						pc += 1;
					sp += 2;
					//printf("RET Z = %04X\n", pc);
					break;

					//RET NC  
				case 0xD0:
					if (!af.f_c)
						pc = (memory[sp + 1] << 8) | memory[sp];
					else
						pc += 1;
					sp += 2;
					//printf("RET NC pc = %04X\n", pc);
					break;

					//RET C   
				case 0xD8:
					if (af.f_c)
						pc = (memory[sp + 1] << 8) | memory[sp];
					else
						pc += 1;
					sp += 2;
					//printf("RET C pc = %04X\n", pc);
					break;

					//RET	  
				case 0xC9:
					pc = (memory[sp + 1] << 8) | memory[sp];
					sp += 2;
					//printf("RET pc = %04X\n", pc);
					//auto disable ime
					this->ime = 1;
					break;

					//RETI	  
				case 0xD9:
					//printf("RETI ?!\r\n");
					pc = (memory[sp + 1] << 8) | memory[sp];
					sp += 2;
					//auto enable ime
					this->ime = 1;
					break;

					//RLA
				case 0x17:
					pc += 1;
					prev_value = af.f_c;
					af.f_c = af.a & 0x80 ? 1 : 0;
					af.f_h = 0;
					af.f_n = 0;
					af.f_z = 0;
					af.a <<= 1;
					af.a |= prev_value;
					//printf("RLA af.a << 1 = %04X\n", af.a);
					break;


					//RLCA
				case 0x07:
					pc += 2;
					af.f_c = af.a & 0x80 ? 1 : 0;
					af.f_h = 0;
					af.f_n = 0;
					af.f_z = 0;
					af.a <<= 1;
					//printf("RLCA af.a << 1 = %04X\n", af.a);
					break;


					//RRA
				case 0x1F:
					pc += 2;
					prev_value = af.f_c;
					af.f_c = af.a & 0x01 ? 1 : 0;
					af.f_h = 0;
					af.f_n = 0;
					af.f_z = 0;
					af.a >>= 1;
					af.a |= (prev_value << 8);
					//printf("RRA af.a >> 1 = %04X\n", af.a);
					break;


					//RRCA
				case 0x0F:
					pc += 2;
					af.f_c = af.a & 0x01 ? 1 : 0;
					af.f_h = 0;
					af.f_n = 0;
					af.f_z = 0;
					af.a >>= 1;
					//printf("RRCA af.a >> 1 = %04X\n", af.a);
					break;

					//SBC subtract xx+cy from A
				case 0xDE:
					//printf("%04X sub %04X cy: %04X\n", af.a, xx, af.f_c);
					pc += 2;
					prev_value = af.a;
					af.a = af.a - af.f_c - xx;
					af.f_c = (prev_value > af.a) ? 1 : 0;
					af.f_h = ((prev_value & 0xF) > (af.a & 0xF)) ? 1 : 0;
					af.f_n = 1;
					af.f_z = (af.a == 0) ? 1 : 0;
				
					break;

					//SBC subtract reg+cy from A
				case 0x98:
				case 0x99:
				case 0x9A:
				case 0x9B:
				case 0x9C:
				case 0x9D:
				case 0x9E:
				case 0x9F:
					//printf("%04X sub %04X cy: %04X\n", af.a, (*idxToRegr_HL(opcode & 0x7)), af.f_c);
					pc += 2;
					reg_value = (*idxToRegr_HL(opcode & 0x7));
					carry     = af.f_c;
					af.f_h = (((reg_value + carry) & 0xF) > (af.a & 0xF)) ? 1 : 0;
					af.f_c = ((reg_value + carry) > af.a) ? 1 : 0;
					af.a = af.a - af.f_c - reg_value;
					af.f_z = (af.a == 0) ? 1 : 0;
					af.f_n = 1;
					break;

					//SUB
				case 0x90:
				case 0x91:
				case 0x92:
				case 0x93:
				case 0x94:
				case 0x95:
				case 0x96:
				case 0x97:
					//printf("%04X sub %04X\n", af.a, (*idxToRegr_HL(opcode & 0x7)));
					pc += 1;
					af.f_c = ((*idxToRegr_HL(opcode & 0x7)) > af.a) ? 1 : 0;
					af.f_h = (((*idxToRegr_HL(opcode & 0x7)) & 0xF) > (af.a & 0xF)) ? 1 : 0;
					af.a -= (*idxToRegr_HL(opcode & 0x7));
					af.f_z = (af.a == 0) ? 1 : 0;
					af.f_n = 1;
					break;

					//SUB xx
				case 0xD6:
					//printf("%04X sub %04X\n", af.a, (*idxToRegr_HL(opcode & 0x7)));
					pc += 2;
					af.f_c = (xx > af.a) ? 1 : 0;
					af.f_h =  ((xx & 0xF) > (af.a & 0xF)) ? 1 : 0;
					af.a -= (*idxToRegr_HL(opcode & 0x7));
					af.f_z = (af.a == 0) ? 1 : 0;
					af.f_n = 1;
					break;

					//XOR
				case 0xA8:
				case 0xA9:
				case 0xAA:
				case 0xAB:
				case 0xAC:
				case 0xAD:
				case 0xAE:
				case 0xAF:
					//printf("%04X xor %04X\n", af.a, (*idxToRegr_HL(opcode & 0x7)));
					pc += 1;
					af.a = (*idxToRegr_HL(opcode & 0x7)) ^ af.a;
					af.f_z = (af.a == 0) ? 1 : 0;
					af.f_n = 0;
					af.f_c = 0;
					af.f_h = 0;
					break;
				default:
					//printf("unknown opcode : \n", opcode);
					break;

				}

			
			//getchar();
			}
		}
	}



};






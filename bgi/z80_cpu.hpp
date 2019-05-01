#pragma once
#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <iomanip>


typedef unsigned char  U8;
typedef unsigned short U16;

#define BIT(x)                 (0x1 << (x))

#define CARTRIDGE_ROM_BASE     (0x0000)
#define VIDEO_RAM_BASE         (0x8000)
#define EXTERNAL_RAM_BASE      (0xA000)
#define WORK_RAM_BASE          (0xC000)
#define OAM_BASE               (0xFE00)
#define PERIPHERAL_BASE        (0xFF00)
#define JOY_PAD                (0xFF00)
#define 	JOY_PAD_RESET          (BIT(4)|BIT(5))
#define 	JOY_PAD_SEL_BUTTON     BIT(4)
#define 	JOY_PAD_SEL_DIRECT     BIT(5)
#define TIMA                   (0xFF05)	//timer counter
#define TMA                    (0xFF06)	//timer modulo
#define TAC                    (0xFF07) // timer control
#define LCD_CTRL_REG           (0xFF40) //LCDC
#define 	BG_CODE_SEL_FALG       BIT(3)   //BG Code Area Selection Flag , tile index
#define 	BG_CHAR_SEL_FALG       BIT(4)   //BG char data Selection Flag , tile data
#define 	BG_WIN_ON_FALG         BIT(5)   //window on Flag
#define 	BG_WIN_VODE_FALG       BIT(6)   //window Code Area Selection Flag
#define SCY                    (0xFF42) //lcd scroll y
#define SCX                    (0xFF43) //lcd scroll x
#define LCD_Y_COORD_REG        (0xFF44) //LY lcd control y coordinate
#define BG_PALETTE_DATA        (0xFF47) //BGP

#define INT_FLAGS         (0xFF0F)
#define 	INT_FLAG_VERT_BLANKING BIT(4)
#define 	INT_FLAG_LCDC          BIT(3)
#define 	INT_FLAG_TIMER         BIT(2)
#define 	INT_FLAG_SERIEAL_TRANS BIT(1)
#define 	INT_FLAG_P10_P13       BIT(0)

#define HIGH_WORK_RAM_BASE     (0xFF80)
#define STACK_BEGIN_ADDR       (0xFFFE)
#define INT_SWITCH             (0xFFFF)

#define BG_CHAR_DATA_SIZE      (0x1000)
#define BG_CODE_DATA_SIZE      (0x400)



#define MEMORY_SIZE            (65536)

#define SCREEN_OFFSET          (10)
#define LCD_WIDTH              (160)
#define LCD_HEIGHT             (144)
#define VIEWPORT_X             (300)
#define VIEWPORT_Y             (225)

#define DEBUG_WINDOW_WIDTH     (200)
#define BG_TILE_BGI_BUFFER_X   (256)
#define BG_TILE_BGI_BUFFER_Y   (0)


#define ISR_VERTICAL_BLANKING  0x40
#define ISR_LCDC               0x48
#define ISR_TIMER              0x50
#define ISR_SERIEAL_TRANS_CPL  0x58
#define ISR_P10_P10_INPUT      0x60


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

class DMGZ80CPU;

class TILE_DOT_DATA_PAINTER
{
public:
	static int paletteCodeToColor(U8 palette_color_code)
	{
		U8  color_code;
		int color;
		U8  bg_palette_color = 0xE4;//memory[BG_PALETTE_DATA];
		U8  palette_code_to_color_code[4] = { (bg_palette_color >> 0) & 0x3,(bg_palette_color >> 2) & 0x3,(bg_palette_color >> 4) & 0x3,(bg_palette_color >> 6) & 0x3 };

		//palette color code -> color code
		color_code = palette_code_to_color_code[palette_color_code];

		//color code -> color
		switch (color_code)
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
	//paint 1 row, 8 dots
	//in BG_TILE_BGI_BUFFER ,each row accommodates 32 tile (256 dots)
	static void paintTileDotData(U16 dot_data_byte_idx, U8 upper_dot_byte, U8 lower_dot_byte)
	{
		// dot_data_idx = (dot_data_byte_idx >> 1) , because two bytes represent one row

		// ---0---  ---8---       --------   --------
		// ---1---  ---9---       --------   --------
		// ---2---  --10---       --------   --------
		// ---3---  --11---       --------   --------
		// ---4---  -+12---       --------   --------
		// ---5---  --13---       --------   --------
		// ---6---  --14---       --------   --------
		// ---7---  --15---   ~   --------   ---255--
		//             ^-- dot_data_idx = 12      ^---- dot_data_idx = 255, 32 tiles 
		//
		// 12 / 8 = 1     -> x_pos = 1*8
		// 12 / 256 = 0   -> y_tile_row = 0*8
		// (12 % 8) = 4 -> y_pos = y_row + 4

		dot_data_byte_idx = dot_data_byte_idx - VIDEO_RAM_BASE;
		U16 dot_data_idx = (dot_data_byte_idx >> 1);
		//each tile contains 64 dots
		int x_pos = BG_TILE_BGI_BUFFER_X + (((dot_data_idx & 0xFF) >> 3) << 3);
		int y_tile_row = (dot_data_idx >> 8) << 3;
		int y_pos = BG_TILE_BGI_BUFFER_Y + y_tile_row + ((dot_data_idx) & 0x7);
		int bgi_color;
		U8  color_code = 0;

		for (int x = x_pos; x < (x_pos + 8); x++)
		{
			color_code = 0;
			color_code += (upper_dot_byte & BIT(7 - (x & 0x7))) ? 2 : 0;
			color_code += (lower_dot_byte & BIT(7 - (x & 0x7))) ? 1 : 0;
			bgi_color = paletteCodeToColor(color_code);
			putpixel(x, y_pos, bgi_color);
		}
	}
};
class U8DATA
{
public:
	static U16                    *pc;
	static DMGZ80CPU              *cpu;
	static TILE_DOT_DATA_PAINTER  *tile_dot_data_painter;
	//-------------------------------
	U8  value;
	U16 access_addr;
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
		switch (this->access_addr)
		{
			//case 0xFF00:
		case 0xFF70:
		case 0xFF01:
		case 0xFF02:
		case 0xFF04:
		case 0xFF05:
		case 0xFF06:
		case 0xFF07:
			//case 0xFF0F:
		case 0xFF40:
		case 0xFF41:
		case 0xFF42:
		case 0xFF43:
		case 0xFF45:
		case 0xFF46:
		case 0xFF47:
		case 0xFF48:
		case 0xFF49:
		case 0xFF4A:
		case 0xFF4B:
		case 0xFE00:
		case 0xFE01:
		case 0xFE02:
		case 0xFE03:
			//case 0xFFFF:
			//printf("pc= %04X ,cast operator @ %04X\n", *pc, *access_addr);
			break;
		}
		////printf("cast operator @ %04X\n", *access_addr);
		if (access_addr == 0xFF44)
		{
			//while (_hang);
			//this->value = 0x94;
		}
		else if (access_addr == 0xFF00)
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
		switch (this->access_addr)
		{
			//case 0xFF00:
		case 0xFF70:
		case 0xFF01:
		case 0xFF02:
		case 0xFF04:
		case 0xFF05:
		case 0xFF06:
		case 0xFF07:
			//case 0xFF0F:
		case 0xFF40:
		case 0xFF41:
		case 0xFF42:
		case 0xFF43:
		case 0xFF45:
		case 0xFF46:
		case 0xFF47:
		case 0xFF48:
		case 0xFF49:
		case 0xFF4A:
		case 0xFF4B:
		case 0xFE00:
		case 0xFE01:
		case 0xFE02:
		case 0xFE03:
			//case 0xFFFF:
			//printf("pc= %04X ,get address operator @ %04X\n", *pc, *access_addr);
			break;
		}
		return &this->value;
	}
	U8DATA &operator=(U8 val);
};

U16                    *U8DATA::pc;
DMGZ80CPU              *U8DATA::cpu;
TILE_DOT_DATA_PAINTER  *U8DATA::tile_dot_data_painter;

class DEBUG_MEM
{
public:

	//U16 access_addr;
	U8DATA  data[MEMORY_SIZE];
	//U8  data[MEMORY_SIZE];
	U8  _hang;
	U8DATA  *stack;
	DEBUG_MEM()
	{
		_hang = true;
		stack = data + 0xCFF0;
		//access_addr = 0;
	}
	U8DATA &operator[](U16 addr)
	{
		//access_addr = addr;
		this->data[addr].access_addr = addr;
		return this->data[addr];
	}
};

class DMGZ80CPU
{
private:

	//U8 memory[MEMORY_SIZE];
	//register
	AF af;
	BC bc;
	DE de;
	HL hl;
	U16 sp;
	U16 stack[16];
	U8 ime;                            //interrupt master enable

									   //emulator flag
	U8               halt_state;
	U8               stop_state;
	void*            tile_buf_ptr;
	void*            viewport_buf_ptr;
	SYSTEMTIME       begin_time;
	SYSTEMTIME       end_time;
	U8               refresh_lcd;
	unsigned int     cpu_cycles;
	U8               tile_data_built;

public:
	U16 pc;
	TILE_DOT_DATA_PAINTER tile_dot_data_painter;
	DEBUG_MEM memory;
	DMGZ80CPU()
	{
		U8DATA::pc = &this->pc;
		U8DATA::tile_dot_data_painter = &this->tile_dot_data_painter;
		U8DATA::cpu = this;

		//boot rom
		//sp = STACK_BEGIN_ADDR;
		//pc = 0x0;
		//af.all = 0x0000;
		//bc.all = 0x0000;
		//de.all = 0x0000;
		//hl.all = 0x0000;
		ime = TRUE;

		tile_buf_ptr = malloc(imagesize(0, 0, 8, 8));
		viewport_buf_ptr = malloc(imagesize(0, 0, LCD_WIDTH, LCD_HEIGHT));
		halt_state = FALSE;
		cpu_cycles = 0;
		refresh_lcd = FALSE;
		

		
		sp = STACK_BEGIN_ADDR;
		pc = 0x100;
		af.all = 0x1100;
		bc.all = 0x0000;
		de.all = 0x0000;
		hl.all = 0x0000;
		ime = TRUE;
		

		//main program start
		//tetris rom initial state
		/*sp = STACK_BEGIN_ADDR;
		pc = 0x150;
		af.all = 0x01B0;
		bc.all = 0x0013;
		de.all = 0x00D8;
		hl.all = 0x014D;*/
		

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
	inline	void printREG()
	{
		printf("A:%02X F:%02X   z:%d  n:%d  h:%d  c:%d \n", af.a, af.all & 0xFF, af.f_z, af.f_n, af.f_h, af.f_c);
		printf("B:%02X C:%02X\n", bc.b, bc.c);
		printf("D:%02X E:%02X\n", de.d, de.e);
		printf("H:%02X L:%02X\n", hl.h, hl.l);
		printf("PC:%04X SP:%04X\n", pc, sp);
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
		for (int i = 0; i < rom_size; i++)
		{
			//fin.read((char *)(&memory[0] + CARTRIDGE_ROM_BASE + sizeof(U8DATA) * i), sizeof(char));

			fin.read((char *)(&one_byte), sizeof(one_byte));
			memory[i] = one_byte;
		}
		fin.close();

		printMEM(0, 64);

	}

	void outputBinary(std::string filename, int base, int size)
	{
		std::ofstream fout;
		U8  one_byte;


		fout.open(filename.c_str(), std::ios::binary);
		for (int i = base; i < (base + size); i++)
		{
			one_byte = memory[i];
			fout.write((char *)(&one_byte), sizeof(one_byte));
		}
		fout.close();

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

	inline int paletteCodeToColor(U8 palette_color_code)
	{
		U8  color_code;
		int color;
		U8  bg_palette_color = 0xE4;//memory[BG_PALETTE_DATA];
		U8  palette_code_to_color_code[4] = { (bg_palette_color >> 0) & 0x3,(bg_palette_color >> 2) & 0x3,(bg_palette_color >> 4) & 0x3,(bg_palette_color >> 6) & 0x3 };

		//palette color code -> color code
		color_code = palette_code_to_color_code[palette_color_code];

		//color code -> color


		switch (color_code)
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
	void buildTile(int y_pos, int x_pos, U16 dot_data_ofst)
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
		int bg_char_data_ofst = ((memory[LCD_CTRL_REG] & BG_CHAR_SEL_FALG) ? 0x8000 : 0x8800);
		U8 _break = false;
		U8 sixteen_byte[16];
		U16 dot_data_ofst;
		for (y_pos = BG_TILE_BGI_BUFFER_Y + 0; y_pos < (256 + BG_TILE_BGI_BUFFER_Y); y_pos += 8)
		{
			for (x_pos = BG_TILE_BGI_BUFFER_X + 0; x_pos < (BG_TILE_BGI_BUFFER_X + 256); x_pos += 8)
			{
				rectangle(x_pos, y_pos, x_pos + 8, y_pos + 8);
				//for (int i = 0; i < 16; i++) 
				//{
				//	sixteen_byte[i] = memory[VIDEO_RAM_BASE + video_ram_offset + i];
				//}
				buildTile(y_pos, x_pos, bg_char_data_ofst + video_ram_offset);


				video_ram_offset += 16;
				if (video_ram_offset >= BG_CHAR_DATA_SIZE)
				{
					_break = true;
				}
			}
			if (_break)
				break;
		}
		tile_data_built = TRUE;
	}
	//get tile from built tile data
	void getBackgroundTile(U8 chr_code, void *tile_ptr)
	{
		U8 tile_idx_x = chr_code & 0x1F; // *8 % 256
		U8 tile_idx_y = chr_code >> 5;   // *8 / 256
		getimage(BG_TILE_BGI_BUFFER_X + tile_idx_x * 8, BG_TILE_BGI_BUFFER_Y + tile_idx_y * 8, BG_TILE_BGI_BUFFER_X + (tile_idx_x + 1) * 8, BG_TILE_BGI_BUFFER_Y + (tile_idx_y + 1) * 8, tile_ptr);
	}
	//render 160 * 144 takes about 40ms 
	//takes about 79800 cpu cycles
	void buildBackground()
	{
		U8    chr_code; //BG MAP
		U16   bg_disp_data_addr = ((memory[LCD_CTRL_REG] & BG_CODE_SEL_FALG) ? 0x9C00 : 0x9800);

		for (int y = 0; y < 256; y += 8)
		{
			for (int x = 0; x < 256; x += 8)
			{
				chr_code = memory[bg_disp_data_addr++];
				//out of viewport
				//if ((x >= 160) || (y >= 144))
				//	continue;
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
		//if (((//cpu_cycles + 5)% 228) <= 10)
		if ((cpu_cycles & 0x7F) == 0)
		{
			//each horizontal line takes 512 cpu cycles
			memory[LCD_Y_COORD_REG] = (U8)(memory[LCD_Y_COORD_REG] + 1);
			if (((U8)memory[LCD_Y_COORD_REG]) == 154)
			{
				//this->refresh_lcd = TRUE;
				memory[LCD_Y_COORD_REG] = (U8)0x0;
			}
		}
	}
	void update_timer()
	{
		U16 input_clk_sel = memory[TAC] & 0x3;
		input_clk_sel = (input_clk_sel == 0) ? 4 : input_clk_sel; // 0 -> 4

		// 0b00 -> freq / 2^10
		// 0b01 -> freq / 2^4
		// 0b10 -> freq / 2^6
		// 0b11 -> freq / 2^8
		U16 timer_mod_mask = (0x1 << ((input_clk_sel + 1) * 2)) - 1;

		if ((cpu_cycles & timer_mod_mask) == 0)
		{
			memory[TIMA] = memory[TIMA] + 1;
			//printf("timer int cpu_cycles = %X, ", cpu_cycles);
			//printf(" memory[TIMA] = %X\n", (U8)(memory[TIMA]));
		}
	}
	void check_interrupt_and_dispatch_isr()
	{
		//
		//memory[INT_FLAGS_ADDR];
		//memory[INT_SWITCH_BASE];

		//not need to clear int flag, programmer would clear flag before using it
		//vertical blanking interupt ISR = 0x40
		if (memory[LCD_Y_COORD_REG] == 0)
		{
			//wake from halt_state
			halt_state = FALSE;
			//printf("\n");
			//set int flag
			memory[INT_FLAGS] = (memory[INT_FLAGS] | INT_FLAG_VERT_BLANKING);

			//trigger refresh lcd, only one time
			if ((cpu_cycles & 0x1FF) == 0)
				this->refresh_lcd = TRUE;

			if (ime && (memory[INT_SWITCH] & INT_FLAG_VERT_BLANKING))
			{
				ime = FALSE;
				memory[--sp] = pc >> 8;
				memory[--sp] = pc & 0xFF;
				pc = ISR_VERTICAL_BLANKING;
				//clear flag if the interrupt is served by isr
				memory[INT_SWITCH] = memory[INT_SWITCH] & (~INT_FLAG_VERT_BLANKING);
			}
		}

		//vertical blanking interupt ISR = 0x50
		if (memory[TIMA] == 0xFF)
		{
			//wake from halt_state
			halt_state = FALSE;

			//set int flag
			memory[INT_FLAGS] = (memory[INT_FLAGS] | INT_FLAG_TIMER);

			//When TIMA overflows, the TMA data is loaded into TIMA
			memory[TIMA] = memory[TMA];

			if (ime)
			{
				ime = FALSE;
				memory[--sp] = pc >> 8;
				memory[--sp] = pc & 0xFF;
				pc = ISR_TIMER;
				//clear flag if the interrupt is served by isr
				memory[INT_SWITCH] = memory[INT_SWITCH] & (~INT_FLAG_TIMER);


				//printf("goto isr = %X\n", pc);
			}
		}


	}
	void refreshLCD()
	{

		//GetSystemTime(&end_time);
		//printf("s: %d , ms : %d\n", begin_time.wSecond, begin_time.wMilliseconds);
		//printf("s: %d , ms : %d\n", end_time.wSecond, end_time.wMilliseconds);
		buildBackground();


		//viewport
		int scy = memory[SCY];
		int scx = memory[SCX];
		//getimage(scx, scy, scx + LCD_WIDTH, scy + LCD_HEIGHT, viewport_buf_ptr);
		getimage(scx, scy, scx + LCD_WIDTH, 0 + LCD_HEIGHT, viewport_buf_ptr);
		putimage(VIEWPORT_X, VIEWPORT_Y - 100, viewport_buf_ptr, COPY_PUT);
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
		U8 showpc = 0;
		U16 debug_pc = 0xc00a;
		//each loop takes about 0.0005 ms
		while (TRUE)
		{
			//STOP instruction takes the highest priority
			//if stop_state is met, do not do any action
			//if (stop_state == TRUE) 
			//{
			//	continue;
			//}
			cpu_cycles++;
			update_lcd_y_coord();
			update_timer();

			check_interrupt_and_dispatch_isr();

			if ((this->refresh_lcd == TRUE))
			{
				refresh_lcd = FALSE;
				refreshLCD();
			}

			//
			if (halt_state == TRUE)
			{
				//do nothing
				//printf(".");
				//cpu_cycles++;
			}
			else
			{
				opcode = memory[pc];
				xx = memory[pc + 1];
				aabb = (memory[pc + 2] << 8) | memory[pc + 1];
				b = (memory[pc + 1] >> 3) & 0x7;

				ly = memory[LCD_Y_COORD_REG];


				if (debug_pc == pc)
				{
					//printf("hit %0x4X\n", debug_pc);
				}

				if(showpc)
					printREG();
				switch (opcode)
				{
					//STOP
				case 0x10:
					pc += 2;
					break;

				case 0xC3:
					pc = aabb;
					//cpu_cycles += 4;
					//printf("jp %04X\n", pc);
					break;

				case 0xE9:
					pc = hl.all;
					//cpu_cycles += 1;
					//printf("jp %04X\n", pc);
					break;

				case 0xDA:
					pc += 3;
					pc = (af.f_c) ? (aabb) : (pc);
					//cpu_cycles += 3;
					//printf("jp c %04X\n", pc);
					break;

				case 0xD2:
					pc += 3;
					pc = (!af.f_c) ? (aabb) : (pc);
					//cpu_cycles += 3;
					//printf("jp nc %04X\n", pc);
					break;

				case 0xC2:
					pc += 3;
					pc = (!af.f_z) ? (aabb) : (pc);
					//cpu_cycles += 3;
					//printf("jp nz %04X\n", pc);
					break;

				case 0xCA:
					pc += 3;
					pc = (af.f_z) ? (aabb) : (pc);
					//cpu_cycles += 3;
					//printf("jp z %04X\n", pc);
					break;

				case 0x18:
					pc += 2;
					//printf("jr %04X\n", (char)xx + pc);
					//cpu_cycles += 3;
					pc += (char)xx;
					break;

				case 0x38:
					pc += 2;
					//printf("jr c %04X\n", pc + (char)xx);
					//cpu_cycles += 2;
					pc += (af.f_c) ? ((char)xx) : (0);
					break;

				case 0x30:
					pc += 2;
					//printf("jr nc %04X\n", pc + (char)xx);
					//cpu_cycles += 2;
					pc += (!af.f_c) ? ((char)xx) : (0);
					break;

				case 0x20:
					pc += 2;
					//printf("jr nz %04X\n", pc + (char)xx);
					//cpu_cycles += 2;
					pc += (!af.f_z) ? ((char)xx) : (0);
					break;

				case 0x28:
					pc += 2;
					//printf("jr z %04X\n", pc + (char)xx);
					//cpu_cycles += 2;
					pc += (af.f_z) ? ((char)xx) : (0);
					break;

					//CALL $aabb
				case 0xCD:
					pc += 3;
					memory[--sp] = pc >> 8;
					memory[--sp] = pc & 0xFF;
					pc = aabb;
					//cpu_cycles += 6;
					break;

					//CALL C,$aabb
				case 0xDC:
					pc += 3;
					if (af.f_c)
					{
						memory[--sp] = pc >> 8;
						memory[--sp] = pc & 0xFF;
						pc = aabb;
						//cpu_cycles += 6;
					}
					else
					{
						//cpu_cycles += 3;
					}
					break;

					//CALL NC,$aabb
				case 0xD4:
					pc += 3;
					if (!af.f_c)
					{
						memory[--sp] = pc >> 8;
						memory[--sp] = pc & 0xFF;
						pc = aabb;
						//cpu_cycles += 6;
					}
					else
					{
						//cpu_cycles += 3;
					}
					break;

					//CALL NZ,$aabb
				case 0xC4:
					pc += 3;
					if (!af.f_z)
					{
						memory[--sp] = pc >> 8;
						memory[--sp] = pc & 0xFF;
						pc = aabb;
						//cpu_cycles += 6;
					}
					else
					{
						//cpu_cycles += 3;
					}
					break;

					//CALL Z,$aabb
				case 0xCC:
					pc += 3;
					if (af.f_z)
					{
						memory[--sp] = pc >> 8;
						memory[--sp] = pc & 0xFF;
						pc = aabb;
						//cpu_cycles += 6;
					}
					else
					{
						//cpu_cycles += 3;
					}
					break;

				case 0x00:
					pc += 1;
					//cpu_cycles += 1;
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
					memory[--sp] = (pc >> 8);
					memory[--sp] = (pc & 0xFF);
					pc = idxToP((opcode >> 3) & 7);
					printf("rst %04X , sp = %04X\n", pc, sp);
					//cpu_cycles += 4;
					break;

					//ADD A.B..(HL)  to A
				case 0x80:
				case 0x81:
				case 0x82:
				case 0x83:
				case 0x84:
				case 0x85:
				case 0x86:
				case 0x87:
					//cpu_cycles += 1;
					//printf("add register to A\n", pc, sp);
					prev_value = af.a;
					af.a += (*idxToRegr_HL(opcode & 0x7));
					af.f_c = (prev_value > af.a) ? 1 : 0;
					af.f_h = ((prev_value & 0xF) > (af.a & 0xF)) ? 1 : 0;
					af.f_n = 0;
					af.f_z = (af.a == 0) ? 1 : 0;
					pc += 1;
					break;


					//ADD xx to A
				case 0xC6:
					//cpu_cycles += 2;
					//printf("add xx to A\n", pc, sp);
					af.f_c = ((af.a + xx) > 0xFF) ? 1 : 0;
					af.f_h = (((af.a & 0xF) + (xx & 0xF))  > 0xF) ? 1 : 0;
					af.a += xx;
					af.f_n = 0;
					af.f_z = (af.a == 0) ? 1 : 0;
					pc += 2;
					break;

					//add BC DE HL SP to HL
				case 0x09:
				case 0x19:
				case 0x29:
				case 0x39:
					//cpu_cycles += 2;
					//printf("add BC DE HL SP to HL\n", pc, sp);
					prev_value = hl.all;
					hl.all += (*idxToRegss((opcode & 0x30) >> 4));
					//af.f_z = (hl.all == 0) ? 1 : 0;
					af.f_n = 0;
					af.f_c = (prev_value > hl.all) ? 1 : 0;
					af.f_h = ((prev_value & 0xFFF) > (hl.all & 0xFFF)) ? 1 : 0;
					pc += 1;
					break;

					//add xx to sp    
				case 0xE8:
					//cpu_cycles += 4;
					//printf("add 0x%02X to 0x%04X\n", xx, sp);
					prev_value = sp;
					sp += (char)xx;
					af.f_n = 0;
					af.f_z = 0;
					if (((char)xx) > 0)
					{
						af.f_h = ((prev_value & 0xFFF) > (sp & 0xFFF)) ? 1 : 0;
						af.f_c = (prev_value > sp) ? 1 : 0;
					}
					else if (((char)xx) < 0)
					{
						af.f_h = ((prev_value & 0xFFF) < (sp & 0xFFF)) ? 1 : 0;
						af.f_c = (prev_value < sp) ? 1 : 0;
					}
					//printREG();
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
					//cpu_cycles += 2;
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
					//cpu_cycles += 2;
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
					//cpu_cycles += 1;
					pc += 1;
					(*idxToRegr_HL((opcode >> 3) & 0x7)) = (*idxToRegr_HL(opcode & 0x7));
					//printf("LD  %04X\n", (*idxToRegr_HL((opcode >> 3) & 0x7)));
					break;

					//load 16bit_reg with aabb
				case 0x01:
				case 0x11:
				case 0x21:
				case 0x31:
					//cpu_cycles += 3;
					pc += 3;
					(*idxToRegdd((opcode >> 4) & 0x3)) = aabb;
					//printf("LD  \n");
					break;


					//load A with (BC)
				case 0x0A:
					//cpu_cycles += 2;
					pc += 1;
					af.a = memory[bc.c];
					//printf("LD  \n");
					break;

					//load A with (DE)
				case 0x1A:
					//cpu_cycles += 2;
					pc += 1;
					af.a = memory[de.all];
					//printf("LD  \n");
					break;

					//LD HLI load A with (HL++)
				case 0x2A:
					//cpu_cycles += 2;
					pc += 1;
					af.a = memory[hl.all++];
					//printf("LDHLI  \n");
					break;

					//LD HLD load A with (HL--)
				case 0x3A:
					//cpu_cycles += 2;
					pc += 1;
					af.a = memory[hl.all--];
					//printf("LDHLD  \n");
					break;


					//LD HLI load (HL++) with A 
				case 0x22:
					//cpu_cycles += 2;
					pc += 1;
					memory[hl.all++] = af.a;
					//printf("LD HLI load (HL++) with A  \n");
					break;

					//LD HLD load(HL--) with A 
				case 0x32:
					//cpu_cycles += 2;
					pc += 1;
					memory[hl.all--] = af.a;
					//printf("LD HLD load(HL--) with A   \n");
					break;

					//load A with (aabb)
				case 0xFA:
					//cpu_cycles += 4;
					af.a = memory[aabb];
					//printf("LD mempry[aabb] = %04X  \n",memory[aabb]);
					pc += 3;
					break;

					//LDHL load HL with SP + e
				case 0xF8:
					//cpu_cycles += 2;
					prev_value = sp;
					hl.all = sp + (char)xx;
					af.f_n = 0;
					af.f_z = 0;
					if (((char)xx) > 0)
					{
						af.f_h = ((prev_value & 0xFFF) > (hl.all & 0xFFF)) ? 1 : 0;
						af.f_c = (prev_value > hl.all) ? 1 : 0;
					}
					else if (((char)xx) < 0)
					{
						af.f_h = ((prev_value & 0xFFF) > (hl.all & 0xFFF)) ? 1 : 0;
						af.f_c = (prev_value < hl.all) ? 1 : 0;
					}
					pc += 2;
					//printf("load HL with SP  \n");
					break;

					//load SP with HL
				case 0xF9:
					//cpu_cycles += 2;
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
					//cpu_cycles += 2;
					//printf("load (HL) with reg  \n");
					pc += 1;
					memory[hl.all] = (*idxToRegr_HL(opcode & 0x7));
					break;

					//load (aabb) with A
				case 0xEA:
					//cpu_cycles += 4;
					memory[aabb] = af.a;
					pc += 3;
					//printf("load (aabb) with A  \n");
					break;

					//load (aabb) with sp
				case 0x08:
					//cpu_cycles += 3;
					//printf("load (aabb) with sp  \n");
					memory[aabb] = sp & 0xFF;
					memory[aabb + 1] = (sp >> 8) & 0xFF;
					pc += 3;
					break;

					//load (0xFF00 + xx) with A
				case 0xE0:
					//cpu_cycles += 3;
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
					//cpu_cycles += 2;
					//printf("load (C) with A  \n");
					memory[PERIPHERAL_BASE + bc.c] = af.a;
					pc += 1;
					break;

					//load A with (0xFF00 + xx)
				case 0xF0:
					//cpu_cycles += 3;
					pc += 2;
					af.a = memory[PERIPHERAL_BASE + xx];
					//printf("LD	\n");
					break;

					//load A with (0xFF00 + C)
				case 0xF2:
					//cpu_cycles += 2;
					pc += 1;
					af.a = memory[PERIPHERAL_BASE + bc.c];
					//printf("LD	\n");
					break;

					//load (BC) with A
				case 0x02:
					//printf("load (BC) with A  \n");
					memory[bc.all] = af.a;
					pc += 1;
					//cpu_cycles += 2;
					break;

					//load (DE) with A
				case 0x12:
					//cpu_cycles += 2;
					//printf("load (DE) with A  \n");
					memory[de.all] = af.a;
					pc += 1;
					break;

					//POP AF,BC,DE,HL
				case 0xF1:
				case 0xC1:
				case 0xD1:
				case 0xE1:
					//cpu_cycles += 3;
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
					//cpu_cycles += 3;
					memory[--sp] = (*idxToRegqq((opcode >> 4) & 0x3)) >> 8;
					memory[--sp] = (*idxToRegqq((opcode >> 4) & 0x3)) & 0xFF;
					pc += 1;
					//printf("push  %04X\n", sp);
					break;

					//ADC  add A,B..(HL) + flag_carry to A
				case 0x88:
				case 0x89:
				case 0x8A:
				case 0x8B:
				case 0x8C:
				case 0x8D:
				case 0x8E:
				case 0x8F:
					//cpu_cycles += 1;
					prev_value = af.a;
					af.a = af.a + (*idxToRegr_HL(opcode & 0x7)) + af.f_c;
					af.f_c = (prev_value >= af.a) ? 1 : 0;
					af.f_h = ((prev_value & 0xF) >= (af.a & 0xF)) ? 1 : 0;
					af.f_n = 0;
					af.f_z = (af.a == 0) ? 1 : 0;
					pc += 1;
					//printf("ADC  %04X\n", af.a);
					break;

					//ADC add xx + flag_carry to A
				case 0xCE:
					//cpu_cycles += 1;
					prev_value = af.a;
					af.a += (xx + af.f_c);
					pc += 2;
					af.f_c = (prev_value >= af.a) ? 1 : 0;
					af.f_h = ((prev_value & 0xF) >= (af.a & 0xF)) ? 1 : 0;
					af.f_n = 0;
					af.f_z = (af.a == 0) ? 1 : 0;
					//printf("ADC  %04X\n", af.a);
					break;

					//CCF clear carry flag
				case 0x3F:
					//cpu_cycles += 1;
					af.f_c = !af.f_c;
					af.f_h = 0;
					af.f_n = 0;
					pc += 1;
					//printf("CCF  %04X\n", af.all);
					break;

					//SCF set carry flag
				case 0x37:
					//cpu_cycles += 1;
					af.f_c = 1;
					af.f_h = 0;
					af.f_n = 0;
					pc += 1;
					//printf("SCF  %04X\n", af.all);
					break;

					//AND xx
				case 0xE6:
					//cpu_cycles += 1;
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
					//cpu_cycles += 1;
					af.a &= (*idxToRegr_HL(opcode & 0x7));
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
					//cpu_cycles += 1;
					pc += 1;
					af.f_c = (*idxToRegr_HL(opcode & 0x7) > af.a) ? 1 : 0;
					af.f_h = ((*idxToRegr_HL(opcode & 0x7) & 0xF) > (af.a & 0xF)) ? 1 : 0;
					af.f_n = 1;
					af.f_z = (*idxToRegr_HL(opcode & 0x7) == af.a) ? 1 : 0;
					//printf("CP  %04X\n", af.a);
					break;

					//CP compare A xx	
				case 0xFE:
					//cpu_cycles += 1;
					af.f_c = (xx > af.a) ? 1 : 0;
					af.f_h = ((xx & 0xF) > (af.a & 0xF)) ? 1 : 0;
					af.f_n = 1;
					af.f_z = (af.a == xx) ? 1 : 0;
					pc += 2;
					//printf("CP  %04X\n", xx);
					break;

					//CPL complement A	
				case 0x2F:
					//cpu_cycles += 1;
					af.f_h = 1;
					af.f_n = 1;
					af.a = ~af.a;
					pc += 1;
					//printf("CPL  %04X\n", af.a);
					break;

					//DAA decimal adjust A
				case 0x27:
					//subtraction
					if (af.f_n)
					{
						if (af.f_c) { af.a -= 0x60; }
						if (af.f_h) { af.a -= 0x06; }
					}//addition
					else
					{
						if (af.f_c || (af.a & 0xFF) > 0x99) { af.a += 0x60; af.f_c = 1; }
						if (af.f_h || (af.a & 0x0F) > 0x09) { af.a += 0x06; }
					}

					//printf("a_decimal = %03d\n", a_decimal);
					//printf("af.a      = %03X\n", af.a);
					af.f_z = (af.a == 0) ? 1 : 0;
					af.f_h = 0;
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
					//cpu_cycles += 1;
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
					//cpu_cycles += 2;
					(*idxToRegss((opcode >> 4) & 0x3))--;
					pc += 1;
					//printf("DEC  %04X\n", (*idxToRegss((opcode >> 4) & 0x3)));
					break;

					//DI disable interrupt
				case 0xF3:
					//cpu_cycles += 1;
					pc += 1;
					//printf("pc = 0x%X ", pc);
					ime = false;
					//printf("DI //cpu_cycles = %08X\n", //cpu_cycles);
					break;

					//EI enable interrupt
				case 0xFB:
					//cpu_cycles += 1;
					//printf("pc = 0x%X ", pc);
					pc += 1;
					ime = true;
					//printf("EI //cpu_cycles = %08X\n", //cpu_cycles);
					break;

					//halt
					//if IME is true -> go to isr upon interrupt occuring
					//if IME is false -> starts from pc
				case 0x76:
					//cpu_cycles += 1;
					halt_state = TRUE;
					//outputBinary("videoram.bin", 0x8000, 0x2000);
					pc += 1;
					//printf("//cpu_cycles = %08X ,halt_state = TRUE\n", //cpu_cycles);
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
					//cpu_cycles += 2;
					prev_value = (*idxToRegr_HL((opcode >> 3) & 0x7));
					(*idxToRegr_HL((opcode >> 3) & 0x7))++;
					reg_value = (*idxToRegr_HL((opcode >> 3) & 0x7));
					af.f_h = ((prev_value & 0xF) > (reg_value & 0xF)) ? 1 : 0;
					af.f_n = 0;
					af.f_z = (reg_value == 0) ? 1 : 0;
					pc += 1;
					if (opcode == 0x34 && hl.all == 0xFFE2)
					{
						//printf("//cpu_cycles :%08X , pc %02X : INC  %04X\n", //cpu_cycles , pc,(*idxToRegr_HL((opcode >> 3) & 0x7)));
					}
					break;

					//INC increment BC..HL by 1 	
				case 0x03:
				case 0x13:
				case 0x23:
				case 0x33:
					//cpu_cycles += 2;
					(*idxToRegss((opcode >> 4) & 0x3))++;
					pc += 1;
					//printf("INC  %04X\n", (*idxToRegss((opcode >> 4) & 0x3)));
					break;

					//OR A with xx
				case 0xF6:
					//cpu_cycles += 1;
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
					//cpu_cycles += 1;
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
					//cpu_cycles += 2;
					if (xx <= 0x07)
					{
						af.f_c = ((*idxToRegr_HL(xx & 0x7)) & 0x80) ? 1 : 0;
						(*idxToRegr_HL(xx & 0x7)) <<= 1;
						(*idxToRegr_HL(xx & 0x7)) = (*idxToRegr_HL(xx & 0x7)) | af.f_c;
						af.f_h = 0;
						af.f_n = 0;
						af.f_z = ((*idxToRegr_HL(xx & 0x7)) == 0) ? 1 : 0;
						//printf("RLC = %04X\n", (*idxToRegr_HL(xx & 0x7)));
					}
					//RRC
					else if ((0x08 <= xx) && (xx <= 0x0F))
					{
						af.f_c = (*idxToRegr_HL(xx & 0x7)) & 0x01 ? 1 : 0;
						(*idxToRegr_HL(xx & 0x7)) >>= 1;
						(*idxToRegr_HL(xx & 0x7)) = (af.f_c == 1) ? ((*idxToRegr_HL(xx & 0x7)) | BIT(7)) : (*idxToRegr_HL(xx & 0x7));
						af.f_h = 0;
						af.f_n = 0;
						af.f_z = ((*idxToRegr_HL(xx & 0x7)) == 0) ? 1 : 0;
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
						(*idxToRegr_HL(xx & 0x7)) |= (prev_value << 7);
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
						while (1);
					}
					pc += 2;
					break;

					//RET NZ  
				case 0xC0:
					if (!af.f_z)
					{
						//cpu_cycles += 4;
						pc = (memory[sp + 1] << 8) | memory[sp];
						sp += 2;
					}
					else
					{
						//cpu_cycles += 2;
						pc += 1;
					}
					//printf("RET NZ = %04X\n", pc);
					break;

					//RET Z   
				case 0xC8:
					if (af.f_z)
					{
						//cpu_cycles += 4;
						pc = (memory[sp + 1] << 8) | memory[sp];
						sp += 2;
					}
					else
					{
						//cpu_cycles += 2;
						pc += 1;
					}
					//printf("RET Z = %04X\n", pc);
					break;

					//RET NC  
				case 0xD0:
					if (!af.f_c)
					{
						//cpu_cycles += 4;
						pc = (memory[sp + 1] << 8) | memory[sp];
						sp += 2;
					}
					else
					{
						//cpu_cycles += 2;
						pc += 1;
					}
					//printf("RET NC pc = %04X\n", pc);
					break;

					//RET C   
				case 0xD8:
					if (af.f_c)
					{
						//cpu_cycles += 4;
						pc = (memory[sp + 1] << 8) | memory[sp];
						sp += 2;
					}
					else
					{
						//cpu_cycles += 2;
						pc += 1;
					}
					//printf("RET C pc = %04X\n", pc);
					break;

					//RET	  
				case 0xC9:
					//cpu_cycles += 4;
					pc = (memory[sp + 1] << 8) | memory[sp];
					sp += 2;
					//printf("RET pc = %04X\n", pc);
					//auto disable ime
					//this->ime = 0;
					break;

					//RETI	  
				case 0xD9:
					//cpu_cycles += 4;
					//printf("RETI \r\n");
					pc = (memory[sp + 1] << 8) | memory[sp];
					sp += 2;
					//auto enable ime
					this->ime = 1;
					break;

					//RLA
				case 0x17:
					//cpu_cycles += 1;
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
					//cpu_cycles += 1;
					pc += 1;
					af.f_c = af.a & 0x80 ? 1 : 0;
					af.f_h = 0;
					af.f_n = 0;
					af.f_z = 0;
					af.a = af.a << 1;
					af.a = af.a | af.f_c;
					//printf("RLCA af.a << 1 = %04X\n", af.a);
					break;

					//RRA
				case 0x1F:
					//cpu_cycles += 1;
					pc += 1;
					prev_value = af.f_c;
					af.f_c = af.a & 0x01 ? 1 : 0;
					af.f_h = 0;
					af.f_n = 0;
					af.f_z = 0;
					af.a >>= 1;
					af.a |= (prev_value << 7);
					//printf("RRA af.a >> 1 = %04X\n", af.a);
					break;

					//RRCA
				case 0x0F:
					//cpu_cycles += 1;
					pc += 1;
					af.f_c = af.a & 0x01 ? 1 : 0;
					af.f_h = 0;
					af.f_n = 0;
					af.f_z = 0;
					af.a >>= 1;
					//if bit0==1 , bit7 should be set
					af.a = (af.f_c) ? (af.a | BIT(7)) : af.a;
					//printf("RRCA af.a >> 1 = %04X\n", af.a);
					break;

					//SBC subtract xx+cy from A
				case 0xDE:
					//cpu_cycles += 1;
					//printf("%04X sub %04X cy: %04X\n", af.a, xx, af.f_c);
					pc += 2;
					prev_value = af.a;
					af.a = af.a - af.f_c - xx;
					af.f_c = (prev_value >= af.a) ? 1 : 0;
					af.f_h = ((prev_value & 0xF) >= (af.a & 0xF)) ? 1 : 0;
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
					//cpu_cycles += 1;
					//printf("%04X sub %04X cy: %04X\n", af.a, (*idxToRegr_HL(opcode & 0x7)), af.f_c);
					pc += 2;
					prev_value = af.a;
					af.a = af.a - (*idxToRegr_HL(opcode & 0x7)) - af.f_c ;					
					af.f_h = ((af.a & 0xF) >= (prev_value & 0xF)) ? 1 : 0;
					af.f_c = (af.a >= prev_value) ? 1 : 0;
					af.f_n = 1;
					af.f_z = (af.a == 0) ? 1 : 0;
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
					//cpu_cycles += 1;
					//printf("%04X sub %04X\n", af.a, (*idxToRegr_HL(opcode & 0x7)));
					pc += 1;
					af.f_c = ((*idxToRegr_HL(opcode & 0x7)) > af.a) ? 1 : 0;
					af.f_h = (((*idxToRegr_HL(opcode & 0x7)) & 0xF) > (af.a & 0xF)) ? 1 : 0;
					af.a = af.a - (*idxToRegr_HL(opcode & 0x7));
					af.f_z = (af.a == 0) ? 1 : 0;
					af.f_n = 1;
					break;

					//SUB xx
				case 0xD6:
					//cpu_cycles += 1;
					//printf("%04X sub %04X\n", af.a, (*idxToRegr_HL(opcode & 0x7)));
					pc += 2;
					af.f_c = (xx > af.a) ? 1 : 0;
					af.f_h = ((xx & 0xF) > (af.a & 0xF)) ? 1 : 0;
					af.a -= xx;
					af.f_z = (af.a == 0) ? 1 : 0;
					af.f_n = 1;
					break;

					//XOR
				case 0xEE:
					pc += 2;					
					af.a = xx ^ af.a;
					af.f_z = (af.a == 0) ? 1 : 0;
					af.f_n = 0;
					af.f_c = 0;
					af.f_h = 0;
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
					//cpu_cycles += 1;
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

				//reserved field in af.f should be zero, in case previous code changed the value, we need to clear it  after every cycle
				af.all &= 0xFFF0;

				//getchar();
			}


		}
	}



};



U8DATA &U8DATA::operator=(U8 val)
{
	U8 _hang = true;
	switch (this->access_addr)
	{
		//case 0xFF00:
	case 0xFF70:
	case 0xFF01:
	case 0xFF02:
	case 0xFF04:
		break;
	//case TIMA:		//0xFF05
	//case TMA:		//0xFF06
	case TAC:		//0xFF07
		//timer emable
		break;
		//case 0xFF0F:
	case 0xFF40:
	case 0xFF41:
	case 0xFF42:
	case 0xFF43:
	case 0xFF45:
	case 0xFF46:
	case 0xFF47:
	case 0xFF48:
	case 0xFF49:
	case 0xFF4A:
	case 0xFF4B:
	case 0xFE00:
	case 0xFE01:
	case 0xFE02:
	case 0xFE03:
		//case 0xFFFF:
		//printf("pc= %04X ,assign operator @ %04X\n", *pc, *access_addr);

		break;
	}

	U16 bg_tile_ram_end_eddr = ((cpu->memory[LCD_CTRL_REG] & BG_CHAR_SEL_FALG) ? 0x8FFF : 0x97FF);
	if (((VIDEO_RAM_BASE <= access_addr) && (access_addr <= bg_tile_ram_end_eddr)))
		//if (((VIDEO_RAM_BASE <= access_addr) && (access_addr <= (VIDEO_RAM_BASE + 0x1000))))
	{
		//we need to collect two byte before painting one row
		if ((access_addr & 0x1) == 0x1)
		{
			if (val)
			{
				//printf("access_addr = %04X\n", access_addr);
				//printf("%02X %02X\n", (U8)(cpu->memory[access_addr - 1]), val);
			}
			tile_dot_data_painter->paintTileDotData(access_addr, (U8)(cpu->memory[access_addr - 1]), val);
		}
	}


	if (access_addr == 0xFF00)
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
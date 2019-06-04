#pragma once
#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <map>


typedef unsigned char  U8;
typedef unsigned short U16;

#define BIT(x)                 (0x1 << (x))

#define CARTRIDGE_ROM_BASE     (0x0000)
#define MBC1_REGISTER_1        (0x2000)
#define ROM_BANK1_START        (0x4000)
#define ROM_END                (0x7FFF)
#define ROM_IN_Z80_SIZE        (0x8000)
#define VIDEO_RAM_BASE         (0x8000)
#define EXTERNAL_RAM_BASE      (0xA000)
#define WORK_RAM_BASE          (0xC000)
#define OAM                    (0xFE00)
#define PERIPHERAL_BASE        (0xFF00)
#define JOY_PAD                (0xFF00)
#define     JOY_PAD_RESET          (BIT(4)|BIT(5))
#define     JOY_PAD_SEL_BUTTON     BIT(4)
#define     JOY_PAD_SEL_DIRECT     BIT(5)
#define DIV                    (0xFF04)    //DIV ,frequently used as random generator
#define TIMA                   (0xFF05)    //timer counter
#define TMA                    (0xFF06)    //timer modulo
#define TAC                    (0xFF07)   // timer control
#define     TAC_START                  BIT(2) //timer start
#define LCD_CTRL_REG           (0xFF40) //LCDC
#define     BG_CODE_SEL_FALG       BIT(3)   //BG Code Area Selection Flag , tile index
#define     BG_CHAR_SEL_FALG       BIT(4)   //BG char data Selection Flag , tile data
#define     BG_WIN_ON_FALG         BIT(5)   //window on Flag
#define     BG_WIN_VODE_FALG       BIT(6)   //window Code Area Selection Flag
#define LCD_STAT               (0xFF41) //STAT
#define     LCD_MATCH_FLAG         BIT(2)//LY == LYC
#define     LCD_INT_H_BLNK         BIT(3)
#define     LCD_INT_V_BLNK         BIT(4)
#define     LCD_INT_OAM            BIT(5)
#define     LCD_INT_LYC_LY         BIT(6)
#define SCY                    (0xFF42) //lcd scroll y
#define SCX                    (0xFF43) //lcd scroll x
#define LY                     (0xFF44) //LY lcd control y coordinate
#define LYC                    (0xFF45) //LY compare
#define DMA                    (0xFF46) //DMA
#define BG_PALETTE_DATA        (0xFF47) //BGP
#define INT_FLAGS              (0xFF0F)
#define     INT_FLAG_VERT_BLANKING BIT(0)
#define     INT_FLAG_LCDC          BIT(1)
#define     INT_FLAG_TIMER         BIT(2)
#define     INT_FLAG_SERIEAL_TRANS BIT(3)
#define     INT_FLAG_P10_P13       BIT(4)
#define HIGH_WORK_RAM_BASE     (0xFF80)
#define STACK_BEGIN_ADDR       (0xFFFE)
#define INT_SWITCH             (0xFFFF)

#define ROM_BANK_SWITCHING     (0x10000)


#define BG_CHAR_DATA_SIZE      (0x1000)
#define BG_CODE_DATA_SIZE      (0x400)


#define OAM_MEM_SIZE           (0xA0)
#define OAM_COUNT              (40)
#define OAM_X_OFFSET           (-8)
#define OAM_Y_OFFSET           (-16)

#define MEMORY_SIZE            (0x10000)
#define SPARE_RAW_SIZE         (0x10000)    //spare space for bank switching
#define ROM_BANK_SIZE          (0x4000)

#define SCREEN_OFFSET          (10)
#define LCD_WIDTH              (160)
#define LCD_HEIGHT             (144)
#define BG_WIDTH               (256)
#define BG_HEIGHT              (256)
#define VIEWPORT_X             (390)
#define VIEWPORT_Y             (0)
#define TWO_PASS_RENDER_X      (0)
#define TWO_PASS_RENDER_Y      (512)
#define PAGE_NUM               (2)

#define DEBUG_WINDOW_WIDTH     (200)
#define BG_TILE_BGI_BUFFER_X   (256)
#define BG_TILE_BGI_BUFFER_Y   (0)

#define FLIP_TILE_BUFFER_X     (270)  
#define FLIP_TILE_BUFFER_Y     (200)   

#define BG_X                   (0)
#define BG_Y                   (0)

#define TILE_SIZE              (8)
#define TILE_MEM_SIZE          (0x1800)

#define ISR_VERTICAL_BLANKING  0x40
#define ISR_LCDC               0x48
#define ISR_TIMER              0x50
#define ISR_SERIEAL_TRANS_CPL  0x58
#define ISR_P10_P10_INPUT      0x60

#define ENABLE_DOUBLE_GRAPHIC_BUFFER (1)
#define ENABLE_DEBUG_RECTANGLE       (0)

#define ASSERT(ERR_MSG,X)      if(!(X))\
                               {\
                                   printf("%s\n",ERR_MSG);\
                                   while (1) {}\
                               }

    

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
    static DMGZ80CPU *cpu;

    static int TILE_DOT_DATA_PAINTER::paletteCodeToColor(U8 palette_color_code, U16 which_palette);
    //paint 1 row, 8 dots
    //in BG_TILE_BGI_BUFFER ,each row accommodates 16 tile (128 dots)
    static void paintTileDotData(U16 dot_data_byte_idx, U8 upper_dot_byte, U8 lower_dot_byte)
    {
        U8 dots_one_row = 0x7F;   //128 dots in a row
        U8 dots_one_row_shift = 0x7;    //2^7 = 128 dots in a row
                                        // ---0---  ---8---       --------   --------
                                        // ---1---  ---9---       --------   --------
                                        // ---2---  --10---       --------   --------
                                        // ---3---  --11---       --------   --------
                                        // ---4---  -+12---       --------   --------
                                        // ---5---  --13---       --------   --------
                                        // ---6---  --14---       --------   --------
                                        // ---7---  --15---   ~   --------   ---127--
                                        //             ^-- dot_data_idx = 12      ^---- dot_data_idx = 127, 16 tiles 
                                        //
                                        // 12 / 7 = 1     -> x_pos = 1*8
                                        // 12 / 128 = 0   -> y_tile_row = 0*8
                                        // (12 % 8) = 4 -> y_pos = y_row + 4

        dot_data_byte_idx = dot_data_byte_idx - VIDEO_RAM_BASE;
        //dot_data_idx = (dot_data_byte_idx >> 1) , because two bytes represent one row
        U16 dot_data_idx = (dot_data_byte_idx >> 1);
        //each tile contains 64 dots
        // 12 / 7 = 1     -> x_pos = 1*8
        int x_pos = BG_TILE_BGI_BUFFER_X + (((dot_data_idx & dots_one_row) >> 3) << 3);
        // 12 / 128 = 0   -> y_tile_row = 0*8
        int y_tile_row = (dot_data_idx >> dots_one_row_shift) << 3;
        // (12 % 8) = 4 -> y_pos = y_row + 4
        int y_pos = BG_TILE_BGI_BUFFER_Y + y_tile_row + ((dot_data_idx) & 0x7);
        int bgi_color;
        U8  color_code = 0;

        //render one row of a tile
        for (int x = x_pos; x < (x_pos + 8); x++)
        {
#if (ENABLE_DOUBLE_GRAPHIC_BUFFER == 1)
            for(int page = 0; page < PAGE_NUM; page++)
            {
                setactivepage(page);
#endif
                color_code = 0;
                color_code += (upper_dot_byte & BIT(7 - (x & 0x7))) ? 2 : 0;
                color_code += (lower_dot_byte & BIT(7 - (x & 0x7))) ? 1 : 0;
                bgi_color = paletteCodeToColor(color_code, BG_PALETTE_DATA);
                putpixel(x, y_pos, bgi_color);

#if (ENABLE_DOUBLE_GRAPHIC_BUFFER == 1)
            }
#endif
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
    U16 access_addr;
    U8 *raw_byte_ptr;
    U8DATA()
    {
    }
    U8DATA(U8 &val)
    {
        //this->value = val;
        *(this->raw_byte_ptr) = val;
    }
    operator U8()
    {
        //if (this->access_addr == TIMA)
        //    printf("cast operator @ %04X\n", this->access_addr);

        if (access_addr == 0xFF44)
        {
            //while (_hang);
            //this->value = 0x94;
        }
        else if (access_addr == 0xFF00)
        {
            if (((*(this->raw_byte_ptr)) & JOY_PAD_RESET) == JOY_PAD_RESET)
                //if ((this->value & JOY_PAD_RESET) == JOY_PAD_RESET)
            {
                //this->value = 0xFF;
                *(this->raw_byte_ptr) = 0xFF;
            }
            else if ((*(this->raw_byte_ptr)) & JOY_PAD_SEL_DIRECT)
                //else if (this->value & JOY_PAD_SEL_DIRECT)
            {
                if (GetKeyState(VK_RIGHT) < 0)
                {
                    //this->value &= (~BIT(0));
                    (*(this->raw_byte_ptr)) &= (~BIT(0));
                }
                if (GetKeyState(VK_LEFT) < 0)
                {
                    //this->value &= (~BIT(1));
                    (*(this->raw_byte_ptr)) &= (~BIT(1));
                }
                if (GetKeyState(VK_UP) < 0)
                {
                    //this->value &= (~BIT(2));
                    (*(this->raw_byte_ptr)) &= (~BIT(2));
                }
                if (GetKeyState(VK_DOWN) < 0)
                {
                    //this->value &= (~BIT(3));
                    (*(this->raw_byte_ptr)) &= (~BIT(3));
                }
            }
            else if ((*(this->raw_byte_ptr)) & JOY_PAD_SEL_BUTTON)
                //else if (this->value & JOY_PAD_SEL_BUTTON)
            {
                if (GetKeyState('A') < 0)
                {
                    //this->value &= (~BIT(0));
                    (*(this->raw_byte_ptr)) &= (~BIT(0));
                }
                if (GetKeyState('S') < 0)
                {
                    //this->value &= (~BIT(1));
                    (*(this->raw_byte_ptr)) &= (~BIT(1));
                }
                if (GetKeyState(VK_SHIFT) < 0)
                {
                    //this->value &= (~BIT(2));
                    (*(this->raw_byte_ptr)) &= (~BIT(2));
                }
                if (GetKeyState(VK_RETURN) < 0)
                {
                    //this->value &= (~BIT(3));
                    (*(this->raw_byte_ptr)) &= (~BIT(3));
                }
            }
            else
            {
                //printf("unknown joy sel %04X\n", this->value);
                //while (_hang);
            }

        }
        //return this->value;
        return (*(this->raw_byte_ptr));
    }
    
    U8* operator &()
    {
        if(this->access_addr == TIMA)
            printf("get address operator @ %04X\n" ,this->access_addr);
        //return &this->value;
        return &(*(this->raw_byte_ptr));
    }
    
    U8DATA &operator=(U8DATA val) 
    {
        //only copy data
        *this->raw_byte_ptr = *val.raw_byte_ptr;
        return (*this);
    }
    U8DATA &operator=(U8 val);
};

U16                    *U8DATA::pc;
DMGZ80CPU              *U8DATA::cpu;
TILE_DOT_DATA_PAINTER  *U8DATA::tile_dot_data_painter;
DMGZ80CPU              *TILE_DOT_DATA_PAINTER::cpu;

//memory bank controller
typedef struct mbc1_struct
{
    U16 register0;
    //rom bank code, write addr : 2000h~3FFFh, write data : 01h~1Fh
    U16 register1;
    //Upper ROM bank code when using 8 Mbits or more of ROM (and register 3is 0)
    //The upper ROM banks can be selected in 512-Kbyte increments.
    //  Write value of 0 selects banks 01h - 1Fh
    //    Write value of 1 selects banks 21h - 3Fh
    //    Write value of 2 selects banks 41h - 5Fh
    //    Write value of 3 selects banks 61h - 7Fh
    U16 register2;
    //ROM / RAM change
    //  Write addresses : 6000h - 7FFFh Write Data : 0 - 1
    //    Writing 0 causes the register 2 output to control switching of the higher ROM
    //    bank.
    //    Writing 1 causes the register 2 output to control switching of the RAM bank
    U16 register3;
}MBC1;

class DEBUG_MEM
{
public:

    //U16 access_addr;
    U8DATA  data[MEMORY_SIZE];
    U8      raw_byte_data[MEMORY_SIZE + SPARE_RAW_SIZE];
    U8DATA  *stack;
    //memory bank controller
    MBC1    mbc1;
    DEBUG_MEM()
    {
        stack = data + 0xCFF0;
        //access_addr = 0;
        memset((void*)raw_byte_data, 0x00, sizeof(U8) * MEMORY_SIZE);
        //point to each raw byte
        for (int i = 0; i < MEMORY_SIZE; i++)
        {
            data[i].raw_byte_ptr = &raw_byte_data[i];
        }

    }
    U8DATA &operator[](U16 addr)
    {
        //access_addr = addr;
        this->data[addr].access_addr = addr;
        return this->data[addr];
    }
};

typedef struct oam_entry_struct
{
    U8 pos_y;
    U8 pos_x;
    U8 tile_no; //charactor code
    U8 rsv : 4;
    U8 palette : 1;
    U8 flip_y_flip_x : 2;
    U8 priority : 1; //0 : priority to sprite
}OAM_ENTRY;


typedef struct debug_log
{
    U16 pc;
    U8  tma;
    U8 opcode;
    U16 sp;
    AF af;
    BC bc;
    DE de;
    HL hl;
    U8 xx;
    U16 aabb;
}DEBUGLOG;

class DMGZ80CPU
{
public:
    //debug
    DEBUGLOG log[8192];

    DEBUGLOG current_log;

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
    void*            flip_tile_buf_ptr;
    void*            viewport_buf_ptr;
    SYSTEMTIME       begin_time;
    SYSTEMTIME       end_time;
    U8               refresh_lcd;
    unsigned int     cpu_cycles;
    U8               tile_data_built;


    int log_idx;

    TILE_DOT_DATA_PAINTER tile_dot_data_painter;
    DEBUG_MEM memory;
    U16       pc;
    U16       debug_pc;
    std::map<U8, U8>  ly_scx_map;
    U8   ly_map[3];
    U8   scx_map[3];
    U8   fixed_ly;
    U8   page;  //two page 0 and 1
    enum      filp_mode { FLIP_NONE, FLIP_HORIZONTAL, FLIP_VERTICAL, FLIP_HORIZONTAL_VERTICAL };
    DMGZ80CPU()
    {
        U8DATA::tile_dot_data_painter = &this->tile_dot_data_painter;
        TILE_DOT_DATA_PAINTER::cpu = this;
        U8DATA::pc       = &this->pc;
        U8DATA::cpu      = this;
        page = 0;

        ime = TRUE;

        //boot rom
        //sp = STACK_BEGIN_ADDR;
        //pc = 0x0;
        //af.all = 0x0000;
        //bc.all = 0x0000;
        //de.all = 0x0000;
        //hl.all = 0x0000;

        flip_tile_buf_ptr = malloc(imagesize(0, 0, TILE_SIZE - 1, TILE_SIZE - 1));
        tile_buf_ptr = malloc(imagesize(0, 0, TILE_SIZE - 1, TILE_SIZE - 1));
        viewport_buf_ptr = malloc(imagesize(0, 0, LCD_WIDTH, LCD_HEIGHT));
        halt_state = FALSE;
        cpu_cycles = 0;
        refresh_lcd = FALSE;

        log_idx = 0;

        /*
        sp = STACK_BEGIN_ADDR;
        pc = 0x100;
        af.all = 0x1100;
        bc.all = 0x0000;
        de.all = 0x0000;
        hl.all = 0x0000;
        ime = TRUE;*/


        //main program start
        //tetris rom initial state
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
        //memset((void*)(&memory[0]), 0x00, 0x10000 * sizeof(U8DATA));
    }
    inline void printREG()
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
    BOOL detectMouseClick()
    {
        BOOL clicked = FALSE;
        if (ismouseclick(WM_LBUTTONDOWN))
        {
            clearmouseclick(WM_LBUTTONDOWN);
            clicked = TRUE;
        }
        return clicked;
    }
    void readROM(std::string filename)
    {
        std::ifstream fin;
        int cartridge_rom_size;
        U8  one_byte;


        fin.open(filename.c_str(), std::ios::ate | std::ios::binary);
        //get rom size
        cartridge_rom_size = fin.tellg();


        //move pointer to beginning of rom
        fin.seekg(0, fin.beg);
        //bulk copy to Z80 ram , if cartridge_rom_size was larger than 32k ,copy only 32k to Z80 RAM
        fin.read((char *)(memory[0].raw_byte_ptr + CARTRIDGE_ROM_BASE), (cartridge_rom_size > ROM_IN_Z80_SIZE) ? ROM_IN_Z80_SIZE : cartridge_rom_size);


        //mario rom         |-bank0-|-bank1-|-bank2-|-bank3-| 64k
        //z80 ram           |-bank0-|-bank1-|---------------| 64k
        //                                  ^32k
        //spare_rom         |-bank1-|-bank2-|-bank3-|-bank4-| 64k
        //back up bank1~bank3 to spare rom
        //move pointer to beginning of bank1
        fin.seekg(ROM_BANK_SIZE, fin.beg);
        //store remaining rom data at 0x10000
        fin.read((char *)(memory[0].raw_byte_ptr + ROM_BANK_SWITCHING), cartridge_rom_size - ROM_BANK_SIZE);




        //move pointer to beginning of rom
        /*fin.seekg(0, fin.beg);

        for (int i = 0; i < rom_size; i++)
        {
        //assign one to one
        fin.read((char *)(&one_byte), sizeof(one_byte));
        memory[i] = one_byte;
        }

        */
        fin.close();

        //show initial 64 bytes
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
    /*
    inline int paletteCodeToColor(U8 palette_color_code)
    {
    U8  color_code;
    int color;
    U8  bg_palette_color = memory[BG_PALETTE_DATA];
    U8  palette_code_to_color_code[4] = { (bg_palette_color >> 0) & 0x3,(bg_palette_color >> 2) & 0x3,(bg_palette_color >> 4) & 0x3,(bg_palette_color >> 6) & 0x3 };

    //palette color code -> color code
    color_code = palette_code_to_color_code[palette_color_code];

    //color code -> color(winbgi)


    switch (color_code)
    {
    case 0x0:
    color = RED;//COLOR(0, 0, 0);
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
    }*/
    /*
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
    }*/
    //build all tiles
    /*void buildAllTileData()
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
    //rectangle(x_pos, y_pos, x_pos + 8, y_pos + 8);
    //for (int i = 0; i < 16; i++)
    //{
    //    sixteen_byte[i] = memory[VIDEO_RAM_BASE + video_ram_offset + i];
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
    }*/
    //render 160 * 144 takes about 40ms 
    //takes about 79800 cpu cycles
    void buildBackground()
    {
        U16   tile_no; //BG MAP , tile number, chr_code
        U16   bg_disp_data_addr = ((memory[LCD_CTRL_REG] & BG_CODE_SEL_FALG) ? 0x9C00 : 0x9800);
        U16   bg_tile_data_ofst = ((memory[LCD_CTRL_REG] & BG_CHAR_SEL_FALG) ? 0x8000 : 0x8800);

        for (int y = 0; y < 256; y += 8)
        {
            for (int x = 0; x < 256; x += 8)
            {
                tile_no = memory[bg_disp_data_addr++];

                //if bg tile data locates at 0x8000 ~ 0x8FFF
                //tile_no can be 0x00 ~ 0xFF
                //if bg tile data locates at 0x8800 ~ 0x97FF
                //tile_no can be 0x80 ~ 0xFF , 0x00 ~ 0x7F
                //0x00 ~ 0x7F actually means 0x100 ~ 0x17F, so shift tile_no by 0x100
                if (bg_tile_data_ofst == 0x8800)
                {
                    if (tile_no < 0x80)
                    {
                        tile_no = tile_no + 0x100;
                    }
                }
                getTile(tile_no, tile_buf_ptr);
                putimage(BG_X + x, BG_Y + y, tile_buf_ptr, COPY_PUT);
            }
        }
    }/*
     void debugPPU()
     {
     rectangle(0, 0, 257, 257);
     buildAllTileData();
     buildBackground();
     }*/

     //LCD timing
     //Mode2 OAM  O_____O_____O_____O_____O_____O___________________O____
     //Mode3 DMA  _DD____DD____DD____DD____DD____DD__________________D___
     //Mode0 H    ___HHH___HHH___HHH___HHH___HHH___HHH________________HHH
     //Mode1 V    ____________________________________VVVVVVVVVVVVVV_____
    void update_lcd_y_coord()
    {
        //this mask controls frame rate
        if ((cpu_cycles & 0x7F) == 0)
        {
            //each horizontal line takes 512 cpu cycles
            memory[LY] = (U8)(memory[LY] + 1);
            if (((U8)memory[LY]) == 154)
            {
                //this->refresh_lcd = TRUE;
                memory[LY] = (U8)0x0;
            }
        }
    }
    void update_timer()
    {
        U16 input_clk_sel = memory[TAC] & 0x7F;
        input_clk_sel = (input_clk_sel == 0) ? 4 : input_clk_sel; // 0 -> 4

                                                                  // 0b00 -> freq / 2^10
                                                                  // 0b01 -> freq / 2^4
                                                                  // 0b10 -> freq / 2^6
                                                                  // 0b11 -> freq / 2^8
        //U16 timer_mod_mask = (0x1 << ((input_clk_sel + 1) * 2)) - 1;
        U16 timer_mod_mask = (0x1 << ((input_clk_sel + 2) * 2)) - 1;

        if ((cpu_cycles & timer_mod_mask) == 0)
        {
            if (memory[TAC] & TAC_START)
            {
                memory[TIMA] = memory[TIMA] + 1;
            }
            //printf("timer int cpu_cycles = %X, ", cpu_cycles);
            //printf(" memory[TIMA] = %X\n", (U8)(memory[TIMA]));
        }

        //update div
        memory[DIV] = (cpu_cycles & 0xFF00) >> 8;
    }
    void check_interrupt_and_dispatch_isr()
    {
        U8 lcd_int_mode;
        //memory[INT_FLAGS_ADDR];
        //memory[INT_SWITCH_BASE];

        //not need to clear int flag, programmer would clear flag before using it
        //vertical blanking interupt ISR = 0x40
        if (memory[LY] == 0)
        {
            //wake from halt_state
            halt_state = FALSE;
            //printf("\n");
            //set int flag
            memory[INT_FLAGS] = (memory[INT_FLAGS] | INT_FLAG_VERT_BLANKING);

            //trigger refresh lcd, only one time
            if ((cpu_cycles & 0xFF) == 0)
                this->refresh_lcd = TRUE;

            //check if int master enable and vbank int is enabled
            if (ime && (memory[INT_SWITCH] & INT_FLAG_VERT_BLANKING))
            {
                ime = FALSE;
                memory[--sp] = pc >> 8;
                memory[--sp] = pc & 0xFF;
                pc = ISR_VERTICAL_BLANKING;
                //clear flag if the interrupt is served by isr
                memory[INT_FLAGS] = memory[INT_FLAGS] & (~INT_FLAG_VERT_BLANKING);
                //printf("ISR_VERTICAL_BLANKING(%04X)\n", ISR_VERTICAL_BLANKING);
            }
        }


        //LCD interupt ISR = 0x48
        lcd_int_mode = memory[LCD_STAT];

        //LYC == LY
        if (lcd_int_mode & LCD_INT_LYC_LY)
        {
            if (memory[LY] == memory[LYC])
            {
                //because only when every 0x7F cpu cycles, can memory[LY] increment by 1
                //in order to preventing from triggering interrupt multiple times
                //increment memory[LY]  by 1 ,right after interrupt triggered
                memory[LY] = memory[LY] + 1;
                //wake from halt_state
                halt_state = FALSE;

                //set int flag
                memory[INT_FLAGS] = (memory[INT_FLAGS] | INT_FLAG_LCDC);

                if (ime)
                {
                    ime = FALSE;
                    memory[--sp] = pc >> 8;
                    memory[--sp] = pc & 0xFF;
                    pc = ISR_LCDC;
                    //clear flag if the interrupt is served by isr
                    memory[INT_FLAGS] = memory[INT_FLAGS] & (~INT_FLAG_LCDC);

                    //printf("ISR_LCDC(%04X)\n", ISR_LCDC);
                    //printf("goto isr = %X\n", pc);

                    memory[LCD_STAT] = memory[LCD_STAT] | LCD_MATCH_FLAG;
                }
            }
        }

        //timer interupt ISR = 0x50
        if (memory[TIMA] == 0xFF)
        {
            //wake from halt_state
            halt_state = FALSE;

            //set int flag
            memory[INT_FLAGS] = (memory[INT_FLAGS] | INT_FLAG_TIMER);

            //When TIMA overflows, the TMA data is loaded into TIMA
            ASSERT("memory[TMA] != 0xFF", memory[TMA] != 0xFF);                
            

            memory[TIMA] = memory[TMA];

            //memory[TMA] was somehow loaded with 0xFF, It causes emulator to be trapped in timer interrupt, and result in stack overflow, and finally overwrite data in VRAM
            //workaround
            //memory[TIMA] = 0;

            if (ime)
            {
                ime = FALSE;
                memory[--sp] = pc >> 8;
                memory[--sp] = pc & 0xFF;
                ASSERT("sp >= EXTERNAL_RAM_BASE", sp >= EXTERNAL_RAM_BASE);
                pc = ISR_TIMER;
                //clear flag if the interrupt is served by isr
                memory[INT_FLAGS] = memory[INT_FLAGS] & (~INT_FLAG_TIMER);
                printf("timer interupt goto isr = %X\n", pc);
            }
        }
    }


    //get tile from built tile data
    //tile_no is actually charactor code in doc
    void getTile(U16 tile_no, void *tile_ptr, U8 fm = FLIP_NONE)
    {
        U16 tile_idx_x = tile_no & 0xF;  // *8 % 128
        U16 tile_idx_y = tile_no >> 4;   // *8 / 128

        int tile_src_x_beg = BG_TILE_BGI_BUFFER_X + tile_idx_x * TILE_SIZE;
        int tile_src_y_beg = BG_TILE_BGI_BUFFER_Y + tile_idx_y * TILE_SIZE;

        //getimage(tile_src_x_beg, tile_src_y_beg, tile_src_x_beg + TILE_SIZE - 1, tile_src_y_beg + TILE_SIZE - 1, tile_ptr);

        if (fm == FLIP_NONE)
        {
            //minus 1 means 0~7  8~15 (pixel)...
            getimage(tile_src_x_beg, tile_src_y_beg, tile_src_x_beg + TILE_SIZE - 1, tile_src_y_beg + TILE_SIZE - 1, tile_ptr);
        }
        else if (fm == FLIP_HORIZONTAL)
        {
            for (U8 y = 0; y < 8; y++)
            {
                for (U8 x = 0; x < 8; x++)
                {
                    putpixel(FLIP_TILE_BUFFER_X + TILE_SIZE - 1 - x, FLIP_TILE_BUFFER_Y + y, getpixel(tile_src_x_beg + x, tile_src_y_beg + y));
                }
            }
            getimage(FLIP_TILE_BUFFER_X , FLIP_TILE_BUFFER_Y, FLIP_TILE_BUFFER_X + TILE_SIZE - 1, FLIP_TILE_BUFFER_Y + TILE_SIZE - 1, tile_ptr);
        }
        else if (fm == FLIP_VERTICAL)
        {
            for (U8 y = 0; y < 8; y++)
            {
                for (U8 x = 0; x < 8; x++)
                {
                    putpixel(FLIP_TILE_BUFFER_X + x, FLIP_TILE_BUFFER_Y + TILE_SIZE - 1 - y, getpixel(tile_src_x_beg + x, tile_src_y_beg + y));
                }
            }
            getimage(FLIP_TILE_BUFFER_X, FLIP_TILE_BUFFER_Y, FLIP_TILE_BUFFER_X + TILE_SIZE - 1, FLIP_TILE_BUFFER_Y + TILE_SIZE - 1, tile_ptr);
        }
        else if (fm == FLIP_HORIZONTAL_VERTICAL)
        {
            for (U8 y = 0; y < 8; y++)
            {
                for (U8 x = 0; x < 8; x++)
                {
                    putpixel(FLIP_TILE_BUFFER_X + TILE_SIZE - 1 - x, FLIP_TILE_BUFFER_Y + TILE_SIZE - 1 - y, getpixel(tile_src_x_beg + x, tile_src_y_beg + y));
                }
            }
            getimage(FLIP_TILE_BUFFER_X, FLIP_TILE_BUFFER_Y, FLIP_TILE_BUFFER_X + TILE_SIZE - 1, FLIP_TILE_BUFFER_Y + TILE_SIZE - 1, tile_ptr);
        }


    }
    inline void updateOAM()
    {
        OAM_ENTRY *oam_entry_ptr;
        U8 oam_entry_4_byte[4];
        for (U8 i = 0; i < OAM_COUNT; i++)
        {
            //because element in memory is raw data
            //in order to take advantage of pointer
            //we neet to conver it to primitive type
            oam_entry_4_byte[0] = memory[OAM + i * sizeof(OAM_ENTRY) + 0];
            oam_entry_4_byte[1] = memory[OAM + i * sizeof(OAM_ENTRY) + 1];
            oam_entry_4_byte[2] = memory[OAM + i * sizeof(OAM_ENTRY) + 2];
            oam_entry_4_byte[3] = memory[OAM + i * sizeof(OAM_ENTRY) + 3];
            oam_entry_ptr = (OAM_ENTRY*)oam_entry_4_byte;

            //invalid, because memory is no primitive U8 array, is U8DATA instead
            //oam_entry_ptr = (OAM_ENTRY*)(&memory[OAM + i * sizeof(OAM_ENTRY) + 3]);

            getTile(oam_entry_ptr->tile_no, tile_buf_ptr, oam_entry_ptr->flip_y_flip_x);
            putimage(VIEWPORT_X + OAM_X_OFFSET + oam_entry_ptr->pos_x, VIEWPORT_Y + OAM_Y_OFFSET + oam_entry_ptr->pos_y, tile_buf_ptr, AND_PUT);

#if            (ENABLE_DEBUG_RECTANGLE == 1)
            rectangle(VIEWPORT_X + OAM_X_OFFSET + oam_entry_ptr->pos_x, VIEWPORT_Y + OAM_Y_OFFSET + oam_entry_ptr->pos_y, VIEWPORT_X + OAM_X_OFFSET + oam_entry_ptr->pos_x + TILE_SIZE - 1, VIEWPORT_Y + OAM_Y_OFFSET + oam_entry_ptr->pos_y + TILE_SIZE - 1);
#endif
        }
    }
    void refreshLCD()
    {
#if (ENABLE_DOUBLE_GRAPHIC_BUFFER == 1)
        setvisualpage(page);
#endif

        //GetSystemTime(&end_time);
        //printf("s: %d , ms : %d\n", begin_time.wSecond, begin_time.wMilliseconds);
        //printf("s: %d , ms : %d\n", end_time.wSecond, end_time.wMilliseconds);
        buildBackground();


        //viewport
        int scy = memory[SCY];
        int scx = memory[SCX];


        int viewport_x_end = BG_X + scx + LCD_WIDTH;
        int viewport_y_end = BG_Y + scy + LCD_HEIGHT;
        int wrap_x_width;
        int wrap_y_height;

        //for ly == lyc effect

        // ly   scx
        // 0    0
        // 0xF  0x26
        // 160  0
        //
        // part0 ly=0~0xF   , scx=0
        // part1 ly=0xF~160 , scx=0x26

        //
        std::pair<U8, U8> last_ly_scx = std::make_pair<U8, U8>(0, 0);
        this->ly_scx_map[LCD_WIDTH] = 0;

        if (viewport_x_end < BG_WIDTH)
        {
            getimage(BG_X + scx, BG_Y + scy, viewport_x_end - 1, viewport_y_end - 1, viewport_buf_ptr);
        }
        else
        {
            getimage(BG_X + scx, BG_Y + scy, BG_WIDTH - 1, viewport_y_end - 1, viewport_buf_ptr);
        }


        putimage(VIEWPORT_X , VIEWPORT_Y, viewport_buf_ptr, COPY_PUT);

        //wrap part
        if (viewport_x_end > BG_WIDTH)
        {
            wrap_x_width = viewport_x_end - BG_WIDTH;
            viewport_x_end = BG_WIDTH;

            getimage(BG_X, BG_Y + scy, BG_X + wrap_x_width - 1, viewport_y_end - 1, viewport_buf_ptr);
            putimage(VIEWPORT_X + LCD_WIDTH - wrap_x_width, VIEWPORT_Y, viewport_buf_ptr, COPY_PUT);
#if (ENABLE_DEBUG_RECTANGLE == 1)
            rectangle(BG_X, BG_Y + scy, BG_X + wrap_x_width, viewport_y_end);
#endif
        }


		//LY == LYC workaround
        getimage(BG_X, BG_Y , BG_X + LCD_WIDTH, BG_Y + this->fixed_ly, viewport_buf_ptr);
        getimage(BG_X, BG_Y, BG_X + LCD_WIDTH, BG_Y + 0xF, viewport_buf_ptr);
        putimage(VIEWPORT_X, VIEWPORT_Y, viewport_buf_ptr, COPY_PUT);

#if (ENABLE_DEBUG_RECTANGLE == 1)
        if (viewport_x_end < BG_WIDTH)
        {
            rectangle(BG_X + scx, BG_Y + scy, viewport_x_end, viewport_y_end);
        }
        else
        {
            rectangle(BG_X + scx, BG_Y + scy, BG_WIDTH, viewport_y_end);
        }
#endif
        //render sprite ,OAM
        updateOAM();


#if (ENABLE_DOUBLE_GRAPHIC_BUFFER == 1)
        setactivepage(page);
        page = !page;
#endif
    }
    //
    bool isFocused()
    {
        char wnd_title[256];
        char correct_window_title[] = "gameboy_emulator";
        HWND hwnd = GetForegroundWindow();

        GetWindowText(hwnd, wnd_title, sizeof(wnd_title));


        if (strncmp(correct_window_title, wnd_title, 10) == 0)
        {
            //printf("*%s\n", wnd_title);
            //printf("-%s\n", correct_window_title);
            return TRUE;
        }
        else
        {
            return FALSE;
        }

    }
    void run()
    {
        U8    opcode, xx;
        U16   aabb;
        U8    b, reg_value, reg_a_temp;
        U16   prev_value;
        U16   prev_a;
        U8    prev_c;
        U8    carry;
        U8    bhere = 1;
        U8    ly = memory[LY];



        U8  click_debug = FALSE;
        U8  showpc = 0;
        U16 debug_pc = 0xc00a;

        U16 no_carry_sum;
        U16 carry_sum;

        U8 TMA_debug_value = 0x00;
        //each loop takes about 0.0005 ms
        while (TRUE)
        {
            //if (this->isFocused() == FALSE) 
            //{
            //    printf("not focus\n");
            //    continue;
            //}
            //STOP instruction takes the highest priority
            //if stop_state is met, do not do any action
            //if (stop_state == TRUE) 
            //{
            //    continue;
            //}
            
            /*if (detectMouseClick())
            {
                printf("clicked %d\n", cpu_cycles);
                click_debug = !click_debug;

            }*/
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
            if ((halt_state == TRUE) /*|| (click_debug == TRUE)*/)
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

                ly = memory[LY];


                if (log_idx == 8192)
                {
                    log_idx = 0;
                }
                log[log_idx].af = this->af;
                log[log_idx].bc = this->bc;
                log[log_idx].de = this->de;
                log[log_idx].hl = this->hl;
                log[log_idx].pc = this->pc;
                log[log_idx].sp = this->sp;
                log[log_idx].opcode = opcode;
                log[log_idx].xx = xx;
                log[log_idx].aabb = aabb;
                log[log_idx].tma = this->memory[TMA];
                current_log = log[log_idx];
                log_idx++;


                if (this->pc == debug_pc)
                {
                    printREG();
                }

                //ASSERT("0x28 should not be 0x00", memory[0x28] != 0x00);


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
                    //printf("rst %04X , sp = %04X\n", pc, sp);
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
                    //if (((char)xx) < 0)
                    //    printf("add 0x%04X , %02d ,", sp, (char)xx );
                    prev_value = sp;
                    sp += (char)xx;
                    af.f_n = 0;
                    af.f_z = 0;
                    if (((char)xx) > 0)
                    {
                        //although it contradict with doc, it can pass test rom
                        af.f_h = ((prev_value & 0xF) > (sp & 0xF)) ? 1 : 0;
                        af.f_c = ((prev_value & 0xFF) > (sp & 0xFF)) ? 1 : 0;
                    }
                    else if (((char)xx) < 0)
                    {
                        af.f_h = ((prev_value & 0xF) < (sp & 0xF)) ? 0 : 1;
                        af.f_c = ((prev_value & 0xFF) < (sp & 0xFF)) ? 0 : 1;
                    }
                    //printREG();
                    //if (((char)xx) < 0)
                    //    printf("AF = 0x%04X , SP = %04X\n",af.all , sp);
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
                    af.a = memory[bc.all];
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
                        //although it contradict with doc, it can pass test rom
                        af.f_h = ((prev_value & 0xF) > (hl.all & 0xF)) ? 1 : 0;
                        af.f_c = ((prev_value & 0xFF) > (hl.all & 0xFF)) ? 1 : 0;
                    }
                    else if (((char)xx) < 0)
                    {
                        af.f_h = ((prev_value & 0xF) < (hl.all & 0xF)) ? 0 : 1;
                        af.f_c = ((prev_value & 0xFF) < (hl.all & 0xFF)) ? 0 : 1;
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
                    memory[PERIPHERAL_BASE + xx] = af.a;
                    pc += 2;


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
                    //printf("LD    \n");
                    break;

                    //load A with (0xFF00 + C)
                case 0xF2:
                    //cpu_cycles += 2;
                    pc += 1;
                    af.a = memory[PERIPHERAL_BASE + bc.c];
                    //printf("LD    \n");
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
                    /*
                    prev_value = af.a;
                    af.a = af.a + (*idxToRegr_HL(opcode & 0x7)) + af.f_c;
                    af.f_c = (prev_value >= af.a) ? 1 : 0;
                    af.f_h = ((prev_value & 0xF) >= (af.a & 0xF)) ? 1 : 0;
                    af.f_n = 0;
                    af.f_z = (af.a == 0) ? 1 : 0;
                    */




                    //first : counteract carrybit
                    //using reg_a_temp istead of af.a is because if addend is af.a , it maybe changed in the process, even if is also augend
                    //the result maybe confused
                    reg_a_temp = af.a + af.f_c;

                    //this line does not chant af.f_c, so put this line ahead
                    if (((reg_a_temp & 0x0F) == 0x00) && (af.f_c == 1))
                        af.f_h = 1;
                    else
                        af.f_h = 0;

                    if ((reg_a_temp == 0x00) && (af.f_c == 1))
                        af.f_c = 1;
                    else
                        af.f_c = 0;

                    //second : like narmal SUB
                    prev_a = reg_a_temp;
                    reg_a_temp = reg_a_temp + (*idxToRegr_HL(opcode & 0x7));
                    if (af.f_c == 0)
                        af.f_c = (prev_a > reg_a_temp) ? 1 : 0;

                    if (af.f_h == 0)
                        af.f_h = ((prev_a & 0xF) > (reg_a_temp & 0xF)) ? 1 : 0;

                    af.f_n = 0;
                    af.f_z = (reg_a_temp == 0) ? 1 : 0;
                    af.a = reg_a_temp;

                    pc += 1;
                    //printf("ADC  %04X\n", af.a);
                    break;

                    //ADC add xx + flag_carry to A
                case 0xCE:
                    //cpu_cycles += 1;
                    //printf("ADC  %04X\n", af.a);
                    pc += 2;


                    //printf("adc %02X,%02X   af=%04X \n", af.a, xx, af.all);

                    /*
                    prev_value = af.a;
                    af.a += (xx + af.f_c);
                    af.f_c = (prev_value >= af.a) ? 1 : 0;
                    af.f_h = ((prev_value & 0xF) >= (af.a & 0xF)) ? 1 : 0;
                    af.f_n = 0;
                    af.f_z = (af.a == 0) ? 1 : 0;
                    */

                    //first : counteract carrybit
                    //using reg_a_temp istead of af.a is because if addend is af.a , it maybe changed in the process, even if is also augend
                    //the result maybe confused
                    af.a = af.a + af.f_c;

                    //this line does not chant af.f_c, so put this line ahead
                    if (((af.a & 0x0F) == 0x00) && (af.f_c == 1))
                        af.f_h = 1;
                    else
                        af.f_h = 0;

                    if ((af.a == 0x00) && (af.f_c == 1))
                        af.f_c = 1;
                    else
                        af.f_c = 0;

                    //second : like narmal SUB
                    prev_a = af.a;
                    af.a = af.a + xx;
                    if (af.f_c == 0)
                        af.f_c = (prev_a > af.a) ? 1 : 0;

                    if (af.f_h == 0)
                        af.f_h = ((prev_a & 0xF) > (af.a & 0xF)) ? 1 : 0;

                    af.f_n = 0;
                    af.f_z = (af.a == 0) ? 1 : 0;


                    //printf("  af= %04X\n", af.all);

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

                    //decrement    A,B..L (HL) by 1            
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
                        (*idxToRegr_HL(xx & 0x7)) |= BIT(b);
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
                    ASSERT("RETI pc = 0 not makes sense", pc != 0);
                    sp += 2;
                    //auto enable ime
                    this->ime = 1;
                    //clear match flag, copy bgb behavior
                    memory[LCD_STAT] = memory[LCD_STAT] & (~LCD_MATCH_FLAG);
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


                    //printf("sbc %02X,%02X   af=%04X \n", af.a, xx, af.all);

                    /*
                    prev_value = af.a;
                    af.a = af.a - af.f_c - xx;
                    af.f_c = (prev_value < af.a) ? 1 : 0;
                    af.f_h = ((prev_value & 0xF) < (af.a & 0xF)) ? 1 : 0;
                    af.f_n = 1;
                    af.f_z = (af.a == 0) ? 1 : 0;
                    */



                    //two stage
                    //reason : in case a=0 c=1 xx=F
                    //          00 - 01 = FF
                    //           ¡Â<-------------
                    //          FF - 0F = F0   
                    //                     ¡Â<---
                    //                         lower nibble is alway zero , cannot tell if iI should set flag_h


                    //first : counteract carrybit
                    af.a = af.a - af.f_c;

                    //this line does not chant af.f_c, so put this line ahead
                    if (((af.a & 0x0F) == 0x0F) && (af.f_c == 1))
                        af.f_h = 1;
                    else
                        af.f_h = 0;

                    if ((af.a == 0xFF) && (af.f_c == 1))
                        af.f_c = 1;
                    else
                        af.f_c = 0;

                    //second : like narmal ADD
                    prev_a = af.a;
                    af.a = af.a - xx;
                    if (af.f_c == 0)
                        af.f_c = (prev_a < af.a) ? 1 : 0;

                    if (af.f_h == 0)
                        af.f_h = ((prev_a & 0xF) < (af.a & 0xF)) ? 1 : 0;

                    af.f_n = 1;
                    af.f_z = (af.a == 0) ? 1 : 0;

                    //printf("  af= %04X\n", af.all);

                    pc += 2;
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
                    pc += 1;
                    /*
                    prev_value = af.a;
                    af.a = af.a - (*idxToRegr_HL(opcode & 0x7)) - af.f_c ;
                    af.f_h = ((af.a & 0xF) >= (prev_value & 0xF)) ? 1 : 0;
                    af.f_c = (af.a >= prev_value) ? 1 : 0;
                    af.f_n = 1;
                    af.f_z = (af.a == 0) ? 1 : 0;*/

                    //first : counteract carrybit
                    //af.a = af.a - af.f_c;
                    //in order not to modify af.a before all process done, so use reg_a_temp
                    reg_a_temp = af.a - af.f_c;

                    //this line does not chant af.f_c, so put this line ahead
                    if (((reg_a_temp & 0x0F) == 0x0F) && (af.f_c == 1))
                        af.f_h = 1;
                    else
                        af.f_h = 0;

                    if ((reg_a_temp == 0xFF) && (af.f_c == 1))
                        af.f_c = 1;
                    else
                        af.f_c = 0;

                    //second : like narmal ADD
                    prev_a = reg_a_temp;
                    reg_a_temp = reg_a_temp - (*idxToRegr_HL(opcode & 0x7));
                    if (af.f_c == 0)
                        af.f_c = (prev_a < reg_a_temp) ? 1 : 0;

                    if (af.f_h == 0)
                        af.f_h = ((prev_a & 0xF) < (reg_a_temp & 0xF)) ? 1 : 0;

                    af.f_n = 1;
                    af.f_z = (reg_a_temp == 0) ? 1 : 0;

                    af.a = reg_a_temp;

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
                    printf("unknown opcode : %02X\n", opcode);
                    while (1);
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
    U16 bg_tile_ram_end_addr = ((cpu->memory[LCD_CTRL_REG] & BG_CHAR_SEL_FALG) ? 0x8FFF : 0x97FF);
    //8000h ~ A000h
    if (((VIDEO_RAM_BASE <= access_addr) && (access_addr <= bg_tile_ram_end_addr)))
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
        //this->value = val;
        (*(this->raw_byte_ptr)) = val;
    }
    else if (access_addr == JOY_PAD) //0xFF00
    {
        //reset joy pad
        //this->value = val | 0xCF;
        (*(this->raw_byte_ptr)) = val | 0xCF;
    }
    else if (access_addr == DMA) //0xFF46
    {
        U16 src_addr = val << 8;
        //doc says dma addr is of increment of 0x100
        //but I think copy 0xA0(OAM_MEM_SIZE) is enough
        //the size of OAM is only 0xA0, and the remaining 0x60 is not used
        for (U16 idx = 0; idx < OAM_MEM_SIZE; idx++)
        {
            this->cpu->memory[OAM + idx] = this->cpu->memory[src_addr + idx];
        }
        //this->value = val;
        (*(this->raw_byte_ptr)) = val;
    }
    //this area is read-only, the only case to access this area is by DEBUG_MEM->raw_byte_data[MEMORY_SIZE] ,like "fin.read((char *)(memory[0].raw_byte_ptr + CARTRIDGE_ROM_BASE), rom_size);"
    //0000h ~ 8000h
    else if (this->access_addr <= ROM_END)
    {
        //for MBC 
        if (this->access_addr == MBC1_REGISTER_1)
        {
            ASSERT("mbc1 can only be assigned <= 3",val <= 3);
            this->cpu->memory.mbc1.register1 = val;
            //bank switching
            void *bank_tgt_addr = (void *)&this->cpu->memory.raw_byte_data[ROM_BANK1_START];
            // (val - 1) because bank0 is not stored here, I only store bank1 bank2 bank3
            void *bank_src_addr = (void *)&this->cpu->memory.raw_byte_data[ROM_BANK_SWITCHING + (val - 1) * ROM_BANK_SIZE];

            memcpy(bank_tgt_addr, bank_src_addr, ROM_BANK_SIZE);
        }
        else
        {
            //normal read-only ram case
        }
        return (*this);
    }
    else if (this->access_addr == SCX)
    {
        //in order to implement that each scanline(ly) corresponds to different scx
        //because my rendering system is not rendering line by line
        //emulator can only collect all <ly,scx> pair
        //and at last, rendering screen all at once
        U8 cur_ly;
        U8 scx;
        if (this->cpu->memory[LCD_STAT] & LCD_MATCH_FLAG)
        {
            cur_ly = this->cpu->memory[LY];
            scx = val;
            this->cpu->ly_scx_map[cur_ly] = scx;
            this->cpu->fixed_ly = cur_ly;
            //printf("pc: %04X  (ly , scx) = (%d , %d)\n",this->cpu->pc, cur_ly, scx);
        }

        (*(this->raw_byte_ptr)) = val;
        return (*this);
    }
    else if (this->access_addr == TMA) 
    {
        printf("write TMA = %02X\n", val);
        (*(this->raw_byte_ptr)) = val;
        return (*this);
    }
    //normal assignment
    else
    {
        //this->value = val;
        (*(this->raw_byte_ptr)) = val;
    }
    return (*this);
}



int TILE_DOT_DATA_PAINTER::paletteCodeToColor(U8 palette_color_code, U16 which_palette)
{
    U8  color_code;
    int color;
    U8  bg_palette_color = TILE_DOT_DATA_PAINTER::cpu->memory[which_palette];
    U8  palette_code_to_color_code[4] = { (bg_palette_color >> 0) & 0x3,(bg_palette_color >> 2) & 0x3,(bg_palette_color >> 4) & 0x3,(bg_palette_color >> 6) & 0x3 };

    //palette color code -> color code
    color_code = palette_code_to_color_code[palette_color_code];

    //color code -> color
    switch (color_code)
    {
    case 0x0:
        color = COLOR(230, 230, 230);
        break;
    case 0x2:
        color = COLOR(154, 154, 154);
        break;
    case 0x1:
        color = COLOR(77, 77, 77);
        break;
    case 0x3:
        color = COLOR(0, 0, 0);
        break;
    default:
        //printf("unknown color code\n");
        break;
    }
    return color;
}
#pragma once
#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <map>
#include <string>

//general

typedef unsigned char  U8;
typedef unsigned short U16;
#define BIT(x)                       (0x1 << (x))

//gameboy spec
#define CARTRIDGE_ROM_BASE           (0x0000)
#define MBC1_REGISTER_1              (0x2000)
#define ROM_BANK1_START              (0x4000)
#define ROM_END                      (0x7FFF)
#define ROM_IN_Z80_SIZE              (0x8000)
#define VIDEO_RAM_BASE               (0x8000)
#define OAM_TILE_DATA_BASE           (0x8000)
#define EXTERNAL_RAM_BASE            (0xA000)
#define WORK_RAM_BASE                (0xC000)
#define OAM                          (0xFE00)
#define PERIPHERAL_BASE              (0xFF00)
#define JOY_PAD                      (0xFF00)
#define     JOY_PAD_RESET                (BIT(4)|BIT(5))
#define     JOY_PAD_SEL_BUTTON           BIT(4)
#define     JOY_PAD_SEL_DIRECT           BIT(5)
#define DIV                          (0xFF04)     //DIV ,frequently used as random generator
#define TIMA                         (0xFF05)     //timer counter
#define TMA                          (0xFF06)     //timer modulo
#define TAC                          (0xFF07)     // timer control
#define     TAC_START                    BIT(2)   //timer start
#define LCD_CTRL_REG                 (0xFF40)     //LCDC
#define     OBJ_COMP_SEL                 BIT(2)   //OBJ Block Composition Selection Flag
#define     BG_CODE_SEL_FALG             BIT(3)   //BG Code Area Selection Flag , tile index
#define     BG_CHAR_SEL_FALG             BIT(4)   //BG char data Selection Flag , tile data
#define     BG_WIN_ON_FALG               BIT(5)   //window on Flag
#define     BG_WIN_CODE_FLAG             BIT(6)   //window Code Area Selection Flag
#define     LCD_ON_OFF_FLAG              BIT(7)   //LCD Controller Operation Stop Flag
#define LCD_STAT                     (0xFF41)     //STAT
#define     LCD_MATCH_FLAG               BIT(2)   //LY == LYC
#define     LCD_INT_H_BLNK               BIT(3)
#define     LCD_INT_V_BLNK               BIT(4)
#define     LCD_INT_OAM                  BIT(5)
#define     LCD_INT_LYC_LY               BIT(6)
#define SCY                          (0xFF42)     //lcd scroll y
#define SCX                          (0xFF43)     //lcd scroll x
#define LY                           (0xFF44)     //LY lcd control y coordinate
#define LYC                          (0xFF45)     //LY compare
#define DMA                          (0xFF46)     //DMA
#define BG_PALETTE_DATA              (0xFF47)     //BGP
#define OBJ0_PALETTE_DATA            (0xFF48)     //OBJ0
#define OBJ1_PALETTE_DATA            (0xFF49)     //OBJ1
#define WY                           (0xFF4A)     //window_y
#define WX                           (0xFF4B)     //window_x
#define INT_FLAGS                    (0xFF0F)
#define     INT_FLAG_VERT_BLANKING       BIT(0)
#define     INT_FLAG_LCDC                BIT(1)
#define     INT_FLAG_TIMER               BIT(2)
#define     INT_FLAG_SERIEAL_TRANS       BIT(3)
#define     INT_FLAG_P10_P13             BIT(4)
#define HIGH_WORK_RAM_BASE           (0xFF80)
#define STACK_BEGIN_ADDR             (0xFFFE)
#define INT_SWITCH                   (0xFFFF)
#define INTERNAL_MEMORY_SIZE         (0x10000)

#define BG_CHAR_DATA_SIZE            (0x1000)
#define BG_CODE_DATA_SIZE            (0x400)
#define WIN_CODE_DATA_SIZE           (0x200)

#define OAM_MEM_SIZE                 (0xA0)
#define OAM_COUNT                    (40)
#define TILE_BYTE_SIZE               (16)       //each tile takes 16 bytes

#define ROM_HEADER_OFFSET            (0x100)
#define    MBC_NONE                  (0x0)
#define    MBC1_NO_SRAM              (0x1)
#define    MBC3_SRAM_BATTERY         (0x13)

#define MBC3_RAM_SIZE                (0x8000)

#define MBC3_REGISTER_0_START        (0x0000)    //Write addresses: 0000h-1FFFh
#define MBC3_REGISTER_0_END          (0x1FFF)    //write 0x0A to Allows access to RAM and the clock counter registers
#define MBC3_REGISTER_1_START        (0x2000)    //Write addresses: 2000h-3FFFh
#define MBC3_REGISTER_1_END          (0x3FFF)    //Write data : 01h - 7Fh, select ROM bank
#define MBC3_REGISTER_2_START        (0x4000)    //Write addresses : 4000h - 5FFFh
#define MBC3_REGISTER_2_END          (0x5FFF)    //Write data: 0-3 : select extRAM bank , Write data: 08h-0Ch : select time counter
#define     MBC3_REGISTER_2_RTC_S        (0x8)   //bank 0x08 second counter
#define     MBC3_REGISTER_2_RTC_M        (0x9)   //bank 0x09 minutes counter
#define     MBC3_REGISTER_2_RTC_H        (0xA)   //bank 0x0A hour counter
#define     MBC3_REGISTER_2_RTC_DL       (0xB)   //bank 0x0B day(L) counter
#define     MBC3_REGISTER_2_RTC_DH       (0xC)   //bank 0x0C day(H) counter
#define         RTC_DH_HALT                 (BIT(6))
#define MBC3_REGISTER_3_START        (0x6000)    //Write addresses: 6000h-7FFFh
#define MBC3_REGISTER_3_END          (0x7FFF)    //Write  0 w 1 causes all counter 


                //bank 0x08 second counter
                //bank 0x09 minutes counter
                //bank 0x0A hour counter
                //bank 0x0B day(L) counter
                //bank 0x0C day(H) counter


#define ROM_32KB                     (0x0)
#define ROM_64KB                     (0x1)
#define ROM_128KB                    (0x2)
#define ROM_256KB                    (0x3)
#define ROM_512KB                    (0x4)
#define ROM_1MB                      (0x5)
#define ROM_2MB                      (0x6)
#define ROM_4MB                      (0x7)
#define ROM_8MB                      (0x8)

#define SRAM_NONE                    (0x0)
#define SRAM_RESERVED                (0x1)
#define SRAM_8KB                     (0x2)
#define SRAM_32KB                    (0x3)
#define SRAM_128KB                   (0x4)



//emulator
#define MEMORY_SIZE                  (0x10000)
#define SPARE_RAW_SIZE               (0x10000)    //spare space for bank switching
#define ROM_BANK_SIZE                (0x4000)
#define EXT_RAM_BANK_SIZE            (0x2000)




//associated with code execute rate, because v-blank acts as timer
#define FRAME_RATE                   (0x40)    //every (FRAME_RATE * 154) cpu cycles, update screen once
#define FRAME_RATE_HOLD_COUNT        (2)       //act as downsample frame rate

#define SCALE_SIZE                   (2)
#define PIXEL_SIZE                   (1 * SCALE_SIZE)
#define SCREEN_OFFSET                (10)
#define LCD_WIDTH                    (160 * SCALE_SIZE)
#define LCD_HEIGHT                   (144 * SCALE_SIZE)
#define BG_WIDTH                     (256 * SCALE_SIZE)
#define BG_HEIGHT                    (256 * SCALE_SIZE)
#define LY_MAX                       (154)
#define BG_X                         (0)
#define BG_Y                         (0)
#define OAM_X_OFFSET                 (-8 * SCALE_SIZE)
#define OAM_Y_OFFSET                 (-16 * SCALE_SIZE)
#define VIEWPORT_X                   (395 * SCALE_SIZE)
#define VIEWPORT_Y                   (0)
#define VIEWPORT_X_END               (VIEWPORT_X + LCD_WIDTH)
#define VIEWPORT_Y_END               (VIEWPORT_Y + LCD_HEIGHT)
#define TWO_PASS_RENDER_X            (0)
#define TWO_PASS_RENDER_Y            (512)
#define PAGE_NUM                     (2)
#define LOAD_STATE_BUTTON_X          (395 * SCALE_SIZE)			//simple gui button
#define LOAD_STATE_BUTTON_Y          (200 * SCALE_SIZE)
#define LOAD_STATE_BUTTON_WIDTH      (70 * SCALE_SIZE)
#define LOAD_STATE_BUTTON_HEIGHT     (20 * SCALE_SIZE)
#define SAVE_STATE_BUTTON_X          (395 * SCALE_SIZE)			//simple gui button
#define SAVE_STATE_BUTTON_Y          (225 * SCALE_SIZE)
#define SAVE_STATE_BUTTON_WIDTH      (70 * SCALE_SIZE)
#define SAVE_STATE_BUTTON_HEIGHT     (20 * SCALE_SIZE)



#define BANK_SWITCHING_SPARE_ADDR    (0x10000)

#define DEBUG_WINDOW_WIDTH           (200)
#define BG_TILE_BGI_BUFFER_X         (256 * SCALE_SIZE)
#define BG_TILE_BGI_BUFFER_Y         (0)

#define FLIP_TILE_BUFFER_X           (270 * SCALE_SIZE)
#define FLIP_TILE_BUFFER_Y           (200 * SCALE_SIZE)

#define TILE_SIZE                    (8 * SCALE_SIZE)
#define TILE_MEM_SIZE                (0x1800)
                                     
#define ISR_VERTICAL_BLANKING        (0x40)
#define ISR_LCDC                     (0x48)
#define ISR_TIMER                    (0x50)
#define ISR_SERIEAL_TRANS_CPL        (0x58)
#define ISR_P10_P10_INPUT            (0x60)

//debug setting
#define ENABLE_DOUBLE_GRAPHIC_BUFFER (1)
#define ENABLE_DEBUG_RECTANGLE       (1)

int _hang = TRUE;
#define ASSERT(ERR_MSG, X)      if(!(X))\
                               {\
                                   printf("%s\n",ERR_MSG);\
                                    while(_hang);\
                                    _hang = TRUE;\
                               }

#define BREAK_POINT(x)          if((x))\
                                {\
                                    printf("hit %s\n",#x);\
                                    while(_hang);\
                                    _hang = TRUE;\
                                }

//ROM header    128 bytes
typedef struct rom_header
{
    U8    nop;                       //0x100
    U8    jp_opcode;                 //0x101
    U16   start_address;             //0x102 ~ 0x103
    U8    nintendo_char_data[48];    //0x104 ~ 0x133
    U8    game_tile[15];             //0x134 ~ 0x142
    U8    reserved_0[4];             //0x143 ~ 0x146
    U8    mbc_ram_type;              //0x147
    U8    rom_size;                  //0x148
    U8    ext_ram_size;              //0x149
    U8      reserved_1[6];           //0x14A ~ 0x14F
}ROM_HEADER , *ROM_HEADER_PTR;


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


void put_super_pixel(int super_pixel_x, int super_pixel_y, int bgi_color, U8 pixel_size)
{
    for(int y = 0; y < pixel_size; y++)
    {        
        for(int x = 0; x < pixel_size; x++)
        {

            putpixel(super_pixel_x + x, super_pixel_y + y, bgi_color);
        }
    }
}


class DMGZ80CPU;


class TILE_DOT_DATA_PAINTER
{
public:
    static DMGZ80CPU *cpu;
    static U8 ff47_bg_palette;
    static U8 ff48_obj0_palette;
    static U8 ff49_obj1_palette;
    static int TILE_DOT_DATA_PAINTER::paletteCodeToColor(U8 palette_color_code, U16 which_palette);
    static int debug_draw_bar;
    //paint 1 row, 8 dots
    //in BG_TILE_BGI_BUFFER ,each row accommodates 16 tile (128 dots)
    static void paintTileDotData(U16 dot_data_byte_idx, U8 upper_dot_byte, U8 lower_dot_byte, U16 which_palette);


    static void paintOneTile(int x, int y, U8 *tile_dot_raw_data_addr, U16 which_palette);
};

int TILE_DOT_DATA_PAINTER::debug_draw_bar    = 0;
U8  TILE_DOT_DATA_PAINTER::ff47_bg_palette   = 0;
U8  TILE_DOT_DATA_PAINTER::ff48_obj0_palette = 0;
U8  TILE_DOT_DATA_PAINTER::ff49_obj1_palette = 0;




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
        //if(this->access_addr == TIMA)
            //printf("get address operator @ %04X\n" ,this->access_addr);
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
/*
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
*/


class DEBUG_MEM
{
public:
    U16 access_addr;
    U8DATA  data[MEMORY_SIZE];
    U8      raw_byte_data[MEMORY_SIZE];
    U8DATA  *stack;
    //memory bank controller
    //MBC1    mbc1;
    
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
        access_addr = addr;
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
    U8  opcode;
    U8  palette_data;
    U16 sp;
    AF af;
    BC bc;
    DE de;
    HL hl;
    U8 xx;
    U16 aabb;
}DEBUGLOG;

class DMGZ80CPU;

class MBC_BASE
{
public:
    std::string rom_file_name;
    DMGZ80CPU   *cpu;
    char        *bank_switched_rom_addr;
    char        *bank_switched_ext_ram_addr;
    U8           cur_bank_no;

    MBC_BASE() {}
    MBC_BASE(std::string str, DMGZ80CPU *c);
    virtual void bank_switching(U16 addr, U8 val) = 0;
};

class MBC3 :public MBC_BASE 
{
public:
    U8 external_ram[MBC3_RAM_SIZE];
    SYSTEMTIME   cur_time;

    //pointer to 0xA000
    U8 *rtc_s;
    U8 *rtc_m;
    U8 *rtc_h;
    U8 *rtc_dl;
    U8 *rtc_dh;

    //constructor
    MBC3(std::string str, DMGZ80CPU *c);
    MBC3();

    //function
    MBC3 &operator=(MBC3 &_mbc3) 
    {
        printf("assign operator\n");
        memset((void*)(this->external_ram), 0x0, MBC3_RAM_SIZE);
        this->cpu                        = _mbc3.cpu;
        this->rom_file_name              = _mbc3.rom_file_name;
        this->bank_switched_rom_addr     = _mbc3.bank_switched_rom_addr;
        this->bank_switched_ext_ram_addr = _mbc3.bank_switched_ext_ram_addr;
        return (*this);
    }

    virtual void bank_switching(U16 addr, U8 val)
    {
        //
        //printf("MBC3 bank switching\n");

        //write 0x0A to get allowed to access RAM
        if ((MBC3_REGISTER_0_START <= addr) && (addr <= MBC3_REGISTER_0_END))
        {
            //do nothing
        }
        //Write data: 01h-7Fh to select rom bank
        else if ((MBC3_REGISTER_1_START <= addr) && (addr <= MBC3_REGISTER_1_END))
        {
            U8 bank_no = val;
            std::ifstream fin;
            fin.open(this->rom_file_name.c_str(), std::ios::ate | std::ios::binary);
            fin.seekg(ROM_BANK_SIZE * bank_no, fin.beg);
            fin.read(this->bank_switched_rom_addr, ROM_BANK_SIZE);
            fin.close();
        }
        //Write data: 0-3 to select ram bank
        //Write data: 08h-0Ch to select time counter
        else if ((MBC3_REGISTER_2_START <= addr) && (addr <= MBC3_REGISTER_2_END))
        {
            U8 bank_no = val;
            //select ram bank
            if ((0 <= val) && (val <= 3))
            {
                //backup current ram to MBC ext_ram
                memcpy((void*)(&this->external_ram[EXT_RAM_BANK_SIZE * this->cur_bank_no]), this->bank_switched_ext_ram_addr, EXT_RAM_BANK_SIZE);          
                //copy MBC ext_ram to cpu ram
                memcpy(this->bank_switched_ext_ram_addr, (void*)(&this->external_ram[EXT_RAM_BANK_SIZE * bank_no]), EXT_RAM_BANK_SIZE);
            }
            //time counter
            else if ((0x8 <= val) && (val <= 0xC))
            {
                GetSystemTime(&cur_time);

                //bank 0x0C day(H) counter
                //bank 0x0B day(L) counter
                //bank 0x0A hour counter
                //bank 0x09 minutes counter
                //bank 0x08 second counter
                switch (val) 
                {
                case MBC3_REGISTER_2_RTC_S:
                    *(this->rtc_s) = cur_time.wSecond;
                    break;

                case MBC3_REGISTER_2_RTC_M:
                    *(this->rtc_m) = cur_time.wMinute;
                    break;

                case MBC3_REGISTER_2_RTC_H:
                    *(this->rtc_h) = cur_time.wHour;
                    break;

                case MBC3_REGISTER_2_RTC_DL:
                    *(this->rtc_dl) = cur_time.wDay;
                    break;

                case MBC3_REGISTER_2_RTC_DH:
                    *(this->rtc_dh) = (cur_time.wDay & BIT(8)) >> 8;
                    //TODO : no concerned with carry bit
                    //no implement halt bit
                    break;

                default:
                    printf("unexpected case\n");
                    while (1);
                }

            }
            else 
            {
                ASSERT("val <= 0xC", val <= 0xC);
            }
        }
        //Writing 0 -> 1 causes all counter data to be latched
        else if ((MBC3_REGISTER_3_START <= addr) && (addr <= MBC3_REGISTER_3_END))
        {
            //do nothing
        }
    }
};

class MBC1 : public MBC_BASE
{
public:

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

    const U16 mbc1_register1_addr;

    MBC1() : MBC_BASE() , mbc1_register1_addr(MBC1_REGISTER_1)  
    {
        printf("default constructor\n");
    }

    MBC1(std::string str, DMGZ80CPU *c) :MBC_BASE(str, c) , mbc1_register1_addr(MBC1_REGISTER_1)
    {
        printf("constructor with filename and cpu_ptr\n");
    }

    MBC1 &operator= (const MBC1 &_mbc1) 
    {
        printf("assign operator\n");
        this->cpu                          = _mbc1.cpu;
        this->rom_file_name              = _mbc1.rom_file_name;
        this->bank_switched_rom_addr     = _mbc1.bank_switched_rom_addr;
        this->bank_switched_ext_ram_addr = _mbc1.bank_switched_ext_ram_addr;
        return (*this);
    }

    //val : which bank
    virtual void bank_switching(U16 addr, U8 val)
    {
        if (addr == this->mbc1_register1_addr) 
        {
            ASSERT("mbc1 can only be assigned <= 3", val <= 3);
            this->register0 = val;

            int rom_bank_offset;
            std::ifstream fin;

            fin.open(this->rom_file_name.c_str(), std::ios::ate | std::ios::binary);
            //move pointer to rom bank n
            rom_bank_offset = ROM_BANK_SIZE * val;
            fin.seekg(ROM_BANK_SIZE * val, fin.beg);
            //copy to Z80 ram
            fin.read((char*)this->bank_switched_rom_addr, ROM_BANK_SIZE);
            //close file
            fin.close();

            //printf("rom bank switching : %d\n", val);
        }
    }
};





/*


ASSERT("mbc1 can only be assigned <= 3",val <= 3);
this->cpu->memory.mbc1.register1 = val;
//bank switching
void *bank_tgt_addr = (void *)&this->cpu->memory.raw_byte_data[ROM_BANK1_START];
// (val - 1) because bank0 is not stored here, I only store bank1 bank2 bank3
void *bank_src_addr = (void *)&this->cpu->memory.raw_byte_data[BANK_SWITCHING_SPARE_ADDR + (val - 1) * ROM_BANK_SIZE];

memcpy(bank_tgt_addr, bank_src_addr, ROM_BANK_SIZE);


*/

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
    U8 ime; 
    U16 sp;                           //interrupt master enable

                                       //emulator flag
    U8               halt_state;
    U8               stop_state;
    void*            tile_buf_ptr;
    void*            flip_tile_buf_ptr;
    void*            viewport_buf_ptr;
    SYSTEMTIME       begin_time;
    SYSTEMTIME       end_time;
    U8               refresh_lcd;
    U8               refresh_lcd_hold_count;
    unsigned int     cpu_cycles;


    int log_idx;

    TILE_DOT_DATA_PAINTER tile_dot_data_painter;
    DEBUG_MEM memory;
    U16       pc;
    U16       debug_pc;
    std::map<U16, U16>  ly_scx_map;
    U8   page;  //two page 0 and 1
    U8   lyc_match_ly;
    int mouse_x;
    int mouse_y;


    MBC1          mbc1_;
    MBC3          mbc3_;
    MBC_BASE     *mbc_ptr;
    std::string   rom_filename;


    enum      filp_mode {FLIP_NONE, FLIP_HORIZONTAL, FLIP_VERTICAL, FLIP_HORIZONTAL_VERTICAL };
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
        this->refresh_lcd_hold_count = 0;
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
            this->mouse_x = mousex();
            this->mouse_y = mousey();
            clearmouseclick(WM_LBUTTONDOWN);
            clicked = TRUE;
        }
        return clicked;
    }
    //todo
    void loadCpuState() 
    {
        std::stringstream ss;
        std::ifstream     fin;
        ROM_HEADER*       rom_header_ptr;
        U16               bg_tile_ram_end_addr;

        ss << this->rom_filename << ".save";
        fin.open(ss.str().c_str(), std::ios::in | std::ios::binary);

        //cpu
        fin.read((char*)(&this->pc),         sizeof(this->pc));
        fin.read((char*)(&this->sp),         sizeof(this->sp));
        fin.read((char*)(&this->ime),        sizeof(this->ime));
        fin.read((char*)(&this->af.all),     sizeof(this->af.all));
        fin.read((char*)(&this->bc.all),     sizeof(this->bc.all));
        fin.read((char*)(&this->de.all),     sizeof(this->de.all));
        fin.read((char*)(&this->hl.all),     sizeof(this->hl.all));
        fin.read((char*)(&this->halt_state), sizeof(this->halt_state));
        fin.read((char*)(&this->cpu_cycles), sizeof(this->cpu_cycles));

        //mbc
        rom_header_ptr = (ROM_HEADER*)(memory[ROM_HEADER_OFFSET].raw_byte_ptr);
        if (rom_header_ptr->mbc_ram_type == MBC3_SRAM_BATTERY)
        {
            fin.read((char*)this->mbc3_.external_ram, MBC3_RAM_SIZE);
        }
        else
        {
            printf("other mbc does not need to save\n");
        }

        //64 kb ram
        fin.read((char*)this->memory.raw_byte_data, INTERNAL_MEMORY_SIZE);

        //re-paint tile
        bg_tile_ram_end_addr = ((this->memory[LCD_CTRL_REG] & BG_CHAR_SEL_FALG) ? 0x8FFF : 0x97FF);
        for (int vram_addr = VIDEO_RAM_BASE; vram_addr <= bg_tile_ram_end_addr; vram_addr++)
        {
            //we need to collect 16 byte before painting one tile
            if ((vram_addr & 0xF) == 0xF)
            {
                //arrange tile location
                int dot_data_byte_idx = vram_addr;
                U8 dots_one_row = 0x7F;   //128 dots in a row, 16 tile in a row
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
                                                // 12 / 8 = 1     -> x_pos = 1*8
                                                // 12 / 128 = 0   -> y_tile_row = 0*8
                                                // (12 % 8) = 4 -> y_pos = y_row + 4


                dot_data_byte_idx = vram_addr - VIDEO_RAM_BASE;
                //dot_data_idx = (dot_data_byte_idx >> 1) , because two bytes represent one row
                U16 dot_data_idx = (dot_data_byte_idx >> 1);
                //each tile contains 64 dots
                // 12 / 8 = 1     -> x_pos = 1*8
                int x_pos = BG_TILE_BGI_BUFFER_X + (((dot_data_idx & dots_one_row) >> 3) * TILE_SIZE);
                // 12 / 128 = 0   -> y_tile_row = 0 * 8
                int y_pos = (dot_data_idx >> dots_one_row_shift) * TILE_SIZE;

                TILE_DOT_DATA_PAINTER::paintOneTile(x_pos, y_pos, &this->memory.raw_byte_data[vram_addr - 0xF], BG_PALETTE_DATA);
            }
        }


        fin.close();

    }
    void saveCpuState() 
    {
        std::stringstream ss;
        std::ofstream     fout;
        ROM_HEADER*       rom_header_ptr;


        ss << this->rom_filename << ".save";
        fout.open(ss.str().c_str(), std::ios::out | std::ios::binary);

        //cpu
        fout.write((char*)(&this->pc),         sizeof(this->pc));
        fout.write((char*)(&this->sp),         sizeof(this->sp));
        fout.write((char*)(&this->ime),        sizeof(this->ime));
        fout.write((char*)(&this->af.all),     sizeof(this->af.all));
        fout.write((char*)(&this->bc.all),     sizeof(this->bc.all));
        fout.write((char*)(&this->de.all),     sizeof(this->de.all));
        fout.write((char*)(&this->hl.all),     sizeof(this->hl.all));
        fout.write((char*)(&this->halt_state), sizeof(this->halt_state));
        fout.write((char*)(&this->cpu_cycles), sizeof(this->cpu_cycles));

        //mbc
        rom_header_ptr = (ROM_HEADER*)(memory[ROM_HEADER_OFFSET].raw_byte_ptr);
        if (rom_header_ptr->mbc_ram_type == MBC3_SRAM_BATTERY)
        {
            fout.write((char*)this->mbc3_.external_ram, MBC3_RAM_SIZE);
        }
        else
        {
            printf("other mbc does not need to save\n");
        }

        //64 kb ram
        fout.write((char*)this->memory.raw_byte_data, INTERNAL_MEMORY_SIZE);


        fout.close();
    }
    void readROM(std::string filename)
    {
        ROM_HEADER    *rom_header_ptr;
        std::ifstream fin;
        int cartridge_rom_size;
        U8  one_byte;

        this->rom_filename = filename;

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
        //back up bank1~bank3 in spare rom
        //move pointer to beginning of bank1
        //fin.seekg(ROM_BANK_SIZE, fin.beg);
        //store remaining rom data at 0x10000
        //fin.read((char *)(memory[0].raw_byte_ptr + BANK_SWITCHING_SPARE_ADDR), cartridge_rom_size - ROM_BANK_SIZE);


        rom_header_ptr = (ROM_HEADER*)(memory[ROM_HEADER_OFFSET].raw_byte_ptr);
        if (rom_header_ptr->mbc_ram_type == MBC1_NO_SRAM)
        {
            printf("MBC1_NO_SRAM\n");
            mbc1_ = MBC1(filename, this);
            this->mbc_ptr = &mbc1_;
        }
        else if (rom_header_ptr->mbc_ram_type == MBC3_SRAM_BATTERY)
        {
            printf("MBC3_SRAM_BATTERY\n");
            mbc3_ = MBC3(filename, this);
            this->mbc_ptr = &mbc3_;
        }
        else  if (rom_header_ptr->mbc_ram_type == MBC_NONE)
        {
            printf("MBC_NONE\n");
        }
        else
        {
            ASSERT("unknown mbc type", 0);
        }

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
    ////rectangle(x_pos, y_pos, x_pos + 8, y_pos + 8);
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

        for (int y = 0; y < BG_HEIGHT; y += TILE_SIZE)
        {
            for (int x = 0; x < BG_WIDTH; x += TILE_SIZE)
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
     //rectangle(0, 0, 257, 257);
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
        
        //if ((cpu_cycles % FRAME_RATE) == 0)
        //suit mario : if ((cpu_cycles & 0x3F) == 0)
        if ((cpu_cycles & 0x1F) == 0)
        {
            memory[LY] = (U8)(memory[LY] + 1);
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

        //interrupt priority 1
        //not need to clear int flag, programmer would clear flag before using it
        //vertical blanking interupt ISR = 0x40
        //LY takes the value 0 ~ 153, with 144 ~ 153 representing v-blanking
        if (memory[LY] == LY_MAX)
        {
            //after refresh lcd, reset LY
            memory[LY] = (U8)0x0;
            this->refresh_lcd_hold_count++;

            //act as downsample, render frame less frequently than it acutually should have randered
            //because rendering too frequently cause bad performace
            if (this->refresh_lcd_hold_count == FRAME_RATE_HOLD_COUNT) 
            {
                this->refresh_lcd = TRUE;
                this->refresh_lcd_hold_count = 0;
            }


            //printf("-- memory[LY] == %02X   cpu_cycles = %08X\n", (U8)memory[LY], cpu_cycles);
            //wake from halt_state
            halt_state = FALSE;
            //printf("\n");
            //set int flag
            memory[INT_FLAGS] = (memory[INT_FLAGS] | INT_FLAG_VERT_BLANKING);

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


        //interrupt priority 2
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

                lyc_match_ly = memory[LY];

                //printf("memory[LY] == memory[LYC]\n");
                //printf("LY = %02X\n", (U8)memory[LY]);

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

                    //printf("ISR_LCDC(%04X) LY 0x%02X\n", ISR_LCDC,memory[LY]);
                    //printf("goto isr = %X\n", pc);

                    memory[LCD_STAT] = memory[LCD_STAT] | LCD_MATCH_FLAG;
                }
            }
        }

        //interrupt priority 3
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
                //printf("timer interupt goto isr = %X\n", pc);
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
            for (U8 y = 0; y < TILE_SIZE; y+= PIXEL_SIZE)
            {
                for (U8 x = 0; x < TILE_SIZE; x+= PIXEL_SIZE)
                {
                    //putpixel(FLIP_TILE_BUFFER_X + TILE_SIZE - 1 - x, FLIP_TILE_BUFFER_Y + y, getpixel(tile_src_x_beg + x, tile_src_y_beg + y));
                    put_super_pixel(FLIP_TILE_BUFFER_X + TILE_SIZE - PIXEL_SIZE - x, 
                                    FLIP_TILE_BUFFER_Y + y, 
                                    getpixel(tile_src_x_beg + x, tile_src_y_beg + y), SCALE_SIZE);
                }
            }
            getimage(FLIP_TILE_BUFFER_X , 
                     FLIP_TILE_BUFFER_Y, 
                     FLIP_TILE_BUFFER_X + TILE_SIZE - 1, 
                     FLIP_TILE_BUFFER_Y + TILE_SIZE - 1, tile_ptr);
        }
        else if (fm == FLIP_VERTICAL)
        {
            for (U8 y = 0; y < TILE_SIZE; y+= PIXEL_SIZE)
            {
                for (U8 x = 0; x < TILE_SIZE; x+= PIXEL_SIZE)
                {
                    //putpixel(FLIP_TILE_BUFFER_X + x, FLIP_TILE_BUFFER_Y + TILE_SIZE - 1 - y, getpixel(tile_src_x_beg + x, tile_src_y_beg + y));
                    put_super_pixel(FLIP_TILE_BUFFER_X + x, 
                                    FLIP_TILE_BUFFER_Y + TILE_SIZE - PIXEL_SIZE - y, 
                                    getpixel(tile_src_x_beg + x, tile_src_y_beg + y), SCALE_SIZE);
                }
            }
            getimage(FLIP_TILE_BUFFER_X, 
                     FLIP_TILE_BUFFER_Y, 
                     FLIP_TILE_BUFFER_X + TILE_SIZE - 1, 
                     FLIP_TILE_BUFFER_Y + TILE_SIZE - 1, tile_ptr);
        }
        else if (fm == FLIP_HORIZONTAL_VERTICAL)
        {
            for (U8 y = 0; y < TILE_SIZE; y+= PIXEL_SIZE)
            {
                for (U8 x = 0; x < TILE_SIZE; x+= PIXEL_SIZE)
                {
                    //putpixel(FLIP_TILE_BUFFER_X + TILE_SIZE - 1 - x, FLIP_TILE_BUFFER_Y + TILE_SIZE - 1 - y, getpixel(tile_src_x_beg + x, tile_src_y_beg + y));
                    put_super_pixel(FLIP_TILE_BUFFER_X + TILE_SIZE - PIXEL_SIZE - x, 
                                    FLIP_TILE_BUFFER_Y + TILE_SIZE - PIXEL_SIZE - y, 
                                    getpixel(tile_src_x_beg + x, tile_src_y_beg + y), SCALE_SIZE);
                }
            }
            getimage(FLIP_TILE_BUFFER_X, 
                     FLIP_TILE_BUFFER_Y, 
                     FLIP_TILE_BUFFER_X + TILE_SIZE - 1, 
                     FLIP_TILE_BUFFER_Y + TILE_SIZE - 1, tile_ptr);
        }
    }

    inline void updateWindow()
    {
        U16   win_code_addr = ((memory[LCD_CTRL_REG] & BG_WIN_CODE_FLAG) ? 0x9C00 : 0x9800);
        U16   tile_no; //BG MAP , tile number, chr_code
        short    win_x = memory[WX] * SCALE_SIZE - TILE_SIZE + PIXEL_SIZE;
        short    win_y = memory[WY] * SCALE_SIZE;



        //if windows is on
        if (memory[LCD_CTRL_REG] & BG_WIN_ON_FALG) 
        {
            //window size : 144 * 160
            for (int y = 0; y < BG_HEIGHT; y = y + TILE_SIZE)
            {
                for (int x = 0; x < BG_WIDTH; x = x + TILE_SIZE)
                {
                    //only render window ahead viewport_y_end, to save rendering resource
                    if (((VIEWPORT_Y + win_y + y) < VIEWPORT_Y_END))
                    {
                        tile_no = memory[win_code_addr++];
                        //win code data is from bg_tiles               
                        if (tile_no < 0x80)
                        {
                            tile_no = tile_no + 0x100;
                        }
                        getTile(tile_no, tile_buf_ptr);
                        putimage(VIEWPORT_X + win_x + x, VIEWPORT_Y + win_y + y, tile_buf_ptr, COPY_PUT);

                    }
                }
            }
        }
#if (1 == ENABLE_DEBUG_RECTANGLE)
                        rectangle(VIEWPORT_X + win_x, VIEWPORT_Y + win_y, VIEWPORT_X + win_x + LCD_WIDTH , VIEWPORT_Y + win_y + LCD_HEIGHT);
#endif
    }
    inline void updateOAM()
    {
        OAM_ENTRY *oam_entry_ptr;
        U8 oam_entry_4_byte[4];
        for (U8 i = 0; i < OAM_COUNT; i++)
        {
            //because element in memory is raw data
            //we neet to conver it to primitive type
            //oam_entry_4_byte[0] = memory[OAM + i * sizeof(OAM_ENTRY) + 0];
            //oam_entry_4_byte[1] = memory[OAM + i * sizeof(OAM_ENTRY) + 1];
            //oam_entry_4_byte[2] = memory[OAM + i * sizeof(OAM_ENTRY) + 2];
            //oam_entry_4_byte[3] = memory[OAM + i * sizeof(OAM_ENTRY) + 3];
            //oam_entry_ptr = (OAM_ENTRY*)oam_entry_4_byte;

            //invalid, because memory is no primitive U8 array, is U8DATA instead
            //in order to take advantage of pointer, let OAM_ENTRY* point to raw data
            oam_entry_ptr = (OAM_ENTRY*)(&memory.raw_byte_data[OAM + i * sizeof(OAM_ENTRY)]);

            //paintSpriteTile(oam_entry_ptr->tile_no, oam_entry_ptr->palette == 0 ? OBJ0_PALETTE_DATA : OBJ1_PALETTE_DATA);

            //if it is 8x16 sprite
            //memory[LCD_CTRL_REG] bit2 = 1
            if (memory[LCD_CTRL_REG] & OBJ_COMP_SEL) 
            {
                //upside-down case
                if ((oam_entry_ptr->flip_y_flip_x == FLIP_VERTICAL) || (oam_entry_ptr->flip_y_flip_x == FLIP_VERTICAL)) 
                {
                    //tile 0
                    getTile(oam_entry_ptr->tile_no + 1, tile_buf_ptr, oam_entry_ptr->flip_y_flip_x);
                    putimage(VIEWPORT_X + OAM_X_OFFSET + oam_entry_ptr->pos_x * PIXEL_SIZE, 
                             VIEWPORT_Y + OAM_Y_OFFSET + oam_entry_ptr->pos_y * PIXEL_SIZE, 
                             tile_buf_ptr, AND_PUT);
                    //tile 1
                    getTile(oam_entry_ptr->tile_no, tile_buf_ptr, oam_entry_ptr->flip_y_flip_x);
                    putimage(VIEWPORT_X + OAM_X_OFFSET + oam_entry_ptr->pos_x * PIXEL_SIZE, 
                             VIEWPORT_Y + OAM_Y_OFFSET + oam_entry_ptr->pos_y * PIXEL_SIZE + TILE_SIZE, 
                             tile_buf_ptr, AND_PUT);
                }
                else 
                {
                    //tile 0
                    getTile(oam_entry_ptr->tile_no + 1, tile_buf_ptr, oam_entry_ptr->flip_y_flip_x);
                    putimage(VIEWPORT_X + OAM_X_OFFSET + oam_entry_ptr->pos_x * PIXEL_SIZE, 
                             VIEWPORT_Y + OAM_Y_OFFSET + oam_entry_ptr->pos_y * PIXEL_SIZE + TILE_SIZE, 
                             tile_buf_ptr, AND_PUT);
                    //tile 1
                    getTile(oam_entry_ptr->tile_no, tile_buf_ptr, oam_entry_ptr->flip_y_flip_x);
                    putimage(VIEWPORT_X + OAM_X_OFFSET + oam_entry_ptr->pos_x * PIXEL_SIZE, 
                             VIEWPORT_Y + OAM_Y_OFFSET + oam_entry_ptr->pos_y * PIXEL_SIZE, 
                             tile_buf_ptr, AND_PUT);
                }
            }
            //if it is 8x8 sprite
            //memory[LCD_CTRL_REG] bit2 = 0
            else
            {
                getTile(oam_entry_ptr->tile_no, tile_buf_ptr, oam_entry_ptr->flip_y_flip_x);
                putimage(VIEWPORT_X + OAM_X_OFFSET + oam_entry_ptr->pos_x * PIXEL_SIZE, 
                         VIEWPORT_Y + OAM_Y_OFFSET + oam_entry_ptr->pos_y * PIXEL_SIZE, 
                         tile_buf_ptr, AND_PUT);
            }

            //setcolor(RED);
#if (ENABLE_DEBUG_RECTANGLE == 1)
            //memory[LCD_CTRL_REG] bit2 = 1, 8x16 sprite
            if ((memory[LCD_CTRL_REG] & OBJ_COMP_SEL))
            {
                rectangle(VIEWPORT_X + OAM_X_OFFSET + oam_entry_ptr->pos_x * PIXEL_SIZE, 
                          VIEWPORT_Y + OAM_Y_OFFSET + oam_entry_ptr->pos_y * PIXEL_SIZE, 
                          VIEWPORT_X + OAM_X_OFFSET + oam_entry_ptr->pos_x * PIXEL_SIZE + TILE_SIZE - 1, 
                          VIEWPORT_Y + OAM_Y_OFFSET + oam_entry_ptr->pos_y * PIXEL_SIZE + TILE_SIZE - 1);
            }
            //memory[LCD_CTRL_REG] bit2 = 0, 8x8 sprite
            else
            {
                rectangle(VIEWPORT_X + OAM_X_OFFSET + oam_entry_ptr->pos_x * PIXEL_SIZE, 
                          VIEWPORT_Y + OAM_Y_OFFSET + oam_entry_ptr->pos_y * PIXEL_SIZE, 
                          VIEWPORT_X + OAM_X_OFFSET + oam_entry_ptr->pos_x * PIXEL_SIZE + TILE_SIZE - 1, 
                          VIEWPORT_Y + OAM_Y_OFFSET + oam_entry_ptr->pos_y * PIXEL_SIZE + TILE_SIZE - 1);
            }
#endif
        }
    }
    void refreshLCD()
    {
#if (ENABLE_DOUBLE_GRAPHIC_BUFFER == 1)
        page = !page;
        setactivepage(page);
#endif


        //simple gui
        rectangle(LOAD_STATE_BUTTON_X, LOAD_STATE_BUTTON_Y, LOAD_STATE_BUTTON_X + LOAD_STATE_BUTTON_WIDTH, LOAD_STATE_BUTTON_Y + LOAD_STATE_BUTTON_HEIGHT);
        rectangle(SAVE_STATE_BUTTON_X, SAVE_STATE_BUTTON_Y, SAVE_STATE_BUTTON_X + SAVE_STATE_BUTTON_WIDTH, SAVE_STATE_BUTTON_Y + LOAD_STATE_BUTTON_HEIGHT);
        outtextxy(LOAD_STATE_BUTTON_X+2, LOAD_STATE_BUTTON_Y+2, "load state");
        outtextxy(SAVE_STATE_BUTTON_X+2, SAVE_STATE_BUTTON_Y+2, "save state");

        //printf("refreshLCD()\n");
        //GetSystemTime(&end_time);
        //printf("s: %d , ms : %d\n", begin_time.wSecond, begin_time.wMilliseconds);
        //printf("s: %d , ms : %d\n", end_time.wSecond, end_time.wMilliseconds);
        buildBackground();

        //viewport
        int scy = memory[SCY] * PIXEL_SIZE;
        int scx ;
        U16 scy_beg; 
        U16 scy_end;
        U16 scy_end_prev ;
        int viewport_x_end;
        int viewport_y_end;
        int wrap_x_width;
        int wrap_y_height;


        //for ly == lyc effect
        // ly   scx
        // 0    0
        // 0xF  0x26
        // 160  0
        //
        // part0 ly=0~0xF   , scx=0
        // part1 ly=0xF~143 , scx=0x26
        //
        //make last pair
        //this map store original metric, 1 means one pixel, do not count in scale_size
        //LCD_HEIGHT / SCALE_SIZE : restore to original metric 
        this->ly_scx_map[LCD_HEIGHT / SCALE_SIZE] = 0;
        //ly_scx_map should be at least size of 2, because 2 pair make 1 interval, so that code can run into "for(; iter != this->ly_scx_map.end(); iter++)", which does rendering
        //if there is no one, add one
        if (this->ly_scx_map.count(0x0) == 0)
        {
            this->ly_scx_map[0x0] = 0x0;
        }

        //iterator
        std::map<U16, U16>::iterator iter = this->ly_scx_map.begin();
        //make previous pair
        std::pair<U16, U16> prev_ly_scx = *iter;
        iter++;

        scy_beg; 
        scy_end = prev_ly_scx.first;
        scy_end_prev = 0;
        //printf("this->ly_scx_map.size() = %d\n", this->ly_scx_map.size());
        for(; iter != this->ly_scx_map.end(); iter++)
        {            
            scx          = prev_ly_scx.second * PIXEL_SIZE;
            scy_end_prev = scy_end;
            scy_beg      =  prev_ly_scx.first * PIXEL_SIZE;
            scy_end      =  (*iter).first * PIXEL_SIZE;

            //printf("LY : %02X ~ %02X , SCX = %02X\n", scy_beg, scy_end, scx);            

            viewport_x_end = BG_X + scx + LCD_WIDTH;
            viewport_y_end = BG_Y + scy + scy_end;

            if (viewport_x_end <= BG_WIDTH)
            {
                if (viewport_y_end <= BG_HEIGHT)
                {
                    getimage(BG_X + scx, 
                             BG_Y + scy + scy_beg, 
                             viewport_x_end - 1, 
                             viewport_y_end - 1, viewport_buf_ptr); 

                putimage(VIEWPORT_X, VIEWPORT_Y + scy_end_prev, viewport_buf_ptr, COPY_PUT);

#if (ENABLE_DEBUG_RECTANGLE == 1)
                        rectangle(BG_X + scx,
                                BG_Y + scy + scy_beg,
                                viewport_x_end - 1,
                                viewport_y_end - 1);
#endif
                }
                //part of y is normal, the other part needs to warp
                else 
                {                
                    getimage(BG_X + scx, 
                             BG_Y + scy + scy_beg, 
                             viewport_x_end - 1, 
                             BG_HEIGHT - 1, viewport_buf_ptr);

                putimage(VIEWPORT_X, VIEWPORT_Y + scy_end_prev, viewport_buf_ptr, COPY_PUT);

#if (ENABLE_DEBUG_RECTANGLE == 1)
                        rectangle(BG_X + scx,
                                BG_Y + scy + scy_beg,
                                viewport_x_end - 1,
                                BG_HEIGHT - 1);
#endif               
                }

                //y wrap part
                if (viewport_y_end > BG_HEIGHT)
                {
                    wrap_y_height = viewport_y_end - BG_HEIGHT;
                    getimage(BG_X + scx,
                             BG_Y,
                             viewport_x_end - 1,
                             BG_Y + wrap_y_height - 1, viewport_buf_ptr); 

                    putimage(VIEWPORT_X, VIEWPORT_Y + LCD_HEIGHT - wrap_y_height, viewport_buf_ptr, COPY_PUT);

#if (ENABLE_DEBUG_RECTANGLE == 1)
                        rectangle(BG_X + scx,
                                BG_Y,
                                viewport_x_end - 1,
                                BG_Y + wrap_y_height - 1);
#endif               

                }                
            }
            //part of X is normal, the other part needs to warp
            else
            {      
                if (viewport_y_end <= BG_HEIGHT)
                {
                    getimage(BG_X + scx,
                            BG_Y + scy + scy_beg,
                            BG_WIDTH - 1,
                            viewport_y_end - 1, viewport_buf_ptr);

                putimage(VIEWPORT_X, VIEWPORT_Y + scy_end_prev, viewport_buf_ptr, COPY_PUT);

#if (ENABLE_DEBUG_RECTANGLE == 1)
                    rectangle(BG_X + scx,
                            BG_Y + scy + scy_beg,
                            BG_WIDTH - 1,
                            viewport_y_end - 1);
#endif
                }
                //part of y is normal, the other part needs to warp
                else
                {
                    getimage(BG_X + scx,
                            BG_Y + scy + scy_beg,
                            BG_WIDTH - 1,
                            BG_HEIGHT - 1, viewport_buf_ptr);

                putimage(VIEWPORT_X, VIEWPORT_Y + scy_end_prev, viewport_buf_ptr, COPY_PUT);

#if (ENABLE_DEBUG_RECTANGLE == 1)
                    rectangle(BG_X + scx,
                            BG_Y + scy + scy_beg,
                            BG_WIDTH - 1,
                            BG_HEIGHT - 1);
#endif
                }

                //y wrap part
                if (viewport_y_end > BG_HEIGHT)
                {
                    wrap_y_height = viewport_y_end - BG_HEIGHT;

                    getimage(BG_X + scx,
                            BG_Y,
                            BG_WIDTH - 1,
                            BG_Y + wrap_y_height - 1, viewport_buf_ptr);

                    putimage(VIEWPORT_X, VIEWPORT_Y + LCD_HEIGHT - wrap_y_height, viewport_buf_ptr, COPY_PUT);

#if (ENABLE_DEBUG_RECTANGLE == 1)
                    rectangle(BG_X + scx,
                            BG_Y,
                            BG_WIDTH - 1,
                            BG_Y + wrap_y_height - 1);
#endif
                }

            }
            //printf("putimage(%02X, %02X)\n", VIEWPORT_X, VIEWPORT_Y + scy_end_prev);
            
            //x wrap part
            if (viewport_x_end > BG_WIDTH)
            {
                wrap_x_width   = viewport_x_end - BG_WIDTH;
                viewport_x_end = BG_WIDTH;

                if (viewport_y_end <= BG_HEIGHT)
                {
                    getimage(BG_X,
                            BG_Y + scy + scy_beg,
                            BG_X + wrap_x_width - 1,
                            viewport_y_end - 1,
                            //BG_Y + scy + scy_end - 1,
                            viewport_buf_ptr); 

                putimage(VIEWPORT_X + LCD_WIDTH - wrap_x_width, VIEWPORT_Y + scy_end_prev, viewport_buf_ptr, COPY_PUT);

#if (ENABLE_DEBUG_RECTANGLE == 1)
                        rectangle(BG_X,
                                    BG_Y + scy + scy_beg,
                                    BG_X + wrap_x_width - 1,
                                    viewport_y_end - 1);
#endif
                }
                //part of y is normal, the other part needs to warp
                else
                {
                    getimage(BG_X,
                            BG_Y + scy + scy_beg,
                            BG_X + wrap_x_width - 1,
                            BG_HEIGHT - 1,
                            viewport_buf_ptr); 

                putimage(VIEWPORT_X + LCD_WIDTH - wrap_x_width, VIEWPORT_Y + scy_end_prev, viewport_buf_ptr, COPY_PUT);

#if (ENABLE_DEBUG_RECTANGLE == 1)
                        rectangle(BG_X,
                            BG_Y + scy + scy_beg,
                            BG_X + wrap_x_width - 1,
                            BG_HEIGHT - 1);
#endif
                }

                //y wrap part
                if (viewport_y_end > BG_HEIGHT)
                {
                    wrap_y_height = viewport_y_end - BG_HEIGHT;

                    getimage(BG_X,
                            BG_Y,
                            BG_X + wrap_x_width - 1,
                            BG_Y + wrap_y_height - 1, viewport_buf_ptr);

                    putimage(VIEWPORT_X + LCD_WIDTH - wrap_x_width, VIEWPORT_Y + LCD_HEIGHT - wrap_y_height, viewport_buf_ptr, COPY_PUT);

#if (ENABLE_DEBUG_RECTANGLE == 1)
                    rectangle(BG_X,
                        BG_Y,
                        BG_X + wrap_x_width - 1,
                        BG_Y + wrap_y_height - 1);
#endif
                }         

            }
            prev_ly_scx = *iter;
        }
        //clear map
        this->ly_scx_map.clear();


        //LY == LYC workaround
        //getimage(BG_X, BG_Y, BG_X + LCD_WIDTH, BG_Y + this->fixed_ly * PIXEL_SIZE, viewport_buf_ptr);
        //putimage(VIEWPORT_X, VIEWPORT_Y, viewport_buf_ptr, COPY_PUT);

        //render window , (overlay)
        updateWindow();
        //printf("\n");

        //render sprite ,OAM
        updateOAM();


#if (ENABLE_DOUBLE_GRAPHIC_BUFFER == 1)
        setvisualpage(page);
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
        U16 debug_pc = 0x1FF7;
        U16 debug_de = 0x8002;

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
            
            if (detectMouseClick())
            {
                //click_debug = !click_debug;

                //simple gui
                //load state
                if ((LOAD_STATE_BUTTON_X < this->mouse_x) && (this->mouse_x < LOAD_STATE_BUTTON_X + LOAD_STATE_BUTTON_WIDTH) &&
                    (LOAD_STATE_BUTTON_Y < this->mouse_y) && (this->mouse_y < LOAD_STATE_BUTTON_Y + LOAD_STATE_BUTTON_HEIGHT))
                {
                    printf("loadCpuState %d\n", cpu_cycles);
                    this->loadCpuState();
                }

                //save state
                if ((SAVE_STATE_BUTTON_X < this->mouse_x) && (this->mouse_x < SAVE_STATE_BUTTON_X + SAVE_STATE_BUTTON_WIDTH) &&
                    (SAVE_STATE_BUTTON_Y < this->mouse_y) && (this->mouse_y < SAVE_STATE_BUTTON_Y + SAVE_STATE_BUTTON_HEIGHT))
                {
                    printf("loadCpuState %d\n", cpu_cycles);
                    this->saveCpuState();
                }


            }
            cpu_cycles++;
            update_lcd_y_coord();
            update_timer();

            check_interrupt_and_dispatch_isr();

            if ((this->refresh_lcd == TRUE) && (memory[LCD_CTRL_REG] & LCD_ON_OFF_FLAG))
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
                xx     = memory[pc + 1];
                aabb   = (memory[pc + 2] << 8) | memory[pc + 1];
                b      = (memory[pc + 1] >> 3) & 0x7;

                ly     = memory[LY];


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
                log[log_idx].palette_data = this->memory[BG_PALETTE_DATA];
                current_log = log[log_idx];
                log_idx++;


                if (this->pc == debug_pc)
                {
                    printREG();
                }

                if (this->de.all == debug_de)
                {
                    //printREG();
                }

                if (memory[0xFF47] == 0x0)
                {
                    //printREG();
                    int abc;
                    abc = 10;
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
                    //printf("DI cpu_cycles = %08X\n", cpu_cycles);
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
    //U16 bg_tile_ram_beg_addr = ((cpu->memory[LCD_CTRL_REG] & BG_CHAR_SEL_FALG) ? 0x8000 : 0x8800);
    U16 bg_tile_ram_end_addr = ((cpu->memory[LCD_CTRL_REG] & BG_CHAR_SEL_FALG) ? 0x8FFF : 0x97FF);

    //8000h ~ A000h
    //BREAK_POINT(access_addr == 0xFF47);
    if (((VIDEO_RAM_BASE <= access_addr) && (access_addr <= bg_tile_ram_end_addr)))
        //if (((VIDEO_RAM_BASE <= access_addr) && (access_addr <= (VIDEO_RAM_BASE + 0x1000))))
    {
        
        //we need to collect two byte before painting one row
       /* if ((access_addr & 0x1) == 0x1)
        {
            if (val)
            {
                //printf("access_addr = %04X\n", access_addr);
                //printf("%02X %02X\n", (U8)(cpu->memory[access_addr - 1]), val);
            }
            tile_dot_data_painter->paintTileDotData(access_addr, (U8)(cpu->memory[access_addr - 1]), val, BG_PALETTE_DATA);
        }
        //this->value = val;
        */


        (*(this->raw_byte_ptr)) = val;

        //we need to collect 16 byte before painting one tile
        if ((access_addr & 0xF) == 0xF)
        {
            //arrange tile location
            int dot_data_byte_idx = access_addr;
            U8 dots_one_row = 0x7F;   //128 dots in a row, 16 tile in a row
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
                                            // 12 / 8 = 1     -> x_pos = 1*8
                                            // 12 / 128 = 0   -> y_tile_row = 0*8
                                            // (12 % 8) = 4 -> y_pos = y_row + 4


            dot_data_byte_idx = access_addr - VIDEO_RAM_BASE;
            //dot_data_idx = (dot_data_byte_idx >> 1) , because two bytes represent one row
            U16 dot_data_idx = (dot_data_byte_idx >> 1);
            //each tile contains 64 dots
            // 12 / 8 = 1     -> x_pos = 1*8
            int x_pos = BG_TILE_BGI_BUFFER_X + (((dot_data_idx & dots_one_row) >> 3) * TILE_SIZE);
            // 12 / 128 = 0   -> y_tile_row = 0 * 8
            int y_pos = (dot_data_idx >> dots_one_row_shift) * TILE_SIZE;

            TILE_DOT_DATA_PAINTER::paintOneTile(x_pos, y_pos, &this->cpu->memory.raw_byte_data[access_addr - 0xF] ,BG_PALETTE_DATA);
        
        
        }


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

        this->cpu->mbc_ptr->bank_switching(this->access_addr, val);

        //this->cpu

        /*
        //for MBC 
        if (this->access_addr == MBC1_REGISTER_1)
        {
            ASSERT("mbc1 can only be assigned <= 3",val <= 3);
            this->cpu->memory.mbc1.register1 = val;
            //bank switching
            void *bank_tgt_addr = (void *)&this->cpu->memory.raw_byte_data[ROM_BANK1_START];
            // (val - 1) because bank0 is not stored here, I only store bank1 bank2 bank3
            void *bank_src_addr = (void *)&this->cpu->memory.raw_byte_data[BANK_SWITCHING_SPARE_ADDR + (val - 1) * ROM_BANK_SIZE];

            memcpy(bank_tgt_addr, bank_src_addr, ROM_BANK_SIZE);
        }
        else
        {
            //normal read-only ram case
        }
        */
        return (*this);
    }
    else if (this->access_addr == SCX)
    {
        //in order to implement that each scanline(ly) corresponds to different scx
        //because my rendering system is not rendering line by line
        //emulator can only collect all <ly,scx> pair
        //and at last, rendering screen all at once
        U8 ly;
        U8 scx;
        U8 last_scx = (*(this->raw_byte_ptr));
        U8 ly_lyc_match_flag = (this->cpu->memory[LCD_STAT] & LCD_MATCH_FLAG) ? 1 : 0;

        if(this->cpu->memory[LCD_STAT] & LCD_MATCH_FLAG)
        {
            ly = this->cpu->lyc_match_ly;
            scx = val;
            this->cpu->ly_scx_map[ly] = scx;
            //printf("pc: %04X  (ly , scx ,match_flag) = (0x%02X , 0x%02X , 0x%02X)\n",this->cpu->pc, ly, val,ly_lyc_match_flag);

        }
        else
        {
            this->cpu->ly_scx_map[0x0] = val;

            //printf("pc: %04X  (ly , scx ,match_flag) = (0x%02X , 0x%02X , 0x%02X)\n",this->cpu->pc, 0, val,ly_lyc_match_flag);
        }

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
    //TODO : bug , so set fixed 0xE4 temporarily
    U8  bg_palette_color = 0xE4;//TILE_DOT_DATA_PAINTER::cpu->memory[which_palette];
    U8  palette_code_to_color_code[4] = { (bg_palette_color >> 0) & 0x3,(bg_palette_color >> 2) & 0x3,(bg_palette_color >> 4) & 0x3,(bg_palette_color >> 6) & 0x3 };

    //back up current palette
    //switch (which_palette) 
    //{
    //case :
    //}

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


MBC_BASE::MBC_BASE(std::string str, DMGZ80CPU *c)
{
    this->cpu                        = c;
    this->rom_file_name              = str;
    this->bank_switched_rom_addr     = (char*)&this->cpu->memory.raw_byte_data[ROM_BANK1_START];
    this->bank_switched_ext_ram_addr = (char*)&this->cpu->memory.raw_byte_data[EXTERNAL_RAM_BASE];

}



MBC3::MBC3() :MBC_BASE()
{
    printf("default constructor\n");
    memset((void*)(this->external_ram), 0x0, MBC3_RAM_SIZE);
    cur_bank_no = 0;
    rtc_s = &this->cpu->memory.raw_byte_data[EXTERNAL_RAM_BASE];
    rtc_m = &this->cpu->memory.raw_byte_data[EXTERNAL_RAM_BASE];
    rtc_h = &this->cpu->memory.raw_byte_data[EXTERNAL_RAM_BASE];
    rtc_dl = &this->cpu->memory.raw_byte_data[EXTERNAL_RAM_BASE];
    rtc_dh = &this->cpu->memory.raw_byte_data[EXTERNAL_RAM_BASE];
}

MBC3::MBC3(std::string str, DMGZ80CPU *c) : MBC_BASE(str, c)
{
    printf("constructor with filename and cpu_ptr\n");
    memset((void*)(this->external_ram), 0x0, MBC3_RAM_SIZE);
    cur_bank_no = 0;
    rtc_s = &this->cpu->memory.raw_byte_data[EXTERNAL_RAM_BASE];
    rtc_m = &this->cpu->memory.raw_byte_data[EXTERNAL_RAM_BASE];
    rtc_h = &this->cpu->memory.raw_byte_data[EXTERNAL_RAM_BASE];
    rtc_dl = &this->cpu->memory.raw_byte_data[EXTERNAL_RAM_BASE];
    rtc_dh = &this->cpu->memory.raw_byte_data[EXTERNAL_RAM_BASE];
}

void TILE_DOT_DATA_PAINTER::paintOneTile(int x_start_pos, int y_start_pos, U8 *tile_dot_raw_data_addr, U16 which_palette)
{
    int bgi_color;
    int y          = y_start_pos;
    U8  color_code = 0;
    U8  upper_dot_byte;
    U8  lower_dot_byte;   

    for (int i = 0; i < TILE_BYTE_SIZE; i += 2) 
    {
        for (int x = x_start_pos; x < (x_start_pos + TILE_SIZE); x += PIXEL_SIZE)
        {
#if (ENABLE_DOUBLE_GRAPHIC_BUFFER == 1)
            for (int page = 0; page < PAGE_NUM; page++)
            {
                setactivepage(page);
#endif
                upper_dot_byte = tile_dot_raw_data_addr[i + 0];
                lower_dot_byte = tile_dot_raw_data_addr[i + 1];

                color_code = 0;
                color_code += (upper_dot_byte & BIT(7 - ((x / PIXEL_SIZE) & 0x7))) ? 2 : 0;
                color_code += (lower_dot_byte & BIT(7 - ((x / PIXEL_SIZE) & 0x7))) ? 1 : 0;

                bgi_color = paletteCodeToColor(color_code, which_palette);
                //putpixel(x, y_pos, bgi_color);

                //if ((0x8060 <= TILE_DOT_DATA_PAINTER::cpu->memory.access_addr) && (TILE_DOT_DATA_PAINTER::cpu->memory.access_addr <= 0x8080))
                {
                    //printf("bgi_color = %08X\n", bgi_color);
                }
                put_super_pixel(x, y, bgi_color, SCALE_SIZE);

#if (ENABLE_DEBUG_RECTANGLE == 1)
                ////rectangle(x_start_pos, y_start_pos, x_start_pos + TILE_SIZE - 1, y_start_pos + TILE_SIZE - 1);
#endif   

#if (ENABLE_DOUBLE_GRAPHIC_BUFFER == 1)
            }
#endif
        }
        y += PIXEL_SIZE;
    }
}

//paint 1 row, 8 dots
//in BG_TILE_BGI_BUFFER ,each row accommodates 16 tile (128 dots)
void TILE_DOT_DATA_PAINTER::paintTileDotData(U16 dot_data_byte_idx, U8 upper_dot_byte, U8 lower_dot_byte, U16 which_palette)
{
    U8 dots_one_row       = 0x7F;   //128 dots in a row, 16 tile in a row
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
                                    // 12 / 8 = 1     -> x_pos = 1*8
                                    // 12 / 128 = 0   -> y_tile_row = 0*8
                                    // (12 % 8) = 4 -> y_pos = y_row + 4


    dot_data_byte_idx = dot_data_byte_idx - VIDEO_RAM_BASE;
    //dot_data_idx = (dot_data_byte_idx >> 1) , because two bytes represent one row
    U16 dot_data_idx = (dot_data_byte_idx >> 1);
    //each tile contains 64 dots
    // 12 / 8 = 1     -> x_pos = 1*8
    int x_pos = BG_TILE_BGI_BUFFER_X + (((dot_data_idx & dots_one_row) >> 3) * TILE_SIZE);
    // 12 / 128 = 0   -> y_tile_row = 0 * 8
    int y_tile_row = (dot_data_idx >> dots_one_row_shift) * TILE_SIZE;
    // (12 % 8) = 4 -> y_pos = y_row + 4
    int y_pos = BG_TILE_BGI_BUFFER_Y + y_tile_row + (((dot_data_idx) & 0x7)) * PIXEL_SIZE;
    int bgi_color;
    U8  color_code = 0;

    if (debug_draw_bar == 1)
    {
        //rectangle(x_pos, y_pos, x_pos + TILE_SIZE - 1, y_pos + 1);
    }
    else
    {
        //render one row of a tile
        for (int x = x_pos; x < (x_pos + TILE_SIZE); x += PIXEL_SIZE)
        {
#if (ENABLE_DOUBLE_GRAPHIC_BUFFER == 1)
            for (int page = 0; page < PAGE_NUM; page++)
            {
                setactivepage(page);
#endif
                color_code = 0;
                color_code += (upper_dot_byte & BIT(7 - ((x / PIXEL_SIZE) & 0x7))) ? 2 : 0;
                color_code += (lower_dot_byte & BIT(7 - ((x / PIXEL_SIZE) & 0x7))) ? 1 : 0;
                
                bgi_color = paletteCodeToColor(color_code, which_palette);
                //putpixel(x, y_pos, bgi_color);

                //if ((0x8060 <= TILE_DOT_DATA_PAINTER::cpu->memory.access_addr) && (TILE_DOT_DATA_PAINTER::cpu->memory.access_addr <= 0x8080))
                {
                    //printf("bgi_color = %08X\n", bgi_color);
                }
                put_super_pixel(x, y_pos, bgi_color, SCALE_SIZE);
#if (ENABLE_DOUBLE_GRAPHIC_BUFFER == 1)
            }
#endif
        }
    }
}
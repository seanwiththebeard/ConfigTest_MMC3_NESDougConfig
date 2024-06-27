//#link "chr_generic.s"
//#resource "MMC3_128_128.cfg"

#define NES_MAPPER 4
#define	NES_PRG_BANKS 8
#define	NES_CHR_BANKS 16
#define	NES_MIRRORING 0

//#resource "MMC3_128_128.cfg"
#define CFGFILE MMC3_128_128.cfg

typedef unsigned char byte;	// 8-bit unsigned
typedef signed char sbyte;	// 8-bit signed
typedef unsigned short word;	// 16-bit signed
 
//Uncomment this below line and recomment it to fix the error...
//I don't completely understand what's going on here
////#link "crt0.s"

//#resource "crt0.s"
//#resource "neslib.s"
//#resource "famitone2.sinc"
//#resource "Gears.chr"
//#resource "Alpha.chr"
//#resource "mmc3_code.asm"
//#resource "nesdoug.s"
//#resource "famitone2.s"
//#resource "TestMusic3.s"
//#resource "SoundFx.s"
//#resource "neslib.sinc"
//#resource "lz4vram.s"


#include "neslib.h"


//#include "neslib.h"
#include <peekpoke.h>

#define MMC_MODE 0x00

#define MMC3_SET_REG(r,n)\
	POKE(0x8000, MMC_MODE|(r));\
	POKE(0x8001, (n));

#define MMC3_CHR_0000(n) MMC3_SET_REG(0,n)
#define MMC3_CHR_0800(n) MMC3_SET_REG(1,n)
#define MMC3_CHR_1000(n) MMC3_SET_REG(2,n)
#define MMC3_CHR_1400(n) MMC3_SET_REG(3,n)
#define MMC3_CHR_1800(n) MMC3_SET_REG(4,n)
#define MMC3_CHR_1C00(n) MMC3_SET_REG(5,n)
#define MMC3_PRG_8000(n) MMC3_SET_REG(6,n)
#define MMC3_PRG_A000(n) MMC3_SET_REG(7,n)

#pragma bss-name(push, "XRAM")
// extra RAM at $6000-$7fff
unsigned char wram_array[0x2000];
#pragma bss-name(pop)

const unsigned char palette_bg[]={
0x02, 0, 0x10, 0x30,
0x0f, 0, 0, 0,
0x0f, 0, 0, 0,
0x0f, 0, 0, 0
}; 

const unsigned char palette_spr[]={
0x0f, 0x09, 0x19, 0x29, // greens
0x0f, 0, 0, 0,
0x0f, 0, 0, 0,
0x0f, 0, 0, 0
}; 

const unsigned char debugString[] = "This is the debug string@"; //Called directly letter by letter
const unsigned char stringMain[] = "Hello main code segment@";
const unsigned char string3[] = "Hello Bank 0@";
const unsigned char string4[] = "Hello Bank 1@";
const unsigned char string5[] = "Hello Bank 2@";
const unsigned char string6[] = "Hello Bank 3@";

void draw_text(word addr, char *text) {
  byte tx = 0;
  vram_adr(addr);
  for (tx = 0; text[tx]!='@'; ++tx)
  {
    vram_put(text[tx]);
  }
}

// test putting things in other banks
#pragma rodata-name ("BANK0")
#pragma code-name ("BANK0")
void FunctionCODE0()
{
  byte x;
  draw_text(NTADR_A(2, 15), "draw_text Bank 0@");
  vram_adr(NTADR_A(2,16));
  for (x = 0; string3[x]!='@';++x)
    vram_put(string3[x]);
}
#pragma rodata-name ("BANK1")
#pragma code-name ("BANK1")
void FunctionCODE1()
{
  byte x;
  draw_text(NTADR_A(2, 17), "draw_text Bank 1@");
  vram_adr(NTADR_A(2,18));
  for (x = 0; string4[x]!='@';++x)
    vram_put(string4[x]);
}

#pragma rodata-name ("BANK2")
#pragma code-name ("BANK2")
void FunctionCODE2()
{
  byte x;
  draw_text(NTADR_A(2, 19), "draw_text Bank 2@");
  vram_adr(NTADR_A(2,20));
  for (x = 0; string5[x]!='@';++x)
    vram_put(string5[x]);
}

#pragma rodata-name ("BANK3")
#pragma code-name ("BANK3")
void FunctionCODE3()
{
  byte x;
  draw_text(NTADR_A(2, 21), "draw_text Bank 3@");
  vram_adr(NTADR_A(2,22));
  for (x = 0; string6[x]!='@';++x)
    vram_put(string6[x]);
}
	
	
// the fixed bank, bank 7
#pragma rodata-name ("CODE")
#pragma code-name ("CODE")	
void main (void) 
{
  byte x = 0;
  //MMC3_PRG_8000(0); //Fixed Bank?
  //MMC3_PRG_A000(3); //Swapable Bank
  
  pal_bg(palette_bg);
  vram_adr(NTADR_A(2,1));
  
  for (x = 0; debugString[x]!='@';++x)
    vram_put(debugString[x]);
  
  draw_text(NTADR_A(2, 3), (char*)stringMain);
  vram_adr(NTADR_A(2,4));
  
  for (x = 0; stringMain[x]!='@';++x)
    vram_put(stringMain[x]);
  
  MMC3_PRG_8000(0);
  FunctionCODE0();
  
  MMC3_PRG_8000(1);
  FunctionCODE1();
  
  MMC3_PRG_8000(2);
  FunctionCODE2();
  
  MMC3_PRG_8000(3);
  FunctionCODE3();
  
  ppu_on_all();
  
  music_play(0);
  
  while (1)
  {
    ++x;
    if (x > 8)
      x = 0;
    delay(15);
    pal_col(0,x);
  }
}


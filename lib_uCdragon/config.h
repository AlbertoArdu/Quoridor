
//********************************************************************************************************/

#include "def.h"


#ifndef TRUE
#define TRUE  1
#endif

#ifndef FALSE
#define FALSE 0
#endif
		

#define NET_DEBUG		0

//#define	SET_SYSCLK_SUPPORT
#define	RTC_TIMER_SUPPORT
#define	INTERRUPT_SUPPORT
//#define	HARDWARE_TIMER_SUPPORT

#ifndef  __OUT_CHIP
	#define	NOR_FLASH_SUPPORT
#endif

#define NOR_FLASH_SUPPORT
#ifdef	NOR_FLASH_SUPPORT
#define	SST39VF160_SUPPORT
#endif
#if defined(NOR_FLASH_SUPPORT) || defined(NAND_FLASH_SUPPORT)
//#define	SAVE_ENV_SUPPORT
#endif

#define	TIMER_FREQ	1000

#define	CMD_SHELL_SUPPORT



#define	SERIAL_PORTS		2
#define	DEFAULT_SERIAL_PORT	0
#define	DEFAULT_SERIAL_BAUD	115200

//#define	IDE_DISK_SUPPORT
#define	INT_ROM_BASE	0		//External Flash
#define	INT_ROM_SIZE	SIZE_256K		//Interrupt Flash size

#define	EXT_ROM_BASE	0x080000000		//External Flash:NorFlash
#define	EXT_ROM_SIZE	SIZE_2M		//External Flash size

#define	SRAM_BASE	0x081000000
#define	SRAM_SIZE	SIZE_512K

#define	_MMUTT_SIZE	SIZE_16K

#define	INT_ROM_END			(INT_ROM_BASE + INT_ROM_SIZE)
#define	EXT_ROM_END			(EXT_ROM_BASE + EXT_ROM_SIZE)
#define	SRAM_END		(SRAM_BASE + SRAM_SIZE)
#define	BIOS_RUN_ADDR	(SRAM_END - SIZE_256K)
#define	_MMUTT_STARTADDRESS	(SRAM_END - _MMUTT_SIZE)
#define	_ISR_STARTADDRESS	(_MMUTT_STARTADDRESS-0x100)

#define	BOOT_PARAMS_ADDR	(SRAM_BASE+0x100)
#define	BIOS_SIZE			(SIZE_64K)
#define	BIOS_BASE			(EXT_ROM_SIZE-BIOS_SIZE)
#define	BIOS_LOAD			(EXT_ROM_SIZE-4)
#define BIOS_ENV_ADDR		(EXT_ROM_SIZE-0x200)

#define	DFT_DOWNLOAD_ADDR	(SRAM_BASE + 0x0000)
#define	DFT_INITRD_ADDR		(SRAM_BASE + (SRAM_SIZE>>1))

#if (SERIAL_PORTS>1)
#define	SERIAL_PORTS_SWITCH
#endif


#include	"LPC2294.h"

/* System setting, Fosc¡¢Fcclk¡¢Fcco¡¢Fpclk */
#define Fosc			14745600					//crystal Frequency:10MHz~25MHz
#define Fcclk   		(Fosc * 4)  				//System Frequency:Fosc*(1~32)£¬and <=60MHZ
#define Fcco			(Fcclk * 4) 				//CCO Frequency:Fcclk*(2¡¢4¡¢8¡¢and 16)£¬range from 156MHz~320MHz
#define Fpclk   		(Fcclk / 4) * 1 			//VPB clock Frequency:(Fcclk / 4)


/*********************************************************************************************************
**  						  End Of File
********************************************************************************************************/

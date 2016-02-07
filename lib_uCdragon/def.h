#ifndef __DEF_H__
#define __DEF_H__

typedef unsigned char uint8;				   
typedef signed   char int8; 				  
typedef unsigned short uint16;  				
typedef signed   short int16;   				
typedef unsigned int uint32;				
typedef signed   int int32; 				 
typedef float fp32; 				  
typedef double fp64;					

typedef unsigned char U8;   				
typedef signed   char S8;   				 
typedef unsigned short U16; 				 
typedef signed   short S16; 				  
typedef unsigned int U32;   			   
typedef signed   int S32;   				

#define	BYTE	char
#define	WORD 	short
#define	DWORD	int
#define	UINT	U32
#define	LPSTR	(U8 *)
#define	BOOL	int
#define	USHORT	U16

#ifndef TRUE
#define TRUE  1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#define	NULL	(void *)0

#define	SIZE_1K		0x00000400
#define	SIZE_2K		0x00000800
#define	SIZE_4K		0x00001000
#define	SIZE_8K		0x00002000
#define	SIZE_16K	0x00004000
#define	SIZE_32K	0x00008000
#define	SIZE_64K	0x00010000
#define	SIZE_128K	0x00020000
#define	SIZE_256K	0x00040000
#define	SIZE_512K	0x00080000
#define	SIZE_1M		0x00100000
#define	SIZE_2M		0x00200000
#define	SIZE_4M		0x00400000
#define	SIZE_8M		0x00800000
#define	SIZE_16M	0x01000000
#define	SIZE_32M	0x02000000
#define	SIZE_64M	0x04000000
#define	SIZE_128M	0x08000000
#define	SIZE_256M	0x10000000
#define	SIZE_512M	0x20000000
#define	SIZE_1G		0x40000000
#define	SIZE_2G		0x80000000

#endif /*__DEF_H__*/


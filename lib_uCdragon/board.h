#ifndef __BOARD_H
#define __BOARD_H

#include "def.h"

#ifdef __cplusplus
extern "C"
{
#endif

	extern void __ENTRY( void );

#define min(x1,x2) ((x1<x2)? x1:x2)
#define max(x1,x2) ((x1>x2)? x1:x2)

	void PortInit( void );
	void TargetResetInit( void );
	void ExternalInterruptInit( void );
	void LedSet( U32 m );

#ifdef __cplusplus
}
#endif

#endif /*__BOARD_H*/

// <<< Use Configuration Wizard in Context Menu >>>	

#include "../LPC2294.h"
#include "timer.h"

void init_timer(){
	
	T0TCR=0x02;	//reset		

// <h> timer0 MCR
//   <o.0> MR0I
//	 <i> 1 Interrupt on MR0: an interrupt is generated when MR0 matches the value in the TC. 0
//	 <i> 0 This interrupt is disabled
//   <o.1> MR0R
//	 <i> 1 Reset on MR0: the TC will be reset if MR0 matches it.
//	 <i> 0 Feature disabled.
//   <o.2> MR0S
//	 <i> 1 Stop on MR0: the TC and PC will be stopped and TCR[0] will be set to 0 if MR0 matches the TC
//	 <i> 0 Feature disabled.
//   <o.3> MR1I
//	 <i> 1 Interrupt on MR1: an interrupt is generated when MR0 matches the value in the TC. 0
//	 <i> 0 This interrupt is disabled
//   <o.4> MR1R
//	 <i> 1 Reset on MR1: the TC will be reset if MR0 matches it.
//	 <i> 0 Feature disabled.
//   <o.5> MR1S
//	 <i> 1 Stop on MR1: the TC and PC will be stopped and TCR[1] will be set to 0 if MR1 matches the TC
//	 <i> 0 Feature disabled.
//   <o.6> MR2I
//	 <i> 1 Interrupt on MR2: an interrupt is generated when MR2 matches the value in the TC.
//	 <i> 0 This interrupt is disabled
//   <o.7> MR2R
//	 <i> 1 Reset on MR2: the TC will be reset if MR2 matches it.
//	 <i> 0 Feature disabled.
//   <o.8> MR2S
//	 <i> 1 Stop on MR2: the TC and PC will be stopped and TCR[2] will be set to 0 if MR2 matches the TC
//	 <i> 0 Feature disabled.
//   <o.9> MR3I
//	 <i> 1 Interrupt on MR3: an interrupt is generated when MR3 matches the value in the TC.
//	 <i> 0 This interrupt is disabled
//   <o.10> MR3R
//	 <i> 1 Reset on MR3: the TC will be reset if MR3 matches it.
//	 <i> 0 Feature disabled.
//   <o.11> MR3S
//	 <i> 1 Stop on MR3: the TC and PC will be stopped and TCR[3] will be set to 0 if MR3 matches the TC
//	 <i> 0 Feature disabled.
		T0MCR = 0x003;
//   </h>

//	 <h> Timer0 Match Register 0
//   <o.0..31> Setup value on 32 bits <0x00000000-0xFFFFFFFF>
//	 <i> Value K to count
//	 <i> K = FR [1/s] * T [s]
		T0MR0 = 0x008FFE8; //B 3FE2
//   </h>
	
	T0TCR=0x01; //enable count
	
	VICIntEnable=0x00020010;
	VICIntSelect=0x00000010;
	
	return;
}

void stop_timer0( void ){
		T0TCR=0x00;
}

void resume_timer0( void ){
		T0TCR=0x01;
}

void reset_intr_timer0( int i ){
	T0IR=i;		
}

//*** <<< end of configuration section >>>    ***

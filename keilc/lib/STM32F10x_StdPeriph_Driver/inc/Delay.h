

#ifndef _DELAY_
#define _DELAY_


#ifdef __cplusplus
extern "C"{
#endif
	
void Fn_DELAY_Short (unsigned int _vrui_Time);
void Fn_DELAY_Long  (unsigned int _vrui_Time);

void Fn_DELAY_Init (unsigned char _CLK);
void Fn_DELAY_us (unsigned long _vrui_Time);
void Fn_DELAY_ms (unsigned int _vrui_Time);

#ifdef __cplusplus
}
#endif

#endif



#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include <stdio.h>

void Delay(unsigned int _Time){		// ham Delay 
	unsigned int i;
	while(_Time--){
		for(i=0;i<100;i++);
	}
}
void Fn_GPIO_Init(void);
void Chon_LED(int);
int Dem(int);
int TinHieu(uint32_t, int);
int fullresult(int);
int check_row(int );
int get_val(int *s);														

const int phim[4][4] = { {1, 2, 3, 12},
                         {4, 5, 6, 13},
	 				  			 			 {7, 8, 9, 14},
                         {10, 0, 11,15},};
/*			1   2  3  +
				4   5  6  -
				7   8  9  mod
  			*   0  /  =  
*/
												 
uint8_t LED[11] = {0xFC, 0x60, 0xDA, 0xF2, 0x66, 0xB6, 0xBE, 0xE0, 0xFE, 0xF6,0xFF};
uint8_t FULL[4] = {0x8E, 0x7C, 0x1C, 0x1C};
// bat led co vi tri
uint8_t Quet[8] = {0x7F, 0xBF, 0xDF, 0xEF, 0xF7, 0xFB, 0xFD, 0xFE};

int main(){
	SystemInit();
	SystemCoreClockUpdate();
	Fn_GPIO_Init();
	int sign1=12,sign2,pt1=0,result=0;
	TinHieu(pt1,Dem(pt1));
	while(1){
		if(sign1==12){ // phep cong +
			pt1=get_val(&sign2);
			result=result+pt1;
		}
		else if(sign1==13){ // phep tru -
			pt1=get_val(&sign2);
		result=result-pt1;
		}
		else if(sign1==10){ // phep nhan *
			pt1=get_val(&sign2);
			result=result*pt1;
		}
		else if(sign1==11){ // phep chia /
			pt1=get_val(&sign2);
			result=result/pt1;
		}
		else if(sign1==14){ // mod
			pt1=get_val(&sign2);
			result=result%pt1;
		}
		else if(sign1==15){ // =
			int a,b=0;
			while(1){//kiem tra nut nhan
				if(result>9999) fullresult(3);
				else TinHieu(result,Dem(result));
				GPIO_WriteBit(GPIOB,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_10|GPIO_Pin_11,Bit_RESET);//cho cac hang o muc 0
				if(!GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0))a=0;
				else if(!GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1))a=0;
				else if(!GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2))a=0;
				else if(!GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3))a=0;
				else a=1;
				if(a==0 && b==1)break;
				b=a;
			}
			result=0;
			pt1=0;
			sign2=12;
		}
		sign1=sign2;
	}
}

void Fn_GPIO_Init(void){//khoi tao cau hinh
	GPIO_InitTypeDef	GPIOInitStruct; // khai bao struct GPIO_Init
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB, ENABLE);//cap xung cho port A va B
	GPIOInitStruct.GPIO_Pin = GPIO_Pin_0| GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;//col 0->3
	GPIOInitStruct.GPIO_Mode = GPIO_Mode_IPU;//che do doc
	GPIOInitStruct.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOA, &GPIOInitStruct);
	
	GPIOInitStruct.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_10|GPIO_Pin_11
	|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;
	//row 0->3:pin0,1,10,11 led 7 doan:7(SCK  SH_CP)dich 8(RCK  ST_CP)chot 9(DI)
	GPIOInitStruct.GPIO_Mode = GPIO_Mode_Out_PP;//1 or 0
	GPIOInitStruct.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOB,&GPIOInitStruct);
}

//led 7 doan:7(SCK  SH_CP) 8(RCK  ST_CP) 9(DI)
void Chon_LED(int j) {//led hien thi gia tri
	unsigned int i;
	for(i=0;i<8;i++) {
		GPIO_ResetBits(GPIOB, GPIO_Pin_7);//SH_CP SCK
		if(((Quet[j]>>i) & 1)==1) GPIO_SetBits(GPIOB, GPIO_Pin_9);//DI
		else if(((Quet[j]>>i) &1)==0) GPIO_ResetBits(GPIOB, GPIO_Pin_9);//DI
		GPIO_SetBits(GPIOB, GPIO_Pin_7);//SH_CP SCK
	}
}

int TinHieu(uint32_t t, int j) {//gia tri cua so
	unsigned int i;
	Chon_LED(j);

	for(i=0;i<8;i++) {
		GPIO_ResetBits(GPIOB, GPIO_Pin_7);//SH_CP SCK
		if(((LED[t%10]>>i) & 1)==1) GPIO_SetBits(GPIOB, GPIO_Pin_9);//DI
		else if(((LED[t%10]>>i) &1)==0) GPIO_ResetBits(GPIOB, GPIO_Pin_9);//DI
		GPIO_SetBits(GPIOB, GPIO_Pin_7);//SH_CP SCK
	}
	GPIO_ResetBits(GPIOB, GPIO_Pin_8);//ST_CP RCK
	GPIO_SetBits(GPIOB, GPIO_Pin_8);//ST_CP RCK
	Delay(1);//quan trong
	if(j>0) return TinHieu(t/10, --j);
	return 0;
}
int fullresult(int j) {//gia tri cua so
	unsigned int i;
	Chon_LED(j);
	for(i=0;i<8;i++) {
		GPIO_ResetBits(GPIOB, GPIO_Pin_7);//SH_CP SCK
		if(((FULL[j]>>i) & 1)==1) GPIO_SetBits(GPIOB, GPIO_Pin_9);//DI
		else if(((FULL[j]>>i) &1)==0) GPIO_ResetBits(GPIOB, GPIO_Pin_9);//DI
		GPIO_SetBits(GPIOB, GPIO_Pin_7);//SH_CP SCK
	}
	GPIO_ResetBits(GPIOB, GPIO_Pin_8);//ST_CP RCK
	GPIO_SetBits(GPIOB, GPIO_Pin_8);//ST_CP RCK
	Delay(1);//quan trong
	if(j>0) return fullresult( --j);
	return 0;
}

int Dem(int n){
	int num=0;
	while(n!=0) {
		num++;
		n/=10;
	}
	if(!num) return 0;
	return num-1;
}

int get_val(int *s){//lay gia tri tu ban phim
	int pt=0,a,b=0,col,row;
	while(1){		
		GPIO_WriteBit(GPIOB,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_10|GPIO_Pin_11,Bit_RESET);//cho cac hang o muc 0
		if(!GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)){col=0;a=0;}
		else if(!GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)){col=1;a=0;}
		else if(!GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2)){col=2;a=0;}
		else if(!GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3)){col=3;a=0;}
		else a=1;
		if(a==0 && b==1){
			row = check_row(col);
			if(phim[row][col]>9 ){
				*s=phim[row][col];
				break;
			}
			else if(pt>99999999)continue;
			else pt=pt*10+phim[row][col];
		}
		TinHieu(pt,Dem(pt));
		b=a;
	}
	TinHieu(pt,Dem(pt));
	return pt;
}

int check_row(int col){
	GPIO_WriteBit(GPIOB,GPIO_Pin_0,Bit_RESET);
	GPIO_WriteBit(GPIOB,GPIO_Pin_1,Bit_SET);
	GPIO_WriteBit(GPIOB,GPIO_Pin_10,Bit_SET);
	GPIO_WriteBit(GPIOB,GPIO_Pin_11,Bit_SET);
	if( (col==0 && !GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)) 
		|| (col==1 && !GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)) 
	  || (col==2 && !GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2)) 
	  || (col==3 && !GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3)))
	return 0;
	
	GPIO_WriteBit(GPIOB,GPIO_Pin_0,Bit_SET);
	GPIO_WriteBit(GPIOB,GPIO_Pin_1,Bit_RESET);
	GPIO_WriteBit(GPIOB,GPIO_Pin_10,Bit_SET);
	GPIO_WriteBit(GPIOB,GPIO_Pin_11,Bit_SET);
	if( (col==0 && !GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)) 
		|| (col==1 && !GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)) 
	  || (col==2 && !GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2)) 
	  || (col==3 && !GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3)))
	return 1;
	
	GPIO_WriteBit(GPIOB,GPIO_Pin_0,Bit_SET);
	GPIO_WriteBit(GPIOB,GPIO_Pin_1,Bit_SET);
	GPIO_WriteBit(GPIOB,GPIO_Pin_10,Bit_RESET);
	GPIO_WriteBit(GPIOB,GPIO_Pin_11,Bit_SET);
	if( (col==0 && !GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)) 
		|| (col==1 && !GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)) 
	  || (col==2 && !GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2)) 
	  || (col==3 && !GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3)))
	return 2;
	
	GPIO_WriteBit(GPIOB,GPIO_Pin_0,Bit_SET);
	GPIO_WriteBit(GPIOB,GPIO_Pin_1,Bit_SET);
	GPIO_WriteBit(GPIOB,GPIO_Pin_10,Bit_SET);
	GPIO_WriteBit(GPIOB,GPIO_Pin_11,Bit_RESET);
	if( (col==0 && !GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)) 
		|| (col==1 && !GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)) 
	  || (col==2 && !GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2)) 
	  || (col==3 && !GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3)))
	return 3;
	return -1;
}


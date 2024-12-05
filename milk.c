
#include<18f4520.h>
#DEVICE ADC=10

#fuses INTRC_IO,NOPROTECT,BROWNOUT,NOMCLR,NOCPD,NOWDT,NOPUT,FCMEN

#use delay(clock=8000000)//,restart_wdt)
#use rs232(baud=9600, xmit=PIN_C6, rcv=PIN_C7)

#byte adresh = 0x1e 
#bit adfm = 0x1f.7

#define BUZZ_ON  OUTPUT_HIGH(PIN_C5);
#define BUZZ_OFF OUTPUT_LOW(PIN_C5);


#define RS_HI   OUTPUT_HIGH(PIN_A5);
#define RS_LO   OUTPUT_LOW(PIN_A5);

#define EN_HI   OUTPUT_HIGH(PIN_A4);
#define EN_LO   OUTPUT_LOW(PIN_A4);

#define D4_HI   OUTPUT_HIGH(PIN_A3);
#define D4_LO   OUTPUT_LOW(PIN_A3);
#define D5_HI   OUTPUT_HIGH(PIN_A2);
#define D5_LO   OUTPUT_LOW(PIN_A2);
#define D6_HI   OUTPUT_HIGH(PIN_A1);
#define D6_LO   OUTPUT_LOW(PIN_A1);
#define D7_HI   OUTPUT_HIGH(PIN_A0);
#define D7_LO   OUTPUT_LOW(PIN_A0);

#define sel_ON          OUTPUT_HIGH(PIN_D2);
#define sel_OFF          OUTPUT_LOW(PIN_D2);

#define trigg_ON      OUTPUT_HIGH(PIN_B1);
#define trigg_OFF      OUTPUT_LOW(PIN_B1);

#define trigg2_ON      OUTPUT_HIGH(PIN_A7);
#define trigg2_OFF      OUTPUT_LOW(PIN_A7);

#define trigg3_ON      OUTPUT_HIGH(PIN_A5);
#define trigg3_OFF      OUTPUT_LOW(PIN_A5);

#define RLY1_ON         OUTPUT_HIGH(PIN_D0);
#define RLY1_OFF      OUTPUT_LOW(PIN_D0);

#define RLY5_ON         OUTPUT_HIGH(PIN_D1);
#define RLY5_OFF      OUTPUT_LOW(PIN_D1);

#define RLY3_ON         OUTPUT_HIGH(PIN_D1);
#define RLY3_OFF      OUTPUT_LOW(PIN_D1);

#define RLY4_ON         OUTPUT_HIGH(PIN_C3);
#define RLY4_OFF      OUTPUT_LOW(PIN_C3);

#define RLY2_ON         OUTPUT_HIGH(PIN_D0);
#define RLY2_OFF      OUTPUT_LOW(PIN_D0)

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define LCD_LINE_1_START_ADDRESS   0x80
#define LCD_LINE_2_START_ADDRESS   0xC0
#define LCD_LINE_3_START_ADDRESS   0x90
#define LCD_LINE_4_START_ADDRESS   0xD0
#define LCD_LINE_1_END_ADDRESS       0x8F
#define LCD_LINE_2_END_ADDRESS       0xCF
#define LCD_LINE_3_END_ADDRESS       0x9F
#define LCD_LINE_4_END_ADDRESS       0xDF

#define LCD_DATA_WRITE             1
#define LCD_CMD_WRITE             0
int8 ucKeyPressed = 0;

int16 uiTemp = 0;
int8 ucLatLongArr[30] = {0,0,0,0,0,0,0,0,0,0,0,0};
int8  ucsendsmsf = 0;
volatile int16 hbyte = 0;
int8 index = 0;
int8 h = 0;
int8 ucpowerf = 0;
int8 uc2ndpowerf = 0; 
int8 ucSmsNo = 0;
int8 ucactionf = 0;
int8 ucget_data = 0;
volatile int8 Q1_array[67] =  {0,0,0,0,0,0,0,0,0,0,0};
volatile int8 ucRxARR[85] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

int16 uiRvlt_adc = 0;
int8 ucRx_Array[20] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int8 i = 0;
int16 ui1SecCNT = 5;//11;
int16 uiVltg = 0;
int8 ucRxTimOut = 0;
float ucfat= 0;
int16 uimoistadc = 0;
int16 ui4SecCNT = 0;
int8 ucmetalf = 0;//1;//10;
int8 ucdisplayf = 0;
int16 ui10SecCNT = 0;
int16 ui2SecCNT = 0;
int16 uiLcd10Sec = 80;
int8 ucuser_f = 0;
int16 ui5SecCnt = 0;
int16 uiprev_xaxis = 0;
int16 uiprev_yaxis = 0;
int16 uiprev_zaxis = 0;

int8 ucdust3 = 0;
int8 ucvibretflag = 0;
int8 ucir = 0;
int8 ucmoist = 0;
int8 ucDecimal_Array[18] = {0,0,0,0,0,0,0,0,0,0,0,0};
int8 ucASCII_Array[18] = {0,0,0,0,0,0,0,0,0,0,0,0};
BYTE CONST ucBLANK_Array[17]     = "                ";//1

BYTE CONST ucWelcm_1_Array[17]     = "IOT BASED MILK  ";//22
BYTE CONST ucWelcm_2_Array[17]     = "QUALITY ANALYSIS ";//23

BYTE CONST ucCITY1_Array[17]     = "PH:     TEMP:   ";//24
BYTE CONST ucCITY2_Array[17]     = "HUMID:    WL:   ";//25
BYTE CONST ucCITY3_Array[17]    = "TEST SAMPLE:     ";//25
BYTE CONST ucCITY4_Array[17]    = "GENERATE BILL:   ";//25

BYTE CONST ucWARD1_Array[17]     = "TEST QUALITY    ";//24
BYTE CONST ucWARD2_Array[17]    = "QUALITY ACCEPTED";//25
BYTE CONST ucWARD3_Array[17]    = "QUALITY REJECTED ";//25

void main(void)
{
   
   SETUP_ADC(ADC_OFF);                  //disable ADC i/p
   SETUP_ADC_PORTS(NO_ANALOGS);          //disable analog i/p
      setup_comparator(NC_NC_NC_NC);
      SETUP_CCP1(CCP_OFF);
   SET_TRIS_A(0x5D);//0110 1100
   SET_TRIS_B(0x1F);//0000 1000
   SET_TRIS_C(0x87);//1000 0010
   SET_TRIS_D(0xF0);//1111 0001
   SET_TRIS_E(0x07);//0000 0111
   
   
   SETUP_TIMER_1(T1_INTERNAL|T1_DIV_BY_8);   //enables timer1
   SET_TIMER1(40536); // timer of 200ms   (64286);//10msec
   enable_interrupts(INT_RDA);
   ENABLE_INTERRUPTS(INT_TIMER1);
   ENABLE_INTERRUPTS(INT_EXT);
    setup_timer_2(T2_DIV_BY_16, 30, 1);//(1/10000000)*4*16*32= 204.3 us or 3.1 khz
    set_pwm1_duty(0);//ok

   ENABLE_INTERRUPTS(GLOBAL);
   RLY1_ON;
   RLY2_ON;
   RLY3_ON;
   RLY4_ON;
   BUZZ_ON;
   delay_ms(500);
   BUZZ_OFF;
   RLY1_OFF;
   RLY2_OFF;
   RLY3_OFF;
   RLY4_OFF;
   RLY5_OFF;

   INIT_LCD();

   ENABLE_INTERRUPTS(GLOBAL);
   
   uiLcd10Sec = 30;

   LCD_WRITE_Const_ARRAY(1,0,15,16);//Blank
   LCD_WRITE_Const_ARRAY(2,0,16,16);//Blank

sel_OFF;
printf("HELLO");
   uiLcd10Sec = 30;
    trigg_ON;
   delay_ms(500);

trigg_OFF;
sel_ON;
Get_Key();
while(1)
{
Get_Key();

if(ucKeyPressed == 1)
{
 LCD_WRITE_Const_ARRAY(1,0,19,16);//Blank
   LCD_WRITE_Const_ARRAY(2,0,1,16);//Blank
  ucget_data = 1;
   BUZZ_ON;
   delay_ms(500);
   BUZZ_OFF;
   delay_ms(1500);   
delay_ms(1500);
 LCD_WRITE_Const_ARRAY(1,0,1,16);//Blank
   LCD_WRITE_Const_ARRAY(2,0,1,16);//Blank
}
if(ucKeyPressed == 2)
{
 LCD_WRITE_Const_ARRAY(1,0,20,16);//Blank
   LCD_WRITE_Const_ARRAY(2,0,1,16);//Blank
  ucget_data = 2;
   BUZZ_ON;
   delay_ms(500);
   BUZZ_OFF;
   delay_ms(1500);   
delay_ms(1500);
 LCD_WRITE_Const_ARRAY(1,0,1,16);//Blank
   LCD_WRITE_Const_ARRAY(2,0,1,16);//Blank
}
if(ucKeyPressed == 3)
{
 LCD_WRITE_Const_ARRAY(1,0,21,16);//Blank
   LCD_WRITE_Const_ARRAY(2,0,1,16);//Blank
  ucget_data = 3;
   BUZZ_ON;
   delay_ms(500);
   BUZZ_OFF;
   delay_ms(1500);   


}
	if(ucRxTimOut == 1)
	{
		ucRxTimOut = 0;
	for(i = 0;i<50;i++)
	{
if(ucget_data == 3)
{
	if((ucRxARR[i] == '+'))//&&(ucRx_Array[i+1] == 'A'))
		{
 LCD_WRITE_Const_ARRAY(2,0,23,16);//Blank
 

		}
	else if((ucRxARR[i] == '-'))//&&(ucRx_Array[i+1] == 'A'))
		{
LCD_WRITE_Const_ARRAY(2,0,22,16);//Blank
		}
}
if(ucget_data == 1)
{
		if((ucRxARR[i] == '*'))//&&(ucRx_Array[i+1] == 'A'))
		{

			LCD_Init_Command(0x80);// 
   			delay_ms(10);
   
		   LCD_Data('P');
		   LCD_Data('H');
		   LCD_Data(':');
			LCD_Data(ucRxARR[i+1]);
			LCD_Data(ucRxARR[i+2]);
			LCD_Data(ucRxARR[i+3]);
			LCD_Data(ucRxARR[i+4]);
			LCD_Data(' ');
	}
if((ucRxARR[i] == '%'))//&&(ucRx_Array[i+1] == 'A'))
{
	LCD_Init_Command(0xC0);// 
   			delay_ms(10);
  LCD_Data('F');
	   LCD_Data('A');
	   LCD_Data('T');
	 //  LCD_Data('L');
	   LCD_Data(':');
		LCD_Data(ucRxARR[i+3]);
		LCD_Data(ucRxARR[i+4]);
		LCD_Data(ucRxARR[i+5]);
}
if((ucRxARR[i] == '@'))//&&(ucRx_Array[i+1] == 'A'))
{
	LCD_Init_Command(0xC9);// 
   			delay_ms(10);
   LCD_Data('T');
   LCD_Data('E');
   LCD_Data('M');
   LCD_Data('P');
   LCD_Data(':');
	LCD_Data(ucRxARR[i+1]);
	LCD_Data(ucRxARR[i+2]);


}

}
if(ucget_data == 2)
{
	if((ucRxARR[i] == '$'))//&&(ucRx_Array[i+1] == 'A'))
	{
	//	LCD_Data(' ');
		LCD_Init_Command(0xC8);// 
	   			delay_ms(10);
	   LCD_Data('B');
	   LCD_Data('I');
	   LCD_Data('L');
	 //  LCD_Data('L');
	   LCD_Data(':');
		LCD_Data(ucRxARR[i+1]);
		LCD_Data(ucRxARR[i+2]);
		LCD_Data(ucRxARR[i+3]);
		LCD_Data(ucRxARR[i+4]);
	
	
	 
	}
	if((ucRxARR[i] == '&'))//&&(ucRx_Array[i+1] == 'A'))
	{
			LCD_Init_Command(0x80);// 
	   			delay_ms(10);
	   LCD_Data('L');
	   LCD_Data('I');
	   LCD_Data('T');
	  // LCD_Data('L');
	   LCD_Data(':');
		LCD_Data(ucRxARR[i+1]);
		LCD_Data(ucRxARR[i+2]);
		LCD_Data(ucRxARR[i+3]);
		LCD_Data(ucRxARR[i+4]);
		   LCD_Data(' ');
	   LCD_Data('L');
	
	 
	}
	if((ucRxARR[i] == '%'))//&&(ucRx_Array[i+1] == 'A'))
	{
			LCD_Init_Command(0xC0);// 
	   			delay_ms(10);
	   LCD_Data('F');
	   LCD_Data('A');
	   LCD_Data('T');
	 //  LCD_Data('L');
	   LCD_Data(':');
		LCD_Data(ucRxARR[i+3]);
		LCD_Data(ucRxARR[i+4]);
		LCD_Data(ucRxARR[i+5]);
	 
	}
}
}
 	//	LCD_WRITE_Const_ARRAY(2,0,33,16);//Blank
		
	index = 0;
	}
 			ADC_CALL(9);//PH
            uiGas_adc = current_adc_val;
      
  			ADC_CALL(11);//Lamp Current
            uiprev_yaxis = current_adc_val/12;
if(ucget_data == 1)
{
	LCD_Init_Command(0x88);// 
   			delay_ms(10);
   LCD_Data(' ');
		   LCD_Data('G');
		   LCD_Data('A');
LCD_Data('S');
		   LCD_Data(':');
        Show_Float_No_ONLine_At_Offset_IntDig_FltDig(1,13,uiprev_yaxis,3,0);
        //   Show_Float_No_ONLine_At_Offset_IntDig_FltDig(1,12,ucfat,4,0);
}
 ///Show_Float_No_ONLine_At_Offset_IntDig_FltDig(1,12,ucfat,3,0);
}
   }      




/*********************************************************************************************
ģ��������  ��������/���ҵ���
��������	PWM�����������
��д�ˣ�	����	
��дʱ�䣺	2018��6��18��
Ӳ��֧�֣�	����1�ſ����� STM32F103C8 �ⲿ����8MHz RCC����������Ƶ72MHz��  

�޸���־������
1-	
	
							
˵����
 # ��������������1�ſ������Ӳ�������ϱ�д�ģ���ֲ���˽�Ӳ���ӿڲ��졣
 # ��ģ�������STM32F103�ڲ���RCCʱ�����ã������������õδ�ʱ������ʱ������
 # �ɸ����Լ�����Ҫ���ӻ�ɾ����

*********************************************************************************************/
#include "stm32f10x.h" //STM32ͷ�ļ�
#include "sys.h"
#include "delay.h"
#include "relay.h"
#include "oled0561.h"
#include "SG90.h"
#include "touch_key.h"

#include "pwm.h"


int main (void){//������
	delay_ms(500); //�ϵ�ʱ�ȴ�������������
	RCC_Configuration(); //ϵͳʱ�ӳ�ʼ�� 
	RELAY_Init();//�̵�����ʼ��

	I2C_Configuration();//I2C��ʼ��
	OLED0561_Init(); //OLED��ʼ��
	OLED_DISPLAY_8x16_BUFFER(0,"   YoungTalk    "); //��ʾ�ַ���
	OLED_DISPLAY_8x16_BUFFER(3,"   SG90 TEST2   "); //��ʾ�ַ���

	TOUCH_KEY_Init();//������ʼ��
	TIM3_PWM_Init(59999,23); //����Ƶ��Ϊ50Hz����ʽΪ�����ʱ��Tout����λ�룩=(arr+1)(psc+1)/Tclk	 20MS = (59999+1)*(23+1)/72000000
                          //TclkΪͨ�ö�ʱ����ʱ�ӣ����APB1û�з�Ƶ�����Ϊϵͳʱ�ӣ�72MHZ
                          //PWMʱ��Ƶ��=72000000/(59999+1)*(23+1) = 50HZ (20ms),�����Զ�װ��ֵ60000,Ԥ��Ƶϵ��24

	while(1){
		if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A)){ //�����������ĵ�ƽ
			OLED_DISPLAY_8x16_BUFFER(6,"  Angle 0       "); //��ʾ�ַ���
			TIM_SetCompare3(TIM3,1500);        //�ı�Ƚ�ֵTIM3->CCR2�ﵽ����ռ�ձȵ�Ч����1500Ϊ0�ȣ�
		}
		if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B)){ //�����������ĵ�ƽ
			OLED_DISPLAY_8x16_BUFFER(6,"  Angle 45      "); //��ʾ�ַ���
			TIM_SetCompare3(TIM3,3000);        //�ı�Ƚ�ֵTIM3->CCR2�ﵽ����ռ�ձȵ�Ч��
		}
		if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C)){ //�����������ĵ�ƽ
			OLED_DISPLAY_8x16_BUFFER(6,"  Angle 90     "); //��ʾ�ַ���
			TIM_SetCompare3(TIM3,4500);        //�ı�Ƚ�ֵTIM3->CCR2�ﵽ����ռ�ձȵ�Ч��
		}
		if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D)){ //�����������ĵ�ƽ
			OLED_DISPLAY_8x16_BUFFER(6,"  Angle 180     "); //��ʾ�ַ���
			TIM_SetCompare3(TIM3,7500);        //�ı�Ƚ�ֵTIM3->CCR2�ﵽ����ռ�ձȵ�Ч��
		}
	}
}

/*********************************************************************************************
 * �������� www.DoYoung.net
 * ���ҵ��� www.DoYoung.net/YT 
*********************************************************************************************/
/*

���������塿
u32     a; //����32λ�޷��ű���a
u16     a; //����16λ�޷��ű���a
u8     a; //����8λ�޷��ű���a
vu32     a; //�����ױ��32λ�޷��ű���a
vu16     a; //�����ױ�� 16λ�޷��ű���a
vu8     a; //�����ױ�� 8λ�޷��ű���a
uc32     a; //����ֻ����32λ�޷��ű���a
uc16     a; //����ֻ�� ��16λ�޷��ű���a
uc8     a; //����ֻ�� ��8λ�޷��ű���a

#define ONE  1   //�궨��

delay_us(1); //��ʱ1΢��
delay_ms(1); //��ʱ1����
delay_s(1); //��ʱ1��

GPIO_WriteBit(LEDPORT,LED1,(BitAction)(1)); //LED����

*/




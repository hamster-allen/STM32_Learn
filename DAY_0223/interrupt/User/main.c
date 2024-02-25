/*********************************************************************************************
ģ��������  ��������/���ҵ���
��������	�����жϲ��Գ���
��д�ˣ�	����	
��дʱ�䣺	2018��5��21��
Ӳ��֧�֣�	STM32F103C8   �ⲿ����8MHz RCC����������Ƶ72MHz��  

�޸���־������
1-	
	
							
˵����
 # ��ģ�������STM32F103�ڲ���RCCʱ�����ã������������õδ�ʱ������ʱ������
 # �ɸ����Լ�����Ҫ���ӻ�ɾ����

*********************************************************************************************/
#include "stm32f10x.h" //STM32ͷ�ļ�
#include "sys.h"
#include "delay.h"
#include "relay.h"
#include "oled0561.h"

#include "KEYPAD4x4.h"
#include "NVIC.h"


int main (void){//������
	u8 s;
	delay_ms(500); //�ϵ�ʱ�ȴ�������������
	RCC_Configuration(); //ϵͳʱ�ӳ�ʼ�� 
	RELAY_Init();//�̵�����ʼ��

	I2C_Configuration();//I2C��ʼ��
	OLED0561_Init(); //OLED��ʼ��
	OLED_DISPLAY_8x16_BUFFER(0,"   YoungTalk    "); //��ʾ�ַ���
	OLED_DISPLAY_8x16_BUFFER(3," KEYPAD4x4 TEST "); //��ʾ�ַ���

	INT_MARK=0;//��־λ��0

	NVIC_Configuration();//�����ж����ȼ�
	KEYPAD4x4_Init();//���м��̳�ʼ��
	KEYPAD4x4_INT_INIT();//���м��̵��жϳ�ʼ��

	while(1){

		//������������

		if(INT_MARK){ //�жϱ�־λΪ1��ʾ�а����ж�
			INT_MARK=0;//��־λ��0
			s=KEYPAD4x4_Read();//��������ֵ
			if(s!=0){ //�簴��ֵ����0��Ҳ����˵�а������������ж�Ϊ��
				//-------------------------"----------------"
				OLED_DISPLAY_8x16_BUFFER(6," KEY NO.        "); //��ʾ�ַ���
				OLED_DISPLAY_8x16(6,8*8,s/10+0x30);//
				OLED_DISPLAY_8x16(6,9*8,s%10+0x30);//
			}
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



/*********************************************************************************************
ģ��������  ��������/���ҵ���
��������	ģ��ҡ��ADC��ȡ����
��д�ˣ�	����	
��дʱ�䣺	2018��4��10��
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
#include "touch_key.h"
#include "relay.h"
#include "oled0561.h"

#include "adc.h"
#include "JoyStick.h"

extern vu16 ADC_DMA_IN[2]; //�����ⲿ����

int main (void){//������
	delay_ms(500); //�ϵ�ʱ�ȴ�������������
	RCC_Configuration(); //ϵͳʱ�ӳ�ʼ�� 
	TOUCH_KEY_Init();//����������ʼ��
	RELAY_Init();//�̵�����ʼ��

	ADC_Configuration(); //ADC��ʼ�����ã�ģ��ҡ�˵�ADC��ʼ����
	JoyStick_Init(); //ģ��ҡ�˵İ�����ʼ��

	I2C_Configuration();//I2C��ʼ��
	OLED0561_Init(); //OLED��ʼ��
	OLED_DISPLAY_8x16_BUFFER(0,"   YoungTalk "); //��ʾ�ַ���
	OLED_DISPLAY_8x16_BUFFER(2,"   ADC TEST  "); //��ʾ�ַ���
	OLED_DISPLAY_8x16_BUFFER(4," ADC_IN6:    "); //��ʾ�ַ���
	OLED_DISPLAY_8x16_BUFFER(6," ADC_IN7:    "); //��ʾ�ַ���


	while(1){
		//�����������ADC������ʾ��OLED��
		OLED_DISPLAY_8x16(4,10*8,ADC_DMA_IN[0]/1000+0x30);//
		OLED_DISPLAY_8x16(4,11*8,ADC_DMA_IN[0]%1000/100+0x30);//
		OLED_DISPLAY_8x16(4,12*8,ADC_DMA_IN[0]%100/10+0x30);//
		OLED_DISPLAY_8x16(4,13*8,ADC_DMA_IN[0]%10+0x30);//

		OLED_DISPLAY_8x16(6,10*8,ADC_DMA_IN[1]/1000+0x30);//
		OLED_DISPLAY_8x16(6,11*8,ADC_DMA_IN[1]%1000/100+0x30);//
		OLED_DISPLAY_8x16(6,12*8,ADC_DMA_IN[1]%100/10+0x30);//
		OLED_DISPLAY_8x16(6,13*8,ADC_DMA_IN[1]%10+0x30);//

		if(GPIO_ReadInputDataBit(JoyStickPORT,JoyStick_KEY)==0){
			OLED_DISPLAY_8x16(0,0,'Y');//
		}else{
			OLED_DISPLAY_8x16(0,0,' ');//
		}
		delay_ms(200); //��ʱ
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




/*********************************************************************************************
ģ��������  ��������/���ҵ���
��������	U�̲�β��Գ���
��д�ˣ�	����	
��дʱ�䣺	2018��4��27��
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

#include "spi.h"
#include "ch376.h"
#include "filesys.h"

int main (void){//������
	u8 s;
	delay_ms(500); //�ϵ�ʱ�ȴ�������������
	RCC_Configuration(); //ϵͳʱ�ӳ�ʼ�� 
	TOUCH_KEY_Init();//����������ʼ��
	RELAY_Init();//�̵�����ʼ��

	I2C_Configuration();//I2C��ʼ��
	OLED0561_Init(); //OLED��ʼ��
	OLED_DISPLAY_8x16_BUFFER(0,"   YoungTalk    "); //��ʾ�ַ���
	OLED_DISPLAY_8x16_BUFFER(2,"  U DISK TEST   "); //��ʾ�ַ���
	//CH376��ʼ��	
	SPI2_Init();//SPI�ӿڳ�ʼ��
	if(mInitCH376Host()== USB_INT_SUCCESS){//CH376��ʼ��
		OLED_DISPLAY_8x16_BUFFER(4,"   CH376 OK!    "); //��ʾ�ַ���
	}
	while(1){
		s = CH376DiskConnect();	 //����U�̵�״̬ 
		if(s == USB_INT_SUCCESS){ //���U���Ƿ�����//�ȴ�U�̲���
			OLED_DISPLAY_8x16_BUFFER(6," U DISK Ready!  "); //��ʾ�ַ���
		}else{
			OLED_DISPLAY_8x16_BUFFER(6,"                "); //��ʾ�ַ���		
		} 
		delay_ms(500); //ˢ�µļ��
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




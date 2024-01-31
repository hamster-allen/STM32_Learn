/*********************************************************************************************
ģ��������  ��������/���ҵ���
��������	�¶ȴ������������ʾ����
��д�ˣ�	����	
��дʱ�䣺	2018��2��10��
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
#include "TM1640.h"

#include "lm75a.h"

int main (void){//������
	u8 buffer[3];
	u8 c=0x01;
	RCC_Configuration(); //ϵͳʱ�ӳ�ʼ�� 

	I2C_Configuration();//I2C��ʼ��

	TM1640_Init(); //TM1640��ʼ��
	TM1640_display(0,20); //��ʼ��ʾ����
	TM1640_display(1,20);
	TM1640_display(2,20);
	TM1640_display(3,20);
	TM1640_display(4,20);
	TM1640_display(5,20);
	TM1640_display(6,20);
	TM1640_display(7,20);

	while(1){
		LM75A_GetTemp(buffer); //��ȡLM75A���¶�����
			
		TM1640_display(0,buffer[1]/10); //��ʾ��ֵ
		TM1640_display(1,buffer[1]%10+10);
		TM1640_display(2,buffer[2]/10);
		TM1640_display(3,buffer[2]%10);

		TM1640_led(c); //��TM1640���ӵ�8��LEDȫ��
		c<<=1; //�������� ��ˮ��
		if(c==0x00)c=0x01; //8������ʾ������¿�ʼ
		delay_ms(150); //��ʱ
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



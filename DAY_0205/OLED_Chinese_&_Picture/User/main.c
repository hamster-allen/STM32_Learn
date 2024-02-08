/*********************************************************************************************
ģ��������  ��������/���ҵ���
��������	OLED��������ͼƬ��ʾ����
��д�ˣ�	����	
��дʱ�䣺	2018��3��7��
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
#include "lm75a.h"

#include "oled0561.h"

#include "usart.h"




int main (void){//������
	//u8 test_data[8];	
	u8 buffer[3];
	delay_ms(100); //�ϵ�ʱ�ȴ�������������
	
	RCC_Configuration(); //ϵͳʱ�ӳ�ʼ�� 

	I2C_Configuration();//I2C��ʼ��
	LM75A_GetTemp(buffer); //��ȡLM75A���¶�����

	OLED0561_Init(); //OLED��ʼ��
	OLED_DISPLAY_LIT(100);//��������

	OLED_DISPLAY_PIC1();//��ʾȫ��ͼƬ
	delay_ms(1000); //��ʱ
	OLED_DISPLAY_CLEAR();
	OLED_DISPLAY_8x16_BUFFER(0,"   YoungTalk "); //��ʾ�ַ���
	OLED_DISPLAY_8x16_BUFFER(6,"  Temp:"); //��ʾ�ַ���

	OLED_DISPLAY_16x16(2,2*16,0);//������ʾ	 ���ҵ���
	OLED_DISPLAY_16x16(2,3*16,1);
	OLED_DISPLAY_16x16(2,4*16,2);
	OLED_DISPLAY_16x16(2,5*16,3);

	USART1_Init(115200);


	while(1){
		//LM75A_GetTemp(buffer); //��ȡLM75A���¶�����
			
		//if(buffer[0]){OLED_DISPLAY_8x16(6,7*8,'-');} //�����1��Ϊ1���Ǹ��¶�
		//OLED_DISPLAY_8x16(6,8*8,buffer[1]/10+0x30);//��ʾ�¶�ֵ
		//OLED_DISPLAY_8x16(6,9*8,buffer[1]%10+0x30);//
		//OLED_DISPLAY_8x16(6,10*8,'.');//
		//OLED_DISPLAY_8x16(6,11*8,buffer[2]/10+0x30);//
		//OLED_DISPLAY_8x16(6,12*8,buffer[2]%10+0x30);//
		OLED_DISPLAY_8x16(4,13*8,'C');//
		OLED_Write_8x16(56, 13*8, 'A');
		//OLED_Read_Display(6, 13*8, test_data);
		//USART1_printf("test_data0: %x\n", test_data[0]);
		//USART1_printf("test_data1: %x\n", test_data[1]);
		//USART1_printf("test_data2: %x\n", test_data[2]);
		//USART1_printf("test_data3: %x\n", test_data[3]);
		//USART1_printf("test_data4: %x\n", test_data[4]);
		//USART1_printf("test_data5: %x\n", test_data[5]);
		//USART1_printf("test_data6: %x\n", test_data[6]);
		//USART1_printf("test_data7: %x\n", test_data[7]);

		delay_ms(500); //��ʱ
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




/*********************************************************************************************
ģ��������  ��������/���ҵ���
��������	��������LED����
��д�ˣ�	����	
��дʱ�䣺	2017��12��29��
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
#include "led.h"

#include "key.h" 

int main (void){//������
	u8 a; //�������
	//��ʼ������
	RCC_Configuration(); //ʱ������
	LED_Init();//LED��ʼ��

	KEY_Init();//������ʼ��

	//��ѭ��
	while(1){

		//ʾ��1��������
// 		if(GPIO_ReadInputDataBit(KEYPORT,KEY1)){ //�������ӿڵĵ�ƽ
// 			GPIO_ResetBits(LEDPORT,LED1); //LED�ƶ�Ϊ�͵�ƽ��0�� 
// 		}else{	
//         	GPIO_SetBits(LEDPORT,LED1); //LED�ƶ�Ϊ�ߵ�ƽ��1�� 
// 		}

		//ʾ��2��������
//		GPIO_WriteBit(LEDPORT,LED1,(BitAction)(!GPIO_ReadInputDataBit(KEYPORT,KEY1))); 

		//ʾ��3��������
// 		if(!GPIO_ReadInputDataBit(KEYPORT,KEY1)){ //�������ӿڵĵ�ƽ
// 			delay_ms(20); //��ʱȥ����
// 			if(!GPIO_ReadInputDataBit(KEYPORT,KEY1)){ //�������ӿڵĵ�ƽ
// 				GPIO_WriteBit(LEDPORT,LED1,(BitAction)(!GPIO_ReadOutputDataBit(LEDPORT,LED1))); //LEDȡ��
// 				while(!GPIO_ReadInputDataBit(KEYPORT,KEY1)); //�ȴ������ɿ� 
// 			}
// 		}

		//ʾ��4��������
		if(!GPIO_ReadInputDataBit(KEYPORT,KEY1)){ //�������ӿڵĵ�ƽ
			delay_ms(20); //��ʱ20msȥ����
			if(!GPIO_ReadInputDataBit(KEYPORT,KEY1)){ //�������ӿڵĵ�ƽ
				//��2��LED����ʾ�����Ƽӷ�
				a++; //������1
				if(a>3){ //����������3ʱ��0
					a=0; 
				}
				GPIO_Write(LEDPORT,a); //ֱ����ֵ����������ֵд��LED��LED��GPIOB���PB0��PB1�ϣ�
				while(!GPIO_ReadInputDataBit(KEYPORT,KEY1)); //�ȴ������ɿ� 
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

*/



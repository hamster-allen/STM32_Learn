/*********************************************************************************************
ģ��������  ��������/���ҵ���
��������	MP3���Ų��Գ���
��д�ˣ�	����	
��дʱ�䣺	2018��4��13��
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

#include "encoder.h"
#include "usart.h"
#include "my1690.h"

int main (void){//������
	u8 b;
	u8 MP3=0;
	delay_ms(500); //�ϵ�ʱ�ȴ�������������
	RCC_Configuration(); //ϵͳʱ�ӳ�ʼ�� 
	TOUCH_KEY_Init();//����������ʼ��
	RELAY_Init();//�̵�����ʼ��

	ENCODER_Init(); //��ת��������ʼ��
	MY1690_Init(); //MP3оƬ��ʼ��

	I2C_Configuration();//I2C��ʼ��
	OLED0561_Init(); //OLED��ʼ��
	OLED_DISPLAY_8x16_BUFFER(0,"   YoungTalk    "); //��ʾ�ַ���
	OLED_DISPLAY_8x16_BUFFER(3," MP3 PLAY TEST  "); //��ʾ�ַ���

	while(1){
		if(GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A)==0    //�ж�4�������Ƿ���
			||GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B)==0 
			||GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C)==0 
			||GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D)==0){
			delay_ms(20); //��ʱ
			if(GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A)==0){	//4��������A��һ����B��һ����
				MY1690_PREV(); //��һ��
				OLED_DISPLAY_8x16_BUFFER(6,"  -- PREV --    "); //��ʾ�ַ���
				delay_ms(500); //��ʱ
				OLED_DISPLAY_8x16_BUFFER(6,"  -- PLAY --    "); //��ʾ�ַ���
			}
			if(GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B)==0){
				MY1690_NEXT(); //��һ��
				OLED_DISPLAY_8x16_BUFFER(6,"  -- NEXT --    "); //��ʾ�ַ���
				delay_ms(500); //��ʱ
				OLED_DISPLAY_8x16_BUFFER(6,"  -- PLAY --    "); //��ʾ�ַ���
			}
			if(GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C)==0){
				MY1690_CMD2(0x31,30); //����������Ϊ30�����
				delay_ms(500); //��ʱ
			}
			if(GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D)==0){
				MY1690_CMD3(0x41,0x04); //ֱ�Ӳ��ŵ�0004��
				delay_ms(500); //��ʱ
			}
			while(GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A)==0	//�ȴ������ſ�
			||GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B)==0 
			||GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C)==0 
			||GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D)==0);
		}
		b=ENCODER_READ();	//������ת����������ֵ	
		if(b==1){MY1690_VUP();} //��ת�����ӡ�
		if(b==2){MY1690_VDOWN();}//��ת��������
		if(b==3){	//���²��Ż���ͣ
			if(MP3==0){	//�жϵ�ǰ�ǲ��Ż�����ͣ
				MP3=1;MY1690_PLAY();
				OLED_DISPLAY_8x16_BUFFER(6,"  -- PLAY --    "); //��ʾ�ַ���
			}else if(MP3==1){
				MP3=0;MY1690_PAUSE();
				OLED_DISPLAY_8x16_BUFFER(6,"  -- PAUSE --   "); //��ʾ�ַ���
			}
			delay_ms(500); //��ʱ
		}
		//���ڽ��մ���
        if(USART3_RX_STA==1){ //�����־λ��1 ��ʾ�յ�STOP
            MP3=0;
            OLED_DISPLAY_8x16_BUFFER(6,"  -- STOP --    "); //��ʾ�ַ���
            USART3_RX_STA=0; //���������ݱ�־λ��0
        }else if(USART3_RX_STA==2){    //�����־λ��1 ��ʾ�յ�OK
            //������صĴ������
            USART3_RX_STA=0; //���������ݱ�־λ��0
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




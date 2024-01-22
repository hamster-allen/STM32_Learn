/*********************************************************************************************
ģ��������  ��������/���ҵ���
��������	������������,����,˫��,��������
��д�ˣ�	����	
��дʱ�䣺	2018��1��28��
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
#include "touch_key.h"
#include "usart.h"

#define KEYA_SPEED1	100	  //������ʱ�䳤�ȣ���λ10mS��
#define KEYA_SPEED2	10	  //˫����ʱ�䳤�ȣ���λ20mS��



int main (void){//������
	u16 k=1000;	//���ڻ����Ӽ�����
	u8 a=0,b,c=0;
	u8 s=0; //�ոս���������־
	RCC_Configuration(); //ϵͳʱ�ӳ�ʼ�� 
	USART1_Init(115200); //���ڳ�ʼ����������д������
	LED_Init();//LED��ʼ��
	TOUCH_KEY_Init();//������ʼ��
	while(1){
//A
		if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A)){ //��ⰴ���Ƿ���
			delay_ms(20); //��ʱȥ����
			if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A)){//�жϳ��̼�
				while((!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A))&&c<KEYA_SPEED1){ //ѭ���жϳ�������ʱ��ת
					c++;delay_ms(10); //�����жϵļ�ʱ
				}
				if(c>=KEYA_SPEED1){ //��������
					//������ִ�еĳ���ŵ��˴�
					GPIO_WriteBit(LEDPORT,LED1,(BitAction)(1));//LED����
					printf("A������ \r\n");
					while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A));
				}else{ //��������
					if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B)){
						k++; //������ʾ�ļ���ֵ
						printf("A���һ� %d \r\n",k); 
						a=1;s=1; //a�ǵ�˫���жϱ�־��s�Ǹոս���������־
					}
					if(a==0){
						for(b=0;b<KEYA_SPEED2;b++){//���˫��
							delay_ms(20);
							if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A)){
								a=1;
								//˫����ִ�еĳ���ŵ��˴�
								GPIO_WriteBit(LEDPORT,LED2,(BitAction)(1));//LED����
								printf("A��˫�� \r\n");
								while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A));
							}
						}
						if(a==0){ //�жϵ���
							if(s==1){ //�ж��ǲ��Ǹ�ִ���껬������
								s=0; //������򱾴β�ִ�е���������Ϊ�ǻ����ķſ�������
							}else{	 //������ǣ�������ִ�е�������
								//������ִ�еĳ���ŵ��˴�
								GPIO_WriteBit(LEDPORT,LED1|LED2,(BitAction)(0));//LED����
								printf("A������ \r\n");
							}
						}
					}
				}
				a=0;c=0; //������0
			}
		} //�����ж��ڴ˽���
//B
		if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B)){ //��ⰴ���Ƿ���
			delay_ms(20); //��ʱȥ����
			if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B)){//�жϳ��̼�
				while((!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B))&&c<KEYA_SPEED1){ //ѭ���жϳ�������ʱ��ת
					c++;delay_ms(10); //�����жϵļ�ʱ
				}
				if(c>=KEYA_SPEED1){ //��������
					//������ִ�еĳ���ŵ��˴�
					GPIO_WriteBit(LEDPORT,LED1,(BitAction)(1));//LED����
					printf("B������ \r\n");
					while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B));
				}else{ //��������
					if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C)){
						k++;
						printf("B���һ� %d \r\n",k); 
						a=1;s=1; //a�ǵ�˫���жϱ�־��s�Ǹոս���������־
					}
					if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A)){
						k--;
						printf("B���� %d \r\n",k); 
						a=1;s=1; //a�ǵ�˫���жϱ�־��s�Ǹոս���������־
					}
					if(a==0){
						for(b=0;b<KEYA_SPEED2;b++){//���˫��
							delay_ms(20);
							if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B)){
								a=1;
								//˫����ִ�еĳ���ŵ��˴�
								GPIO_WriteBit(LEDPORT,LED2,(BitAction)(1));//LED����
								printf("B��˫�� \r\n");
								while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B));
							}
						}
						if(a==0){ //�жϵ���
							if(s==1){ //�ж��ǲ��Ǹ�ִ���껬������
								s=0; //������򱾴β�ִ�е���������Ϊ�ǻ����ķſ�������
							}else{	 //������ǣ�������ִ�е�������
								//������ִ�еĳ���ŵ��˴�
								GPIO_WriteBit(LEDPORT,LED1|LED2,(BitAction)(0));//LED����
								printf("B������ \r\n");
							}
						}
					}
				}
				a=0;c=0; //������0
			}
		} //�����ж��ڴ˽���
//C
		if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C)){ //��ⰴ���Ƿ���
			delay_ms(20); //��ʱȥ����
			if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C)){//�жϳ��̼�
				while((!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C))&&c<KEYA_SPEED1){ //ѭ���жϳ�������ʱ��ת
					c++;delay_ms(10); //�����жϵļ�ʱ
				}
				if(c>=KEYA_SPEED1){ //��������
					//������ִ�еĳ���ŵ��˴�
					GPIO_WriteBit(LEDPORT,LED1,(BitAction)(1));//LED����
					printf("C������ \r\n");
					while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C));
				}else{ //��������
					if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D)){
						k++;
						printf("C���һ� %d \r\n",k); 
						a=1;s=1; //a�ǵ�˫���жϱ�־��s�Ǹոս���������־
					}
					if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B)){
						k--;
						printf("C���� %d \r\n",k); 
						a=1;s=1; //a�ǵ�˫���жϱ�־��s�Ǹոս���������־
					}
					if(a==0){
						for(b=0;b<KEYA_SPEED2;b++){//���˫��
							delay_ms(20);
							if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C)){
								a=1;
								//˫����ִ�еĳ���ŵ��˴�
								GPIO_WriteBit(LEDPORT,LED2,(BitAction)(1));//LED����
								printf("C��˫�� \r\n");
								while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C));
							}
						}
						if(a==0){ //�жϵ���
							if(s==1){ //�ж��ǲ��Ǹ�ִ���껬������
								s=0; //������򱾴β�ִ�е���������Ϊ�ǻ����ķſ�������
							}else{	 //������ǣ�������ִ�е�������
								//������ִ�еĳ���ŵ��˴�
								GPIO_WriteBit(LEDPORT,LED1|LED2,(BitAction)(0));//LED����
								printf("C������ \r\n");
							}
						}
					}
				}
				a=0;c=0; //������0
			}
		} //�����ж��ڴ˽���
//D
		if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D)){ //��ⰴ���Ƿ���
			delay_ms(20); //��ʱȥ����
			if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D)){//�жϳ��̼�
				while((!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D))&&c<KEYA_SPEED1){ //ѭ���жϳ�������ʱ��ת
					c++;delay_ms(10); //�����жϵļ�ʱ
				}
				if(c>=KEYA_SPEED1){ //��������
					//������ִ�еĳ���ŵ��˴�
					GPIO_WriteBit(LEDPORT,LED1,(BitAction)(1));//LED����
					printf("D������ \r\n");
					while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D));
				}else{ //��������
					if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C)){
						k--;
						printf("D���� %d \r\n",k); 
						a=1;s=1; //a�ǵ�˫���жϱ�־��s�Ǹոս���������־
					}
					if(a==0){
						for(b=0;b<KEYA_SPEED2;b++){//���˫��
							delay_ms(20);
							if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D)){
								a=1;
								//˫����ִ�еĳ���ŵ��˴�
								GPIO_WriteBit(LEDPORT,LED2,(BitAction)(1));//LED����
								printf("D��˫�� \r\n");
								while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D));
							}
						}
						if(a==0){ //�жϵ���
							if(s==1){ //�ж��ǲ��Ǹ�ִ���껬������
								s=0; //������򱾴β�ִ�е���������Ϊ�ǻ����ķſ�������
							}else{	 //������ǣ�������ִ�е�������
								//������ִ�еĳ���ŵ��˴�
								GPIO_WriteBit(LEDPORT,LED1|LED2,(BitAction)(0));//LED����
								printf("D������ \r\n");
							}
						}
					}
				}
				a=0;c=0; //������0
			}
		} //�����ж��ڴ˽���

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




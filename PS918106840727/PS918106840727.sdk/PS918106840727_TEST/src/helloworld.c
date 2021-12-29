//#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"

//���� MIO���ŵ�ַ
#define MIO_PIN_07		(*(volatile unsigned int *)0xF800071C)
#define MIO_PIN_50		(*(volatile unsigned int *)0xF80007C8)
#define MIO_PIN_51		(*(volatile unsigned int *)0xF80007CC)

//���� GPIO�˿ڷ���Ĵ�����ַ
#define DIRM_0			(*(volatile unsigned int *)0xE000A204)
#define DIRM_1			(*(volatile unsigned int *)0xE000A244)
#define DIRM_2			(*(volatile unsigned int *)0xE000A284)
#define DIRM_3			(*(volatile unsigned int *)0xE000A2C4)
//���� GPIO�˿����ʹ�ܼĴ�����ַ
#define OEN_0			(*(volatile unsigned int *)0xE000A208)
#define OEN_1			(*(volatile unsigned int *)0xE000A248)
#define OEN_2			(*(volatile unsigned int *)0xE000A288)
#define OEN_3			(*(volatile unsigned int *)0xE000A2C8)
//���� GPIO�˿�����Ĵ�����ַ
#define DATA_0			(*(volatile unsigned int *)0xE000A040)
#define DATA_1			(*(volatile unsigned int *)0xE000A044)
#define DATA_2			(*(volatile unsigned int *)0xE000A048)
#define DATA_3			(*(volatile unsigned int *)0xE000A04C)
//���� GPIO�˿�����Ĵ�����ַ
#define DATA_0_RO		(*(volatile unsigned int *)0xE000A060)
#define DATA_1_RO		(*(volatile unsigned int *)0xE000A064)
#define DATA_2_RO		(*(volatile unsigned int *)0xE000A068)
#define DATA_3_RO		(*(volatile unsigned int *)0xE000A06C)

//���� UART1���ŵ�ַ�ĺ궨��
#define rMIO_PIN_48		(*(volatile unsigned long*)0xF80007C0)
#define rMIO_PIN_49 	(*(volatile unsigned long*)0xF80007C4)
#define rUART_CLK_CTRL 	(*(volatile unsigned long*)0xF8000154)
#define rControl_reg0 	(*(volatile unsigned long*)0xE0001000)
#define rMode_reg0 		(*(volatile unsigned long*)0xE0001004)
//���� UART1�˿ڲ����ʵȲ�����ַ�Ĵ����ĺ궨��
#define rBaud_rate_gen_reg0 (*(volatile unsigned long*)0xE0001018)
#define rBaud_rate_divider_reg0 (*(volatile unsigned long*)0xE0001034)
#define rTx_Rx_FIFO0 (*(volatile unsigned long*)0xE0001030)
#define rChannel_sts_reg0 (*(volatile unsigned long*)0xE000102C)

void send_Char_9(unsigned char modbus[]);				//9�ֽڴ��ڷ��ͺ���
void send_Char(unsigned char data);						//���ڷ��ͺ�����һ��һ���ֽ�
void RS232_Init();										//���ڳ�ʼ������
void delay(int i,int n,int m);							//��ʱ����

int getId();			//��ȡѡ�еĻ�е��
int getDir();			//Ҫת���ķ���
int getBoxTrans();		//��ȡ����ʹ��ʹ�ָ��

void turnArm(int arm_id,int dir,int spd);		//arm_id��е�۳��Ŷ�Ӧ�ķ��򰴵�λת��һ��
void moveArm(int dir,int spd);					//��е�����ŵ��������ƶ�
void swTrans(int fg);							//�������߹رմ��ʹ�
void proBox(int fg);							//�ڶ��Ŵ��ʹ���һ������
void catch(int fg);								//��������
void initArm();
void endArm();
void GoBlue();
void GoRed();
void GoGreen();
void autoExColor();
//9���ֽ����ݵķ��ͺ���
void send_Char_9(unsigned char modbus[])
{
	int i;
	char data;
	for(i=0;i<9;i++){
		data=modbus[i];
		send_Char(data);
		delay(100,10,10);		//��ʱ
	}
}

//�����ֽ����ݵķ��ͺ���
void send_Char(unsigned char data)
{
	//���жϷ���FIFO�Ƿ�����������ѭ����ѯ��ֱ������FIFO����
     while((rChannel_sts_reg0&0x10)==0x10);
     //����һ���ֽڵ�����
     rTx_Rx_FIFO0=data;
}

//���պ���
unsigned rec_Char(void)
{
	char data;
	//���жϽ���FIFO�Ƿǿգ�������ѭ����ѯ��ֱ������FIFO����
	while((rChannel_sts_reg0&0x2)==0x2);
	data= rTx_Rx_FIFO0;
	return data;
}

//UART1�ĳ�ʼ������
void RS232_Init()
{
     rMIO_PIN_48=0x000026E0;
     rMIO_PIN_49=0x000026E0;
     rUART_CLK_CTRL=0x00001402;
     rControl_reg0=0x00000017;
     rMode_reg0=0x00000020;
     rBaud_rate_gen_reg0=62;
     rBaud_rate_divider_reg0=6;
}

//��ʱ����
void delay(int n,int m,int p)
{
	 int i,j,k;
	 for(i=1;i<=n;i++){
		 for(j=1;j<=m;j++){
			 for(k=1;k<=p;k++){

			 }
		 }
	 }
}


int direction=0,arm_idx=-1,speed=0,btn=0,track=0;   //����,��е�����±�,ת��,��ť,����
int CurPos[6]={0,0,0,0,0,0}; 	//��ĽǶ�

//��λ����
void reset(){
	for(int i=0;i<6;i++){
		if(CurPos[i]!=0){
			if(CurPos[i]>0){
				while(CurPos[i]!=0){
					CurPos[i]=(CurPos[i]-5)%360;
					turnArm(i,-1,4);
				}
			 }
			 if(CurPos[i]<0){
				 while(CurPos[i]!=0){
					CurPos[i]=(CurPos[i]+5)%360;
					turnArm(i,1,4);
				}
			 }
		}
	}

	//��λ����
	if(track!=0){
		if(track>0){
			moveArm(2,2);
			track-=2;
		}
		else{
			moveArm(1,2);
			track+=2;
		}
	}
}

void singleStep(){
	direction=getDir(); 	//��û�е�۷���
	arm_idx=getId(); 			//��û�е�����
	delay(100,10,10);
	btn=getBoxTrans();
	if(direction!=0){
		DATA_0 = DATA_0 | 0x00000080;		//LED9ָʾ����
		delay(100,10,10);
		while(getDir()!=0);
		DATA_0 = DATA_0 & 0xFFFFFF7F;		//LED9ָʾ����
		CurPos[arm_idx]+=direction*5;
		turnArm(arm_idx,direction,4);
	}
	if(btn!=0){
		DATA_0 = DATA_0 | 0x00000080;		//LED9ָʾ����
		delay(100,10,10);
		while(getBoxTrans()!=0);
		DATA_0 = DATA_0 & 0xFFFFFF7F;		//LED9ָʾ����
		switch(btn){
		case 1:
			moveArm(1,2);track+=2;//��������
			break;
		case 2:
			moveArm(2,2);track-=2;//��������
			break;
		case 4:
			swTrans(1);//���ʹ���
			break;
		case 8:
			swTrans(2);//���ʹ���
			break;
		case 16:
			proBox(1);//������
			break;
		}
	}
}

//������ɫִ�ж���
void autoExColor(){
	initArm();
	for(int count=0;count<10;count++){
		swTrans(1);
		if(count)for(int j=0;j<25;j++){
			delay(1000,500,10);
		}
		proBox(1);
		RS232_Init();
		delay(1000,100,50);
		unsigned char color = rec_Char();
		if(color=='B'){
			GoBlue();
		}else if(color=='G'){
			GoGreen();
		}else if(color=='R'){
			GoRed();
		}
	}
	endArm();
}

void initArm(){
	for(int i=1;i<=18;i++){
		turnArm(0,1,4);
	}
	delay(100,100,50);
	for(int i=1;i<=5;i++){
		turnArm(1,1,4);

	}delay(100,100,50);
	for(int i=1;i<=15;i++){
		turnArm(2,-1,4);

	}delay(100,100,50);
	for(int i=1;i<=28;i++){
		turnArm(4,1,4);

	}delay(100,100,50);
}
void endArm(){
	for(int i=1;i<=18;i++){turnArm(0,-1,4);}
	delay(100,100,50);
	for(int i=1;i<=5;i++){turnArm(1,-1,4);}
	delay(100,100,50);
	for(int i=1;i<=15;i++){turnArm(2,1,4);}
	delay(100,100,50);
	for(int i=1;i<=28;i++){turnArm(4,-1,4);}
	delay(100,100,50);
}

void GoBlue(){
	catch(1);
	for(int i=1;i<=36;i++){
		turnArm(0,-1,4);

	}delay(100,100,50);
	for(int i=1;i<=20;i++){
		moveArm(1,3);

	}delay(100,100,50);
	catch(2);
	for(int i=1;i<=36;i++){
		turnArm(0,1,4);

	}delay(100,100,50);
	for(int i=1;i<=20;i++){
		moveArm(2,3);

	}delay(100,100,50);
}

void GoRed(){
	catch(1);
	for(int i=1;i<=36;i++){
		turnArm(0,-1,4);
	}delay(100,100,50);
	for(int i=1;i<=20;i++){
		moveArm(2,3);

	}delay(100,100,50);
	catch(2);
	for(int i=1;i<=36;i++){
		turnArm(0,1,4);

	}delay(100,100,50);
	for(int i=1;i<=20;i++){
		moveArm(1,3);

	}delay(100,100,50);
}
void GoGreen(){
	catch(1);
	for(int i=1;i<=36;i++){
		turnArm(0,-1,4);

	}delay(100,100,50);
	catch(2);
	for(int i=1;i<=36;i++){
		turnArm(0,1,4);

	}delay(100,100,50);
}


int main()
{

	init_platform();
	u32 flag;		//����flag���ڼ�¼SW0~SW7����������Ϣ;

	//ע������MIO���ź�EMIO���ŵ������ͳһ��ŵģ�MIO���Ϊ0~31��32~53��EMIO���Ϊ54~85��86~117
	//���ü���ʼ��MIO07���ŵ���ؼĴ�����MIO07��ΪLED�ƿ��Ƶ��������
	MIO_PIN_07 = 0x00003600;
	DIRM_0 = DIRM_0|0x00000080;
	OEN_0 = OEN_0|0x00000080;
	//���ü���ʼ��MIO50��MIO51���ŵ���ؼĴ�����MIO50��MIO51��Ϊ������������
	MIO_PIN_50 = 0x00003600;
	MIO_PIN_51 = 0x00003600;
	DIRM_1 = DIRM_1 & 0xFFF3FFFF;
	//��ʼ��EMIO54~EMIO58�����ţ����Ƕ�ӦBTNU��BTND��BTNL��BTNR��BTNC����������
	DIRM_2 = DIRM_2 & 0xFFFFFFE0;
	//��ʼ��EMIO59~EMIO66�����ţ����Ƕ�ӦSW7~SW0�������أ�����
	DIRM_2 = DIRM_2 & 0xFFFFE01F;
	//��ʼ��EMIO67~EMIO74�����ţ����Ƕ�ӦLED7~LED0�����
	DIRM_2 = DIRM_2|0x001FE000;
	OEN_2 = OEN_2|0x001FE000;

	//��ʼ��UART1
	RS232_Init();

	int fgauto=1;
    while(1){
    	//��ģʽ��Ϣ������SW7��SW6��������Ϣ
    	flag = DATA_2_RO&0x00000060;//��EMIO 59-60��
        switch(flag){
        case 0x00:					//��λģʽ
        	DATA_2 = DATA_2&0xFFE01FFF;		//ģʽָʾ��LED7~LED0��
        	reset();
        	fgauto=1;
    		delay(100,10,10);
        	break;
        case 0x20:					//�ֶ�����ģʽ
        	DATA_2 = (DATA_2|0x00002000)&0xFFFFBFFF;	//ָʾ��LED7����LED6��EMIO 67 68��
        	singleStep();
			break;
        case 0x40:					//�Զ�����ģʽ
        	DATA_2 = (DATA_2|0x00004000)&0xFFFF7FFF;	//LED7��LED6��
        	if(fgauto){
				autoExColor();
				delay(1000,100,500);
        	}
        	fgauto=0;
        	break;
        case 0x60:					//��е��ʾ��ģʽ����ģʽ�ݲ�ʵ�֣�
        	DATA_2 = DATA_2|0x00006000;					//LED7����LED6��
        	break;
        }
    }
    return 0;
}


//��ȡ����ʹ��ʹ�ָ��
int getBoxTrans(){
	int flag=DATA_2_RO & 0x0000001f; //0-4λ����Ӧ��������˶������ʹ����ء������ӡ�EMIO 54-58��
	return flag;
}

//��ȡѡ�еĻ�е��
int getId(){
   u32 flag;
   flag=DATA_2_RO & 0x00001f80; //7-12λ��Ӧ6����š�EMIO 61-66��
   switch(flag){				//����ֻ��ѡ��һ�������ת��
   	   case 0x00000080:
   		   return 5;
   	       break;
   	   case 0x00000100:
	       return 4;
	       break;
   	   case 0x00000200:
   		   return 3;
   		   break;
   	   case 0x00000400:
   		   return 2;
   		   break;
   	   case 0x00000800:
   		   return 1;
   		   break;
   	   case 0x00001000:
   		   return 0;
   }
   return -1;
}

//Ҫת���ķ���
int getDir(){
   int flag=DATA_1_RO & 0x000c0000;
   if(flag==0x00040000) return -1; 		//MIO50��
   else if(flag==0x00080000) return 1;  //MIO51˳
   return 0;
}

//arm_id��е�۳��Ŷ�Ӧ�ķ��򰴵�λת��һ��
void turnArm(int arm_id,int dir,int spd){
	unsigned char modbus_com[9];
	modbus_com[0]='#';	//��ʼ�����̶�Ϊ#
	modbus_com[1]='2';
	modbus_com[2]='0';
	modbus_com[3]='0';
	modbus_com[4]='0';
	modbus_com[5]='0';
	modbus_com[6]='0';
	modbus_com[7]='0';
	modbus_com[8]='0';

	if(arm_id==-1 || dir==0)return;
	else if(dir==1)modbus_com[arm_id+2]=0x30+spd;	//˳ʱ��
	else if(dir==-1)modbus_com[arm_id+2]=0x34+spd;	//��ʱ��

	send_Char_9(modbus_com);
}

//��е�����ŵ��������ƶ�
void moveArm(int dir,int spd){
	unsigned char modbus_com[9];
	modbus_com[0]='#';
	modbus_com[1]='2';
	modbus_com[2]='0';
	modbus_com[3]='0';
	modbus_com[4]='0';
	modbus_com[5]='0';
	modbus_com[6]='0';
	modbus_com[7]='0';
	modbus_com[8]='0';

	if(dir==0)return;
	else if(dir==1)modbus_com[8]=0x30+spd;	//�������
	else if(dir==2)modbus_com[8]=0x33+spd;	//�������

	send_Char_9(modbus_com);
}

//�������߹رմ��ʹ�
void swTrans(int fg){
	unsigned char modbus_com[9];
	modbus_com[0]='#';
	modbus_com[1]='6';
	modbus_com[2]='0';
	modbus_com[3]='0';
	modbus_com[4]='0';
	modbus_com[5]='0';
	modbus_com[6]='0';
	modbus_com[7]='0';
	modbus_com[8]='0';
	modbus_com[3]=0x30+fg; 		//fg==1�� 2��

	send_Char_9(modbus_com);
}

//�ڶ��Ŵ��ʹ���һ������
void proBox(int fg){
	unsigned char modbus_com[9];
	modbus_com[0]='#';
	modbus_com[1]='4';
	modbus_com[2]='0';
	modbus_com[3]='0';
	modbus_com[4]='0';
	modbus_com[5]='0';
	modbus_com[6]='0';
	modbus_com[7]='0';
	modbus_com[8]='0';
	modbus_com[3]=0x30+fg;		//fg==0��������   1������
	send_Char_9(modbus_com);

}

//��������
void catch(int fg){
	unsigned char modbus_com[9];
	modbus_com[0]='#';
	modbus_com[1]='5';
	modbus_com[2]='0';
	modbus_com[3]='0';
	modbus_com[4]='0';
	modbus_com[5]='0';
	modbus_com[6]='0';
	modbus_com[7]='0';
	modbus_com[8]='0';
	modbus_com[3]=0x30+fg;		//fg==0������   1��  2��
	send_Char_9(modbus_com);
}
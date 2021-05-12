/*******************************************************
*Copyright(C),2020,Shanghai Eastsoft Microelectronics Co.,Ltd.
*�ļ�����capture.c
*��  �ߣ�AE Team
*��  ����v1.0��������iDesigner(v4.2.3.166) + ������HRCC(v1.2.0.106)
*��  �ڣ�2020/07/08
*��  ����T21��ʱ����׽ģʽ��T21_CH0(PA0)�����ز�׽����׽�ɹ���תPA1��T21_CH1(PA7)�½��ز�׽����׽�ɹ���תPA6��
        T21_CH2(PB7)˫�ز�׽����׽�ɹ���תPA3��PA3������Ϊ���������������capvalue0��capvalue1��capvalue2���沶׽ʱ��׽�Ĵ�����ֵ��
*��  ע������������ѧϰ����ʾʹ�ã����û�ֱ�����ô����������ķ��ջ������е��κη������Ρ�
*******************************************************/
#include <hic.h>

#define CLRWDT()   {__Asm CWDT;}        //�궨���幷ָ��

unsigned int capvalue0,capvalue1,capvalue2;        //�������沶׽�Ĵ�����ֵ

/******************************************************
��������void RAMclear(void)
��  ����RAM���������㣬RAM����ַ0x0000~0x03FF
����ֵ����
���ֵ����
����ֵ����
*******************************************************/
void RAMclear(void)
{
    for (IAAH=0; IAAH<=0x03; IAAH++)
    {
        for (IAAL=0; IAAL<0xFF; IAAL++)
            IAD = 0x00;
        IAD = 0x00;
    }
}

/******************************************************
��������void GPIOInit(void)
��  �������ȳ�ʼ������δ�õ���IO��Ϊ����͵�ƽ
����ֵ����
���ֵ����
����ֵ����
*******************************************************/
void GPIOInit(void)
{
    ANSL = 0xFF;        //ѡ���Ӧ�˿�Ϊ����IO����
	ANSH = 0xFF;
    PAT = 0x00;         //����IO��Ϊ���
    PBT = 0x00;
    PCT = 0x00;
    PA = 0x00;
    PB = 0x00;
    PC = 0x00;

    PAT0 = 1;           //T21_CH0��Ϊ����
    PAT7 = 1;           //T21_CH1��Ϊ����
    PBT7 = 1;           //T21_CH2��Ϊ����
    //PAPD0 = 1;        //ʹ������
    //PAPU7 = 1;        //ʹ������
	//PBPD7 = 1;        //ʹ������
}

/*******************************************************
��������void isr(void) interrupt
��  �����жϷ������
����ֵ����
���ֵ����
����ֵ����
*******************************************************/
void isr(void) interrupt
{
    if (T21MIE0==1 && T21MIF0==1)        //��ʱ����׽0�жϣ������ز�׽
    { 
        T21MIF0 = 0;        //���־λ
        capvalue0 = T21R0H;               //���沶׽�Ĵ�����ֵ
        capvalue0 = (capvalue0<<8) | T21R0L;
        PA1 = ~PA1;
    }

    if (T21MIE1==1 && T21MIF1==1)        //��ʱ����׽1�жϣ��½��ز�׽
    {       
        T21MIF1 = 0;        //���־λ
        capvalue1 = T21R1H;               //���沶׽�Ĵ�����ֵ
        capvalue1 = (capvalue1<<8) | T21R1L;
        PA6 = ~PA6;
    }

    if (T21MIE2==1 && T21MIF2==1)        //��ʱ����׽2�жϣ�˫�ز�׽
    {    
        T21MIF2 = 0;        //���־λ
        capvalue2 = T21R2H;               //���沶׽�Ĵ�����ֵ
        capvalue2 = (capvalue2<<8) | T21R2L;
        PA3 = ~PA3;
    }
}

/*******************************************************
��������void main(void)
��  ������������T21_CH0(PA0)��T21_CH1(PA7)������ͬ�Ĳ��Σ�����״̬�¿���ֱ�ӿ������Ⲩ�ε����ں�ռ�ձ�
����ֵ����
���ֵ����
����ֵ����
*******************************************************/
void main(void) 
{
    RAMclear();
    GPIOInit();
    T21CL = 0x46;        //��׽ģʽ��T21_CH0�����ز�׽��T21_CH1�½��ز�׽
    T21CM = 0xCF;        //T21_CH2˫�ز�׽��������Ԥ��Ƶ1:16
    T21OC = 0x25;        //ʹ��T21_CH0(PA0)��T21_CH1(PA7)��T21_CH2(PB7)�˿ڹ���
    T21MIE0 = 1;         //��T21��׽0�ж�
    T21MIE1 = 1;         //��T21��׽1�ж�
    T21MIE2 = 1;         //��T21��׽2�ж�
    T21MIF0 = 0;         //���־λ
    T21MIF1 = 0;         //���־λ
    T21MIF2 = 0;         //���־λ
    GIE = 1;             //��ȫ���ж�
    T21EN = 1;           //ʹ��T21
    while (1)
    {
        if (T21VIF == 1)        //���������
        {
            T21VIF = 0;
        }
        CLRWDT();
    }
}
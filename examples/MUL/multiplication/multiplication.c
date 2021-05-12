/*********************************************************
*Copyright (C), 2020, Shanghai Eastsoft Microelectronics Co., Ltd
*�ļ���:  multiplication.c
*��  ��:  AE Team
*��  ��:  v1.0��������iDesigner(v4.2.3.166) + ������HRCC(v1.2.0.118)
*��  ��:  2020/12/28
*��  ��:  Ӳ���˷�������
          �궨��MULA_DATA������A
          �궨��MULB_DATA������B
		  ����product���˻�
          ע�⣺�жϷ��������ܻ�ı�����Ĵ���MULA��MULB�����յ��³˻������ж��ַ�ʽ��������ַ��ա�
          ��ʽһ��ʹ��Ӳ���˷���֮ǰ����ֹȫ���ж�ʹ�ܣ�GIE=0������ǰ���жϵ�ǰGIE״̬����
		          �˷�������ɺ󣬽��˻��������ٻָ�ȫ���ж�ʹ�ܣ�GIE=1����
          ��ʽ�����Ƚ������ͱ������������ض��ı����С������������������жϷ���������Զ����ݺͻָ������Ĵ�����
		  ������չʾ��ʽ����Ϊ����ʹ�ã���ʹ��v1.2.0.113�����ϰ汾�Ĺ���������ͷ�ļ�������Ҫ�ı��������ͺ궨�塣
*��  ע:  ����������ѧϰ����ʾʹ�ã����û�ֱ�����ô����������ķ��ջ������е��κη������Ρ�
**********************************************************/
#include <hic.h>

#define CLRWDT()   {__Asm CWDT;}        //�궨���幷ָ��

#define uchar unsigned char
#define uint unsigned int

#define MULA_DATA 0xFE  //����A
#define MULB_DATA 0xDC  //����B

uint product;     //�˻�����

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
    PAT = 0x00;         //����IO��Ϊ����͵�ƽ
    PBT = 0x00;
    PCT = 0x00;
    PA = 0x00;
    PB = 0x00;
    PC = 0x00;
}

/**********************************************
��������void main(void)
��  ������������SET_MULA��SET_MULB���ó���A������B��ֵ��ͷ�ļ��Ѱ����궨���������product�洢�˻�ֵ��
����ֵ����
���ֵ����
����ֵ����
**********************************************/
void main(void) 
{
    RAMclear();
    GPIOInit();

	SET_MULA(MULA_DATA);
	SET_MULB(MULB_DATA);

    product = (MULH << 8) | MULL;

    while(1);
}
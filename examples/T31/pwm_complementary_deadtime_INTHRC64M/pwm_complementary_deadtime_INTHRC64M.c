/*******************************************************
*Copyright(C),2020,Shanghai Eastsoft Microelectronics Co.,Ltd.
*文件名：pwm_complementary_deadtime_INTHRC64M.c
*作  者：AE Team
*版  本：v1.0，编译于iDesigner(v4.2.3.166) + 工具链HRCC(v1.2.0.106)
*日  期：2020/12/17
*描  述：多功能定时器，PWM模式计时时钟源选择INTHRC，T31_CH1(PB5)/T31_CH1N(PB2)输出死区互补pwm，T31_ETR(PB0)高电平时T31_CH1输出低，
        T31_CH1N输出高(与CH1P/CH1NP有关)，PB1高电平刹车。
        注意：PWM模式调试状态下，须软件固定设置T31C0H<6>=1，否则PWM输出可能会异常。
*备  注：对用户直接引用代码所带来的风险或后果不承担任何法律责任。
*******************************************************/
#include <hic.h>

#define CLRWDT()   {__Asm CWDT;}        //宏定义清狗指令

/******************************************************
函数名：void RAMclear(void)
描  述：RAM区数据清零，RAM区地址0x0000~0x03FF
输入值：无
输出值：无
返回值：无
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
函数名：void GPIOInit(void)
描  述：首先初始化所有未用到的IO口为输出低电平
输入值：无
输出值：无
返回值：无
*******************************************************/
void GPIOInit(void)
{
    ANSL = 0xFF;        //选择对应端口为数字IO功能
	ANSH = 0xFF;        //选择对应端口为数字IO功能
    PAT = 0x00;         //所有IO设为输出低电平
    PBT = 0x00;
    PCT = 0x00;
    PA = 0x00;
    PB = 0x00;
    PC = 0x00;

    PBT0 = 1;           //T31_ETR设为输入
    PBPD0 = 1;          //使能下拉
    PBT1 = 1;           //TIM1_BRK设为输入
    PBPD1 = 1;
}

/*******************************************************
函数名：void main(void)
描  述：主函数，T31_CH1(PB5)/T31_CH1N(PB2)输出死区互补pwm，PB0高电平时T31_CH1输出低，T31_CH1N输出高，
        PB1高电平刹车刹车自动关闭管脚输出功能，即刹车CHOE自动清0，重启需手动置CHOE为1
输入值：无
输出值：无
返回值：无
*******************************************************/
void main(void) 
{
    RAMclear();
    GPIOInit();
    T31C0H = 0xC2;          //T31C0H<6>(HTOEOFF)调试模式下需软件固定为1，死区发生器时钟频率tDTS=2*tCK_INT，PWM时钟源选择INTHRC
    T31C0L = 0x80;          //边沿计数模式，向上计数，使能T31CNTLD缓冲功能(即写入值在下一次更新事件时生效)
    T31CH1C = 0xE8;         //通道1配置为输出，使能输出比较1清零，pwm1模式,使能TIM_CCR1缓冲功能 
    T31PINCL = 0x05;        //T31_CH1高电平有效，使能输出。T31_CH1N通道高电平有效，使能输出。
    T31C2L = 0x08;          //禁止从模式,使能外部事件清除OCnREF
    T31PRSH = 0x00;         //预分频系数1:16
    T31PRSL = 0x0F;
    T31CNTLDH = 0x03;       //重装载值，代表pwm周期长度
    T31CNTLDL = 0xE8;
    T31CH1RH = 0x01;        //比较寄存器1值，代表pwm占空比长度
    T31CH1RL = 0x4D;
    T31CHBK |= 0x30;        //使能高电平刹车
    T31DLYT = 0x7F;         //Tdtg=tDTS=2*tCK_INT,死区时间31.75us
    HALT_PWM = 0;           //在线调试暂停状态下，PWM仍输出
    UPT = 1;        //更新所有寄存器的缓冲器，使写入值立即生效
    BKIC = 1;       //清刹车标志位
    CHOE = 1;       //使能输出
    T31EN = 1;      //使能计数器
    while (1)
    {
        CLRWDT();
    }
}
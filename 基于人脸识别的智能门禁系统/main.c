#define GPA1CON (*(volatile unsigned int *)0x11400020)      //配置寄存器
#define ULCON2 (*(volatile unsigned int *)0x13820000)       //串口模块功能配置寄存器1
#define UCON2 (*(volatile unsigned int *)0x13820004)        //串口模块功能配置寄存器2
#define UBRDIV2 (*(volatile unsigned int *)0x13820028)      //波特率整数部分的设置
#define UFRACVAL2 (*(volatile unsigned int *)0x1382002c)    //波特率小数部分的设置
#define UTXH2 (*(volatile unsigned int *)0x13820020)        //串口输出缓冲寄存器
#define URXH2 (*(volatile unsigned int *)0x13820024)        //串口接收缓冲寄存器
#define UTRSTAT2 (*(volatile unsigned int *)0x13820010)        //串口输出状态寄存器

#define GPX1CON (*(volatile unsigned int *)0x11000C20) //(*(unsigned int*)0X11000C40)//将基地址加上偏移的地址强转为指针
#define GPX1DAT (*(volatile unsigned int *)0x11000C24) //数据寄存器
#define GPF3CON (*(volatile unsigned int *)0x114001E0) //(*(unsigned int*)0X11000C40)//将基地址加上偏移的地址强转为指针
#define GPF3DAT (*(volatile unsigned int *)0x114001E4) //数据寄存器
#define GPX2CON (*(volatile unsigned int *)0x11000C40) //(*(unsigned int*)0X11000C40)//将基地址加上偏移的地址强转为指针
#define GPX2DAT (*(volatile unsigned int *)0x11000C44) //数据寄存器

#define EXT_INT41CON    (*(volatile unsigned int *)0x11000E04)
#define EXT_INT41_MASK  (*(volatile unsigned int *)0x11000F04)
#define EXT_INT41_PEND  (*(volatile unsigned int *)0x11000F44)

#define GPX3CON         (*(volatile unsigned int *)0x11000C60)

#define EXT_INT43CON    (*(volatile unsigned int *)0x11000E0C)
#define EXT_INT43_MASK  (*(volatile unsigned int *)0x11000F0C)
#define EXT_INT43_PEND  (*(volatile unsigned int *)0x11000F4C)


#define ICDISER1_CPU0   (*(volatile unsigned int *)0x10490104)
#define ICDIPTR14_CPU0  (*(volatile unsigned int *)0x10490838)
#define ICDISER2_CPU0   (*(volatile unsigned int *)0x10490108)
#define ICDIPTR16_CPU0  (*(volatile unsigned int *)0x10490840)

#define ICDDCR          (*(volatile unsigned int *)0x10490000)
#define ICCICR_CPU0     (*(volatile unsigned int *)0x10480000)
#define ICCPMR_CPU0     (*(volatile unsigned int *)0x10480004)
#define ICCIAR_CPU0     (*(volatile unsigned int *)0x1048000C)
#define ICDICPR1_CPU0   (*(volatile unsigned int *)0x10490284)
#define ICDICPR2_CPU0   (*(volatile unsigned int *)0x10490288)
#define ICCEOIR_CPU0    (*(volatile unsigned int *)0x10480010)

int Key2 = 0;
int Key3 = 0;
void interrupt_init()
{
    //1. GPIO通用功能的设置,设置为中断模式    
	//k2   GPX1_1
    GPX1CON = GPX1CON | (0xf<<4);  //set interrupt mode
    EXT_INT41CON = (EXT_INT41CON & (~(0x7<<4))) | (0x2<<4); //Triggers Falling edge
    EXT_INT41_MASK = EXT_INT41_MASK & (~(0x1<<1)); //set Enabled interrupt
	//k3   GPX1_2
	GPX1CON = GPX1CON | (0xf<<8);
	EXT_INT41CON = (EXT_INT41CON & (~(0x7<<8))) | (0x2<<8);
	EXT_INT41_MASK = EXT_INT41_MASK & (~(0x1<<2));
	//k4   GPX3_2
	GPX3CON = GPX3CON | (0xf<<8);
	EXT_INT43CON = (EXT_INT43CON & (~(0x7<<8))) | (0x2<<8);
	EXT_INT43_MASK = EXT_INT43_MASK & (~(0x1<<2));
	
    //2. 中断功能模块的配置
	//k2
    ICDISER1_CPU0 = ICDISER1_CPU0 | (0x1<<25); //set port=25 Enabled Interrupt
    ICDIPTR14_CPU0 = (ICDIPTR14_CPU0 & (~(0xff<<8))) | (0x1<<8); //把port=25 sent cpu0
	//k3
	ICDISER1_CPU0 = ICDISER1_CPU0 | (0x1<<26); //set port=26 Enabled Interrupt
	ICDIPTR14_CPU0 = (ICDIPTR14_CPU0 & (~(0xff<<16))) | (0x1<<16); //把port=26 sent cpu0
	//k4
	ICDISER2_CPU0 = ICDISER2_CPU0 | (0x1);   //set port=32 Enabled Interrupt
	ICDIPTR16_CPU0 = (ICDIPTR16_CPU0 & (~(0xff))) | (0x1); //把port=32 sent cpu0
	
    ICDDCR = ICDDCR | 0x1; //set 分发使能
    ICCICR_CPU0 = ICCICR_CPU0 | 0x1; //设置接口使能
    ICCPMR_CPU0 = ICCPMR_CPU0 | 0xff; //设置优先级
}

char STRING[20] = "";                                       //gets的全局变量
void delay(int x)                                           //延时函数
{
    int i;
    while (x--)
    {
        for (i = 0; i < 1024; i++)
        {}
    }
}

void init_urat(void)
{
    //GPA1CON-0、1分别置为普通串口输入输出模式
    GPA1CON = (GPA1CON & (~(0xff<<0)))|(0x22<<0);   //低8位清零后赋值0x22
    //配置COM2对应串口功能
    ULCON2 = 0x3;                                   //配置数据位8位，停止位1位，无校验位。
    UCON2 = 0x5;                                    //配置接收和发送方式皆为轮询模式。
    //波特率设置
    UBRDIV2 = 53;                                   //整数部分设置为53
    UFRACVAL2 = 4;                                  //小数部分设置为4
}

void putc(char ch)
{
    while ((UTRSTAT2 & (1<<1))== 0)
    {
        ;//判断发送缓冲区中是否为空，为空才继续往里放新的数据（跳出循环）
    }
    
    UTXH2 = ch;
}

char getc(void)
{
    while ((UTRSTAT2 & (1<<0))== 0)
    {
        ;//判断接收缓冲区中是否为空，非空才读取里的数据（跳出循环）
    }
    putc(URXH2 & 0xff);
    return (URXH2 & 0xff) ; //保留一个数的低八位
}

void do_irq()
{
    int irq_num = ICCIAR_CPU0 & 0x3ff; //get interrupt ID
    switch (irq_num) {
        case 57:        //key2
            Key2 = 1;   //对应赋值1
            GPX2DAT = (GPX2DAT | (1<<7));               //第8位赋值0x1
            if(Key3 == 1)   //如果在此之前已经按了Key3键
            {   //点亮所有灯
                GPX2DAT = (GPX2DAT | (1<<7));  //第7位赋值0x1
                //GPX1CON-0置为高电平
                GPX1DAT = (GPX1DAT | (1<<0));  //第0位赋值0x1
                //GPF3DAT-4置为高电平
                GPF3DAT = (GPF3DAT | (1<<4));  //第4位赋值0x1
                //GPF3DAT-5置为高电平
                GPF3DAT = (GPF3DAT | (1<<5));  //第5位赋值0x1
                Key2 = 0;//对应清零
                Key3 = 0;//对应清零
            }
            else
            {   // 关闭所有灯
                GPX2DAT = (GPX2DAT & (~(1<<7)));  //第7位赋值0
                //GPX1CON-0置为低电平
                GPX1DAT = (GPX1DAT & (~(1<<0)));  //第0位赋值0
                //GPF3DAT-4置为低电平
                GPF3DAT = (GPF3DAT & (~(1<<4)));  //第4位赋值0
                //GPF3DAT-5置为低电平
                GPF3DAT = (GPF3DAT & (~(1<<5)));  //第5位赋值0
            }
            putc('2');  //打印当前按键信息（调试时使用）
            EXT_INT41_PEND = EXT_INT41_PEND | (0x1<<1); //对外:pending-clear清除中断标志
            ICDICPR1_CPU0 = ICDICPR1_CPU0 | (0x1<<25); //对内:pending-clear清除中断标志
            break;
		case 58:        //key3
            Key3 = 1;   //对应赋值1
            GPX2DAT = (GPX2DAT & (~(1<<7)));  //第7位赋值0
            //GPX1CON-0置为低电平
            GPX1DAT = (GPX1DAT & (~(1<<0)));  //第0位赋值0
            //GPF3DAT-4置为低电平
            GPF3DAT = (GPF3DAT & (~(1<<4)));  //第4位赋值0
            //GPF3DAT-5置为低电平
            GPF3DAT = (GPF3DAT & (~(1<<5)));  //第5位赋值0
            GPX2DAT = (GPX2DAT | (1<<7));  //第8位赋值0x1
            if(Key2 == 1)//如果在此之前已经按了Key2键
            {   // 关闭所有灯
                Key2 = 0;//对应清零
                Key3 = 0;//对应清零
                GPX2DAT = (GPX2DAT & (~(1<<7)));  //第7位赋值0
                //GPX1CON-0置为低电平
                GPX1DAT = (GPX1DAT & (~(1<<0)));  //第0位赋值0
                //GPF3DAT-4置为低电平
                GPF3DAT = (GPF3DAT & (~(1<<4)));  //第4位赋值0
                //GPF3DAT-5置为低电平
                GPF3DAT = (GPF3DAT & (~(1<<5)));  //第5位赋值0
            }
			putc('3');//打印当前按键信息（调试时使用）
			EXT_INT41_PEND = EXT_INT41_PEND | (0x1<<2); //对外:pending-clear清除中断标志
			ICDICPR1_CPU0 = ICDICPR1_CPU0 | (0x1<<26);  //对内:pending-clear清除中断标志
			break;
		case 64:   //key4
			putc('4'); 
			EXT_INT43_PEND = EXT_INT43_PEND | (0x1<<2); //对外:pending-clear清除中断标志
			ICDICPR2_CPU0 = ICDICPR2_CPU0 | (0x1);
			break;
        default:
            putc('0');
            break;
    }
    ICCEOIR_CPU0 = (ICCEOIR_CPU0 & (~0x3ff)) | irq_num; //结束中断
}



void main(void)
{
    GPX2CON = (GPX2CON & (~(0xf<<28)))|(0x1<<28);  //高四位清零后赋值0x1
    //LED3:GPX1CON-0置为输出模式
    GPX1CON = (GPX1CON & (~(0xf<<0)))|(0x1<<0);  //四位清零后赋值0x1
    //LED4:GPF3CON-4置为输出模式
    GPF3CON = (GPF3CON & (~(0xf<<16)))|(0x1<<16);  //四位清零后赋值0x1
    //LED5:GPF3CON-5置为输出模式
    GPF3CON = (GPF3CON & (~(0xf<<20)))|(0x1<<20);  //四位清零后赋值0x1

    int i;
    char ch;
    //所有灯灭
    GPX2DAT = (GPX2DAT & (~(1<<7)));  //第8位赋值0
    //GPX1CON-0置为低电平
    GPX1DAT = (GPX1DAT & (~(1<<0)));  //第4位赋值0x1
    //GPF3DAT-4置为低电平
    GPF3DAT = (GPF3DAT & (~(1<<4)));  //第5位赋值0x1
    //GPF3DAT-5置为低电平
    GPF3DAT = (GPF3DAT & (~(1<<5)));  //第6位赋值0x1
    interrupt_init();   //中断初始化
    init_urat();        //串口初始化
    while (1)
    {
        while(1)
    {
        ch = getc();    //获取接收缓冲区的字符
        if(ch == 'o')   //如为开门命令
        break;          //跳转打开灯
    }
    GPX2DAT = (GPX2DAT | (1<<7));  //第8位赋值0x1
    //GPX1CON-0置为高电平
    GPX1DAT = (GPX1DAT | (1<<0));  //第8位赋值0x1
    //GPF3DAT-4置为高电平
    GPF3DAT = (GPF3DAT | (1<<4));  //第8位赋值0x1
    //GPF3DAT-5置为高电平
    GPF3DAT = (GPF3DAT | (1<<5));  //第8位赋值0x1

    delay(1000);

    GPX2DAT = (GPX2DAT & (~(1<<7)));  //第8位赋值0
    //GPX1CON-0置为低电平
    GPX1DAT = (GPX1DAT & (~(1<<0)));  //第4位赋值0x1
    //GPF3DAT-4置为低电平
    GPF3DAT = (GPF3DAT & (~(1<<4)));  //第5位赋值0x1
    //GPF3DAT-5置为低电平
    GPF3DAT = (GPF3DAT & (~(1<<5)));  //第6位赋值0x1
    
    delay(1000);
    }
    return;
}
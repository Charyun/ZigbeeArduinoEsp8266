#include "sensor.h"
#include  "hal_adc.h"
//32MHZ us延时函数；
#pragma optimize=none
void delay_us(unsigned int n)
{
    n>>=1;
    while(n--)
    {
          asm("NOP");
          asm("NOP");
          asm("NOP");
          asm("NOP");
          asm("NOP");
          asm("NOP");
          asm("NOP");
          asm("NOP");
          asm("NOP");
          asm("NOP");
          asm("NOP");
          asm("NOP");
          asm("NOP");
          asm("NOP");
          asm("NOP");
    }
}



/***********************************************
*  函数名称： delay_ms
*  功能    ： ms延时函数，系统默认时钟为16MHZ
*  参数列表： n_ms 为延时的ms数，最大不超过65535 
*  返回值  ： 无
************************************************/
void delay_ms(unsigned int n_ms)
{
    unsigned int i,j;
    
    for (i = 0; i < n_ms; i++)
    {
        for (j = 0; j < 1070; j++);
    }
}


void sensor_init(void)
{
     io_init();
     HalAdcInit();
}



unsigned char uint16_to_str(unsigned char *str , unsigned int n)
{
      unsigned char len = 0;
                 
      if(n/10000 != 0)
      {
          str[0] = n/10000+0x30;
          str[1] = n%10000/1000+0x30;
          str[2] = n%1000/100 +0x30;
          str[3] = n%100/10 + 0x30;
          str[4] = n%10+0x30;
                 
          len = 5;
      }
      else if(n/1000 != 0)
      {
          str[0] = n/1000+0x30;
          str[1] = n%1000/100+0x30;
          str[2] = n%100/10 +0x30;
          str[3] = n%10 + 0x30;
              
          len = 4;
      }
      else if(n/100 != 0)
      {
          str[0] = n/100+0x30;
          str[1] = n%100/10+0x30;
          str[2] = n%10 +0x30;

          len = 3;
      }
      else if(n/10 != 0)
      {
          str[0] = n/10+0x30;
          str[1] = n%10+0x30;
          
          len = 2;
      }
      else
      {
          str[0] = n+0x30;
          
          len = 1;
      }
              
      return len;
}



unsigned int sennor_getpmval(void)
{
    unsigned int pmval = 0;
      
    sensor_led = 0;  
    delay_us(280); //280us;
    
    pmval = HalAdcRead(HAL_ADC_CHANNEL_5,HAL_ADC_RESOLUTION_12);
    delay_us(40); //40us;
    
    sensor_led = 1;
    delay_us(9680);   //9680us;
 
    return pmval;
}





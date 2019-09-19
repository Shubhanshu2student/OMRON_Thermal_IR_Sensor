#include "i2c.h"
#include "stm32f0xx_rcc.h"
#include "stm32f0xx_gpio.h"
#include "stm32f0xx_i2c.h"
#include "stm32f0xx_rcc.h"
#include "stm32f0xx.h"
void I2C1_gpio_init()
{
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
  RCC_I2CCLKConfig(RCC_I2C1CLK_HSI);
  GPIO_InitTypeDef GPIO_InitStructure;
  
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_1);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_1);
  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;     //SDA
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD; //GPIO_OType_PP
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; //GPIO_PuPd_UP; 
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;     //SCL
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void I2C1_init()
{
  I2C_InitTypeDef I2C_InitStructure;
  I2C_InitStructure.I2C_Timing = 0x2000090E;    //100Khz
  I2C_InitStructure.I2C_AnalogFilter = I2C_AnalogFilter_Enable;
  I2C_InitStructure.I2C_DigitalFilter = 0x00;
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
  I2C_InitStructure.I2C_OwnAddress1 = 0x00;
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
  I2C_Init(I2C1,&I2C_InitStructure);
  I2C_Cmd(I2C1, ENABLE);
  
  I2C_AcknowledgeConfig(I2C1, ENABLE);
  I2C_AutoEndCmd(I2C1, ENABLE);
}

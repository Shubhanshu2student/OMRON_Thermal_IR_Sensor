#include "thermal_ir.h"
#include "stm32f0xx_i2c.h"
#include "i2c.h"
#include "uart-handler.h"
#include "byte-stuffing/byte-stuffing.h"


uint8_t command,rx_buff[40],buf[100];

__packed struct thermal_ir_data
{
  uint16_t pTAT;
  uint16_t P0;
  uint16_t P1;
  uint16_t P2;
  uint16_t P3;
  uint16_t P4;
  uint16_t P5;
  uint16_t P6;
  uint16_t P7;
  uint16_t P8;
  uint16_t P9;
  uint16_t P10;
  uint16_t P11;
  uint16_t P12;
  uint16_t P13;
  uint16_t P14;
  uint16_t P15;
  uint8_t PEC;
}thermal_ir_data_tag;

void 
thermal_ir_read()
{  
  command = 0x4C;
  I2C_TransferHandling(I2C1,0x14,1,I2C_SoftEnd_Mode,
                       I2C_Generate_Start_Write);
  while(I2C_GetFlagStatus(I2C1, I2C_ISR_TXIS) == RESET);
  I2C_SendData(I2C1,command);
  while(I2C_GetFlagStatus(I2C1, I2C_ISR_TC) == RESET);

  I2C_TransferHandling(I2C1,0x15,35,I2C_SoftEnd_Mode,
                       I2C_Generate_Start_Read);
  
  for(uint8_t idx = 0;idx<35;idx++)
  {
    while(I2C_GetFlagStatus(I2C1, I2C_ISR_RXNE) == RESET);
    rx_buff[idx] = I2C_ReceiveData(I2C1);
    I2C_ClearFlag(I2C1,I2C_FLAG_TC);
  }
  I2C_GenerateSTOP(I2C1,ENABLE);
  while(I2C_GetFlagStatus(I2C1,I2C_ISR_STOPF) == RESET);
  if(!D6T_checkPEC(rx_buff, 35))
  {
    return ;    //crc not matched
  }
  
  thermal_ir_data_tag.pTAT = 256*rx_buff[1] + rx_buff[0];
  thermal_ir_data_tag.P0 = 256*rx_buff[3] + rx_buff[2];
  thermal_ir_data_tag.P1 = 256*rx_buff[5] + rx_buff[4];
  thermal_ir_data_tag.P2 = 256*rx_buff[7] + rx_buff[6];
  thermal_ir_data_tag.P3 = 256*rx_buff[9] + rx_buff[8];
  thermal_ir_data_tag.P4 = 256*rx_buff[11] + rx_buff[10];
  thermal_ir_data_tag.P5 = 256*rx_buff[13] + rx_buff[12];
  thermal_ir_data_tag.P6 = 256*rx_buff[15] + rx_buff[14];
  thermal_ir_data_tag.P7 = 256*rx_buff[17] + rx_buff[16];
  thermal_ir_data_tag.P8 = 256*rx_buff[19] + rx_buff[18];
  thermal_ir_data_tag.P9 = 256*rx_buff[21] + rx_buff[20];
  thermal_ir_data_tag.P10 = 256*rx_buff[23] + rx_buff[22];
  thermal_ir_data_tag.P11 = 256*rx_buff[25] + rx_buff[24];
  thermal_ir_data_tag.P12 = 256*rx_buff[27] + rx_buff[26];
  thermal_ir_data_tag.P13 = 256*rx_buff[29] + rx_buff[28];
  thermal_ir_data_tag.P14 = 256*rx_buff[31] + rx_buff[30] ;
  thermal_ir_data_tag.P15 = 256*rx_buff[33] + rx_buff[32];
  thermal_ir_data_tag.PEC = rx_buff[34];
}


uint8_t Len=0;
void
thermal_ir_communication()
{
  memset(buf,0,100);
  Len = byte_stuff((uint8_t *)&thermal_ir_data_tag,buf,sizeof(thermal_ir_data_tag));
  uart_handler_write_data(Len,buf);
}

uint8_t
calc_crc(uint8_t data)
{
  uint8_t index,temp;
  
  for(index=0;index<8;index++){
    temp = data;
    data <<= 1;
    if(temp & 0x80) data ^= 0x07;
  }
  return data;
}

uint8_t
D6T_checkPEC(uint8_t buf[],uint8_t size)
{
  uint8_t crc,i;
  
  crc = calc_crc( 0x14 );
  crc = calc_crc( 0x4C ^ crc );
  crc = calc_crc( 0x15 ^ crc );
  for(i = 0; i <size; i++){
    crc = calc_crc( buf[i] ^ crc );
  }
  return (crc == buf[size]);
}

#ifndef __SERIAL_H
#define __SERIAL_H

#include "dk.h"

extern volatile uint8_t avoid_flag;

void Serial_Init(void);
void Serial_SendByte(uint8_t Byte);
void Serial_SendArray(uint8_t *Array, uint16_t Length);
void Serial_SendString(char *String);
void Serial_SendNumber(uint32_t Number, uint8_t Length);
void Serial_Printf(char *format, ...);
void echo_serial_data(void);
void ProcessReceivedPacket(void);

uint8_t Serial_GetRxFlag(void);
uint8_t Serial_GetRxData(void);

#endif

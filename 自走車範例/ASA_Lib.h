// build at 12/19/2018 16:07:53
// version : 0.1.0

#ifndef ASA_LIB_H
#define ASA_LIB_H

/*-- common section ----------------------------------------------------------*/
void ASABUS_ID_init(void);
void ASABUS_ID_set(char id);

void ASABUS_SPI_init(void);
char ASABUS_SPI_swap(char data);

void ASABUS_UART_init(void);
void ASABUS_UART_transmit(char data);
char ASABUS_UART_receive(void);

#include <avr\interrupt.h>
#undef ISR
#ifdef __cplusplus
#  define ISR(vector, ...)            \
    extern "C" void vector ## _routine (void) __attribute__ ((__INTR_ATTRS)) __VA_ARGS__; \
    void vector ## _routine (void)
#else
#  define ISR(vector, ...)            \
    void vector ## _routine (void) __attribute__ ((__INTR_ATTRS)) __VA_ARGS__; \
    void vector ## _routine (void)
#endif

#include <stdio.h>
#include <avr\io.h>
#include <avr\interrupt.h>
#include <util\delay.h>
#include <inttypes.h>
void ASA_M128_set(void);

void ASA_STDIO_init(void);

/*-- hardware section --------------------------------------------------------*/
char M128_ACOMP_set(char LSByte, char Mask, char Shift, char Data);
char M128_ACOMP_fpt(char LSByte, char Mask, char Shift, char Data);
char M128_ACOMP_fgt(char LSByte, char Mask, char Shift, void *Data_p);
char M128_ACOMP_put(char LSbyte, char Bytes, void *Data_p);
char M128_ACOMP_get(char LSbyte, char Bytes, void *Data_p);
char M128_ACOMP_isr(char number, void (*function)(void));

char M128_ADC_set(char LSByte, char Mask, char Shift, char Data);
char M128_ADC_fpt(char LSByte, char Mask, char Shift, char Data);
char M128_ADC_fgt(char LSByte, char Mask, char Shift, void *Data_p);
char M128_ADC_get(char LSByte, char Bytes, void *Data_p);
char M128_ADC_isr(char Number, void (*function)(void));

char M128_DIO_set(char LSByte, char Mask, char Shift, char Data);
char M128_DIO_fpt(char LSByte, char Mask, char Shift, char Data);
char M128_DIO_fgt(char LSByte, char Mask, char Shift, void *Data_p);
char M128_DIO_put(char LSByte, char Bytes, void *Data_p);
char M128_DIO_get(char LSByte, char Bytes, void *Data_p);

char M128_EXT_set(char LSByte, char Mask, char Shift, char Data);
char M128_EXT_fpt(char LSByte, char Mask, char Shift, char Data);
char M128_EXT_fgt(char LSByte, char Mask, char Shift, void *Data_p);
char M128_EXT_isr(char number, void (*function)(void));

char M128_SPI_set(char LSByte, char Mask, char Shift, char Data);
char M128_SPI_fpt(char LSByte, char Mask, char Shift, char Data);
char M128_SPI_fgt(char LSByte, char Mask, char Shift, void *Data_p);
char M128_SPI_put(char LSByte,char Bytes,char *Data_p);
char M128_SPI_get(char LSByte,char Bytes,char *Data_p);
char M128_SPI_isr(char number, void (*function)(void));

char M128_TIM_set(char LSByte, char Mask, char Shift, char Data);
char M128_TIM_fpt(char LSByte, char Mask, char Shift, char Data);
char M128_TIM_put(char LSByte, char Bytes, void *Data_p);
char M128_TIM_get(char LSByte, char Bytes, void *Data_p);
char M128_TIM_isr(char number, void (*function)(void));

char M128_TWI_set(char LSByte, char Mask, char Shift, char Data);
char M128_TWI_fpt(char LSByte, char Mask, char Shift, char Data);
char M128_TWI_fgt(char LSByte, char Mask, char Shift, void *Data_p);
char M128_TWI_put(char LSbyte, char Bytes, void *Data_p);
char M128_TWI_get(char LSbyte, char Bytes, void *Data_p);
char M128_TWI_isr(char number, void (*function)(void));

char M128_UART_set(char LSByte, char Mask, char Shift, char Data);
char M128_UART_fpt(char LSByte, char Mask, char Shift, char Data);
char M128_UART_fgt(char LSByte, char Mask, char Shift, void *Data_p);
char M128_UART_put(char LSbyte, char Bytes, void *Data_p);
char M128_UART_get(char LSbyte, char Bytes, void *Data_p);
char M128_UART_isr(char number, void (*function)(void));

/*-- hmi section -------------------------------------------------------------*/
// HMI macro
#define HMI_TYPE_I8   0
#define HMI_TYPE_I16  1
#define HMI_TYPE_I32  2
#define HMI_TYPE_I64  3
#define HMI_TYPE_UI8  4
#define HMI_TYPE_UI16 5
#define HMI_TYPE_UI32 6
#define HMI_TYPE_UI64 7
#define HMI_TYPE_F32  8
#define HMI_TYPE_F64  9

// HMI declaration
char HMI_put_array(int Bytes, char Type, void *Data_p);
char HMI_get_array(int Bytes, void *Data_p);
char HMI_put_struct(char *FormatString, int Bytes, void *Data_p);
char HMI_get_struct(char *FormatString, int Bytes, void *Data_p);

char HMI_snput_array(int Bytes, char Type, void *Data_p);
char HMI_snget_array(int Bytes, void *Data_p);
char HMI_snput_struct(char *FormatString, int Bytes, void *Data_p);
char HMI_snget_struct(char *FormatString, int Bytes, void *Data_p);

// Old declaration, plz use new declaration upper.
char M128_HMI_put(int Bytes, char Type, void *Data_p);
char M128_HMI_get(int Bytes, void *Data_p);
char M128_HMI_FORM_put(char *FormatString, int Bytes, void *Data_p);
char M128_HMI_FORM_get(char *FormatString, int Bytes, void *Data_p);

char M128_HMI_snput(int Bytes, char Type, void *Data_p);
char M128_HMI_snget(int Bytes, void *Data_p);
char M128_HMI_FORM_snput(char *FormatString, int Bytes, void *Data_p);
char M128_HMI_FORM_snget(char *FormatString, int Bytes, void *Data_p);

/*-- 7s00 section ------------------------------------------------------------*/
char ASA_7S00_set(char ASA_ID, char LSByte, char Mask, char shift, char Data);
char ASA_7S00_put(char ASA_ID, char LSByte, char Bytes, void *Data_p);
char ASA_7S00_get(char ASA_ID, char LSByte, char Bytes, void *Data_p);

/*-- kb00 section ------------------------------------------------------------*/
char ASA_KB00_set(char ASA_ID, char LSByte, char Mask, char shift, char Data);
char ASA_KB00_get(char ASA_ID, char LSByte, char Bytes, void *Data_p);

/*-- stp00 section -----------------------------------------------------------*/
char ASA_STP00_put(char ASA_ID, char LSByte, char Bytes, void *Data_p);


#endif // ASA_LIB_H

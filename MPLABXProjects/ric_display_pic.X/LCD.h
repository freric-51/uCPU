/**
 * @file LCD.h
 * @brief prototipos para LCD.c
 */

#ifndef _LCD_H_
#define _LCD_H_

#include <xc.h>
#include <stdint.h> /** @brief For uint8_t definition */
#include <stdbool.h> /** @brief For true/false definition */

#define _XTAL_FREQ 4000000
#define LCD_EN_Delay 500

#define LCD_RS PORTAbits.RA0 /** @brief  LCD RS (04) PIC(17)
                            *            H-data input
                            *            L-instruction input
                            **/
#define LCD_EN PORTAbits.RA1 /** @brief PIC(18) LCD Enable (6)  */
#define LCD_RW PORTAbits.RA2 /** @brief PIC(01) LCD RW     (5)  */

// #define LED_T0 PORTAbits.RA3 /** @brief PIC(02) Indicador Tarefa 0 */
// #define LED_T1 PORTBbits.RB3 /** @brief PIC(09) Indicador Tarefa 1 */

#define LCD_D4 PORTBbits.RB4 /** @brief PIC(10) LCD Data 4 (11) */
#define LCD_D5 PORTBbits.RB5 /** @brief PIC(11) LCD Data 5 (12) */
#define LCD_D6 PORTBbits.RB6 /** @brief PIC(12) LCD Data 6 (13) */
#define LCD_D7 PORTBbits.RB7 /** @brief PIC(13) LCD Data 7 (14) */

/**
 * LCD area reservada para comandos especiais
 * jump 00 ~ 1F
 * jump 80 ~ 9F
 * jump A6 ~ DA
 * jump F9 ~ xx
 */

#define LCD_cmd_Clear 0x01 /** @brief LCD_Clear() */
#define LCD_cmd_R1    0x02 /** @brief LCD_Set_Cursor(0x01, 0x00) */
#define LCD_cmd_R2    0x03 /** @brief LCD_Set_Cursor(0x02, 0x00) */
#define LCD_cmd_R3    0x04 /** @brief LCD_Set_Cursor(0x03, 0x00) */
#define LCD_cmd_R4    0x05 /** @brief LCD_Set_Cursor(0x04, 0x00) */
#define LCD_cmd_SL    0x06 /** @brief LCD_Shift_Left */
#define LCD_cmd_SR    0x07 /** @brief LCD_Shift_Right */

#define LCD_cmd_2R    0x08 /** @brief LCD_Shift_Right */
#define LCD_cmd_4R    0x09 /** @brief LCD_Shift_Right */
#define LCD_cmd_16C    0x0A /** @brief LCD_Shift_Right */
#define LCD_cmd_20C    0x0B /** @brief LCD_Shift_Right */

#define LCD_Cursor_off 0x0C /** cursor off */
#define LCD_Cursor_on  0x0E /** cursor on */
#define LCD_Cursor_blk 0x0F /** blinking */

#define LCD_cmd_Col00 0x10 /** @brief LCD_Set_Cursor to nn Col */
#define LCD_cmd_Col01 0x11
#define LCD_cmd_Col02 0x12
#define LCD_cmd_Col03 0x13
#define LCD_cmd_Col04 0x14
#define LCD_cmd_Col05 0x15
#define LCD_cmd_Col06 0x16
#define LCD_cmd_Col07 0x17
#define LCD_cmd_Col08 0x18
#define LCD_cmd_Col09 0x19
#define LCD_cmd_Col10 0x1A
#define LCD_cmd_Col11 0x1B
#define LCD_cmd_Col12 0x1C
#define LCD_cmd_Col13 0x1D
#define LCD_cmd_Col14 0x1E
#define LCD_cmd_Col15 0x1F /** @brief LCD_Set_Cursor to nn Col */

//==========================================
//-----[ Prototypes For All Functions ]-----

void LCD_Init(void); /** @brief Initialize The LCD For 4-Bit Interface */
void LCD_Clear(void); /** @brief Clear The LCD Display */
void LCD_SL(void); /** @brief Shift The Entire Display To The Left */
void LCD_SR(void); /** @brief Shift The Entire Display To The Right */

void LCD_CMD_Nible(uint8_t); /** @brief Send Command To LCD */
void LCD_DATA(uint8_t); /** @brief Send 4-Bit Data To LCD */
void LCD_Set_Cursor(uint8_t, uint8_t); /** @brief Set Cursor Position */
void LCD_Write_Char(char); /** @brief Write Character To LCD At Current Position */
void LCD_Insere_String(char*); /** @brief Write A String To LCD */

char LCD_Remove_Char(void);
void LCD_Insere_Char(char);

uint8_t LCD_Has_Empty(void);
void LCD_MaqEstado(void);
void LCD_MaqEstadoLiberar(void);
//------------------------------------------

#endif //_LCD_H_
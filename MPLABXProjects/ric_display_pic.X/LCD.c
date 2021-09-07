/**
 * @file LCD.c
 *
 */

#include <pic16f627a.h>
#include "LCD.h"

/**
 * Alphanumeric LCD 16x2 or 16x4
 * 4 bits of bus data/command
 */
#define M_LCD_SIZE 0x4F

char M_LCD [M_LCD_SIZE];
uint8_t M_LCD_insere = 0;
uint8_t M_LCD_remove = 0;
uint8_t ME_Running;
uint8_t LCD_Current_Row = 0x01;
uint8_t LCD_Next_Col = 0x00;

uint8_t LCD_Number_Col = 0x0F;
uint8_t LCD_Number_Row = 0x01;

/**
 * @fn LCD_DATA
 * @param[in] Data
 */
void LCD_DATA(uint8_t Data) {
    LCD_D4 = (Data & 0x01) ? 1 : 0;
    LCD_D5 = (Data & 0x02) ? 1 : 0;
    LCD_D6 = (Data & 0x04) ? 1 : 0;
    LCD_D7 = (Data & 0x08) ? 1 : 0;
}

/**
 * @fn LCD_CMD_Nible
 * @param[in] CMD
 */
void LCD_CMD_Nible(uint8_t CMD) {
    LCD_RW = 0; // write info
    LCD_RS = 0; // Select Command Register
    LCD_DATA(CMD); // Move The Command Data To LCD
    LCD_EN = 1; // Send The EN Clock Signal
    _delay(LCD_EN_Delay);
    LCD_EN = 0;
}

/**
 * @fn LCD_CMD_Word
 * @param[in] CMD
 */
void LCD_CMD_Word(uint8_t CMD) {
    uint8_t Low4, High4;
    High4 = CMD >> 4;
    Low4 = CMD & 0x0F;
    LCD_CMD_Nible(High4);
    LCD_CMD_Nible(Low4);
}

/**
 * @fn LCD_Clear
 *
 */
void LCD_Clear(void) {
    LCD_CMD_Nible(0);
    LCD_CMD_Nible(1);
}

/**
 * @fn LCD_Set_Cursor
 * @brief 2 or 4 rows and 16 cols
 * @param[in] row
 * @param[in] col
 */
void LCD_Set_Cursor(uint8_t row, uint8_t col) {
    uint8_t Temp, Low4, High4;
    if (row == 1)
        Temp = 0x80 + col;

    if (row == 2)
        Temp = 0xC0 + col;

    if (row == 3)
        Temp = 0x80 + 0x10 + col;

    if (row == 4)
        Temp = 0xC0 + 0x10 + col;

    LCD_Current_Row = row;
    LCD_Next_Col = col + 1;
    High4 = Temp >> 4;
    Low4 = Temp & 0x0F;
    LCD_CMD_Nible(High4);
    LCD_CMD_Nible(Low4);
}

/**
 * @fn LCD_Init
 */
void LCD_Init(void) {
    ME_Running = 1; /** LCD_MaqEstado n&atilde;o habilitada */

    // IO Pin Configurations in ConfigureCPU()
    LCD_RW = 0; // write data/command
    LCD_EN = 0;

    _delay(30000); //Time before anything by Freitas
    // The Init. Procedure As Described In The Datasheet
    LCD_DATA(0x00); /** 0F = display on/off control */
    _delay(30000);
    _delay(LCD_EN_Delay);

    LCD_CMD_Nible(0x03); /** 3F = set 8 bits 1/3 */
    _delay(5000);

    LCD_CMD_Nible(0x03); /** 3F = set 8 bits2/3 */
    _delay(150);

    LCD_CMD_Nible(0x03); /** 3F = set 8 bits 3/3 */

    LCD_CMD_Nible(0x02); /** 2F = set 4 bits */

    LCD_CMD_Nible(0x02); /** 28 : 2 line 5x7 dot*/
    LCD_CMD_Nible(0x08);

    /**
     * 0C = cursor off
     * 0E = cursor on
     * 0F = blinking
     */
    LCD_CMD_Nible(0x00);
    LCD_CMD_Nible(0x0C);

    LCD_CMD_Nible(0x00); /** 06 = Entry Mode */
    LCD_CMD_Nible(0x06);
    ME_Running = 0; /** LCD_MaqEstado liberada */
}

/**
 * @fn LCD_Write_Char
 * @param[in] Data
 */
void LCD_Write_Char(char Data) {
    uint8_t Low4, High4;

    Low4 = Data & 0x0F;
    //High4 = Data & 0xF0;
    //High4 = High4 >> 4;
    High4 = Data >> 4;

    LCD_RS = 1;

    LCD_DATA(High4);
    LCD_EN = 1;
    _delay(LCD_EN_Delay);
    LCD_EN = 0;
    _delay(LCD_EN_Delay);

    LCD_DATA(Low4);
    LCD_EN = 1;
    _delay(LCD_EN_Delay);
    LCD_EN = 0;
    _delay(LCD_EN_Delay);
    ++LCD_Next_Col;
}

/**
 * @fn LCD_SL
 */
void LCD_SL(void) {
    LCD_CMD_Nible(0x01);
    LCD_CMD_Nible(0x08);
    --LCD_Next_Col;
}

/**
 * @fn LCD_SR
 */
void LCD_SR(void) {
    LCD_CMD_Nible(0x01);
    LCD_CMD_Nible(0x0C);
    ++LCD_Next_Col;
}

/**
 * @fn LCD_Remove_Char
 * @param[out] char
 */
char LCD_Remove_Char(void) {
    M_LCD_remove++;
    if (M_LCD_remove == M_LCD_SIZE) {
        M_LCD_remove = 0;
    }
    return M_LCD[M_LCD_remove];
}

/**
 * @fn LCD_Insere_Char
 * @param[in] dado
 */
void LCD_Insere_Char(char dado) {
    M_LCD_insere++;
    if (M_LCD_insere == M_LCD_SIZE) {
        M_LCD_insere = 0;
    }
    M_LCD[M_LCD_insere] = dado;
}

/**
 * @fn LCD_Insere_String
 * @param[in] dado
 */
void LCD_Insere_String(char *dado) {
    int i;
    for (i = 0; dado[i] != '\0'; i++) {
        LCD_Insere_Char(dado[i]);
    }
}

/**
 * @fn LCD_Has_Empty
 * @param[out] yes no
 */
uint8_t LCD_Has_Empty(void) {
    return (M_LCD_remove == M_LCD_insere) ? 1 : 0;
}

/**
 * @fn LCD_MaqEstadoLiberar
 * @brief ME is able to run
 */
void LCD_MaqEstadoLiberar(void) {
    ME_Running = false; /** LCD_MaqEstado liberada */
}

/**
 * @fn LCD_MaqEstado
 */
void LCD_MaqEstado(void) {
    uint8_t Col;

    if (ME_Running) {
        return;
    }

    if (LCD_Has_Empty()) {
        ME_Running = false;
        return;
    } else {
        ME_Running = true;
    }

    char c = LCD_Remove_Char();
    switch (c) {
        case LCD_cmd_Clear:
            LCD_Clear();
            break;
        case LCD_cmd_R1:
            LCD_Set_Cursor(0x01, 0x00);
            break;
        case LCD_cmd_R2:
            LCD_Set_Cursor(0x02, 0x00);
            break;
        case LCD_cmd_R3:
            LCD_Set_Cursor(0x03, 0x00);
            break;
        case LCD_cmd_R4:
            LCD_Set_Cursor(0x04, 0x00);
            break;
        case LCD_cmd_SL:
            LCD_SL();
            break;
        case LCD_cmd_SR:
            LCD_SR();
            break;

        case LCD_Cursor_off:
            LCD_CMD_Word(0x0C);
            break;
        case LCD_Cursor_on:
            LCD_CMD_Word(0x0E);
            break;
        case LCD_Cursor_blk:
            LCD_CMD_Word(0x0F);
            break;

        case LCD_cmd_2R:
            LCD_Number_Row = 0x01;
            break;
        case LCD_cmd_4R:
            LCD_Number_Row = 0x03;
            break;
        case LCD_cmd_16C:
            LCD_Number_Col = 0x0F;
            break;
        case LCD_cmd_20C:
            LCD_Number_Col = 0x13;
            break;

        case LCD_cmd_Col00:
        case LCD_cmd_Col01:
        case LCD_cmd_Col02:
        case LCD_cmd_Col03:
        case LCD_cmd_Col04:
        case LCD_cmd_Col05:
        case LCD_cmd_Col06:
        case LCD_cmd_Col07:
        case LCD_cmd_Col08:
        case LCD_cmd_Col09:
        case LCD_cmd_Col10:
        case LCD_cmd_Col11:
        case LCD_cmd_Col12:
        case LCD_cmd_Col13:
        case LCD_cmd_Col14:
        case LCD_cmd_Col15:
            Col = (c & 0x0F);
            LCD_Set_Cursor(LCD_Current_Row, Col);
            break;

        default:
            LCD_Write_Char(c);
            if (LCD_Next_Col > 0x10) {
                if (LCD_Current_Row < 4) {
                    ++LCD_Current_Row;
                } else {
                    LCD_Current_Row = 1;
                }
                LCD_Set_Cursor(LCD_Current_Row, 0x00);
            }
            break;
    }

    ME_Running = false;
}

/**
 * @file main.c
 * @author Ricardo de Freitas
 * @date 2021/08/17
 * @mainpage Freitas & Eletr&ocirc;nica
 * @brief Este projeto &eacute; composto de um LCD comandado pela comunica&ccedil;&atilde;o serial.
 *
 */

#include "config.h"
#include <xc.h>
#include <pic16f627a.h>
#include <stdint.h>        /** For uint8_t definition */
#include <stdbool.h>       /** For true/false definition */
#include "LCD.h"

#define T0_cfg 0xE0
#define T1_cfg 0xA1AD
/** T1_cfg = 0xC1AD */
// bool LED_RAM_T0;
// bool LED_RAM_T1;
char LCD_Test_Char;

/**
 * prototipos
 */
void ConfigureCPU(void); /** Handles clock switching/osc initialization */

/**
 * @fn main
 * @brief initialize CPU and stay running in loop
 *
 */
void main(void) {
    ConfigureCPU();
    LCD_Init();
    LCD_Clear();

    LCD_Insere_Char(LCD_cmd_R1);
    LCD_Insere_Char(LCD_cmd_2R);
    LCD_Insere_Char(LCD_cmd_16C);

    /**
     * //                 1234567890123456
     * LCD_Insere_String("Ricardo de Frei-\0");
     * LCD_Insere_Char(LCD_cmd_R2);
     * LCD_Insere_String("tas          v02\0");
     */

    //                 0---------1---------2---------3---------4---------5---------6123
    //LCD_Insere_String("Ricardo de Freitas 2021 08 13 entrada de dados para 4 linhas   .\0");


    /** end of int with GIE */
    LCD_MaqEstadoLiberar();
    INTCONbits.GIE = 1;

    while (1) {
        NOP();
        NOP();
        NOP();
    }
    return;
}

/**
 * @fn ConfigureCPU
 */
void ConfigureCPU(void) {
    /** To Initialize User Ports/Peripherals/Project here */
    CMCONbits.CM = 0b00000111; /** Comparators Off 111 */
    // CMCONbits.CIS = 0; /** Comparator Input Switch bit */

    VRCONbits.VREN = 0; /** VREF circuit powered down, no IDD drain */
    VRCONbits.VROE = 0; /** VREF is disconnected from RA2 pin */

    CCP1CONbits.CCP1M = 0b00000000;

    /* 4Mhz */
    PCONbits.OSCF = 1;
    // timer 0
    TMR0 = T0_cfg;
    OPTION_REGbits.T0CS = 0; /** 0=fosc/4 */

    OPTION_REGbits.PSA = 0; /** prescaler to T0=0 */
    /**
     * PS
     * 000  1:2
     * 001  1:4
     * 010  1:8
     * 011  1:16
     * 100  1:32
     * 101  1:64
     * 110  1:128
     * 111  1:256
     */
    OPTION_REGbits.PS2 = 0;
    OPTION_REGbits.PS1 = 0;
    OPTION_REGbits.PS0 = 1;

    INTCONbits.TMR0IE = 1;
    INTCONbits.TMR0IF = 0;

    // timer 1
    T1CON = 0x00; /** desabilita temporizadores */
    TMR1 = T1_cfg;
    T1CONbits.T1CKPS0 = 1; /** 11 = 1:8 Prescale value */
    T1CONbits.T1CKPS1 = 1;
    T1CONbits.TMR1CS = 0; /** 0 = Internal clock (FOSC/4) */
    T1CONbits.T1OSCEN = 0; /** 0=not external clk */
    T1CONbits.nT1SYNC = 1;
    T1CONbits.TMR1ON = 1; /** 1 = Enables Timer1 */
    PIR1bits.TMR1IF = 0;

    /** timer 2 */
    T2CON = 0x00;
    T2CONbits.TMR2ON = 0;
    PIR1bits.TMR2IF = 0;

    /** T1 e T2 config */
    PIE1bits.CCP1IE = 0;
    PIE1bits.CMIE = 0; /** comparadores off */
    PIE1bits.EEIE = 0;
    PIE1bits.RCIE = 0;
    PIE1bits.TMR1IE = 1; /** 1 para liberar T1 */
    PIE1bits.TMR2IE = 0;
    PIE1bits.TXIE = 1;

    // timer 0
    INTCONbits.T0IE = 1;
    INTCONbits.T0IF = 1;

    INTCONbits.PEIE = 1; /** enable periferical int */
    INTCONbits.RBIE = 0;
    INTCONbits.INTE = 0; /** OPTION_REGbits.INTEDG */
    INTCONbits.INTF = 0;

    /** TRIS - define o sentido das portas
     * set all ports as input TRISn = 0XFF;
     * PORTA = 0x00;
     * PORTB = 0x00;
     */
    TRISA = 0x00;
    TRISB = 0x00;
    TRISBbits.TRISB1 = 1; /** RX=input RB1 */
    OPTION_REGbits.nRBPU = 1; //1 = PORTB pull-ups are disabled
}

/**
 * @fn __interrupt() TrataInt
 *
 */
void __interrupt() TrataInt(void) {
    if (INTCONbits.TMR0IE && INTCONbits.TMR0IF) {
        /**
         * @brief executa a maquina de estado
         * que desvazia a pilha de mensagens
         */
        TMR0 = T0_cfg;
        // LED_RAM_T0 = !LED_RAM_T0;
        // LED_T0 = LED_RAM_T0;
        INTCONbits.TMR0IF = 0; /** overflow FF > 00 */
        LCD_MaqEstado();

    } else if (PIR1bits.RCIF) {
        // PIR1bits.TXIF = false;
        PIR1bits.RCIF = 0;
        // LED_RAM_T1 = !LED_RAM_T1;
        // LED_T1 = LED_RAM_T1;

    } else if (PIE1bits.TMR1IE && PIR1bits.TMR1IF) {
        /**
         * @brief mantem o LCD trocando caracteres
         */
        TMR1 = T1_cfg;
        PIR1bits.TMR1IF = 0; //overflow FF > 00
        LCD_Test_Char++;
        /**
         * @brief jump unespected characters
         */
        if (LCD_Test_Char < 0x20) /** jump 00 ~ 1F */
            LCD_Test_Char = 0x20;

        if (LCD_Test_Char == 0x80) /** jump 80 ~ 9F */
            LCD_Test_Char = 0xA0;

        if (LCD_Test_Char == 0xA6) /** jump A6 ~ DA */
            LCD_Test_Char = 0xDB;

        if (LCD_Test_Char == 0xF9) /** jump F9 ~ xx */
            LCD_Test_Char = 0x20;

        LCD_Insere_Char(LCD_Test_Char);

    }

}

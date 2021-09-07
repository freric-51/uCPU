
/** @file  config.h
 * @brief PIC16F627A Configuration Bit Settings\n
 * 'C' source line config statements\n
 * pragma CONFIG
 */

#pragma config FOSC = INTOSCIO  // @brief Oscillator Selection bits (INTOSC oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // @brief Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = ON       // @brief Power-up Timer Enable bit (PWRT enabled)
#pragma config MCLRE = ON       // @brief RA5/MCLR/VPP Pin Function Select bit (RA5/MCLR/VPP pin function is MCLR)
#pragma config BOREN = OFF      // @brief Brown-out Detect Enable bit (BOD disabled)
#pragma config LVP = OFF        // @brief Low-Voltage Programming Enable bit (RB4/PGM pin has digital I/O function, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // @brief Data EE Memory Code Protection bit (Data memory code protection off)
#pragma config CP = OFF         // @brief Flash Program Memory Code Protection bit (Code protection off)

//  @brief #pragma config statements should precede project file includes.\n
// Use project enums instead of #define for ON and OFF.

#include <xc.h>

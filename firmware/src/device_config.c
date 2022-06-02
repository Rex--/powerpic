/** @file device_config.c
 * This file contains device configuration information.
 * Namely, the device configuration words.
*/



// Configuration Word 1

/** External Clock mode - External clock not enabled. **/
#pragma config FEXTOSC = OFF

/** Power-up Oscillator mode - High freq internal oscillator at 1MHz **/
#pragma config RSTOSC = HFINT1

/** Clock out Enable **/
#pragma config CLKOUTEN = OFF

/** VBAT Pin Enable **/
#pragma config VBATEN = OFF

/** LCD Charge Pump Enable Bit. **/
#pragma config LCDPEN = ON

/** Clock Switch Enable **/
#pragma config CSWEN = ON

/** Fail-Safe Clock Monitor Enable **/
#pragma config FCMEN = OFF



// Configuration Word 2

/** Master Clear Enable **/
#pragma config MCLRE = ON

/** Power-up Timer Selection **/
#pragma config PWRTE = PWRT_64

/** Low-Power BOR Enable **/
#pragma config LPBOREN = OFF

/** Brown-out Reset Enable **/
#pragma config BOREN = OFF

/** Brown-out Reset Voltage Selection **/
#pragma config BORV = LO

/** Zero-cross detect Disable **/
#pragma config ZCD = OFF

/** Peripherial Pin Select one-way control **/
#pragma config PPS1WAY = OFF

/** Stack Overflow/Underflow Reset Enable **/
#pragma config STVREN = OFF



// Configuration Word 3

/** WDT Period Select **/
#pragma config WDTCPS = WDTCPS_0

/** WDT Operating Mode **/
#pragma config WDTE = OFF

/** WDT Window Select **/
#pragma config WDTCWS = WDTCWS_0

/** WDT Input Clock Selector **/
#pragma config WDTCCS = SOSC



// Configuration Word 4

/** Boot Block Size Selection **/
#pragma config BBSIZE = 512

/** Boot Block Enable **/
#pragma config BBEN = OFF

/** SAF Enable **/
#pragma config SAFEN = OFF

/** Application Block Write Protection **/
#pragma config WRTAPP = OFF

/** Boot Block Write Protection **/
#pragma config WRTB = OFF

/** Configuration Register Write Protection **/
#pragma config WRTC = OFF

/** Data EEPROM Write Protection **/
#pragma config WRTD = OFF

/** Storage Area Flash Write Protection **/
#pragma config WRTSAF = OFF

/** Low Voltage Programming Enable **/
#pragma config LVP = ON



// Configuration Word 5

/** UserNVM Program Memory Code Protection **/
#pragma config CP = OFF


/* EOF */

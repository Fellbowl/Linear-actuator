/**
   NCO1 Generated Driver File
 
   @Company
     Microchip Technology Inc.
 
   @File Name
     nco1.c
 
   @Summary
     This is the generated driver implementation file for the NCO1 driver using PIC10 / PIC12 / PIC16 / PIC18 MCUs
 
   @Description
     This source file provides implementations for driver APIs for NCO1.
     Generation Information :
         Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.8
         Device            :  PIC16F18426
         Driver Version    :  2.11
     The generated drivers are tested against the following:
         Compiler          :  XC8 2.36 and above or later
         MPLAB             :  MPLAB X 6.00
 */ 

 /*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
*/
 
 /**
   Section: Included Files
 */

#include <xc.h>
#include "nco1.h"

/**
  Section: NCO Module APIs
*/

void NCO1_Initialize (void)
{
    // Set the NCO to the options selected in the GUI
    // EN disabled; POL active_hi; PFM FDC_mode; 
    NCO1CON = 0x00;
    // CKS MFINTOSC_500KHz; PWS 2_clk; 
    NCO1CLK = 0x23;
    // 
    NCO1ACCU = 0x00;
    // 
    NCO1ACCH = 0x00;
    // 
    NCO1ACCL = 0x00;
    // 
    NCO1INCU = 0x01;
    // 
    NCO1INCH = 0x89;
    // 
    NCO1INCL = 0x37;

    // Enable the NCO module
    NCO1CONbits.EN = 0;
   
    // Clearing IF flag before enabling the interrupt.
    PIR7bits.NCO1IF = 0;
    // Enabling NCO1 interrupt.
    PIE7bits.NCO1IE = 1;
}

void NCO1_ISR(void)
{
    // Clear the NCO1 interrupt flag
    PIR7bits.NCO1IF = 0;
}

// Asumiendo NCO1CLK = MFINTOSC (500 kHz) y NCO1CON en FDC
void NCO1_SetFrequency_FDC(uint32_t freq_hz)
{
    // usamos 2^21 porque en FDC la salida togglea cada overflow (2 overflows = 1 ciclo)
    uint32_t inc = (uint32_t)((((uint64_t)freq_hz) << 21) / 500000U);

    NCO1INCL = (uint8_t)(inc & 0xFF);
    NCO1INCH = (uint8_t)((inc >> 8) & 0xFF);
    NCO1INCU = (uint8_t)((inc >> 16) & 0x0F);
}

/**
 End of File
*/


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
#include <stdint.h>

static uint32_t nco_current_inc = 0;
static uint32_t nco_ramp_target = 0;
static uint32_t nco_ramp_step = 1000; // Adjust this value to change ramp speed
static bool nco_ramp_active = false;
/**
  Section: NCO Module APIs
*/

void NCO1_Initialize (void)
{
    // Set the NCO to the options selected in the GUI
    // EN disabled; POL active_hi; PFM PFM_mode; 
    NCO1CON = 0x01;
    // CKS MFINTOSC_500KHz; PWS 1_clk; 
    NCO1CLK = 0x03;
    // 
    NCO1ACCU = 0x00;
    // 
    NCO1ACCH = 0x00;
    // 
    NCO1ACCL = 0x00;
    // 
    NCO1INCU = 0x00;
    // 
    NCO1INCH = 0xC4;
    // 
    NCO1INCL = 0x9C;

    nco_current_inc = ((uint32_t)NCO1INCU << 16) | ((uint32_t)NCO1INCH << 8) | NCO1INCL;

    // Enable the NCO module
    NCO1CONbits.EN = 0;
   
}

bool NCO1_GetOutputStatus(void)
{
    // Return output status on accumulator over flow
    return (NCO1CONbits.OUT);
}

void NCO1_Start(void)
{
    // Start the NCO
    NCO1CONbits.EN = 1;
}

void NCO1_Stop(void)
{
    // Stop the NCO
    NCO1CONbits.EN = 0;
}

bool NCO1_GetStatus(void)
{
    // Return the status of the NCO
    return (NCO1CONbits.EN);
}

void SetFrequency(uint16_t velocity)
{
    uint32_t freq;
    uint32_t increment;
    const uint32_t L = 8, N = 200, MS = 8;

    // Calculate desired frequency in Hz from velocity
    // frequency = velocity * N * MS / L
    freq = (uint32_t)(2UL * velocity * N * MS / L);

    // Compute target frequency increment
    increment = (uint32_t)(((uint64_t)freq << 24) / NCO1_CLK_HZ);

    // Start ramp from 5 kHz up to target (puede ajustarse)
    uint32_t start_inc = (uint32_t)(((uint64_t)5000 << 24) / NCO1_CLK_HZ);
    NCO1INCU = (uint8_t)((start_inc >> 16) & 0xFF);
    NCO1INCH = (uint8_t)((start_inc >> 8) & 0xFF);
    NCO1INCL = (uint8_t)(start_inc & 0xFF);
    nco_current_inc = start_inc;
    // Enable NCO1
    NCO1CONbits.EN = 1;

    nco_ramp_target = increment;
    nco_ramp_active = true;
}

void NCO1_RampTick(void)
{
    if(!nco_ramp_active) return;

    if(nco_current_inc == nco_ramp_target) { 
      nco_ramp_active = false; 
      return; 
    }
    else if(nco_current_inc < nco_ramp_target)
    {
        uint32_t next = nco_current_inc + nco_ramp_step;
        if(next > nco_ramp_target) next = nco_ramp_target;
        NCO1CONbits.EN = 0;
        NCO1INCU = (uint8_t)((next >> 16) & 0xFF);
        NCO1INCH = (uint8_t)((next >> 8) & 0xFF);
        NCO1INCL = (uint8_t)(next & 0xFF);
        NCO1CONbits.EN = 1;
        nco_current_inc = next;
    }
    else
    {
        uint32_t next = (nco_current_inc > nco_ramp_step) ? nco_current_inc - nco_ramp_step : 0;
        if(next < nco_ramp_target) next = nco_ramp_target;
        NCO1CONbits.EN = 0;
        NCO1INCU = (uint8_t)((next >> 16) & 0xFF);
        NCO1INCH = (uint8_t)((next >> 8) & 0xFF);
        NCO1INCL = (uint8_t)(next & 0xFF);
        NCO1CONbits.EN = 1;
        nco_current_inc = next;
    }
}
/**
 End of File
*/


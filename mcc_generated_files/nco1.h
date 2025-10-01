/**
   NCO1 Generated Driver API Header File
 
   @Company
     Microchip Technology Inc. 

   @File Name
    nco1.h

   @Summary
     This is the generated header file for the NCO1 driver using PIC10 / PIC12 / PIC16 / PIC18 MCUs
 
   @Description
     This header file provides APIs for driver for NCO1.
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

#ifndef NCO1_H
 #define NCO1_H
 
 /**
   Section: Included Files
 */

#include <xc.h>
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif

/**
  Section: NCO1 Module APIs
*/

/**
  @Summary
    Initializes the NCO1_Initialize

  @Description
    This routine initializes the NCO1_Initialize
    This routine must be called before any other NCO1 routine is called.
    This routine should only be called once during system initialization.

  @Preconditions
    None

  @Param
    None

  @Returns
    None

  @Comment

 @Example
    <code>
    NCO1_Initialize();
    </code>
 */
void NCO1_Initialize(void);

/**
  @Summary
    Implements ISR.

  @Description
    This routine is used to implement the ISR for the interrupt-driven
    implementations.

  @Returns
    None

  @Param
    None
*/
void NCO1_ISR(void);

void NCO1_SetFrequency_FDC(uint32_t);

/**
  @Summary
    Configures the NCO1 output frequency in Fixed Duty Cycle (FDC) mode.

  @Description
    This routine calculates the increment value (INC) required to generate 
    the desired output frequency in FDC mode with a 500 kHz NCO clock.
    It then writes the calculated value into the NCO1INCU:INCH:INCL registers.
    In FDC mode the NCO toggles the output on every accumulator overflow,
    therefore the increment is calculated using 2^21 as the divisor.

  @Preconditions
    - NCO1 module must be enabled and configured in Fixed Duty Cycle mode.
    - NCO1 clock source must be set to 500 kHz (e.g., MFINTOSC).
    - NCO1 output pin must be enabled if the signal is required externally.

  @Param
    freq_hz - Desired output frequency in Hertz (uint32_t).

  @Returns
    None

  @Example
    // Set NCO1 output to 10 kHz in FDC mode
    NCO1_SetFrequency_FDC(10000);
*/

int MS = 8;

bool SetSpeed(float vel);
/**
  @Summary
    Converts linear speed to NCO frequency and programs NCO1 in FDC mode.

  @Description
    This routine computes the required output frequency from a linear velocity
    using the relation Hz = vel * N * MS / L with constants:
      - N  = 200
      - MS = 8
      - L  = 0.008
    (This simplifies to Hz = vel * 200000)
    The computed frequency is validated against the acceptable range used by
    the driver (200 Hz .. 25000 Hz). If valid, NCO1_SetFrequency_FDC is called
    to write the increment registers.

  @Preconditions
    - NCO1_Initialize() must have been called.
    - NCO1 must be configured in Fixed Duty Cycle (FDC) mode and use a 500 kHz clock.

  @Param
    vel - Linear speed in meters per second (float).

  @Returns
    true  - if the calculated frequency is within the acceptable range and was applied.
    false - if the frequency is out of range and the NCO was not updated.

  @Example
    <code>
    // set NCO output for a wheel speed of 0.75 m/s
    if (SetSpeed(0.75f)) {
        // success
    } else {
        // out of range
    }
    </code>
*/




#ifdef __cplusplus  // Provide C++ Compatibility

    }

#endif

#endif  //NCO1_H
/**
 End of File
*/



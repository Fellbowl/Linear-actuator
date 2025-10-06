/**
  Generated Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This is the main file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.8
        Device            :  PIC16F18426
        Driver Version    :  2.00
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

#include "mcc_generated_files/mcc.h"
#include "mcc_generated_files/nco1.h"
#include "mcc_generated_files/pin_manager.h"

//TMR2
bool PulseEnable = false;
uint32_t PulseCount = 0;
int32_t TrueCount = 0;

uint16_t velocity = 0;

//NCO1
uint32_t nco_current_freq = 0;
uint32_t nco_ramp_target = 0;

//GoTo
bool GoToFlag = false;


/*
                         Main application
 */
void SetVelocity(uint16_t);
void GoHome(void);
typedef enum {
    MOVE_IDLE,
    MOVE_RUNNING,
    MOVE_DONE
} MoveState;

MoveState moveState = MOVE_IDLE;
uint16_t ReqStep = 0;

bool GoTo(uint16_t);

void main(void)
{
    // initialize the device
    SYSTEM_Initialize();

    // When using interrupts, you need to set the Global and Peripheral Interrupt Enable bits
    // Use the following macros to:

    // Enable the Global Interrupts
    INTERRUPT_GlobalInterruptEnable();

    // Enable the Peripheral Interrupts
    INTERRUPT_PeripheralInterruptEnable();

    // Disable the Global Interrupts
    //INTERRUPT_GlobalInterruptDisable();

    // Disable the Peripheral Interrupts
    //INTERRUPT_PeripheralInterruptDisable();
    
    //GoHome();`

    while (1)
    {
        if (!GoToFlag)   // solo si estamos en movimiento
        {
            if (GoTo(1000)) {
                PulseEnable = false;
            }
        }

        // aquí puedes hacer otras tareas sin volver a llamar a GoTo()
    }
}

void SetVelocity(uint16_t velocity)
{
    
    // Calculate desired frequency in Hz from velocity
    // frequency = velocity * N * MS / L
    //nco_ramp_target = (uint32_t)(velocity * 400U); // Activate to recieve mm/s
    nco_ramp_target = velocity;
    nco_current_freq = 10000;
    SetFrequency(nco_current_freq);
}

void GoHome()
{
    DIR_SetHigh();//Reverse
    if(MIN_GetValue()){
        SetVelocity(15000);
        PulseEnable = true;
    } 
    else{
        PulseEnable = false;
        DIR_SetLow();
        TrueCount = 0;
    }
}

bool GoTo(uint16_t pos)
{
    ReqStep = pos * 20;

    if(ReqStep < TrueCount) {
        DIR_SetHigh();
    } else if(ReqStep > TrueCount) {
        DIR_SetLow();
    }

    if(ReqStep != TrueCount) {
        if(!GetStatus()) SetVelocity(15000);
        PulseEnable = true;
        return false;
    } else {
        PulseEnable = false;
        GoToFlag = true;    // <-- la ISR detecta esto
        return true;
    }
}

/*bool GoTo(uint16_t pos)
{
    ReqStep = pos * 20;

    switch(moveState)
    {
        case MOVE_IDLE:
            if(ReqStep != TrueCount)
            {
                if(ReqStep < TrueCount) {
                    DIR_SetHigh();
                } else {
                    DIR_SetLow();
                }
                SetVelocity(15000);   // Solo una vez al iniciar
                PulseEnable = true;
                moveState = MOVE_RUNNING;
            }
            break;

        case MOVE_RUNNING:
            if(ReqStep == TrueCount) {
                PulseEnable = false;
                moveState = MOVE_DONE;
            }
            break;

        case MOVE_DONE:
            moveState = MOVE_IDLE;   // reset para poder hacer otro GoTo
            GoToFlag = true;
            return true;             // señal de que llegó
    }

    return false;  // si todavía no ha llegado
}*/




/**
 End of File
*/
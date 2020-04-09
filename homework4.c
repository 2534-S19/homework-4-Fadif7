#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "homework4.h"
#include <stdio.h>

int main(void)
{
    char rChar;
    char *response = "\n\n\r2534 is a fun course curriculum\r\n\n";

    // TODO: Declare the variables that main uses to interact with your state machine.

    bool finished = false;

    // Stops the Watchdog timer.
    initBoard();
    // TODO: Declare a UART config struct as defined in uart.h.
    //       To begin, configure the UART for 9600 baud, 8-bit payload (LSB first), no parity, 1 stop bit.
    eUSCI_UART_ConfigV1 uartConfig =
    {
        EUSCI_A_UART_CLOCKSOURCE_SMCLK,
        19,
        8,
        0x55,
        EUSCI_A_UART_NO_PARITY,
        EUSCI_A_UART_LSB_FIRST,
        EUSCI_A_UART_ONE_STOP_BIT,
        EUSCI_A_UART_MODE,
        EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION,
        EUSCI_A_UART_8_BIT_LEN
    };

    // TODO: Make sure Tx AND Rx pins of EUSCI_A0 work for UART and not as regular GPIO pins.

    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P1, GPIO_PIN2,GPIO_PRIMARY_MODULE_FUNCTION); //Port 1, Pin2
    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P1, GPIO_PIN3,GPIO_PRIMARY_MODULE_FUNCTION); //Port 1, Pin3

    // TODO: Initialize EUSCI_A0
    UART_initModule(EUSCI_A0_BASE, &uartConfig);

    // TODO: Enable EUSCI_A0
    UART_enableModule(EUSCI_A0_BASE);

    while(1)
    {
        // TODO: Check the receive interrupt flag to see if a received character is available.
        //       Return 0xFF if no character is available.

        if (UART_getInterruptStatus(EUSCI_A0_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG)
                        == EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG)                               //Checks for received interruptes
            rChar = UART_receiveData(EUSCI_A0_BASE);                                          //recieveData for whatever interrupts and then stores them in rChar
        else
            rChar = 0xFF;                                                                     //else rChar is 0xFF which means that there is no characters

        // TODO: If an actual character was received, echo the character to the terminal AND use it to update the FSM.
        //       Check the transmit interrupt flag prior to transmitting the character.

        if(rChar != 0xFF)                                                                     //if something is being written
        {
            if (UART_getInterruptStatus(EUSCI_A0_BASE, EUSCI_A_UART_TRANSMIT_INTERRUPT_FLAG)
                        == EUSCI_A_UART_TRANSMIT_INTERRUPT_FLAG)                              //checks if something is being typed
            {
                UART_transmitData(EUSCI_A0_BASE, rChar);                                      //Sends data to terminal
                finished = charFSM(rChar);                                                    //Updates the FSM with whatever is written
            }
        }



        // TODO: If the FSM indicates a successful string entry, transmit the response string.
        //       Check the transmit interrupt flag prior to transmitting each character and moving on to the next one.
        //       Make sure to reset the success variable after transmission.

        if(finished)                                                                            //If finished
        {
            if (UART_getInterruptStatus (EUSCI_A0_BASE, EUSCI_A_UART_TRANSMIT_INTERRUPT_FLAG)   //Checks and transmits
                        == EUSCI_A_UART_TRANSMIT_INTERRUPT_FLAG)
            {
                int i;
                for(i = 0; response[i] !='\0';i++)                                              //makes the response into an array and then increments
                {
                    UART_transmitData(EUSCI_A0_BASE, response[i]);                              //transmits the response
                }
                finished = false;
            }
        }

    }
}

void initBoard()
{
    WDT_A_hold(WDT_A_BASE);
}

// TODO: FSM for detecting character sequence.
bool charFSM(char rChar)
{
    bool response = false;

    if(currentState == ECEX)        //State before anything is written
    {
        if(rChar == '2')            //if the number 2 is written
        {
            currentState = ECE2;    //if it is 2 then the currentState becomes ECE2  which is the next state
        }
    }
    else if (currentState == ECE2)  //ECE2 is the next state that occurs
    {
        if(rChar == '5')            //if the number 5 is written
        {
            currentState = ECE25;   //if it is 5 then the currentState becomes ECE25  which is the next state
        }
        else
            currentState = ECEX;    //if not then returns to ECEX
    }
    else if (currentState == ECE25) //ECE25 is the next state that occurs
    {
        if(rChar == '3')            //if the number 3 is written
        {
            currentState = ECE253;  //if it is 3 then the currentState becomes ECE253  which is the next state
        }
        else
            currentState = ECEX;    //if not then returns to ECEX
    }
    else if (currentState == ECE253)//ECE253 is the next state that occurs
    {
        if(rChar == '4')            //if the number 4 is written
        {
            currentState = ECEX;    //if it is 4 then the currentState becomes ECEX  which is the next state since it completed the sequence
            response = true;        //if the sequence is correct then the response is true and displays
        }
        else
            currentState = ECEX;    //otherwise it resets to ECEX
    }
    else if (currentState == ECEX)
    {
        currentState = ECEX;        //repeats back to the top of the FSM
    }
    return response;
}

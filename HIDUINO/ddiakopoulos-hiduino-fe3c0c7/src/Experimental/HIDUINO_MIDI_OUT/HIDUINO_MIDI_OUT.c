/***********************************************************************
 *  HIDUINO_MIDI_OUT Firmware v1.2
 *  by Dimitri Diakopoulos (http://www.dimitridiakopoulos.com)
 *  Music Technology: Interaction, Intelligence & Design - April 2011
 *  http://mtiid.calarts.edu
 *  http://www.dimitridiakopoulos.com
 *  Based on the LUFA MIDI Demo by Dean Camera 
 * 		- http://www.fourwalledcubicle.com/LUFA.php
 ***********************************************************************/

#include "HIDUINO_MIDI_OUT.h"

MIDI_EventPacket_t MIDI_FROM_ARDUINO; 

volatile uint8_t dCount = 0;
volatile uint8_t complete = 0; 

USB_ClassInfo_MIDI_Device_t MIDI_Interface =
	{
		.Config =
			{
				.StreamingInterfaceNumber = 1,

				.DataINEndpointNumber      = MIDI_STREAM_IN_EPNUM,
				.DataINEndpointSize        = MIDI_STREAM_EPSIZE,
				.DataINEndpointDoubleBank  = false,

				.DataOUTEndpointNumber     = MIDI_STREAM_OUT_EPNUM,
				.DataOUTEndpointSize       = MIDI_STREAM_EPSIZE,
				.DataOUTEndpointDoubleBank = false,
			},
	};

	
int main(void) {

	SetupHardware();
	
	sei();
	
	for (;;) { 

		MIDI_OUT();
		
		MIDI_Device_USBTask(&MIDI_Interface);
		USB_USBTask();
		
	}
	
}


// Configures the board hardware and chip peripherals for the demo's functionality.
void SetupHardware(void) {

	// Disable watchdog if enabled by bootloader/fuses
	MCUSR &= ~(1 << WDRF);
	wdt_disable();
	
	// Hardware Initialization  
	Serial_Init(115200, false);
	
	LEDs_Init();
	USB_Init();
	
	// Start the flush timer so that overflows occur rapidly to push received bytes to the USB interface
	TCCR0B = (1 << CS02);

}


// Event handler for the library USB Connection event. */
void EVENT_USB_Device_Connect(void) {
	LEDs_SetAllLEDs(LEDMASK_USB_ENUMERATING);
}

// Event handler for the library USB Disconnection event. */
void EVENT_USB_Device_Disconnect(void) {
	LEDs_SetAllLEDs(LEDMASK_USB_NOTREADY);
}

// Event handler for the library USB Configuration Changed event. */
void EVENT_USB_Device_ConfigurationChanged(void) {
	bool ConfigSuccess = true;
	ConfigSuccess &= MIDI_Device_ConfigureEndpoints(&MIDI_Interface);
	LEDs_SetAllLEDs(ConfigSuccess ? LEDMASK_USB_READY : LEDMASK_USB_ERROR);
}

// Event handler for the library USB Control Request reception event. */
void EVENT_USB_Device_ControlRequest(void) {
	MIDI_Device_ProcessControlRequest(&MIDI_Interface);
}

void MIDI_OUT(void) {

	if (complete == 1) {

		complete = 0;
		
		uint8_t Channel = 0;  

		MIDI_EventPacket_t MIDIEvent = (MIDI_EventPacket_t) {
			.CableNumber = 0,
			.Command     = MIDI_FROM_ARDUINO.Data1 >> 4,
			.Data1       = MIDI_FROM_ARDUINO.Data1 | Channel, 
			.Data2       = MIDI_FROM_ARDUINO.Data2, 
			.Data3       = MIDI_FROM_ARDUINO.Data3,		
		};
		
		
		MIDI_Device_SendEventPacket(&MIDI_Interface, &MIDIEvent);
		MIDI_Device_Flush(&MIDI_Interface);
		
	}
	
}

// Serial interrupt 
ISR(USART1_RX_vect, ISR_BLOCK) {
	
	uint8_t ReceivedByte = UDR1;
	
	// Basic MIDI parser
	if (USB_DeviceState == DEVICE_STATE_Configured) {
	
		if ( (ReceivedByte >> 7 ) == 1 ) {
			dCount = 0; 
			memset(&MIDI_FROM_ARDUINO, 0, sizeof(MIDI_EventPacket_t));
			MIDI_FROM_ARDUINO.Data1 = ReceivedByte;
		}
			
		else if ( ((ReceivedByte >> 7) == 0) && (dCount == 0) ) {
			dCount = 1; 
			MIDI_FROM_ARDUINO.Data2 = ReceivedByte;
		}
		
		else if ( ((ReceivedByte >> 7) == 0) && (dCount == 1) ) {
			dCount = 0; 
			MIDI_FROM_ARDUINO.Data3 = ReceivedByte;
			complete = 1; 
		}
		
	}
	
}
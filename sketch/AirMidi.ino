/***********************************************************************
 *  HIDUINO_MIDI_Example01
 *  MIDI Output
 *  by Dimitri Diakopoulos (http://www.dimitridiakopoulos.com)
 *  Music Technology: Interaction, Intelligence & Design, October 2011
 *************************************************************************/

#include <MIDI.h>
#define MaxAD 400
int MinAD = 243

int sensorValue;
int MIDICC,MIDICC_Last;


void setup() {
  MIDI.begin(MIDI_CHANNEL_OMNI);
  //Serial.begin(9600);
}

void loop() {
  sensorValue = analogRead(A0) - MinAD;
  if (sensorValue > MaxAD) sensorValue = MaxAD;
  else {
    if (sensorValue < MinAD) sensorValue = 0;
    else sensorValue -= MinAD;
  } 
  MIDICC = map(sensorValue,0,MaxAD,0,127);
  if (abs(MIDICC - MIDICC_Last) > 3){
    //Serial.println(MIDICC);
    MIDI.sendControlChange(2,MIDICC,1);
    MIDICC_Last = MIDICC;
  }
}


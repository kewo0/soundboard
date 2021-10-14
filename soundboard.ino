const byte TOTAL_BUTTONS = 16;

// All the STM pins used for buttons
const byte BUTTONS_PIN[TOTAL_BUTTONS] = {PB5,PB6,PB7,PB12,PB13,PB14,PB15,PA0,PA1,PA2,PA3,PA4,PA5,PA6,PA11,PA12};

// Every pitch corresponding to pin. Each note has an associated numeric pitch
const byte BUTTONS_PITCH[TOTAL_BUTTONS] = {24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39};

// Current state of the pressed buttons (low or high)
byte currentRead[TOTAL_BUTTONS];

// Temporary input reads to check against current state.
byte tempRead;

void setup()
{
  Serial.begin(115200);
  for (byte i = 0; i < TOTAL_BUTTONS; i++) {
    pinMode(BUTTONS_PIN[i], INPUT_PULLUP);
  }
}

// Fuction used to send midi messages
// A MIDI command plus its MIDI data parameters (pitch, and velocity) make up a midi message
void midi(byte command, byte pitch, byte velocity)
{
  Serial.write(command);
  Serial.write(pitch);
  Serial.write(velocity);
}

// MIDI Commands
//  0x80     Note Off
//  0x90     Note On

// Send midi bytes for note-on
void noteOn(byte pitch) {
  midi(0x90, pitch, 127);
}

// Send midi bytes for note-off
void noteOff(byte pitch) {
  midi(0x80, pitch, 0);
}

// main program loop
void loop()
{
  // traverse all buttons
  for (byte i = 0; i < TOTAL_BUTTONS; i++) {
    
    // Get the digital state from the button pin.
    // (HIGH - not pressed, LOW - pressed).
    byte buttonState = digitalRead(BUTTONS_PIN[i]);
    
    // Temporarily store the digital state.
    tempRead = buttonState;

    // Continue only if the last state is different to the current state.
    if (currentRead[i] != tempRead) {

      // Added debouncing - see https://www.arduino.cc/en/pmwiki.php?n=Tutorial/Debounce
      delay(2);
      
      // Get the pitch mapped to the pressed button.
      byte pitch = BUTTONS_PITCH[i];
      // Save the new input state.
      currentRead[i] = tempRead;
      
      // Execute note on or note off depending on the button state.
      if (buttonState == LOW) {
        noteOn(pitch);
      } else {
        noteOff(pitch);
      }
    }
  }
}

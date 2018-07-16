
#include <NewPing.h>
#include "MIDIUSB.h"

// RFID Stuff
#include <SPI.h>
#include <MFRC522.h>
//#define MFRC522_SPICLOCK SPI_CLOCK_DIV16     // MFRC522 accept upto 10MHz
#define RST_PIN         9          // Configurable, see typical pin layout above
#define SS_1_PIN        10         // Configurable, take a unused pin, only HIGH/LOW required, must be diffrent to SS 2
#define SS_2_PIN        8          // Configurable, take a unused pin, only HIGH/LOW required, must be diffrent to SS 1
#define NR_OF_READERS   1
#define READER_1   0


byte ssPins[] = {SS_1_PIN};

MFRC522 mfrc522[NR_OF_READERS];   // Create MFRC522 instance.

// Init array that will store new NUID 
byte nuidPICC[4];


#define TRIGGER_PIN   6//12 // Arduino pin tied to trigger pin on ping sensor.
#define ECHO_PIN      5//11 // Arduino pin tied to echo pin on ping sensor.
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
unsigned int pingSpeed = 50; // How frequently are we going to send out a ping (in milliseconds). 50ms would be 20 times a second.
unsigned long pingTimer;     // Holds the next ping time.



//MIDI SECTION
int midiChannel1 = 1;  
 


byte note = 36; //* Lowest note to be used
byte cc = 1; //* Lowest MIDI CC to be used

int ModWheelccNumber = 1; // Mod Wheel is always ccNumber 1 and that is needed to change 'sel' within Ableton Sampler
int AftertouchNumber = 2;
int FootccNumber = 4;

int ccMute = 0;
int cc1 = 10;   // These are mapped to trigger 4x selection ranges in ableton sampler (0-127)
int cc2 = 50;
int cc3 = 90;




void setup() {
  Serial.begin(115200); // Open serial monitor at 115200 baud to see ping results.
  pingTimer = millis(); // Start now.

  // RFID Stuff
  //while (!Serial);    // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
  SPI.begin();        // Init SPI bus
  for (uint8_t reader = 0; reader < NR_OF_READERS; reader++) {
    mfrc522[reader].PCD_Init(ssPins[reader], RST_PIN); // Init each MFRC522 card
    Serial.print(F("Reader "));
    Serial.print(reader);
    Serial.print(F(": "));
    mfrc522[reader].PCD_DumpVersionToSerial();
  }  
}






void loop() {
  // Notice how there's no delays in this sketch to allow you to do other processing in-line while doing distance pings.
  if (millis() >= pingTimer) {   // pingSpeed milliseconds since last ping, do another ping.
    pingTimer += pingSpeed;      // Set the next ping time.
    sonar.ping_timer(echoCheck); // Send out the ping, calls "echoCheck" function every 24uS where you can check the ping status.
  }

  //ObjectSamples(); 
  ObjectsForSongs();
}






void echoCheck() { // Timer2 interrupt calls this function every 24uS where you can check the ping status.
  // Don't do anything here!
  if (sonar.check_timer()) { // This is how you check to see if the ping was received.
    // Here's where you can add code.
//    Serial.print("Ping: ");
//    Serial.print(6*(sonar.ping_result / US_ROUNDTRIP_CM)); // Ping returned, uS result in ping_result, convert to cm with US_ROUNDTRIP_CM.
//    Serial.println("cm");
    
//    int mappedVal = sonar.ping_cm();
   int mappedVal = map((6*(sonar.ping_result / US_ROUNDTRIP_CM)), 18, 126, 126, 0);
   Serial.println(mappedVal);
    
        controlChange(midiChannel1, 14, mappedVal); // (Channel, CC number, CC value)
        //controlChange(midiChannel1, 14, 10*(sonar.ping_result / US_ROUNDTRIP_CM)); // (Channel, CC number, CC value)

        MidiUSB.flush();      
  }
  // Don't do anything here!
}





//Helper routine to dump a byte array as hex values to Serial.
void dump_byte_array(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
}







void ObjectsForSongs(){
  for (uint8_t reader = 0; reader < NR_OF_READERS; reader++) {

    // Look for new cards
    if (mfrc522[READER_1].PICC_IsNewCardPresent() && mfrc522[READER_1].PICC_ReadCardSerial()) {
        nuidPICC[0] = mfrc522[0].uid.uidByte[0];
        Serial.print("                   Reader 2: ");           
        Serial.println(nuidPICC[0]);

        Songs();
//    if (nuidPICC[0] == 7) {        // GROOVE ARMADA
//        noteOn(midiChannel1, 1, 127); //channel, note, velocity
//        noteOn(midiChannel1, 1, 0);        //channel, note, velocity
//        MidiUSB.flush();         
//    } 
//
//    if (nuidPICC[0] ==39) {        // BIG MATROSKA
//        noteOn(midiChannel1, 2, 127); //channel, note, velocity
//        noteOn(midiChannel1, 2, 0);        //channel, note, velocity
//        MidiUSB.flush();         
//    } 
//
//    if (nuidPICC[0] ==247) {        // MED MATROSKA
//        noteOn(midiChannel1, 3, 127); //channel, note, velocity
//        noteOn(midiChannel1, 3, 0);        //channel, note, velocity
//        MidiUSB.flush();         
//    } 
//
//    if (nuidPICC[0] ==167) {        // TINY MATROSKA
//        noteOn(midiChannel1, 4, 127); //channel, note, velocity
//        noteOn(midiChannel1, 4, 0);        //channel, note, velocity
//        MidiUSB.flush();         
//    } 
//    
//    if (nuidPICC[0] == 71) {        // ACE OF SPADES
//        noteOn(midiChannel1, 5, 127); //channel, note, velocity
//        noteOn(midiChannel1, 5, 0);        //channel, note, velocity
//        MidiUSB.flush();         
//    } 
//
//    if (nuidPICC[0] == 183) {        // ACE OF SPADES
//        noteOn(midiChannel1, 6, 127); //channel, note, velocity
//        noteOn(midiChannel1, 6, 0);        //channel, note, velocity
//        MidiUSB.flush();         
//    } 
//        
//
//    if (nuidPICC[0] == 84) {        // ***** I CHANGED THIS SECTION ******   THE TIN
//      Serial.println("TIN");
//        controlChange(midiChannel1, FootccNumber, cc1); // (Channel, CC number, CC value)
//        MidiUSB.flush();         
//    } 
//
//    if (nuidPICC[0] == 230) {        // ***** I CHANGED THIS SECTION ******   THE CUP
//      Serial.println("CUP");
//        controlChange(midiChannel1, FootccNumber, cc2); // (Channel, CC number, CC value)
//        MidiUSB.flush();         
//    } 
//
//    if (nuidPICC[0] == 103) {        // ***** I CHANGED THIS SECTION ******   THE GLASS
//      Serial.println("GLASS");
//        controlChange(midiChannel1, FootccNumber, cc3); // (Channel, CC number, CC value)
//        MidiUSB.flush();         
//    } 
//
////MUTE
//    if (nuidPICC[0] == 132) {        // ***** I CHANGED THIS SECTION ******   THE GLASS
//      Serial.println("MUTE");
//        controlChange(midiChannel1, FootccNumber, ccMute); // (Channel, CC number, CC value)
//        MidiUSB.flush();         
//    } 

        
    
      // Halt PICC
      mfrc522[reader].PICC_HaltA();
      // Stop encryption on PCD
      mfrc522[reader].PCD_StopCrypto1();
    } //if (mfrc522[reader].PICC_IsNewC
  } //for(uint8_t reader
}









//******************************************************************
// These are midi functions specifically for the Arduino Micro Pro
void noteOn(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOn = {0x09, 0x90 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOn);
}

void noteOff(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOff = {0x08, 0x80 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOff);
}

void controlChange(byte channel, byte control, byte value) {
  midiEventPacket_t event = {0x0B, 0xB0 | channel, control, value};
  MidiUSB.sendMIDI(event);
}

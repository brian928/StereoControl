/*
  
 */

#include <IRLib.h>

/*
 * Define the values for our Sony remote
*/
#define RECV_PIN   12
#define MY_PROTOCOL SONY
//       Button        Value
#define POWER         0xA81 //
#define RIGHT_ARROW   0x2C1 //
#define LEFT_ARROW    0xCC1 //
#define OK_BUTTON     0xF16 //
#define UP_ARROW      0x4C1 //
#define DOWN_ARROW    0xEC1 //
#define ENTER         0xF16 //
#define MENU          0x9E1 //
#define EXIT          0x1C1 //
#define MUTE          0x281 //
#define VOL_DOWN      0xC81 //
#define VOL_UP        0x481 //
#define CHAN_DOWN     0x896 //
#define CHAN_UP       0x096 //
#define BUTTON_0      0xFFF //
#define BUTTON_1      0x461 // 
#define BUTTON_2      0xA61
#define BUTTON_3      0x716
#define BUTTON_4      0xD0C
#define BUTTON_5      0xAD1
#define BUTTON_6      0x2CE
#define BUTTON_7      0x28E
#define BUTTON_8      0x88E
#define BUTTON_9      0xE96
/*
 * Sony protocol sends the signal twice. We need to throw one of them out.
 */
unsigned long signalTime, lastSignalTime;
unsigned long lastSignal = 0; 
/*
 * Pin number assignments for the relays
 */
#define RELAY1    7
#define RELAY2    8
#define RELAY3    9
#define RELAY4    10
#define RELAY_OFF 11    // turns all relays off     
#define RELAY_TIME  10  // Delay time it takes to switch the relay

#define ONOFF 6   // pin for on/off light for remote learning

int lastRelay = RELAY_OFF;
int powerState = 0;  // we'll assume we are off

/*
 * IR Remote control variables
 */

IRrecv My_Receiver(RECV_PIN);
IRdecode My_Decoder;

/*
 * the setup function runs once when you press reset or power the board
*/
void setup() {
  Serial.begin(9600);
  pinMode(ONOFF, OUTPUT);
  powerOn();  // let's turn us on
   
  // initialize pins for the relays.
  pinMode(RELAY1, OUTPUT);
  pinMode(RELAY2, OUTPUT);
  pinMode(RELAY3, OUTPUT);
  pinMode(RELAY4, OUTPUT);
  pinMode(RELAY_OFF, OUTPUT);

  // IR Receiver setup
  My_Receiver.No_Output();//Turn off any unused IR LED output circuit
  My_Receiver.enableIRIn(); // Start the receiver

  lastSignalTime = millis();  // initialize time
    
}

// the loop function runs over and over again forever
void loop() {
    if (My_Receiver.GetResults(&My_Decoder)) {
       My_Decoder.decode();
         Serial.print("real decode type:");
         Serial.print(Pnames(My_Decoder.decode_type));
         Serial.print(" value: 0x");
         Serial.println(My_Decoder.value, HEX);
       
       if(My_Decoder.decode_type==MY_PROTOCOL) {
          signalTime = millis();
          Serial.print("Signal time: ");
          Serial.println(signalTime - lastSignalTime);
          if (((signalTime - lastSignalTime) > 100) || (My_Decoder.value != lastSignal)) {
             switch(My_Decoder.value) {
               case POWER:         powerOn(); break; 
               case LEFT_ARROW:    ; break;
               case RIGHT_ARROW:   ; break;
               case OK_BUTTON:     ; break;
               case UP_ARROW:      ; break;
               case DOWN_ARROW:    ; break;
               case BUTTON_0:      setRelay(RELAY_OFF); break;
               case BUTTON_1:      setRelay(RELAY1); break;
               case BUTTON_2:      ; break;
               case BUTTON_3:      ; break;
               case BUTTON_4:      ; break;
               case BUTTON_5:      ; break;
               case BUTTON_6:      ; break;
               case BUTTON_7:      ; break;
               case BUTTON_8:      ; break;
               case BUTTON_9:      ; break;
               default: Serial.println("not programmed button"); break;
             }
          }
          lastSignal = My_Decoder.value;
          lastSignalTime = signalTime;
       }
     My_Receiver.resume();
    }

}

/*
 * Function to set the correct relay
 */
void setRelay(int relay) {
  /* We are using a Latching relay so the pin just needs to be HIGH for a short
   *  ammount of time (RELAY_TIME) then brought LOW. 
   */
   if (relay != lastRelay) {
     lastRelay = relay;
     
     digitalWrite(RELAY_OFF, HIGH);   // turn off the all relays
     delay(RELAY_TIME);
     digitalWrite(RELAY_OFF, LOW);    // turn the LED off by making the voltage LOW

     digitalWrite(relay, HIGH);   // turn the LED on (HIGH is the voltage level)
     delay(RELAY_TIME);
     digitalWrite(relay, LOW);    // turn the LED off by making the voltage LOW

   }
}
/*
 * Function to turn this unit on, for setup of new remote
 */
void powerOn() {

   if (powerState == 1) {  // then turn off
     powerState = 0;    
     digitalWrite(ONOFF, LOW);
   }
   else {                  // turn on
    powerState = 1;    
     digitalWrite(ONOFF, HIGH);
   }
}

/* Teensy CAN-Bus with OLED 128x64 demo
 *  
 * www.skpang.co.uk
 * 
 * V1.0 Dec 2016
 *  
 * For use with Teensy CAN-Bus Breakout board:
 * http://skpang.co.uk/catalog/teensy-canbus-breakout-board-include-teensy-32-p-1507.html
 * requires OLED display
 * http://skpang.co.uk/catalog/oled-128x64-display-for-teensy-breakout-board-p-1508.html
 * 
 * Also requres new FlexCAN libarary
 * https://github.com/collin80/FlexCAN_Library
 *  
 */
#include <FlexCAN.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <Fonts/FreeMono9pt7b.h>

#define OLED_DC     6
#define OLED_CS     10
#define OLED_RESET  5
Adafruit_SSD1306 display(OLED_DC, OLED_RESET, OLED_CS);


#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

static CAN_message_t msg,rxmsg;
volatile uint32_t count = 0;
IntervalTimer TX_timer;
String CANStr(""); 
volatile uint32_t can_msg_count = 0;

void setup(){
  
  Can0.begin(500000); //set speed here. 
  
  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC);
  // Clear the buffer.
  display.clearDisplay();
  
  delay(1000);
  Serial.println(F("CAN Bus Tx test"));

  display.setTextSize(0);
  display.setTextColor(WHITE);
  display.setCursor(0,15);
  display.println(" Teensy CAN-Bus demo");
  display.println(" ");
  display.println(" skpang.co.uk");
  display.println(" ");
  display.println(" 12/16");
  display.display();

  delay(1000);
  
  display.clearDisplay();
  
  msg.buf[0] = 1;
  msg.buf[1] = 2;
  msg.buf[2] = 0;
  msg.buf[3] = 0;
  msg.buf[4] = 0;
  msg.buf[5] = 0;
  msg.buf[6] = 0;
  msg.len = 8;
  msg.id = 0x222;
  msg.timeout = 500;
  TX_timer.begin(tx_CAN, 1000000);    /* Start interrutp timer */
 
}

/* From Timer Interrupt */
void tx_CAN(void)
{
  msg.buf[4] = can_msg_count >> 24;
  msg.buf[5] = can_msg_count >> 16;
  msg.buf[6] = can_msg_count >> 8;
  msg.buf[7] = can_msg_count;
  Can0.write(msg);
  can_msg_count++;
}

void loop() {
  int i;
  
  while(Can0.read(rxmsg))
  { 
     String CANStr(""); 
     for (int i=0; i < 8; i++) {     

         CANStr += String(rxmsg.buf[i],HEX);
         CANStr += (" ") ;
     }
     Serial.print(rxmsg.id,HEX); 
     Serial.print(' '); 
     Serial.print(rxmsg.len,HEX); 
     Serial.print(' ');
     Serial.println(CANStr);  
     
     display.fillRect(0,15,128, 30,BLACK);
     display.setCursor(0,15);
     
     display.println(rxmsg.id,HEX); 
     display.println(rxmsg.len,HEX); 
     display.println(CANStr);
     display.display();

     
  }
}




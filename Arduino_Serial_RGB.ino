#include <Adafruit_NeoPixel.h>
#include <SoftwareSerial.h>
#define PIN 6

SoftwareSerial impSerial(10, 9); // RX on 8, TX on 9
String code = "";
String prevCode = "";
String type = "";
String where = "";

boolean isRed = false;
boolean isBlue = false;
boolean isGreen = false;

int r = -1;
int g = -1;
int b = -1;

int currentRed = 0;
int currentBlue = 0;
int currentGreen = 0;

int buttonHue = 0;
int hue = 0;
int INCREMENT = 3;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, PIN, NEO_GRB + NEO_KHZ800);

void setup()  
{
 // Open the hardware serial port
  Serial.begin(19200);  
  impSerial.begin(19200);
  
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  
}

void loop() // run over and over
{ 
  // Send data from the software serial
  
  poll();

}

void testCode(String type, String whereString, int red, int green, int blue){

    Serial.print(red);
    Serial.print(green);
    Serial.print(blue);
    int where = whereString.toInt();
    
  
if (type == ("NORMAL")){

    Serial.print(red);
    Serial.print(green);
    Serial.print(blue);
    
    for(int i = 0; i < strip.numPixels(); i++){
        strip.setPixelColor(i, strip.Color(red, green, blue));
        currentRed = red;
        currentGreen = green;
        currentBlue = blue;
    }
    strip.show(); 
    poll();
}
   
  
 if (type == ("INDIVIDUAL")){

    int counter = 0;
    
    for(int x = 0; x < whereString.length(); x++){
      if(whereString.charAt(x) == '*'){counter++;}
    }    
    
    int LEDs[counter];

      int arrayItem = 0;
      int startNum = 0;

    for(int x = 0; x < whereString.length(); x++){

      if(whereString.charAt(x) == '*'){
        LEDs[arrayItem] = whereString.substring(startNum, (x)).toInt();
        arrayItem++;
        startNum = x+1;
    }
    
    }

    
    for(int x = 0; x < counter; x++){
      strip.setPixelColor(LEDs[x], strip.Color(red, green, blue));
        
      currentRed = red;
      currentGreen = green;
      currentBlue = blue;
    }
   
    
    strip.show();
    type = "";
 }
 
 
 
 if (type == ("RAINBOW")){
   rainbow(where);
 }
 
 if (type == ("WIPE")){
   colorWipe(strip.Color(red, green, blue), where);
 }
 
 if (type == ("WHEEL")){
  rainbowCycle(where);
 }
 
 if (type == ("FLAME")){
   
   while(true){
     
     int Red = random(120)+135;
     int Yellow = random(120)+135;
     
       for(int i = 0; i < strip.numPixels(); i++){
            strip.setPixelColor(i, strip.Color(Red, Yellow, 0));
        }
        strip.show(); 
        poll();
        
        delay(random(100));
        
       }
 }
 
  if (type == ("SNAKE")){
       snake(strip.Color(red, green, blue), where);
 }


}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      poll();
      
      delay(wait);
  }
}

void snake(uint32_t c, uint8_t wait) {
  while(true){
    for(uint16_t i=0; i<48; i++) {
        strip.setPixelColor(i, c);  
        strip.setPixelColor(i-1, strip.Color(0, 0, 0));
        strip.setPixelColor(0, strip.Color(0, 0, 0));
        strip.show();
        poll();
        
        delay(wait);
    }
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    poll();
    
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*1; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    poll();
    
    delay(wait);
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}

void poll(){

  if(impSerial.available()>0){
    
    char c = impSerial.read();

    if(c == '!'){
        //testCode(code);

          if(code.equals("NORMAL")){type = "NORMAL";}
          if(code.equals("INDIVIDUAL")){type = "INDIVIDUAL";}
          if(code.equals("RAINBOW")){type = "RAINBOW";}
          if(code.equals("COMPLEMENTARY")){type = "COMPLEMENTARY";}
          if(code.equals("WIPE")){type = "WIPE";}
          if(code.equals("WHEEL")){type = "WHEEL";}
          if(code.equals("FLAME")){type = "FLAME";}
          if(code.equals("SNAKE")){type = "SNAKE";}


          if(prevCode.equals("where")){where = code;}

          if(prevCode.equals("red")){r = code.toInt();}
          if(prevCode.equals("green")){g = code.toInt();}
          if(prevCode.equals("blue")){b = code.toInt();
             testCode(type, where, r, g, b);
             strip.show();
             poll();
             
             r = -1;
             g = -1;
             b = -1;
             type = "";
             where = "";
             code = "";
     }
        prevCode = code;
        code = "";
    
  }
        
    else{code += c;}
}


//5 = up
//11 = left

  if(digitalRead(4) == LOW){}  
//    Serial.println("Center");
  if(digitalRead(5)== LOW){
      left();
    strip.show();}
//    Serial.println("Down");
  if(digitalRead(11) == LOW){
 //   Serial.println("Right"); 
    right();
    strip.show();    
}
  if(digitalRead(12) == LOW)
//    Serial.println("Up");
  if(digitalRead(8) == LOW){
//    Serial.println("Left");

  }


}

void left(){
   INCREMENT = -3;
   right();
   INCREMENT = 3;
}
   
void right(){
  
 if(hue == 0){
    currentRed = 255;
    currentGreen = 0;
    currentBlue = 0;
    hue++;
  }
 else if(hue > 0 && hue <= 59){
    currentRed = 255;
    currentBlue = 0;
    currentGreen += INCREMENT;
    hue++;
 }
 else if(hue == 60){
   currentRed = 255;
   currentBlue = 0;
   currentGreen = 255;
   hue++;
 }
 else if(hue > 60 && hue <= 119){
   currentGreen = 255;
   currentBlue = 0;
   currentRed -= INCREMENT;
   hue++;
 }
 else if(hue == 120){
   currentGreen = 255;
   currentRed = 0;
   currentBlue = 0;
   hue++;
 }
 else if(hue > 120 && hue <= 179){
   currentGreen = 255;
   currentRed = 0;
   currentBlue += INCREMENT;
   hue++;
 }
 else if(hue == 180){
   currentGreen = 255;
   currentBlue = 255;
   currentRed = 0;
   hue++;
 }
 else if(hue > 180 && hue <= 239){
   currentBlue = 255;
   currentRed = 0;
   currentGreen -= INCREMENT;
   hue++;
 }
 else if (hue == 240){
   currentRed = 0;
   currentBlue = 255;
   currentGreen = 0;
   hue++;
 }
 else if (hue > 240 && hue <= 299){
   currentBlue = 255;
   currentGreen = 0;
   currentRed += INCREMENT;
   hue++;
 }
 else if (hue == 300){
   currentBlue = 255;
   currentGreen = 0;
   currentRed = 255;
   hue++;
 }
 else if (hue > 300 && hue <= 359){
   currentRed = 255;
   currentGreen = 0;
   currentBlue -= INCREMENT;
   hue++;
 }
 else if (hue == 360){hue = 0;}
    
  
  for(int i = 0; i < strip.numPixels(); i++)
        strip.setPixelColor(i, strip.Color(currentRed, currentGreen, currentBlue));
                                                                                                                                                                                                                                                                                              
}

void setRed(){
    for(int i = 0; i < strip.numPixels(); i++)
        strip.setPixelColor(i, strip.Color(255, 0, 0));}
void setOrange(){
    for(int i = 0; i < strip.numPixels(); i++)
        strip.setPixelColor(i, strip.Color(255, 100, 0));}
void setYellow(){
    for(int i = 0; i < strip.numPixels(); i++)
        strip.setPixelColor(i, strip.Color(255, 255, 0));}
void setGreen(){
    for(int i = 0; i < strip.numPixels(); i++)
        strip.setPixelColor(i, strip.Color(0, 255, 0));}
void setAqua(){
    for(int i = 0; i < strip.numPixels(); i++)
        strip.setPixelColor(i, strip.Color(0, 255, 255));}
void setBlue(){
    for(int i = 0; i < strip.numPixels(); i++)
        strip.setPixelColor(i, strip.Color(0, 0, 255));}
void setViolet(){
    for(int i = 0; i < strip.numPixels(); i++)
        strip.setPixelColor(i, strip.Color(255, 0, 255));}



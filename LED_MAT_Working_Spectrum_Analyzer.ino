/***
 *                                                                                                                                        
 *    MMMMMMMM               MMMMMMMM   SSSSSSSSSSSSSSS         GGGGGGGGGGGGGEEEEEEEEEEEEEEEEEEEEEE     QQQQQQQQQ     77777777777777777777
 *    M:::::::M             M:::::::M SS:::::::::::::::S     GGG::::::::::::GE::::::::::::::::::::E   QQ:::::::::QQ   7::::::::::::::::::7
 *    M::::::::M           M::::::::MS:::::SSSSSS::::::S   GG:::::::::::::::GE::::::::::::::::::::E QQ:::::::::::::QQ 7::::::::::::::::::7
 *    M:::::::::M         M:::::::::MS:::::S     SSSSSSS  G:::::GGGGGGGG::::GEE::::::EEEEEEEEE::::EQ:::::::QQQ:::::::Q777777777777:::::::7
 *    M::::::::::M       M::::::::::MS:::::S             G:::::G       GGGGGG  E:::::E       EEEEEEQ::::::O   Q::::::Q           7::::::7 
 *    M:::::::::::M     M:::::::::::MS:::::S            G:::::G                E:::::E             Q:::::O     Q:::::Q          7::::::7  
 *    M:::::::M::::M   M::::M:::::::M S::::SSSS         G:::::G                E::::::EEEEEEEEEE   Q:::::O     Q:::::Q         7::::::7   
 *    M::::::M M::::M M::::M M::::::M  SS::::::SSSSS    G:::::G    GGGGGGGGGG  E:::::::::::::::E   Q:::::O     Q:::::Q        7::::::7    
 *    M::::::M  M::::M::::M  M::::::M    SSS::::::::SS  G:::::G    G::::::::G  E:::::::::::::::E   Q:::::O     Q:::::Q       7::::::7     
 *    M::::::M   M:::::::M   M::::::M       SSSSSS::::S G:::::G    GGGGG::::G  E::::::EEEEEEEEEE   Q:::::O     Q:::::Q      7::::::7      
 *    M::::::M    M:::::M    M::::::M            S:::::SG:::::G        G::::G  E:::::E             Q:::::O  QQQQ:::::Q     7::::::7       
 *    M::::::M     MMMMM     M::::::M            S:::::S G:::::G       G::::G  E:::::E       EEEEEEQ::::::O Q::::::::Q    7::::::7        
 *    M::::::M               M::::::MSSSSSSS     S:::::S  G:::::GGGGGGGG::::GEE::::::EEEEEEEE:::::EQ:::::::QQ::::::::Q   7::::::7         
 *    M::::::M               M::::::MS::::::SSSSSS:::::S   GG:::::::::::::::GE::::::::::::::::::::E QQ::::::::::::::Q   7::::::7          
 *    M::::::M               M::::::MS:::::::::::::::SS      GGG::::::GGG:::GE::::::::::::::::::::E   QQ:::::::::::Q   7::::::7           
 *    MMMMMMMM               MMMMMMMM SSSSSSSSSSSSSSS           GGGGGG   GGGGEEEEEEEEEEEEEEEEEEEEEE     QQQQQQQQ::::QQ77777777            
 *                                                                                                              Q:::::Q                   
 *                                                                                                               QQQQQQ                   
 *                                                                                                                                        
 *                                                                                                                                        
 */

#include <Adafruit_NeoPixel.h>

int numStrips = 32;
int ledDataPin = 6;
int ledsPerChannel = 8;
int numLeds = ledsPerChannel * numStrips;

double divideBy1023 = 1.0 / 1023.0;
double divideTimesLeds = divideBy1023 * (ledsPerChannel);


Adafruit_NeoPixel leds = Adafruit_NeoPixel(numLeds, ledDataPin, NEO_GRB + NEO_KHZ800);

uint32_t black = leds.Color(0, 0, 0);

uint32_t colors[5] = {
  leds.Color(0, 150, 150),
  leds.Color(0, 255, 0),
  leds.Color(255, 255, 0),
  leds.Color(255, 105, 0),
  leds.Color(255, 0, 0)
};

int readPin = 0;
int strobePin = 4;
int resetPin = 5;

int spectrum[7] = {0, 0, 0, 0, 0, 0, 0};

void setup() {
  //#if defined DEBUG
  Serial.begin(2000000);
  //#endif
  
  pinMode(readPin, INPUT);
  pinMode(strobePin, OUTPUT);
  pinMode(resetPin, OUTPUT);

  digitalWrite(resetPin, LOW);
  digitalWrite(strobePin, HIGH);

  leds.begin();
  leds.setBrightness(30);
}

void loop() {
  #if defined DEBUG
  unsigned long start = micros();
  #endif

  readSpectrum();
  displayAll();

  #if defined DEBUG
  unsigned long endTime = micros();
  Serial.println(endTime - start);
  #endif
}

void readSpectrum() {
  digitalWrite(resetPin, HIGH);
  digitalWrite(resetPin, LOW);

  delayMicroseconds(75);
  
  for (int i = 0; i < numStrips; i++) {
    digitalWrite(strobePin, LOW);
    delayMicroseconds(40);
    spectrum[i] = analogRead(readPin);
    digitalWrite(strobePin, HIGH);
    delayMicroseconds(40);
  }
}

void displayAll() {
  leds.clear();
  for (int i = 0; i < numStrips; i++) {
    displaySpectrum(spectrum[i], i);
  }

  leds.show();
}

void displaySpectrum(int value, int channel) {
  
  int level = value * divideTimesLeds;
  
  if (value > 100) {
    level++;
  }

  int beginningIndex = ledsPerChannel * channel;
  int endingIndex = beginningIndex + level;

  int ledIndex = 0;
  for(int i = beginningIndex; i < endingIndex; i++){
    leds.setPixelColor(i, colors[ledIndex++]);
  }
}

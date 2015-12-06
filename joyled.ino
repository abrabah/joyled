#include <Adafruit_NeoPixel.h>

#define JOY_LR_PIN  A0
#define JOY_UD_PIN  A1

#define LED_PIN 12
#define MAX_BRIGHTNESS 255 //Define brightness between 0 and 255
#define SATURATION 1.0
int calLR,calUD;

Adafruit_NeoPixel leds = Adafruit_NeoPixel(1,LED_PIN,NEO_GRB + NEO_KHZ800);

void setup() {
  calLR = -1*analogRead(JOY_LR_PIN) + 512;
  calUD = -1*analogRead(JOY_UD_PIN) + 512;
  leds.begin();
}

void loop() {
  int lr = (analogRead(JOY_LR_PIN) - 512) + calLR;
  int ud = (analogRead(JOY_UD_PIN) - 512) + calUD;
  if(zero(lr,ud)){
    lr = 0; ud = 1;
  }

  calculateRGB(lr,ud);
}

bool zero(int lr, int ud){
  return (  -20 < ud && ud < 20  && -20 < lr && lr < 20);
}

void calculateRGB(int lr, int ud){
  //HSV to RGB color conversion: http://stackoverflow.com/a/6930407

  double hue = rad2deg(getQuadrant(lr,ud) + atan(lr/ ((float)ud)));
  double value = (min(490,sqrt(pow(lr,2) +pow(ud,2)))/490.0)*MAX_BRIGHTNESS;

  double       p, q, t, ff;
  int r,g,b;
  long        i;

   hue /= 60.0;
   i = (long)hue;
   ff = hue - i;
   p = value * (1.0 - SATURATION);
   q = value * (1.0 - (SATURATION * ff));
   t = value * (1.0 - (SATURATION * (1.0 - ff)));

   switch(i) {
   case 0:
       r = value;
       g = t;
       b = p;
       break;
   case 1:
       r = q;
       g = value;
       b = p;
       break;
   case 2:
       r = p;
       g = value;
       b = t;
       break;

   case 3:
       r = p;
       g = q;
       b = value;
       break;
   case 4:
       r = t;
       g = p;
       b = value;
       break;
   case 5:
   default:
       r = value;
       g = p;
       b = q;
       break;
   }
   leds.setPixelColor(0, leds.Color(g,r,b));
   leds.show();
}

double getQuadrant(int lr, int ud){
  if(lr < 0 && ud >= 0){
    return 2*PI;
  }else if(ud < 0){
    return PI;
  } else if(lr < 0 && ud ==0){
    return PI;
  }else {
    return 0.0;
  }
}

int rad2deg(float rad){
  return (rad/(2*PI))*360;
}

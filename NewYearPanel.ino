#include <FastLED.h>

#include <inttypes.h>
#include <avr/pgmspace.h>

//-----------Settings-------------
#define LED_PIN     6
#define COLOR_ORDER RGB
#define CHIPSET     WS2811
#define NUM_LEDS_X    10
#define NUM_LEDS_Y    10
#define NUM_LEDS     100
#define STRIP_VOLTS 5
#define STRIP_MILLIAMPS 2500

#define BRIGHTNESS  100
#define RANDOM 0

#define FPS_SLOW 5
#define FPS_NORMAL 10
#define FPS_FAST 25

#define MODE_DURATION_MS 30000

//Snowflakes mode
#define SNOWFLAKES_CNT 7

//Stars mode
#define STARS_CNT 10

//Meteor mode
#define METEOR_CNT 4
#define METEOR_TAIL 4

//Pong transition
#define PONG_BALLS 2

//For TRANSITION_CIRCLES
#define CIRCLES_CNT 5;

//Modified version from https://robocraft.ru/blog/892
#define FONT_WIDTH 5
#define FONT_HEIGHT 7
const uint8_t FontRus[] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00,// (space) 0x20
  0x00, 0x00, 0x5F, 0x00, 0x00,// ! 0x21
  0x00, 0x07, 0x00, 0x07, 0x00,// " 0x22
  0x14, 0x7F, 0x14, 0x7F, 0x14,// # 0x23
  0x24, 0x2A, 0x7F, 0x2A, 0x12,// $ 0x24
  0x23, 0x13, 0x08, 0x64, 0x62,// % 0x25
  0x36, 0x49, 0x55, 0x22, 0x50,// & 0x26
  0x00, 0x05, 0x03, 0x00, 0x00,// ' 0x27
  0x00, 0x1C, 0x22, 0x41, 0x00,// ( 0x28
  0x00, 0x41, 0x22, 0x1C, 0x00,// ) 0x29
  0x08, 0x2A, 0x1C, 0x2A, 0x08,// * 0x2A
  0x08, 0x08, 0x3E, 0x08, 0x08,// + 0x2B
  0x00, 0x50, 0x30, 0x00, 0x00,// , 0x2C
  0x08, 0x08, 0x08, 0x08, 0x08,// - 0x2D
  0x00, 0x60, 0x60, 0x00, 0x00,// . 0x2E
  0x20, 0x10, 0x08, 0x04, 0x02,// / 0x2F
  // 0x3E, 0x51, 0x49, 0x45, 0x3E,// 0 0x30 - zero crossed
  0x3E, 0x41, 0x41, 0x41, 0x3E,// 0 0x30
  0x00, 0x42, 0x7F, 0x40, 0x00,// 1 0x31
  0x42, 0x61, 0x51, 0x49, 0x46,// 2 0x32
  0x21, 0x41, 0x45, 0x4B, 0x31,// 3 0x33
  0x18, 0x14, 0x12, 0x7F, 0x10,// 4 0x34
  0x27, 0x45, 0x45, 0x45, 0x39,// 5 0x35
  0x3C, 0x4A, 0x49, 0x49, 0x30,// 6 0x36
  0x01, 0x71, 0x09, 0x05, 0x03,// 7 0x37
  0x36, 0x49, 0x49, 0x49, 0x36,// 8 0x38
  0x06, 0x49, 0x49, 0x29, 0x1E,// 9 0x39
  0x00, 0x36, 0x36, 0x00, 0x00,// : 0x3A
  0x00, 0x56, 0x36, 0x00, 0x00,// ; 0x3B
  0x00, 0x08, 0x14, 0x22, 0x41,// < 0x3C
  0x14, 0x14, 0x14, 0x14, 0x14,// = 0x3D
  0x41, 0x22, 0x14, 0x08, 0x00,// > 0x3E
  0x02, 0x01, 0x51, 0x09, 0x06,// ? 0x3F
  0x32, 0x49, 0x79, 0x41, 0x3E,// @ 0x40
  0x7E, 0x11, 0x11, 0x11, 0x7E,// A 0x41
  0x7F, 0x49, 0x49, 0x49, 0x36,// B 0x42
  0x3E, 0x41, 0x41, 0x41, 0x22,// C 0x43
  0x7F, 0x41, 0x41, 0x22, 0x1C,// D 0x44
  0x7F, 0x49, 0x49, 0x49, 0x41,// E 0x45
  0x7F, 0x09, 0x09, 0x01, 0x01,// F 0x46
  0x3E, 0x41, 0x41, 0x51, 0x32,// G 0x47
  0x7F, 0x08, 0x08, 0x08, 0x7F,// H 0x48
  0x00, 0x41, 0x7F, 0x41, 0x00,// I 0x49
  0x20, 0x40, 0x41, 0x3F, 0x01,// J 0x4A
  0x7F, 0x08, 0x14, 0x22, 0x41,// K 0x4B
  0x7F, 0x40, 0x40, 0x40, 0x40,// L 0x4C
  0x7F, 0x02, 0x04, 0x02, 0x7F,// M 0x4D
  0x7F, 0x04, 0x08, 0x10, 0x7F,// N 0x4E
  0x3E, 0x41, 0x41, 0x41, 0x3E,// O 0x4F
  0x7F, 0x09, 0x09, 0x09, 0x06,// P 0x50
  0x3E, 0x41, 0x51, 0x21, 0x5E,// Q 0x51
  0x7F, 0x09, 0x19, 0x29, 0x46,// R 0x52
  0x46, 0x49, 0x49, 0x49, 0x31,// S 0x53
  0x01, 0x01, 0x7F, 0x01, 0x01,// T 0x54
  0x3F, 0x40, 0x40, 0x40, 0x3F,// U 0x55
  0x1F, 0x20, 0x40, 0x20, 0x1F,// V 0x56
  0x7F, 0x20, 0x18, 0x20, 0x7F,// W 0x57
  0x63, 0x14, 0x08, 0x14, 0x63,// X 0x58
  0x03, 0x04, 0x78, 0x04, 0x03,// Y 0x59
  0x61, 0x51, 0x49, 0x45, 0x43,// Z 0x5A
  0x00, 0x00, 0x7F, 0x41, 0x41,// [ 0x5B
  0x02, 0x04, 0x08, 0x10, 0x20,// "\" 0x5C
  0x41, 0x41, 0x7F, 0x00, 0x00,// ] 0x5D
  0x04, 0x02, 0x01, 0x02, 0x04,// ^ 0x5E
  0x40, 0x40, 0x40, 0x40, 0x40,// _ 0x5F
  0x00, 0x01, 0x02, 0x04, 0x00,// ` 0x60
  0x20, 0x54, 0x54, 0x54, 0x78,// a 0x61
  0x7F, 0x48, 0x44, 0x44, 0x38,// b 0x62
  0x38, 0x44, 0x44, 0x44, 0x20,// c 0x63
  0x38, 0x44, 0x44, 0x48, 0x7F,// d 0x64
  0x38, 0x54, 0x54, 0x54, 0x18,// e 0x65
  0x08, 0x7E, 0x09, 0x01, 0x02,// f 0x66
  0x08, 0x14, 0x54, 0x54, 0x3C,// g 0x67
  0x7F, 0x08, 0x04, 0x04, 0x78,// h 0x68
  0x00, 0x44, 0x7D, 0x40, 0x00,// i 0x69
  0x20, 0x40, 0x44, 0x3D, 0x00,// j 0x6A
  0x00, 0x7F, 0x10, 0x28, 0x44,// k 0x6B
  0x00, 0x41, 0x7F, 0x40, 0x00,// l 0x6C
  0x7C, 0x04, 0x18, 0x04, 0x78,// m 0x6D
  0x7C, 0x08, 0x04, 0x04, 0x78,// n 0x6E
  0x38, 0x44, 0x44, 0x44, 0x38,// o 0x6F
  0x7C, 0x14, 0x14, 0x14, 0x08,// p 0x70
  0x08, 0x14, 0x14, 0x18, 0x7C,// q 0x71
  0x7C, 0x08, 0x04, 0x04, 0x08,// r 0x72
  0x48, 0x54, 0x54, 0x54, 0x20,// s 0x73
  0x04, 0x3F, 0x44, 0x40, 0x20,// t 0x74
  0x3C, 0x40, 0x40, 0x20, 0x7C,// u 0x75
  0x1C, 0x20, 0x40, 0x20, 0x1C,// v 0x76
  0x3C, 0x40, 0x30, 0x40, 0x3C,// w 0x77
  0x44, 0x28, 0x10, 0x28, 0x44,// x 0x78
  0x0C, 0x50, 0x50, 0x50, 0x3C,// y 0x79
  0x44, 0x64, 0x54, 0x4C, 0x44,// z 0x7A
  0x00, 0x08, 0x36, 0x41, 0x00,// { 0x7B
  0x00, 0x00, 0x7F, 0x00, 0x00,// | 0x7C
  0x00, 0x41, 0x36, 0x08, 0x00,// } 0x7D
  0x08, 0x08, 0x2A, 0x1C, 0x08,// ->  0x7E
  0x08, 0x1C, 0x2A, 0x08, 0x08, // <- 0x7F
  0x7C, 0x14, 0x14, 0x14, 0x08,//p  0x80
  0x38, 0x44, 0x44, 0x44, 0x20,//c  0x81
  0x04, 0x04, 0x7c, 0x04, 0x04,//т  0x82
  0x0C, 0x50, 0x50, 0x50, 0x3C,//у  0x83
  0x30, 0x48, 0xfc, 0x48, 0x30,//ф  0x84
  0x44, 0x28, 0x10, 0x28, 0x44,//x  0x85
  0x7c, 0x40, 0x40, 0x40, 0xfc,//ц  0x86
  0x0c, 0x10, 0x10, 0x10, 0x7c,//ч  0x87
  0x7c, 0x40, 0x7c, 0x40, 0x7c,//ш  0x88
  0x7c, 0x40, 0x7c, 0x40, 0xfc,//щ  0x89
  0x04, 0x7c, 0x50, 0x50, 0x20,//ъ  0x8A
  0x7c, 0x50, 0x50, 0x20, 0x7c,//ы  0x8B
  0x7c, 0x50, 0x50, 0x20, 0x00,//ь  0x8C
  0x28, 0x44, 0x54, 0x54, 0x38,//э  0x8D
  0x7c, 0x10, 0x38, 0x44, 0x38,//ю  0x8E
  0x08, 0x54, 0x34, 0x14, 0x7c,//я  0x8F
  0x7e, 0x11, 0x11, 0x11, 0x7e,//A  0x90
  0x7f, 0x49, 0x49, 0x49, 0x33,//Б  0x91
  0x7f, 0x49, 0x49, 0x49, 0x36,//В  0x92
  0x7f, 0x01, 0x01, 0x01, 0x03,//Г  0x93
  0xe0, 0x51, 0x4f, 0x41, 0xff,//Д  0x94
  0x7f, 0x49, 0x49, 0x49, 0x41,//E  0x95
  0x77, 0x08, 0x7f, 0x08, 0x77,//Ж  0x96
  0x41, 0x49, 0x49, 0x49, 0x36,//З  0x97
  0x7f, 0x10, 0x08, 0x04, 0x7f,//И  0x98
  0x7c, 0x21, 0x12, 0x09, 0x7c,//Й  0x99
  0x7f, 0x08, 0x14, 0x22, 0x41,//K  0x9A
  0x20, 0x41, 0x3f, 0x01, 0x7f,//Л  0x9B
  0x7f, 0x02, 0x0c, 0x02, 0x7f,//M  0x9C
  0x7f, 0x08, 0x08, 0x08, 0x7f,//H  0x9D
  0x3e, 0x41, 0x41, 0x41, 0x3e,//O  0x9E
  0x7f, 0x01, 0x01, 0x01, 0x7f,//П  0x9F
  0x7f, 0x09, 0x09, 0x09, 0x06,//P  0xA0
  0x3e, 0x41, 0x41, 0x41, 0x22,//C  0xA1
  0x01, 0x01, 0x7f, 0x01, 0x01,//T  0xA2
  0x47, 0x28, 0x10, 0x08, 0x07,//У  0xA3
  0x1c, 0x22, 0x7f, 0x22, 0x1c,//Ф  0xA4
  0x63, 0x14, 0x08, 0x14, 0x63,//X  0xA5
  0x7f, 0x40, 0x40, 0x40, 0xff,//Ц  0xA6
  0x07, 0x08, 0x08, 0x08, 0x7f,//Ч  0xA7
  0x7f, 0x40, 0x7f, 0x40, 0x7f,//Ш  0xA8
  0x7f, 0x40, 0x7f, 0x40, 0xff,//Щ  0xA9
  0x01, 0x7f, 0x48, 0x48, 0x30,//Ъ  0xAA
  0x7f, 0x48, 0x30, 0x00, 0x7f,//Ы  0xAB
  0x00, 0x7f, 0x48, 0x48, 0x30,//Э  0xAC
  0x22, 0x41, 0x49, 0x49, 0x3e,//Ь  0xAD
  0x7f, 0x08, 0x3e, 0x41, 0x3e,//Ю  0xAE
  0x46, 0x29, 0x19, 0x09, 0x7f,//Я  0xAF
  0x20, 0x54, 0x54, 0x54, 0x78,//a  0xB0
  0x3c, 0x4a, 0x4a, 0x49, 0x31,//б  0xB1
  0x7c, 0x54, 0x54, 0x28, 0x00,//в  0xB2
  0x7c, 0x04, 0x04, 0x04, 0x0c,//г  0xB3
  0xe0, 0x54, 0x4c, 0x44, 0xfc,//д  0xB4
  0x38, 0x54, 0x54, 0x54, 0x18,//e  0xB5
  0x6c, 0x10, 0x7c, 0x10, 0x6c,//ж  0xB6
  0x44, 0x44, 0x54, 0x54, 0x28,//з  0xB7
  0x7c, 0x20, 0x10, 0x08, 0x7c,//и  0xB8
  0x7c, 0x41, 0x22, 0x11, 0x7c,//й  0xB9
  0x7c, 0x10, 0x28, 0x44, 0x00,//к  0xBA
  0x20, 0x44, 0x3c, 0x04, 0x7c,//л  0xBB
  0x7c, 0x08, 0x10, 0x08, 0x7c,//м  0xBC
  0x7c, 0x10, 0x10, 0x10, 0x7c,//н  0xBD
  0x38, 0x44, 0x44, 0x44, 0x38,//o  0xBE
  0x7c, 0x04, 0x04, 0x04, 0x7c //п  0xBF
};

#define MODE_CLOCK12 0
#define MODE_MARQUEE_TEXT 1
#define MODE_MARQUEE_YEAR 2
#define MODE_SNOWFLAKES_STRAIGHT 3
#define MODE_HEART 4
#define MODE_SNOWFLAKES 5
#define MODE_XMAS_TREE 6
#define MODE_SNOWFLAKES_STAY 7
#define MODE_STARS 8
#define MODE_METEORS 9
#define MODE_SNOWMAN 10
#define MODE_FATHER_FROST 11
#define MODE_GRADIENT 12
#define COUNT_MODES 13

#define TRANSITION_CLEAR 0
#define TRANSITION_CURTAIN 1
#define TRANSITION_SNAKE 2
#define TRANSITION_SPIRAL 3
#define TRANSITION_PONG 4
#define TRANSITION_DOTS 5
#define TRANSITION_STRIPES 6
#define TRANSITION_CIRCLES 7
#define TRANSITION_CHECKERS 8
#define COUNT_TRANSITIONS 9


//Global vars
CRGB leds[NUM_LEDS];

int mode = 255;
int newMode = 0;

int transition = 255;
int newTransition = 0;

void setup() {
  delay(3000); // sanity delay
  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness( BRIGHTNESS );
  FastLED.setMaxPowerInVoltsAndMilliamps(STRIP_VOLTS, STRIP_MILLIAMPS);
  randomSeed(analogRead(0));
  Serial.begin(9600);
  Serial.println("Init");
  //drawPixel(0, NUM_LEDS_Y - 1, CRGB::Yellow);
  //drawPixel(NUM_LEDS_X - 1, NUM_LEDS_Y - 1, CRGB::White);
  //drawPixel(NUM_LEDS_X - 1, 0, CRGB::White);
  //drawPixel(0, 0, CRGB::White);
  copyright();
  tFade();
}

void loop()
{
  if (RANDOM) {
    do {
      newTransition = random(0, COUNT_TRANSITIONS);
    } while (transition == newTransition);

    do {
      newMode = random(0, COUNT_MODES);
    } while (mode == newMode);
  } else {
    newTransition = transition + 1;
    if (newTransition >= COUNT_TRANSITIONS) {
      newTransition = 0;
    }

    newMode = mode + 1;
    if (newMode >= COUNT_MODES) {
      newMode = 0;
    }
  }

  transition = newTransition;
  mode = newMode;
  Serial.print("Transition ");
  Serial.println(transition);

  switch (transition) {
    case TRANSITION_SNAKE:
      tSnake();
      tFade();
      break;
    case TRANSITION_CURTAIN:
      tCurtain();
      break;
    case TRANSITION_CHECKERS:
      tCheckers();
      tCurtain();
      break;
    case TRANSITION_PONG:
      tPong();
      break;
    case TRANSITION_CLEAR:
      tClear();
      break;
    case TRANSITION_DOTS:
      tRandomDots(false);
      tCurtain();
      break;
    case TRANSITION_STRIPES:
      tStripes();
      tCurtain();
      break;
    case TRANSITION_SPIRAL:
      tSpiral();
      tCurtain();
      break;
    case TRANSITION_CIRCLES:
      tCircles();
      break;

    default:
      error();
  }

  FastLED.clear(true);
  FastLED.show();

  Serial.print("Mode ");
  Serial.println(mode);

  switch (mode) {
    case MODE_CLOCK12:
      clock12();
      break;
    case MODE_MARQUEE_TEXT:
      marquee("С Наступающим!", false, true);
      break;
    case MODE_SNOWFLAKES_STAY:
      snowFlakes(true, true);
      break;
    case MODE_MARQUEE_YEAR:
      marquee("2023", true, false);
      break;
    case MODE_HEART:
      heart();
      break;
    case MODE_SNOWFLAKES:
      snowFlakes(false, true);
      break;
    case MODE_XMAS_TREE:
      xmasTree();
      break;
    case MODE_STARS:
      stars();
      break;
    case MODE_METEORS:
      meteors();
      break;
    case MODE_SNOWMAN:
      snowMan();
      break;
    case MODE_SNOWFLAKES_STRAIGHT:
      snowFlakes(true, false);
      break;
    case MODE_FATHER_FROST:
      fatherFrost();
      break;
    case MODE_GRADIENT:
      gradient();
      break;
    default:
      error();
  }
}

void tClear()
{
  FastLED.clear(true);
  FastLED.show();
}

void tFade()
{
  for ( int x = BRIGHTNESS; x > 0; x--) {
    FastLED.setBrightness(x);
    FastLED.delay(1000 / FPS_FAST);
  }

  FastLED.clear(true);
  FastLED.setBrightness(BRIGHTNESS);
}

void tCurtain()
{
  for ( byte x = 0; x < NUM_LEDS_X; x++) {
    for ( byte y = 0; y < NUM_LEDS_Y; y++) {
      drawPixel(x, y, CRGB::Black);
    }

    FastLED.delay(1000 / FPS_NORMAL);
  }
  FastLED.clear(true);
  FastLED.show();
}

void tPong()
{
  unsigned long t0 = millis();
  unsigned long t;
  int x[PONG_BALLS], y[PONG_BALLS], prevX[PONG_BALLS], prevY[PONG_BALLS], xStep[PONG_BALLS], yStep[PONG_BALLS];
  int maxX;
  byte hue, i;
  int delay = (int)round(1000 / FPS_NORMAL / PONG_BALLS);

  //square screen is boring, resize it to rectangular
  for (maxX = NUM_LEDS_X; maxX >= 2; maxX--) {
    if (hcf(maxX, NUM_LEDS_Y) == 1) {
      break;
    }
  }

  if (maxX < 2) {
    maxX = NUM_LEDS_X;
  }

  //clear unused pixels
  for (byte xc = maxX; xc < NUM_LEDS_X; xc++) {
    drawLine(xc, 0, xc, NUM_LEDS_Y - 1, CRGB::Black);
  }

  t = t0;
  maxX--;
  xStep[0] = random(1, 3) * 2 - 3; //-1 or 1, i.e. left or right
  for (i = 0; i < PONG_BALLS; i++) {
    prevX[i] = prevY[i] = -1;
    yStep[i] = 1;
    y[i] = 0;
    if (i > 0) {
      xStep[i] = xStep[i - 1] * -1;
    }

    x[i] = random(1, maxX);
    for (byte j = 0; j < i; j++) {
      if (x[i] == x[j]) {
        y[i] = random(1, NUM_LEDS_Y);
        break;
      }
    }
  }

  hue = random(0, 256);
  CHSV clr0 = CHSV(hue, 255, 255);
  CHSV clr1 = CHSV(hue, 255, 128);
  do {
    for (i = 0; i < PONG_BALLS; i++) {
      drawPixel(prevX[i], prevY[i], CRGB::Black);
      prevX[i] = x[i];
      prevY[i] = y[i];
      drawPixel(prevX[i], prevY[i], clr1);
      x[i] += xStep[i];
      y[i] += yStep[i];
      drawPixel(x[i], y[i], clr0);
      //Serial.print(x[i]);
      //Serial.print(" ");
      //Serial.println(y[i]);
      FastLED.delay(delay);

      if (y[i] <= 0 || y[i] >= NUM_LEDS_Y - 1) {
        yStep[i] *= -1;
      }

      if (x[i] <= 0 || x[i] >= maxX) {
        xStep[i] *= -1;
      }
    }

    t = millis();
  } while (t - t0 <= MODE_DURATION_MS);
  FastLED.clear(true);
  FastLED.show();
}

void tRandomDots(bool dissolve)
{
  CRGB clr;
  if (dissolve) {
    clr = CRGB::Black;
  } else {
    clr = getRandomColor();
  }

  unsigned long t0 = millis();
  unsigned long t;
  int x, y;
  int set = 0;
  t = t0;
  do {
    x = random(0, NUM_LEDS_X);
    y = random(0, NUM_LEDS_Y);
    if (getColor(x, y) != clr) {
      drawPixel(x, y, clr);
      FastLED.delay(1000 / FPS_FAST);
      set++;
    }

    t = millis();
  } while (t - t0 <= MODE_DURATION_MS && set < NUM_LEDS);

}

void tSpiral()
{
  byte hue, val;
  int x, y;
  hue = random(0, 16) * 16;
  val = 48;

  int xMin = 0;
  int xMax = NUM_LEDS_X - 1;
  int yMin = 0;
  int yMax = NUM_LEDS_Y - 1;

  do {
    for ( x = xMin; x <= xMax; x++) {
      if (val < 255) {
        val++;
      }
      drawPixel(x, yMin, CHSV(hue, 255, val));
      FastLED.delay(1000 / FPS_NORMAL);
    }

    yMin++;

    for ( y = yMin; y <= yMax; y++) {
      if (val < 255) {
        val++;
      }
      drawPixel(xMax, y, CHSV(hue, 255, val));
      FastLED.delay(1000 / FPS_NORMAL);
    }

    xMax--;

    for ( x = xMax; x >= xMin; x--) {
      if (val < 255) {
        val++;
      }
      drawPixel(x, yMax, CHSV(hue, 255, val));
      FastLED.delay(1000 / FPS_NORMAL);
    }

    yMax--;

    for ( y = yMax; y >= yMin; y--) {
      if (val < 255) {
        val++;
      }
      drawPixel(xMin, y, CHSV(hue, 255, val));
      FastLED.delay(1000 / FPS_NORMAL);
    }

    xMin++;

  } while (xMin < xMax && yMin < yMax);

}

void tSnake()
{
  CRGB clr = getRandomColor();
  if (random(0, 2) == 0) {
    for (int L = 0; L < NUM_LEDS; L++) {
      leds[L] = clr;
      FastLED.delay(1000 / FPS_FAST);
    }
  } else {
    for (int L = NUM_LEDS - 1; L >= 0; L--) {
      //Serial.println(L);
      leds[L] = clr;
      FastLED.delay(1000 / FPS_FAST);
    }
  }
}

void tStripes()
{
  CRGB clr = getRandomColor();
  byte xC;
  for ( byte t = 0; t < NUM_LEDS_X; t++) {
    for ( byte y = 0; y < NUM_LEDS_Y; y++) {
      for ( byte x = 0; x <= t; x++) {
        if (y % 2 == 0) {
          xC = x;
        } else {
          xC = NUM_LEDS_X - x;
        }

        drawPixel(xC, y, clr);
      }
    }

    FastLED.delay(1000 / FPS_NORMAL);
  }

  FastLED.clear(true);
  FastLED.show();
}

void tCircles()
{
  CRGB clr = getRandomColor();
  int x, y, Xc, Yc, r, maxR;
  Xc = (int)round(NUM_LEDS_X / 2);
  Yc = (int)round(NUM_LEDS_Y / 2);
  if (NUM_LEDS_X > NUM_LEDS_Y) {
    maxR = NUM_LEDS_X;
  } else {
    maxR = NUM_LEDS_Y;
  }

  for (byte t = 0; t < 5; t++) {
    r = 1;
    drawPixel(Xc, Yc, clr);
    FastLED.delay(1000 / FPS_NORMAL);
    drawPixel(Xc, Yc, CRGB::Black);
    while (r < maxR) {
      drawCircle(Xc, Yc, r, clr);
      FastLED.delay(1000 / FPS_NORMAL);
      drawCircle(Xc, Yc, r, CRGB::Black);
      r++;
    }
  }

}

void tCheckers()
{
  byte hue = random(0, 256);
  CHSV clr1 = CHSV(hue, 255, 255);
  CHSV clr2 = CHSV(256 - hue, 255, 255);
  byte xC;
  for ( byte t = 0; t < NUM_LEDS_X; t++) {
    for ( byte y = 0; y < NUM_LEDS_Y; y++) {
      for ( byte x = 0; x <= t; x++) {
        if (x % 4 < 2 && y % 4 < 2 || x % 4 >= 2 && y % 4 >= 2) {
          drawPixel(x, y, clr1);
        } else {
          drawPixel(x, y, clr2);
        }
      }
    }

    FastLED.delay(1000 / FPS_SLOW);
  }

  FastLED.delay(1000);
  FastLED.clear(true);
  FastLED.show();
}


void snowFlakes(bool stay, bool oscillX)
{
  unsigned long t0 = millis();
  unsigned long t;
  int x, maxPyleY;
  t = t0;
  word tFrame = 0;
  int snowFlakesX0[SNOWFLAKES_CNT];
  int snowFlakesY[SNOWFLAKES_CNT];
  int snowFlakesAmp[SNOWFLAKES_CNT];
  bool respawn, exitLoop;
  CRGB snowflake = CRGB(255, 255, 255);

  //effective if stay=true
  maxPyleY = (int)round(NUM_LEDS_Y / 2);
  exitLoop = false;

  for (byte s = 0; s < SNOWFLAKES_CNT; s++) {
    if (oscillX) {
      snowFlakesAmp[s] = random(1, 3);
    } else {
      snowFlakesAmp[s] = 0;
    }

    snowFlakesX0[s] = random(snowFlakesAmp[s], NUM_LEDS_X - snowFlakesAmp[s]);
    snowFlakesY[s] = random(0, NUM_LEDS_Y) - NUM_LEDS_Y;
  }

  do {

    tFrame++;
    //separate cycle so that snowflakes with bigger index do not erase snowflakes with lower
    for (byte s = 0; s < SNOWFLAKES_CNT; s++) {
      x = snowFlakesX0[s] + (int)round(snowFlakesAmp[s] * sin(tFrame - 1));
      drawPixel(x, snowFlakesY[s], CRGB::Black);
    }

    for (byte s = 0; s < SNOWFLAKES_CNT; s++) {
      respawn = false;
      x = snowFlakesX0[s] + (int)round(snowFlakesAmp[s] * sin(tFrame));
      snowFlakesY[s]++;
      //Serial.print(s);
      //Serial.print(" ");
      //Serial.print(x);
      //Serial.print(" ");
      //Serial.print(snowFlakesY[s]);
      //Serial.println();

      if (stay) {
        if (snowFlakesY[s] >= NUM_LEDS_Y - 1) {
          drawPixel(x, snowFlakesY[s], CRGB::White);
          respawn = true;
        } else if (getColor(x, snowFlakesY[s] + 1) == snowflake) {
          if (
            (x + 1 >= NUM_LEDS_X || getColor(x + 1, snowFlakesY[s] + 1) == snowflake) &&
            (x - 1 < 0 || getColor(x - 1, snowFlakesY[s] + 1) == snowflake)
          ) {
            drawPixel(x, snowFlakesY[s], CRGB::White);
            respawn = true;
          }

          if (respawn && snowFlakesY[s] < maxPyleY) {
            //exitLoop = true;
          }
        }
      }

      if (snowFlakesY[s] >= NUM_LEDS_Y || respawn) {
        snowFlakesY[s] = random(0, NUM_LEDS_Y) - NUM_LEDS_Y;
        snowFlakesX0[s] = random(snowFlakesAmp[s], NUM_LEDS_X - snowFlakesAmp[s]);
        continue;
      }

      drawPixel(x, snowFlakesY[s], CRGB::White);
    }


    FastLED.show(); // display this frame
    FastLED.delay(1000 / FPS_SLOW);
    t = millis();
  } while (t - t0 <= MODE_DURATION_MS && !exitLoop);
}

void clock12()
{
  unsigned long t0 = millis();
  unsigned long t;
  t = t0;

  drawCircle(5, 5, 4, CRGB::Yellow);

  do {
    drawLine(5, 5, 5, 2, CRGB::White);
    drawLine(5, 5, 2, 5, CRGB::White);
    FastLED.delay(1000);
    drawLine(5, 5, 2, 5, CRGB::Black);
    drawLine(5, 5, 3, 3, CRGB::White);
    drawLine(5, 5, 5, 2, CRGB::White);
    FastLED.delay(1000);
    drawLine(5, 5, 3, 3, CRGB::Black);
    drawLine(5, 5, 5, 2, CRGB::White);
    FastLED.delay(1000);
    t = millis();
  } while (t - t0 <= MODE_DURATION_MS / 3);
}

void marquee(String str, bool randomDir, bool randomColor)
{
  static int dir = 0;
  int x;
  CRGB clr[str.length()];

  if (!randomDir) {
    dir = 1;
  } else {
    dir = (dir + 1) % 2;
  }

  //ugly hack to support cyrillic letters - drop UTF-8 first byte
  str.replace("\320", "");
  str.replace("\321", "");

  byte buffer[str.length() + 1];
  str.toCharArray(buffer, str.length() + 1);

  CRGB oneClr = getRandomColor();
  for (int i = 0; i < str.length(); i++) {
    if (randomColor) {
      clr[i] = getRandomColor();
    } else {
      clr[i] = oneClr;
    }
  }

  for (int step = -(FONT_WIDTH + 1) * str.length(); step <= NUM_LEDS_X; step++) {
    if (!dir) { //L2R
      x = step;
    } else { //R2L
      x = NUM_LEDS_X - step - (FONT_WIDTH + 1) * str.length();
    }


    FastLED.clear();
    for (int i = 0; i < str.length(); i++) {
      drawChar(x + i * (FONT_WIDTH + 1), 1, buffer[i], clr[i]);
    }

    FastLED.delay(1000 / FPS_NORMAL);
  }

}

void heart()
{
  byte cols[9] = {0x0C, 0x12, 0x21, 0x41, 0x82, 0x41, 0x21, 0x12, 0x0C};

  unsigned long t0 = millis();
  unsigned long t;
  int x, y, xM, yM, brightness;
  int set = 0;
  bool fill;
  t = t0;
  brightness = 255;
  int brightnessStep = -3;
  xM = (int)round((NUM_LEDS_X - 9) / 2);
  yM = (int)round((NUM_LEDS_Y - 8) / 2);
  CHSV fillClr = CHSV(HSVHue::HUE_PINK, 255, brightness);
  do {
    fill = false;
    brightness += brightnessStep;
    if (brightness <= 0 || brightness >= 255) {
      brightnessStep *= -1;
      brightness += brightnessStep * 2;
    }

    fillClr = CHSV(HSVHue::HUE_PINK, 255, brightness);

    for (int x = 0; x < 9; x++) {
      for (int y = 0; y < 8; y++) {
        if (bitRead(cols[x], y)) {
          drawPixel(x + xM, y + yM, CRGB::Red);
          fill = !fill;
        } else if (fill) {
          drawPixel(x + xM, y + yM, fillClr);
        }
      }
    }

    FastLED.delay(1000 / FPS_FAST);
    t = millis();
  } while (t - t0 <= MODE_DURATION_MS && set < NUM_LEDS);
}

void xmasTree()
{
  byte cols[7] = {0x20, 0x34, 0x3E, 0xFF, 0x3E, 0x34, 0x20};

  unsigned long t0 = millis();
  unsigned long t;
  int x0, y0, x, y, brightness;
  int set = 0;
  bool fill;
  t = t0;
  brightness = 255;
  int brightnessStep = -3;
  x0 = (int)round((NUM_LEDS_X - 7) / 2);
  y0 = 0;
  CHSV treeClr = CHSV(HSVHue::HUE_GREEN, 255, brightness);
  CHSV snowClr = CHSV(0, 0, brightness);

  do {
    brightness += brightnessStep;
    if (brightness <= 30 || brightness >= 255) {
      brightnessStep *= -1;
      brightness += brightnessStep * 2;
    }

    treeClr = CHSV(HSVHue::HUE_GREEN, 255, brightness);
    snowClr = CHSV(0, 0, brightness);

    for (int x = 0; x < 7; x++) {
      for (int y = 0; y < 8; y++) {
        if (bitRead(cols[x], y)) {
          drawPixel(x0 + x, y0 + y, treeClr);
        }
      }
    }

    drawLine(0, 8, NUM_LEDS_X - 1, 8, snowClr);
    drawLine(0, 9, NUM_LEDS_X - 1, 9, snowClr);

    FastLED.delay(1000 / FPS_FAST);
    t = millis();
  } while (t - t0 <= MODE_DURATION_MS && set < NUM_LEDS);
}

void stars()
{
  unsigned long t0 = millis();
  unsigned long t;
  t = t0;
  int starsX[STARS_CNT];
  int starsY[STARS_CNT];
  byte starsClr[STARS_CNT];
  byte starsBr[STARS_CNT];

  for (byte s = 0; s < STARS_CNT; s++) {
    starsBr[s] = 0;
  }

  do {
    for (byte s = 0; s < STARS_CNT; s++) {
      starsBr[s]--;
      if (starsBr[s] <= 30) {
        drawPixel(starsX[s], starsY[s], CRGB::Black);
        starsX[s] = random(0, NUM_LEDS_X);
        starsY[s] = random(0, NUM_LEDS_Y);
        starsClr[s] = random(0, 256);
        starsBr[s] = random(128, 256);
      }

      drawPixel(starsX[s], starsY[s], CHSV(starsClr[s], 255, starsBr[s]));
    }


    FastLED.show(); // display this frame
    FastLED.delay(1000 / FPS_FAST);
    t = millis();
  } while (t - t0 <= MODE_DURATION_MS);
}

void meteors()
{
  unsigned long t0 = millis();
  unsigned long t;
  t = t0;
  int meteorsX[METEOR_CNT];
  int meteorsY[METEOR_CNT];
  int br;

  for (byte s = 0; s < METEOR_CNT; s++) {
    meteorsY[s] = NUM_LEDS_Y + METEOR_TAIL;
  }

  do {
    for (byte s = 0; s < METEOR_CNT; s++) {
      meteorsY[s]++;
      if (meteorsY[s] > NUM_LEDS_Y + METEOR_TAIL) {
        meteorsX[s] = random(0, NUM_LEDS_X);
        meteorsY[s] = random(0, NUM_LEDS_Y) - NUM_LEDS_Y;
      }

      br = 255;
      for (byte b = 0; b < METEOR_TAIL; b++) {
        drawPixel(meteorsX[s], meteorsY[s] - b, CHSV(0, 0, br));
        br = (int)round(br / 1.5);
      }

      drawPixel(meteorsX[s], meteorsY[s] - METEOR_TAIL, CRGB::Black);

      //Serial.print(s);
      //Serial.print(" ");
      //Serial.print(meteorsX[s]);
      //Serial.print(" ");
      //Serial.print(meteorsY[s]);
      //Serial.println();
    }

    FastLED.show(); // display this frame
    FastLED.delay(1000 / FPS_FAST);
    t = millis();


  } while (t - t0 <= MODE_DURATION_MS);
}

void fatherFrost()
{
  CRGB hat = CRGB(255, 0, 0);
  CRGB hair = CRGB(255, 255, 255);
  CRGB lips = CRGB(255, 0, 0);
  CRGB skin = CRGB(255, 200, 61);
  CRGB eye = CRGB(0, 153, 255);
  for ( byte x = 4; x <= 8; x++) {
    for ( byte y = 0; y <= 3; y++) {
      drawPixel(x, y, hat);
    }
  }

  drawPixel(3, 1, hat);
  drawPixel(2, 2, hat);
  drawPixel(3, 2, hat);

  drawLine(8, 0, 9, 0, hair);
  drawLine(8, 1, 9, 1, hair);

  for ( byte x = 1; x <= 8; x++) {
    for ( byte y = 3; y <= 9; y++) {
      drawPixel(x, y, hair);
    }
  }

  drawLine(2, 4, 7, 4, skin);
  drawLine(2, 5, 7, 5, skin);
  drawLine(2, 6, 7, 6, skin);

  drawPixel(3, 4, eye);
  drawPixel(6, 4, eye);

  drawPixel(4, 5, lips);
  drawPixel(5, 5, lips);

  drawPixel(3, 7, lips);
  drawPixel(4, 8, lips);
  drawPixel(5, 8, lips);
  drawPixel(6, 7, lips);

  drawPixel(1, 8, CRGB::Black);
  drawPixel(1, 9, CRGB::Black);
  drawPixel(2, 9, CRGB::Black);
  drawPixel(7, 9, CRGB::Black);
  drawPixel(8, 8, CRGB::Black);
  drawPixel(8, 9, CRGB::Black);
  FastLED.show(); // display this frame
  FastLED.delay(MODE_DURATION_MS / 5);
}

void snowMan()
{
  for ( byte x = 1; x <= 7; x++) {
    for ( byte y = 4; y <= 9; y++) {
      drawPixel(x, y, CRGB::White);
    }
  }

  drawPixel(3, 0, CRGB::White);
  drawPixel(4, 0, CRGB::White);
  drawPixel(5, 0, CRGB::White);
  drawPixel(2, 1, CRGB::White);
  drawPixel(4, 1, CRGB::White);
  drawPixel(6, 1, CRGB::White);
  drawPixel(2, 2, CRGB::White);
  drawPixel(3, 2, CRGB::White);
  drawLine(3, 3, 5, 3, CRGB::White);
  drawLine(4, 2, 6, 2, CRGB::Orange);
  drawPixel(3, 1, CRGB::DarkGreen);
  drawPixel(5, 1, CRGB::DarkGreen);

  drawPixel(1, 4, CRGB::Black);
  drawPixel(7, 4, CRGB::Black);
  drawPixel(1, 9, CRGB::Black);
  drawPixel(7, 9, CRGB::Black);
  FastLED.show(); // display this frame
  FastLED.delay(MODE_DURATION_MS / 5);
}

void gradient()
{
  unsigned long t0 = millis();
  unsigned long t;
  t = t0;
  int tFrame = 0;
  CHSV clr;
  bool transp = false;
  if (random(0, 1)) {
    transp = true;
  }
  do {
    tFrame++;
    for ( byte x = 0; x <= NUM_LEDS_X; x++) {
      clr = CHSV((tFrame + x * 10) % 255, 255, 255);
      for ( byte y = 0; y <= NUM_LEDS_Y; y++) {
        if (transp) {
          drawPixel(x, y, clr);
        } else {
          drawPixel(y, x, clr);
        }
      }
    }

    FastLED.delay(1000 / FPS_FAST);
    t = millis();
  } while (t - t0 <= MODE_DURATION_MS);
}


void copyright()
{
  byte signature[9] = {0xE0, 0xA0, 0xEE, 0x8A, 0x8e, 0, 0xEE, 0x88, 0xEE};
  for (int y = 0; y < 9; y++) {
    for (int x = 0; x < 8; x++) {
      if (bitRead(signature[y], x)) {
        drawPixel(8 - x, y, CRGB::Orange);
      }
    }
  }
}

void error()
{
  FastLED.clear(true);
  drawChar(1, 1, 'X', CRGB::Red);
  FastLED.show();
}

int hcf(int a, int b)
{
  if (a == 0) {
    return b;
  } else if (b == 0) {
    return a;
  }

  while (a != b) {
    if (a > b) {
      a = a - b;
    } else {
      b = b - a;
    }
  }

  return a;
}

void drawChar(int x0, int y0, int chr, CRGB fg)
{
  drawChar(x0, y0, chr, fg, true, CRGB::Black);
}

void drawChar(int x0, int y0, int chr, CRGB fg, bool transparent)
{
  drawChar(x0, y0, chr, fg, transparent, CRGB::Black);
}

void drawChar(int x0, int y0, int chr, CRGB fg, bool transparent, CRGB bg)
{
  if (x0 >= NUM_LEDS_X || y0 >= NUM_LEDS_Y || x0 < -FONT_WIDTH || y0 < -FONT_HEIGHT) {
    return;
  }

  char charData;
  int charCode = chr - 32;
  int x = x0;
  int y;
  for (word k = charCode * FONT_WIDTH; k < charCode * FONT_WIDTH + FONT_WIDTH; k++) {
    charData = pgm_read_byte_near(FontRus + k);
    //Serial.println(charData);
    for (int i = 0; i < FONT_HEIGHT; i++) {
      if (bitRead(charData, i)) {
        drawPixel(x, y0 + i, fg);
      } else if (!transparent) {
        drawPixel(x, y0 + i, bg);
      }
    }

    x++;
  }
}

void drawCircle(byte x0, byte y0, byte R, CRGB color)
{
  byte x = 0;
  byte y = R;
  int delta = 1 - 2 * R;
  int error = 0;
  while (y >= x) {
    drawPixel(x0 + x, y0 + y, color);
    drawPixel(x0 + x, y0 - y, color);
    drawPixel(x0 - x, y0 + y, color);
    drawPixel(x0 - x, y0 - y, color);
    drawPixel(x0 + y, y0 + x, color);
    drawPixel(x0 + y, y0 - x, color);
    drawPixel(x0 - y, y0 + x, color);
    drawPixel(x0 - y, y0 - x, color);
    error = 2 * (delta + y) - 1;
    if ((delta < 0) && (error <= 0)) {
      delta += 2 * ++x + 1;
      continue;
    }
    if ((delta > 0) && (error > 0)) {
      delta -= 2 * --y + 1;
      continue;
    }
    delta += 2 * (++x - --y);
  }
}

void drawLine(int x1, int y1, int x2, int y2, CRGB color) {
  const int deltaX = abs(x2 - x1);
  const int deltaY = abs(y2 - y1);
  const int signX = x1 < x2 ? 1 : -1;
  const int signY = y1 < y2 ? 1 : -1;
  int error = deltaX - deltaY;
  drawPixel(x2, y2, color);
  while (x1 != x2 || y1 != y2)
  {
    drawPixel(x1, y1, color);
    int error2 = error * 2;
    if (error2 > -deltaY)
    {
      error -= deltaY;
      x1 += signX;
    }
    if (error2 < deltaX)
    {
      error += deltaX;
      y1 += signY;
    }
  }

}

CRGB getRandomColor()
{
  byte rand = random(0, 9);
  switch (rand) {
    case 0:
      return CRGB::Gray;
    case 1:
      return CRGB::Aqua;
    case 2:
      return CRGB::Gold;
    case 3:
      return CRGB::Red;
    case 4:
      return CRGB::Violet;
    case 5:
      return CRGB::Navy;
    case 6:
      return CRGB::Green;
    case 7:
      return CRGB::Orange;
    default:
      return CRGB(random(0, 255), random(0, 255), random(0, 255));
  }
}

CRGB getColor(int x, int y)
{
  int l = getCoord(x, y);
  if (l >= 0 && l < NUM_LEDS) {
    return leds[l];
  }

  return CRGB(0, 0, 0);
}

int getCoord(int x, int y)
{
  if (x < 0 || y < 0 || x >= NUM_LEDS_X || y >= NUM_LEDS_Y) {
    return -1;
  }

  int l;
  l = x * NUM_LEDS_Y;
  if (x % 2 == 1) {
    l = l + 9 - y;
  } else {
    l += y;
  }

  return l;
}

void drawPixel(int x, int y, CRGB color)
{
  int l = getCoord(x, y);
  if (l >= 0 && l < NUM_LEDS) {
    leds[l] = color;
  }
}

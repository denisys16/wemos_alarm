#include <LEDStrip.h>

#define NUM_PIXELS 60

Adafruit_NeoPixel pixels(NUM_PIXELS, D2, NEO_GRB | NEO_KHZ800);

typedef enum {lsmIDLE, lsmPOWERON, lsmALARM, lsmCOLOR} ledstrip_mode;
ledstrip_mode mode = lsmIDLE;
ledstrip_mode next_mode = lsmIDLE;

uint8_t color_r;
uint8_t color_g;
uint8_t color_b;

uint8_t alarm_r;
uint8_t alarm_g;
uint8_t alarm_b;

// This table remaps linear input values (the numbers we’d
// like to use; e.g. 127 = half brightness) to nonlinear
// gamma-corrected output values (numbers producing the
// desired effect on the LED; e.g. 36 = half brightness).
const  uint8_t gamma8[] = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
    2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
    5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
   10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
   17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
   25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
   37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
   51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
   69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
   90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
  115,117,119,120,122,124,126,127,129,131,133,135,137,138,140,142,
  144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175,
  177,180,182,184,186,189,191,193,196,198,200,203,205,208,210,213,
  215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255 };

static long currTime = millis();
static long next_delay = 1000;
static int alarm_cnt = 0;

static void setColor(uint32_t dly, uint8_t r, uint8_t g, uint8_t b)
{
    mdebug_printf(MLVL_DEBUG,"Set LED strip color to R:%u G:%u B:%u\n", r, g, b);
    uint8_t g_r = gamma8[r];
    uint8_t g_g = gamma8[g];
    uint8_t g_b = gamma8[b];
    for (int i = 0; i < NUM_PIXELS; i++)
    {
        pixels.setPixelColor(i, g_r, g_g, g_b);
        pixels.show();
        delay(dly);
        yield();
    }
}

static void to_idle()
{
    next_mode = lsmIDLE;
    next_delay = 1000;
}

static void on_poweron()
{
    to_idle();
}

static void on_idle()
{

}

static void on_alarm()
{
    if (alarm_cnt<510)
    {
        mdebug_printf(MLVL_DEBUG,"Alarm counter:%u/509\n", alarm_cnt);
        alarm_cnt++;
        if (alarm_r < 128)
            alarm_r++;
        else if (alarm_g < 127)
            {alarm_r++;alarm_g++;}
        else if (alarm_b < 128)
            {alarm_g++;alarm_b++;}
        else if (alarm_b < 255)
            alarm_b++;
        setColor(0,alarm_r,alarm_g,alarm_b);
    }
    else
    {
        mdebug_printf(MLVL_DEBUG,"Alarm finished\n");
        to_idle();
    }
}

static void on_color()
{
    setColor(0,color_r,color_g,color_b);
    to_idle();
}

void ledstrip_start_alarm()
{
    if (mode != lsmALARM)
    {
        next_delay = 3530; // (30min * 60c *1000ms) / (510) = 3530ms
        alarm_cnt = 0;
        alarm_r = 0;
        alarm_g = 0;
        alarm_b = 0;
        next_mode = lsmALARM;
    }
}

void ledstrip_set_color(uint8_t r, uint8_t g, uint8_t b)
{
    color_r = r;
    color_g = g;
    color_b = b;
    next_delay = 1000;
    next_mode = lsmCOLOR;
}

void setup_ledstrip()
{
    pixels.begin();
    setColor(0,255,255,255);
    next_mode = lsmPOWERON;
}

void loop_ledstrip()
{
    if ( (next_mode != mode) || (((millis() - currTime) > next_delay)) )
    {
        mode = next_mode;
        currTime = millis();
        switch(mode)
        {
            case lsmIDLE:
                on_idle();
                break;
            case lsmPOWERON:
                on_poweron();
                break;
            case lsmALARM:
                on_alarm();
                break;
            case lsmCOLOR:
                on_color();
                break;
        }
    }
}

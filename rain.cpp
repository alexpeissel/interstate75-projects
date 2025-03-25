#include "pico/stdlib.h"
#include "pico/rand.h"

#include "libraries/pico_graphics/pico_graphics.hpp"
#include "libraries/interstate75/interstate75.hpp"

#include <stdio.h>

using namespace pimoroni;

// Display size in pixels, note: the i75W RP2350 kit includes two 128x64
// pixel displays which form a chain that's 256x64 pixels in side.
Hub75 hub75(256, 64, nullptr, PANEL_GENERIC, false);

// Our PicoGraphics surface. If we want a square output then use 128x128
// as the width and height, which Hub75 will automatically remap to 256x64.
PicoGraphics_PenRGB888 graphics(128, 128, nullptr);

#define MAX_WIDTH 128
#define MAX_HEIGHT 128
#define FRAME_DELAY_MS 25
#define NUM_DROPS 100

typedef struct
{
  int x;
  int y;
  int width;
  float brightness;
  int tail_length;
  int speed;
} Drop;

// Interrupt callback required function
void __isr dma_complete()
{
  hub75.dma_complete();
}

int random_int(int min, int max)
{
  return (uint8_t)get_rand_32() % (max - min + 1) + min;
}

void clear()
{
  graphics.set_pen(0, 0, 0);
  graphics.clear();
}

void init_drops(Drop *drops)
{
  for (int i = 0; i < NUM_DROPS; i++)
  {
    drops[i].x = random_int(0, MAX_WIDTH);
    drops[i].y = 0;
    drops[i].width = random_int(1, 2);
    drops[i].brightness = 1.0f;
    drops[i].tail_length = random_int(20, 80);
    drops[i].speed = random_int(1, 4);
  }
}

void draw_drops(Drop *drops)
{
  for (int i = 0; i < NUM_DROPS; i++)
  {
    if (drops[i].brightness == 0)
    {
      if (random_int(0, NUM_DROPS) > 1)
      {
        continue;
      }

      drops[i].x = random_int(0, MAX_WIDTH);
      drops[i].y = 0;
      drops[i].brightness = 1.0f;
    }
    else
    {
      drops[i].y = drops[i].y + drops[i].speed;
      if (drops[i].y - drops[i].tail_length >= MAX_HEIGHT)
      {
        drops[i].brightness = 0;
        continue;
      }

      graphics.set_pen(graphics.create_pen(0, 255, 0));
      graphics.rectangle(Rect(drops[i].x, drops[i].y, drops[i].width, 2));

      float tail_segment_brightness = 255 / drops[i].tail_length;
      for (int j = 0; j <= drops[i].tail_length; j++)
      {
        graphics.set_pen(graphics.create_pen(0, 255 - (tail_segment_brightness * j), 0));
        graphics.rectangle(Rect(drops[i].x, drops[i].y - j, drops[i].width, 2));
      }
    }
  }
  hub75.update(&graphics);
}

int main()
{
  // Start hub75 driver
  hub75.start(dma_complete);
  clear();

  Drop drops[NUM_DROPS];
  init_drops(drops);

  while (true)
  {
    clear();
    draw_drops(drops);
    sleep_ms(FRAME_DELAY_MS);
  }

  return 0;
}

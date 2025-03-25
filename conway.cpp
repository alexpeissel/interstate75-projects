#include "pico/stdlib.h"
#include "pico/rand.h"

#include "libraries/pico_graphics/pico_graphics.hpp"
#include "libraries/interstate75/interstate75.hpp"

#include <string.h>

using namespace pimoroni;

// Display size in pixels, note: the i75W RP2350 kit includes two 128x64
// pixel displays which form a chain that's 256x64 pixels in side.
Hub75 hub75(256, 64, nullptr, PANEL_GENERIC, false);

// Our PicoGraphics surface. If we want a square output then use 128x128
// as the width and height, which Hub75 will automatically remap to 256x64.
PicoGraphics_PenRGB888 graphics(128, 128, nullptr);

#define MAX_WIDTH 128
#define MAX_HEIGHT 128
#define FRAME_DELAY_MS 100
#define MAX_GENERATIONS 150

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

void set_pixel(int x, int y, bool alive)
{
  if (x < 0 || x >= MAX_WIDTH || y < 0 || y >= MAX_HEIGHT)
    return;

  if (alive)
  {
    graphics.set_pen(255, 255, 255);
    graphics.pixel(Point(x, y));
  }
}

void show(unsigned (&univ)[MAX_HEIGHT][MAX_WIDTH], int w, int h)
{
  for (int y = 0; y < h; y++)
  {
    for (int x = 0; x < w; x++)
    {
      bool sh = (univ[y][x] == 1);
      set_pixel(x, y, sh);
    }
  }
  hub75.update(&graphics);
}

int count_neighbors(unsigned (&univ)[MAX_HEIGHT][MAX_WIDTH], int w, int h, int x, int y)
{
  int count = 0;
  for (int dy = -1; dy <= 1; dy++)
  {
    for (int dx = -1; dx <= 1; dx++)
    {
      if (dx == 0 && dy == 0)
        continue;

      int nx = (x + dx + w) % w;
      int ny = (y + dy + h) % h;

      count += univ[ny][nx];
    }
  }
  return count;
}

void evolve(unsigned (&univ)[MAX_HEIGHT][MAX_WIDTH], int w, int h)
{
  // Temporary array to store next state
  unsigned newar[h][w] = {0};

  for (int y = 0; y < h; y++)
  {
    for (int x = 0; x < w; x++)
    {
      int neighbors = count_neighbors(univ, w, h, x, y);
      newar[y][x] = (neighbors == 3 || (neighbors == 2 && univ[y][x]));
    }
  }

  memcpy(univ, newar, w * h * sizeof(unsigned));
}

void game(int w, int h)
{
  unsigned univ[MAX_HEIGHT][MAX_WIDTH];

  for (int x = 0; x < w; x++)
  {
    for (int y = 0; y < h; y++)
    {
      univ[y][x] = random_int(0, 100) > 65 ? 1 : 0;
    }
  }

  int sc = 0;

  while (1)
  {
    clear();
    show(univ, w, h);
    evolve(univ, w, h);
    // sleep_ms(FRAME_DELAY_MS);

    // sc++;
    if (sc > MAX_GENERATIONS)
      break;
  }
}

int main()
{
  // Start hub75 driver
  hub75.start(dma_complete);

  while (true)
  {
    game(MAX_WIDTH, MAX_HEIGHT);
  }

  return 0;
}

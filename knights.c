#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "image.h"

#define MAXMOVES  8
#define WIDTH     32
#define HEIGHT    32
#define KNIGHTS   1

#define LIME    0xFFA4C400
#define GREEN   0xFF60A917
#define EMERALD 0xFF008A00
#define TEAL    0xFF00ABA9
#define CYAN    0xFF1BA1E2
#define COBALT  0xFF0050EF
#define INDIGO  0xFF6A00FF
#define VIOLET  0xFFAA00FF
#define PINK    0xFFF472D0
#define MAGENTA 0xFFD80073
#define CRIMSON 0xFFA20025
#define RED     0xFFE51400
#define ORANGE  0xFFFA6800
#define AMBER   0xFFF0A30A
#define YELLOW  0xFFE3C800
#define BROWN   0xFF825A2C
#define OLIVE   0xFF6D8764
#define STEEL   0xFF647687
#define MAUVE   0xFF76608A
#define TAUPE   0xFF87794E

uint32_t colors[] = {
   LIME, GREEN, EMERALD, TEAL, CYAN, COBALT, INDIGO, VIOLET, PINK, MAGENTA,
   CRIMSON, RED, ORANGE, AMBER, YELLOW, BROWN, OLIVE, STEEL, MAUVE, TAUPE
};

#define COLOR_COUNT (sizeof(colors) / sizeof(colors[0]))

typedef struct {
   uint16_t x;
   uint16_t y;
   uint32_t pos;
} Coords;

Coords* coord_to_pos = NULL;
Coords* pos_to_coord = NULL;

typedef struct {
   const char* name;
   uint8_t moves;
   int8_t move_x[MAXMOVES];
   int8_t move_y[MAXMOVES];
} Player;

Player contestants[] = {
   {
      .name = "Knight",    // 2 - 1
      .moves = 8,
      .move_x = { -2, -1,  1,  2, 2, 1, -1, -2 },
      .move_y = { -1, -2, -2, -1, 1, 2,  2,  1 },
   },
   {
      .name = "Leaper",
      .moves = 4,
      .move_x = { 3, 0, -3,  0 },
      .move_y = { 0, 3,  0, -3 },
   },
   {
      .name = "Alfil",
      .moves = 4,
      .move_x = { -2,  2, 2, -2 },
      .move_y = { -2, -2, 2,  2 },
   },
   {
      .name = "Antilope",     // 4 - 3
      .moves = 8,
      .move_x = { -4, -3,  3,  4, 4, 3, -3, -4 },
      .move_y = { -3, -4, -4, -3, 3, 4,  4,  3 },
   },
   {
      .name = "Dabbaba",
      .moves = 4,
      .move_x = { -2, 0, -2,  0 },
      .move_y = {  0, 2,  0, -2 },
   },
   {
      .name = "Wazir",
      .moves = 4,
      .move_x = { 1, 0, -1,  0 },
      .move_y = { 0, 1,  0, -1 },
   },
   {
      .name = "Zebra",     // 3 - 2
      .moves = 8,
      .move_x = { -3, -2,  2,  3, 3, 2, -2, -3 },
      .move_y = { -2, -3, -3, -2, 2, 3,  3,  2 },
   },
   {
      .name = "Ferz",
      .moves = 4,
      .move_x = { -1,  1, 1, -1 },
      .move_y = { -1, -1, 1,  1 },
   },
};

/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////

// Define the contestants here. These are indices in the contestants[] array.
uint8_t players[] = { 0 };    // 1 knight
//uint8_t players[] = { 0, 0 };    // 2 knights
//uint8_t players[] = { 0, 1, 0 };    // a knight, a leaper and another knight

#define PLAYER_COUNT (sizeof(players) / sizeof(players[0]))

/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////

void calculate_indices()
{
   coord_to_pos = malloc(sizeof(Coords) * WIDTH * HEIGHT);
   pos_to_coord = malloc(sizeof(Coords) * WIDTH * HEIGHT);

   uint16_t x, y;
   x = WIDTH / 2;
   y = HEIGHT / 2;
   for (int i = 0; i < WIDTH * HEIGHT; i++)
   {
      
   }
}

void free_indices()
{
   if (coord_to_pos != NULL)
   {
      free(coord_to_pos);
   }

   if (pos_to_coord != NULL)
   {
      free(pos_to_coord);
   }
}

bool save_image(const char* filename)
{
   printf("Saving %s\n", filename);
   Image out_image = alloc_image(WIDTH, HEIGHT);

   uint8_t r, g, b;
   for (int y = 0; y < HEIGHT; ++y)
   {
      for (int x = 0; x < WIDTH; ++x)
      {
         r = x % 256;
         g = y % 256;
         b = (x + y) * 256;
         set_pixel(out_image, x, y, r, g, b);
      }
   }

   write_png_file(filename, out_image);
   free_image(out_image);
   return true;
}

int main(int argc, char **argv)
{
   srand(time(NULL));

   const char *program = args_shift(&argc, &argv);

   if (argc <= 0)
   {
      fprintf(stderr, "Usage: %s <input.png>\n", program);
      fprintf(stderr, "ERROR: no outpu file is provided\n");
      return 1;
   }

   const char *output_file = args_shift(&argc, &argv);

   calculate_indices();

   if (!save_image(output_file))
   {
      fprintf(stderr, "Could not save image %s.\n", output_file);
      return 1;
   }

   free_indices();

   return 0;
}

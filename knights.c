#include <assert.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "image.h"

#define MAXPLAYERS   20
#define MAXMOVES     8

#define MIN_SIZE 16
#define MAX_SIZE 2048

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

uint16_t width, height;

typedef struct {
   uint16_t x;
   uint16_t y;
} Coord;

// used as a 2D grid. Contains the spiral number.
int32_t* coord_to_pos = NULL;

// a 1D array, indexed by spiral number, contains the grid coords for that spiral number.
Coord* pos_to_coord = NULL;

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

#define CONTESTANT_COUNT (sizeof(contestants) / sizeof(contestants[0]))


// TODO: Vulture, Mantis, Sipius, Xoch

/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////

uint8_t players[MAXPLAYERS] = { 0 };
uint8_t player_count = 0;

/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////

int8_t dx[] = { 1,  0, -1, 0}; 
int8_t dy[] = { 0, -1,  0, 1}; 

#define GIX(x,y) (((y) * width) + (x))

int32_t pos_from_coord(int16_t x, int16_t y)
{
   if (x < 0 || x >= width || y < 0 || y >= height)
   {
      return INT_MIN;
   }

   return coord_to_pos[GIX(x, y)];
}

void calculate_indices()
{
   coord_to_pos = malloc(sizeof(int32_t) * width * height);
   pos_to_coord = malloc(sizeof(Coord) * width * height);

   for (int x = 0; x < width; x++)
   {
      for (int y = 0; y < height; y++)
      {
         coord_to_pos[GIX(x, y)] = INT_MAX;
      }
   }

   for (int i = 0; i < width * height; i++)
   {
      pos_to_coord[i].x = 0;
      pos_to_coord[i].y = 0;
   }

   int32_t n = 0;
   uint8_t dir = 3;
   uint8_t ndir = (dir + 1) % 4;
   int16_t x, y;
   x = width / 2 - 1;
   y = height / 2;
   while (n < width * height)
   {
      coord_to_pos[GIX(x, y)] = n;
      pos_to_coord[n].x = x;
      pos_to_coord[n].y = y;

      int32_t npos = pos_from_coord( x + dx[ndir], y + dy[ndir] );
      if (npos == INT_MIN)
      {
         printf("ERROR: INT_MIN n:%d x:%d y:%d \n", n, x, y);
      }

      bool canturn = n >= 0 && npos == INT_MAX;
      if (canturn)
      {
         dir = ndir;
         ndir = (dir + 1) % 4;
      }

      x += dx[dir];
      y += dy[dir];

      n++;      
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
   Image out_image = alloc_image(width, height);

   uint8_t r, g, b;
   for (int y = 0; y < height; ++y)
   {
      for (int x = 0; x < width; ++x)
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

bool parse_size(const char* str)
{
   width = atoi(str);
   if (width == 0 || width < MIN_SIZE || width > MAX_SIZE)
   {
      fprintf(stderr, "ERROR: Size is out of range.\n");
      return false;
   }

   if (width % 2 != 0)
   {
      fprintf(stderr, "ERROR: Size must be even.\n");
      return false;
   }

   height = width;

   return true;
}

bool check_filename(const char* filename)
{
   int l = strlen(filename);
   const char* ext = filename + l - 4;
   if (strcmp(ext, ".png") != 0 && strcmp(ext, ".PNG") != 0)
   {
      fprintf(stderr, "ERROR: Filename must end in .png\n");
      return false;
   }

   return true;
}

bool parse_players(const char *str)
{
   player_count = 0;
   char temp[1000];
   strncpy(temp, str, 1000);
   temp[1000 - 1] = '\0';
   int l = strlen(temp);
   char* p = temp;
   char* n = p;
   bool done = false;
   while (true)
   {
      while (*n != '-' && *n != 0)
      {
         n++;
      }

      if (*n == 0)
      {
         done = true;
      }
      
      *n = 0;
      bool foundc = false;
      for (int c = 0; c < CONTESTANT_COUNT; c++)
      {
         if (strcmp(p, contestants[c].name) == 0)
         {
            printf("Player %d = %s\n", player_count, p);
            players[player_count++] = c;
            foundc = true;
         }
      }

      if (!foundc)
      {
         fprintf(stderr, "ERROR: Unknown player %s\n", p);
         return false; 
      }

      p = n + 1;
      n = p;

      if (done)
      {
         break;
      }
   }

   return true;
}

void usage(const char* program)
{
   fprintf(stderr, "Usage: %s <players> <size> <filename>\n", program);
   fprintf(stderr, " <players>: a list of players, seperated by a dash.\n");
   fprintf(stderr, "            Maximum = %d\n", MAXPLAYERS);
   fprintf(stderr, "    <size>: The square size of the image to generated, between %d and %d\n", MIN_SIZE, MAX_SIZE);
   fprintf(stderr, "            Must be even.\n");
   fprintf(stderr, "<filename>: The name of the PNG file to save.\n");
   fprintf(stderr, "\n");
   fprintf(stderr, "Examples: %s Knight-Zebra-Leaper 256 out.png\n", program);
   fprintf(stderr, "          %s Ferz-Wazir-Dabbaba-Antilope 1024 out.png\n", program);
   fprintf(stderr, "          %s Knight-Alfil-Knight 2048 out.png\n", program);
}

int main(int argc, char **argv)
{
   srand(time(NULL));

   const char *program = args_shift(&argc, &argv);

   if (argc <= 0)
   {
      usage(program);
      return 1;
   }

   const char *player_list = args_shift(&argc, &argv);

   if (!parse_players(player_list))
   {
      usage(program);
      return 1;
   }

   if (argc <= 0)
   {
      usage(program);
      return 1;
   }

   const char *size_str = args_shift(&argc, &argv);

   if (argc <= 0)
   {
      usage(program);
      return 1;
   }

   if (!parse_size(size_str))
   {
      usage(program);
      return 1;
   }

   const char *output_file = args_shift(&argc, &argv);

   if (!check_filename(output_file))
   {
      usage(program);
      return 1;
   }

   calculate_indices();

   /*
   for (int y = 0; y < height; y++)
   {
      for (int x = 0; x < width; x++)
      {
         printf("%3d ", pos_from_coord(x, y));
      }

      printf("\n");
   }

   printf("----------------\n");

   for (int i = 0; i < width * height; i++)
   {
      printf("%d = (%d, %d) ", i, pos_to_coord[i].x, pos_to_coord[i].y);
   }

   printf("----------------\n");
   */

   /*
   if (!save_image(output_file))
   {
      fprintf(stderr, "Could not save image %s.\n", output_file);
      return 1;
   }
   */

   free_indices();

   return 0;
}

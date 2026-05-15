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

#define MAXOPPONENTS   20
#define MAXMOVES     12

#define MIN_SIZE     16
#define MAX_SIZE     2048
#define NO_PIECE    -1

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
   RED, YELLOW, GREEN, COBALT, EMERALD, TEAL, CYAN, INDIGO, VIOLET, PINK, MAGENTA,
   CRIMSON, ORANGE, AMBER, BROWN, OLIVE, STEEL, MAUVE, LIME, TAUPE
};

#define COLOR_COUNT (sizeof(colors) / sizeof(colors[0]))

uint16_t width, height;

typedef struct {
   uint16_t x;
   uint16_t y;
} Coord;

// Used as a 2D grid. Contains the index of the placed opponent, or -1
int8_t* results = NULL;

// Used as a 2D grid. Contains a bitmask. 
// A 1-bit means that the opponent with that bit-index guards that spot.
uint32_t* guarding = NULL;

// Used as a 2D grid. Contains the spiral number.
int32_t* coord_to_pos = NULL;

// A 1D array, indexed by spiral number. 
// Contains the grid coords for that spiral number.
Coord* pos_to_coord = NULL;

typedef struct {
   const char* name;
   uint8_t moves;
   int8_t move_x[MAXMOVES];
   int8_t move_y[MAXMOVES];
} Piece;

Piece pieces[] = {
   {
      .name = "Knight",    // 1 - 2
      .moves = 8,
      .move_x = { -2, -1,  1,  2, 2, 1, -1, -2 },
      .move_y = { -1, -2, -2, -1, 1, 2,  2,  1 },
   },
   {
      .name = "Dromedary",
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
      .name = "Antilope",     // 3 - 4
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
      .name = "Zebra",     // 2 - 3
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
   {
      .name = "Mantis",    // 1 - 3
      .moves = 8,
      .move_x = { -1,  1,  3, 3, 1, -1, -3, -3 },
      .move_y = { -3, -3, -1, 1, 3,  3,  1, -1 },
   },
   {
      .name = "Sipius",    // 2 - 4
      .moves = 8,
      .move_x = { -2,  2,  4, 4, 2, -2, -4, -4 },
      .move_y = { -4, -4, -2, 2, 4,  4,  2, -2 },
   },
   {
      .name = "Xoch",    // 1 - 2 - 3
      .moves = 12,
      .move_x = { -2, -4, -2,  2,  4,  2,  2, 4, 2, -2, -4, -2 },
      .move_y = { -2, -2, -4, -2, -2, -4,  2, 2, 4,  2,  2,  4 },
   },
};

#define PIECE_COUNT (sizeof(pieces) / sizeof(pieces[0]))

uint8_t opponents[MAXOPPONENTS] = { 0 };
uint8_t opponent_count = 0;

int8_t dx[] = { 1,  0, -1, 0}; 
int8_t dy[] = { 0, -1,  0, 1}; 

#define IX(x,y) (((y) * width) + (x))

int32_t pos_from_coord(int16_t x, int16_t y)
{
   if (x < 0 || x >= width || y < 0 || y >= height)
   {
      return INT_MIN;
   }

   return coord_to_pos[IX(x, y)];
}

void calculate_indices()
{
   coord_to_pos = malloc(sizeof(int32_t) * width * height);
   pos_to_coord = malloc(sizeof(Coord) * width * height);

   for (int x = 0; x < width; x++)
   {
      for (int y = 0; y < height; y++)
      {
         coord_to_pos[IX(x, y)] = INT_MAX;
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
      coord_to_pos[IX(x, y)] = n;
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

   if (guarding != NULL)
   {
      free(guarding);
   }

   if (results != NULL)
   {
      free(results);
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
         int8_t player = results[IX(x, y)];
         if (player != NO_PIECE)
         {
            r = (colors[player] & 0xFF0000) >> 16;
            g = (colors[player] & 0x00FF00) >> 8;
            b = (colors[player] & 0x0000FF);
            set_pixel(out_image, x, y, r, g, b);
         }
         else
         {
            set_pixel(out_image, x, y, 0, 0, 0);
         }
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

bool parse_opponents(const char *str)
{
   opponent_count = 0;
   char temp[1000];
   strncpy(temp, str, 1000);
   temp[1000 - 1] = '\0';
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
      for (int c = 0; c < PIECE_COUNT; c++)
      {
         if (strcmp(p, pieces[c].name) == 0)
         {
            opponents[opponent_count++] = c;
            foundc = true;
         }
      }

      if (!foundc)
      {
         fprintf(stderr, "ERROR: Unknown piece: %s\n", p);
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

bool can_place(uint8_t opponent, uint32_t position)
{
   uint16_t x, y;
   x = pos_to_coord[position].x;
   y = pos_to_coord[position].y;

   // is this position free ?
   if (results[IX(x, y)] != NO_PIECE)
   {
      return false;
   }

   // is it guarded by another player ?
   if ((guarding[IX(x, y)] & ~(1 << opponent)) != 0)
   {
      return false;
   }

   return true;
}

void place(uint8_t opponent, uint32_t position)
{
   uint16_t x, y;
   x = pos_to_coord[position].x;
   y = pos_to_coord[position].y;

   // place the player
   results[IX(x, y)] = opponent;

   // mark all places guarded
   guarding[IX(x, y)] = 0xFFFFFFFF;

   uint8_t piece = opponents[opponent];

   int16_t gx, gy;
   for (int m = 0; m < pieces[piece].moves; m++)
   {
      gx = (int16_t)x + (int16_t)pieces[piece].move_x[m];
      gy = (int16_t)y + (int16_t)pieces[piece].move_y[m];
      if (gx >= 0 && gx < width && gy >= 0 && gy < height)
      {
         guarding[IX(gx, gy)] |= (1 << opponent);
      }
   }
}

void play()
{
   // all opponents start at position 0
   uint32_t positions[MAXOPPONENTS] = { 0 };

   guarding = malloc(width * height * sizeof(uint32_t));

   for (int i = 0; i < width * height; i++)
   {
      results[i] = NO_PIECE;
      guarding[i] = 0;
   }

   bool placed = true;
   while (placed)
   {
      placed = false;
      for (uint8_t current_opponent = 0; current_opponent < opponent_count; current_opponent++)
      {
         while (positions[current_opponent] < width * height && 
                !can_place(current_opponent, positions[current_opponent]))
         {
            positions[current_opponent]++;
         }

         if (positions[current_opponent] < width * height)
         {
            place(current_opponent, positions[current_opponent]);
            positions[current_opponent]++;
            placed = true;
         }
      }
   }
}

void print_summary(const char* filename)
{
   printf("%d x %d = %d\n", width, height, width * height);
   printf("%s\n", filename);
   printf("Opponents: ");
   for (int p = 0; p < opponent_count; p++)
   {
      if (p > 0) printf(", ");
      printf("%s", pieces[opponents[p]].name);
   }

   printf("\n");
}

void usage(const char* program)
{
   fprintf(stderr, "Usage: %s <opponents> <size> <filename>\n", program);
   fprintf(stderr, "<opponents>: a list of opponents, seperated by a dash.\n");
   fprintf(stderr, "             Maximum = %d\n", MAXOPPONENTS);
   fprintf(stderr, "     <size>: The square size of the image to generated, between %d and %d\n", MIN_SIZE, MAX_SIZE);
   fprintf(stderr, "             Must be even.\n");
   fprintf(stderr, " <filename>: The name of the PNG file to save.\n");
   fprintf(stderr, "\n");
   fprintf(stderr, "Supported opponents:\n");
   for (int p = 0; p < PIECE_COUNT; p++)
   {
      if (p > 0) fprintf(stderr, ", ");
      fprintf(stderr, "%s", pieces[p].name);
   }

   fprintf(stderr, "\n\n");
   fprintf(stderr, "Examples: %s Knight-Zebra-Dromedary 256 out.png\n", program);
   fprintf(stderr, "          %s Ferz-Wazir-Dabbaba-Antilope 1024 out.png\n", program);
   fprintf(stderr, "          %s Knight-Alfil-Knight 2048 out.png\n", program);
}

int main(int argc, char **argv)
{
   const char *program = args_shift(&argc, &argv);

   if (argc <= 0)
   {
      usage(program);
      return 1;
   }

   const char *opponent_list = args_shift(&argc, &argv);

   if (!parse_opponents(opponent_list))
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
   
   print_summary(output_file);

   results = malloc(width * height * sizeof(int8_t));

   play();

   if (!save_image(output_file))
   {
      fprintf(stderr, "Could not save image %s.\n", output_file);
      return 1;
   }

   free_indices();

   return 0;
}

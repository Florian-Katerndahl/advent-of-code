#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define LINE_BUFFER 30

enum {
    POINTS_ROCK = 1,
    POINTS_PAPER = 2,
    POINTS_SCISSORS = 3,
    POINTS_LOSS = 0,
    POINTS_WIN = 6,
    POINTS_DRAW = 3
};

int decode_elf_move(const char *const a);

int decode_outcome(const char *const a);

int main(int argc, char *argv[]) {
  assert(argc == 2);

  FILE *f = fopen(*(argv + 1), "rt");
  if (f == NULL) {
    fprintf(stderr, "Error, could not open file %s\n", *(argv + 1));
    exit(1);
  }

  char line_buffer[LINE_BUFFER];
  char move_elf, move_self;
  unsigned long game_points = 0;
  const int game_matrix[3][3] = {
          {POINTS_SCISSORS + POINTS_LOSS, POINTS_ROCK + POINTS_LOSS,    POINTS_PAPER + POINTS_LOSS},
          {POINTS_ROCK + POINTS_DRAW,     POINTS_PAPER + POINTS_DRAW,   POINTS_SCISSORS + POINTS_DRAW},
          {POINTS_PAPER + POINTS_WIN,     POINTS_SCISSORS + POINTS_WIN, POINTS_ROCK + POINTS_WIN}
  };

  while (fgets(line_buffer, LINE_BUFFER, f) != NULL &&
         sscanf(line_buffer, "%c %c", &move_elf, &move_self) == 2) {
    game_points += game_matrix[decode_outcome(&move_self)][decode_elf_move(&move_elf)];

    if (feof(f))
      break;
  }

  printf("Following the great secret strategy, I end up with %ld points.\n", game_points);

  fclose(f);
  return 0;
}

int decode_elf_move(const char *const a) {
  switch (*a) {
    case 'A':
      return 0;
    case 'B':
      return 1;
    case 'C':
      return 2;
    default:
      return -1;
  }
}

int decode_outcome(const char *const a) {
  switch (*a) {
    case 'X': // lose
      return 0;
    case 'Y': // draw
      return 1;
    case 'Z': // win
      return 2;
    default:
      return -1;
  }
}

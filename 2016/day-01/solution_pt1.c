#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

struct vec_2d {
    int x;
    int y;
};

int absolute_difference(int a, int b);
int manhattan_distance(const struct vec_2d *a, const struct vec_2d *b);
void rotate_vector(const int rot_mat[4], struct vec_2d *a);

int main(int argc, char *argv[]) {
  assert(argc == 2);

  char rotation;
  int blocks;

  int rotate_clockwise[4] = {0, 1, -1, 0};
  int rotate_counter_clockwise[4] = {0, -1, 1, 0};

  struct vec_2d start_position = {.x = 0, .y = 0};
  struct vec_2d directions_vector = {.x = 0, .y = 0};

  FILE *directions = fopen(*(argv + 1), "rt");
  if (directions == NULL) {
    fprintf(stderr, "Could not open %s\n", *(argv + 1));
    exit(-1);
  }

  while (fscanf(directions, "%c%d, ", &rotation, &blocks) == 2) {
    if (rotation == 'L') {
      rotate_vector(rotate_counter_clockwise, &directions_vector);
      directions_vector.x += blocks;
    } else {
      rotate_vector(rotate_clockwise, &directions_vector);
      directions_vector.x += blocks;
    }
  }

  int distance_to_hq = manhattan_distance(&start_position, &directions_vector);

  printf("Distance to Easter-Bunny-HQ: %d\n", distance_to_hq);

  return 1;
}

int absolute_difference(int a, int b) {
  int diff = a - b;

  return (diff < 0) ? -1 * diff : diff;
}

int manhattan_distance(const struct vec_2d *a, const struct vec_2d *b) {
  return absolute_difference(b->x, a->x) + absolute_difference(b->y, a->y);
}

void rotate_vector(const int rot_mat[4], struct vec_2d *a) {
  int x = a->x, y = a->y;
  a->x = *rot_mat * x + *(rot_mat + 1) * y;
  a->y = *(rot_mat + 2) * x + *(rot_mat + 3) * y;
}

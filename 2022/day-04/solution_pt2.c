#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#define LINE 100

struct Elf {
    int start_id;
    int end_id;
};

struct Pair {
    struct Elf first;
    struct Elf second;
};

bool are_sections_disjoint(struct Pair *pair);

int main(int argc, char *argv[]) {
  assert(argc == 2);

  char line[LINE];
  struct Pair pair_of_elves;
  unsigned long complete_overlap = 0;

  FILE *f = fopen(*(argv + 1), "rt");

  if (f == NULL) {
    fprintf(stderr, "Failed to open file %s\n", *(argv + 1));
    exit(1);
  }

  while (!feof(f)) {
    if (fgets(line, LINE, f) == NULL)
      break;

    if (sscanf(line, "%d-%d,%d-%d\n", &pair_of_elves.first.start_id, &pair_of_elves.first.end_id,
               &pair_of_elves.second.start_id, &pair_of_elves.second.end_id) != 4)
      break;

    if (!are_sections_disjoint(&pair_of_elves))
      complete_overlap++;
  }

  printf("Number of pairs of elves where one assigned range overlaps with the other: %ld\n",
         complete_overlap);

    fclose(f);

  return 0;
}

bool are_sections_disjoint(struct Pair *pair) {
  return (unsigned long) ((pair->first.start_id < pair->second.start_id &&
                           pair->first.start_id < pair->second.end_id &&
                           pair->first.end_id < pair->second.start_id &&
                           pair->first.end_id < pair->second.end_id) ||
                          (pair->second.start_id < pair->first.start_id &&
                           pair->second.start_id < pair->first.end_id &&
                           pair->second.end_id < pair->first.start_id &&
                           pair->second.end_id < pair->first.end_id));
}

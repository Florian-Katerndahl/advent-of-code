#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define ARMY_OF_ELFS 1000

struct elf {
    unsigned long calories_carried;
};

void sort_elfs(struct elf *a, size_t arr_size);

int main(int argc, char *argv[]) {
  assert(argc == 2);

  FILE *f = fopen(*(argv + 1), "rt");
  if (f == NULL) {
    fprintf(stderr, "Could not open file %s\n", *(argv + 1));
    exit(1);
  }

  struct elf elfs[ARMY_OF_ELFS] = { 0 };
  char calories_as_string[30];
  size_t actual_size_of_army = 0;

  for (int i = 0; i < ARMY_OF_ELFS; ++i) {
    unsigned long calories = 0;
    while(fgets(calories_as_string, sizeof(calories_as_string), f) != NULL) {
      if (strcmp(calories_as_string, "\n") == 0)
        break;
      sscanf(calories_as_string, "%ld", &calories);
      elfs[i].calories_carried += calories;
    }
    actual_size_of_army +=1;

    if (feof(f))
      break;
  }

  sort_elfs(elfs, actual_size_of_army);

  // adjust for indices starting at 0
  printf("Elf with the most magical snacks carries %ld calories\n", elfs[actual_size_of_army - 1].calories_carried);

  fclose(f);
  return 0;
}

void sort_elfs(struct elf *a, size_t arr_size) {
  struct elf temporary_elf;

  for(size_t i = 0; i < arr_size - 1; i++) {
    for(size_t j = 0; j < arr_size - i - 1; j++) {
      if (a[j].calories_carried > a[j + 1].calories_carried) {
        temporary_elf = a[j];
        a[j] = a[j + 1];
        a[j + 1] = temporary_elf;
      }
    }
  }
}
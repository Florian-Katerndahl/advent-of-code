/*
 * Hash table filtering >= 3 for common items
 * struct of three elves for company
 * work with single struct instance, no need for an array
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define SQUAD_SIZE 3
#define ASCII_LENGTH 128
#define MAX_LINE 1024

struct Squad {
    char *elfs[SQUAD_SIZE];
    char *duplicates;
};

void initialize_array(struct Squad *squad, size_t squad_members);

void find_common_item(struct Squad *squad, size_t squad_members);

unsigned long sum_up_priorities(struct Squad *squad);

void zero_out_arrays(struct Squad *squad, size_t length, size_t members);

void free_malloced_squad(struct Squad *squad, size_t squad_members);

int main(int argc, char *argv[]) {
  assert(argc == 2);

  FILE *f = fopen(*(argv + 1), "rt");
  if (f == NULL) {
    fprintf(stderr, "Failed to open file %s\n", *(argv + 1));
    exit(1);
  }

  struct Squad squad = {
          .elfs = { 0 },
          .duplicates = calloc(MAX_LINE, sizeof(char))
  };

  char line[MAX_LINE] = {0};
  unsigned long summed_priorities = 0;

  initialize_array(&squad, SQUAD_SIZE);

  while (1) {
    for (size_t i = 0; i < SQUAD_SIZE; i++) {
      if (fgets(line, MAX_LINE, f) != NULL) {
        line[strlen(line) - 1] = '\0';
        strcpy(squad.elfs[i], line);
      }

      memset(line, 0, MAX_LINE);
    }

    if (feof(f))
      break;

    find_common_item(&squad, SQUAD_SIZE);
    summed_priorities += sum_up_priorities(&squad);

    zero_out_arrays(&squad, MAX_LINE, SQUAD_SIZE);
  }

  printf("Summed priorities over all squads: %ld\n", summed_priorities);

  free_malloced_squad(&squad, SQUAD_SIZE);
  fclose(f);

  return 0;
}

void initialize_array(struct Squad *squad, size_t squad_members) {
  for (size_t i = 0; i < squad_members; i++) {
    squad->elfs[i] = calloc(MAX_LINE, sizeof(char));
  }
}

void find_common_item(struct Squad *squad, size_t squad_members) {
  /*
   * return of first item present in all members!
   */
  int char_counts[SQUAD_SIZE][ASCII_LENGTH] = { 0 };
  for (size_t i = 0; i < squad_members; i++) {
    char *char_p = squad->elfs[i];
    while (*char_p) {
      char_counts[i][(int) *char_p]++;
      char_p++;
    }
  }

  for (size_t j = 0; j < ASCII_LENGTH; j++) {
    int identical_item = 0;
    for (size_t i = 0; i < squad_members; i++) {
      if (char_counts[i][j] > 0) {
        identical_item++;
      }
    }

    if (identical_item == (int) squad_members) {
      squad->duplicates[0] = (char) j;
      return;
    }
  }
}

unsigned long sum_up_priorities(struct Squad *squad) {
  unsigned long sum = 0;
  char *duplicates_p = squad->duplicates;

  while (*duplicates_p) {
    if (*duplicates_p > 64 && *duplicates_p < 91)
      sum += *duplicates_p - 38;
    else if (*duplicates_p > 96 && *duplicates_p < 123)
      sum += *duplicates_p - 96;
    else
      sum *= 0;

    duplicates_p++;
  }

  return sum;
}

void zero_out_arrays(struct Squad *squad, size_t length, size_t members) {
  for (size_t i = 0; i < members; ++i) {
    memset(squad->elfs[i], 0, length);
  }
  memset(squad->duplicates, 0, length);
}

void free_malloced_squad(struct Squad *squad, size_t squad_members) {
  for (size_t i = 0; i < squad_members; i++) {
    free(squad->elfs[i]);
  }

  free(squad->duplicates);
}

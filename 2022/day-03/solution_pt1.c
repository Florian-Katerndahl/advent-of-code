#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define ASCII_LENGTH 128
#define MAX_LINE 1024
#define HALF_SIZE(A) ((A / 2) + 1)

struct Rucksack {
    char *first_compartment;
    char *second_compartment;
    char *duplicates;
};

void split_items_into_compartment(struct Rucksack *rucksack, char *items);

void find_duplicate_items(struct Rucksack *rucksack);

void unique_duplicate_items(struct Rucksack *rucksack);

void str_shift_left(char *start, unsigned int by);

unsigned long sum_up_priorities(struct Rucksack *rucksack);

void zero_out_arrays(struct Rucksack *rucksack, size_t length);

void free_malloced_rucksack(struct Rucksack *rucksack);

int main(int argc, char *argv[]) {
  assert(argc == 2);

  FILE *f = fopen(*(argv + 1), "rt");
  if (f == NULL) {
    fprintf(stderr, "Failed to open file %s\n", *(argv + 1));
    exit(1);
  }

  struct Rucksack rucksack = {
          .first_compartment = calloc(HALF_SIZE(MAX_LINE), sizeof(char)),
          .second_compartment = calloc(HALF_SIZE(MAX_LINE), sizeof(char)),
          .duplicates = calloc(HALF_SIZE(MAX_LINE), sizeof(char))
  };
  char line[MAX_LINE] = {0};
  unsigned long summed_priorities = 0;

  while (fgets(line, MAX_LINE, f) != NULL) {
    line[strlen(line) - 1] = '\0';

    split_items_into_compartment(&rucksack, line);
    find_duplicate_items(&rucksack);
    unique_duplicate_items(&rucksack);
    summed_priorities += sum_up_priorities(&rucksack);

    zero_out_arrays(&rucksack,HALF_SIZE(MAX_LINE));
  }

  printf("Summed priorities over all rucksacks: %ld\n", summed_priorities);

  free_malloced_rucksack(&rucksack);
  fclose(f);

  return 0;
}

void split_items_into_compartment(struct Rucksack *rucksack, char *items) {
  int number_of_items = strlen(items);
  assert(number_of_items % 2 == 0);

  int compartment_size = number_of_items / 2;
  char *start_of_second_compartment = items + compartment_size;

  strncpy(rucksack->first_compartment, items, compartment_size);
  rucksack->first_compartment[compartment_size] = '\0';

  strncpy(rucksack->second_compartment, start_of_second_compartment, compartment_size);
  rucksack->second_compartment[compartment_size] = '\0';
}

void find_duplicate_items(struct Rucksack *rucksack) {
  size_t duplicate_entry = 0;
  char *needle = rucksack->second_compartment;
  char *needle_position;
  while (*needle) {
    if ((needle_position = strchr(rucksack->first_compartment, *needle)) != NULL) {
      rucksack->duplicates[duplicate_entry] = *needle_position;
      duplicate_entry++;
    }
    needle++;
  }
}

void unique_duplicate_items(struct Rucksack *rucksack) {
  int char_counts[ASCII_LENGTH] = { 0 };
  char *char_p = rucksack->duplicates;
  int iterations = 0;
  while (*char_p) {
    if (char_counts[(int) *char_p] == 0) {
      char_counts[(int) *char_p]++;
    }
    else {
      // shift all following character to the left by one, as if all _string safety measures_ had
      // not been ignored enough
      str_shift_left(char_p, 1);
      char_p--;
    }
    char_p++;
    iterations++;
  }
}

void str_shift_left(char *start, unsigned int by) {
  char *curr_pos = start;
  char temp;

  if (*curr_pos){
    temp = *curr_pos;
    *(curr_pos - by) = temp;
    curr_pos++;
    str_shift_left(curr_pos, by);
  } else {
    temp = *curr_pos;
    *(curr_pos - by) = temp;
  }
}

unsigned long sum_up_priorities(struct Rucksack *rucksack) {
  unsigned long sum = 0;
  char *duplicates_p = rucksack->duplicates;

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

void zero_out_arrays(struct Rucksack *rucksack, size_t length) {
  memset(rucksack->first_compartment, 0, length);
  memset(rucksack->second_compartment, 0, length);
  memset(rucksack->duplicates, 0, length);
}

void free_malloced_rucksack(struct Rucksack *rucksack) {
  free(rucksack->first_compartment);
  free(rucksack->second_compartment);
  free(rucksack->duplicates);
}

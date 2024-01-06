#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

struct Card {
	size_t id;
	int *my_nums;
	int *winning_nums;
	size_t mn_len;
	size_t wn_len;
};

void insertion_sort(int *arr, size_t size);
int binary_s(int *arr, size_t size, int val);
void part_1(FILE *fp);
void part_2(FILE *fp);

int main(void) {
	FILE *fp = fopen("input-04.txt", "rt");
	if (fp == NULL) {
		fprintf(stderr, "ERROR: Unable to open input file\n");
		exit(1);
	}

	part_1(fp);
	rewind(fp);
	part_2(fp);

	fclose(fp);
	return 0;
}

void insertion_sort(int *arr, size_t size) {
	size_t sorted_before = 0;
	int	smallest_value, smallest_value_idx, temp;
	while (sorted_before < size - 1) {
		for (size_t i = sorted_before; i < size; i++) {
			if (i == sorted_before || smallest_value > arr[i]) {
				smallest_value = arr[i];
				smallest_value_idx = i;
			}
		}
		temp = arr[sorted_before];
		arr[sorted_before] = smallest_value;
		arr[smallest_value_idx] = temp;
		sorted_before++;
	}
}


int binary_s(int *arr, size_t size, int val) {
	int *midpoint;
	int *left = arr;
	int *right = arr + (size - 1);
	while (left <= right) {
		midpoint = left + ((right - left) / 2);
		if (val == *midpoint) return 1;
		if (val > *midpoint) left = midpoint + 1;
		if (val < *midpoint) right = midpoint - 1;

	}

	return 0;
}

void part_1(FILE *fp) {
	unsigned long score, total_score = 0;
	char line[1024];
	char *p, *endptr;
	int game_id, score_streak;
	while (fgets(line, 1024, fp)) {
		int winning_numbers[1024];
		int my_numbers[1024];
		int wn = 0;
		int mn = 0;

		score_streak = 0;
		score = 0;

		if (line[0] == '\n')
			continue;
		
		char *first_ws = index(line, ' '); first_ws++;
		game_id	= atoi(first_ws);
		
		p = index(line, ':'); p++;
		endptr = p;

		char *separator = index(line, '|');

		while (endptr + 1 < separator) {
			// error checking omitted!
			winning_numbers[wn] = (int) strtol(p, &endptr, 10);
			p = endptr;
			wn++;
		}
		
		p += 2; // step-over '|'
		
		while (*endptr && *endptr != '\n') {
			// error checking omitted!
			my_numbers[mn] = (int) strtol(p, &endptr, 10);
			p = endptr;
			mn++;
		}

		insertion_sort(winning_numbers, (size_t) wn);

		for (size_t i = 0; i < (size_t) mn; i++) {
			if (binary_s(winning_numbers, wn, my_numbers[i])) {
				score = (unsigned long) powl(2, score_streak);
				score_streak++;
			}
		}

		total_score += score;
	}

	printf("Total score: %ld\n", total_score);
}

void part_2(FILE *fp) {
	unsigned long score, total_cards = 0;
	char line[1024];
	char *p, *endptr;
	int game_id, winning_nums_current_card;
	int card_counts[1024];
	while (fgets(line, 1024, fp)) {
		int winning_numbers[1024];
		int my_numbers[1024];
		int wn = 0;
		int mn = 0;
		int counted_self = 0;
		winning_nums_current_card = 0;

		if (line[0] == '\n')
			continue;
		
		char *first_ws = index(line, ' '); first_ws++;
		game_id	= atoi(first_ws);
		
		p = index(line, ':'); p++;
		endptr = p;

		char *separator = index(line, '|');

		while (endptr + 1 < separator) {
			// error checking omitted!
			winning_numbers[wn] = (int) strtol(p, &endptr, 10);
			p = endptr;
			wn++;
		}
		
		p += 2; // step-over '|'
		
		while (*endptr && *endptr != '\n') {
			// error checking omitted!
			my_numbers[mn] = (int) strtol(p, &endptr, 10);
			p = endptr;
			mn++;
		}

		insertion_sort(winning_numbers, (size_t) wn);
		
		// no matter if the card is a winning card:
		card_counts[game_id - 1]++;
		for (size_t i = 0; i < (size_t) mn; i++) {
			if (binary_s(winning_numbers, wn, my_numbers[i])) {
				winning_nums_current_card++;
			}
		}
		
		for (size_t j = 0; j < card_counts[game_id - 1]; j++){	
			for (size_t i = game_id - 1; i < game_id - 1 + winning_nums_current_card; i++)
			   	card_counts[i + 1]++;
		}
	}

	for (size_t i = 0; i < game_id; i++) {
		total_cards += card_counts[i];
	}
	printf("Total cards: %ld\n", total_cards);
}

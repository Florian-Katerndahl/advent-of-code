#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

int char_to_int(int arr[2]) {
	return (arr[0] - '0') * 10 + (arr[1] - '0');
}

unsigned long part1(FILE *f) {
	char line[1024];
	char *p = line;
	int number[2];
	unsigned long result = 0;
	int first,last;
	while (fgets(line, 1024, f)) {
		number[0] = number[1] = 0;
		size_t length = strlen(line);
		for (size_t i = 0; i < length; i++) {
			if (isdigit(line[i])) {
				number[0] = line[i];
				break;
			}
		}

		for (size_t i = length - 1; i >= 0; i--) {
			if (isdigit(line[i])) {
				number[1] = line[i];
				break;
			}
		}
		result += number[0] && number[1] ? (unsigned long) char_to_int(number) : 0;
	}

	return result;
}

unsigned long part2(FILE *f) {
	char line[1024];
	char *p = line;
	int number[2];
	unsigned long result = 0;
	int first,last;
	char digits[9][10] = {
		"one", "two", "three", "four", "five", "six", "seven", "eight", "nine"
	};
	char digits2[9] = {'1','2','3','4','5','6','7','8','9'};

	while (fgets(line, 1024, f)) {
		number[0] = number[1] = 0;
		size_t length = strlen(line);
		char *pp;
		for (int i = 0; i < 9; i++) {
			while ((pp = strstr(line, digits[i]))) {
				*(pp+1) = digits2[i]; // stupid overlapping numbers
			}
		}
		for (size_t i = 0; i < length; i++) {
			if (isdigit(line[i])) {
				number[0] = line[i];
				break;
			}
		}

		for (size_t i = length - 1; i >= 0; i--) {
			if (isdigit(line[i])) {
				number[1] = line[i];
				break;
			}
		}
		result += number[0] && number[1] ? (unsigned long) char_to_int(number) : 0;
	}

	return result;
}

int main(int argc, char **argv) {
	if (argc < 2) {
		fprintf(stderr, "ERROR: Expected file path as second argument\n");
		exit(1);
	}

	FILE *fp = fopen(*(argv + 1), "rt");
	if (fp == NULL) {
		fprintf(stderr, "ERROR: Failed to open file %s\n", *(argv+1));
		exit(1);
	}

	printf("Result to puzzle 1: %ld\n", part1(fp));
	rewind(fp);
	printf("Result to puzzle 2: %ld\n", part2(fp));

	fclose(fp);
	return 0;
}

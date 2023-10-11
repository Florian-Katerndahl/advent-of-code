#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main(int argc, char **argv) {
	if (argc < 2) {
		fprintf(stderr, "ERROR: Missing input file\n");
		return 1;
	}

	FILE *fp = fopen(*(argv + 1), "rt");
	if (fp == NULL) {
		fprintf(stderr, "ERROR: Could not open file %s\n", *(argv + 1));
		return 1;
	}

	// assumes all values are positive
	size_t max;
	size_t min;
	size_t temp;
	char line[2048];
	char *lp;
	size_t checksum = 0;

	while (fgets(line, 2048, fp)) {
		lp = &line[0];
		max = 0;
		min = SIZE_MAX;
		while (*lp != '\0' && *lp != '\n') {
			temp = atol(lp);
			if (temp > max)
				max = temp;
			if (temp < min)
				min = temp;
			while (*lp >= 48 && *lp <= 57) lp++;
			while (*lp == ' ' || *lp == '\t') lp++;
		}
		checksum += (max - min);
	}

	printf("Result: %ld\n", checksum);	

	fclose(fp);

	return 0;
}


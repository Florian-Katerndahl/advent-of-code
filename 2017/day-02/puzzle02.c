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
	char line[2048];
	size_t converted_nums[2048];
	size_t nNums;
	char *lp;
	size_t checksum = 0;

	while (fgets(line, 2048, fp)) {
		nNums = 0;
		lp = &line[0];
		while (*lp != '\0' && *lp != '\n') {
			converted_nums[nNums] = atol(lp);
			nNums++;
			while (*lp >= 48 && *lp <= 57) lp++;
			while (*lp == ' ' || *lp == '\t') lp++;
		}
		for (size_t i = 0; i < nNums - 1; i++) {
			for (size_t j = i + 1; j < nNums; j++) {
				checksum += converted_nums[i] % converted_nums[j] ? 0 : converted_nums[i] / converted_nums[j];
				checksum += converted_nums[j] % converted_nums[i] ? 0 : converted_nums[j] / converted_nums[i];
			}
		}
	}

	printf("Result: %ld\n", checksum);	

	fclose(fp);

	return 0;
}


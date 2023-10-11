#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// discarding ring buffer via struct
struct cBuff {
	int start;
	int val;
	struct cBuff *next;
};

int main(int argc, char **argv) {
	if (argc < 2) {
		fprintf(stderr, "ERROR: Failed to specify file\n");
		return 1;
	}
	
	FILE *fp = fopen(*(argv + 1), "rt");
	if (fp == NULL) {
		fprintf(stderr, "ERROR: Failed to open file\n");
		return 1;
	}

	char c;	
	char *st = malloc(2048 * sizeof(uint8_t));
	size_t nElements = 2048;
	size_t nDigits = 0;
	size_t sum = 0;

	while((c = fgetc(fp)) != EOF && c > 47 && c < 58) {
		if ((nDigits - 1) >= nElements) {
			nElements *= 2;
			st = reallocarray(st, nElements, sizeof(int));
		}
		*(st + nDigits) = c - '0';
		nDigits++;
	}
	//printf("%c\n", c);

	for (size_t i = 0; i <= nDigits; i++) {
		if (*(st + i) == *(st + ((i + 1) % nDigits)))
		   sum += *(st + i);	
	}

	printf("Result: %ld\n", sum);

	fclose(fp);
	free(st);

	return 0;
}

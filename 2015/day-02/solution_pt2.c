#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define CMIN(A,B,C) ((A < B) ? (A < C) ? A : C : (B < C) ? B : C)

void sort(int *arr, int arr_length);

int main(int argc, char *argv[]) {
	assert(argc == 2);

	int nread, conversions, sides[3];
	size_t string_size = 13;
	unsigned total = 0;
	char dimensions[13];
	char *dimensions_p = dimensions;

	FILE *input = fopen(*(argv + 1), "rt");
	if (input == NULL) {
		fprintf(stderr, "Could not open file %s\n", *(argv + 1));
		exit(1);
	}

	while ((nread = getline(&dimensions_p, &string_size, input)) != -1) {
		if ((conversions = sscanf(dimensions_p, "%dx%dx%d\n", &sides[0], &sides[1], &sides[2])) != 3 || conversions == EOF) break;
		
		sort(sides, 3);

		total += ((2 * sides[0] + 2 * sides[1]) + (sides[0] * sides[1] * sides[2]));
	}
	printf("The elfs need %u feet of ribbon\n", total);

	fclose(input);
	return 0;
}

void sort(int *arr, int arr_length) {
    int temp;
    for (int i = 0; i < arr_length - 1; i++) {
        for (int j = 0; j < arr_length - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
			}
		}
	}
}

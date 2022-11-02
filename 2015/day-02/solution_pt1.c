#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define CMIN(A,B,C) ((A < B) ? (A < C) ? A : C : (B < C) ? B : C)

int main(int argc, char *argv[]) {
	assert(argc == 2);

	int nread, length, width, height, face_1, face_2, face_3, conversions;
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
		if ((conversions = sscanf(dimensions_p, "%dx%dx%d\n", &length, &width, &height)) != 3 || conversions == EOF) break;
		face_1 = length * width;
		face_2 = width * height;
		face_3 = height * length;
		total += ((2 * face_1 + 2 * face_2 + 2 * face_3) + CMIN(face_1, face_2, face_3));
		//printf("%dx%dx%d = %d\n", length, height, width, (2 * face_1 + 2 * face_2 + 2 * face_3 ) + CMIN(face_1, face_2, face_3));
	}
	printf("The elfs need %u square feet of wrapping paper\n", total);

	fclose(input);
	return 0;
}


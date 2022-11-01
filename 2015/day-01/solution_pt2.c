#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

void read_directions_from_file(FILE *file, char **buff, size_t *inital_buffer);
int parse_directions(char *direction);

int main(int argc, char *argv[]) {

	assert(argc == 2);

	size_t buffer_size = 2000;

	char *directions = calloc(buffer_size, sizeof(char));

	FILE *f_input = fopen(*(argv + 1), "rt");

	if (f_input == NULL) {
		fprintf(stderr, "Could not open %s\n", *(argv + 1));
		exit(1);
	}

	read_directions_from_file(f_input, &directions, &buffer_size);

	int instruction = parse_directions(directions);

	printf("Santa ends up on floor -1 at the %dth instruction\n", instruction);

	fclose(f_input);
	free(directions);
	return 0;
}

void read_directions_from_file(FILE *file, char **buff, size_t *inital_buffer) {
	int nread = 0;
	if (( nread = getline(buff, inital_buffer, file)) == -1) {
		fprintf(stderr, "Failed to read string");
		exit(1);
	}

	(*buff)[*inital_buffer - 1] = '\0';
}

int parse_directions(char *direction) {
	int starting_point = 0;
	char *p = direction;
	while (*p && starting_point >= 0) {
		switch (*p) {
			case '(':
				starting_point++;
				break;
			case ')':
				starting_point--;
				break;
			default:
				break;
		}
		p++;
	}
	return (int)(p - direction);
}

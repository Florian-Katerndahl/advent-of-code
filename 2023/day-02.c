#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define LEN 1024

struct Cubes {
	int r;
	int g;
	int b;
};

struct Game {
	size_t id;
	size_t len;
	struct Cubes cubes[LEN];
};

struct Game parse_game_line(char *l);
size_t part1(const struct Game Game, const int allowed_r, const int allowed_g, const int allowed_b);
size_t part2(const struct Game Game);
char *move_to_next_digit(char *l);
char *move_to_next_char(char *l);
char *move_to_next_punct(char *l);

int main(void) {
	size_t puzzle1 = 0, puzzle2 = 0;
	const int r = 12;
	const int g = 13;
	const int b = 14;
	char line [LEN];

	FILE *fp = fopen("input-02.txt", "rt");
	if (fp == NULL) {
		fprintf(stderr, "ERROR: Failed to open input file\n");
		exit(1);
	}

	while (fgets(line, LEN, fp)) {
		if (line[0] == '\n') break;
		struct Game Game = parse_game_line(line);
		puzzle1 += part1(Game, r, g, b);
		puzzle2 += part2(Game);
	}
	
	printf("Solution to puzzle 1: %ld\n", puzzle1);
	printf("Solution to puzzle 2: %ld\n", puzzle2);

	fclose(fp);
	return 0;
}

struct Game parse_game_line(char *l) {
	struct Game Game = {0}; // zero initialization fucked me over hard
	char *p = move_to_next_digit(l);
	size_t idx = 0;
	int tmp;
	Game.id = atoi(p);
	p = move_to_next_punct(p); // move pointer past parsed number
	
	while (*p) {
		p = move_to_next_digit(p);
		tmp = atoi(p);
		p = move_to_next_char(p);
		switch(*p) {
				case 'r':
					Game.cubes[idx].r = tmp;
					break;
				case 'g':
					Game.cubes[idx].g = tmp;
					break;
				case 'b':
					Game.cubes[idx].b = tmp;
					break;
				default:
					break;
		}
		p = move_to_next_punct(p);

		if (*p == ';') idx++;
	}

	Game.len = idx + 1;

	return Game;
}

size_t part1(struct Game Game, const int allowed_r, const int allowed_g, const int allowed_b) {
	for (size_t i = 0; i < Game.len; i++) {
		if (Game.cubes[i].r > allowed_r 
				|| Game.cubes[i].g > allowed_g 
				|| Game.cubes[i].b > allowed_b) {
			return 0;
		}
	}

	return Game.id;
}

size_t part2(struct Game Game) {
	int min_r = 1, min_g = 1, min_b = 1;
	for (size_t i = 0; i < Game.len; i++) {
		if (Game.cubes[i].r > min_r) min_r = Game.cubes[i].r;
		if (Game.cubes[i].g > min_g) min_g = Game.cubes[i].g;
		if (Game.cubes[i].b > min_b) min_b = Game.cubes[i].b;
	}
	return min_r * min_g * min_b;
}

char *move_to_next_digit(char *l) {
	while (*l && !isdigit(*l)) l++;
	return l;
}

char *move_to_next_char(char *l) {
	while (*l && !isalpha(*l)) l++;
	return l;
}

char *move_to_next_punct(char *l) {
	while (*l && !ispunct(*l)) l++;
	return l;
}

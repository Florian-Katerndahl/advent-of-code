#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define LLENGTH 1024

struct Board {
	size_t rows;
	size_t cols;
	char *board;
};

struct Board parse_board(FILE *f);
void print_board(const struct Board Board);
struct Board remove_solitaire_numbers(struct Board Board);
struct Board keep_only_double_gears(struct Board Board);
int issymbol(int c);
int isgear(int c);
unsigned long part1(const struct Board *Board);
unsigned long part2(const struct Board *Board);

int main(void) {
	FILE *fp = fopen("input-03.txt", "rt");
	if (fp == NULL) {
		fprintf(stderr, "ERROR: Failed to open input\n");
		exit(1);
	}

	struct Board board = parse_board(fp);
	struct Board board1 = remove_solitaire_numbers(board);
	//struct Board board2 = keep_only_double_gears(board);

	printf("Solution 1: %ld\n", part1(&board1));
	//printf("Solution 2: %ld\n", part2(&board2));

	fclose(fp);
	free(board.board);
	return 0;
}

struct Board parse_board(FILE *f) {
	struct Board board = {0};
	int c;
	char line[LLENGTH];

	while ((c = fgetc(f)) != EOF && c != '\n') board.cols++;
	rewind(f);
	while (fgets(line, LLENGTH, f) && line[0] != '\n') board.rows++;
	rewind(f);
	// add padding, so sliding window below is easier
	// with this, the bounds are 1 to n - 2 (inclusive)
	board.rows += 2;
	board.cols += 2;
	board.board = calloc(board.rows * board.cols, sizeof(char));
	
	for (size_t row = 1; row < board.rows - 1; row++) {
		for (size_t col = 1; col < board.cols - 1; col++) {
			if ((c = fgetc(f)) == EOF) break;
			board.board[row * board.cols + col] = c;
		}
		// discard newline
		c = fgetc(f);
	}

	return board;
}

void print_board(struct Board board) {
	for (size_t row = 1; row < board.rows - 1; row++) {
		for (size_t col = 1; col < board.cols - 1; col++) {
			putc(board.board[row * board.cols + col], stdout);
		}
		putc('\n', stdout);
	}
}

struct Board remove_solitaire_numbers(struct Board Board) {
	char *start, *end;
	int adjacent;
	size_t subcol;
	int ul, um, ur, ml, mr, ll, lm, lr;
	for (size_t row = 1; row < Board.rows - 1; row++) {
		for (size_t col = 1; col < Board.cols - 1; col++) {
			if (!isdigit(Board.board[row * Board.cols + col])) continue;

			adjacent = 0;
			start = &Board.board[row * Board.cols + col];
			end = start;
			while (end < (Board.board + (row * Board.cols + Board.cols - 1)) && isdigit(*end)) end++;

			subcol = col;
			for (char *s = start; s < end; s++) {
				ul = Board.board[(row - 1) * Board.cols + (subcol - 1)];
				um = Board.board[(row - 1) * Board.cols + subcol];
				ur = Board.board[(row - 1) * Board.cols + (subcol + 1)];
				ml = Board.board[row * Board.cols + (subcol - 1)];
				mr = Board.board[row * Board.cols + (subcol + 1)];
				ll = Board.board[(row + 1) * Board.cols + (subcol - 1)];
				lm = Board.board[(row + 1) * Board.cols + subcol];
				lr = Board.board[(row + 1) * Board.cols + (subcol + 1)];
				//printf("-\n|%c %c %c\n|%c %c %c\n|%c %c %c\n-\n", ur, um, ul, ml, *s, mr, ll, lm, lr);
				if (issymbol(ul) || issymbol(um) ||  issymbol(ur) 
						|| issymbol(ml) || issymbol(mr) 
						|| issymbol(ll) || issymbol(lm) || issymbol(lr)){
					adjacent++;
					break;
				}
				if (subcol >= Board.cols - 1) break;
				subcol++;
			}

			if (!adjacent) {
				for (char *s = start; s < end; s++) {
					*s = '.';
				}
			}
			// skip parsed number
			col += end - start;
		}
	}
	return Board;
}

struct Board keep_only_double_gears(struct Board Board) {
	int ul, um, ur, ml, mr, ll, lm, lr;
	for (size_t row = 1; row < Board.rows - 1; row++) {
		for (size_t col = 1; col < Board.cols - 1; col++) {
			if (issymbol(Board.board[row * Board.cols + col]) && !isgear(Board.board[row * Board.cols + col])) {
			   Board.board[row * Board.cols + col] = '.';
			   continue;
			}
			if (isdigit(Board.board[row * Board.cols + col]) || Board.board[row * Board.cols + col] == '.') continue;

			ul = Board.board[(row - 1) * Board.cols + (col - 1)];
			um = Board.board[(row - 1) * Board.cols + col];
			ur = Board.board[(row - 1) * Board.cols + (col + 1)];
			ml = Board.board[row * Board.cols + (col - 1)];
			mr = Board.board[row * Board.cols + (col + 1)];
			ll = Board.board[(row + 1) * Board.cols + (col - 1)];
			lm = Board.board[(row + 1) * Board.cols + col];
			lr = Board.board[(row + 1) * Board.cols + (col + 1)];
			//printf("-\n|%c %c %c\n|%c %c %c\n|%c %c %c\n-\n", ur, um, ul, ml, Board.board[row * Board.cols + col], mr, ll, lm, lr);
			// return value of isdigit can be anything
			// and the operator precedence fucked me over
			if ( (isdigit(ul) || isdigit(um) || isdigit(ur)) +
				 (isdigit(ul) && !isdigit(um) && isdigit(ur)) +
				 (isdigit(ml) && 1) +
				 (isdigit(mr) && 1) +
				 (isdigit(ll) || isdigit(lm) || isdigit(lr)) +
				 (isdigit(ll) && !isdigit(lm) && isdigit(lr)) != 2) {
				Board.board[row * Board.cols + col] = '.';
			}
		}
	}
	return remove_solitaire_numbers(Board);
}

int issymbol(int c) {
	return isascii(c) && !iscntrl(c) && !isdigit(c) && !isalpha(c) && c != '.';
}

int isgear(int c) {
	return c == '*';
}

unsigned long part1(const struct Board *Board) {
	char *start, *end;
	unsigned long result = 0;
	for (size_t row = 1; row < Board->rows - 1; row++) {
		for (size_t col = 1; col < Board->cols - 1; col++) {
			if (!isdigit(Board->board[row * Board->cols + col])) continue;
			start = &Board->board[row * Board->cols + col];
			result += strtol(start, &end, 10);
			// skip parsed number
			col += end - start;
		}
	}
	return result;
}

unsigned long part2(const struct Board *Board) {
	unsigned long result = 0;
	char *start;
	unsigned long nums[8];
	for (size_t row = 1; row < Board->rows - 1; row++) {
		for (size_t col = 1; col < Board->cols - 1; col++) {
			if (!isgear(Board->board[row * Board->cols + col])) continue;

			nums[0] = nums[1] = nums[2] = nums[3] = nums[4] = nums[5] = nums[6] = nums[7] = 1;
			// center left
			if (isdigit(Board->board[row * Board->cols + (col - 1)])) {
				start = &Board->board[row * Board->cols + (col - 1)];
				// move left until there are no more digits
				while (isdigit(*start)) start--;
				start++;
				nums[0] = atoi(start);
			}
			// center right
			if (isdigit(Board->board[row * Board->cols + (col + 1)])) {
				nums[1] = atoi(&Board->board[row * Board->cols + (col + 1)]);
			}
			// bottom, potentially bottom left
			if (isdigit(Board->board[(row + 1) * Board->cols + col])) {
				start = &Board->board[(row + 1) * Board->cols + col];
				// check left
				if (isdigit(Board->board[(row + 1) * Board->cols + (col - 1)])) {
					while (isdigit(*start)) start--;
					start++; // offset overshoot
				}
				nums[2] = atoi(start);
			}
			// top, potentially top left
			if (isdigit(Board->board[(row - 1) * Board->cols + col])) {
				start = &Board->board[(row - 1) * Board->cols + col];
				// check left
				if (isdigit(Board->board[(row - 1) * Board->cols + (col - 1)])) {
					while (isdigit(*start)) start--;
					start++; // offset overshoot
				}
				nums[3] = atoi(start);
			}
			// top left
			if (isdigit(Board->board[(row - 1) * Board->cols + (col - 1)])
					&& !isdigit(Board->board[(row - 1) * Board->cols + col])) {
				start = &Board->board[(row - 1) * Board->cols + (col - 1)];
				while (isdigit(*start)) start--;
				start++;
				nums[4] = atoi(start);

			}
			// bottom left
			if (isdigit(Board->board[(row + 1) * Board->cols + (col - 1)])
					&& !isdigit(Board->board[(row + 1) * Board->cols + col])) {
				start = &Board->board[(row + 1) * Board->cols + (col - 1)];
				while (isdigit(*start)) start--;
				start++;
				nums[5] = atoi(start);

			}
			// bottom right
			if (isdigit(Board->board[(row + 1) * Board->cols + (col + 1)])
					&& !isdigit(Board->board[(row + 1) * Board->cols + col])) {
				nums[6] = atoi(&Board->board[(row + 1) * Board->cols + (col + 1)]);
			}
			// top right
			if (isdigit(Board->board[(row - 1) * Board->cols + (col + 1)])
					&& !isdigit(Board->board[(row - 1) * Board->cols + col])) {
				nums[7] = atoi(&Board->board[(row - 1) * Board->cols + (col + 1)]);
			}
			result += nums[0] * nums[1] * nums[2] * nums[3] * nums[4] * nums[5] * nums[6] * nums[7];
		}
	}
	return result;
}

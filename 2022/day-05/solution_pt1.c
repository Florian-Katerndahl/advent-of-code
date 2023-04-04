#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define STACK_SIZE 100
#define LINE 1000
#define NUMBER_OF_CRATE_STACKS 100

struct Stack {
    size_t width;
    int crates_height[NUMBER_OF_CRATE_STACKS];
    char crates[NUMBER_OF_CRATE_STACKS][STACK_SIZE];
};

struct Instructions {
    size_t times;
    size_t from;
    size_t to;
};

void parse_stack(struct Stack *stack, FILE *file);

void reverse_string(char *s);

int parse_instruction_set(struct Instructions *instructions, FILE *written_instruction);

void execute_instruction(struct Stack *stack, struct Instructions *instructions);

int main(int argc, char *argv[]) {
    assert(argc == 2);

    FILE *f = fopen(*(argv + 1), "rt");

    if (f == NULL) {
        fprintf(stderr, "Failed to read file %s.\n", *(argv + 1));
        exit(1);
    }

    struct Stack stack = {0};

    struct Instructions instructions = {0};

    parse_stack(&stack, f);

    while(!feof(f)) {
        if (!parse_instruction_set(&instructions, f))
            continue;

        // adjust indices
        instructions.from--;
        instructions.to--;

        execute_instruction(&stack, &instructions);
    }

    printf("Result: ");

    for (size_t i = 0; i < stack.width; i++)
        printf("%c", stack.crates[i][stack.crates_height[i] - 1]);

    printf("\n");

    fclose(f);

    return 0;
}

void parse_stack(struct Stack *stack, FILE *file) {
    size_t n_crates;
    ssize_t stack_index;
    char line[LINE];

    while (fgets(line, LINE, file) != NULL && line[1] != '1') {
        n_crates = 0;
        stack_index = -1;

        for (size_t i = 0; i < strlen(line); i++) {
            // ugly, but increments counter everytime a slot is passed
            if ((i % 2) == 1 && ((i + 1) / 2) % 2 == 1)
                stack_index++;

            if (line[i] > 64 && line[i] < 91) {
                stack->crates[stack_index][stack->crates_height[stack_index]] = line[i];

                stack->crates_height[stack_index]++;

                n_crates++;
            }
        }

        if (stack->width < n_crates)
            stack->width = n_crates;
    }

    for (size_t i = 0; i < stack->width; i++) {
        reverse_string(stack->crates[i]);
    }
}

void reverse_string(char *s) {
    size_t len = strlen(s);
    char temp;

    for (size_t i = 0; i < len / 2; i++) {
        temp = s[i];
        s[i] = s[len - i - 1];
        s[len - i - 1] = temp;
    }
}

int parse_instruction_set(struct Instructions *instructions, FILE *written_instruction) {
    char line[LINE];

    if (fgets(line, LINE, written_instruction) != NULL &&
        sscanf(line, "move %ld from %ld to %ld\n",
               &instructions->times, &instructions->from, &instructions->to) == 3)
        return 1;
    else
        return 0;
}

void execute_instruction(struct Stack *stack, struct Instructions *instructions) {
    for (size_t i = 0; i < instructions->times; i++) {
        stack->crates[instructions->to][stack->crates_height[instructions->to]] = stack->crates[instructions->from][stack->crates_height[instructions->from] - 1];

        stack->crates[instructions->from][stack->crates_height[instructions->from] - 1] = '\0';

        stack->crates_height[instructions->to]++;

        stack->crates_height[instructions->from]--;
    }
}

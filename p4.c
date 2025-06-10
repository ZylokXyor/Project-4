#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INSTRUCTIONS 256 

int main(int argc, char *argv[]) {
    int nregisters;
    char *input_file;
    char *output_file;
    FILE *in;
    FILE *out;
    int *registers;
    char line[256];
    char instructions[MAX_INSTRUCTIONS][256];
    int instruction_lines[MAX_INSTRUCTIONS];
    int num_instructions = 0;
    int current_line = 1;
    int exec_line = 0;
    int i;

    if (argc != 4) { // Checks if the argument count is correct
        fprintf(stderr, "Usage: %s nregisters input_file output_file\n", argv[0]);
        return 1;
    }

    nregisters = atoi(argv[1]); 
    input_file = argv[2];
    output_file = argv[3];

    registers = (int *)malloc(sizeof(int) * nregisters); // Initializes n number of registers
    if (!registers) {
        perror("malloc failed"); // Sends error if allocation fails and exits the program
        return 1;
    }
    for (i = 0; i < nregisters; i++) {
        registers[i] = 0;
    }

    in = fopen(input_file, "r");
    if (!in) {
        perror("fopen failed"); // Sends error if file open fails, frees registers and exits the program
        free(registers);
        return 1;
    }

    out = fopen(output_file, "w");
    if (!out) {
        perror("fopen failed"); // Sends error if file open fails, frees registers, frees previously opened input file and exits the program
        free(registers);
        fclose(in);
        return 1;
    }

    // First pass: Read and print instructions, store instructions and their line numbers
    while (fgets(line, sizeof(line), in) && num_instructions < MAX_INSTRUCTIONS) {
        if (strspn(line, " \t\n\r") == strlen(line)) {
            fprintf(out, "%s", line);
        } else {
            strcpy(instructions[num_instructions], line);
            instruction_lines[num_instructions] = current_line;
            fprintf(out, "%d: %s", current_line, line);
            num_instructions++;
        }
        current_line++;
    }

    rewind(in);
    current_line = 1;
    exec_line = 0;

    // Second pass: Execute instructions
    while (exec_line < num_instructions) {
        char *instruction, *argument, *saveptr;

        strcpy(line, instructions[exec_line]);
        instruction = strtok_r(line, " \t\n\r", &saveptr);
        argument = strtok_r(NULL, " \t\n\r", &saveptr);

        if (strcmp(instruction, "inc") == 0) { // Handles the increment function
            int reg = atoi(argument);
            if (reg >= 0 && reg < nregisters) {
                registers[reg]++;
            } else {
                fprintf(out, "\nErrors\nLine %d: Invalid register.\n", instruction_lines[exec_line]); // Error printing if register number is invalid
                goto cleanup; // shortcut to close files and free register memory
            }
            exec_line++;
            current_line++;
        } else if (strcmp(instruction, "dec") == 0) { // Handles the decrement function
            int reg = atoi(argument);
            if (reg >= 0 && reg < nregisters) {
                if (registers[reg] > 0) {
                    registers[reg]--;
                    exec_line += 2;
                    current_line += 2;
                } else {
                    exec_line++;
                    current_line++;
                }
            } else {
                fprintf(out, "\nErrors\nLine %d: Invalid register.\n", instruction_lines[exec_line]); // Error printing if register number is invalid
                goto cleanup;
            }
        } else if (strcmp(instruction, "jump") == 0) { // Handles the jump function
            int jump_to = atoi(argument); 
            exec_line += jump_to;
            current_line += jump_to;
            if (exec_line < 0 || exec_line >= num_instructions) {
                break; // Break if jump is out of bounds
            }
        } else {
            fprintf(out, "\nErrors\nLine %d: Unknown instruction.\n", instruction_lines[exec_line]); // Error printing if instruction is unknown
            goto cleanup;
        }
    }

    fprintf(out, "\nFinal Register Values:\n"); // Print register values
    for (i = 0; i < nregisters; i++) {
        fprintf(out, "%02d: 0x%08X\n", i, registers[i]);
    }

cleanup: // Shortcut that frees memory and closes files
    fclose(in);
    fclose(out);
    free(registers);
    return 0;
}
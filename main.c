#include <stdio.h>
#include "io.h"
#include "decoder.h"

// store address, increment address and then compare label with current_address (where label addr is the addr where current was called (ex: beq 1, 2, label) + (label_desloc * 4)) 

int main() {
    char **labelsIndexes = malloc(1000 * sizeof(char *));
    FILE *f = getFile("test.txt", "r");

    int index = 0;
    char *line;
    while ((line = getNextLine(f)) != NULL) {
        if(labelsIndexes[index] != NULL) {
            printf("Label: %s\n", labelsIndexes[index]);
        }
        printf("%s\n", line);

        char *binary = hexToBinary(line);
        printf("%s\n", binary);

        int opcode = getOpcode(binary);
        printf("Opcode: %d\n", opcode);

        if(opcode == 0) {
            char **splitted = splitRTypeInstruction(binary);
            char *instructionString = mountTypeRInstructionString(splitted);
            printf("Instruction string: %s\n", instructionString);
        }

        else if (opcode == 2){
            // jump instruction
        }

        else {
            char **splitted = splitITypeInstruction(binary);
            char *instructionString = mountTypeIInstructionString(splitted);
            printf("Instruction string: %s\n", instructionString);

            if(isBeqInstruction(instructionString)) {
                printf("Is beq instruction\n");
                int desloc = getBeqInstructionDesloc(instructionString);
                char *label = getLabelFromInstruction(instructionString);
                printf("Label: %s\n", label);

                printf("Desloc: %d\n", desloc);
                labelsIndexes[desloc] = label;
                printf("Instruction string %s", instructionString);
            }
        }

        index++;
        free(line);
    }

    closeFile(f);
}
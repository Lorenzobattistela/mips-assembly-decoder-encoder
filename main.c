#include <stdio.h>
#include "io.h"
#include "decoder.h"


int main() {
    FILE *f = getFile("test.txt", "r");

    char *line;
    while ((line = getNextLine(f)) != NULL) {
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
        }


        free(line);
    }

    // for (int i = 0; i < 6; i++) {
    //     printf("Segment %d: %s\n", i, result[i]);
    //     int num = binaryStringToInt(result[i]);
    //     printf("Segment %d as int: %d\n", i, num);
    // }

    // char *instructionString = mountInstructionString(result);

    // printf("\nInstruction string: %s\n", instructionString);



    // // Free the allocated memory for each string and the array
    // for (int i = 0; i < 6; i++) {
    //     free(result[i]);
    // }
    // free(result);

    // int opcode = hexStringToInt(line);
    // printf("%d", opcode);

    closeFile(f);
}
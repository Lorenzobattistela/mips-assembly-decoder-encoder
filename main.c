#include <stdio.h>
#include <stdlib.h>
#include "io.h"
#include "decoder.h"

// store address, increment address and then compare label with current_address (where label addr is the addr where current was called (ex: beq 1, 2, label) + (label_desloc * 4)) 

int compareIntegers(const void *a, const void *b) {
    int intA = *((int*)a);
    int intB = *((int*)b);

    if (intA < intB) return -1;
    if (intA > intB) return 1;
    return 0;
}

int main() {
    int memoryAddress = 0x00400000;
    int count = 0;
    int last_written_address = 0;

    int *labelsIndexes = malloc(1000 * sizeof(int));
    FILE *f = getFile("test.txt", "r");
    FILE *outputFile = createFile("output.txt");

    char *header = ".text\n.globl main\nmain:";
    writeLine(outputFile, header);
    
    char *line;
    while ((line = getNextLine(f)) != NULL) {
        char *binary = hexToBinary(line);
        printf("%s\n", binary);

        int opcode = getOpcode(binary);
        printf("Opcode: %d\n", opcode);

        if(opcode == 0) {
            char **splitted = splitRTypeInstruction(binary);
            char *instructionString = mountTypeRInstructionString(splitted);
            printf("Instruction string: %s\n", instructionString);
            writeLine(outputFile, instructionString);
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

                int calculatedAddress = calculateAddress(memoryAddress, desloc);

                printf("Desloc: %d\n", desloc);
                labelsIndexes[count] = calculatedAddress + 4;
                count++;
                printf("initial address: %d\n", 0x00400000);
                printf("current address: %d\n", memoryAddress);
                printf("Label should be written in address %d\n", calculatedAddress);

                char * newString = removeBeqPrefix(instructionString);
                writeLine(outputFile, newString);
                
            }  
            else {
                writeLine(outputFile, instructionString);
            }
        }

        printf("\n\n");
        memoryAddress += 4;
        free(line);
    }
    closeFile(f);
    closeFile(outputFile);
    

    outputFile = getFile("output.txt", "r");
    FILE *out = getFile("out.txt", "w");

    // sort labelsIndexes
    qsort(labelsIndexes, count, sizeof(int), compareIntegers);
    printf("Sorted labelsIndexes\n");
    for(int i = 0; i < count; i++) {
        printf("%d\n", labelsIndexes[i]);
    }

    // skip first 3 lines of the file
    for(int i = 0; i < 3; i++) {
        line = getNextLine(outputFile);
        printf("%s", line);
        writeLineWithoutBreak(out, line);
        free(line);
    }

    memoryAddress = 0x00400000;
    while ((line = getNextLine(outputFile)) != NULL) {
        printf("%s", line);
        if(labelsIndexes[last_written_address] == memoryAddress) {
            char *label = getLabel(last_written_address);
            printf("Label: %s should be written in address %d\n", label, memoryAddress);
            writeLabel(out, label);
            last_written_address++;
        }
        writeLineWithoutBreak(out, line);
        memoryAddress += 4;
        free(line);
    }
    closeFile(outputFile);
    closeFile(out);
}
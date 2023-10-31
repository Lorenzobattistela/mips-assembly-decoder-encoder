#include <stdio.h>
#include <stdlib.h>
#include "io.h"
#include "decoder.h"
#include "encoder.h"

// store address, increment address and then compare label with current_address (where label addr is the addr where current was called (ex: beq 1, 2, label) + (label_desloc * 4)) 

int compareIntegers(const void *a, const void *b) {
    int intA = *((int*)a);
    int intB = *((int*)b);

    if (intA < intB) return -1;
    if (intA > intB) return 1;
    return 0;
}

int main() {
    FILE *input = getFile("entrada.asm", "r");
    FILE *output = getFile("saida.txt", "w");

    char *line;
    while(line = getNextLine(input)) {
        printf("%s\n", line);
        char *trimmedLine = trim(line);
        printf("%s\n", trimmedLine);
        // writeLine(output, line);
        int opcode = getOpcodeFromAsm(trimmedLine);
        printf("Opcode: %d\n", opcode);
        
        free(line);
    }
}

int decode() {
    int memoryAddress = 0x00400000;
    int count = 0;
    int last_written_address = 0;

    char **labels = malloc(1000 * sizeof(char*));
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
            char **splitted = splitJTypeInstruction(binary);
            char *instructionString = mountTypeJInstructionString(splitted);
            int addr = getAddressFromJInstruction(instructionString);

            printf("Instruction string: %s\n", instructionString);
            printf("Address: %d\n", addr);

            // find some label (if exists) that has the same addr of this instruction
            // if exists, its ok because it will be written anyway
            // if it does not exist, we need to create a new label on that addr

            bool labelExists = false;
            for(int i = 0; i < count; i++) {
                if(labelsIndexes[i] == addr) {
                    labelExists = true;
                    break;
                }
            }

            printf("Label exists: %d\n", labelExists);

            if(!labelExists) {
                labelsIndexes[count] = addr;
                labels[count] = createLabel();
                count++;
            }

            // add label to instruction string
            char *instr = malloc(100);
            sprintf(instr, "j %s", labels[count - 1]);
            writeLine(outputFile, instr);

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

                if(desloc < 0) {
                    printf("Desloc is negative\n");
                    calculatedAddress -= 4;
                }

                else {
                    calculatedAddress += 4;
                }

                labelsIndexes[count] = calculatedAddress;
                labels[count] = getLabelFromInstruction(instructionString);
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
            char *label = labels[last_written_address];
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
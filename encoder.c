#include "encoder.h"

char *trim(char *line) {
    char *end;

    while(isspace((unsigned char)*line)) line++;

    if(*line == 0) return line;

    end = line + strlen(line) - 1;
    while(end > line && isspace((unsigned char)*end)) end--;

    end[1] = '\0';

    return line;
}

int getRegisterInt(char *register) {
    if(strncmp(register, "$zero", 5) == 0) {
        return 0;
    }
    else if(strncmp(register, "$at", 3) == 0) {
        return 1;
    }
    else if(strncmp(register, "$v0", 3) == 0) {
        return 2;
    }
    else if(strncmp(register, "$v1", 3) == 0) {
        return 3;
    }
    else if(strncmp(register, "$a0", 3) == 0) {
        return 4;
    }
    else if(strncmp(register, "$a1", 3) == 0) {
        return 5;
    }
    else if(strncmp(register, "$a2", 3) == 0) {
        return 6;
    }
    else if(strncmp(register, "$a3", 3) == 0) {
        return 7;
    }
    else if(strncmp(register, "$t0", 3) == 0) {
        return 8;
    }
    else if(strncmp(register, "$t1", 3) == 0) {
        return 9;
    }
    else if(strncmp(register, "$t2", 3) == 0) {
        return 10;
    }
    else if(strncmp(register, "$t3", 3) == 0) {
        return 11;
    }
    else if(strncmp(register, "$t4", 3) == 0) {
        return 12;
    }
    else if(strncmp(register, "$t5", 3) == 0) {
        return 13;
    }
    else if(strncmp(register, "$t6", 3) == 0) {
        return 14;
    }
    else if(strncmp(register, "$t7", 3) == 0) {
        return 15;
    }
    else if(strncmp(register, "$s0", 3) == 0) {
        return 16;
    }
    else if(strncmp(register, "$s1", 3) == 0) {
        return 17;
    }
    else if(strncmp(register, "$s2", 3) == 0) {
        return 18;
    }
    else if(strncmp(register, "$s3", 3) == 0) {
        return 19;
    }
    else if(strncmp(register, "$s4", 3) == 0) {
        return 20;
    }
    else if(strncmp(register, "$s5", 3) == 0) {
        return 21;
    }
    else if(strncmp(register, "$s6", 3) == 0) {
        return 22;
    }
    else if(strncmp(register, "$s7", 3) == 0) {
        return 23;
    }
    else if(strncmp(register, "$t8", 3) == 0) {
        return 24;
    }
    else if(strncmp(register, "$t9", 3) == 0) {
        return 25;
    }
    else if(strncmp(register, "$k0", 3) == 0) {
        return 26;
    }
    else if(strncmp(register, "$k1", 3) == 0) {
        return 27;
    }
    else if(strncmp(register, "$gp", 3) == 0) {
        return 28;
    }
    else if(strncmp(register, "$sp", 3) == 0) {
        return 29;
    }
    else if(strncmp(register, "$fp", 3) == 0) {
        return 30;
    }
    else if(strncmp(register, "$ra", 3) == 0) {
        return 31;
    }
}

int getOpcodeFromAsm(char *instruction) {
    // beq 1, 2, label
    if(strncmp(instruction, "j", 1) == 0) {
        return 2;
    }
    else if(strncmp(instruction, "sw", 2) == 0) {
        return 43;
    }
    else if(strncmp(instruction, "lw", 2) == 0) {
        return 23;
    }
    else if(strncmp(instruction, "beq", 3) == 0) {
        return 4;
    }
    else if(strncmp(instruction, "sltiu", 5) == 0) {
        return 11;
    }
    return 0;
}
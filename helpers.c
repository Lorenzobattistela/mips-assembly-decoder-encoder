#include "helpers.h"

char *intToBinary(int n, int bits) {
    char *binary = malloc(bits * sizeof(char));
    if (binary == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(1);
    }

    for (int i = bits - 1; i >= 0; i--) {
        binary[i] = (n & 1) + '0';
        n >>= 1;
    }

    return binary;
}

char *binaryToHexadecimal(char *binaryInstruction) {
    char *hexadecimal = malloc(8 * sizeof(char));
    if (hexadecimal == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(1);
    }

    int decimal = (int) strtol(binaryInstruction, NULL, 2);
    sprintf(hexadecimal, "0x%08X", decimal);

    return hexadecimal;
}

int getFunctBinaryString(char *opcode) {
    if(strncmp(opcode, "sub", 3) == 0) {
        return 34;
    }
    else if(strncmp(opcode, "and", 3) == 0) {
        return 36;
    }
    else if(strncmp(opcode, "or", 2) == 0) {
        return 37;
    }
    else {
        return -1;
    }
}

int getRegisterInt(char *regist) {
    if(strncmp(regist, "$zero", 5) == 0) {
        return 0;
    }
    else if(strncmp(regist, "$at", 3) == 0) {
        return 1;
    }
    else if(strncmp(regist, "$v0", 3) == 0) {
        return 2;
    }
    else if(strncmp(regist, "$v1", 3) == 0) {
        return 3;
    }
    else if(strncmp(regist, "$a0", 3) == 0) {
        return 4;
    }
    else if(strncmp(regist, "$a1", 3) == 0) {
        return 5;
    }
    else if(strncmp(regist, "$a2", 3) == 0) {
        return 6;
    }
    else if(strncmp(regist, "$a3", 3) == 0) {
        return 7;
    }
    else if(strncmp(regist, "$t0", 3) == 0) {
        return 8;
    }
    else if(strncmp(regist, "$t1", 3) == 0) {
        return 9;
    }
    else if(strncmp(regist, "$t2", 3) == 0) {
        return 10;
    }
    else if(strncmp(regist, "$t3", 3) == 0) {
        return 11;
    }
    else if(strncmp(regist, "$t4", 3) == 0) {
        return 12;
    }
    else if(strncmp(regist, "$t5", 3) == 0) {
        return 13;
    }
    else if(strncmp(regist, "$t6", 3) == 0) {
        return 14;
    }
    else if(strncmp(regist, "$t7", 3) == 0) {
        return 15;
    }
    else if(strncmp(regist, "$s0", 3) == 0) {
        return 16;
    }
    else if(strncmp(regist, "$s1", 3) == 0) {
        return 17;
    }
    else if(strncmp(regist, "$s2", 3) == 0) {
        return 18;
    }
    else if(strncmp(regist, "$s3", 3) == 0) {
        return 19;
    }
    else if(strncmp(regist, "$s4", 3) == 0) {
        return 20;
    }
    else if(strncmp(regist, "$s5", 3) == 0) {
        return 21;
    }
    else if(strncmp(regist, "$s6", 3) == 0) {
        return 22;
    }
    else if(strncmp(regist, "$s7", 3) == 0) {
        return 23;
    }
    else if(strncmp(regist, "$t8", 3) == 0) {
        return 24;
    }
    else if(strncmp(regist, "$t9", 3) == 0) {
        return 25;
    }
    else if(strncmp(regist, "$k0", 3) == 0) {
        return 26;
    }
    else if(strncmp(regist, "$k1", 3) == 0) {
        return 27;
    }
    else if(strncmp(regist, "$gp", 3) == 0) {
        return 28;
    }
    else if(strncmp(regist, "$sp", 3) == 0) {
        return 29;
    }
    else if(strncmp(regist, "$fp", 3) == 0) {
        return 30;
    }
    else if(strncmp(regist, "$ra", 3) == 0) {
        return 31;
    }
    else {
        return -1;
    }
}
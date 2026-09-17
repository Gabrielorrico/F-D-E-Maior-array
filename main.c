/**
 * #################################
 * # Esqueleto do processador em C #
 * #################################
*/
#include <stdio.h>
#include <stdint.h>

uint8_t mem[256] = {0};
uint8_t reg[4] = {0};
uint8_t pc = 0, zf = 0, running = 1;
int ciclo = 0;

void fetch(uint8_t *op, uint8_t *a, uint8_t *b) {
    *op = mem[pc]; *a = mem[pc+1]; *b = mem[pc+2];
    pc += 3;
}

void decode_execute(uint8_t op, uint8_t a, uint8_t b) {
    switch (op) {
        case 0x01: reg[a] = mem[b]; break;
        case 0x02: mem[b] = reg[a]; break;
        case 0x03: reg[a] = reg[a] + reg[b]; break;
        case 0x04: reg[a] = reg[a] - reg[b]; break;
        case 0x05: reg[a] = b; break;
        case 0x06: zf = (reg[a] == reg[b]) ? 1 : 0; break;
        case 0x07: pc = a; break;
        case 0x08: if (zf) pc = a; break;
        case 0x09: if (!zf) pc = a; break;
        case 0x0A: running = 0; break;
    }
}

void trace(uint8_t op, uint8_t a, uint8_t b) {
    const char *nomes[] = {"","LOAD","STORE","ADD",
        "SUB","MOV","CMP","JMP","JZ","JNZ","HALT"};
    printf("Ciclo %d: %-5s %d,%d | R0=%3d R1=%3d"
           " R2=%3d R3=%3d | PC=%3d ZF=%d\n",
           ciclo, nomes[op], a, b,
           reg[0], reg[1], reg[2], reg[3], pc, zf);
}

void carregar_desafio_maximo() {

    mem[0x10] = 12;
    mem[0x11] = 45;
    mem[0x12] = 7;
    mem[0x13] = 89;
    mem[0x14] = 23;
    mem[0x15] = 56;
    mem[0x16] = 3;
    mem[0x17] = 67;


    int idx = 0;

    mem[idx++] = 0x01; mem[idx++] = 1; mem[idx++] = 0x10; 


    uint8_t addrs[] = {0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17};
    
    for (int i = 0; i < 7; i++) {

        mem[idx++] = 0x01; mem[idx++] = 2; mem[idx++] = addrs[i];
        mem[idx++] = 0x05; mem[idx++] = 3; mem[idx++] = 0; 

    }

    idx = 0;
    
    mem[idx++] = 0x01; mem[idx++] = 1; mem[idx++] = 0x10; 

    for (uint8_t pos = 0x11; pos <= 0x17; pos++) {
        mem[idx++] = 0x01; mem[idx++] = 2; mem[idx++] = pos;  
        mem[idx++] = 0x05; mem[idx++] = 0; mem[idx++] = pos;  
    }
}

int main() {

    mem[0x10] = 12; 
    mem[0x11] = 45; 
    mem[0x12] = 7;  
    mem[0x13] = 89;
    mem[0x14] = 23; 
    mem[0x15] = 56; 
    mem[0x16] = 3;  
    mem[0x17] = 67;


    mem[0] = 0x07; mem[1] = 0x30; mem[2] = 0; 

    int p = 0x30; 


    mem[p++] = 0x01; mem[p++] = 1; mem[p++] = 0x10; 


    mem[p++] = 0x01; mem[p++] = 2; mem[p++] = 0x11;  
    mem[p++] = 0x01; mem[p++] = 1; mem[p++] = 0x11;

    mem[p++] = 0x01; mem[p++] = 2; mem[p++] = 0x12; 

   
    mem[p++] = 0x01; mem[p++] = 2; mem[p++] = 0x13; 
    mem[p++] = 0x01; mem[p++] = 1; mem[p++] = 0x13; 


    mem[p++] = 0x01; mem[p++] = 2; mem[p++] = 0x14;  
    mem[p++] = 0x01; mem[p++] = 2; mem[p++] = 0x15;  
    mem[p++] = 0x01; mem[p++] = 2; mem[p++] = 0x16;  
    mem[p++] = 0x01; mem[p++] = 2; mem[p++] = 0x17;  

    mem[p++] = 0x02; mem[p++] = 1; mem[p++] = 0x20;  

    mem[p++] = 0x0A; mem[p++] = 0; mem[p++] = 0; 

    while (running && pc < 256) {
        uint8_t op, a, b;
        ciclo++;
        fetch(&op, &a, &b);
        decode_execute(op, a, b);
        trace(op, a, b);
    }

    printf("\nMaior valor gravado em 0x20 (32 em decimal): %d\n", mem[0x20]);

    return 0;
}
#include "simulator.h"

CPU cpu;
int debugMode = 1;
int instructionCount = 0;

void initCPU() {
    cpu.A = cpu.B = cpu.C = cpu.D = cpu.E = cpu.H = cpu.L = 0;
    cpu.PC = 0;
    cpu.SP = 0xFFFF;
    cpu.Z = cpu.S = cpu.P = cpu.CY = cpu.AC = 0;
}

void updateFlags(unsigned char result) {
    cpu.Z = (result == 0);
    cpu.S = (result & 0x80) != 0;

    int count = 0;
    for(int i = 0; i < 8; i++)
        if(result & (1 << i)) count++;
    cpu.P = (count % 2 == 0);
}

void printState() {
    printf("\n================ CPU STATE ================\n");
    printf("A:%02X  B:%02X  C:%02X  D:%02X  E:%02X  H:%02X  L:%02X\n",
           cpu.A,cpu.B,cpu.C,cpu.D,cpu.E,cpu.H,cpu.L);
    printf("PC:%04X  SP:%04X\n", cpu.PC, cpu.SP);
    printf("FLAGS -> Z:%d S:%d P:%d CY:%d AC:%d\n",
           cpu.Z,cpu.S,cpu.P,cpu.CY,cpu.AC);
    printf("===========================================\n");
}

void printFlags() {
    printf("\nFinal Flags: Z=%d S=%d P=%d CY=%d AC=%d\n",
           cpu.Z,cpu.S,cpu.P,cpu.CY,cpu.AC);
}

void viewMemory(int start, int end) {
    printf("\n--- MEMORY OUTPUT ---\n");
    for(int i = start; i <= end; i++)
        printf("Addr %04X : %02X\n", i, memory[i]);
}

void initializeEnvironment() {
    char choice;
    unsigned int val, addr;

    printf("Do you want to initialize registers? (y/n): ");
    scanf(" %c", &choice);

    if(choice == 'y' || choice == 'Y') {
        printf("Enter A register (hex): "); scanf("%x", &val); cpu.A = val;
        printf("Enter B register (hex): "); scanf("%x", &val); cpu.B = val;
        printf("Enter C register (hex): "); scanf("%x", &val); cpu.C = val;
        printf("Enter D register (hex): "); scanf("%x", &val); cpu.D = val;
        printf("Enter E register (hex): "); scanf("%x", &val); cpu.E = val;
        printf("Enter H register (hex): "); scanf("%x", &val); cpu.H = val;
        printf("Enter L register (hex): "); scanf("%x", &val); cpu.L = val;
    }

    printf("Do you want to initialize memory? (y/n): ");
    scanf(" %c", &choice);

    while(choice == 'y' || choice == 'Y') {
        printf("Enter memory address (hex): ");
        scanf("%x", &addr);
        printf("Enter value (hex): ");
        scanf("%x", &val);
        memory[addr] = val;

        printf("Add another memory value? (y/n): ");
        scanf(" %c", &choice);
    }
}

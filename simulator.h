#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MEM_SIZE 65536
#define MAX_LABELS 50
#define MAX_PROGRAM_LINES 500


extern unsigned char memory[MEM_SIZE];

typedef struct {
    unsigned char A, B, C, D, E, H, L;
    unsigned short PC;
    unsigned short SP;
    int Z, S, P, CY, AC;
} CPU;

extern CPU cpu;
extern int debugMode;
extern int instructionCount;
extern char program[500][100];
extern int lines;


typedef struct {
    char name[20];
    int address;
} Label;

extern Label labelTable[MAX_LABELS];
extern int labelCount;

extern char program[MAX_PROGRAM_LINES][100];
extern int lines;


void initCPU();
void updateFlags(unsigned char result);
void printState();
void printFlags();
void parseInstruction(char *line);
void loadProgram();
void runProgram();
void viewMemory(int start, int end);
int findLabelAddress(char *name);
void initializeEnvironment();

void MVI(char r, int data);
void MOV(char d, char s);
void ADD(char r);
void SUB(char r);
void INR(char r);
void DCR(char r);
void LDA(int addr);
void STA(int addr);
void JMP(int addr);
void JM(int addr);
void JZ(int addr);
void JNZ(int addr);
void JNC(int addr);
void PUSH(char rp);
void POP(char rp);
void CALL(int addr);
void ADC(char r);
void SBB(char r);
void ADI(int data);
void ACI(int data);
void SUI(int data);
void SBI(int data);
void INX(char rp); void DCX(char rp); void DAD(char rp);
void ANI(int data); void ORI(int data); void XRI(int data); void CPI(int data);
void RAL(); void RAR();
void LHLD(int addr); void SHLD(int addr);
void LDAX(char rp); void STAX(char rp); void XCHG();
void CZ(int addr); void CNZ(int addr); void RZ(); void RNZ();
void OUT(int port); void IN(int port);
void NOP(); void EI(); void DI(); void SIM(); void RIM();
void JP(int addr); void JPE(int addr); void JPO(int addr);
void CC(int addr); void CNC(int addr); void CP(int addr);
void CM(int addr); void CPE(int addr); void CPO(int addr);
void RC(); void RNC(); void RP(); void RM(); void RPE(); void RPO();
void RST(int n);
void SPHL(); void PCHL();
void CMA(); void CMC(); void STC();
void DAA();
void LXI(char rp, int data);
void RET();
#endif

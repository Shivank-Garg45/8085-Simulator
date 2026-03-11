#include "simulator.h"

Label labelTable[MAX_LABELS];
int labelCount = 0;

char program[MAX_PROGRAM_LINES][100];
int lines = 0;

void loadProgram(){
    FILE *fp = fopen("program.asm","r");
    if(!fp){ printf("program.asm not found\n"); exit(1); }

    lines = 0;
    labelCount = 0;

    while(fgets(program[lines],100,fp)){
        char temp[100];
        strcpy(temp, program[lines]);

        char *colon = strchr(temp, ':');
        if(colon){
            *colon = '\0';
            strcpy(labelTable[labelCount].name, temp);
            labelTable[labelCount].address = lines;
            labelCount++;
        }
        lines++;
    }
    fclose(fp);
}

void runProgram(){
    while(cpu.PC < lines){
        printf("\nExecuting Line %d: %s\n", cpu.PC+1, program[cpu.PC]);
        parseInstruction(program[cpu.PC]);
        printState();

        instructionCount++;
        cpu.PC++;

        if(debugMode){
            printf("\nPress ENTER to execute next instruction...");
            getchar();
        }
    }
}

int main(){
    initCPU();
    initializeEnvironment();
    loadProgram();
    runProgram();

    printFlags();
    printf("\nTotal Instructions Executed: %d\n", instructionCount);

    char choice;
    unsigned int start, end;

    printf("\nDo you want to see memory output? (y/n): ");
    scanf(" %c", &choice);

    while(choice=='y' || choice=='Y'){
        printf("Enter start address (in HEX): ");
        scanf("%x", &start);
        printf("Enter end address (in HEX): ");
        scanf("%x", &end);
        viewMemory(start,end);

        printf("\nDo you want to see other output? (y/n): ");
        scanf(" %c", &choice);
    }

    printf("\nProgram Finished.\n");
    return 0;
}

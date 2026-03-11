#include "simulator.h"

int findLabel(char *name){
    for(int i = 0; i < labelCount; i++){
        if(strcmp(labelTable[i].name, name) == 0)
            return labelTable[i].address;
    }
    printf("Label not found: %s\n", name);
    exit(1);
}

void parseInstruction(char *line)
{
    while(*line==' ' || *line=='\t') line++;
    line[strcspn(line, "\r\n")] = 0;

    if(line[0]=='\0' || line[0]==';') return;

    char *colon = strchr(line, ':');
    if(colon){
        line = colon + 1;
        while(*line==' ' || *line=='\t') line++;
        if(line[0]=='\0' || line[0]==';') return;
    }

    char op[10], r1, r2, lbl[20];
    int data;

    sscanf(line,"%s",op);

    if(strcmp(op,"MVI")==0){ sscanf(line,"MVI %c, %x",&r1,&data); MVI(r1,data); }
    else if(strcmp(op,"MOV")==0){ sscanf(line,"MOV %c, %c",&r1,&r2); MOV(r1,r2); }
    else if(strcmp(op,"ADD")==0){ sscanf(line,"ADD %c",&r1); ADD(r1); }
    else if(strcmp(op,"SUB")==0){ sscanf(line,"SUB %c",&r1); SUB(r1); }
    else if(strcmp(op,"INR")==0){ sscanf(line,"INR %c",&r1); INR(r1); }
    else if(strcmp(op,"DCR")==0){ sscanf(line,"DCR %c",&r1); DCR(r1); }
    else if(strcmp(op,"LDA")==0){ sscanf(line,"LDA %x",&data); LDA(data); }
    else if(strcmp(op,"STA")==0){ sscanf(line,"STA %x",&data); STA(data); }

    else if(strcmp(op,"JMP")==0){ sscanf(line,"JMP %s",lbl); cpu.PC = findLabel(lbl)-1; }
    else if(strcmp(op,"JZ")==0){ sscanf(line,"JZ %s",lbl); if(cpu.Z) cpu.PC = findLabel(lbl)-1; }
    else if(strcmp(op,"JNZ")==0){ sscanf(line,"JNZ %s",lbl); if(!cpu.Z) cpu.PC = findLabel(lbl)-1; }
    else if(strcmp(op,"JM")==0){ sscanf(line,"JM %s",lbl); if(cpu.S) cpu.PC = findLabel(lbl)-1; }
    else if(strcmp(op,"JNC")==0){ sscanf(line,"JNC %s",lbl); if(!cpu.CY) cpu.PC = findLabel(lbl)-1; }
   
    else if(strcmp(op,"PUSH")==0){ sscanf(line,"PUSH %c",&r1); PUSH(r1); }
    else if(strcmp(op,"POP")==0){ sscanf(line,"POP %c",&r1); POP(r1); }
    else if(strcmp(op,"CALL")==0){ sscanf(line,"CALL %s",lbl); CALL(findLabel(lbl)); }
    else if(strcmp(op,"RET")==0){ RET(); }
    else if(strcmp(op,"HLT")==0){ cpu.PC = lines; }

    else if(strcmp(op,"LXI")==0){ sscanf(line,"LXI %c, %x",&r1,&data); LXI(r1,data); }
    else if(strcmp(op,"INX")==0){ sscanf(line,"INX %c",&r1); INX(r1); }
    else if(strcmp(op,"DCX")==0){ sscanf(line,"DCX %c",&r1); DCX(r1); }


    else { printf("Unknown Instruction: %s\n", line); exit(1); }
}

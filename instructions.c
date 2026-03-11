#include "simulator.h"

int isValidRegister(char r){
    return (r=='A'||r=='B'||r=='C'||r=='D'||r=='E'||r=='H'||r=='L'||r=='M');
}


unsigned char* getRegister(char r){
    switch(r){
        case 'A': return &cpu.A;
        case 'B': return &cpu.B;
        case 'C': return &cpu.C;
        case 'D': return &cpu.D;
        case 'E': return &cpu.E;
        case 'H': return &cpu.H;
        case 'L': return &cpu.L;
        case 'M': return &memory[(cpu.H<<8) | cpu.L];  
    }
    return NULL;
}


unsigned short getRP(char rp){
    if(rp=='B') return (cpu.B<<8)|cpu.C;
    if(rp=='D') return (cpu.D<<8)|cpu.E;
    if(rp=='H') return (cpu.H<<8)|cpu.L;
    return 0;
}

void setRP(char rp, unsigned short val){
    if(rp=='B'){ cpu.B=val>>8; cpu.C=val&0xFF; }
    if(rp=='D'){ cpu.D=val>>8; cpu.E=val&0xFF; }
    if(rp=='H'){ cpu.H=val>>8; cpu.L=val&0xFF; }
}

void INX(char rp){ setRP(rp, getRP(rp)+1); }
void DCX(char rp){ setRP(rp, getRP(rp)-1); }

void DAD(char rp){
    unsigned int res = getRP('H') + getRP(rp);
    cpu.CY = res > 0xFFFF;
    setRP('H', res & 0xFFFF);
}

void LXI(char rp, int data){
    if(rp=='B'){ cpu.B = (data>>8)&0xFF; cpu.C = data&0xFF; }
    else if(rp=='D'){ cpu.D = (data>>8)&0xFF; cpu.E = data&0xFF; }
    else if(rp=='H'){ cpu.H = (data>>8)&0xFF; cpu.L = data&0xFF; }
}


void MVI(char r, int data){
    if(!isValidRegister(r)){ printf("Invalid Register\n"); exit(1);}
    *getRegister(r) = data;
}

void MOV(char d, char s){
    if(!isValidRegister(d)||!isValidRegister(s)){ printf("Invalid Register\n"); exit(1);}
    *getRegister(d) = *getRegister(s);
}

void ADD(char r){
    unsigned short res = cpu.A + *getRegister(r);
    cpu.CY = (res > 0xFF);
    cpu.A = res & 0xFF;
    updateFlags(cpu.A);
}

void SUB(char r){
    unsigned short res = cpu.A - *getRegister(r);
    cpu.CY = (cpu.A < *getRegister(r));
    cpu.A = res & 0xFF;
    updateFlags(cpu.A);
}

void JM(int addr){
    if(cpu.S) cpu.PC = addr - 1;
}

void ADC(char r){
    unsigned short res = cpu.A + *getRegister(r) + cpu.CY;
    cpu.CY = res > 0xFF;
    cpu.A = res & 0xFF;
    updateFlags(cpu.A);
}

void SBB(char r){
    unsigned short res = cpu.A - *getRegister(r) - cpu.CY;
    cpu.CY = (cpu.A < (*getRegister(r) + cpu.CY));
    cpu.A = res & 0xFF;
    updateFlags(cpu.A);
}

void ADI(int data){
    unsigned short res = cpu.A + data;
    cpu.CY = res > 0xFF;
    cpu.A = res & 0xFF;
    updateFlags(cpu.A);
}

void ACI(int data){
    unsigned short res = cpu.A + data + cpu.CY;
    cpu.CY = res > 0xFF;
    cpu.A = res & 0xFF;
    updateFlags(cpu.A);
}

void SUI(int data){
    cpu.CY = (cpu.A < data);
    cpu.A = (cpu.A - data) & 0xFF;
    updateFlags(cpu.A);
}

void SBI(int data){
    unsigned short res = cpu.A - data - cpu.CY;
    cpu.CY = (cpu.A < (data + cpu.CY));
    cpu.A = res & 0xFF;
    updateFlags(cpu.A);
}

void ANI(int data){ cpu.A &= data; cpu.CY=0; updateFlags(cpu.A); }
void ORI(int data){ cpu.A |= data; cpu.CY=0; updateFlags(cpu.A); }
void XRI(int data){ cpu.A ^= data; cpu.CY=0; updateFlags(cpu.A); }
void CPI(int data){
    unsigned char res = cpu.A - data;
    cpu.Z = (res==0);
    cpu.S = (res & 0x80)!=0;
    cpu.CY = cpu.A < data;
}

void RAL(){
    int newCY = (cpu.A & 0x80)!=0;
    cpu.A = (cpu.A<<1) | cpu.CY;
    cpu.CY = newCY;
}

void RAR(){
    int newCY = cpu.A & 1;
    cpu.A = (cpu.A>>1) | (cpu.CY<<7);
    cpu.CY = newCY;
}

void LHLD(int addr){
    cpu.L = memory[addr];
    cpu.H = memory[addr+1];
}

void SHLD(int addr){
    memory[addr] = cpu.L;
    memory[addr+1] = cpu.H;
}

void LDAX(char rp){ cpu.A = memory[getRP(rp)]; }
void STAX(char rp){ memory[getRP(rp)] = cpu.A; }

void XCHG(){
    unsigned char tH=cpu.H, tL=cpu.L;
    cpu.H=cpu.D; cpu.L=cpu.E;
    cpu.D=tH; cpu.E=tL;
}

void CALL(int addr){
    memory[--cpu.SP] = (cpu.PC+1)&0xFF;
    memory[--cpu.SP] = ((cpu.PC+1)>>8)&0xFF;
    cpu.PC = addr-1;
}

void RET(){
    int low = memory[cpu.SP++];
    int high = memory[cpu.SP++];
    cpu.PC = ((high<<8)|low)-1;
}

void CZ(int addr){ if(cpu.Z) CALL(addr); }
void CNZ(int addr){ if(!cpu.Z) CALL(addr); }
void RZ(){ if(cpu.Z) RET(); }
void RNZ(){ if(!cpu.Z) RET(); }

unsigned char ports[256];

void OUT(int port){
    ports[port] = cpu.A;
    printf("OUT Port %02X = %02X\n", port, cpu.A);
}

void IN(int port){
    cpu.A = ports[port];
    printf("IN Port %02X -> A = %02X\n", port, cpu.A);
}

void NOP(){}

void EI(){ printf("Interrupts Enabled (simulated)\n"); }
void DI(){ printf("Interrupts Disabled (simulated)\n"); }
void SIM(){ printf("SIM executed (simulated)\n"); }
void RIM(){ printf("RIM executed (simulated)\n"); }


void INR(char r){ (*getRegister(r))++; updateFlags(*getRegister(r)); }
void DCR(char r){ (*getRegister(r))--; updateFlags(*getRegister(r)); }

void LDA(int addr){ cpu.A = memory[addr]; }
void STA(int addr){ memory[addr] = cpu.A; }

void JMP(int addr){ cpu.PC = addr - 1; }
void JZ(int addr){ if(cpu.Z) cpu.PC = addr - 1; }
void JNZ(int addr){ if(!cpu.Z) cpu.PC = addr - 1; }
void JNC(int addr){
    if(!cpu.CY) cpu.PC = addr - 1;
}


void PUSH(char rp){
    if(rp=='B'){ memory[--cpu.SP] = cpu.B; memory[--cpu.SP] = cpu.C; }
    else if(rp=='D'){ memory[--cpu.SP] = cpu.D; memory[--cpu.SP] = cpu.E; }
    else if(rp=='H'){ memory[--cpu.SP] = cpu.H; memory[--cpu.SP] = cpu.L; }
}


void POP(char rp){
    if(rp=='B'){ cpu.C = memory[cpu.SP++]; cpu.B = memory[cpu.SP++]; }
    else if(rp=='D'){ cpu.E = memory[cpu.SP++]; cpu.D = memory[cpu.SP++]; }
    else if(rp=='H'){ cpu.L = memory[cpu.SP++]; cpu.H = memory[cpu.SP++]; }
}

void JP(int addr){ if(!cpu.S) cpu.PC = addr-1; }
void JPE(int addr){ if(cpu.P) cpu.PC = addr-1; }
void JPO(int addr){ if(!cpu.P) cpu.PC = addr-1; }

void CC(int addr){ if(cpu.CY) CALL(addr); }
void CNC(int addr){ if(!cpu.CY) CALL(addr); }
void CP(int addr){ if(!cpu.S) CALL(addr); }
void CM(int addr){ if(cpu.S) CALL(addr); }
void CPE(int addr){ if(cpu.P) CALL(addr); }
void CPO(int addr){ if(!cpu.P) CALL(addr); }

void RC(){ if(cpu.CY) RET(); }
void RNC(){ if(!cpu.CY) RET(); }
void RP(){ if(!cpu.S) RET(); }
void RM(){ if(cpu.S) RET(); }
void RPE(){ if(cpu.P) RET(); }
void RPO(){ if(!cpu.P) RET(); }

void RST(int n){
    memory[--cpu.SP] = (cpu.PC+1)&0xFF;
    memory[--cpu.SP] = ((cpu.PC+1)>>8)&0xFF;
    cpu.PC = (n * 8) - 1;
}

void SPHL(){ cpu.SP = (cpu.H<<8)|cpu.L; }
void PCHL(){ cpu.PC = ((cpu.H<<8)|cpu.L) - 1; }

void CMA(){ cpu.A = ~cpu.A; }
void CMC(){ cpu.CY = !cpu.CY; }
void STC(){ cpu.CY = 1; }

void DAA(){
    if((cpu.A & 0x0F) > 9) cpu.A += 6;
    if((cpu.A >> 4) > 9 || cpu.CY) {
        cpu.A += 0x60;
        cpu.CY = 1;
    }
    updateFlags(cpu.A);
}

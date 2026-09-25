#include "raylib.h"
#include "../simulator.h"

#define MAX_GUI_LINES 30

char guiProgram[MAX_GUI_LINES][100];
int guiLines = 0;

int running = 0;
int halted = 0;
int loadMessage = 0;

int memoryAddress = 0;
char memoryInput[10] = "";
int memoryInputLength = 0;
int memoryInputActive = 0;

int selectedLine = 0;
char editBuffer[100] = "";
int editLength = 0;


/* Load assembly code only for displaying it in the GUI */
void loadAssemblyFile() {

    FILE *file = fopen("program.asm", "r");

    if (file == NULL) {
        return;
    }

    guiLines = 0;

    while (guiLines < MAX_GUI_LINES &&
           fgets(guiProgram[guiLines], 100, file) != NULL) {

        guiProgram[guiLines][strcspn(guiProgram[guiLines], "\n")] = '\0';

        if (strlen(guiProgram[guiLines]) > 0) {
            guiLines++;
        }
    }

    fclose(file);
}


/* Load program into the actual simulator */
void loadSimulatorProgram() {

    FILE *file = fopen("program.asm", "r");

    if (file == NULL) {
        return;
    }

    lines = 0;

    while (lines < MAX_PROGRAM_LINES &&
           fgets(program[lines], 100, file) != NULL) {

        program[lines][strcspn(program[lines], "\n")] = '\0';

        if (strlen(program[lines]) > 0) {
            lines++;
        }
    }

    fclose(file);
}


/* Reset the simulator */
void resetSimulator() {

    initCPU();

    instructionCount = 0;

    running = 0;
    halted = 0;

    loadSimulatorProgram();
}


/* Execute one instruction */
void executeStep() {

    if (cpu.PC >= lines) {
        running = 0;
        halted = 1;
        return;
    }

    parseInstruction(program[cpu.PC]);

    instructionCount++;

    cpu.PC++;

    if (cpu.PC >= lines) {
        running = 0;
        halted = 1;
    }
}


int main() {

    initCPU();

    loadAssemblyFile();
    loadSimulatorProgram();

    if (guiLines > 0) {
        strcpy(editBuffer, guiProgram[0]);
        editLength = strlen(editBuffer);
    }

    InitWindow(1200, 700, "8085 Simulator");

    SetTargetFPS(60);


    while (!WindowShouldClose()) {

        /*
         * Button positions
         */

        Rectangle runButton =
            (Rectangle){70, 525, 100, 40};

        Rectangle stepButton =
            (Rectangle){270, 525, 100, 40};

        Rectangle resetButton =
            (Rectangle){470, 525, 100, 40};

        Rectangle stopButton =
            (Rectangle){670, 525, 100, 40};

        Rectangle loadButton =
            (Rectangle){790, 525, 100, 40};


        /*
         * Memory address input
         */

        if (memoryInputActive) {

            int key = GetCharPressed();

            while (key > 0) {

                if ((key >= '0' && key <= '9') ||
                    (key >= 'A' && key <= 'F') ||
                    (key >= 'a' && key <= 'f')) {

                    if (memoryInputLength < 4) {
                        memoryInput[memoryInputLength++] = (char)key;
                        memoryInput[memoryInputLength] = '\0';
                    }
                }

                key = GetCharPressed();
            }

            if (IsKeyPressed(KEY_BACKSPACE) &&
                memoryInputLength > 0) {

                memoryInputLength--;
                memoryInput[memoryInputLength] = '\0';
            }

            if (IsKeyPressed(KEY_ENTER) &&
                memoryInputLength > 0) {

                memoryAddress =
                    (int)strtol(memoryInput, NULL, 16);

                if (memoryAddress >= MEM_SIZE) {
                    memoryAddress = MEM_SIZE - 1;
                }

                memoryInputActive = 0;
            }
        }


        /*
         * Assembly editor keyboard input
         */

        if (IsKeyPressed(KEY_UP)) {

            if (selectedLine > 0) {
                selectedLine++;
                selectedLine--;
            }

            editBuffer[0] = '\0';
            editLength = 0;
        }

        if (IsKeyPressed(KEY_DOWN)) {

            if (selectedLine < guiLines - 1) {
                selectedLine++;
            }

            editBuffer[0] = '\0';
            editLength = 0;
        }

        int assemblyKey = GetCharPressed();

        while (assemblyKey > 0) {

            if (assemblyKey >= 32 &&
                assemblyKey <= 126 &&
                editLength < 98) {

                editBuffer[editLength++] =
                    (char)assemblyKey;

                editBuffer[editLength] = '\0';
            }

            assemblyKey = GetCharPressed();
        }

        if (IsKeyPressed(KEY_BACKSPACE) &&
            editLength > 0) {

            editLength--;

            editBuffer[editLength] = '\0';
        }

        if (IsKeyPressed(KEY_ENTER) &&
            guiLines > 0 &&
            editLength > 0) {

            strcpy(
                guiProgram[selectedLine],
                editBuffer
            );
        }


        /*
         * Mouse input
         */

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {

            Vector2 mouse = GetMousePosition();


            Rectangle memoryInputBox =
                (Rectangle){850, 605, 250, 30};

            if (CheckCollisionPointRec(mouse, memoryInputBox)) {
                memoryInputActive = 1;
            }

            if (CheckCollisionPointRec(mouse, runButton)) {

                running = 1;
                halted = 0;
            }


            if (CheckCollisionPointRec(mouse, stepButton)) {

                executeStep();
            }


            if (CheckCollisionPointRec(mouse, resetButton)) {

                resetSimulator();
            }


            if (CheckCollisionPointRec(mouse, stopButton)) {

                running = 0;
            }

            if (CheckCollisionPointRec(mouse, loadButton)) {

                for (int i = 0; i < guiLines; i++) {
                    strcpy(program[i], guiProgram[i]);
                }

                lines = guiLines;

                initCPU();

                instructionCount = 0;

                running = 0;
                halted = 0;

                selectedLine = 0;

                if (guiLines > 0) {
                    strcpy(editBuffer, guiProgram[0]);
                    editLength = strlen(editBuffer);
                }

                loadMessage = 1;
            }

        }


        /*
         * RUN mode
         *
         * Currently executes one instruction
         * per frame.
         */

        if (running && !halted) {

            executeStep();
        }


        /*
         * Drawing
         */

        BeginDrawing();

        ClearBackground(
            (Color){18, 18, 24, 255}
        );


        /*
         * Title
         */

        DrawText(
            "8085 MICROPROCESSOR SIMULATOR",
            30,
            25,
            28,
            RAYWHITE
        );


        /*
         * Assembly Code Panel
         */

        DrawRectangle(
            30,
            80,
            700,
            400,
            (Color){30, 30, 40, 255}
        );

        DrawText(
            "ASSEMBLY CODE",
            50,
            100,
            20,
            RAYWHITE
        );


        for (int i = 0; i < guiLines; i++) {

            char line[120];

            if (i == selectedLine && editLength > 0) {

                sprintf(
                    line,
                    "%02d   %s",
                    i + 1,
                    editBuffer
                );

            } else {

                sprintf(
                    line,
                    "%02d   %s",
                    i + 1,
                    guiProgram[i]
                );
            }


            /*
             * Highlight currently executing line
             */

            if (i == cpu.PC && !halted) {

                DrawRectangle(
                    45,
                    135 + i * 30,
                    650,
                    28,
                    (Color){80, 65, 20, 255}
                );

                DrawText(
                    line,
                    60,
                    140 + i * 30,
                    20,
                    YELLOW
                );

            }
            else if (i == selectedLine) {

                DrawRectangle(
                    45,
                    135 + i * 30,
                    650,
                    28,
                    (Color){50, 70, 100, 255}
                );

                DrawText(
                    line,
                    60,
                    140 + i * 30,
                    20,
                    SKYBLUE
                );

            }
            else {

                DrawText(
                    line,
                    60,
                    140 + i * 30,
                    20,
                    LIGHTGRAY
                );
            }
        }


        /*
         * CPU Registers Panel
         */

        DrawRectangle(
            760,
            80,
            400,
            250,
            (Color){30, 30, 40, 255}
        );

        DrawText(
            "CPU REGISTERS",
            780,
            100,
            20,
            RAYWHITE
        );


        char text[50];


        sprintf(
            text,
            "A     %02XH",
            cpu.A
        );

        DrawText(
            text,
            790,
            145,
            20,
            LIGHTGRAY
        );


        sprintf(
            text,
            "B     %02XH",
            cpu.B
        );

        DrawText(
            text,
            790,
            175,
            20,
            LIGHTGRAY
        );


        sprintf(
            text,
            "C     %02XH",
            cpu.C
        );

        DrawText(
            text,
            790,
            205,
            20,
            LIGHTGRAY
        );


        sprintf(
            text,
            "D     %02XH",
            cpu.D
        );

        DrawText(
            text,
            790,
            235,
            20,
            LIGHTGRAY
        );


        sprintf(
            text,
            "E     %02XH",
            cpu.E
        );

        DrawText(
            text,
            950,
            145,
            20,
            LIGHTGRAY
        );


        sprintf(
            text,
            "H     %02XH",
            cpu.H
        );

        DrawText(
            text,
            950,
            175,
            20,
            LIGHTGRAY
        );


        sprintf(
            text,
            "L     %02XH",
            cpu.L
        );

        DrawText(
            text,
            950,
            205,
            20,
            LIGHTGRAY
        );


        sprintf(
            text,
            "PC    %04XH",
            cpu.PC
        );

        DrawText(
            text,
            950,
            235,
            20,
            LIGHTGRAY
        );


        sprintf(
            text,
            "SP    %04XH",
            cpu.SP
        );

        DrawText(
            text,
            790,
            275,
            20,
            LIGHTGRAY
        );


        /*
         * Flags Panel
         */

        DrawRectangle(
            760,
            350,
            400,
            130,
            (Color){30, 30, 40, 255}
        );

        DrawText(
            "FLAGS",
            780,
            370,
            20,
            RAYWHITE
        );


        sprintf(
            text,
            "S     %d",
            cpu.S
        );

        DrawText(
            text,
            790,
            415,
            18,
            LIGHTGRAY
        );


        sprintf(
            text,
            "Z     %d",
            cpu.Z
        );

        DrawText(
            text,
            880,
            415,
            18,
            LIGHTGRAY
        );


        sprintf(
            text,
            "AC    %d",
            cpu.AC
        );

        DrawText(
            text,
            970,
            415,
            18,
            LIGHTGRAY
        );


        sprintf(
            text,
            "P     %d",
            cpu.P
        );

        DrawText(
            text,
            790,
            445,
            18,
            LIGHTGRAY
        );


        sprintf(
            text,
            "CY    %d",
            cpu.CY
        );

        DrawText(
            text,
            880,
            445,
            18,
            LIGHTGRAY
        );


        /*
         * Control Panel
         */

        DrawRectangle(
            30,
            510,
            1130,
            80,
            (Color){30, 30, 40, 255}
        );


        DrawRectangleRec(
            runButton,
            DARKGREEN
        );

        DrawRectangleRec(
            stepButton,
            DARKBLUE
        );

        DrawRectangleRec(
            resetButton,
            ORANGE
        );

        DrawRectangleRec(
            stopButton,
            MAROON
        );

        DrawRectangleRec(
            loadButton,
            DARKPURPLE
        );


        DrawText(
            "RUN",
            95,
            536,
            20,
            RAYWHITE
        );


        DrawText(
            "STEP",
            290,
            536,
            20,
            RAYWHITE
        );


        DrawText(
            "RESET",
            490,
            536,
            20,
            RAYWHITE
        );


        DrawText(
            "STOP",
            695,
            536,
            20,
            RAYWHITE
        );

        DrawText(
            "LOAD",
            815,
            536,
            20,
            RAYWHITE
        );


        if (loadMessage) {

            DrawText(
                "PROGRAM LOADED",
                790,
                575,
                18,
                GREEN
            );
        }


        /*
         * Execution status
         */

        if (halted) {

            DrawText(
                "STATUS: HALTED",
                930,
                535,
                20,
                RED
            );

        } else if (running) {

            DrawText(
                "STATUS: RUNNING",
                930,
                535,
                20,
                GREEN
            );

        } else {

            DrawText(
                "STATUS: READY",
                930,
                535,
                20,
                SKYBLUE
            );
        }


        /*
         * Memory Panel
         */

        DrawRectangle(
            30,
            600,
            1130,
            120,
            (Color){30, 30, 40, 255}
        );

        DrawText(
            "MEMORY",
            50,
            610,
            18,
            RAYWHITE
        );

        DrawRectangle(
            920,
            605,
            250,
            30,
            (Color){20, 20, 28, 255}
        );

        DrawText(
            "ADDR:",
            860,
            612,
            14,
            RAYWHITE
        );

        DrawText(
            memoryInput,
            920,
            612,
            14,
            YELLOW
        );

        char memoryText[120];

        for (int row = 0; row < 4; row++) {

            int address = memoryAddress + row * 16;

            if (address >= MEM_SIZE)
                break;

            sprintf(
                memoryText,
                "%04X: %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X",
                address,
                memory[address],
                memory[address + 1],
                memory[address + 2],
                memory[address + 3],
                memory[address + 4],
                memory[address + 5],
                memory[address + 6],
                memory[address + 7],
                memory[address + 8],
                memory[address + 9],
                memory[address + 10],
                memory[address + 11],
                memory[address + 12],
                memory[address + 13],
                memory[address + 14],
                memory[address + 15]
            );

            DrawText(
                memoryText,
                50,
                635 + row * 20,
                14,
                LIGHTGRAY
            );
        }


        EndDrawing();
    }


    CloseWindow();

    return 0;
}

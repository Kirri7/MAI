#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef enum {
    SUCCESS = 0,
    INCORRECT_INPUT,
    OVERFLOW_ERROR,
    MALLOC_ERROR,
    FILE_OPENING_ERROR,
    FILE_READING_ERROR,
    INSTRUCTION_ERROR,
    UNKNOWN_ERROR
} ErrorCode;

static const char* errorMessages[] = {
    "Всё хорошо, можно идти пить чай ☕",
    "Некорректный ввод, попробуйте ещё раз 🤨",
    "Произошло переполнение, ой 🤯",
    "Проблемы с выделением памяти, грустно 😐",
    "Не удалось открыть файл, грустно 😥",
    "Файл прочитан не полностью, грустно 😿",
    "Не удалось выполнить инструкцию, грустно 😿",
    "Неизвестная ошибка, что-то пошло не так 🫢"
};

#define BUFFER_SIZE 100
#define VAR_SIZE 50

typedef struct {
    char* variable;
    int data;
} MemoryCell;

typedef struct {
    MemoryCell* args;
    char* operation;
} Instruction;

typedef struct {
    MemoryCell* data;
    int size;
} CellsTable;

int compareCells(const void* a, const void *b) {
    return strcmp(((MemoryCell*)a)->variable, ((MemoryCell*)b)->variable);
}

int isOperation(char chr) {
    switch (chr) {
        case '+':
        case '-':
        case '/':
        case '%':
        case '*':
            return 1;
        default:
            return 0;
    }
}

ErrorCode addToCellsTable(CellsTable* cellsTable, MemoryCell node) {
    if (!cellsTable)
        return INCORRECT_INPUT;

    int size = cellsTable->size;

    MemoryCell* tmp = (MemoryCell*)realloc(cellsTable->data, sizeof(MemoryCell) * (size + 1));
    if (!tmp)
        return MALLOC_ERROR;

    cellsTable->data = tmp;

    cellsTable->data[cellsTable->size].variable = (char*)malloc(sizeof(char) * sizeof(node.variable));

    if (!cellsTable->data[cellsTable->size].variable) {
        free(tmp);
        return MALLOC_ERROR;
    }

    strcpy(cellsTable->data[cellsTable->size].variable, node.variable);
    cellsTable->data[cellsTable->size].data = node.data;
    cellsTable->size++;
    qsort(cellsTable->data, cellsTable->size, sizeof(MemoryCell), compareCells);
    return SUCCESS;
}

ErrorCode doOperation(int num1, int num2, char operation, int* result) {
    switch (operation) {
        case '+':
            *result = num1 + num2;
            break;
        case '-':
            *result = num1 - num2;
            break;
        case '*':
            *result = num1 * num2;
            break;
        case '/':
            if (num2 == 0)
                return INSTRUCTION_ERROR;
            *result = num1 / num2;
            break;
        case '%':
            if (num2 == 0)
                return INSTRUCTION_ERROR;
            *result = num1 % num2;
            break;
        default:
            return INSTRUCTION_ERROR;
    }
    
    return SUCCESS;
}

int convertToDigit(const char* string, int* number) {
    int digit;
    int num = 0;
    for (size_t i = 0; i < strlen(string); ++i) {
        if (isdigit(string[i])) {
            digit = string[i] - '0';
            num = num * 10 + digit;
        } else {
            return 0;
        }
    }
    *number = num;
    return 1;
}


int binSearch(CellsTable* state, const char* varName) {
    int a = 0; int b = state->size - 1, m, cmp;
    if(state->size < 0) {
        return -1;
    }
    while (a <= b) {
        m = (a + b) / 2;
        cmp = strcmp(state->data[m].variable, varName);
        if (cmp == 0) {
            return m;
        } else if (cmp < 0) {
            a = m + 1;
        } else {
            b = m - 1;
        }
    }
    return -1; 
}

ErrorCode printNoVar(char *s) {
    printf("Такой переменной '%s' нет в списке\n", s);
    fflush(stdout);
    return INSTRUCTION_ERROR;
}

ErrorCode doInstructions(CellsTable* cellsTable, const Instruction* instructions, int instructionsCnt) {
    if (!cellsTable || !instructions || instructionsCnt <= 0)
        return INCORRECT_INPUT;

    for (int instructionInd = 0; instructionInd < instructionsCnt; ++instructionInd) {
        Instruction instruction = instructions[instructionInd];
        
        if (strcmp(instruction.operation, "print") == 0) {
            if (instruction.args) {
                int index = binSearch(cellsTable, instruction.args[0].variable);
                if (index != -1) {
                    printf("%s = %d\n", instruction.args[0].variable, cellsTable->data[index].data);
                } else {
                    return printNoVar(instruction.args[0].variable);
                }
            } else if (!instruction.args) {
                for (int i = 0; i < cellsTable->size; ++i) {
                    printf("%s = %d\n", cellsTable->data[i].variable, cellsTable->data[i].data);
                }
            }
        } else {
            int num1, num2, num, num1Index, num2Index;
            int flag = 0;
            if (isOperation(instruction.operation[1])) {
                if (convertToDigit(instruction.args[2].variable, &num2) == 0) {
                    num2Index = binSearch(cellsTable, instruction.args[2].variable);
                    if (num2Index == -1) {
                        return printNoVar(instruction.args[2].variable);
                    }
                    num2 = (*cellsTable).data[num2Index].data;
                }
                if (convertToDigit(instruction.args[1].variable, &num1) == 0) {
                    num1Index = binSearch(cellsTable, instruction.args[1].variable);
                    if (num1Index == -1) {
                        return printNoVar(instruction.args[1].variable);
                    }
                    num1 = (*cellsTable).data[num1Index].data;
                }
                ErrorCode code = doOperation(num1, num2, instruction.operation[1], &num);
                if (code != SUCCESS)
                    return INSTRUCTION_ERROR;
                flag = 1;
            }
            if (instruction.operation[0] == '=') {
                int variableIndex;
                int operandInd;
                MemoryCell variableNode;
                if ((variableIndex = binSearch(cellsTable, instruction.args[0].variable)) == -1) {
                    variableNode.variable = (char*)malloc(sizeof(char) * sizeof(instruction.args[0].variable));
                    if (!variableNode.variable)
                        return MALLOC_ERROR;

                    strcpy(variableNode.variable, instruction.args[0].variable);
                    if (flag) { // попалось число
                        variableNode.data = num;
                    } else if (convertToDigit(instruction.args[1].variable, &num) == 1) {
                        variableNode.data = num;
                    } else {
                        if ((operandInd = binSearch(cellsTable, instruction.args[1].variable)) == -1) {
                            return printNoVar(instruction.args[1].variable);
                        }
                        variableNode.data = cellsTable->data[operandInd].data;
                    }

                    if (addToCellsTable(cellsTable, variableNode) != SUCCESS) {
                        free(variableNode.variable);
                        return INSTRUCTION_ERROR;
                    }

                    free(variableNode.variable);
                } else {
                    if (flag) {
                        cellsTable->data[variableIndex].data = num;
                    } else if (convertToDigit(instruction.args[1].variable, &num) == 1) {
                        cellsTable->data[variableIndex].data = num;
                    } else {
                        if ((operandInd = binSearch(cellsTable, instruction.args[1].variable)) == -1) {
                            return printNoVar(instruction.args[1].variable);
                        }
                        cellsTable->data[variableIndex].data = cellsTable->data[operandInd].data;
                    }
                }
            } else {
                return INCORRECT_INPUT;
            }
        }
    }
    return SUCCESS;
}

ErrorCode processFile(char* filePath, Instruction** instructions, int* instrCnt) {
    if (!filePath || !instructions || !instrCnt)
        return INCORRECT_INPUT;
    
    FILE* fileIn = fopen(filePath, "r");

    if (!fileIn)
        return FILE_OPENING_ERROR;

    int instructsCnt = 0;
    char buffer[BUFFER_SIZE];
    int bufSize = sizeof(buffer);

    while(fgets(buffer, bufSize, fileIn)) {
        // читаем строку и готовимся к обработке (инициализируем переменные)
        Instruction newInstruction;
        int operationNum = 0;
        int numOfVars = 0;

        char* variable = (char*)malloc(sizeof(char) * VAR_SIZE);
        if (!variable) {
            fclose(fileIn);
            return MALLOC_ERROR;
        }

        char* operand1 = (char*)malloc(sizeof(char) * VAR_SIZE);
        if (!operand1) {
            free(variable);
            fclose(fileIn);
            return MALLOC_ERROR;
        }

        char* operand2 = (char*)malloc(sizeof(char) * VAR_SIZE);
        if (!operand2) {
            free(variable);
            free(operand1);
            fclose(fileIn);
            return MALLOC_ERROR;
        }

        char* printOper = (char*)malloc(sizeof(char) * 5);
        if (printOper == NULL) {
            free(variable);
            free(operand1);
            free(operand2);
            fclose(fileIn);
            return MALLOC_ERROR;
        }

        char operation;
        // %[^;] -> чтение до ;
        if (sscanf(buffer, "%s = %s %c %[^;];", variable, operand1, &operation, operand2) == 4) {
                // todo realloc
                variable = (char*)realloc(variable, sizeof(char) * (strlen(variable) + 1));
                if (!variable) {
                    free(operand1); 
                    free(operand2);
                    fclose(fileIn);
                    return MALLOC_ERROR;
                }

                operand1 = (char*)realloc(operand1, sizeof(char) * (strlen(operand1) + 1));
                if (!operand1) {
                    free(operand2);
                    free(variable); 
                    fclose(fileIn);
                    return MALLOC_ERROR;
                }

                operand2 = (char*)realloc(operand2, sizeof(char) * (strlen(operand2) + 1));
                if (!operand2) {
                    free(operand1); 
                    free(variable);
                    fclose(fileIn);
                    return MALLOC_ERROR;
                }

                // operation is true
                if (isOperation(operation)) {
                    newInstruction.operation = (char*)malloc(sizeof(char) * 2);
                    if (!newInstruction.operation) {
                        free(variable);
                        free(operand1);
                        free(operand2);
                        fclose(fileIn);
                        return MALLOC_ERROR;
                    }

                    newInstruction.operation[operationNum++] = '=';
                    newInstruction.operation[operationNum++] = operation;

                    MemoryCell variable_node, operand_node_f, operand_node_s;
                    
                    variable_node.variable = (char*)malloc(sizeof(variable) * sizeof(char));

                    if (variable_node.variable == NULL) {
                        free(variable);
                        free(operand1);
                        free(operand2);
                        free(newInstruction.operation);
                        fclose(fileIn);
                        return MALLOC_ERROR;
                    }

                    operand_node_f.variable = (char*)malloc(sizeof(operand1) * sizeof(char));

                    if (!operand_node_f.variable) {
                        free(variable);
                        free(operand1);
                        free(operand2);
                        free(newInstruction.operation);
                        free(variable_node.variable);
                        fclose(fileIn);
                        return MALLOC_ERROR;
                    }

                    operand_node_s.variable = (char*)malloc(sizeof(operand2) * sizeof(char));
                    if (!operand_node_s.variable) {
                        free(variable);
                        free(operand1);
                        free(operand2);
                        free(newInstruction.operation);
                        free(operand_node_f.variable);
                        free(variable_node.variable);
                        fclose(fileIn);
                        return MALLOC_ERROR;
                    }

                    strcpy(variable_node.variable, variable);
                    strcpy(operand_node_f.variable, operand1);
                    strcpy(operand_node_s.variable, operand2);
                    
                    newInstruction.args = (MemoryCell*)malloc(sizeof(MemoryCell) * 3);
                    if (!newInstruction.args) {
                        free(variable);
                        free(operand1);
                        free(operand2);
                        free(newInstruction.operation);
                        free(operand_node_f.variable);
                        free(operand_node_s.variable);
                        free(variable_node.variable);
                        fclose(fileIn);
                        return MALLOC_ERROR;
                    }

                    newInstruction.args[numOfVars++] = variable_node;
                    newInstruction.args[numOfVars++] = operand_node_f;
                    newInstruction.args[numOfVars++] = operand_node_s;

                    free(variable);
                    free(operand1);
                    free(operand2);
                } else {
                    free(variable);
                    free(operand1);
                    free(operand2);
                    return INCORRECT_INPUT;
                }
        } else if (sscanf(buffer, "%s = %[^;];", variable, operand1) == 2) {
                // подбиваем память под имена переменных
                variable = (char*)realloc(variable, sizeof(char) * (strlen(variable) + 1));
                operand1 = (char*)realloc(operand1, sizeof(char) * (strlen(operand1) + 1));
                //todo realloc
                MemoryCell varNode, operand1Node;

                newInstruction.operation = (char*)malloc(sizeof(char));
                if (!newInstruction.operation) {
                    free(variable);
                    free(operand1);
                    fclose(fileIn);
                    return MALLOC_ERROR;
                }
                newInstruction.operation[operationNum++] = '=';

                varNode.variable = (char*)malloc((strlen(variable) + 1) * sizeof(char));
                if (!varNode.variable) {
                    free(variable);
                    free(operand1);
                    free(newInstruction.operation);
                    fclose(fileIn);
                    return MALLOC_ERROR;
                }

                strcpy(varNode.variable, variable);

                operand1Node.variable = (char*)malloc((strlen(operand1) + 1) * sizeof(char));
                if (!varNode.variable) {
                    free(variable);
                    free(operand1);
                    free(newInstruction.operation);
                    free(varNode.variable);
                    fclose(fileIn);
                    return MALLOC_ERROR;
                }

                strcpy(operand1Node.variable, operand1); 

                newInstruction.args = (MemoryCell*)malloc(sizeof(MemoryCell) * 2);
                if (!newInstruction.args) {
                    free(variable);
                    free(operand1);
                    free(newInstruction.operation);
                    free(operand1Node.variable);
                    free(varNode.variable);
                    fclose(fileIn);
                    return MALLOC_ERROR;
                }

                newInstruction.args[numOfVars++] = varNode;
                newInstruction.args[numOfVars++] = operand1Node;
                free(variable);
                free(operand1);
        } else if (sscanf(buffer, "%s %[^;];", printOper, variable) == 2) {
            variable = (char*)realloc(variable, sizeof(char) * (strlen(variable) + 1));
            //todo realloc
            if(strcmp(printOper, "print") == 0) {

                newInstruction.operation = (char*)malloc(sizeof(char) * sizeof(printOper));
                if (!newInstruction.operation) {
                    free(variable);
                    free(operand1);
                    free(operand2);
                    free(printOper);
                    fclose(fileIn);
                    return MALLOC_ERROR;
                }

                strcpy(newInstruction.operation, printOper);
                ++operationNum;

                MemoryCell variableNode;
                newInstruction.args = (MemoryCell*)malloc(sizeof(MemoryCell));
                if (!newInstruction.args) {
                    free(variable);
                    free(operand1);
                    free(operand2);
                    free(printOper);
                    free(newInstruction.operation);
                    fclose(fileIn);
                    return MALLOC_ERROR;
                }

                variableNode.variable = (char*)malloc(sizeof(char) * sizeof(variable)); // todo sus
                if (!newInstruction.args->variable) {
                    free(variable);
                    free(operand1);
                    free(operand2);
                    free(printOper);
                    free(newInstruction.operation);
                    free(newInstruction.args);
                    fclose(fileIn);
                    return MALLOC_ERROR;
                }

                strcpy(variableNode.variable, variable);
                newInstruction.args[numOfVars++] = variableNode;
                free(printOper);
                free(variable);
            } else {
                free(variable);
                free(operand1);
                free(operand2);
                free(printOper);
                fclose(fileIn);
                return INCORRECT_INPUT;
            }
        } else if (sscanf(buffer, "%[^;];", printOper) == 1) {
            if (strcmp(printOper, "print") == 0) {
                newInstruction.operation = (char*)malloc(sizeof(printOper) * sizeof(char)); // todo sus
                if (!newInstruction.operation) {
                    free(variable);
                    free(operand1);
                    free(operand2);
                    free(printOper);
                    fclose(fileIn);
                    return MALLOC_ERROR;
                }

                strcpy(newInstruction.operation, printOper);
                ++operationNum;
                newInstruction.args = NULL;
            } else {
                free(variable);
                free(operand1);
                free(operand2);
                free(printOper);
                fclose(fileIn);
                return INCORRECT_INPUT;
            }
        } else {
            free(variable);
            free(operand1);
            free(operand2);
            free(printOper);
            fclose(fileIn);
            return INCORRECT_INPUT;
        }

        // добавление новой инструкции
        if (*instructions == NULL) {
            *instructions = (Instruction*)malloc(sizeof(Instruction) * (instructsCnt+1));
            if (!(*instructions)) {
                fclose(fileIn);
                return MALLOC_ERROR;
            }
        } else {
            Instruction* temp = (Instruction*)realloc(*instructions, sizeof(Instruction) * (instructsCnt+1));
            if (!temp) {
                fclose(fileIn);
                return MALLOC_ERROR;
            }
            *instructions = temp;
        }
        (*instructions)[instructsCnt++] = newInstruction;
    }
    *instrCnt = instructsCnt;
    fclose(fileIn);
    return SUCCESS;
}

int main (int argc, char* argv[]) {
    if (argc != 2) {
        printf("%s <файлик_с_верными_инструкциями.txt>\n", argv[0]);
        return INCORRECT_INPUT;
    }

    char* filePath = argv[1];
    int instructions_num;
    Instruction* instructions = NULL;
    CellsTable cellsTable;
    cellsTable.size = 0;
    cellsTable.data = NULL;

    ErrorCode code = processFile(filePath, &instructions, &instructions_num);
    for (int i = 0; i < instructions_num; ++i) {
        Instruction temp = instructions[i];
        printf("operation: %s,", temp.operation); //temp.node->variable
        // for (int j = 0; j < 3; ++j) {
        //     char *s = (temp.node[j].variable);
        //     printf("operand: %s, ", s);
        // }
        printf("\n");
        fflush(stdout);        
    }
    printf("\n\n\n");
    if (code == SUCCESS) {
        code = doInstructions(&cellsTable, instructions, instructions_num);
        if (code == SUCCESS) {
            printf("%s\n", errorMessages[SUCCESS]);
        } else {
            printf("%s\n", errorMessages[code]);
            return code;
        }
    } else {
        printf("%s\n", errorMessages[code]);
        return code;
    }
    return SUCCESS;
}
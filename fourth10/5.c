/*
в каждой строке 1 выражение
+ - * / % ^ ( )
вывести исходное, обратную польскую, значение
собственную реализацию стека через односвязный список
*/

#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef enum {
    SUCCESS = 0,
    INCORRECT_INPUT,
    OVERFLOW_ERROR,
    MALLOC_ERROR,
    FILE_OPENING_ERROR,
    FILE_READING_ERROR,
    OPEN_BRACE_MISSING,
    CLOSE_BRACE_MISSING,
    UNKNOWN_SYMBOL,
    DIVISION_BY_ZERO,
    UNKNOWN_ERROR,
} ErrorCode;

static const char* errorMessages[] = {
    "Всё хорошо, можно идти пить чай ☕",
    "Некорректный ввод, попробуйте ещё раз 🤨",
    "Произошло переполнение, ой 🤯",
    "Проблемы с выделением памяти, грустно 😐",
    "Не удалось открыть файл, грустно 😥",
    "Файл прочитан не полностью, грустно 😿",
    "Месье, у Вас '(' пропала из выражения 🧐",
    "Месье, у Вас ')' пропала из выражения 🧐",
    "Месье, найден неопознанный символ в выражении 🧐",
    "Месье, делить на ноль запрещенно 🧐",
    "Неизвестная ошибка, что-то пошло не так 🫢"
};

#define BUFFER_SIZE 100

typedef struct Node {
    int data;
    struct Node* next;
} Node;

typedef struct Stack {
    Node* top;
} Stack;

Node* createNode(const int data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

void initStack(Stack* stack) {
    stack->top = NULL;
}

int isEmpty(Stack* stack) {
    return (stack->top == NULL);
}

void push(Stack* stack, const int data) {
    Node* newNode = createNode(data);
    newNode->next = stack->top;
    stack->top = newNode;
}

int pop(Stack* stack) {
    if (isEmpty(stack)) {
        printf("Стек пуст 🪣\n");
        return -1;
    }
    int poppedData = stack->top->data;
    Node* temp = stack->top;
    stack->top = stack->top->next;
    free(temp);
    return poppedData;
}

int peek(Stack* stack) {
    if (isEmpty(stack)) {
        printf("Стек пуст 🪣\n");
        return -1;
    }
    return stack->top->data;
}

void deleteStack(Stack* stack) {
    while (stack->top != NULL) {
        Node* temp = stack->top;
        stack->top = stack->top->next;
        free(temp);
    }
}

int isoperator(const char chr) {
    switch (chr) {
        default:
            return 0;
        case '+':
        case '-':
        case '*':
        case '/':
        case '%':
        case '^':
            return 1;
    }
}

int opPr(const char chr) {
    switch(chr) {
        default:
            return -1;
        case '+':
        case '-':
            return 1;
        case '*':
        case '/':
        case '%':
            return 2;
        case '^':
            return 3;
    }
}

ErrorCode checkOutputOverflow(const int i, char output[BUFFER_SIZE]) {
    if (i >= BUFFER_SIZE-1) {
        output[BUFFER_SIZE-1] = '\0';
        return OVERFLOW_ERROR;
    }
    return SUCCESS;
}

ErrorCode shuntingYard(const char input[], int inpLen, char output[BUFFER_SIZE]) {
    Stack stack;
    initStack(&stack);
    memset(output, '\0', BUFFER_SIZE);
    int j = 0;

    for (int i = 0; i < inpLen; ++i) {
        if (checkOutputOverflow(j, output) != SUCCESS) {
            deleteStack(&stack);
            return OVERFLOW_ERROR;
        }
        char chr = input[i];

        if (isdigit(chr)) 
        {
            output[j++] = chr;
            while (isdigit(input[++i])) {
                output[j++] = input[i];
            }
            output[j++] = ' ';
            --i;
            continue;
        } 
        else if (isoperator(chr)) 
        {
            char opr1 = chr;
            char opr2 = ' ';
            if (!isEmpty(&stack))
                opr2 = peek(&stack);
            if (opr1 != '^') 
            {   // правоассоциативность
                while (!isEmpty(&stack) && opPr(opr1) <= opPr(opr2) && isoperator(opr2)) 
                {
                    opr2 = pop(&stack);
                    output[j++] = opr2;
                    output[j++] = ' ';
                    if (checkOutputOverflow(j, output) != SUCCESS) {
                        deleteStack(&stack);
                        return OVERFLOW_ERROR;
                    }
                    if (!isEmpty(&stack)) {
                        opr2 = peek(&stack);
                    } else {
                        break;
                    }
                }
                push(&stack, opr1);
            }
            else 
            {   // левоассоциативность
                while (!isEmpty(&stack) &&opPr(opr1) < opPr(opr2) && isoperator(opr2))
                {
                    opr2 = pop(&stack);
                    output[j++] = opr2;
                    output[j++] = ' ';
                    if (checkOutputOverflow(j, output) != SUCCESS) {
                        deleteStack(&stack);
                        return OVERFLOW_ERROR;
                    }
                    if (!isEmpty(&stack)) {
                        opr2 = peek(&stack);
                    } else {
                        break;
                    }
                }
                push(&stack, opr1);
            }
        }
        else if (chr == '(')
        {
            push(&stack, '(');
        }
        else if (chr == ')')
        {
            while (!isEmpty(&stack) && peek(&stack) != '(') {
                output[j++] = pop(&stack);
                output[j++] = ' ';
            }
            if (isEmpty(&stack)) {
                deleteStack(&stack);
                return OPEN_BRACE_MISSING;
            } else {
                pop(&stack);
            }
        }
        else if (!isspace(chr) && chr != '\0' && chr != '\n')
        {
            deleteStack(&stack);
            return UNKNOWN_SYMBOL;
        }
    }
    while (!isEmpty(&stack))
    {
        if (checkOutputOverflow(j, output) != SUCCESS) {
            deleteStack(&stack);
            return OVERFLOW_ERROR;
        }
        if (peek(&stack) == '(') {
            deleteStack(&stack);
            return CLOSE_BRACE_MISSING;
        }

        output[j++] = pop(&stack);
        output[j++] = ' ';
    }
    return SUCCESS;
}

void calcExit(const char input[], const size_t inpLen, Stack *stack) {
    deleteStack(stack);
    // for (size_t i = 0; i < inpLen-1; ++i) {
    //     if (input[i] == '\0') input[i] = ' ';
    // }
}

ErrorCode calculate(const char input2[], const size_t inpLen, int *result) {
    if (result == NULL)
        return INCORRECT_INPUT;
    Stack stack;
    initStack(&stack);
    char input[BUFFER_SIZE];
    for (size_t i = 0; i < inpLen; ++i)
        input[i] = input2[i];

    char *token = strtok(input, " ");
    while (token != NULL) {
        if (isdigit(token[0])) {
            char *endptr;
            long num = strtol(token, &endptr, 10);
            if (*endptr != '\0') {
                calcExit(input, inpLen, &stack);
                return INCORRECT_INPUT;
            }
            push(&stack, (int)num);
        } else {
            if (isEmpty(&stack)) {
                calcExit(input, inpLen, &stack);
                return INCORRECT_INPUT;
            }
            int operand2 = pop(&stack);
            if (isEmpty(&stack)) {
                calcExit(input, inpLen, &stack);
                return INCORRECT_INPUT;
            }
            int operand1 = pop(&stack);
            int res;
            switch (token[0]) {
                case '+':
                    res = operand1 + operand2;
                    break;
                case '-':
                    res = operand1 - operand2;
                    break;
                case '*':
                    res = operand1 * operand2;
                    break;
                case '/':
                    if (operand2 == 0) {
                        calcExit(input, inpLen, &stack);
                        return DIVISION_BY_ZERO;
                    }
                    res = operand1 / operand2;
                    break;
                case '%':
                    if (operand2 == 0) {
                        calcExit(input, inpLen, &stack);
                        return DIVISION_BY_ZERO;
                    }
                    res = operand1 % operand2;
                    break;
                case '^':
                    res = 1;
                    for (int i = 0; i < operand2; ++i) {
                        res *= operand1;
                    }
                    break;
                default:
                    calcExit(input, inpLen, &stack);
                    return INCORRECT_INPUT;
            }
            push(&stack, res);
        }
        token = strtok(NULL, " ");
    }

    if (isEmpty(&stack)) {
        calcExit(input, inpLen, &stack);
        return INCORRECT_INPUT;
    }
    *result = pop(&stack);
    if (!isEmpty(&stack)) {
        calcExit(input, inpLen, &stack);
        return INCORRECT_INPUT;
    }

    calcExit(input, inpLen, &stack);
    return SUCCESS;
}

ErrorCode openOutputFile(const char inputFile[], const int fileExists, FILE** file) {
    size_t inputLen = strnlen(inputFile, BUFFER_SIZE);
    if (inputLen >= BUFFER_SIZE - 7) {
        return INCORRECT_INPUT;
    }

    char outputFile[BUFFER_SIZE];
    strncpy(outputFile, inputFile, inputLen);
    strncpy(outputFile + inputLen, ".output", 7);

    if (fileExists) {
        *file = fopen(outputFile, "a");
    } else {
        *file = fopen(outputFile, "w");
    }
    if (*file == NULL) {
        return FILE_OPENING_ERROR;
    }

    return SUCCESS;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("%s <file1> <file2> ... <fileN>\n", argv[0]);
        return INCORRECT_INPUT;
    }

    for (int i = 1; i < argc; ++i) {
        FILE *file = fopen(argv[i], "r");
        if (file == NULL) {
            printf("%s: %s\n", argv[i], errorMessages[FILE_OPENING_ERROR]);
            continue;
        }
        printf("📄 Файл: %s\n", argv[i]);
        fflush(stdout);

        char line[BUFFER_SIZE];
        size_t lineLen = sizeof(line);
        memset(line, '\0', lineLen);
        int lineNum = 0;
        int hasOutFile = 0;
        while (fgets(line, sizeof(line), file) != NULL) {
            char output[BUFFER_SIZE];
            size_t outLen = sizeof(output);
            int res;
            printf(" Исходное: %s", line);

            ErrorCode problem = shuntingYard(line, lineLen, output);
            if (problem != SUCCESS) {
                FILE *out;
                ErrorCode code = openOutputFile(argv[i], hasOutFile, &out);
                switch (code) {
                    default:
                        fclose(file);
                        return code;
                    case SUCCESS:
                        hasOutFile = 1;
                        break;
                }
                fprintf(out, "выражение %d: %s -> ошибка '%s'\n", lineNum, line, errorMessages[problem]);
                fflush(out);
                fclose(out);
            } else {
                printf(" Обр-Поль: %s\n", output);

                problem = calculate(output, outLen, &res);
                if (problem != SUCCESS) {
                    FILE *out;
                    ErrorCode code = openOutputFile(argv[i], hasOutFile, &out);
                    switch (code) {
                        default:
                            fclose(file);
                            return code;
                        case SUCCESS:
                            hasOutFile = 1;
                            break;
                    }
                    fprintf(out, "выражение %d: %s -> ошибка '%s'\n", lineNum, line, errorMessages[problem]);
                    fflush(out);
                    fclose(out);
                } else {
                    printf(" = %d\n\n", res);
                }
            }

            memset(line, '\0', lineLen);
            ++lineNum;
        }

        fclose(file);
    }

    return SUCCESS;
}

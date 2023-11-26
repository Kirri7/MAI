#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

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

typedef struct SNode {
    int data;
    struct SNode* next;
} SNode;

typedef struct Stack {
    SNode* top;
} Stack;

SNode* createSNode(const int data) {
    SNode* newNode = (SNode*)malloc(sizeof(SNode));
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
    SNode* newNode = createSNode(data);
    newNode->next = stack->top;
    stack->top = newNode;
}

int pop(Stack* stack) {
    if (isEmpty(stack)) {
        printf("Стек пуст 🪣\n");
        return -1;
    }
    int poppedData = stack->top->data;
    SNode* temp = stack->top;
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
        SNode* temp = stack->top;
        stack->top = stack->top->next;
        free(temp);
    }
}

int isoperator(const char chr) {
    switch (chr) {
        default:
            return 0;
        case '&':
        case '|':
        case '~':
        case '-': // ->
        case '+': // +>
        case '<': // <>
        case '=':
        case '!':
        case '?':
            return 1;
    }
}

int opPr(const char chr) {
    switch(chr) {
        default:
            return -1;
        case '|':
        case '-':
        case '<':
        case '=':
            return 1;
        case '?':
        case '!':
        case '+':
        case '&':
            return 2;
        case '~':
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

        if (isdigit(chr) || isalpha(chr)) 
        {
            if (isdigit(input[i+1]) || isalpha(input[i+1])) {
                deleteStack(&stack);
                return INCORRECT_INPUT;
            }
            output[j++] = chr;
            // while (isdigit(input[++i]) || isalpha(input[i])) {
            //     output[j++] = input[i];
            // }
            // --i;
            output[j++] = ' ';
            continue;
        } 
        else if (isoperator(chr)) 
        {
            char opr1 = chr;
            char opr2 = ' ';
            if (!isEmpty(&stack))
                opr2 = peek(&stack);
            
            // if (opr1 != '^') все операции левоассоциативны
            {   // левоассоциативность
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
            // else 
            // {   // правоассоциативность
            //     while (!isEmpty(&stack) &&opPr(opr1) < opPr(opr2) && isoperator(opr2))
            //     {
            //         opr2 = pop(&stack);
            //         output[j++] = opr2;
            //         output[j++] = ' ';
            //         if (checkOutputOverflow(j, output) != SUCCESS) {
            //             deleteStack(&stack);
            //             return OVERFLOW_ERROR;
            //         }
            //         if (!isEmpty(&stack)) {
            //             opr2 = peek(&stack);
            //         } else {
            //             break;
            //         }
            //     }
            //     push(&stack, opr1);
            // }
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

struct Node {
    char data;
    int isNumber;
    struct Node* left;
    struct Node* right;
};

struct Node* createNode(int data, int isNumber) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    if (newNode == NULL)
        return NULL;
    newNode->data = data;
    newNode->isNumber = isNumber;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

void destroyTree(struct Node* root) {
    if (root == NULL)
        return;
    destroyTree(root->left);
    destroyTree(root->right);
    free(root);
}

// ЛКП - по возрастанию
void inorderTraversal(struct Node* root) {
    if (root == NULL)
        return;
    inorderTraversal(root->left);
    printf("%c ", root->data);
    inorderTraversal(root->right);
}

ErrorCode atExit(ErrorCode code, struct Node* root) {
    destroyTree(root);
    return code;
}

ErrorCode convertToExpressionTree(char* expression, struct Node** result) {
    struct Node* stack[BUFFER_SIZE];
    stack[0] = NULL;
    int top = -1;
    char* token = strtok(expression, " ");
    while (token != NULL) {
        if (isdigit(token[0]) || isalpha(token[0])) {
            struct Node* newNode = createNode(token[0], 1);
            if (newNode == NULL)
                return atExit(MALLOC_ERROR, stack[0]);
            
            stack[++top] = newNode;
        } else if (isoperator(token[0])) {
            if (top < 1) {
                return atExit(INCORRECT_INPUT, stack[0]);;
            }
            struct Node* newNode = createNode(token[0], 0);
            if (newNode == NULL)
                return atExit(MALLOC_ERROR, stack[0]);

            newNode->right = stack[top--];
            newNode->left = stack[top--];
            stack[++top] = newNode;
        } else {
            return atExit(UNKNOWN_SYMBOL, stack[0]);
        }
        token = strtok(NULL, " ");
    }
    if (top != 0) {
        return atExit(INCORRECT_INPUT, stack[0]);
    }
    *result = stack[0];
    return SUCCESS;
}

int evaluateNode(const struct Node* node, const int* input) {
    if (node->isNumber && isdigit(node->data)) {
        return node->data - '0';
    }
    else if (node->isNumber) {
        return input[node->data - 'A'];
    } 
    else {
        int left = evaluateNode(node->left, input);
        int right = evaluateNode(node->right, input);
        switch (node->data) {
            // ru.cppreference.com/w/cpp/language/operator_precedence
            // ~0 не эквивалентно !0
            // & <=> &&
            // | <=> ||
            case '&':
                return left & right;
            case '|':
                return left | right;
            case '~':
                return !left;
            case '-': // ->
                return !left | right;
            case '+': // +>
                // коимпликация (инверсия обратной импликации)
                return !( left | !right );
            case '<': // <>
                return left ^ right;
            case '=':
                return (left & right) | (!left & !right);
            case '!':
                return !(left & right);
            case '?':
                return !(left | right);
            default:
                break;
        }
    }
    return -1;
}

ErrorCode printTruthTable(const struct Node* root, const int numVariables, FILE* file) {
    // numVariables: A -> Z
    if (numVariables < 1 || numVariables > 26)
        return INCORRECT_INPUT;

    int input[26];
    for (int i = 0; i < numVariables; ++i) {
        fprintf(file, "%c ", 'A' + i);
    }
    fprintf(file, "=> Значение\n");
    for (int i = 0; i < (1 << numVariables); ++i) {
        for (int j = 0; j < numVariables; ++j) {
            // ABCD...Z <-> [1011...1]
            input[j] = (i >> (numVariables-1 - j)) & 1;
            fprintf(file, "%d ", input[j]);
        }
        fprintf(file, "=> %d\n", evaluateNode(root, input));
    }
    fflush(file);
    return SUCCESS;
}

ErrorCode compressOperators(char string[], size_t strLen) {
    for (size_t i = 0; i < strLen-1; ++i) {
        if (string[i] == '<' && string[i+1] != '>') {
            return INCORRECT_INPUT;
        } else if (string[i] == '-' && string[i+1] != '>') {
            return INCORRECT_INPUT;
        } else if (string[i] == '+' && string[i+1] != '>') {
            return INCORRECT_INPUT;
        } else if (string[i+1] == '>') {
            string[i+1] = ' ';
        }
    }
    return SUCCESS;
}

char generateRandomChar() {
    char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    return charset[rand() % (sizeof(charset)-1)];
}

void generateRandomFileName(char *fileName, size_t nameLen) {
    for (size_t i = 0; i < nameLen-5; ++i) {
        fileName[i] = generateRandomChar();
    }
    char ending[] = ".txt";
    for (size_t i = nameLen-5; i < nameLen; ++i) {
        fileName[i] = ending[i-(nameLen-5)];
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("%s <file>\n", argv[0]);
        return INCORRECT_INPUT;
    }

    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        printf("%s\n", errorMessages[FILE_OPENING_ERROR]);
        fflush(stdout);
        return FILE_OPENING_ERROR;
    }

    char line[BUFFER_SIZE];
    size_t lineLen = sizeof(line);
    memset(line, '\0', lineLen);
    if (fgets(line, sizeof(line), file) == NULL) {
        fclose(file);
        printf("%s\n", errorMessages[INCORRECT_INPUT]);
        fflush(stdout);
        return INCORRECT_INPUT;
    }
    fclose(file);

    printf("'%s'\n", line);
    fflush(stdout);

    if (compressOperators(line, lineLen) != SUCCESS) {
        printf("%s\n", errorMessages[INCORRECT_INPUT]);
        fflush(stdout);
        return INCORRECT_INPUT;
    }
    
    char expression[BUFFER_SIZE];
    ErrorCode code = shuntingYard(line, sizeof(line), expression);
    if (code != SUCCESS) {
        printf("%s\n", errorMessages[code]);
        fflush(stdout);
        return code;
    }

    printf("'%s'\n", expression);
    fflush(stdout);
    struct Node* root;
    convertToExpressionTree(expression, &root);

    char fileName[10];
    srand(time(NULL));
    generateRandomFileName(fileName, sizeof(fileName));
    printf("'%s'\n", fileName);
    fflush(stdout);

    FILE *out = fopen(fileName, "w");
    if (out == NULL) {
        printf("%s\n", errorMessages[FILE_OPENING_ERROR]);
        fflush(stdout);
        destroyTree(root);
        return FILE_OPENING_ERROR;
    }
    printTruthTable(root, 4, out);
    fclose(out);

    destroyTree(root);
    return 0;
}
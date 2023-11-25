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
    // todo
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
    // todo
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

struct Node {
    int data;
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
    printf("%d ", root->data);
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
        if (isdigit(token[0])) {
            char *endptr;
            int num = strtol(token, &endptr, 10);
            if (*endptr != '\0') 
                return atExit(INCORRECT_INPUT, stack[0]);
            
            struct Node* newNode = createNode(num, 1);
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


int main() {
    char expression[] = "12 25 + 3 * ";
    struct Node* root;
    convertToExpressionTree(expression, &root);

    printf("Inorder Traversal: ");
    inorderTraversal(root);
    printf("\n");

    destroyTree(root);

    return 0;
}
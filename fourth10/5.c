/*
в каждой строке 1 выражение
+ - * / % ^ ( )
вывести исходное, обратную польскую, значение
собственную реализацию стека через односвязный список
*/

#include <ctype.h>
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
    "Неизвестная ошибка, что-то пошло не так 🫢"
};

#define BUFFER_SIZE 100

typedef struct Node {
    char data;
    struct Node* next;
} Node;

typedef struct Stack {
    Node* top;
} Stack;

Node* createNode(char data) {
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

void push(Stack* stack, char data) {
    Node* newNode = createNode(data);
    newNode->next = stack->top;
    stack->top = newNode;
}

char pop(Stack* stack) {
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

char peek(Stack* stack) {
    if (isEmpty(stack)) {
        printf("Стек пуст 🪣\n");
        return -1;
    }
    return stack->top->data;
}

int isoperator(char chr) {
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

int opPr(char chr) {
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

ErrorCode shuntingYard(char input[], int inpLen, char output[BUFFER_SIZE]) {
    Stack stack;
    initStack(&stack);
    memset(output, '\0', BUFFER_SIZE);
    int j = 0;

    for (int i = 0; i < inpLen; ++i) {
        if (j+1 == BUFFER_SIZE) {
            output[j] = '\0';
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
                return OPEN_BRACE_MISSING;
            } else {
                pop(&stack);
            }
        }
        else if (!isspace(chr) && chr != '\0')
        {
            return UNKNOWN_SYMBOL;
        }
    }
    while (!isEmpty(&stack))
    {
        if (j+1 == BUFFER_SIZE) {
            output[j] = '\0';
            return OVERFLOW_ERROR;
        }
        if (peek(&stack) == '(')
            return CLOSE_BRACE_MISSING;

        output[j++] = pop(&stack);
        output[j++] = ' ';
    }
    return SUCCESS;
}

int main() {

    char input[] = "2 + 1%0";
    int inpLen = sizeof(input);

    char output[BUFFER_SIZE];

    ErrorCode code = shuntingYard(input, inpLen, output);




    return SUCCESS;
}
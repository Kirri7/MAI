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
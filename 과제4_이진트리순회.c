#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct Node {
    char data;
    struct Node* left;
    struct Node* right;
} Node;

typedef struct StackNode {
    Node* treeNode;
    struct StackNode* next;
} StackNode;

void push(StackNode** top, Node* node) {
    StackNode* newNode = (StackNode*)malloc(sizeof(StackNode));
    newNode->treeNode = node;
    newNode->next = *top;
    *top = newNode;
}

Node* pop(StackNode** top) {
    StackNode* temp;
    Node* node;

    if (*top == NULL)
        return NULL;

    temp = *top;
    node = temp->treeNode;
    *top = temp->next;
    free(temp);

    return node;
}

int isEmpty(StackNode* top) {
    return top == NULL;
}

Node* createNode(char data) {
    Node* newNode = (Node*)malloc(sizeof(Node));

    if (newNode == NULL) {
        printf("메모리 할당 오류\n");
        exit(1);
    }

    newNode->data = data;
    newNode->left = NULL;
    newNode->right = NULL;

    return newNode;
}

Node* buildTree(char* str, int* index, int* error) {
    Node* root;

    while (isspace(str[*index]))
        (*index)++;

    if (str[*index] == '\0') {
        *error = 1;
        return NULL;
    }

    if (!isalnum(str[*index])) {
        *error = 1;
        return NULL;
    }

    root = createNode(str[*index]);
    (*index)++;

    while (isspace(str[*index]))
        (*index)++;

    if (str[*index] == '(') {
        (*index)++;

        while (isspace(str[*index]))
            (*index)++;

        if (str[*index] == ',') {
            root->left = NULL;
        }
        else {
            root->left = buildTree(str, index, error);
            if (*error)
                return root;
        }

        while (isspace(str[*index]))
            (*index)++;

        if (str[*index] != ',') {
            *error = 1;
            return root;
        }

        (*index)++;

        while (isspace(str[*index]))
            (*index)++;

        if (str[*index] == ')') {
            root->right = NULL;
        }
        else {
            root->right = buildTree(str, index, error);
            if (*error)
                return root;
        }

        while (isspace(str[*index]))
            (*index)++;

        if (str[*index] != ')') {
            *error = 1;
            return root;
        }

        (*index)++;
    }

    return root;
}

void printTree(Node* root, int level) {
    int i;

    if (root == NULL)
        return;

    for (i = 0; i < level; i++)
        printf("  ");

    printf("%c\n", root->data);

    if (root->left != NULL)
        printTree(root->left, level + 1);

    if (root->right != NULL)
        printTree(root->right, level + 1);
}

void preorder(Node* root) {
    StackNode* stack = NULL;
    Node* current;

    if (root == NULL)
        return;

    push(&stack, root);

    while (!isEmpty(stack)) {
        current = pop(&stack);

        printf("%c ", current->data);

        if (current->right != NULL)
            push(&stack, current->right);

        if (current->left != NULL)
            push(&stack, current->left);
    }

    printf("\n");
}

void inorder(Node* root) {
    StackNode* stack = NULL;
    Node* current = root;

    while (current != NULL || !isEmpty(stack)) {

        while (current != NULL) {
            push(&stack, current);
            current = current->left;
        }

        current = pop(&stack);

        printf("%c ", current->data);

        current = current->right;
    }

    printf("\n");
}

void postorder(Node* root) {
    StackNode* stack1 = NULL;
    StackNode* stack2 = NULL;
    Node* current;

    if (root == NULL)
        return;

    push(&stack1, root);

    while (!isEmpty(stack1)) {
        current = pop(&stack1);
        push(&stack2, current);

        if (current->left != NULL)
            push(&stack1, current->left);

        if (current->right != NULL)
            push(&stack1, current->right);
    }

    while (!isEmpty(stack2)) {
        current = pop(&stack2);
        printf("%c ", current->data);
    }

    printf("\n");
}

void freeTree(Node* root) {
    StackNode* stack = NULL;
    Node* current;

    if (root == NULL)
        return;

    push(&stack, root);

    while (!isEmpty(stack)) {
        current = pop(&stack);

        if (current->left != NULL)
            push(&stack, current->left);

        if (current->right != NULL)
            push(&stack, current->right);

        free(current);
    }
}

int main() {
    char input[1000];
    int index = 0;
    int error = 0;
    Node* root;

    printf("괄호 표기법으로 이진트리를 입력하세요.\n");
    printf("예: A(B(D,E),C(,F))\n");
    printf("입력: ");

    fgets(input, sizeof(input), stdin);

    root = buildTree(input, &index, &error);

    while (isspace(input[index]))
        index++;

    if (error || input[index] != '\0') {
        printf("잘못된 이진트리 입력입니다.\n");
        freeTree(root);
        return 1;
    }

    printf("\n[입력된 이진트리]\n");
    printTree(root, 0);

    printf("\n[Preorder]\n");
    preorder(root);

    printf("\n[Inorder]\n");
    inorder(root);

    printf("\n[Postorder]\n");
    postorder(root);

    freeTree(root);

    return 0;
}
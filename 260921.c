#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 1000

char tree[MAX];
int used[MAX];

int nodeCountArray(int index) {
    if (index >= MAX || !used[index])
        return 0;

    return 1 + nodeCountArray(index * 2)
        + nodeCountArray(index * 2 + 1);
}

int leafCountArray(int index) {
    int count = 0;

    if (index >= MAX || !used[index])
        return 0;

    if (!used[index * 2] && !used[index * 2 + 1])
        return 1;

    count += leafCountArray(index * 2);
    count += leafCountArray(index * 2 + 1);

    return count;
}

int heightArray(int index) {
    int leftHeight, rightHeight;

    if (index >= MAX || !used[index])
        return 0;

    leftHeight = heightArray(index * 2);
    rightHeight = heightArray(index * 2 + 1);

    return (leftHeight > rightHeight ? leftHeight : rightHeight) + 1;
}

int degreeArray(int index) {
    int degree = 0;
    int left, right;
    int ld, rd;

    if (index >= MAX || !used[index])
        return 0;

    left = index * 2;
    right = index * 2 + 1;

    if (left < MAX && used[left])
        degree = 1;

    if (right < MAX && used[right])
        degree = 2;

    ld = degreeArray(left);
    rd = degreeArray(right);

    if (ld > degree)
        degree = ld;

    if (rd > degree)
        degree = rd;

    return degree;
}

void printArray(int index, int level) {
    int i;

    if (index >= MAX || !used[index])
        return;

    printArray(index * 2 + 1, level + 1);

    for (i = 0; i < level; i++)
        printf("    ");

    printf("%c\n", tree[index]);

    printArray(index * 2, level + 1);
}

int isCompleteArray() {
    int i;
    int foundEmpty = 0;

    for (i = 1; i < MAX; i++) {
        if (used[i]) {
            if (foundEmpty)
                return 0;
        }
        else {
            foundEmpty = 1;
        }
    }

    return 1;
}

int isFullArray(int index) {
    if (index >= MAX || !used[index])
        return 1;

    if (used[index * 2] != used[index * 2 + 1])
        return 0;

    return isFullArray(index * 2) &&
        isFullArray(index * 2 + 1);
}

int isSkewedArray(int index) {
    if (index >= MAX || !used[index])
        return 1;

    if (used[index * 2] && used[index * 2 + 1])
        return 0;

    return isSkewedArray(index * 2) &&
        isSkewedArray(index * 2 + 1);
}

typedef struct Node {
    char data;
    struct Node* left;
    struct Node* right;
} Node;

Node* createNode(char data) {
    Node* newNode = (Node*)malloc(sizeof(Node));

    newNode->data = data;
    newNode->left = NULL;
    newNode->right = NULL;

    return newNode;
}

int pos = 0;

Node* parseTree(char* str) {
    Node* node;
    char data;

    while (str[pos] == ' ')
        pos++;

    if (str[pos] == '\0' || str[pos] == ')')
        return NULL;

    if (str[pos] == ',') {
        pos++;
        return NULL;
    }

    data = str[pos++];
    node = createNode(data);

    if (str[pos] == '(') {
        pos++;

        if (str[pos] == ',') {
            node->left = NULL;
        }
        else {
            node->left = parseTree(str);
        }

        if (str[pos] == ',')
            pos++;

        if (str[pos] == ')') {
            node->right = NULL;
        }
        else {
            node->right = parseTree(str);
        }

        if (str[pos] == ')')
            pos++;
    }

    return node;
}

int nodeCountPointer(Node* root) {
    if (root == NULL)
        return 0;

    return 1 + nodeCountPointer(root->left)
        + nodeCountPointer(root->right);
}

int leafCountPointer(Node* root) {
    if (root == NULL)
        return 0;

    if (root->left == NULL && root->right == NULL)
        return 1;

    return leafCountPointer(root->left)
        + leafCountPointer(root->right);
}

int heightPointer(Node* root) {
    int leftHeight, rightHeight;

    if (root == NULL)
        return 0;

    leftHeight = heightPointer(root->left);
    rightHeight = heightPointer(root->right);

    return (leftHeight > rightHeight ? leftHeight : rightHeight) + 1;
}

int degreePointer(Node* root) {
    int degree = 0;
    int leftDegree, rightDegree;

    if (root == NULL)
        return 0;

    if (root->left != NULL)
        degree = 1;

    if (root->right != NULL)
        degree = 2;

    leftDegree = degreePointer(root->left);
    rightDegree = degreePointer(root->right);

    if (leftDegree > degree)
        degree = leftDegree;

    if (rightDegree > degree)
        degree = rightDegree;

    return degree;
}

void printPointer(Node* root, int level) {
    int i;

    if (root == NULL)
        return;

    printPointer(root->right, level + 1);

    for (i = 0; i < level; i++)
        printf("    ");

    printf("%c\n", root->data);

    printPointer(root->left, level + 1);
}

int checkCompletePointer(Node* root) {
    Node* queue[MAX];
    int front = 0;
    int rear = 0;
    int foundEmpty = 0;

    if (root == NULL)
        return 1;

    queue[rear++] = root;

    while (front < rear) {
        Node* current = queue[front++];

        if (current->left != NULL) {
            if (foundEmpty)
                return 0;

            queue[rear++] = current->left;
        }
        else {
            foundEmpty = 1;
        }

        if (current->right != NULL) {
            if (foundEmpty)
                return 0;

            queue[rear++] = current->right;
        }
        else {
            foundEmpty = 1;
        }
    }

    return 1;
}

int checkFullPointer(Node* root) {
    if (root == NULL)
        return 1;

    if ((root->left == NULL && root->right != NULL) ||
        (root->left != NULL && root->right == NULL))
        return 0;

    return checkFullPointer(root->left) &&
        checkFullPointer(root->right);
}

int checkSkewedPointer(Node* root) {
    if (root == NULL)
        return 1;

    if (root->left != NULL && root->right != NULL)
        return 0;

    return checkSkewedPointer(root->left) &&
        checkSkewedPointer(root->right);
}

void findArray(int index, char target) {
    int left, right;
    int parent;

    if (index >= MAX || !used[index])
        return;

    if (tree[index] == target) {
        printf("노드: %c\n", tree[index]);

        left = index * 2;
        right = index * 2 + 1;

        printf("자식: ");

        if (left < MAX && used[left])
            printf("%c ", tree[left]);

        if (right < MAX && used[right])
            printf("%c", tree[right]);

        printf("\n");

        if (index == 1) {
            printf("부모: 없음\n");
            printf("형제: 없음\n");
        }
        else {
            parent = index / 2;

            printf("부모: %c\n", tree[parent]);

            if (index % 2 == 0) {
                if (used[parent * 2 + 1])
                    printf("형제: %c\n", tree[parent * 2 + 1]);
                else
                    printf("형제: 없음\n");
            }
            else {
                if (used[parent * 2])
                    printf("형제: %c\n", tree[parent * 2]);
                else
                    printf("형제: 없음\n");
            }
        }

        return;
    }

    findArray(index * 2, target);
    findArray(index * 2 + 1, target);
}

Node* findPointer(Node* root, char target) {
    Node* result;

    if (root == NULL)
        return NULL;

    if (root->data == target)
        return root;

    result = findPointer(root->left, target);

    if (result != NULL)
        return result;

    return findPointer(root->right, target);
}

Node* findParent(Node* root, char target) {
    Node* result;

    if (root == NULL)
        return NULL;

    if ((root->left != NULL && root->left->data == target) ||
        (root->right != NULL && root->right->data == target))
        return root;

    result = findParent(root->left, target);

    if (result != NULL)
        return result;

    return findParent(root->right, target);
}

void printPointerRelations(Node* root, char target) {
    Node* node;
    Node* parent;

    node = findPointer(root, target);

    if (node == NULL) {
        printf("해당 노드가 없습니다.\n");
        return;
    }

    printf("노드: %c\n", node->data);

    printf("자식: ");

    if (node->left != NULL)
        printf("%c ", node->left->data);

    if (node->right != NULL)
        printf("%c", node->right->data);

    if (node->left == NULL && node->right == NULL)
        printf("없음");

    printf("\n");

    parent = findParent(root, target);

    if (parent == NULL) {
        printf("부모: 없음\n");
        printf("형제: 없음\n");
    }
    else {
        printf("부모: %c\n", parent->data);

        if (parent->left != NULL &&
            parent->left->data == target) {

            if (parent->right != NULL)
                printf("형제: %c\n", parent->right->data);
            else
                printf("형제: 없음\n");
        }
        else {
            if (parent->left != NULL)
                printf("형제: %c\n", parent->left->data);
            else
                printf("형제: 없음\n");
        }
    }
}

void makeArray(Node* root, int index) {
    if (root == NULL || index >= MAX)
        return;

    tree[index] = root->data;
    used[index] = 1;

    makeArray(root->left, index * 2);
    makeArray(root->right, index * 2 + 1);
}

int arrayMemory() {
    int last = MAX - 1;

    while (last > 0 && !used[last])
        last--;

    return (last + 1) * sizeof(char) +
        (last + 1) * sizeof(int);
}

int pointerMemory(Node* root) {
    if (root == NULL)
        return 0;

    return sizeof(Node) +
        pointerMemory(root->left) +
        pointerMemory(root->right);
}

void freeTree(Node* root) {
    if (root == NULL)
        return;

    freeTree(root->left);
    freeTree(root->right);

    free(root);
}

int main() {
    char input[1000];
    char target;

    Node* root;

    memset(used, 0, sizeof(used));
    memset(tree, 0, sizeof(tree));

    printf("괄호 표기법으로 이진트리를 입력하세요.\n");
    printf("예: A(B(D,E),C(,F))\n");
    printf("입력: ");

    fgets(input, sizeof(input), stdin);

    pos = 0;
    root = parseTree(input);

    makeArray(root, 1);

    printf("\n============================\n");
    printf("1. 배열을 이용한 이진트리\n");
    printf("============================\n");

    printf("\n[1] 이진트리 출력\n");
    printArray(1, 0);

    printf("\n[2] 트리 정보\n");

    {
        int total = nodeCountArray(1);
        int leaf = leafCountArray(1);
        int height = heightArray(1);
        int degree = degreeArray(1);

        printf("전체 노드의 수: %d\n", total);
        printf("단말 노드의 수: %d\n", leaf);
        printf("비단말 노드의 수: %d\n", total - leaf);
        printf("트리의 높이: %d\n", height);
        printf("트리의 차수: %d\n", degree);
    }

    printf("\n[3] 이진트리 형태 판별\n");

    printf("완전 이진트리: ");
    if (isCompleteArray())
        printf("YES\n");
    else
        printf("NO\n");

    printf("포화 이진트리: ");
    if (isFullArray(1) &&
        nodeCountArray(1) == (1 << heightArray(1)) - 1)
        printf("YES\n");
    else
        printf("NO\n");

    printf("편향 이진트리: ");
    if (isSkewedArray(1))
        printf("YES\n");
    else
        printf("NO\n");

    printf("\n============================\n");
    printf("2. 포인터를 이용한 이진트리\n");
    printf("============================\n");

    printf("\n[1] 이진트리 출력\n");
    printPointer(root, 0);

    printf("\n[2] 트리 정보\n");

    {
        int total = nodeCountPointer(root);
        int leaf = leafCountPointer(root);
        int height = heightPointer(root);
        int degree = degreePointer(root);

        printf("전체 노드의 수: %d\n", total);
        printf("단말 노드의 수: %d\n", leaf);
        printf("비단말 노드의 수: %d\n", total - leaf);
        printf("트리의 높이: %d\n", height);
        printf("트리의 차수: %d\n", degree);
    }

    printf("\n[3] 이진트리 형태 판별\n");

    printf("완전 이진트리: ");
    if (checkCompletePointer(root))
        printf("YES\n");
    else
        printf("NO\n");

    printf("포화 이진트리: ");
    if (checkFullPointer(root) &&
        nodeCountPointer(root) == (1 << heightPointer(root)) - 1)
        printf("YES\n");
    else
        printf("NO\n");

    printf("편향 이진트리: ");
    if (checkSkewedPointer(root))
        printf("YES\n");
    else
        printf("NO\n");

    printf("\n============================\n");
    printf("3. 메모리 사용량 비교\n");
    printf("============================\n");

    printf("배열 구현 메모리: %d bytes\n", arrayMemory());
    printf("포인터 구현 메모리: %d bytes\n", pointerMemory(root));

    printf("\n============================\n");
    printf("4. 자식 / 부모 / 형제 출력\n");
    printf("============================\n");

    printf("찾을 노드를 입력하세요: ");
    scanf(" %c", &target);

    printf("\n[배열 구현]\n");
    findArray(1, target);

    printf("\n[포인터 구현]\n");
    printPointerRelations(root, target);

    freeTree(root);

    return 0;
}
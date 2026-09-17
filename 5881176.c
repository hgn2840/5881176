#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LEN 100005
#define MAX_DEPTH 30

char input[MAX_LEN];
int len;

int totalNodes = 0;
int leafNodes = 0;
int maxDegree = 0;
int maxDepth = 0;      

int  parentOfCFound = 0;
char parentOfC = 0;     

char childrenOfC[26];
int  childrenOfCCount = 0;

char ancestorStack[MAX_DEPTH];  
int  ancestorTop = 0;

int errorFlag = 0;

void fail(void) {
    printf("입력한 문자열은 올바른 트리의 괄호 표기법이 아닙니다.\n");
    exit(0);
}


int parseNode(int pos, int depth) {
    if (errorFlag) return pos;
    if (pos >= len || !isupper((unsigned char)input[pos])) { errorFlag = 1; return pos; }

    char node = input[pos++];
    totalNodes++;
    if (depth > maxDepth) maxDepth = depth;

    char parent = (ancestorTop > 0) ? ancestorStack[ancestorTop - 1] : 0;

    if (node == 'C') {
        parentOfC = parent;
        parentOfCFound = 1;
    }
    if (parent == 'C') {
        childrenOfC[childrenOfCCount++] = node;
    }

    int childCount = 0;

    if (pos < len && input[pos] == '(') {
        pos++;
        if (ancestorTop >= MAX_DEPTH) { errorFlag = 1; return pos; }
        ancestorStack[ancestorTop++] = node;  

        while (1) {
            pos = parseNode(pos, depth + 1);
            if (errorFlag) return pos;
            childCount++;
            if (pos < len && input[pos] == ',') { pos++; continue; }
            break;
        }

        if (pos >= len || input[pos] != ')') { errorFlag = 1; return pos; }
        pos++;
        ancestorTop--;                          
    }
    else {
        leafNodes++;
    }

    if (childCount > maxDegree) maxDegree = childCount;
    return pos;
}

int printNode(int pos, int level) {
    char node = input[pos++];

    if (level == 0) {
        printf("%c\n", node);
    }
    else {
        for (int i = 0; i < (level - 1) * 4; i++) putchar(' ');
        printf("+---%c\n", node);
    }

    if (pos < len && input[pos] == '(') {
        pos++;
        while (1) {
            pos = printNode(pos, level + 1);
            if (input[pos] == ',') { pos++; continue; }
            break;
        }
        pos++; 
    }
    return pos;
}

int main(void) {
    printf("트리의 괄호 표기법을 입력하세요: ");
    if (!fgets(input, sizeof(input), stdin)) return 0;

    int j = 0;
    for (int i = 0; input[i]; i++)
        if (!isspace((unsigned char)input[i])) input[j++] = input[i];
    input[j] = '\0';
    len = j;

    if (len == 0) fail();

    int pos = parseNode(0, 1);
    if (errorFlag || pos != len) fail();

    int nonLeaf = totalNodes - leafNodes;

    printf("\n=== 트리 정보 ===\n");
    printf("전체 노드의 수   : %d\n", totalNodes);
    printf("단말 노드의 수   : %d\n", leafNodes);
    printf("비단말 노드의 수 : %d\n", nonLeaf);
    printf("트리의 높이     : %d\n", maxDepth);
    printf("트리의 차수     : %d\n", maxDegree);

    if (parentOfCFound) {
        if (parentOfC == 0) printf("C의 부모 노드   : 없음 (루트 노드)\n");
        else                printf("C의 부모 노드   : %c\n", parentOfC);

        if (childrenOfCCount == 0) {
            printf("C의 자식 노드   : 없음 (단말 노드)\n");
        }
        else {
            printf("C의 자식 노드   : ");
            for (int i = 0; i < childrenOfCCount; i++)
                printf("%c%s", childrenOfC[i], (i < childrenOfCCount - 1) ? ", " : "\n");
        }
    }
    else {
        printf("트리에 노드 C가 존재하지 않습니다.\n");
    }

    printf("\n=== 트리 구조 ===\n");
    printNode(0, 0);

    return 0;
}
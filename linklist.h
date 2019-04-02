#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node *next;
} node;

node *createNode();
node *addNode(node *head, int ele);
node *delNode(node *head, int ele);

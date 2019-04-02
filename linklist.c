#include "linklist.h"



/* 链表初始化 */
node *createNode()
{
    node *n;
    n = (node *)malloc(sizeof(node));
    if (n == NULL) {
        printf("init link list error\n");    
        exit(-1);
    }
    n->next = NULL;
    return n;
}

/* 向链表添加元素 */
node *addNode(node *head, int ele)
{
    node *n, *p;
    n = createNode();
    n->data = ele;
    if (head == NULL) {
        head = n;    
    } else {
        p = head;
        while (p->next != NULL) {
            p = p->next;    
        }
        p->next = n;
    }
    return head;
}

/* 链表中删除元素 */
node *delNode(node *head, int ele)
{
    node *current, *prev, *temp;
    if (head == NULL) {
        printf("no element in linked list.\n");
        return;
    } else {
        if (head != NULL && head->data == ele) {
            temp = head;
            head = head->next;
            free(temp);
            temp = NULL;
            return head;
        }
        prev = head;
        current = head->next;
        while (current != NULL) {
            if (current->data == ele) {
                prev->next = current->next;
                free(current);
                current = NULL;
                return  head;
            }    
            current = current->next;
        }
        return head;
    }   
}


#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct _LinkedList {
	char word[14];
	struct _LinkedList *nextNode;
};

typedef struct _LinkedList ListNode;
typedef ListNode* ListNodePtr;

void insertNode_nor(ListNodePtr *StartNode, char word[]);
void insertNode_arr(ListNodePtr *StartNode, char word[]);
void deleteNode(ListNodePtr *StartNode, char word[]);
void PrintList(ListNodePtr StartNode);

int main(void)
{
	char word[14];
	
	ListNodePtr StartNode;
	StartNode = NULL;

	ListNodePtr StartNode2;
	StartNode2 = NULL;

	ListNodePtr StartNode3;
	StartNode3 = NULL;

	FILE *fp, *fp2, *fp3;
	fp = fopen("input1.txt", "r");
	fp2 = fopen("input2.txt", "r");
	fp3 = fopen("output.txt", "w");

	/*1�� LinkedList ����*/
	for (int i = 0; i < 10; i++) {
		fscanf(fp, "%s", word);
		insertNode_nor(&StartNode, word);
	}
	printf("Input linked list 1: ");
	PrintList(StartNode);
	//fseek(fp, 0, SEEK_SET);

	/*2�� LinkedList ����*/
	for (int i = 0; i < 10; i++) {
		fscanf(fp2, "%s", word);
		insertNode_nor(&StartNode2, word);
	}
	printf("Input linked list 2: ");
	PrintList(StartNode2);
	//fseek(fp, 0, SEEK_SET);
	
	//3�� LinkedList ����
	for (int i = 0; i < 10; i++) {
		deleteNode(&StartNode, word);
		insertNode_arr(&StartNode3, word);
	}
	for (int i = 0; i < 10; i++) {
		deleteNode(&StartNode2, word);
		insertNode_arr(&StartNode3, word);
	}
	printf("Result linked list: ");
	PrintList(StartNode3);
}

void insertNode_nor(ListNodePtr *StartNodePtr, char word[])
{
	ListNodePtr NewNode;
	ListNodePtr previousNode;
	ListNodePtr currentNode;

	NewNode = malloc(sizeof(ListNode));
	strcpy(NewNode->word, word);
	NewNode->nextNode = NULL;

	previousNode = NULL;
	currentNode = *StartNodePtr;

	if (NewNode != NULL) {
		if (currentNode == NULL) {									//����Ʈ�� ����ִ� ���
			*StartNodePtr = NewNode;
		}
		else {														//������� �ʴٸ�
			while (currentNode != NULL) {
				previousNode = currentNode;
				currentNode = currentNode->nextNode;
			}
			previousNode->nextNode = NewNode;
			NewNode->nextNode = currentNode;
		}
	}

	else {
		printf("%s is not inserted. Memory is not available.\n\n", word);
	}
}

void insertNode_arr(ListNodePtr *StartNodePtr, char word[])
{
	ListNodePtr NewNode;
	ListNodePtr previousNode;
	ListNodePtr currentNode;

	NewNode = malloc(sizeof(ListNode));
	strcpy(NewNode->word, word);
	NewNode->nextNode = NULL;

	previousNode = NULL;
	currentNode = *StartNodePtr;

	

	if (NewNode != NULL) {
		if (currentNode == NULL) {									//����Ʈ�� ����ִ� ���
			*StartNodePtr = NewNode;
		}
		else {														//������� �ʴٸ�
			if (strcmp(word, currentNode->word) == -1) {			//�� �տ� �ֱ�
				*StartNodePtr = NewNode;
				NewNode->nextNode = currentNode;
			}
			else {
				while (currentNode != NULL && (strcmp(word, currentNode->word) > -1)) { //�� or �߰��� �ޱ�
					if (!strcmp(word, currentNode->word)) {
						return;
					}
					previousNode = currentNode;
					currentNode = currentNode->nextNode;
				}
				previousNode->nextNode = NewNode;
				NewNode->nextNode = currentNode;
			}
		}
	}

	else {
		printf("%s is not inserted. Memory is not available.\n\n", word);
	}
}

void PrintList(ListNodePtr StartNode)
{
	int i = 1;

	ListNodePtr currentNode;
	currentNode = StartNode;
	
	if (StartNode != NULL) {											//������� �ʴٸ�
		while (currentNode != NULL) {
			if (i) {
				printf("%s ", currentNode->word);
				i--;
			}
			else {
				printf("-> %s ", currentNode->word);
			}
			currentNode = currentNode->nextNode;
		}
		printf("\n\n");
	}
	else {
		printf("List is empty.\n\n");
	}
}

void deleteNode(ListNodePtr *StartNodePtr, char word[])
{
	ListNodePtr tempNode;

	if (*StartNodePtr != NULL) {
		tempNode = *StartNodePtr;
		*StartNodePtr = tempNode->nextNode;
		strcpy(word, tempNode->word);
		free(tempNode);
	}
}

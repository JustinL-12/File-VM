#include <stdlib.h>

// Node struct
typedef struct node {
	int data;
	struct node* next;
} node;

// Create node in stack
node* newNode(int value) {
	node* temp = (node*) malloc(sizeof(node));
	temp->data = value;
	temp->next = NULL;
	return temp;
}

// Push node 
void push(node** root, int value) {
	node* temp = newNode(value);
	temp->next = *root;
	*root = temp;
}

// Pop node
int pop(node** root) {
	node* temp = *root;
	*root = (*root)->next;
	int ret = temp->data;
	//free(temp);
	return ret; 
}

// Peek top value
int peek(node* root) {
	return root->data;
}

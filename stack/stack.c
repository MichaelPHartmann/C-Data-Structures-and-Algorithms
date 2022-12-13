#include <stdio.h>
#include <stdlib.h>

// Define a stack struct
typedef struct stack
{
	int maxSize;
	int top;
	int *items;
} stack;

// initialize the stack
stack newStack(int capacity) {
	stack *pStack = malloc(sizeof(stack));
	pStack->maxSize = capacity;
	pStack->top = -1;
	pStack->items = (int*)malloc(sizeof(int) * capacity);
	
	return *pStack;
}

// Find the size of a stack
int size(stack *pt) {
	return pt->top + 1;
}

// Determine if a stack is empty
int isEmpty(stack *pt) {
	return pt->top == -1;
}

// Determine if a stack is empty
int isFull(stack *pt) {
	return pt->top == pt->maxSize -1;
}

// Put element on top of the stack
void push(stack *pt, int x) {
	if (isFull(pt)) {
		printf("Stack Overflow\n");
		exit(EXIT_FAILURE);
	} else {
		printf("Inserting %d\n", x);
		pt->items[++pt->top] = x;
	}
}

// Look at the top element of the stack
int peek(stack *pt) {
	if (isEmpty(pt)) {
		printf("Stack Empty\n");
		exit(EXIT_FAILURE);
	} else {
		printf("Peeking %d\n", pt->items[pt->top]);
		return pt->items[pt->top];
	}
}

int pop(stack *pt) {
	if (isEmpty(pt)) {
		printf("Stack Underflow\n");
		exit(EXIT_FAILURE);
	} else {
		printf("Removing %d\n", peek(pt));
		return pt->items[pt->top--];
	}
}

int main() {
	stack myStack = newStack(5);
	push(&myStack, 13);
	push(&myStack, 897);
	peek(&myStack);
	pop(&myStack);
}

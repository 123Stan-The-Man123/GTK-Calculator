#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXLINE 100
#define STACK_SIZE 100
#define QUEUE_SIZE 100

typedef struct Stack {
  char *stack[STACK_SIZE];
  int top;
} Stack;

typedef struct Queue {
  char *queue[QUEUE_SIZE];
  int front;
  int back;
} Queue;

Stack *initialize_stack(void);
void push(Stack *stack, char *string);
char *pop(Stack *stack);
char *peek(Stack *stack);
void free_stack(Stack *stack);
char *dequeue(Queue *queue);

char *rpn_evaluator(Queue *queue, char *result);

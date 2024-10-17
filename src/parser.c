#include <stdlib.h>
#include <string.h>
#include "parser.h"

#define MAXLINE 100
#define STACK_SIZE 100
#define QUEUE_SIZE 100

typedef struct stack {
    char *stack[STACK_SIZE];
    int top;
} stack;

typedef struct queue {
    char *queue[QUEUE_SIZE];
    int front;
    int back;
} queue;

static stack *initialize_stack(void);
static void push(stack *stack, char *string);
static char *pop(stack *stack);
static char *peek(stack *stack);
static void free_stack(stack *stack);
static queue *initialize_queue(void);
static void enqueue(queue *queue, char *string);
static char *dequeue(queue *queue);
static void free_queue(queue *queue);

stack *initialize_stack(void) {
    stack *stack = malloc(sizeof(stack));

    for (int i = 0; i < STACK_SIZE; ++i) {
        stack->stack[i] = malloc(sizeof(char) * MAXLINE);
        memset(stack->stack[i], '\0', MAXLINE);
    }

    stack->top = -1;

    return stack;
}

void push(stack *stack, char *string) {
    if (stack->top == STACK_SIZE - 1)
        return ;
    
    stack->top++;
    strcpy(stack->stack[stack->top], string);
}

char *pop(stack *stack) {
    if (stack->top < 0)
        return NULL;
    
    return stack->stack[stack->top--];
}

char *peek(stack *stack) {
    return stack->stack[stack->top];
}

void free_stack(stack *stack) {
    for (int i = 0; i < STACK_SIZE; ++i)
        free(stack->stack[i]);
    
    free(stack);
}

queue *initialize_queue(void) {
    queue *queue = malloc(sizeof(queue));

    for (int i = 0; i < QUEUE_SIZE; ++i) {
        queue->queue[i] = malloc(sizeof(char) * MAXLINE);
        memset(queue->queue[i], '\0', MAXLINE);
    }

    queue->front = 0;
    queue->back = 0;
    return queue;
}

void enqueue(queue *queue, char *string) {
    if (queue->back == QUEUE_SIZE - 1)
        return ;
    
    strcpy(queue->queue[queue->back++], string);
}

char *dequeue(queue *queue) {
    if (queue->front == queue->back)
        return NULL;
    
    return queue->queue[queue->front++];
}

void free_queue(queue *queue) {
    for (int i = 0; i < QUEUE_SIZE; ++i)
        free(queue->queue[i]);
    
    free(queue);
}

char *operator_precedence_parser(char *string) {
    return string;
}
#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "parser.h"

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

static Stack *initialize_stack(void);
static void push(Stack *stack, char *string);
static char *pop(Stack *stack);
static char *peek(Stack *stack);
static void free_stack(Stack *stack);
static Queue *initialize_queue(void);
static void enqueue(Queue *queue, char *string);
static char *dequeue(Queue *queue);
static void free_queue(Queue *queue);

Stack *initialize_stack(void) {
  Stack *stack = malloc(sizeof(Stack));

  for (int i = 0; i < STACK_SIZE; ++i) {
    stack->stack[i] = malloc(sizeof(char) * MAXLINE);
    memset(stack->stack[i], '\0', MAXLINE);
  }

  stack->top = -1;

  return stack;
}

void push(Stack *stack, char *string) {
  if (stack->top == STACK_SIZE - 1)
    return ;
    
  stack->top++;
  strcpy(stack->stack[stack->top], string);
}

char *pop(Stack *stack) {
  if (stack->top < 0)
    return NULL;
    
  return stack->stack[stack->top--];
}

char *peek(Stack *stack) {
  if (stack->top < 0)
      return NULL;

  return stack->stack[stack->top];
}

void free_stack(Stack *stack) {
  for (int i = 0; i < STACK_SIZE; ++i)
    free(stack->stack[i]);
    
  free(stack);
}

Queue *initialize_queue(void) {
  Queue *queue = malloc(sizeof(Queue));

  for (int i = 0; i < QUEUE_SIZE; ++i) {
    queue->queue[i] = malloc(sizeof(char) * MAXLINE);
    memset(queue->queue[i], '\0', MAXLINE);
  }

  queue->front = 0;
  queue->back = 0;
  return queue;
}

void enqueue(Queue *queue, char *string) {
  if (queue->back == QUEUE_SIZE)
    return ;
    
  strcpy(queue->queue[queue->back++], string);
}

char *dequeue(Queue *queue) {
  if (queue->front == queue->back)
    return NULL;
    
  return queue->queue[queue->front++];
}

void free_queue(Queue *queue) {
  for (int i = 0; i < QUEUE_SIZE; ++i)
      free(queue->queue[i]);

  free(queue);
}

char *operator_precedence_parser(char *string) {
  Stack *operator_stack = initialize_stack();
  Queue *output_queue = initialize_queue(); 
  int index;
  char item_string[MAXLINE];
  bool is_operator = false;

  while (*string != '\0') {
    index = 0;
    memset(item_string, '\0', MAXLINE);

    if ((*string == '+' || *string == '-') && !is_operator && isdigit(*(string+1))) {
      item_string[index++] = *string++;
      is_operator = true;
    }

    if (isdigit(*string)) {
      while (isdigit(*string))
        item_string[index++] = *string++;

      if (*string == '.') {
        item_string[index++] = *string++;

        if (!isdigit(*string)) {
          free_stack(operator_stack);
          free_queue(output_queue);
          return "syntax error";
        }

        else {
          while (isdigit(*string))
            item_string[index++] = *string++;

          if (*string == '.') {
            free_stack(operator_stack);
            free_queue(output_queue);
            return "syntax error";
          }
        }
      }

      item_string[index] = '\0';
      enqueue(output_queue, item_string);
      is_operator = true;
    }

    else if (isalpha(*string)){
      while (isalpha(*string))
        item_string[index++] = *string++;
  
      item_string[index] = '\0';
      push(operator_stack, item_string);

      is_operator = false;
    }

    else {
      item_string[index++] = *string++;

      item_string[index] = '\0';
      push(operator_stack, item_string);

      is_operator = false;
    }
  } 

  for (int i = 0; i < output_queue->back; ++i)
      printf("%s\n", output_queue->queue[i]);

  for (int i = 0; i < operator_stack->top + 1; ++i)
      printf("%s\n", operator_stack->stack[i]);

  free_stack(operator_stack);
  free_queue(output_queue);
  return string;
}

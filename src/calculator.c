#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "calculator.h"

#define QUEUE_SIZE 100
#define STACK_SIZE 100

typedef struct output_queue {
  char *queue[QUEUE_SIZE];
  int front;
  int back;
} output_queue;

typedef struct operator_stack {
  char *stack[STACK_SIZE];
  int top;
} operator_stack;

static output_queue *initialize_queue(void);
static void enqueue(output_queue *queue, char *string);
static char *dequeue(output_queue *queue);
static void free_queue(output_queue *queue);
static operator_stack *initialize_stack(void);
static void push(operator_stack *stack, char *operator);
static char *pop(operator_stack *stack);
static void free_stack(operator_stack *stack);

output_queue *initialize_queue(void) {
  output_queue *operand_queue = malloc(sizeof(output_queue));

  for (int i = 0; i < QUEUE_SIZE; ++i) {
      operand_queue->queue[i] = malloc(sizeof(char) * 100);
      operand_queue->queue[i][0] = '\0';
    }
  
  operand_queue->front = 0;
  operand_queue->back = 0;
  return operand_queue;
}

void enqueue(output_queue *queue, char *string) {
  strcpy(queue->queue[queue->back++], string);
}

char *dequeue(output_queue *queue) {
  return queue->queue[queue->front++];
}

void free_queue(output_queue *queue) {
  for (int i = 0; i < QUEUE_SIZE; ++i)
    free(queue->queue[i]);

  free(queue);
}

operator_stack *initialize_stack(void) {
  operator_stack *stack = malloc(sizeof(operator_stack));
  
  for (int i = 0; i < STACK_SIZE; ++i) {
    stack->stack[i] = malloc(sizeof(char) * 4);
    stack->stack[i][0] = '\0';
  }

  stack->top = 0;
  return stack;
}

void push(operator_stack *stack, char *operator) {
  strcpy(stack->stack[stack->top++], operator);
}

char *pop(operator_stack *stack) {
  stack->top--; 

  if (stack->top < 0)
      return "empty stack";

  return stack->stack[stack->top];
}

void free_stack(operator_stack *stack) {
  for (int i = 0; i < STACK_SIZE; ++i)
      free(stack->stack[i]);

  free(stack);
}

char *operator_precedence_parser(char *string) {
  char num[100] = {'\0'};
  char operator[4] = {'\0'};
  output_queue *operand_queue = initialize_queue();
  operator_stack *stack = initialize_stack();
  bool is_operator = false;

  while (*string != '\0') {
    if (isdigit(*string) || (*string == '-' && is_operator == false) && isdigit(*(string + 1))) {
      num[0] = *string++;

      for (int i = 1; isdigit(*string) || *string == '.'; ++i, ++string) {
        num[i] = *string;
      }

      enqueue(operand_queue, num);
      memset(num, '\0', sizeof(num));
      is_operator = true;
    }
    
    else {
      if (isalpha(*string) && *string != 'x') 
        for (int i = 0; isalpha(*string); ++i, ++string) {
          operator[i] = *string;
          operator[i+1] = '\0';
        }
      
      else {
        operator[0] = *string++;
        operator[1] = '\0';
      }

      push(stack, operator);
      is_operator = false;
    }
  }

  for (int i = 0; operand_queue->queue[i][0] != '\0'; ++i) {
    printf("%s\n", operand_queue->queue[i]);
  }
  
  printf("%c\n", 251);

  free_queue(operand_queue);
  free_stack(stack);
  return string;
}

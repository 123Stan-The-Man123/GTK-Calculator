#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "calculator.h"

#define QUEUE_SIZE 100
#define STACK_SIZE 100
#define OPERATOR_SIZE 5
#define OPERATOR_AMOUNT 7

typedef struct operator_info {
  char operator[OPERATOR_SIZE];
  int precedence;
  char associativity; //l = left, r = right
} operator_info;

operator_info operator_array[OPERATOR_AMOUNT];

typedef struct output_queue {
  char *queue[QUEUE_SIZE];
  int front;
  int back;
} output_queue;

typedef struct operator_stack {
  char *stack[STACK_SIZE];
  int top;
} operator_stack;

static void initialize_operators(int status);
static operator_info find_operator(char *operator);
static output_queue *initialize_queue(void);
static void enqueue(output_queue *queue, char *string);
static char *dequeue(output_queue *queue);
static void free_queue(output_queue *queue);
static operator_stack *initialize_stack(void);
static void push(operator_stack *stack, char *operator);
static char *pop(operator_stack *stack);
static void free_stack(operator_stack *stack);

void initialize_operators(int status) {
  if (status == 1)
      return ;

  //mod
  strcpy(operator_array[0].operator, "mod");
  operator_array[0].precedence = 3;
  operator_array[0].associativity = 'l';

  //divide
  strcpy(operator_array[1].operator, "/");
  operator_array[1].precedence = 3;
  operator_array[1].associativity = 'l';

  //square root
  strcpy(operator_array[2].operator, "sqrt");
  operator_array[2].precedence = 4;
  operator_array[2].associativity = 'r';

  //multiply
  strcpy(operator_array[3].operator, "*");
  operator_array[3].precedence = 3;
  operator_array[3].associativity = 'l';

  //pow
  strcpy(operator_array[4].operator, "^");
  operator_array[4].precedence = 4;
  operator_array[4].associativity = 'r';

  //minus
  strcpy(operator_array[5].operator, "-");
  operator_array[5].precedence = 2;
  operator_array[5].associativity = 'l';

  //add
  strcpy(operator_array[6].operator, "+");
  operator_array[6].precedence = 2;
  operator_array[6].associativity = 'l';
}

operator_info find_operator(char *operator) {
  for (int i = 0; i < OPERATOR_AMOUNT; ++i)
    if (strcmp(operator_array[i].operator, operator) == 0)
      return operator_array[i];
}

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
    stack->stack[i] = malloc(sizeof(char) * OPERATOR_SIZE);
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
  static int status  = 0;

  initialize_operators(status);
  status = 1;

  char num[100] = {'\0'};
  char operator[OPERATOR_SIZE] = {'\0'};
  output_queue *operand_queue = initialize_queue();
  operator_stack *stack = initialize_stack();
  bool is_operator = false;

  while (*string != '\0') {
    if (isdigit(*string) || (*string == '-' && is_operator == false) && isdigit(*(string + 1))) {
      num[0] = *string++;

      for (int i = 1; isdigit(*string) || *string == '.' || *string == '%'; ++i, ++string) {
        num[i] = *string;
      }

      enqueue(operand_queue, num);
      memset(num, '\0', sizeof(num));
      is_operator = true;
    }

    else if (*string == 'p' && *(string+1) == 'i') {
      strcpy(num, "pi");
      enqueue(operand_queue, num);
      memset(num, '\0', sizeof(num));
      is_operator = true;
      string += 2;
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

      if (strcmp(operator, "(") == 0)
        push(stack, operator);
      
      if (strcmp(operator, ")") == 0) {
        char *temp_operator = pop(stack);

        while (strcmp(temp_operator, "(") != 0 && strcmp(temp_operator, "empty stack") != 0) {
          enqueue(operand_queue, temp_operator);
          temp_operator = pop(stack);
        }

        if (strcmp(temp_operator, "empty stack") == 0)
          return "Syntax error: missing bracket";
      }

      operator_info current_operator = find_operator(operator);
      operator_info top_operator = find_operator(stack->stack[stack->top]);

      while (stack->top >= 0 && strcmp(top_operator.operator, "(") != 0 && top_operator.precedence > current_operator.precedence || top_operator.precedence == current_operator.precedence && current_operator.associativity == 'l') {
        enqueue(operand_queue, pop(stack));
        top_operator = find_operator(stack->stack[stack->top]);
      }

      push(stack, current_operator.operator);

      push(stack, operator);
      is_operator = false;
    }
  }

  while (stack->top >= 0)
    enqueue(operand_queue, pop(stack));

  for (int i = 0; operand_queue->queue[i][0] != '\0'; ++i) {
    printf("%s\n", operand_queue->queue[i]);
  }

  char *output_string;
  while (strcmp((output_string = pop(stack)), "empty stack") != 0)
    printf("%s\n", output_string);

  free_queue(operand_queue);
  free_stack(stack);
  return string;
}

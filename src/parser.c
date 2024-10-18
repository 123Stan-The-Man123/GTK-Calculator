#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "parser.h"

#define MAXLINE 100
#define MAX_OPERATOR 4
#define OPERATOR_COUNT 8
#define STACK_SIZE 100
#define QUEUE_SIZE 100

typedef struct Operator_Info {
  char operator[MAX_OPERATOR];
  int precedence;
  char associativity;
} Operator_Info;

typedef struct Stack {
  char *stack[STACK_SIZE];
  int top;
} Stack;

typedef struct Queue {
  char *queue[QUEUE_SIZE];
  int front;
  int back;
} Queue;

Operator_Info operators[OPERATOR_COUNT];
static void initialize_operators(void);
static Operator_Info find_operator(char *operator);
static Stack *initialize_stack(void);
static void push(Stack *stack, char *string);
static char *pop(Stack *stack);
static char *peek(Stack *stack);
static void free_stack(Stack *stack);
static Queue *initialize_queue(void);
static void enqueue(Queue *queue, char *string);
static char *dequeue(Queue *queue);
static void free_queue(Queue *queue);

void initialize_operators(void) {
  // add
  strcpy(operators[0].operator, "+");
  operators[0].precedence = 2;
  operators[0].associativity = 'l';

  // subtract
  strcpy(operators[1].operator, "-");
  operators[1].precedence = 2;
  operators[1].associativity = 'l';
  
  // multiply
  strcpy(operators[2].operator, "*");
  operators[2].precedence = 3;
  operators[2].associativity = 'l';

  // divide
  strcpy(operators[3].operator, "/");
  operators[3].precedence = 3;
  operators[3].associativity = 'l';

  // power
  strcpy(operators[4].operator, "^");
  operators[4].precedence = 4;
  operators[4].associativity = 'r';

  // square root
  strcpy(operators[5].operator, "sqrt");
  operators[5].precedence = 4;
  operators[5].associativity = 'r';

  // modulus
  strcpy(operators[6].operator, "mod");
  operators[6].precedence = 3;
  operators[6].associativity = 'l';

  // percent
  strcpy(operators[7].operator, "%");
  operators[7].precedence = 3;
  operators[7].associativity = 'l';
}

Operator_Info find_operator(char *operator) {
  for (int i = 0; i < OPERATOR_COUNT; ++i)
    if (strcmp(operators[i].operator, operator) == 0)
      return operators[i];
}

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
  initialize_operators();
  Stack *operator_stack = initialize_stack();
  Queue *output_queue = initialize_queue(); 
  Operator_Info current_operator;
  Operator_Info top_operator;
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
      while (isalpha(*string) && (strcmp(item_string, "mod") != 0 && strcmp(item_string, "pi") != 0 && strcmp(item_string, "sqrt") != 0))
        item_string[index++] = *string++;
  
      item_string[index] = '\0';

      if (strcmp(item_string, "pi") == 0) {
        enqueue(output_queue, item_string);
        continue;
      }
      
      if (peek(operator_stack) != NULL) {
        current_operator = find_operator(item_string);
        top_operator = find_operator(peek(operator_stack));

        while (peek(operator_stack) != NULL && strcmp(top_operator.operator, "(") != 0 && (top_operator.precedence > current_operator.precedence || (top_operator.precedence == current_operator.precedence && current_operator.associativity == 'l'))) {
          puts("test");
          enqueue(output_queue, pop(operator_stack));

          if (peek(operator_stack) != NULL)
              top_operator = find_operator(peek(operator_stack));
        }
      }

      push(operator_stack, item_string);

      is_operator = false;
    }

    else {
      item_string[index++] = *string++;

      item_string[index] = '\0';

      if (strcmp(item_string, "(") == 0) {
        push(operator_stack, item_string);
      }

      else if (strcmp(item_string, ")") == 0) {
        char *popped_operator = pop(operator_stack);

        while (popped_operator != NULL && strcmp(popped_operator, "(") != 0) {
          enqueue(output_queue, popped_operator);
          popped_operator = pop(operator_stack);
        }

        if (popped_operator == NULL) {
          free_stack(operator_stack);
          free_queue(output_queue);
          return "syntax error";
        }
      }

      else {
        if (peek(operator_stack) != NULL) {
          current_operator = find_operator(item_string);
          top_operator = find_operator(peek(operator_stack));

          while (peek(operator_stack) != NULL && strcmp(top_operator.operator, "(") != 0 && (top_operator.precedence > current_operator.precedence || (top_operator.precedence == current_operator.precedence && current_operator.associativity == 'l'))) {
            puts("test");
            enqueue(output_queue, pop(operator_stack));

            if (peek(operator_stack) != NULL)
                top_operator = find_operator(peek(operator_stack));
          }
        }

        push(operator_stack, item_string);
      }

      is_operator = false;
    }
  } 

  while (peek(operator_stack) != NULL)
      enqueue(output_queue, pop(operator_stack));

  free_stack(operator_stack);
  free_queue(output_queue);
  return string;
}

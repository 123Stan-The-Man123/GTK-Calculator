#include <math.h>
#include "calculator.h"

#define PI "3.14159265358979323846"

static void handle_operation(Stack *stack, char *item);
static void trim_output(char *string);

char *rpn_evaluator(Queue *queue, char *result) {
  Stack *stack = initialize_stack();
  char *item;
  
  while ((item = dequeue(queue)) != NULL) {
    if (isdigit(*item) || isdigit(*(item+1)))
      push(stack, item);

    else {
      handle_operation(stack, item);
    }
  }
  
  strcpy(result, pop(stack));
  free_stack(stack);
  trim_output(result);
  return result;
}

void handle_operation(Stack *stack, char *operator) {
  if (strcmp(operator, "pi") == 0) {
    push(stack, PI);
    return ;
  }

  char *operand1 = pop(stack);
  
  // addition
  if (strcmp(operator, "+") == 0) {
    char *operand2 = pop(stack);

    if (operand1 != NULL && operand2 != NULL) {
      double converted_operand1 = strtod(operand1, NULL);
      double converted_operand2 = strtod(operand2, NULL);

      char buffer[MAXLINE] = {'\0'};
      sprintf(buffer, "%lf", converted_operand1 + converted_operand2);
      push(stack, buffer);
    }

    else 
      push(stack, "0");

    return ;
  }
  
  // subtraction
  if (strcmp(operator, "-") == 0) {
    char *operand2 = pop(stack);
    
    if (operand1 != NULL && operand2 != NULL) {
      double converted_operand1 = strtod(operand1, NULL);
      double converted_operand2 = strtod(operand2, NULL);

      char buffer[MAXLINE] = {'\0'};
      sprintf(buffer, "%lf", converted_operand2 - converted_operand1);
      push(stack, buffer);
    }

    else 
      push(stack, "0");

    return ;
  }

  // multiplication
  if (strcmp(operator, "*") == 0) {
    char *operand2 = pop(stack);

    if (operand1 != NULL && operand2 != NULL) {
      double converted_operand1 = strtod(operand1, NULL);
      double converted_operand2 = strtod(operand2, NULL);

      char buffer[MAXLINE] = {'\0'};
      sprintf(buffer, "%lf", converted_operand1 * converted_operand2);
      push(stack, buffer);
    }

    else 
      push(stack, "0");

    return ;
  }

  // division
  if (strcmp(operator, "/") == 0) {
    char *operand2 = pop(stack);

    if (operand1 != NULL && operand2 != NULL) {
      double converted_operand1 = strtod(operand1, NULL);
      double converted_operand2 = strtod(operand2, NULL);

      char buffer[MAXLINE] = {'\0'};
      sprintf(buffer, "%lf", converted_operand2 / converted_operand1);
      push(stack, buffer);
    }

    else 
      push(stack, "0");

    return ;
  }

  // power
  if (strcmp(operator, "^") == 0) {
    char *operand2 = pop(stack);

    if (operand1 != NULL && operand2 != NULL) {
      double converted_operand1 = strtod(operand1, NULL);
      double converted_operand2 = strtod(operand2, NULL);

      char buffer[MAXLINE] = {'\0'};
      sprintf(buffer, "%lf", pow(converted_operand2, converted_operand1));
      push(stack, buffer);
    }

    else 
      push(stack, "0");

    return ;
  }

  // sqaure root
  if (strcmp(operator, "sqrt") == 0) {
    if (operand1 != NULL) {
      double converted_operand1 = strtod(operand1, NULL);

      char buffer[MAXLINE] = {'\0'};
      sprintf(buffer, "%lf", sqrt(converted_operand1));
      push(stack, buffer);
    }

    else 
      push(stack, "0");

    return ;
  }

  // modulus
  if (strcmp(operator, "mod") == 0) {
    char *operand2 = pop(stack);

    if (operand1 != NULL && operand2 != NULL) {
      double converted_operand1 = strtod(operand1, NULL);
      double converted_operand2 = strtod(operand2, NULL);

      char buffer[MAXLINE] = {'\0'};
      sprintf(buffer, "%lf", fmod(converted_operand2, converted_operand1));
      push(stack, buffer);
    }

    else 
      push(stack, "0");

    return ;
  }
  
  // percent
  if (strcmp(operator, "%") == 0) {
    if (operand1 != NULL) {
      double converted_operand1 = strtod(operand1, NULL);

      char buffer[MAXLINE] = {'\0'};
      sprintf(buffer, "%lf", converted_operand1 / 100);
      push(stack, buffer);
    }

    else 
      push(stack, "0");

    return ;
  }
}

void trim_output(char *string) {
  int index = strlen(string) - 1;
  bool is_float = false;

  for (int i = 0; i < index + 1; ++i) {
    printf("%c\n", string[i]);
    if (string[i] == '.') {
      is_float = true;
      break;
    }
  }
  
  if (!is_float)
      return ;

  while (string[index] == '0')
    string[index--] = '\0';

  if (string[index] == '.')
      string[index] = '\0';
}

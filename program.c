#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>

#define LIMIT 500

struct Stack
{
    unsigned int head;
    unsigned int size;
    int *values;
};

int infix2Postfix(struct Stack *stack, char expression[LIMIT], char postExpression[LIMIT]);
void calculatePostfix(int variables[LIMIT], char postfix[LIMIT], char variable, int postLength, struct Stack *stack);
int checkPriority(char sign);
unsigned int pushToStack(struct Stack *stack, int value);
int popFromStack(struct Stack *stack);
int peekStack(struct Stack stack);
bool isStackEmpty(struct Stack stack);
bool isStackFull(struct Stack stack);
void printStack(struct Stack stack);
struct Stack *initializeStack(unsigned int size);
int str2Int(char *str);
void printArray(int *array, size_t length);

int main()
{
    printf("\n");

    FILE *file;

    int i;
    int loop_counter = 1;
    int variable_index = 0;
    int variable_list[LIMIT];
    int postfix_expression_length;
    int expression_length;
    char expression[LIMIT];
    char postfix_expression[LIMIT];
    char variables[LIMIT];

    file = fopen("input.txt", "r");

    struct Stack *stack = initializeStack(LIMIT);

    while (!feof(file))
    {

        memset(postfix_expression, 0, sizeof(expression));
        fscanf(file, "%[^;]", expression);

        expression_length = strlen(expression);
        expression[expression_length] = (char)fgetc(file);

        printf("%d. expression: ", loop_counter);

        puts(expression);
        fgetc(file);

        printf("\n\n");
        printf("- Infix to postfix:\n\n");

        postfix_expression_length = infix2Postfix(stack, expression, postfix_expression);

        printf("\n\n\n");
        printf("Postfix expression : %c = ", expression[0]);

        puts(postfix_expression);

        printf("\n\n\n");
        printf("- Solution: \n\n");

        calculatePostfix(variable_list, postfix_expression, expression[0], postfix_expression_length, stack);

        printf("\n\n\n");
        printf("- Final value of the variable:  %c = %d \n", expression[0], variable_list[expression[0]]);

        variables[variable_index] = expression[0];
        variable_index++;
        loop_counter++;

        if (loop_counter == 3)
        {
            break;
        }

        printf("\n\n");
    }

    printf("\n");
    printf("Final values: \n\n");

    for (i = 0; i < variable_index - 1; i++)
    {
        printf("%c = %d\n", variables[i], variable_list[variables[i]]);
    }

    return 0;
}

int infix2Postfix(struct Stack *stack, char expression[LIMIT], char postExpression[LIMIT])
{
    int i = 4;
    int j = 0;
    int flag = 0;
    char nextChar;
    char nextStack;
    nextChar = expression[i];

    while (nextChar != ';')
    {
        if (nextChar >= '0' && nextChar <= '9')
        {
            while (nextChar != ';' && nextChar != ' ')
            {
                i++;
                postExpression[j] = nextChar;
                j++;
                nextChar = expression[i];
            }
            postExpression[j] = ' ';
            j++;
            if (nextChar == ' ')
            {
                i++;
            }
        }
        else
        {
            if (nextChar == ')')
            {
                nextStack = stack->values[(stack->head) - 1];

                while (!flag)
                {
                    nextStack = stack->values[(stack->head) - 1];
                    if (nextStack == '(')
                    {
                        flag = 1;
                        popFromStack(stack);
                    }
                    else
                    {
                        postExpression[j] = (char)popFromStack(stack);
                        j++;
                        postExpression[j] = ' ';
                        j++;
                    }
                }

                i += 2;
            }
            else
            {
                if ((nextChar >= 'a' && nextChar <= 'z') || (nextChar >= 'A' && nextChar <= 'Z'))
                {
                    postExpression[j] = nextChar;
                    j++;
                    postExpression[j] = ' ';
                    j++;
                    i += 2;
                }
                else
                {

                    if (isStackEmpty(*stack) || nextChar == '(')
                    {
                        pushToStack(stack, nextChar);
                        i += 2;
                    }
                    else
                    {
                        nextStack = stack->values[(stack->head) - 1];

                        if (nextStack == '(')
                        {
                            pushToStack(stack, nextChar);
                            i += 2;
                        }
                        else
                        {
                            if (checkPriority(nextChar) > checkPriority(nextStack))
                            {
                                pushToStack(stack, nextChar);
                                i += 2;
                            }
                            else
                            {
                                postExpression[j] = (char)popFromStack(stack);
                                j++;
                                postExpression[j] = ' ';
                                j++;
                                pushToStack(stack, nextChar);
                                i += 2;
                            }
                        }
                    }
                }
            }
        }
        nextChar = expression[i];
        printf("\n\nPostfix: ");
        printf("%s", postExpression);
        printf("\n");
        printStack(*stack);

    }

    while (!isStackEmpty(*stack))
    {

        postExpression[j] = (char)popFromStack(stack);

        j++;

        postExpression[j] = ' ';

        j++;
    }

    postExpression[j] = '\0';

    printf("\n\nPostfix: ");

    printf("%s", postExpression);

    printf("\n");

    printStack(*stack);

    return j;
}

void calculatePostfix(int variables[LIMIT], char postfix[LIMIT], char variable, int postLength, struct Stack *stack)
{

    int i = 0;
    int j = 0;
    int operand_1;
    int operand_2;
    char number[9];
    char nextChar;
    while (i < postLength)
    {
        nextChar = postfix[i];
        if (nextChar >= '0' && nextChar <= '9')
        {
            while (nextChar != ' ')
            {
                number[j] = nextChar;
                j++;
                number[j] = '\0';
                i++;
                nextChar = postfix[i];
            }
            i++;
            pushToStack(stack, str2Int(number));
        }
        else if ((nextChar >= 'a' && nextChar <= 'z') || (nextChar >= 'A' && nextChar <= 'Z'))
        {
            pushToStack(stack, variables[nextChar]);
            i += 2;
        }
        else if (nextChar == '+' || nextChar == '*' || nextChar == '-' || nextChar == '/')
        {
            operand_1 = popFromStack(stack);
            operand_2 = popFromStack(stack);
            switch (nextChar)
            {
            case '+':
                pushToStack(stack, operand_2 + operand_1);
                break;
            case '-':
                pushToStack(stack, operand_2 - operand_1);
                break;
            case '*':
                pushToStack(stack, operand_2 * operand_1);
                break;
            case '/':
                pushToStack(stack, operand_2 / operand_1);
                break;
            }
            i += 2;
        }
        printf("\n");
        printStack(*stack);
    }
    variables[variable] = popFromStack(stack);
}

int checkPriority(char sign)
{
    if (sign == '/' || sign == '*')
    {
        return 1;
    }
    else if (sign == '+' || sign == '-')
    {
        return 0;
    }
}

/**
 * Pushes a char to a stack. If pushing is succesful returns current size of the
 * stack, if it is not then returns -1
 */
unsigned int pushToStack(struct Stack *stack, int value)
{
    if (!isStackFull(*stack))
    {
        stack->head++;
        stack->values[stack->head - 1] = value;
        return stack->head;
    }
    else
    {
        return -1;
    }
}

/**
 * Pops a char from stack. Returns a char value. 
 * If stack is empty, returns -1
 */
int popFromStack(struct Stack *stack)
{
    if (!isStackEmpty(*stack))
    {
        char *values = stack->values;
        // Last element of the stack
        char value = values[stack->head - 1];
        stack->head--;
        return value;
    }
    else
    {
        return -1;
    }
}

/**
 * Looks at the char at the top of this stack without removing it from the stack.
 * If stack is empty, returns -1
 */
int peekStack(struct Stack stack)
{
    if (isStackEmpty(stack) == 0)
    {
        return stack.values[stack.head - 1];
    }
    else
    {
        return -1;
    }
}

/**
 * Checks whether given stack is empty or not and returns a boolean.
 */
bool isStackEmpty(struct Stack stack)
{
    if (stack.head == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

/**
 * Checks whether given stack is full or not and returns a boolean.
 */
bool isStackFull(struct Stack stack)
{
    if (stack.head == stack.size - 1)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void printStack(struct Stack stack)
{
    for (int i = stack.head - 1; i >= 0; i--)
    {
        printf("|---|\n");
        printf("|-%d-|\n", stack.values[i]);
    }
    printf("|___|\n");
}

/**
 * Initializes a stack
 */
struct Stack *initializeStack(unsigned int size)
{
    struct Stack *stack = (struct Stack *)malloc(sizeof(struct Stack));
    stack->head = 0;
    stack->size = size;
    int *valueArray = (int *)malloc(sizeof(int) * size);
    stack->values = valueArray;
    return stack;
}

/**
 * Converts string (char array) to int
 */
int str2Int(char *str)
{
    int integer = 0;
    int i = 0;
    while (str[i] != '\0')
    {
        integer = integer * 10 + str[i] - '0';
        i++;
    }

    return integer;
}

/**
 * Prints integer array
 */
void printArray(int *array, size_t length)
{
    printf("\n");
    for (int i = 0; i < length; i++)
    {
        printf("%d, ", array[i]);
    }
    printf("\n");
}

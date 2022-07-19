#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#define MY_PI 3.14159
#define COL 80
#define ROW 25
#define pop(sp) (--(sp)), (tops--)
#define top(sp) (*(sp - 1))
#define push(sp, n) (*((sp)++) = (n)), (tops++)

char * Input(FILE* fp, size_t size);
char * PostFixConversion(char * input, int n);
int inpriority(char c);
int outpriority(char c);
int Isoperator(char ptr);
int IsEmpty(int tops);
char FunctionSeeker(char *);
double Evaluation(char * resultant, double num);
double PerformOperation(char operation, double, double);
double PerformFunction(char operation, double operand);
int IsNumericDigit(char c);
int IsFunction(char c);
void PrintGraph(char * postfix);
void CheckParanthesis(char *);

int main(void) {
    char *input;
    input = Input(stdin, 10);
    int n = strlen(input);
    CheckParanthesis(input);
    char *postfix = PostFixConversion(input, n);
    PrintGraph(postfix);
    if (input != NULL)
        free(input);
    if (postfix != NULL)
        free(postfix);
    return 0;
}

char * Input(FILE* fp, size_t size) {
    char *str;
    int ch;
    size_t len = 0;
    str = realloc(NULL, sizeof(*str)*size);
    if(!str)return str;
    while(EOF!=(ch=fgetc(fp)) && ch != '\n'){
        str[len++]=ch;
        if(len==size){
            char * ptr = realloc(str, sizeof(*str)*(size+=20));
            if (ptr != NULL) {
                str = ptr;
            } else {
                free(str);
                printf("Out of memory");
                exit(1);
            }
        }
    }
    str[len++]='\0';
    return realloc(str, sizeof(*str)*len);
}

char * PostFixConversion(char * input, int n) {
    char * result = (char *)malloc(n * 2 * sizeof(char));
    char * sp = (char *)malloc(n*sizeof(char));
    int tops = -1;
    int k = 0;
    for (int i = 0; input[i] != '\0'; i++) {
        if ((input[i] == '-') && !((input[i - 1] <= '9') && (input[i - 1] >= '0'))) {
            result[k] = ' ';
            k++;
            result[k] = '0';
            k++;
            push(sp, input[i]);
            result[k] = ' ';
            k++;
            continue;
        }
        if ((input[i] >= '0' && input[i] <= '9') || input[i] == '.') {
            result[k] = input[i];
            k++;
        } else if (FunctionSeeker(input + i) != 0) {
            char z = FunctionSeeker(input + i);
            if (IsEmpty(tops) || outpriority(z) > inpriority(top(sp))) {
                push(sp, z);
            } else {
                while(!IsEmpty(tops) && outpriority(z) < inpriority(top(sp))) {
                    result[k] = ' ';
                    k++;
                    result[k] = top(sp);
                    k++;
                    result[k] = ' ';
                    k++;
                    pop(sp);
                }
                push(sp, z);
            }
            if (z == 's' || z == 'c' || z == 't' || z == 'k') {
                i = i + 2;
            }
            if (z == 'q')
                i = i + 3;
            if (z == 'l')
                i = i + 1;
        } else if (input[i] == ' ' || input[i] == ',') {
            continue;
        } else if (input[i] == 'x') {
            result[k] = ' ';
            k++;
            result[k] = input[i];
            k++;
            result[k] = ' ';
            k++;
        } else if (Isoperator(input[i]) == 1) {
            if (IsEmpty(tops) || outpriority(input[i]) > inpriority(top(sp))) {
                result[k] = ' ';
                k++;
                push(sp, input[i]);
            } else {
                while(!IsEmpty(tops) && outpriority(input[i]) < inpriority(top(sp))) {
                    result[k] = ' ';
                    k++;
                    result[k] = top(sp);
                    k++;
                    result[k] = ' ';
                    k++;
                    pop(sp);
                }
                push(sp, input[i]);
            }
        } else if (input[i] == '(') {
            push(sp, input[i]);
        } else if (input[i] == ')') {
            while(!IsEmpty(tops) && top(sp) != '(') {
                result[k] = ' ';
                k++;
                result[k] = top(sp);
                k++;
                result[k] = ' ';
                k++;
                pop(sp);
            }
            pop(sp);
        } else {
            printf("Incorrect input\n");
            exit(1);
        }
    }
    while (!IsEmpty(tops)) {
        if (top(sp) != ')' || top(sp) != '(') {
            result[k] = ' ';
            k++;
            result[k] = top(sp);
            pop(sp);
            k++;
            result[k] = ' ';
            k++;
        }
    }
    result[k] = '\0';
    if (sp != NULL)
        free(sp);
    return result;
}

int inpriority(char c) {
    int value = 0;
    if (c == '(')
        value = 0;
    if (c == '+' || c == '-')
        value = 2;
    if (c == '*' || c == '/')
        value = 4;
    if (c == '^')
        value = 5;
    if (c == 's' || c == 'c' || c == 't' || c == 'k' || c == 'q' || c == 'l')
        value = 11;
    return value;
}

int outpriority(char c) {
    int value = 0;
    if (c == '(')
        value = 100;
    if (c == '+' || c == '-')
        value = 1;
    if (c == '*' || c == '/')
        value = 3;
    if (c == '^')
        value = 6;
    if (c == 's' || c == 'c' || c == 't' || c == 'k' || c == 'q' || c == 'l')
        value = 10;
    return value;
}

int Isoperator(char ptr) {
    int value = 0;
    if (ptr == '*' || ptr == '/' || ptr == '-' || ptr == '+' || ptr == '^') {
        value = 1;
    } else {
        value = 0;
    }
    return value;
}

int IsEmpty(int tops) {
    if (tops == -1) return 1;
    return 0;
}

char FunctionSeeker(char * string) {
    char x = 0;
    if (strncmp(string, "sin", 3) == 0) {
        x = 's';
    }
    if (strncmp(string, "cos", 3) == 0) {
        x = 'c';
    }
    if (strncmp(string, "tan", 3) == 0) {
        x = 't';
    }
    if (strncmp(string, "ctg", 3) == 0) {
        x = 'k';
    }
    if (strncmp(string, "sqrt", 4) == 0) {
        x = 'q';
    }
    if (strncmp(string, "ln", 2) == 0) {
        x = 'l';
    }
    return x;
}

double PerformOperation(char operation, double operand2, double operand1) {
    double res = 0;
    if(operation == '+')
        res = operand1 +operand2;
    else if(operation == '-')
        res = operand1 - operand2;
    else if(operation == '*')
        res = operand1 * operand2;
    else if(operation == '/')
        res = operand1 / operand2;
    else if(operation == '^')
        res = pow(operand1, operand2);
    return res; 
}

double PerformFunction(char operation, double operand) {
    double result = 0;
    if (operation == 'q')
        result = sqrt(operand);
    if (operation == 'l')
        result = log(operand);
    if (operation == 's')
        result = sin(operand);
    if (operation == 'c')
        result = cos(operand);
    if (operation == 't')
        result = tan(operand);
    if (operation == 'k')
        result = 1 / tan(operand);
    return result;
}

int IsNumericDigit(char c) {
    if(c >= '0' && c <= '9')
        return 1;
    return 0;
}

int IsFunction(char c) {
    int value = 0;
    if (c == 's' || c == 'c' || c == 't' || c == 'k' || c == 'q' || c == 'l')
        value = 1;
    return value;
}

double Evaluation(char * result, double num) {
    double stack[100];
    double * sp = stack;
    int tops = -1;
    for (char * resultant = result; *resultant != '\0'; resultant++) {
        if (*resultant == 'x') {
            push(sp, num);
        } else if (*resultant == ' ') {
            continue;
        } else if (Isoperator(*resultant)) {
            double op1 = top(sp);
            pop(sp);
            double op2 = top(sp);
            pop(sp);
            double result1 = PerformOperation(*resultant, op1, op2);
            push(sp, result1);
        } else if (IsFunction(*resultant)) {
            double operand = top(sp);
            pop(sp);
            double result1 = PerformFunction(*resultant, operand);
            push(sp, result1);
        } else if (IsNumericDigit(*resultant)) {
            double operand = strtod(resultant, &resultant);
            push(sp, operand);
        }
    }
    return top(sp);
}

void PrintGraph(char * postfix) {
    double st_x = 0;
    double end_x = 4 * MY_PI;
    double stp_x = (end_x - st_x) / COL;
    double st_y = -1;
    double end_y = 1;
    double stp_y = (end_y - st_y) / ROW;
    for (int r = 0; r < ROW; r++) {
        for (double x = st_x; x <= end_x; x += stp_x) {
            double posy = Evaluation(postfix, x);
            double check_s = st_y + r * stp_y;
            double check_end = check_s + stp_y;
            if (posy >= check_s && posy <= check_end) {
                putchar('*');
            } else {
                putchar('.');
            }
        }
        putchar('\n');
    }
}

void CheckParanthesis(char * string) {
    char stack[50];
    char * sp = stack;
    int tops = -1;
    for (char * ptr = string; *ptr != '\0'; ptr++) {
        if (*ptr == '(') {
            push(sp, *ptr);
        }
        if (*ptr == ')' && top(sp) == '(') {
            pop(sp);
        }
    }
    if (tops != -1) {
        free(string);
        exit(1);
    }
}
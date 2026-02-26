#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

typedef enum {
    TO_MM,
    TO_IN
} t_unit;

// Forward declarations
double parseExpression( char **expr);
double parseTerm( char **expr);
double parseFactor( char **expr);

// Helper to skip whitespace and peek at the next char
void skipWhiteSpace(char **expr) {
    while (isspace(**expr)) {
        (*expr)++;
    }
}

double parseFactor(char **expr) {
    skipWhiteSpace(expr);
    
    char *end;
    double val = strtod(*expr, &end);
    
    if (*expr == end){
        return 0;
    }
    
    *expr = end; //update the bookmark for caller
    return val;
}

double parseTerm(char **expr) {
    double nodes = parseFactor(expr);
    skipWhiteSpace(expr);
    
    while (**expr == '*' || **expr == '/'){
        char op = *(*expr)++;
        double nextFactor = parseFactor(expr);
        
        if (op == '*'){
            nodes *= nextFactor;
        } else {
            if (nextFactor != 0) {
                nodes /= nextFactor;
            } else {
                fprintf(stderr, "Error: Divide by zero\n");
            }
        }
        skipWhiteSpace(expr);
    }
    return nodes;
}

double parseExpression(char **expr){
    double nodes = parseTerm(expr);
    skipWhiteSpace(expr);
    
    while (**expr == '+' || **expr == '-'){
        char op = *(*expr)++;
        double nextTerm = parseTerm(expr);
        
        if (op == '+'){
            nodes += nextTerm;
        } else {
            nodes -= nextTerm;
        }
        skipWhiteSpace(expr);
    }
    return nodes;
}

void convert(char* input, char* output){
    
    if (strchr(input, 'i') != NULL){
        double coef = atof(input);
        coef *= 25.4;
        sprintf(output, "%g", coef);
    } else {
        double coef = atof(input);
        sprintf(output, "%g", coef);
    }
    
}


void splitBuffer(char* input, char* output)
{
    char term[32] = {0};
    char convertedTerm[32] = {0};
    int termIdx = 0;
    output[0] = '\0';
    
    for (int i = 0; i <= strlen(input); i++)
    {
        char c = input[i];
        
        if (c == '+'|| c == '-'|| c == '*'|| c == '/' || c == '\0') {
            
            if (termIdx > 0) {
                term[termIdx] = '\0';
                convert(term, convertedTerm);
                strcat(output, convertedTerm);
                termIdx = 0;
            }
            
            if (c != '\0'){
                int len = strlen(output);
                output[len] = c;
                output[len + 1] = '\0';
            }
        } else if (c != ' ') {
            term[termIdx++] = c;
        }
    }
    
}

double unitConversion(double input, int unit){
    if (unit){
        return input/25.4;
    } else {
        return input;
    }
}

void convertToString(double value, char *inputString, int unit){
    snprintf(inputString, sizeof(inputString), "%g", value);
    if (unit){
        strcat(inputString, "in");
    } else {
        strcat(inputString, "mm");
    }
}

void calculation(char *buffer, bool unit, char *calculatedString){
    
    char convertedBuffer[100] = {0};
    char outputString[16] = {0};
    
    splitBuffer(buffer, convertedBuffer);
    
    char *ptr = convertedBuffer;
    double value = parseExpression(&ptr);
    
    double convertedValue = unitConversion(value, unit);
    
    convertToString(convertedValue, outputString, unit);
    
    strcpy(calculatedString, outputString);
    
}

int main() {
    
    bool unit = TO_IN;
    char buffer[] = "0.3725in*3.2mm";
    char calculatedString[16] = {0};
    
    calculation(buffer, unit, calculatedString);
    
    printf("%s\n", buffer);
    printf("%s\n", calculatedString);
    
    return 0;
}


#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAX_INPUT 64
#define MAX_OUTPUT 16

typedef struct {
	char input_buffer[MAX_INPUT];
	char outputString[MAX_OUTPUT];
	uint8_t input_length;
} calculator_t;


typedef enum {
	TO_MM,
	TO_IN
} unit_t;

typedef enum {
	KEY_MM,
	KEY_IN,
	KEY_TOMM,
	KEY_TOIN,
	KEY_7,
	KEY_8,
	KEY_9,
	KEY_DIVIDE,
	KEY_4,
	KEY_5,
	KEY_6,
	KEY_MULTIPLY,
	KEY_1,
	KEY_2,
	KEY_3,
	KEY_SUBTRACT,
	KEY_0,
	KEY_DECIMAL,
	KEY_C,
	KEY_ADD
} Key_t;

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

void appendChar(const char c, calculator_t *calc)
{
	if (calc->input_length < MAX_INPUT - 1) {
		calc->input_buffer[calc->input_length++] = c;
		calc->input_buffer[calc->input_length] = '\0';
	}
}

void appendString(const char *unit, calculator_t *calc) {
	if (calc->input_length < MAX_INPUT - 2)
	{
		memcpy(&calc->input_buffer[calc->input_length], unit, 2);
		calc->input_length += 2;
		calc->input_buffer[calc->input_length] = '\0';
	}
}

void clearBuffer(calculator_t *calc)
{
	calc->input_length = 0;
	calc->input_buffer[0] = '\0';
}

char keyToChar(Key_t key)
{
	switch (key)
	{
	case KEY_0:
		return '0';
	case KEY_1:
		return '1';
	case KEY_2:
		return '2';
	case KEY_3:
		return '3';
	case KEY_4:
		return '4';
	case KEY_5:
		return '5';
	case KEY_6:
		return '6';
	case KEY_7:
		return '7';
	case KEY_8:
		return '8';
	case KEY_9:
		return '9';
	default:
		return '\0';
	}
}

void processKey(Key_t key, calculator_t *calc) {

	char digit = keyToChar(key);
	if (digit) {
		appendChar(digit, calc);
		return;
	}

	switch(key)
	{
	case KEY_DECIMAL:
		appendChar('.', calc);
		break;
	case KEY_ADD:
		appendChar('+', calc);
		break;
	case KEY_SUBTRACT:
		appendChar('-', calc);
		break;
	case KEY_MULTIPLY:
		appendChar('*', calc);
		break;
	case KEY_DIVIDE:
		appendChar('/', calc);
		break;

	case KEY_MM:
		appendString("mm", calc);
		break;
	case KEY_IN:
		appendString("in", calc);
		break;
	case KEY_C:
		clearBuffer(calc);
		break;
	case KEY_TOIN:
		calculation(calc->input_buffer, TO_IN, calc->outputString);
		break;
	case KEY_TOMM:
		calculation(calc->input_buffer, TO_MM, calc->outputString);
		break;
	default:
		break;
	}
}

int main()
{

	calculator_t calc = {0};

	processKey(KEY_2, &calc);
	processKey(KEY_5, &calc);
	processKey(KEY_DECIMAL, &calc);
	processKey(KEY_4, &calc);
	processKey(KEY_MM, &calc);
	processKey(KEY_ADD, &calc);
	processKey(KEY_1, &calc);
	processKey(KEY_IN, &calc);
	processKey(KEY_TOIN, &calc);
	
	printf("%s\n", calc.input_buffer);
	printf("%s\n", calc.outputString);

	return 0;
}

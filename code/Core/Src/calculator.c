#include "calculator.h"
#include <math.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ssd1305.h"
#include "processKey.h"
#include "types.h"
#include <stdbool.h>

void skipWhiteSpace(char **expr) {
    while (isspace((unsigned char)**expr)) {
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
            if (nextFactor != 0.0) {
                nodes /= nextFactor;
            } else {
                return NAN;
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

void convert(char* input, char* output, unitFlags_t *flags){
    
    double val = atof(input);
    if (strchr(input, 'i') != NULL) {
        val *= 25.4;
        flags->inFlag = true;
    } else if (strchr(input, 'm') != NULL){
        flags->mmFlag = true;
    } else {
        flags->blankFlag = true;
    }
    sprintf(output, "%g", val);
}

void splitBuffer(char* input, char* output, unitFlags_t *flags)
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
                convert(term, convertedTerm, flags);
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

double unitConversion(double input, int unit, unitFlags_t *flags){
        
    if (flags->blankFlag && !flags->inFlag && !flags->mmFlag){
        if (unit){
            return input/25.4;
        } else {
            return input*25.4;
        }
    } else if (flags->blankFlag && flags->inFlag && flags->mmFlag){
        return -1000.0;
    } else {
        if (unit){
            return input/25.4;
        } else {
            return input;
        }
    }   
}

void convertToString(double value, char *inputString, int unit, size_t inputSize){

	double rounded = round(value);
	    if (fabs(value - rounded) < 0.001) {
	        value = rounded;
	    }
    
    if (isnan(value))
    {
        snprintf(inputString, inputSize, "ERROR");
        return;
    }
    if (isinf(value))
    {
        snprintf(inputString, inputSize, "OVERFLOW");
        return;
    }
    if (value == -1000.0)
    {
        snprintf(inputString, inputSize, "ASSIGN UNITS");
        return;
    }
    
    int len = snprintf(inputString, inputSize, "%.4g", value);
    if (unit == TO_IN){
        strncat(inputString, "in", inputSize - len - 1);
    } else {
        strncat(inputString, "mm", inputSize - len - 1);
    }
}

void calculation(char *buffer, unit_t unit, char *calculatedString, char *previousString){

    unitFlags_t flags = {0};

    char convertedBuffer[MAX_INPUT * 2] = {0};
    char outputString[MAX_OUTPUT] = {0};
    
    splitBuffer(buffer, convertedBuffer, &flags);
    
    char *ptr = convertedBuffer;
    double value = parseExpression(&ptr);
    
    double convertedValue = unitConversion(value, unit, &flags);
    
    convertToString(convertedValue, outputString, unit, MAX_OUTPUT);
    
    strncpy(previousString, buffer, MAX_OUTPUT - 1);

    if (strlen(previousString) + 1 < MAX_OUTPUT) {
        previousString[strlen(previousString)] = '=';      // Place the char at the old null position
        previousString[strlen(previousString) + 1] = '\0';   // Manually add the new null terminator
    }

    strncpy(buffer, outputString, MAX_OUTPUT - 1);

    displayString(previousString);
    //strncpy(calculatedString, testString, MAX_OUTPUT - 1);
}

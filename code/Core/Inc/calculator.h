#ifndef CALCULATOR_H
#define CALCULATOR_H
#include "constants.h"
#include "types.h"
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

double parseExpression( char **expr);
double parseTerm( char **expr);
double parseFactor( char **expr);
void skipWhiteSpace(char **expr);
void convert(char* input, char* output, unitFlags_t *flags);
void splitBuffer(char* input, char* output, unitFlags_t *flags);
double unitConversion(double input, int unit, unitFlags_t *flags);
void convertToString(double value, char *inputString, int unit, size_t inputSize);
void calculation(char *buffer, unit_t unit, char *calculatedString, char *previousString);

#endif

#ifndef PROCESS_KEY_H
#define PROCESS_KEY_H
#include "constants.h"
#include "types.h"
#include <string.h>

void appendChar(const char c, calculator_t *calc);
void appendString(const char *unit, calculator_t *calc);
void clearBuffer(calculator_t *calc);
char keyToChar(key_t key);
void processKey(key_t, calculator_t *calc);

#endif
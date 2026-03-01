#ifndef TYPE_H
#define TYPE_H
#include "constants.h"
#include <stdint.h>
#include <stdbool.h>

typedef struct{
	bool mmFlag;
	bool inFlag;
	bool blankFlag;
} unitFlags_t;

typedef struct{
    char input_buffer[MAX_INPUT];
    char output_buffer[MAX_INPUT];
	char previous_buffer[MAX_INPUT];
    uint8_t input_length;
} calculator_t;

typedef enum {
    TO_MM,
    TO_IN
} unit_t;

typedef enum {
	KEY_MM,
	KEY_7,
	KEY_4,
	KEY_1,
	KEY_C,
	KEY_IN,
	KEY_8,
	KEY_5,
	KEY_2,
	KEY_0,
	KEY_TOMM,
	KEY_9,
	KEY_6,
	KEY_3,
	KEY_DECIMAL,
	KEY_TOIN,
	KEY_DIVIDE,
	KEY_MULTIPLY,
	KEY_SUBTRACT,
	KEY_ADD
} key_t;

#endif

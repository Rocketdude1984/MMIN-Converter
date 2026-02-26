
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#define MAX_INPUT 64

typedef struct {
	char input_buffer[MAX_INPUT];
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

void calculation(calculator_t *calc, unit_t unit) {
	printf("%s\n", calc->input_buffer);
	printf("%d", unit);
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
		calculation(calc, TO_IN);
		break;
	case KEY_TOMM:
		calculation(calc, TO_MM);
		break;
	default:
		break;
	}
}

int main()
{

	calculator_t calc = {0};

	processKey(KEY_3, &calc);
	processKey(KEY_DIVIDE, &calc);
	processKey(KEY_4, &calc);
	processKey(KEY_IN, &calc);
	processKey(KEY_ADD, &calc);
	processKey(KEY_1, &calc);
	processKey(KEY_2, &calc);
	processKey(KEY_MM, &calc);

	processKey(KEY_TOMM, &calc);

	return 0;
}

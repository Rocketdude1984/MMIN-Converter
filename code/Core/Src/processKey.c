#include "processKey.h"
#include "calculator.h"

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

char keyToChar(key_t key)
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

void processKey(key_t key, calculator_t *calc) {

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
		calculation(calc->input_buffer, TO_IN, calc->output_buffer, calc->previous_buffer);
		break;
	case KEY_TOMM:
		calculation(calc->input_buffer, TO_MM, calc->output_buffer, calc->previous_buffer);
		break;
	default:
		break;
	}
}

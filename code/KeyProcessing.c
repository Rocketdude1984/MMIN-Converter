
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#define MAX_INPUT 64

char input_buffer[MAX_INPUT];
uint8_t input_length = 0;
bool decimal_used = false;

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

void appendChar(char c)
{
    if (input_length < MAX_INPUT - 1) {
        input_buffer[input_length++] = c;
        input_buffer[input_length] = '\0';
    }
}

void appendString(char *unit){
    if (input_length < MAX_INPUT - 2){
        strcat(input_buffer, unit);
    }
}

void calculation(){
    printf(input_buffer);
}

void processKey(Key_t key){
    switch(key)
    {
        case KEY_0: appendChar('0'); break;
        case KEY_1: appendChar('1'); break;
        case KEY_2: appendChar('2'); break;
        case KEY_3: appendChar('3'); break;
        case KEY_4: appendChar('4'); break;
        case KEY_5: appendChar('5'); break;
        case KEY_6: appendChar('6'); break;
        case KEY_7: appendChar('7'); break;
        case KEY_8: appendChar('8'); break;
        case KEY_9: appendChar('9'); break;
        
        case KEY_DECIMAL: appendChar('.'); break;
        
        case KEY_ADD: appendChar('+'); break;
        case KEY_SUBTRACT: appendChar('-'); break;
        case KEY_MULTIPLY: appendChar('*'); break;
        case KEY_DIVIDE: appendChar('/'); break;
        
        case KEY_MM: appendString("mm"); break;
        case KEY_IN: appendString("in"); break;
        
        case KEY_TOIN: calculation(); break;
        case KEY_TOMM: calculation(); break;
        
        default: break;
        
    }
}

int main()
{
    processKey(KEY_3);
    processKey(KEY_DIVIDE);
    processKey(KEY_4);
    processKey(KEY_IN);
    processKey(KEY_ADD);
    processKey(KEY_1);
    processKey(KEY_2);
    processKey(KEY_MM);
    processKey(KEY_TOIN);
    
    return 0;
}

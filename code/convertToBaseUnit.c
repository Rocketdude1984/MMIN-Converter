#include <stdio.h>
#include <string.h>
#include <stdlib.h>

double convertToBaseUnit(char buffer[])
{
    char valueString[16];
    double value = 0;
    int stringCount = 0;
    for (int c = 0; c < strlen(buffer); c++){
        
        if (buffer[c] >= '0' && buffer[c] <= '9' || buffer[c] == '.')
        {
            valueString[c] = buffer[c]; 
            stringCount++;
        }
    }
    valueString[stringCount] = '\0';
    value = atof(valueString);
    char *remainingString = buffer + stringCount;
    if (remainingString[0] == 'i') 
    {
        value *= 25.4;
    }
    //printf("%.3lf\n", value);
    
    return value;
}

void float_to_string(float num, char *buffer, int buffer_size) {
    // %.2f limits the output to 2 decimal places. 
    // Use %f for default precision (usually 6).
    snprintf(buffer, buffer_size, "%.3f", num);
}

int main(){
    
    char input_buffer[32];
    char convertedString[32];
    convertedString[0] = '\0';
    float_to_string(convertToBaseUnit("1in"), input_buffer, sizeof(input_buffer));
    strcat(convertedString, input_buffer);
    float_to_string(convertToBaseUnit("12mm"), input_buffer, sizeof(input_buffer));
    strcat(convertedString, input_buffer);
    printf("%s", convertedString);
    
    return 0;
}

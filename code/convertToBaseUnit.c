#include <stdio.h>
#include <stdlib.h>
#include <string.h>


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

int main(){
    
    char buffer[] = "1in+25.4mm+13in*55mm-15";
    char convertedBuffer[100] = {0};
    
    splitBuffer(buffer, convertedBuffer);
    
    printf("Orginal Buffer: %s\n", buffer);
    printf("convertedBuffer: %s\n", convertedBuffer);
    
    return 0;
}

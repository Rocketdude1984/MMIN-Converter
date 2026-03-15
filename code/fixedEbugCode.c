#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct{
	bool mmFlag;
	bool inFlag;
	bool blankFlag;
} unitFlags_t;

void splitBuffer(char* input, char* output, unitFlags_t *flags);
void convert(char* input, char* output, unitFlags_t *flags);

void splitBuffer(char* input, char* output, unitFlags_t *flags)
{

  \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\START of Edits\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
    char term[32] = {0};
    char convertedTerm[32] = {0};
    int termIdx = 0;
    output[0] = '\0';
    bool eFlag = false;
    
    for (int i = 0; i <= strlen(input); i++)
    {
        char c = input[i];
            
        if ((c == '+'|| c == '-'|| c == '*'|| c == '/' || c == '\0') && !eFlag) {
            
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
        
        if (c == 'e')
        {
            eFlag = true;
        }
        
        if ((c == '+' || c == '-') && eFlag)
        {
            eFlag = false;
        }
      \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\END of Edits\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
    }
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

int main()
{
    char testStr[] = "5e+05mm-300mm";
    char outputStr[32];
    unitFlags_t flags = {0};
    
    splitBuffer(testStr, outputStr, &flags);
    printf("Original string: %s\n", testStr);
    printf("Reformatted string: %s\n", outputStr);
    return 0;
}

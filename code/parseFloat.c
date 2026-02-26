#include <stdio.h>

float parseFloat(const char *str)
{
    float result = 0.0f;
    float fraction = 0.0f;
    float divisor = 10.0f;
    int sign = 1;
    
    if (*str == '-')
    {
        sign = -1;
        str++;
    } else if (*str == '+')
    {
        str++;
    }
    
    while (*str >= '0' && *str <= '9')
    {
        result = result * 10.0f + (*str - '0');
        str++;
    }
    
    if (*str == '.')
    {
        str++;
        
        while (*str >= '0' && *str <= '9')
        {
            fraction += (*str - '0') / divisor;
            divisor *= 10.0f;
            str++;
        }
    }
    return sign * (result + fraction);
}

int main()
{
    printf("%g\n", parseFloat("-2.5123mm"));
    
    return 0;
}

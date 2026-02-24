#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

const char *expressionToParse;

// Forward declarations
double parseExpression();
double parseTerm();
double parseFactor();

// Helper to skip whitespace and peek at the next char
char peek() {
    return *expressionToParse;
}

char get() {
    return *expressionToParse++;
}

double parseFactor() {
    if (peek() >= '0' && peek() <= '9') {
        char *end;
        double val = strtod(expressionToParse, &end);
        expressionToParse = end;
        return val;
    } else if (peek() == '(') {
        get(); // consume '('
        double val = parseExpression();
        get(); // consume ')'
        return val;
    }
    return 0;
}

double parseTerm() {
    double nodes = parseFactor();
    while (peek() == '*' || peek() == '/') {
        char op = get();
        if (op == '*') nodes *= parseFactor();
        else nodes /= parseFactor();
    }
    return nodes;
}

double parseExpression() {
    double nodes = parseTerm();
    while (peek() == '+' || peek() == '-') {
        char op = get();
        if (op == '+') nodes += parseTerm();
        else nodes -= parseTerm();
    }
    return nodes;
}

int main() {
    expressionToParse = "12.56+2.33*2";
    printf("Result: %f\n", parseExpression());
    return 0;
}

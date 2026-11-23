#ifndef CALCULATOR_H
#define CALCULATOR_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct { char value[64]; double stored; char op; int fresh, error; } Calculator;
static void calc_clear(Calculator *c) { memset(c, 0, sizeof(*c)); strcpy(c->value, "0"); }
static void calc_digit(Calculator *c, char key) {
    if (c->error) calc_clear(c);
    if (c->fresh) { strcpy(c->value, "0"); c->fresh = 0; }
    size_t n = strlen(c->value);
    if (key == '.') { if (!strchr(c->value, '.')) { c->value[n] = '.'; c->value[n+1] = 0; } }
    else if (!strcmp(c->value, "0")) { c->value[0] = key; }
    else if (n < 12) { c->value[n] = key; c->value[n+1] = 0; }
}
static int calc_apply(Calculator *c) {
    double b = strtod(c->value, NULL), result = 0;
    if (c->op == '/' && b == 0) { c->error = 1; c->op = 0; return 0; }
    switch (c->op) {
        case '+': result = c->stored + b; break;
        case '-': result = c->stored - b; break;
        case '*': result = c->stored * b; break;
        case '/': result = c->stored / b; break;
    }
    if (!isfinite(result)) { c->error = 2; c->op = 0; return 0; }
    snprintf(c->value, sizeof(c->value), "%.12g", result == 0 ? 0 : result);
    return 1;
}
static void calc_operator(Calculator *c, char op) {
    if (c->error) return;
    if (c->op && !c->fresh && !calc_apply(c)) return;
    c->stored = strtod(c->value, NULL); c->op = op; c->fresh = 1;
}
static void calc_equals(Calculator *c) {
    if (!c->op || c->error) return;
    if (calc_apply(c)) { c->op = 0; c->fresh = 1; }
}
#endif

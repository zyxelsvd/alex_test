#include <assert.h>
#include "calculator.h"
static Calculator c;
static void sequence(const char *keys, const char *expected) {
    calc_clear(&c);
    for (; *keys; keys++) {
        if ((*keys >= '0' && *keys <= '9') || *keys == '.') calc_digit(&c,*keys);
        else if (*keys == '=') calc_equals(&c);
        else calc_operator(&c,*keys);
    }
    assert(!c.error); assert(!strcmp(c.value,expected));
}
int main(void) {
    sequence("8+2=","10"); sequence("8-2=","6");
    sequence("8*2=","16"); sequence("8/2=","4");
    sequence("0.1+0.2=","0.3"); sequence("2+3*4=","20");
    sequence("8+-2=","6"); sequence("2-8=","-6");
    sequence("1..5+2=","3.5"); sequence("2+3=7","7");
    sequence("2+3=*2=","10"); sequence("0+0=","0");
    calc_clear(&c); calc_digit(&c,'8'); calc_operator(&c,'/'); calc_digit(&c,'0'); calc_equals(&c);
    assert(c.error == 1); calc_digit(&c,'5'); assert(!c.error && !strcmp(c.value,"5"));
    calc_operator(&c,'+'); calc_clear(&c); assert(!c.op && !c.error && !strcmp(c.value,"0"));
    puts("All calculator tests passed.");
}

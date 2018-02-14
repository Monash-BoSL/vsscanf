#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <stdarg.h>
#include <stdlib.h>
#include "vsscanf.h"
//#include <stdio.h>

#define BUF_SIZE 512

int sscanf(char *str, const char *fmt, ...)
{
    va_list args;
    int ret;
    va_start(args, fmt);
    ret = vsscanf(str, fmt, args);
    va_end(args);

    return ret;
}

int test_vsscanf()
{
    unsigned int n;
    long long z;
    int x, y;
    unsigned int u = 0;
    char *str1, *str2;
    char s1[128], s2[64];
    char c1, c2;
    x = y = 0;
    n = sscanf("test", NULL, &x);
    assert(x == 0);
    n = sscanf("123:333\n", "%d:%d\n", &x, &y);
    assert(x == 123);
    assert(y == 333);
    assert(n == 2);
    x = y = 0;
    n = sscanf("444 : 222", "%d:%d", &x, &y);
    assert(x == 444);
    assert(y == 0);
    assert(n == 1);
    n = sscanf(":-1234567890123456", ":%Ld", &z);
    assert(z == -1234567890123456);
    n = sscanf("0XDEADC0DE", "%x", &u);
    assert(u == 0xDEADC0DE);
    n = sscanf("--0XDEADbabe", "--%X", &u);
    assert(u == 0xDEADBABE);
    n = sscanf("--DEADbabe", "--%4X", &u);
    assert(u == 0xDEAD);

    x = y = 0;
    n = sscanf("3333-4444", "%2d-%2d", &x, &y);
    assert(x == 33);
    assert(y == 0);

    n = sscanf("  \n\r\t\f123456", " \t%2d%3d", &x, &y);
    assert(x == 12);
    assert(y == 345);

    z=0;
    n = sscanf("   -123456", "%3ld", &z);
    assert(z == -12);
    n = sscanf("-123", "%4lld", &z);
    assert(z == -123);
    n = sscanf(" t e \n", " %c%c", &c1, &c2);
    assert(n==2);
    assert(c1 == 't');
    assert(c2 == ' ');
    n = sscanf("t23", "%c  \r\t\n %c", &c1, &c2);
    assert(n == 2);
    assert(c1 == 't');
    assert(c2 == '2');
   
    // octals
    n = sscanf("022233344", "%o", &x);
    assert(x == 022233344);
    assert(n == 1);
    // malformed octal
    n = sscanf("02223338844", "%o", &x);
    assert(x == 0222333);
    assert(n == 1);


    n = sscanf(" \nLorem\n\nIpsum is simply dummy text", " %s  %s ", &s1, &s2);
    assert(strcmp(s1, "Lorem") == 0);
    assert(strcmp(s2, "Ipsum") == 0);

    n = sscanf(" \nLorem\n\nIpsum is dummy text", " %3s  %4s ", &s1, &s2);
    assert(strcmp(s1, "Lor") == 0);
    assert(strcmp(s2, "em") == 0);

#if 0
    // Malloc-ated - this is not POSIX
    str1 = str2 = NULL;
    n = sscanf(" \nLorem\n\nIpsum is dummy text", " %3ms  %5ms ", &str1, &str2);
    assert(strcmp(str1, "Lor") == 0);
    assert(strcmp(str2, "em") == 0);
    free(str1); free(str2);
   
    str1 = str2 = NULL;
    n = sscanf("LoremIpsumisdummy text", "%ms %ms", &str1, &str2);
    assert(strcmp(str1, "LoremIpsumisdummy") == 0);
    assert(strcmp(str2, "text") == 0);
    free(str1);free(str2);
#endif
    // Sets
    // Skip one match - aka '*'
    s1[0] = s2[0] = 0;
    n = sscanf(" Lorem Ipsum is dummy text", " %*s %s", &s1, &s2);
    assert(strcmp(s1, "Ipsum") == 0);
    assert(strcmp(s2, "") == 0);

    s1[0] = s2[0] = 0;
    n = sscanf("Lorem Ipsum is dummy text", "%[^]I] %[mupsI]", &s1, &s2);
    assert(strcmp(s1, "Lorem ") == 0);
    assert(strcmp(s2, "Ipsum") == 0);
    
    char Y[16], m[16], d[16], h[16], i[16], s[16];
    n = sscanf("2018-02-12, 16:02:05\n", 
        "%4[0-9]%*[ -]%2[0-9]%*[ -]%2[0-9]%*[^0-9]%2[0-9]:%2[0-9]:%2[0-9]", 
        &Y, &m, &d, &h, &i, &s);
    assert(strcmp(Y, "2018") == 0);
    assert(strcmp(m, "02") == 0);
    assert(strcmp(d, "12") == 0);
    assert(strcmp(h, "16") == 0);
    assert(strcmp(i, "02") == 0);
    assert(strcmp(s, "05") == 0);

    n = sscanf(" Lorem Ipsum", "%4[^r]", &s1);
    assert(strcmp(s1, " Lo") == 0);

    // test literal % %%
    n = sscanf("123 % 456", "123 %%%128s", &s1);
    assert(n == 1);
    assert(strcmp(s1, "456"));
    n = sscanf("123 % 456", "123 %c", &c1);
    assert(c1 == '%');
    assert(n == 1);
    return 0;
}

int main()
{
    if (test_vsscanf() == 0) {
        return 0;
    }

    return 1;
}

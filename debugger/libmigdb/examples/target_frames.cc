#include <stdio.h>
#include <string.h>

int v=2; struct { char *a; int q; } sup;

int FuncLev2(char *str)
{
 v=5;
 return strlen(str);
}

struct nested
{
    int nested_x;
    int nested_y;
};

struct test
{
    int field_xxx;
    int field_yyy;
    nested n;
};

int FuncLev1(char *s, int i)
{
 char string[] = {'f', 'u', 'c', 'k', 't', 'h', 'e', 's', 'y', 's', 't', 'e', 'm', '\0'};
 test tt;
 tt.field_xxx = 10;
 tt.field_yyy = 50;
 int v1 = 7;
 int v2 = 8;
 i+=FuncLev2(s);
 tt.field_xxx += 19;
 for (int k = 5; k < 10; k++)
 {
    int e = k;
    v2 += e;
 }
 string[40000000] = 5;
 return i;
}

int main(int argc, char *argv[])
{
 FuncLev1("Hi!",2500);
 printf("v=%d\n",v);
 return 0;
}

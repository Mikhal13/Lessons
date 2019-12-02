#include <stdio.h>

union complex_type
{
    char c_v;
    int i_v;
    float f_v;
} c_t;
 
int main()
{
    printf("\n");
    c_t.c_v = 'a';                              /* Не инициализирует c_t. */
    c_t.i_v = 13;                               /* Не инициализирует c_t. */
    c_t.f_v = 1.1e2;                            /* Инициализирует c_t. */
    printf("c_t.c_v     = %c     ", c_t.c_v);   /* Не читаемый символ, */
    printf("Code c_t.c_v= %u     ", c_t.c_v);   /* код которого равен 0. */
    printf("c_t.i_v     = %i     ", c_t.i_v);   /* Печатает какое-то целое число. */
    printf("c_t.f_v     = %f     ", c_t.f_v);   /* Печатает 110, что надо. */
    printf("\n");

    c_t.c_v++;                                  /* Не инкрементит. */
    c_t.i_v++;                                  /* Инкрементит два раза */
    c_t.f_v++;                                  /* Инкрементит с ошибкой. */
    printf("c_t.c_v     = %c     ", c_t.c_v);   /* Не читаемый символ, */
    printf("Code c_t.c_v= %u     ", c_t.c_v);   /* код которого равен 2. */
    printf("c_t.i_v     = %i     ", c_t.i_v);   /* Печатает какое-то (иногда такое же) целое число. */
    printf("c_t.f_v     = %f     ", c_t.f_v);   /* Печатает 111.000015 вместо 111. */
    printf("\n");

    return 0;
}
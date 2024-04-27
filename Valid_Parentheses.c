#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
int n;
char array[10001];
void push(char c)
{
    n += 1;
    array[n] = c;
}

char pop()
{
    n -= 1;
    return (array[n + 1]);
}

char top()
{
    if (n < 0)
        return (0);
    return (array[n]);
}

int isOParen(char c)
{
    if (c == '[' || c == '{' || c == '(')
        return (1);
    return (0);
}

int isPair(char in, char out)
{
    if (in == '{' && out == '}')
        return (1);
    else if (in == '[' && out == ']')
        return (1);
    else if (in == '(' && out == ')')
        return (1);
    return (0);
}

int isValid(char* s) {
    int i = 0;
    n = -1;

    while (s[i])
    {
        // printf("n = %d\n", n);
        if (isOParen(s[i])){
            push(s[i]);
            // printf("push %c\n", s[i]);
        }
        else
        {
            printf("Pairing %c %c\n",top(), s[i]);
            if (isPair(top(), s[i]))
            {
                printf("Paired %c %c\n",top(), s[i]);
                pop();
            }
            else
                return (0);
        }
        i++;
    }
    // printf("n = %d\n", n);
    if (n != -1)
        return (0);
    return (1);
}

int main(int ac, char **av)
{
    if (av[1])
        printf("%d", isValid(av[1]));
    return (0);
}
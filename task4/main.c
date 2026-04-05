#include <stdio.h>
#include <string.h>

#define MAX_SIZE 1024

void args_parser(const char* s)
{

}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        printf("Short Options: \n");
        printf("Long Options: \n");
        printf("Non options: \n");
        return 0;
    }

    int count = argc - 1;
    char s[count];
    for (int i = 0; i < count; ++i)
    {
        s[i] = argv[i + 1];
    }




}
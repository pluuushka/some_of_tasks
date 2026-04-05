#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


#define MAX_LINE 4096

int has_any_char(const char* s)
{
    while (*s)
    {
        if (isgrapgh((usigned char)*s))
        {
            return 1;
        }
        s++;
    }

    return 0;
}


int main(int argc, char *argv)
{
    if (argc != 4)
    {
        perror("must be 4 args!\n");
        exit(1);
    }

    const char *infile = argv[1];
    const char *outfile = argv[2];
    const char *mode = argv[3];

    FILE *fin = fopen(infile, "r");
    if (!fin)
    {
        perror("fopen input");
        exit(1);
    }

    char **lines = NULL;
    int count = 0;
    char buffer[MAX_LINE];

    while (fgets(buffer, sizeof(buffer), fin))
    {
        buffer[strcspn(buffer, "\n")] = '\0';
        if (has_any_char(buffer)) 
        {
            char **tmp = realloc(lines, (count + 1)*sizeof(char*));
            if (!tmp)
            {
                perror("realloc");
                exit(1);
            }

            lines = tmp;
            lines[count] = malloc(strlen(buffer) + 1);
            if (!lines[count])
            {
                perror("malloc");
                exit(1);
            }
            strcpy(lines[count], buffer);
            counnt++;
        }
    }

    fclose(fin);
    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


#define MAX_LINE 4096

int has_any_char(const char* s)
{
    while (*s)
    {
        if (isgraph((char)*s))
        {
            return 1;
        }
        s++;
    }

    return 0;
}

int cmp_plain(const void *a, const void *b) {
    return strcmp(*(const char**)a, *(const char**)b);
}
int cmp_lex(const void *a, const void *b) {
    return strcasecmp(*(const char**)a, *(const char**)b);
}
int cmp_rplain(const void *a, const void *b) {
    return strcmp(*(const char**)b, *(const char**)a);
}
int cmp_rlex(const void *a, const void *b) {
    return strcasecmp(*(const char**)b, *(const char**)a);
}

int main(int argc, char **argv)
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
            count++;
        }
    }

    fclose(fin);

    if (strcmp(mode, "plain") == 0)
    {
        qsort(lines, count, sizeof(char*), cmp_plain);
    }
    else if (strcmp(mode, "lex") == 0)
    {
        qsort(lines, count, sizeof(char*), cmp_lex);
    }
    else if (strcmp(mode, "rplain") == 0)
    {
        qsort(lines, count, sizeof(char*), cmp_rplain);
    }
    else if (strcmp(mode, "rlex") == 0)
    {
        qsort(lines, count, sizeof(char*), cmp_rlex);
    }
    else 
    {
        perror("incorrect mode");
        exit(1);
    }

    FILE *fout = fopen(outfile, "w");
    if (!fout) 
    {
        perror("fopen output");
        exit(1);
    }
    for (int i = 0; i < count; i++) 
    {
        fprintf(fout, "%s\n", lines[i]);
        free(lines[i]);
    }
    fclose(fout);
    free(lines);

    return 0;
}
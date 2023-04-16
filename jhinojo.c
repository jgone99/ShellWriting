#include<stdlib.h>
#include<unistd.h>
#include<stdio.h>
#include<string.h>
#include<ctype.h>
#include<errno.h>

char *trim(char *line) {
    if(line == NULL)
    {
        return line;
    }
    int len = strlen(line);
    if (len > 0) {
        char *final = &line[len-1];
        while ((final >= line) && isspace(*final))
            final--;
        *(final+1) = '\0';
    }

    while (isspace(*line))
        line++;

    return line;
}

int main(int argc, char *args[])
{
    char *line = NULL;
    size_t size = 0; 
    char *prompt = NULL;

    if(argc == 1)
    {
        prompt = "jsh: ";
    }
    else if(argc == 2)
    {
        if(strcmp(args[1], "-") == 0)
        {
            prompt = "";
        }
        else
        {
            prompt = args[1];
        }
    }
    else
    {
        printf("illegal number of arguments\n");
        _exit(1);
    }

    while(1)
    {
        printf("%s", prompt);
        if(getline(&line, &size, stdin) < 0)
        {
            free(line);
            line = NULL;
            perror("getline error");
            _exit(1);
        }
        char *trimline = trim(line);
        if(strcmp(trimline, "exit") == 0)
        {
            break;
        }
        
        char *cmd = strtok(line, " \t");

        while(cmd != NULL)
        {
            cmd = trim(cmd);
            printf("%s\n", cmd);
            cmd = strtok(NULL, " \t");
        }
    }

    free(line);
    line = NULL;

    return 0;
}




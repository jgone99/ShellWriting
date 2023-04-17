#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

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

typedef struct args_info
{
    char *delim;
    char **args;
    int arg_count;
    int background_run;
} args_info;

void parse_args(args_info **arg_info, char *line)
{
    int arg_count = 0;
    char *ptr = line;
    int traversing_delim = 0;

    // count the number of arguments in line ptr
    while(*ptr)
    {
        int in_delim = 0;
        for(int i = 0; i < strlen((*arg_info)->delim); i++)
        {
            in_delim += *ptr == (*arg_info)->delim[i];
        }
        arg_count += !in_delim && traversing_delim;
        traversing_delim = in_delim;
        ptr++;
    }

    (*arg_info)->arg_count = ++arg_count;

    (*arg_info)->args = malloc((arg_count + 1) * sizeof(char*));
    (*arg_info)->args[arg_count] = NULL;

    char *next_arg = strtok(line, (*arg_info)->delim);

    int i = 0;
    while(next_arg)
    {
        (*arg_info)->args[i++] = next_arg;
        next_arg = strtok(NULL, (*arg_info)->delim);
    }
    
    if((*arg_info)->background_run = (*arg_info)->args[arg_count - 1][strlen((*arg_info)->args[arg_count - 1]) - 1] == '&')
    {
        (*arg_info)->args[arg_count - 1][strlen((*arg_info)->args[arg_count - 1]) - 1] = '\0';
    }
}

int main(int argc, char *args[])
{
    char *line = NULL;
    size_t size = 0; 
    char *prompt = NULL;
    args_info *arg_info = malloc(sizeof(args_info));
    arg_info->delim = " \t";

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

        if(trimline[0] == '\0')
        {
            continue;
        }

        if(strcmp(trimline, "exit") == 0)
        {
            break;
        }

        parse_args(&arg_info, trimline);

        pid_t fork_pid = fork();

        if (fork_pid < 0) {
            perror("forking");
        } else if (fork_pid == 0) {
            /* child */
            execvp(arg_info->args[0], arg_info->args);
            perror(arg_info->args[0]);

            free(arg_info->args);
            free(line);
            arg_info->args = NULL;
            line = NULL;
            free(arg_info);
            arg_info = NULL;

            _exit(1);
        } else {
            /* parent */
            int status;
            if(!arg_info->background_run)
            {
                waitpid(fork_pid, &status, 0);
            }
        }
        
        free(arg_info->args);
        arg_info->args = NULL;
    }

    free(arg_info);
    arg_info = NULL;
    free(line);
    line = NULL;

    return 0;
}




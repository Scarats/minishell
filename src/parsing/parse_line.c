#include "./minishell.h"

bool ft_hasspace(char *str)
{
    int i;

    i = 0;
    while(str[i])
    {
        if(str[i] == ' ')
            return(true);
        i++;
    }
    return(false);
}

bool ft_haspipe(char *str)
{
    int i;

    i = 0;
    while(str[i])
    {
        if(str[i] == '|')
            return(true);
        i++;
    }
    return(false);
}

bool ft_hasredirect(char *str)
{
    int i;

    i = 0;
    while(str[i])
    {
        if(str[i] == '<' || str[i] == '>' || str[i] == '<<' || str[i] == '>>')
            return(true);
        i++;
    }
    return(false);
}

bool ft_hasoption(char *str)
{
    int i;

    i = 0;
    while(str[i])
    {
        if(str[i] == '-')
            return(true);
        i++;
    }
    return(false);
}
#include "./minishell.h"
#include <stdbool.h>

#define bool int
#define true 1
#define false 0


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
        if(str[i] == '|')
            return(true);
        i++;
    }
    return(false);
}
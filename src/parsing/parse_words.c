bool ft_hasspace(char *str)
{
    int i;

    i = 0;
    while(str[i])
    {
        if(str[i] == ' ')
            return(true)
        i++;
    }
    return(false);
}

void ft_haspipe(char *str)
{
    int i;

    i = 0;
    while(str[i])
    {
        if(str[i] == '|')
            return(true)
        i++;
    }
    return(false);
}

int i;

    i = 0;
    while(str[i])
    {
        if(str[i] == 32)
            return(true)
        i++;
    }
    return(false);

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_env_var.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcardair <tcardair@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 15:10:00 by tcardair          #+#    #+#             */
/*   Updated: 2025/10/22 15:12:31 by tcardair         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// Return 1 if there is NO space immediately before the token start.
// - Skip quote delimiters.
// - If allow_skip_dollar=1 and the immediate previous char is '$',
// skip that '$' too.
// - Otherwise, never merge across a '$'.
int	no_space_before_token_start(const char *s, int start, int allow_skip_dollar)
{
	int	i;

	if (start <= 0)
		return (0);
	i = start - 1;
	// Skip immediate quote delimiters
	while (i >= 0 && (s[i] == '\'' || s[i] == '"'))
		i--;
	if (i < 0)
		return (0);
	// Handle a preceding '$'
	if (s[i] == '$')
	{
		if (!allow_skip_dollar)
			return (0);
		// Skip this '$' and any quote delimiters before it
		i--;
		while (i >= 0 && (s[i] == '\'' || s[i] == '"'))
			i--;
		if (i < 0)
			return (0);
	}
	// Merge only when there is no space separating
	return (s[i] != ' ');
}

// Merge current word-like token into the previous one if adjacent (no space).
// just_removed_dollar indicates we removed a preceding '$' for this token.
int	merge_with_prev_if_adjacent(t_main_data *data, t_token *tok, int start,
		int just_removed_dollar)
{
	t_token	*prev;
	size_t	a;
	size_t	b;
	char	*joined;

	if (!tok || !(prev = tok->prev_token))
		return (0);
	if (!is_word_token(prev->type) || !is_word_token(tok->type))
		return (0);
	if (!no_space_before_token_start(data->tok->input, start,
			just_removed_dollar))
		return (0);
	a = ft_strlen(prev->word);
	b = ft_strlen(tok->word);
	joined = my_malloc(&data->root->list_of_list, &data->malloc_tok, a + b + 1);
	ft_memcpy(joined, prev->word, a);
	ft_memcpy(joined + a, tok->word, b);
	joined[a + b] = '\0';
	prev->word = joined;
	if (prev->type == TOKEN_TEXT)
		prev->type = get_word_type(prev);
	remove_token(&data->tok->token_list, tok);
	data->tok->last_token = prev;
	while (data->tok->last_token && data->tok->last_token->next_token)
		data->tok->last_token = data->tok->last_token->next_token;
	return (1);
}

int	set_token_word_with_suffix(t_main_data *data, t_token *tok, char *expanded,
		size_t var_len)
{
	char	*rest;
	size_t	vlen;
	size_t	rlen;
	char	*joined;

	if (!tok || !tok->slice)
		return (1);
	rest = tok->slice + var_len;
	if (expanded)
		vlen = ft_strlen(expanded);
	else
		vlen = 0;
	rlen = ft_strlen(rest);
	joined = my_malloc(&data->root->list_of_list, &data->malloc_tok, vlen + rlen
			+ 1);
	if (vlen)
		ft_memcpy(joined, expanded, vlen);
	if (rlen)
		ft_memcpy(joined + vlen, rest, rlen);
	joined[vlen + rlen] = '\0';
	tok->word = joined;
	return (0);
}

int	expand_var(t_main_data *data, t_token *tok, t_token *removed, int *removed_dollar)
{
    char	saved;
    char	*expanded;
    size_t	var_len;

    expanded = NULL;
    var_len = 0;
    if (!tok || !tok->slice)
        return (1);
    if (tok->slice[0] == '?')
    {
        var_len = 1;
        expanded = ft_itoa(data->root->last_exit_status);
        if (expanded)
            my_addtolist(&data->malloc_tok, expanded);
    }
    else if (ft_isalpha((unsigned char)tok->slice[0]) || tok->slice[0] == '_')
    {
        var_len = 1;
        while (tok->slice[var_len]
            && (ft_isalnum((unsigned char)tok->slice[var_len])
                || tok->slice[var_len] == '_'))
            var_len++;
        saved = tok->slice[var_len];
        tok->slice[var_len] = '\0';
        expanded = get_env_var(data->root->env, tok->slice);
        tok->slice[var_len] = saved;
    }
    if (tok->prev_token && tok->prev_token->type == TOKEN_DOLLAR)
    {
        remove_token(&data->tok->token_list, removed);
        if (data->tok->last_token == removed)
            data->tok->last_token = tok;
        *removed_dollar = 1;
    }
    tok->type = get_word_type(tok);
	if (set_token_word_with_suffix(data, tok, expanded, var_len))
        return (1);
    return (0);
}

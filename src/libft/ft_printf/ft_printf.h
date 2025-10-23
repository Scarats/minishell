/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aadeikal <aadeikal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 14:54:38 by tcardair          #+#    #+#             */
/*   Updated: 2025/10/06 15:11:28 by aadeikal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include "../libft.h"
# include <stdarg.h>
# include <stdlib.h>
# include <unistd.h>
# define HEXA "0123456789abcdef"
# define HEXA_MAJ "0123456789ABCDEF"

# include <stdint.h>

typedef struct s_buffer
{
	char			*buffer;
	size_t			*pos;
	size_t			max_size;
}					t_buffer;

int					ft_printf(const char *str, ...);
int					type_c(char c);
int					type_s(char *str);
int					type_i(int nb);
int					type_p(void *p);
void				swap(char *ptr, int i);
unsigned long long	to_hexa(unsigned long long addr);
unsigned int		to_hexa_ptr(unsigned int nb, char *ptr);
int					type_xx(unsigned int nb, char c);
int					type_u(unsigned int u);
int					loop(char *str, va_list list);

int					ft_snprintf(char *buffer, size_t max_size,
						const char *format, ...);

int					handle_percent(char *str, size_t *i, va_list list,
						t_buffer *s_buffer);
void				null_terminate_buffer(t_buffer *s_buffer);
void				write_char_to_buffer(char c, t_buffer *s_buffer);
int					buffer_loop(char *str, va_list list, char *buffer,
						size_t max_size);
int					buffer_router(char c, va_list list, t_buffer *s_buffer);
int					buffer_type_c(char c, t_buffer *s_buffer);
int					buffer_type_i(int nb, t_buffer *s_buffer);
int					buffer_type_p(void *ptr, t_buffer *s_buffer);
int					buffer_type_s(char *str, t_buffer *s_buffer);
int					buffer_type_u(unsigned int nb, t_buffer *s_buffer);
int					buffer_type_xx(unsigned int nb, char c, t_buffer *s_buffer);

#endif

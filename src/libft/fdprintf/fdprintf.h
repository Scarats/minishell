/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdprintf.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aadeikal <aadeikal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 14:54:38 by tcardair          #+#    #+#             */
/*   Updated: 2025/10/30 18:38:39 by aadeikal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FDRINTF_H
# define FDPRINTF_H

# include "../libft.h"
# include <stdarg.h>
# include <stdlib.h>
# include <unistd.h>
# define HEXA "0123456789abcdef"
# define HEXA_MAJ "0123456789ABCDEF"

int					fdprintf(int fd, const char *str, ...);
int					fdtype_c(char c, int fd);
int					fdtype_s(char *str, int fd);
int					fdtype_i(int nb, int fd);
int					fdtype_p(void *p, int fd);
unsigned long long	fdto_hexa(unsigned long long addr, int fd);
int					fdtype_xx(unsigned int nb, char c, int fd);
int					fdtype_u(unsigned int u, int fd);
int					fdloop(char *str, va_list list, int fd);

#endif

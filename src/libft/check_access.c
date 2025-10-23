/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_access.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcardair <tcardair@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 17:02:28 by tcardair          #+#    #+#             */
/*   Updated: 2025/09/03 15:28:27 by tcardair         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// Check file permissions:
// give file path.
// to check read / write / execute:
// 		set to 1.
//		else, put 0.
// silent:
//		1 = will not print error messages.
//		0 = will print error messages.
// the output is 0 if everything worked fine.
// 1 for error.
int	check_access(char *path, int r, int w, int x)
{
	int	error;

	error = 0;
	if (r > 0 && access(path, R_OK) == -1)
		error = 1;
	if (w > 0 && access(path, W_OK) == -1)
		error = 1;
	if (x > 0 && access(path, X_OK) == -1)
		error = 1;
	return (error);
}
	// if (error && !silent)
	// {
	// 	if (r > 0 && access(path, R_OK) == -1)
	// 		write(2, "cannot read\n", 12);
	// 	if (w > 0 && access(path, W_OK) == -1)
	// 		write(2, "cannot write\n", 13);
	// 	if (x > 0 && access(path, X_OK) == -1)
	// 		write(2, "cannot execute\n", 15);
	// 	write(2, path, ft_strlen(path));
	// 	write(2, " ", 1);
	// 	perror("access");
	// }
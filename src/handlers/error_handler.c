#include "../../minishell.h"

// Check if an error, print the error message and error code.
// Reset the error flags and str_error.
int	check_error(t_main_data *data)
{
	if (data->error)
	{

		return (1);
	}
	else
		return (0);
}

// Set the error flag with error code in t_main_data
// Store the str_error in the t_main_data
// str_error is the contextual part of the error message :
// cewogowjkn
// bash: cewogowjkn: command not found
int	set_error(t_main_data *data, int error_code, char str_error)
{

}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgonga <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 10:40:19 by pgomes            #+#    #+#             */
/*   Updated: 2024/11/28 22:33:37 by sgonga           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_exec_builtin(t_data *data, char **expander)
{
	(void)data;
	if (ft_strcmp(expander[0], "echo") == 0)
		return (ft_echo(expander));
	if (ft_strcmp(expander[0], "cd") == 0)
		return (ft_cd(data, expander));
	if (ft_strcmp(expander[0], "env") == 0)
		return (ft_env(data));
	if (ft_strcmp(expander[0], "pwd") == 0)
		return (ft_pwd());
	if (ft_strcmp(expander[0], "export") == 0)
		return (ft_export(data, expander));
	if (ft_strcmp(expander[0], "unset") == 0)
		return (ft_unset(data, expander));
	ft_exit(data, expander);
	return (ENO_GENERAL);
}

bool	ft_is_builtin(char *arg)
{
	if (!arg)
		return (false);
	if (!ft_strcmp(arg, "echo") || !ft_strcmp(arg, "cd") || !ft_strcmp(arg,
			"exit") || !ft_strcmp(arg, "pwd") || !ft_strcmp(arg, "export")
		|| !ft_strcmp(arg, "unset") || !ft_strcmp(arg, "env"))
		return (true);
	return (false);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgonga <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 10:37:18 by pgomes            #+#    #+#             */
/*   Updated: 2024/11/28 22:19:46 by sgonga           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	ft_change_pwd(t_data *data)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
		return (1);
	return (ft_update_envlst(data, "PWD", cwd, false),free(cwd), 0);
}

static int	ft_cd_home(t_data *data)
{
	char	*home;

	ft_update_envlst(data, "OLDPWD", ft_get_envlst_val(data, "PWD"), false);
	home = ft_get_envlst_val(data, "HOME");
	if (!home)
		return (ft_putstr_fd("minishell: cd: HOME not set\n", 2), 1);
	if (chdir(home) == ENO_SUCCESS)
		return (ft_update_envlst(data, "PWD", home, false), 0);
	return (1);
}

static int	ft_cd_err_msg(char *err_msg)
{
	ft_putstr_fd("minishell: cd: `", 2);
	ft_putstr_fd(err_msg, 2);
	ft_putstr_fd("': No such file or directory\n", 2);
	return (1);
}

int	ft_cd(t_data *data, char **args)
{
	if (args[1] == NULL)
		return (ft_cd_home(data));
	if (chdir(args[1]) != 0)
		return (ft_cd_err_msg(args[1]));
	ft_update_envlst(data, "OLDPWD", ft_get_envlst_val(data, "PWD"), false);
	return (ft_change_pwd(data));
}

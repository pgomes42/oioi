/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgonga4242 <sgonga4242@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 10:39:33 by pgomes            #+#    #+#             */
/*   Updated: 2024/11/27 16:40:18 by sgonga4242       ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "minishell.h"

int	ft_pwd(void)
{
	char	*cwd;
	cwd = NULL;
	cwd = getcwd(cwd, 0);
	if (!cwd)
		return (1);
	ft_putstr_fd(cwd, 1);
	ft_putstr_fd("\n", 1);
	free(cwd);
	return (0);
}

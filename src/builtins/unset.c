/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgonga <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 10:39:41 by pgomes            #+#    #+#             */
/*   Updated: 2024/11/28 12:10:40 by sgonga           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_unset_helper(t_data *data, char *key)
{
	t_env	*current;
	t_env	*prev;

	prev = NULL;
	current = data->envlst;
	while (current)
	{
		if (!ft_strcmp(key, current->key))
		{
			if (prev)
				prev->next = current->next;
			else
				data->envlst = current->next;
			free(current);
			return ;
		}
		prev = current;
		current = current->next;
	}
}

int	ft_unset(t_data *data, char **expand)
{
	bool	err;
	int		i;

	i = -1;
	if (!expand[1])
		return (0);
	err = false;
	while (expand[++i])
	{
		if (!ft_check_key(expand[i]))
		{
			ft_putstr_fd("minishell: unset: `", 2);
			ft_putstr_fd(expand[i], 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			err = true;
		}
		else
			ft_unset_helper(data,
				ft_garbage_collector(ft_extract_key(expand[i]), false));
	}
	return (err);
}

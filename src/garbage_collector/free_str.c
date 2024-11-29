/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_str.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgomes <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 10:15:24 by pgomes            #+#    #+#             */
/*   Updated: 2024/11/29 10:26:23 by pgomes           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_free_char2(char **tofree)
{
	size_t	i;

	if (!tofree)
		return ;
	i = 0;
	while (tofree[i])
		free(tofree[i++]);
	free(tofree);
}

void	ft_free_char3(char ***tofree)
{
	size_t	i;

	if (!tofree)
		return ;
	i = 0;
	while (tofree[i])
		ft_free_char2(tofree[i++]);
	free(tofree);
}

void	ft_clear_envlst(t_data *data)
{
	t_env	*envlst;
	t_env	*envlst_tofree;

	envlst = data->envlst;
	while (envlst)
	{
		envlst_tofree = envlst;
		envlst = envlst->next;
		free(envlst_tofree);
	}
	data->envlst = NULL;
}

void	ft_clean_ms(t_data *data)
{
	ft_garbage_collector(NULL, true);
	rl_clear_history();
	//ft_clean_tokens(data->tokens);
	ft_clear_envlst(data);
	//free(data->line);
	tcsetattr(STDIN_FILENO, TCSANOW, &data->original_term);
	
}

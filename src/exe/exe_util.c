/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_util.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgomes <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/24 15:42:01 by pgomes            #+#    #+#             */
/*   Updated: 2024/11/29 11:57:08 by pgomes           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	h_c_proces(t_data *data, t_token *tokens[2], int pipes[2][2],
		int i)
{
	t_token	*current;
	t_token	*cmd_tokens;
	int status;

	cmd_tokens = tokens[0];
	current = tokens[1];
	
	if (i > 0)
		dup2(pipes[(i - 1) % 2][0], STDIN_FILENO);
	if (current != NULL && current->next != NULL)
		dup2(pipes[i % 2][1], STDOUT_FILENO);
	close(pipes[0][0]);
	close(pipes[0][1]);
	close(pipes[1][0]);
	close(pipes[1][1]);
	status = ft_exec_cmd(data, cmd_tokens, true);
	
	exit(status);
}

int	ft_add_tokens_collect(t_token **cmd_tokens, t_token **current)
{
	t_token	*new_token;

	while (*current != NULL && (*current)->type != T_PIPE)
	{
		new_token = ft_new_token((*current)->value, (*current)->type);
		if (new_token == NULL)
			return 0;
		ft_token_list_add_back(cmd_tokens, new_token);
		*current = (*current)->next;
	}
	
		
	return 1;
}

int	ft_status(int status)
{
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (WEXITSTATUS(status));
}

bool	ft_is_pepiline(t_token *tokens)
{
	t_token	*current;

	current = tokens;
	while (current != NULL)
	{
		if (current->type == T_PIPE)
			return (true);
		current = current->next;
	}
	return (false);
}

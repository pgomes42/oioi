/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exec_simple_cmd.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgonga <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 10:45:38 by pgomes            #+#    #+#             */
/*   Updated: 2024/11/29 06:08:53 by sgonga           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_check_redirection(t_data *data)
{
	t_token	*tmp;
	int		tmp_status;

	tmp = data->tokens;
	while (tmp)
	{
		if (tmp->type == 2 && ft_out(tmp, &tmp_status) != ENO_SUCCESS)
			return (tmp_status);
		else if (tmp->type == 1 && ft_in(tmp, &tmp_status) != ENO_SUCCESS)
			return (tmp_status);
		else if (tmp->type == 4 && ft_append(tmp, &tmp_status) != ENO_SUCCESS)
			return (tmp_status);
		else if (tmp->type == 3)
			(dup2(data->here_doc, 0), close(data->here_doc));
		tmp = tmp->next;
	}
	return (ENO_SUCCESS);
}

int	ft_expand_args_2(char **argv, char **expanded, int *count)
{
	if (expanded == NULL)
	{
		char *dup = ft_strdup("");
		argv[++*count] = dup;
		free(dup);
		return (0);
	}
	argv[++*count] = ft_strdup(expanded[0]);
	return (1);
}

char	**ft_expand_args(t_data *data, t_token *current_token)
{
	t_token	*current;
	int		count;
	char	**argv;
	char	**expanded;

	count = 0;
	current = current_token;
	while (current != NULL && current->type == T_IDENTIFIER && (++count))
		current = current->next;
	argv = (char **)malloc(sizeof(char *) * (count + 1));
	current = current_token;
	count = -1;
	while (current != NULL && current->type == T_IDENTIFIER)
	{
		expanded = ft_expand(data, current->value);
		if (!ft_expand_args_2(argv, expanded, &count))
			break ;
		ft_free_char2(expanded);
		if (argv[count] == NULL)
			(ft_free_char2(argv), ft_clean_ms(data), exit(ENO_GENERAL));
		current = current->next;
	}
	argv[++count] = NULL;
	return (argv);
}

static int	ft_exec_child(t_data *data, char **expand, int tmp_status)
{
	t_path	path_status;
	int		fork_pid;
	
	data->signint_child = true;
	signal(SIGINT, handle_sigint_heredoc);
	fork_pid = fork();
	if (!fork_pid)
	{
		tmp_status = ft_check_redirection(data);
		if (tmp_status != ENO_SUCCESS)
			(ft_clean_ms(data), ft_free_char2(expand), exit(ENO_GENERAL));
		path_status = ft_get_path(data, (expand[0]));
		if (path_status.err.no != ENO_SUCCESS)
		{
			tmp_status = ft_err_msg(path_status.err);
			(ft_clean_ms(data), ft_free_char2(expand), exit(tmp_status));
		}
		if (execve(path_status.path, expand, data->environ) == -1)
			(ft_clean_ms(data), ft_free_char2(expand), exit(1));
	}
	data->signint_child = (waitpid(fork_pid, &tmp_status, 0), false);
	if (WIFSIGNALED(tmp_status))
		return (128 + WTERMSIG(tmp_status));
	return (WEXITSTATUS(tmp_status));
}

int	ft_exec_simple_cmd(t_data *data, char **expand, bool piped)
{
	int	tmp_status;

	tmp_status = 0;
	if (!expand[0])
	{
		tmp_status = ft_check_redirection(data);
		if (!piped)
			(dup2(data->stdin, 0), dup2(data->stdout, 1));
		return (tmp_status && ENO_GENERAL);
	}
	else if (ft_is_builtin(expand[0]))
	{
		tmp_status = ft_check_redirection(data);
		if (tmp_status != ENO_SUCCESS && !piped)
			return (dup2(data->stdin, 0), dup2(data->stdout, 1), ENO_GENERAL);
		else if (tmp_status != ENO_SUCCESS)
			return (ENO_GENERAL);
		tmp_status = ft_exec_builtin(data, expand);
		if (!piped)
			(dup2(data->stdin, 0), dup2(data->stdout, 1));
		return (tmp_status);
	}
	else
		return (ft_exec_child(data, expand, tmp_status));
}

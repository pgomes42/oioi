/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgomes <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 10:40:52 by pgomes            #+#    #+#             */
/*   Updated: 2024/11/29 12:23:15 by pgomes           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_parent_process(int pipes[2][2], int i, t_token *current)
{
	if (i > 0)
		close(pipes[(i - 1) % 2][0]);
	if (current != NULL && current->next != NULL)
		close(pipes[i % 2][1]);
}

static int	ft_pipe_or_null(t_token **current, int pipes[2][2], int i,
		bool task)
{
	if (task)
	{
		if (*current != NULL && (*current)->next != NULL)
		{
			if (pipe(pipes[i % 2]) == -1)
			{
				perror("error pipe\n");
				return (1);
			}
		}
	}
	else
	{
		if (*current == NULL || (*current)->next == NULL)
			{printf("NULL\n"); *current = NULL;}
		else
			{printf("NAO NULL\n");*current = (*current)->next;}
	}
	return (0);
}

void	ft_wait_child(int *i, int *status)
{
     int w; // Variável para armazenar o retorno de wait

    *status = 0; // Inicializa *status
    while (*i > 0) // Condição de parada corrigida para evitar underflow
    {
        w = wait(status); // Armazena o resultado da chamada wait()
        if (w == -1) 
        {
            perror("wait");
            exit(1);
        }
        (*i)--; // Decrementa i após wait ser concluído
    }
}
void	ft_clean_tokens1(t_token *data)
{
	t_token	*tokens;
	t_token	*tmp;
	

//	printf("Limpando %d\n");
	tokens = data;
	while (tokens)
	{
		printf("Limpando  2\n");
		tmp = tokens;
		tokens = tokens->next;
		free(tmp);
	}
	//printf("Limpou %s \n", data->value);
}

int	ft_exec_pipeline(t_data *data, t_token *curren, int status, int i)
{
	int pipes[2][2] = {{-1, -1}, {-1, -1}}; // Inicializa todos os elementos como -1
	int		pid;
	t_token	*c_token;
    t_token *current = curren;
	c_token = NULL;
    status = 0;
    
    i = 0;
	if (current == NULL)
		return (printf("ENO S\n"), ENO_SUCCESS);
        
	while (current != NULL)
	{
        printf("current->value: %s\n", current->value);
		if(!ft_add_tokens_collect(&c_token, &current))
           {printf("cai0"); return (1);}
		if (ft_pipe_or_null(&current, pipes, i, true))
			{printf("cai1"); return (1);}
		data->signint_child = true;
		pid = fork();
		if (pid == 0)
        {
			h_c_proces(data, (t_token *[]){c_token, current}, pipes, i);
        }
		else
		{
			handle_parent_process(pipes, i++, current);
            ft_clean_tokens(c_token);
			c_token = NULL;
           
            
		}
        ft_pipe_or_null(&current, pipes, i, false);
        ft_clean_tokens(c_token);

    }
	data->signint_child = false;
	return (ft_wait_child(&i, &status), ft_status(status));
}

/*
static void ft_run_command(t_data *data, t_token *cmd_tokens, int pipes[2][2], int i, bool has_next_command);
static void close_pipes(int pipes[2][2]);
static t_token *ft_find_next_command(t_token *current);
//static t_token *ft_copy_token_list(t_token *begin, t_token *end);

int ft_exec_pipeline(t_data *data, t_token *tokens) {
    int pipes[2][2] = {{-1, -1}, {-1, -1}}; // Inicializa pipes
    int pid;
    int i = 0;
    int status = 0;
    t_token *current = tokens;
    t_token *cmd_tokens = NULL;

    if (current == NULL) return ENO_SUCCESS;

    while (current != NULL) {
        // Coleta tokens do comando atual até encontrar T_PIPE ou o final da lista
        t_token *next_cmd = ft_find_next_command(current);
        //printf("next_cmd: %s\n", next_cmd->value);
        //cmd_tokens = ft_copy_token_list(current, next_cmd);
		ft_add_tokens_collect(&cmd_tokens, &current);
        //printf("cmd_tokens: %s\n", cmd_tokens->value);
        if (next_cmd != NULL) 
        {
            if (pipe(pipes[i % 2]) == -1)
            {
                perror("pipe");
                exit(1);
            }
        }

        pid = fork();
        if (pid == 0) {
            // Processo filho
            printf("cmd_: %s\n", cmd_tokens->value);
            ft_run_command(data, cmd_tokens, pipes, i, next_cmd != NULL);
            exit(1); 
        } else {
            // Processo pai
            if (i > 0) 
            close(pipes[(i-1) % 2][0]);
            if (next_cmd != NULL) 
            close(pipes[i % 2][1]);
            //ft_free_token_list(cmd_tokens);
            i++;
        }
        current = next_cmd;
    }
    
    while (i-- > 0) wait(&status);
    data->signint_child = false;
    //ft_clear_token_list(&cmd_tokens);
    return ft_get_exit_status(status;);
}


// Função para executar um comando em um processo filho, considerando pipes
static void ft_run_command(t_data *data, t_token *cmd_tokens, int pipes[2][2], int i, bool has_next_command)
{
    if (i > 0)
        dup2(pipes[(i - 1) % 2][0], STDIN_FILENO);
    if (has_next_command)
        dup2(pipes[i % 2][1], STDOUT_FILENO);
    close_pipes(pipes); 
    printf("0 - cmd_tokens: %s\n", cmd_tokens->value);
    printf("1 - cmd_tokens: %s\n", cmd_tokens->next->value);
    char **expander = ft_expand_args(data, cmd_tokens);
    printf("0 - expanders: %s\n", expander[0]);
    printf("1 - expanders: %s\n", expander[1]);
    ft_exec_simple_cmd(data, expander, true); // Execute o comando
    //exit(0);
}

// Função para fechar todos os descritores de arquivos de pipe
static void close_pipes(int pipes[2][2])
{
    close(pipes[0][0]);
    close(pipes[0][1]);
    close(pipes[1][0]);
    close(pipes[1][1]);
}


// Função para encontrar o próximo comando na lista de tokens
static t_token *ft_find_next_command(t_token *current)
{
    if (current == NULL) return NULL;
    while (current->next != NULL && current->next->type != T_PIPE)
        current = current->next;
    return current->next;
}
*/

// Função para copiar uma sublista de tokens até um determinado limite
/*static t_token *ft_copy_token_list(t_token *begin, t_token *end)
{
    t_token *new_list = NULL;
    t_token *current = begin;
    t_token *last = NULL;
    while (current != end) 
    {
        t_token *new_token = ft_new_token(current->value, current->type);
        if (new_token == NULL) 
            return NULL;
        if (new_list == NULL) 
            new_list = new_token;
        else
            last->next = new_token;
        last = new_token;
        current = current->next;
    }
    return new_list;
}*/

int	ft_exec_cmd(t_data *data, t_token *current, bool piped)
{
    char **expand = NULL;
    int status;
   
	if (!current)
		return (130);
	if (ft_is_pepiline(current) && !piped)
    {
        printf("pipe\n");
		(ft_exec_pipeline(data, current, 0 ,0));
    }
	else
	{
        expand = ft_expand_args(data, current);
        if (expand == NULL)
        {
            printf("NULL expand\n");
            return 1;
        }
        status = ft_exec_simple_cmd(data, expand, piped);
        ft_free_char2(expand);   
        
       	return (status);
	}
	return (ENO_GENERAL);
}

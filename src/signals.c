/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgonga4242 <sgonga4242@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 16:39:19 by pgomes            #+#    #+#             */
/*   Updated: 2024/11/26 14:42:22 by sgonga4242       ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "minishell.h"

static void	suppress_output(void)
{
	struct termios	termios_p;

	if (tcgetattr(0, &termios_p) != 0)
		perror("Minishell: tcgetattr");
	termios_p.c_lflag &= ~ECHOCTL;
	if (tcsetattr(0, 0, &termios_p) != 0)
		perror("Minishell: tcsetattr");
}

static void	handle_sigint(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	handle_sigquit(int sig)
{
	(void)sig;
	rl_redisplay();
}
void	ft_sigquit_handler(int num)
{
	(void)num;
	ft_putstr_fd("Quit: 3\n", 1);
}

void	handle_sigint_heredoc(int sig)
{
	(void)sig;
	write(1, "\n", 1);
}

void	signals_init(t_data *data)
{
	suppress_output();
	if (data->signint_child)
	{
		data->signint_child = false;
		data->heredoc_sigint = true;
		write(1, "\n", 1);
	}
	else
	{
		signal(SIGINT, handle_sigint);
	}
	signal(SIGQUIT, SIG_IGN);
}

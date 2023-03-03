/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aybiouss <aybiouss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 15:14:26 by aybiouss          #+#    #+#             */
/*   Updated: 2023/03/03 11:47:06 by aybiouss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../mini_shell.h"


t_cmd	*last_cmd(t_cmd *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next)
		lst = lst->next;
	return (lst);
}

t_cmd	*new_cmd(char *content)
{
	t_cmd	*new;
	new = malloc(sizeof(t_cmd));
	if(!new)
		return (0);
	new->cmd = content;
	new->fd.in = 0;
	new->fd.out = 0;
	new->next = NULL;
	return (new);
}

void	cmd_add_back(t_cmd **lst, t_cmd *new)
{
	t_cmd	*tmp;

	if (*lst == NULL)
		*lst = new;
	else
	{
		tmp = last_cmd(*lst);
		tmp->next = new;
	}
}

t_redire	*redilast(t_redire *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next)
		lst = lst->next;
	return (lst);
}

void	redi_add_back(t_redire **lst, t_redire *new)
{
	t_redire	*tmp;
	if (*lst == NULL)
		*lst = new;
	else
	{
		tmp = redilast(*lst);
		tmp->next = new;
	}
}

char **full_cmds(t_cmd *cmd)
{
	char	**cmds;
	int i;

	i = 0;
	cmds = malloc(sizeof(char *) * (cmd_size(cmd) + 1));
	while(cmd)
	{
		cmds[i] = cmd->cmd;
		cmd = cmd->next;
		i++;
	}
	cmds[i] = 0;
	return (cmds);
}

int	cmd_size(t_cmd *cmds)
{
	int size;

	size = 0;
	while(cmds)
	{
		size++;
		cmds = cmds->next;
	}
	return (size);
}

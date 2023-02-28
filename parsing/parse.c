/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sben-ela <sben-ela@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/23 11:01:50 by sben-ela          #+#    #+#             */
/*   Updated: 2023/02/28 11:17:47 by sben-ela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../mini_shell.h"

int handle_pipes(char *line)
{
	char *str;

	if (count_char(line, '\"') == ft_strlen(line) || count_char(line, '\'') == ft_strlen(line))
		return (1);
	str = ft_strtrim(line, " ");
	if (str[0] == '|' || str[ft_strlen(str) - 1] == '|')
		return (1);
	return (0);
}

int parse_redir(char*	line, int	i)
{
	while(line[i])
	{
    	if (line[i] == '<')
		{
			i++;
			while (line[i] == ' ')
				i++;
			if (!line[i] || (line[i - 1] == ' ' && line[i] == '<')
				|| line[i] == '>'|| (line[i] == '<' && !line[i + 1]) 
				|| (line[i] == '<' && line[i + 1] == '<'))
				return (1);
		}
		if (line[i] == '>')
		{
			i++;
			while (line[i] == ' ')
				i++;
			if ((line[i - 1] == ' ' && line[i] == '>') || line[i] == '<'
				|| !line[i] || (line[i] == '>' && !line[i + 1]) 
				|| (line[i] == '>' && line[i + 1] == '>'))
				return (1);
		}
		i++;
	}
	return (0);
}

int	parse_syntax(char *line, char c)
{
	static int i;

	if(handle_pipes(line) || count_single_couts(line) || count_double_couts(line))
		return (1);
	// hanldle if the line par example == "<"
	line = ft_strtrim(line, "\"");
	////hanldle if the line par example == '<'
	line = ft_strtrim(line, "\'");
	if (parse_redir(line, 0))
		return (1);
	while (line[i])
	{
		if(line[i] == '\'' || line[i] == '\"')
    	{
    	    c = line[i++];
    	    while(line[i] && line[i++] != c);
    	}
		if(line[i] == '|')
		{
			while(line[++i] == ' ');
			if(!line[i] || line[i] == '|')
				return (1);
		}
		i++;
	}
	return(0);
}

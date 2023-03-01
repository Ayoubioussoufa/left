#include "mini_shell.h"

void	print_data(t_shell *shell)
{
	int i;

	while(shell)
	{
		i = 0;
		printf("index : %d      pipe : %d\n", shell->index, shell->pipe);
		while(shell->cmds[i])
			printf("%s ", shell->cmds[i++]);
		printf("\n");
		while(shell->redir)
		{
			if (shell->redir->type == INFILE)
				printf("infile : %s\n", shell->redir->infile);
			if (shell->redir->type == OUTFILE)
				printf("OUTFILE : %s\n", shell->redir->outfile);
			if (shell->redir->type == APPEND)
				printf("APPEND : %s\n", shell->redir->outfile);
			if (shell->redir->type == DELIMITER)
				printf("DELIMITER : %s\n", shell->redir->delimiter);
			shell->redir = shell->redir->next;
		}
		shell = shell->next;
	}
}

int check_single_cout(char *str)
{
	int i;
	int j;
	char c;

	i = j = c = 0;
	while(str[i] && str[i] != '$')
		i++;
	if(i > 0)
		c = str[i - 1];
	while(str[i  + j] && str[i + j] != c)
		j++;
	if(c == '\'' && str[i + j] == '\'')
		return (0);
	return (1);
}

int check_edges(char *str)
{
	int	i;

	i = 0;
	while(*str && *str != '\'' && *str != '\"')
		str++;
	while(str[ft_strlen(str) - i] != '\'' && str[ft_strlen(str) - i] != '\"')
		i++;
	if (str[0] == '\"' && str[ft_strlen(str) - i] == '\"')
		return (1);
	if (str[0] == '\'' && str[ft_strlen(str) - i] == '\'')
		return (2);
	else
		return (0);
}

t_shell	*ft_lstnew(char *content, int index, char **env, int	pipe)
{
	t_shell	*new;
	char	**split;
	int		i;

	i = 0;
	new = malloc(sizeof(t_shell));
	if (!new)
		return (NULL);
	split = ft_split_v2(content, ' ');
	if(!split)
		return (0);
	new->cmd = 0;
	new->redir = 0;
	while(split[i])
	{
		if (check_edges(split[i]) == 1)
		{
			split[i] = get_value(split[i], env);
			split[i] = handle_param(split[i] , '\"');
		}
		else if (check_edges(split[i]) == 2)
			split[i] = handle_param(split[i] , '\'');
		else
			split[i] = get_value(split[i], env);
		if(!ft_strcmp(split[i], ">"))
			redi_add_back(&new->redir, new_redir(split[++i], OUTFILE));
		else if(!ft_strcmp(split[i], "<"))
			redi_add_back(&new->redir, new_redir(split[++i], INFILE));
		else if(!ft_strcmp(split[i], "<<"))
			redi_add_back(&new->redir, new_redir(split[++i], DELIMITER));
		else if(!ft_strcmp(split[i], ">>"))
			redi_add_back(&new->redir, new_redir(split[++i], APPEND));
		else
			cmd_add_back(&new->cmd, new_cmd(split[i]));
		i++;
	}
	new->pipe = pipe;
	new->index = index;
	new->type = 3;
	new->cmds = full_cmds(new->cmd);
	return (new);
}

void	ft_lstadd_back(t_shell **lst, t_shell *new)
{
	t_shell	*tmp;

	if (*lst == NULL)
	{
		new->previous = 0;
		*lst = new;
	}
	else
	{
		tmp = ft_lstlast(*lst);
		new->previous = ft_lstlast(*lst);
		tmp->next = new;
	}
}

t_shell *parse_line(char *line, char **env)
{
	t_shell	*shell;
	char	**args;
	int		i;
	int		pipe;

	i = -1;
	shell = 0;
	args = ft_split_v2(line, '|');
	if(!args)
		return (0);
	while(args[++i])
		args[i] = ft_strtrim(args[i], " ");
	i = -1;
	while(args[++i])
	{
		pipe = 0;
		if(args[i + 1])
			pipe = 1;
		ft_lstadd_back(&shell, ft_lstnew(args[i], i, env, pipe));
	}
	free(line);
	return(shell);
}

int	ft_checkspace(const char *str)
{
	int	i;

	i = 0;
	while (str[i] == ' ' || str[i] == '\t'
		|| str[i] == '\n' || str[i] == '\v'
		|| str[i] == '\f' || str[i] == '\r')
		i++;
	return (i);
}

void	sigint_handler(int sig) {
	if(sig == SIGINT)
	{
		write(1, "\n", 1);
		rl_on_new_line();
		// rl_replace_line("", 1);
		rl_redisplay();
	}
	else if(sig == SIGQUIT)
			return ;
}

void	mini_shell(char **env)
{
	char *read;
	t_shell *shell;

	while (1)
	{
		read = readline("\033[0;32mMinishell>> \033[0m");
		if (!read)
			exit(0);
		add_history(read);
		read += ft_checkspace(read);
		if (read[0] && !parse_syntax(read, 0))
		{
			read = parse_read(read);
			shell = parse_line(read, env);
			// ft_execute(shell, env);
			execute(shell, &env);
		}
		else if (read[0])
			printf("syntax error\n");
	}
}

int main(int ac, char **av, char **env)
{
	(void)av;
	if(ac != 1)
	{
		printf("invalid number of argument\n");
		return(0);
	}
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, sigint_handler);
	mini_shell(env);
	return 0;
}

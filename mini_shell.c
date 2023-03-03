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
				printf("INFILE : %s\n", shell->redir->infile);
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

int check_edges(char *str)
{
	int	i;

	i = 0;
	while (*str && *str != '\'' && *str != '\"')
		str++;
	while (str[i] && str[i] != '\'' && str[i] != '\"')
		i++;
	if (str[0] == '\"' && str[i]== '\"')
		return (1);
	if (str[0] == '\'' && str[i] == '\'')
		return (2);
	else
		return (0);
}

void	ft_getnew(char **split, char **env, int i, t_shell **new)
{
	while(split[i])
	{
		if (check_edges(split[i]) == 1 && (!count_char(split[i] , ' ')
			|| (split[i - 1] && !ft_strcmp(split[i - 1], "echo"))))
		{
			split[i] = get_value(split[i], env);
			split[i] = handle_param(split[i] , '\"');
		}
		else if(check_edges(split[i]) == 2 && (!count_char(split[i] , ' ')))
			split[i] = handle_param(split[i] , '\'');
		else
			split[i] = get_value(split[i], env);
		if(!ft_strcmp(split[i], ">"))
			redi_add_back(&(*new)->redir, new_redir(split[++i], OUTFILE));
		else if(!ft_strcmp(split[i], "<"))
			redi_add_back(&(*new)->redir, new_redir(split[++i], INFILE));
		else if(!ft_strcmp(split[i], "<<"))
			redi_add_back(&(*new)->redir, new_redir(split[++i], DELIMITER));
		else if(!ft_strcmp(split[i], ">>"))
			redi_add_back(&(*new)->redir, new_redir(split[++i], APPEND));
		else
			cmd_add_back(&(*new)->cmd, new_cmd(split[i]));
		i++;
	}
}

t_shell	*ft_lstnew(char *content, int index, char **env, int pipe)
{
	t_shell	*new;
	char	**split;
	new = malloc(sizeof(t_shell));
	if (!new)
		return (NULL);
	new->cmd = 0;
	new->redir = 0;
	split = ft_split_v2(content, ' ');
	if(!split)
		return (0);
	ft_getnew(split, env, 0, &new);
	new->type = 3;
	new->pipe = pipe;
	new->index = index;
	new->next = 0;
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

void	freedata(t_shell *data)
{
	while(data)
	{
		while (data->cmd)
		{
			free(data->cmd->cmd);
			data->cmd = data->cmd->next;
		}
		data = data->next;
	}
}

t_shell *parse_line(char *line, char **env)
{
	t_shell	*shell;
	t_shell	*new;
	char	**args;
	int		i;
	int		pipe;

	i = -1;
	shell = 0;
	args = ft_split_v2(line, '|');
	if(!args)
		return (0);
	while(args[++i])
	{
		args[i] = ft_strtrimfree(args[i], " ");
	}
	i = -1;
	while(args[++i])
	{
		pipe = 0;
		if(args[i + 1])
			pipe = 1;   
		new = ft_lstnew(args[i], i, env, pipe);
		ft_lstadd_back(&shell, new);
	}
	freedouble(args);
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
		//rl_replace_line("", 1);
		rl_redisplay();
	}
	else if(sig == SIGQUIT)
			return ;
}

void	mini_shell(char **env)
{
	char *read;
	char *line;
	char *tmp;
	t_shell *shell;
	t_env	*ev;

	ev = create_env(env);
	while (1)
	{
		read = readline("\033[0;32mMinishell>> \033[0m");
		if (!read)
			exit(0);
		tmp = ft_strdup(read);
		add_history(read);
		read += ft_checkspace(read);
		if (read[0] && !parse_syntax(tmp, 0))
		{
			line = parse_read(read);
			shell = parse_line(line, env);
			// print_data(shell);
			execute(shell, ev);
			free(line);
			//system("leaks mini_shell");
		}
		else if (read[0])
			printf("syntax error\n");
		// printf("lol\n");
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
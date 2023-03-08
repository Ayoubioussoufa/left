#include "mini_shell.h"

void	print_data(t_shell *shell)
{
	int i;

	while(shell)
	{
		i = 0;
		printf("index : %d      pipe : %d\n", shell->index, shell->pipe);
		while(shell->cmds[i])
			printf("%s\t", shell->cmds[i++]);
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
				printf("DELIMITER : %s : %d\n", shell->redir->delimiter, shell->redir->quotes);
			shell->redir = shell->redir->next;
		}
		shell = shell->next;
	}
}

int check_edges(char *str)
{
	if (str[0] == '\"' && str[ft_strlen(str) - 1] == '\"')
		return (1);
	if (str[0] == '\'' && str[ft_strlen(str) - 1] == '\'')
		return (2);
	else
		return (0);
}

char	*parseword(char *word, char **env)
{
	char	*new_word;

	if (check_edges(word) == 1)
	{
		new_word = get_value(word, env);
		new_word = handle_param(new_word, '\"');
	}
	else if (check_edges(word) == 2)
	{
		new_word = get_value(word, env);
		new_word = handle_param(new_word, '\'');
	}
	else
	{
		new_word = get_value(word, env);
		new_word = handle_param(new_word, '\'');
		new_word = handle_param(new_word, '\"');
	}
	free(word);
	return (new_word);
}

void	ft_getnew(char **split, char **env, int i, t_shell **new)
{
	char	**tmp;
	int		j;

	while (split[i])
	{
		split[i] = parseword(split[i], env);
		if (!ft_strcmp(split[i], ">"))
			redi_add_back(&(*new)->redir, new_redir(parseword(split[++i], env), OUTFILE));
		else if (!ft_strcmp(split[i], "<"))
			redi_add_back(&(*new)->redir, new_redir(parseword(split[++i], env), INFILE));
		else if (!ft_strcmp(split[i], "<<"))
			redi_add_back(&(*new)->redir, new_redir(parseword(split[++i], env), DELIMITER));
		else if (!ft_strcmp(split[i], ">>"))
			redi_add_back(&(*new)->redir, new_redir(parseword(split[++i], env), APPEND));
		else
		{
			if (check_edges(split[i]))
			{
				tmp = ft_split(split[i], ' ');
				j = 0;
				while (tmp[j])
					cmd_add_back(&(*new)->cmd, new_cmd(tmp[j++]));
			}
			else
				cmd_add_back(&(*new)->cmd, new_cmd(split[i]));
		}
		i++;
	}
}

t_shell	*ft_lstnew(char *content, int index, char **env, int pipe)
{
	t_shell	*new;
	char	**split;
	(void)env;
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
	free(split);
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

void	freedata(t_shell **data)
{
	while(*data)
	{
		while ((*data)->cmd)
		{
			free((*data)->cmd->cmd);
			(*data)->cmd = (*data)->cmd->next;
		}
		free((*data)->cmd);
		while((*data)->redir)
		{
			if ((*data)->redir->type == OUTFILE)
				free((*data)->redir->outfile);
			if ((*data)->redir->type == INFILE)
				free((*data)->redir->infile);
			if ((*data)->redir->type == DELIMITER)
				free((*data)->redir->delimiter);
			if ((*data)->redir->type == APPEND)
				free((*data)->redir->outfile);
		}
		free((*data)->redir);
		(*data) = (*data)->next;
	}
	free(*data);
}

t_shell	*parse_line(char *line, char **env)
{
	t_shell	*shell;
	t_shell	*new;
	char	**args;
	int		i;
	int		pipe;
	// (void)env;
	// (void)pipe;
	// (void)new;
	// (void)line;
	// (void)args;
	i = -1;
	shell = 0;
	args = ft_split_v2(line, '|');
	if (!args)
		return (0);
	while (args[++i])
		args[i] = ft_strtrimfree(args[i], " ");
	i = -1;
	while (args[++i])
	{
		pipe = 0;
		if(args[i + 1])
			pipe = 1;   
		new = ft_lstnew(args[i], i, env, pipe);
		ft_lstadd_back(&shell, new);
	}
	freedouble(args);
	return (shell);
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
	char *line;
	t_shell *shell;
	t_env	*ev;

	ev = create_env(env);
	while (1)
	{
		read = readline("\033[0;32mMinishell>> \033[0m");
		if (!read)
			exit(0);
		add_history(read);
		if (read[0])
		{
			if (!parse_syntax(read, 0))
			{
				line = parse_read(read);
				shell = parse_line(line, ev->env);
				execute(shell, ev);
			}
			else
				printf("syntax error\n");
		}
	}
}

int main(int ac, char **av, char **env)
{
	(void)av;

	if (ac != 1)
	{
		printf("invalid number of argument\n");
		return(0);
	}
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, sigint_handler);
	mini_shell(env);
	return 0;
}

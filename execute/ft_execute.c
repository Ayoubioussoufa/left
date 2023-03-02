/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execute.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aybiouss <aybiouss@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/09 18:28:01 by aybiouss          #+#    #+#             */
/*   Updated: 2023/03/02 10:25:37 by aybiouss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../mini_shell.h"

void	exec_redir_in(char *infile, int *in)
{
	if (access(infile, F_OK) == 0)
	{
		close(*in);
		*in = open(infile, O_RDONLY, 0666);
	}
	else
	{
		*in = -1;
		write(2, "minishell: ", 11);
		ft_perror(infile, ": No such file or directory");
	}
}

void	exec_redir(t_redire *redir, t_fd *fd)
{
	t_redire	*tmp;

	tmp = redir;
	while (tmp)
	{
		if (tmp->type == INFILE)
			exec_redir_in(tmp->infile, &fd->in);
		else if (tmp->type == OUTFILE)
		{
			close(fd->out);
			fd->out = open(tmp->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0666);
		}
		else if (tmp->type == APPEND)
		{
			close(fd->out);
			fd->out = open(tmp->outfile, O_WRONLY | O_CREAT | O_APPEND, 0666);
		}
		else if (tmp->type == DELIMITER)
		{
			close(fd->in);
			fd->in = open(tmp->delimiter, O_RDONLY, 0666);
		}
		tmp = tmp->next;
	}
}

void	check_fd_builtins(t_cmd *cmd)
{
	if (cmd->fd.in != 0)
	{
		dup2(cmd->fd.in, STDIN_FILENO);
		close(cmd->fd.in);
	}
	if (cmd->fd.out != 1)
	{
		dup2(cmd->fd.out, STDOUT_FILENO);
		close(cmd->fd.out);
	}
}

void	error(char *str, int n)
{
	if (str)
	{
		ft_putstr_fd(strerror(n), 2);
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(str, 2);
		ft_putstr_fd("\n", 2);
	}
	exit(n);
}

char	*get_cmd(char **paths, char *cmd)
{
	int		i;
	char	*tmp;
	char	*path;

	if (ft_strchrr(cmd, '/'))
		return (cmd);
	i = 0;
	while (paths[i])
	{
		tmp = ft_strjoin(paths[i], "/");
		path = ft_strjoin(tmp, cmd);
		free(tmp);
		if (access(path, F_OK) == 0)
			return (path);
		free(path);
		i++;
	}
	return (NULL);
}

char	**get_paths(char **env, t_shell *shell)
{
	char	**paths;
	int		i;

	i = 0;
	while (env[i])
	{
		if (ft_strnstr(env[i], "PATH", 4))
		{
			paths = ft_split(env[i] + 5, ':');
			if (!paths)
				error("Split function failed", 1);
			return (paths);
		}
		i++;
	}
	ft_putstr_fd("Command not found: ", 2);
	ft_putstr_fd(*shell->cmds, 2);
	ft_putstr_fd("\n", 2);
	exit(127);
	return (NULL);
}

void	ft_execute(t_shell *shell, char **env)
{
	pid_t	pid;
	char	**paths = NULL;
	char	*argv = NULL;

	pid = fork();
	if (pid == -1)
		return ;
	else if (pid == 0)
	{
		if (shell->redir)
		{
			exec_redir(shell->redir, &shell->cmd->fd);
			if (shell->cmd->fd.in == -1)
				exit(1);
		}
		check_fd_builtins(shell->cmd);
		paths = get_paths(env, shell);
		argv = get_cmd(paths, shell->cmds[0]);
		if (execve(argv, shell->cmds, env) == -1)
			error(NULL, errno);
	}
	if (shell->cmd->fd.in != 0)
		close(shell->cmd->fd.in);
	if (shell->cmd->fd.out != 1)
		close(shell->cmd->fd.out);
}

int	check_builtins(char *cmd)
{
	if (cmd && !ft_strcmp(cmd, "export"))
		return (1);
	else if (cmd && !ft_strcmp(cmd, "unset"))
		return (1);
	else if (cmd && !ft_strcmp(cmd, "pwd"))
		return (1);
	else if (cmd && !ft_strcmp(cmd, "exit"))
		return (1);
	else if (cmd && !ft_strcmp(cmd, "cd"))
		return (1);
	else if (cmd && !ft_strcmp(cmd, "env"))
		return (1);
	else if (cmd && !ft_strcmp(cmd, "echo"))
		return (1);
	return (0);
}

void	ft_which_cmd(char **cmd, char ***env)
{
	if (cmd[0] && !ft_strcmp(cmd[0], "export"))
		export_builtin(cmd, env);
	else if (cmd[0] && !ft_strcmp(cmd[0], "unset"))
		unset_builtin(cmd, env);
	else if (cmd[0] && !ft_strcmp(cmd[0], "pwd"))
		pwd_builtin(*cmd);
	else if (cmd[0] && !ft_strcmp(cmd[0], "exit"))
		exit_builtin(cmd, *env);
	else if (cmd[0] && !ft_strcmp(cmd[0], "cd"))
		cd_builtin(cmd, env);
	else if (cmd[0] && !ft_strcmp(cmd[0], "env"))
		env_builtin(cmd, *env);
	else if (cmd[0] && !ft_strcmp(cmd[0], "echo"))
		echo_builtin(cmd);
}

void	execute_builtin(t_shell *shell, char ***env)
{
	int	in;
	int	out;

	in = dup(STDIN_FILENO);
	out = dup(STDOUT_FILENO);
	if (shell->redir)
		exec_redir(shell->redir, &shell->cmd->fd);
	if (shell->cmd->fd.in == -1)
	{
		//status = 1;
		return ;
	}
	check_fd_builtins(shell->cmd);
	ft_which_cmd(shell->cmds, env);
	dup2(in, STDIN_FILENO);
	dup2(out, STDOUT_FILENO);
}

int	exec_builtins_execve(t_shell *shell, char ***env)
{
	if (check_builtins(shell->cmds[0]) == 1)
		execute_builtin(shell, env);
	else
		ft_execute(shell, *env);
	return (0);
}

void	execute(t_shell *shell, char ***env)
{
	shell->cmd->fd.in = 0;
	shell->cmd->fd.out = 1;
	if (shell->type == CMD)
		exec_builtins_execve(shell, env);
}

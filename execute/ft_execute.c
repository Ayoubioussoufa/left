/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execute.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aybiouss <aybiouss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/09 18:28:01 by aybiouss          #+#    #+#             */
/*   Updated: 2023/03/01 11:59:07 by aybiouss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../mini_shell.h"

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

	if (ft_strchr(cmd, '/'))
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
	char	**paths;
	int	fd[2];
	char	*argv;
	int	ret = 0;
	// (void)env;
	pipe(fd);
	pid = fork();
	// int i = ft_lstsize(shell);
	// while (shell)
	// {
	// 	printf("%s \t %d\n", shell->cmd, shell->type);
	// 	shell = shell->next;
	// }
	// printf("%d\n", i);
	if (pid == 0)
	{
		// shell->cmds = ft_split(shell->cmd, ' '); //split the given commands
		// printf("%s\n", shell->cmds[0]);
		// printf("%s\n", shell->cmds[0] + 1);
		paths = get_paths(env, shell); //get all paths
		argv = get_cmd(paths, shell->cmds[0]); // join the path with the command
		if (execve(argv, shell->cmds, env) == -1) //execute the cmd in the given env
			error(NULL, errno);
	}
	else
		waitpid(-1, &ret, 0);
}

// void	checktype(t_shell *shell, char **env)
// {
// 	// printf("%d\n", shell->type);
// 	// while (1);
// 	if (shell->type == 0)
// 	{
// 		shell->infile = open(shell->next->cmd, O_RDONLY);
// 		if (shell->infile == -1)
// 			error("File not found", 2);
// 		dup2(shell->infile, 0);
// 		close(shell->infile);
// 		dup2(shell->fd[1], 1);
// 		close(shell->fd[1]);
// 	}
// 	// else if (shell->type == 2)
// 	// {
// 	// 	dup2(shell->fd[1], 1);
// 	// 	close(shell->fd[1]);
// 	// }
// 	if (shell->type == 3)
// 	{
// 		// if (shell->next->type == 1)
// 		// {
// 		// 	shell->outfile = open(shell->next->next->cmd, O_WRONLY | O_CREAT | O_TRUNC, 0644);
// 		// 	if (shell->outfile == -1)
// 		// 		error("File not found", 13);
// 		// 	dup2(shell->outfile, 1);
// 		// 	close(shell->outfile);
// 		// }
// 		// close(shell->fd[0]);
// 		// close(shell->fd[1]);
// 		ft_execute(shell, env);
// 	}
// }
// void	checktype(t_shell *shell, char **env)
// {
// 	if (shell->type == 0)
// 	{
// 		shell->infile = open(shell->next->cmd, O_RDONLY);
// 		dup2(shell->fd[0], 0);
// 		close(shell->fd[0]);
// 	}
// 	else if (shell->type == 2)
// 	{
// 		dup2(shell->fd[1], 1);
// 		close(shell->fd[1]);
// 	}
// 	if (shell->type == 3)
// 	{
// 		if (shell->next->type == 1)
// 		{
// 			shell->infile = open(shell->next->next->cmd, O_WRONLY | O_CREAT | O_TRUNC, 0644);
// 			dup2(shell->infile, 1);
// 			close(shell->infile);
// 		}
// 		close(shell->fd[0]);
// 		close(shell->fd[1]);
// 		ft_execute(shell, env);
// 	}
// }

// void	checks(t_shell *shell, char **env)
// {
// 	pid_t	pid;

// 	pipe(shell->fd);//protection mnb3d
// 	pid = fork();//protection mnb3d
// 	if (pid == 0)
// 		checktype(shell, env);
// 	else
// 	{
// 		if (dup2(shell->fd[0], 0) == -1)
// 			error("dup2", errno);
// 		close(shell->fd[0]);
// 		close(shell->fd[1]);
// 	}
// }

// void	whatever(t_shell *shell, char **env)
// {
// 	int	ret = 0;

// 	// while (shell)
// 	// {
// 		checks(shell, env);
// 		// shell = shell->next;
// 	// }
// 	waitpid(-1, &ret, 0);
// // 	while (wait(NULL) != -1)
// // 		;
// }
// void	whatever(t_shell *shell, char **env)
// {
// 	while (shell)
// 	{
// 		checks(shell, env);
// 		shell = shell->next;
// 	}
// 	while (wait(NULL) != -1)
// 		;
// }

/*&& shell->type == 0)
		{
			shell->infile = open(shell->next->cmd, O_RDONLY);
			dup2(shell->fd[0], 0);
			close(shell->fd[0]);
		}
		else if (pid == 0 && shell->type == 2)
		{
			dup2(shell->fd[1], 1);
			close(shell->fd[1]);
		}
		if (pid == 0 && shell->type == 3)
		{
			if (shell->next->type == 1)
			{
				shell->infile = open(shell->next->next->cmd, O_WRONLY | O_CREAT | O_TRUNC, 0644);
				dup2(shell->infile, 1);
				close(shell->infile);
			}
			ft_execute(shell, env);
		}
		else
		{
			shell = shell->next;
			printf("%d\n", ret++);
		}
	}*/
	
//List of WHAT I SHOULD DO :
//check if it's a builtin command
//check if it's a system command
//check if it's file or folder
//if nothing above then error

/* I gotta build th following builtins :
echo with option -n
◦ cd with only a relative or absolute path
◦ pwd with no options
◦ export with no options
◦ unset with no options
◦ env with no options or arguments
◦ exit with no options
Each one with it's own functions !!

Implement pipes (| character). The output of each command in the pipeline is
connected to the input of the next command via a pipe.
• Handle environment variables ($ followed by a sequence of characters) which
should expand to their values.
• Handle $? which should expand to the exit status of the most recently executed
foreground pipeline

!!!!!Explanation of each function we can use:!!!!!

readline: readline is a function that reads a line from the specified stream and stores it into the string pointed to by str. It stops when either (n-1) characters are read, the newline character is read, or the end-of-file is reached, whichever comes first.

rl_clear_history: rl_clear_history is a function that clears the history list of previously entered commands.

rl_on_new_line: rl_on_new_line is a function that sets up the state for reading a new line of input. This includes setting up the prompt and any other necessary state variables.

rl_replace_line: rl_replace_line is a function that replaces the current line of input with a new string. This can be used to modify an existing command before executing it.

rl_redisplay: rl_redisplay is a function that redraws the current line of input on the screen. This can be used to update the display after modifying an existing command before executing it.

add_history: add_history is a function that adds a line of input to the history list of previously entered commands. This can be used to save commands for later recall or execution.

printf: printf is a function that prints formatted output to stdout (the standard output stream). It takes a format string and optional arguments and prints them according to the format string's specifications.

malloc: malloc is a function that allocates memory on the heap (dynamic memory). It takes one argument, which specifies how many bytes of memory should be allocated, and returns a pointer to this newly allocated memory block.

free: free is a function that deallocates memory on the heap (dynamic memory). It takes one argument, which should be a pointer to previously allocated memory, and deallocates this block of memory so it can be reused by other parts of the program.

write: write is a system call that writes data from a buffer to an open file descriptor (a file or device). It takes three arguments; the file descriptor, pointer to data buffer, and number of bytes to write from buffer.

access: access is a system call that checks if calling process has permission to access specified file or directory pathname. It takes two arguments; pathname and mode (which specifies what type of access should be checked).

open: open is a system call that opens an existing file or creates a new one if it doesn't exist yet. It takes three arguments; pathname, flags (which specify how file should be opened), and mode (which specifies permissions for newly created files).

read: read is a system call that reads data from an open file descriptor into buffer provided by caller process. It takes three arguments; file descriptor, pointer to data buffer, and number of bytes to read into buffer from file descriptor.

close: close is a system call that closes an open file descriptor so it can no longer be used by calling process. It takes one argument; file descriptor which should be closed by calling process.

fork: fork is a system call that creates another process with same environment as calling process but with different PID (process ID). It takes no arguments and returns PID of newly created process in parent process while returning 0 in child process created by fork().

wait: wait is a system call which waits for any child processes created by calling process using fork() system call to terminate before continuing execution in parent process. It takes no arguments and returns PID of terminated child process in parent while returning -1 if there are no child processes left for parent process to wait for termination.

waitpid: waitpid is similar to wait but instead of waiting for any child processes created by calling process using fork() system call it waits only for specific child processes specified by its PID argument passed as first argument in waitpid().

wait3/wait4: wait3/wait4 are similar to wait/waitpid but they provide more information about terminated child processes such as resource usage statistics like CPU time used etc.. They take additional argument which points to structure where this information will be stored after termination of child processes specified in their first argument passed as PIDs in these functions respectively..

signal/sigaction/sigemptyset/sigaddset : signal/sigaction/sigemptyset/sigaddset are functions related to handling signals sent from kernel or other processes running on same machine as calling process.. Signal() sets handler for specific signal passed as its first argument while sigaction() sets more detailed handler with additional flags like SA_RESTART etc.. Sigemptyset() initializes empty set while sigaddset() adds specific signal passed as its second argument into set pointed by its first argument..

kill : kill is system call which sends signal specified as its second argument passed as integer value representing signal number into another process specified by its first argument passed as PID..

exit : exit is library function which terminates current running program with return code specified as its only argument passed as integer value..

getcwd : getcwd is library function which gets current working directory pathname relative from root directory '/'.. It takes two arguments; pointer where returned pathname will be stored and size limit for returned pathname..

chdir : chdir is system call which changes current working directory relative from root directory '/' into directory specified by its only argument passed as pathname string..

stat/lstat/fstat : stat/lstat/fstat are functions related with getting information about files such as size, permissions etc.. Stat() gets information about regular files while lstat() gets information about symbolic links while fstat() gets information about open files using their respective file descriptors passed as their only arguments respectively..

unlink : unlink is system call which deletes regular files or symbolic links specified by its only argument passed as pathname string..

execve : execve is system call which replaces currently running program image with new program image specified by its first argument passed as pathname string pointing towards executable binary image while passing additional arguments needed for execution into this new program image using second and third arguments passed respectively as array containing these additional arguments followed by array containing environment variables needed for execution respectively..

dup/dup2 : dup/dup2 are functions related with duplicating existing open file descriptors so they can be used multiple times without closing them explicitly each time they are not needed anymore.. Dup() duplicates given open file descriptor while dup2() duplicates given open file descriptor into specific given target file descriptor both taking single argument each respectively being source open file descriptor in case of dup() and source & target open file descriptors in case of dup2() respectively..

pipe : pipe creates two connected pipes between two processes so they can communicate between each other using these pipes without having direct access between them due their separate address spaces provided by operating systems virtual memory management techniques like paging etc... It takes two arguments; array containing two elements representing both ends of pipe where first element represents reading end while second element represents writing end respectively ..

opendir: This function is used to open a directory stream for the specified directory name. It returns a pointer to the opened directory stream or NULL if the directory could not be opened.

readdir: This function is used to read an entry from the specified directory stream. It returns a pointer to a structure containing information about the next entry in the directory, or NULL if there are no more entries in the directory.

closedir: This function is used to close a directory stream that was previously opened with opendir(). It returns 0 on success and -1 on failure.

strerror: This function is used to return a string describing an error code. It takes an integer error code as its argument and returns a pointer to a string describing the error code.

perror: This function is used to print an error message corresponding to an error code. It takes a string as its argument and prints it followed by a colon and then the error message corresponding to the error code passed as its argument.

isatty: This function is used to check whether a file descriptor refers to a terminal device or not. It takes an integer file descriptor as its argument and returns 1 if it refers to a terminal device, 0 otherwise.

ttyname: This function is used to return the name of the terminal device associated with a file descriptor. It takes an integer file descriptor as its argument and returns a pointer to a string containing the name of the terminal device associated with it, or NULL if there is no such device associated with it.

ttyslot: This function is used to return the index of the slot in which the terminal device associated with a file descriptor resides. It takes an integer file descriptor as its argument and returns an integer index of the slot in which it resides, or -1 if there is no such device associated with it.

ioctl: This function is used for performing various operations on devices such as terminals, disks, tapes etc. It takes three arguments – an integer file descriptor, an operation code and optionally some data depending on what operation needs to be performed – and performs that operation on that device associated with that file descriptor.

getenv: This function is used for retrieving environment variables from the environment list maintained by the operating system.  – and returns a pointer to that variable’s value in case it exists in the environment list, or NULL otherwise.

tcsetattr: This function is used for setting parameters associated with terminals such as baud rate, parity etc. It takes three arguments – an integer file descriptor referring to a terminal device, optional flags indicating how certain parameters should be set, and another structure containing all parameters that need to be set – and sets those parameters accordingly for that terminal device associated with that file descriptor.

tcgetattr: This function is similar to tcsetattr() but instead of setting parameters for terminals, it retrieves them from them instead. It takes two arguments – an integer file descriptor referring to a terminal device, and another structure where all retrieved parameters will be stored – and retrieves all parameters from that terminal device into that structure accordingly.

tgetent: This function is used for retrieving information about terminals from their termcap database entries (termcap entries). It takes two arguments –a buffer where all retrieved information will be stored, and another string containing either name of termcap entry or pathname of termcap database entry–and retrieves all information about that particular terminal into that buffer accordingly.

tgetflag: This function is similar tgetent() but instead of retrieving information about terminals from their termcap database entries (termcap entries), it retrieves only flags (boolean values) from those entries instead .It takes two arguments–a buffer where all retrieved flags will be stored, and another string containing either name

tgetnum: This function is used to retrieve a numeric value from the terminal's capability database. It takes two arguments, the first being the name of the capability to be retrieved and the second being a pointer to an integer where the value will be stored.

tgetstr: This function is used to retrieve a string value from the terminal's capability database. It takes two arguments, the first being the name of the capability to be retrieved and the second being a pointer to a character array where the string will be stored.

tgoto: This function is used to move the cursor on a terminal screen. It takes three arguments, the first being an escape sequence that specifies how many columns and rows to move, the second being an integer specifying how many columns to move, and third being an integer specifying how many rows to move.

tputs: This function is used to output a string of characters on a terminal screen. It takes two arguments, the first being a pointer to a character array containing the string of characters to be outputted, and second being an integer specifying how many characters should be outputted per line.
*/

//symply pipex with some builtins???
//for cd : PWD=/Users/aybiouss/Desktop/Minishella ||OLDPWD=/Users/aybiouss (cd to HOME variable and cd - to OLDPWD) [Navigate the env]

//for pwd : PWD in env	

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

void	exec_redir(t_shell *shell, t_fd *fd)
{
	t_redire	*tmp;

	tmp = shell->redir;
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

void	execute_builtin(t_shell *shell, char **env, t_fd *fd)
{
	int	in;
	int	out;

	in = dup(STDIN_FILENO);
	out = dup(STDOUT_FILENO);
	exec_redir(shell, fd);
	// if (fd.in == -1)
	// {
	// 	//status = 1;
	// 	return ;
	// }
	check_fd_builtins(shell->cmd);
	ft_which_cmd(shell->cmds, &env);
	// dup2(in, STDIN_FILENO);
	// dup2(out, STDOUT_FILENO);
}

int	exec_builtins_execve(t_shell *shell, char ***env, t_fd *fd)
{
	// int	pid;

	// pid = 0;
	// (void)fd;
	// (void)env;
	if (check_builtins(shell->cmds[0]) == 1)
		execute_builtin(shell, *env, fd);
	else
	{
		printf("hhhhhh\n");
		ft_execute(shell, *env);
	}
	return (0);
}

void	execute(t_shell *shell, char ***env)
{
	t_fd	*fd;

	fd = malloc(sizeof(t_fd));
	printf("%s\n", shell->cmds[0]);
	printf("%d\n", shell->type);
	if (shell->type == CMD)
		exec_builtins_execve(shell, env, fd);
}

// void	checkingcmd(t_shell *shell, char **env)
// {
// 	while (shell)
// 	{
// 		if (check_builtin(shell) && shell->type == 3) // if = 1 donc machi built in donc ghir nsiftha t executa
// 			ft_execute(shell, env);
// 		if (shell->type == 2) //pipe
// 		{
			
// 		}
// 	}
// }

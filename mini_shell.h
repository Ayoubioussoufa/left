/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_shell.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aybiouss <aybiouss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/08 17:36:00 by sben-ela          #+#    #+#             */
/*   Updated: 2023/03/02 16:40:20 by aybiouss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


# ifndef MINI_SHELL_H
# define  MINI_SHELL_H

#define		PIPE 2
#define		STRING 3
#define		INFILE 0
#define		OUTFILE 1
#define		APPEND 5
#define		DELIMITER 4
#define		CMD 3
#include	<stdio.h>
#include	<stdlib.h>
#include	<unistd.h>
#include	<readline/readline.h>
#include	<readline/history.h>
#include	<fcntl.h>
# include <sys/wait.h>
# include <sys/errno.h>
# include <string.h>

typedef struct s_env_elem
{
	char				*key;
	char				*value;
	struct s_env_elem	*next;
	struct s_env_elem	*prev;
}	t_env_elem;

typedef struct s_env
{
	t_env_elem	*head;
	char		**env;
	int			size;
}	t_env;

typedef struct s_fd
{
	int	in;
	int	out;
}	t_fd;

typedef struct redire
{
	int		type;
	char	*infile;// 0 : input
	char	*outfile; // 1 : out
	char	*delimiter;
	struct redire *next;
}	t_redire;

typedef	struct cmd
{
	char		*cmd;
	t_fd		fd;
	struct cmd	*next;
}	t_cmd;

typedef	struct	shell
{
	char			**cmds;
	int				pipe;
	t_cmd			*cmd;
	int				type;
	t_redire		*redir;
	struct shell	*next;
	struct shell	*previous;
	int				index;
}	t_shell;

char **full_cmds(t_cmd *cmd);
int	cmd_size(t_cmd *cmds);
void	execute(t_shell *shell, char ***env);
char	*ft_strchrr(const char *s, int c);
char	**ft_split(char const *str, char c);
void    ft_execute(t_shell *shell, char **env);
// void    ft_execute(t_shell *shell, char **env,t_fd *fd);
void	whatever(t_shell *shell, char **env);
char	*parse_redirect(char *line);
//utils
int ft_strcmp(char *s1, char *s2);
int	handle_couts(t_shell *shell);
int		ft_strncmp(char *s1, char *s2, unsigned int n);
// char	*ft_strjoin(char const *s1, char const *s2);
char	*ft_strjoinfree(char *s1, char *s2);
char	*ft_strnstr(const char *haystack, const char *needle, size_t len);
void	ft_putstr_fd(char *s, int fd);
void	ft_putnstr(char *str, int n);
int    echo_builtin(char **cmd);
void	ft_putchar(char c);
int	unset_builtin(char **cmd, char ***ev);
int	ft_isalpha(int c);
char	*ft_strdup(char *str);
char	*ft_substr(char *s, int start, int len);
// char	*ft_substr(char const *s, unsigned int start, size_t len);
// char	*ft_strdup(const char *source);
void	redi_add_back(t_redire **lst, t_redire *new);
t_redire	*redilast(t_redire *lst);
t_redire	*new_redir(char *content, int type);
t_cmd	*last_cmd(t_cmd *lst);
void	cmd_add_back(t_cmd **lst, t_cmd *new);
t_cmd	*new_cmd(char *content);
char	**ft_split_v2(char const *str, char c);
char *parse_read(char *read);
//env functions
char	**convert_array(t_env *env);
void	del_env(t_env *env);
void	print_env(t_env *env);
t_env	*create_env(char **tab);
t_env	*init_env(t_env *env);
t_env_elem	*new_env_elem(char *line);
void	add_env_elem(t_env *env, t_env_elem *new);
t_env_elem	*search_env_elem(t_env *env, char *key);
void	del_env_elem(t_env *env, t_env_elem *elem);
char	*expand_env(char *line, char **env);
char	*get_expanded_value(char **line, char **env);
char	*search_env_var(char *key, char **env);
int	ft_isalnum(int c);
char	*ft_itoa(int n);
//error
int	ft_puterr(char *cmd, char *arg, char *msg, int errnum);
int	ft_perror(char *msg, char *utils);

int    ft_strchr(char *s, int c);
int        ft_strlen(char *str);
char    **ft_split_v2(char const *str, char c);
char    *parse_redirect(char *line);
//util
int        ft_isalpha(int c);
int        count_redirect(char *line);
t_shell    *ft_lstlast(t_shell *lst);
void    ft_lstadd_back(t_shell **lst, t_shell *new);
int        count_char(char *str, char c);
char    *handle_param(char *str, char c);
int        ft_strncmp(char *s1, char *s2, unsigned int n);
char    *ft_strjoin(char *left_str, char *buff);
void    ft_putstr_fd(char *s, int fd);
int	parse_syntax(char *line, char c);
int        ft_checkspace(const char *str);
int        count_single_couts(char *line);
int        count_double_couts(char *line);
char    **second_split(char const *s, char c);
char       *get_value(char *str,char **env);
int        ft_isalnum(int c);
char    *ft_strnstr(const char *haystack, const char *needle, size_t len);
char    *ft_strdup(char *source);
int handle_pipes(char *line);
char	*ft_strtrimfree(char *s1, char *set);
char	*ft_strtrim(char *s1, char *set);
void    freedouble(char **args);

// builtin cd
int	cd_builtin(char **cmd, char ***env);
void	set_dir(t_env *ev, char *cwd, char *owd);

//builtin echo
int	echo_builtin(char **cmd);
void	echo(char **str, int pos);
void	ft_putstr(char *s);
void	ft_putchar(char c);

// builtin env
int env_builtin(char **cmd, char **env);

// builtin exit
int exit_builtin(char **cmd, char **env);

//builtin export
int	export_builtin(char **cmd, char ***ev);
void	ft_swapelem(t_env_elem *a, t_env_elem *b);
void	ft_sort(t_env *env);
void	print_sorted_env(t_env *env);
void	add_var(t_env *env, char *cmd); //hadchi ghalt f export 
//builtin pwd
int	pwd_builtin(char *cmd);
//builtin unset
int	unset_builtin(char **cmd, char ***ev);
#endif

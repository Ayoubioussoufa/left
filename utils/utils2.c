#include "../mini_shell.h"

void    freedouble(char **args)
{
    int    i;

    i = 0;
    while(args[i])
        free(args[i++]);
    free(args);
}

int ft_strcmp(char *s1, char *s2)
{
    int i;

    i = 0;
    while (s1[i] && s2[i] && s1[i] == s2[i])
        i++;
    return (s1[i] - s2[i]);
}

t_redire	*new_redir(char *content, int type)
{
	t_redire	*new;
	new = malloc(sizeof(t_redire));
	if (!new)
		return (0);
	if (type == 0)
		new->infile = content;
	if (type == 1)
		new->outfile = content;
	if (type == 4)
		new->delimiter = content;
	if (type == 5)
		new->outfile = content;
	if (type != 3)
		new->type = type;
	new->next = 0;
	return (new);
}

t_shell	*ft_lstlast(t_shell *lst)
{
	if (!lst)
		return (0);
	while (lst->next)
	{
		lst = lst->next;
	}
	return (lst);
}

static int	check(char const *set, char s)
{
	int	i;

	i = 0;
	while (set[i])
	{
		if (set[i] == s)
			return (1);
		i++;
	}
	return (0);
}

char	*ft_strtrimfree(char *s1, char *set)
{
	int		i;
	int		len;
	char	*mem;

	if (!s1 || !set)
		return (NULL);
	i = 0;
	len = ft_strlen(s1) - 1;
	while (s1[i] && check(set, s1[i]))
		i++;
	while (s1[len] && check(set, s1[len]))
		len--;
	mem = ft_substr(s1, i, len - i + 1);
	//free(s1);
	return (mem);
}
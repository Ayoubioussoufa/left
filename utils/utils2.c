#include "../mini_shell.h"

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
	if(type == 0)
		new->infile = content;
	if(type == 1)
		new->outfile = content;
	if(type == 4)
		new->delimiter = content;
	if(type == 5)
		new->outfile = content;
	if(type != 3)
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
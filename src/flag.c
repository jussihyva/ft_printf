/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flag.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkauppi <jkauppi@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/11 09:59:09 by jkauppi           #+#    #+#             */
/*   Updated: 2019/12/12 13:06:09 by jkauppi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void			format_string(t_substring *substring)
{
	int				flags;
	t_list			*elem;
	t_formatter		*formatter;

	flags = substring->flags;
	elem = *(substring->formatter_list);
	while (elem)
	{
		formatter = (t_formatter *)(elem->content);
		if (flags & formatter->flag)
			substring->output_string =
						formatter->function_ptr(substring->output_string);
		elem = elem->next;
	}
	return ;
}

static char		*format_minus(char *s)
{
	return (s);
}

static char		*format_plus(char *s)
{
	char		*new_string;

	if (s[0] != '-')
	{
		new_string = ft_strjoin("+", s);
		ft_strdel(&s);
	}
	else
		new_string = s;
	
	return (new_string);
}

static char		*format_space(char *s)
{
	return (s);
}

static char		*format_zero(char *s)
{
	return (s);
}

static char		*format_hash(char *s)
{
	char		*new_string;

	if (s[0] != '#')
	{
		new_string = ft_strjoin("0x", s);
		ft_strdel(&s);
	}
	else
		new_string = s;
	
	return (new_string);
}

int				parse_flags(char *s, int valid_flags, t_list **formatter_list)
{
	int		i;
	int		s_len;
	int		flags;
	t_list	*elem;

	(void)valid_flags;
	(void)formatter_list;
	s_len = ft_strlen(s);
	i = 1;
	if (s[0] != '%' || s[s_len - 1] == '%')
		flags = 0;
	else
	{
		flags = 0;
		i = 0;
		while (++i < (s_len - 1))
		{
			elem = *formatter_list;
			while (elem)
			{
				if (s[i] == ((t_formatter *)(elem->content))->character)
					flags |= ((t_formatter *)(elem->content))->flag;
				elem = elem->next;
			}
		}
	}
	return (flags);
}

static t_list	*new_formatter(void *function, char character, t_flag flag)
{
	t_formatter		*formatter;
	t_list			*elem;

	formatter = (t_formatter *)ft_memalloc(sizeof(*formatter));
	formatter->character = character;
	formatter->flag = flag;
	formatter->function_ptr = function;
	elem = (t_list *)ft_memalloc(sizeof(*elem));
	elem->content = (void *)formatter;
	elem->next = NULL;
	return (elem);
}

t_list			**create_formatters(void)
{
	t_list			**formatter_list;

	formatter_list = (t_list **)ft_memalloc(sizeof(*formatter_list));
	ft_lstadd_e(formatter_list, new_formatter((void *)format_minus, '-', minus));
	ft_lstadd_e(formatter_list, new_formatter((void *)format_plus, '+', plus));
	ft_lstadd_e(formatter_list, new_formatter((void *)format_space, ' ', space));
	ft_lstadd_e(formatter_list, new_formatter((void *)format_zero, '0', zero));
	ft_lstadd_e(formatter_list, new_formatter((void *)format_hash, '#', hash));
	return (formatter_list);
}

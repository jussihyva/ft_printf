/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   modify_integer.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkauppi <jkauppi@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/18 15:24:14 by jkauppi           #+#    #+#             */
/*   Updated: 2019/12/18 17:37:35 by jkauppi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static void				modify_substring_1(char *new_string, char filler,
											int size, t_substring *substring)
{
	ft_memset(new_string, filler, size -
								ft_strlen(substring->output_string));
	ft_strcat(new_string, substring->output_string);
	return ;
}

static void				modify_substring_3(char *new_string,
													t_substring *substring)
{
	ft_strcat(new_string, substring->output_string);
	ft_memset(new_string + ft_strlen(substring->output_string),
								' ', substring->width -
								ft_strlen(substring->output_string));
	return ;
}

static void				add_min_mum_of_digits(t_substring *substring, char c)
{
	char		*new_string;

	new_string = ft_strnew(sizeof(*new_string) * (substring->precision));
	*new_string = '\0';
	if (c == '@')
		modify_substring_1(new_string, '0', substring->precision, substring);
	else
	{
		*new_string = c;
		ft_memset(new_string + 1, '0', substring->precision -
									ft_strlen(substring->output_string) + 1);
		ft_strcat(new_string, substring->output_string + 1);
	}
	ft_strdel(&substring->output_string);
	substring->output_string = new_string;
	return ;
}

static void				add_min_mum_of_chars(t_substring *substring, char c)
{
	char		*new_string;

	new_string = ft_strnew(sizeof(*new_string) * (substring->width));
	if (substring->left_adjust)
		modify_substring_3(new_string, substring);
	else
	{
		if (substring->filler == ' ' || substring->precision != -1)
			modify_substring_1(new_string, ' ', substring->width, substring);
		else
		{
			if (c == '@')
				modify_substring_1(new_string, substring->filler,
												substring->width, substring);
			else
			{
				*new_string = c;
				ft_memset(new_string + 1, '0', substring->width -
									ft_strlen(substring->output_string));
				ft_strcat(new_string, substring->output_string + 1);
			}
		}
	}
	ft_strdel(&substring->output_string);
	substring->output_string = new_string;
}

void					adjust_int(t_substring *substring)
{
	if (substring->output_string[0] == '+' ||
		substring->output_string[0] == '-' ||
		substring->output_string[0] == ' ')
	{
		if ((int)ft_strlen(substring->output_string) < substring->precision + 1)
			add_min_mum_of_digits(substring, substring->output_string[0]);
		if ((int)ft_strlen(substring->output_string) < substring->width)
			add_min_mum_of_chars(substring, substring->output_string[0]);
	}
	else
	{
		if ((int)ft_strlen(substring->output_string) < substring->precision)
			add_min_mum_of_digits(substring, '@');
		if ((int)ft_strlen(substring->output_string) < substring->width)
			add_min_mum_of_chars(substring, '@');
	}
	return ;
}

static unsigned long	read_int_param(t_type type, va_list *ap)
{
	unsigned long	nbr;

	if (type == hh)
		nbr = (char)(va_arg(*ap, void *));
	else if (type == h)
		nbr = (short int)(va_arg(*ap, void *));
	else if (type == l)
		nbr = (long int)(va_arg(*ap, void *));
	else if (type == ll)
		nbr = (long long int)(va_arg(*ap, void *));
	else if (type == j)
		nbr = (intmax_t)(va_arg(*ap, void *));
	else if (type == z)
		nbr = (size_t)(va_arg(*ap, void *));
	else if (type == t)
		nbr = (ptrdiff_t)(va_arg(*ap, void *));
	else if (type == L)
		nbr = (int)(va_arg(*ap, void *));
	else
		nbr = (int)(va_arg(*ap, void *));
	return (nbr);
}

char					*conv_int(va_list *ap, t_substring *substring,
											int *attrs)
{
	unsigned long	nbr;
	char			*s;
	char			*output_string;

	(*attrs)++;
	if (!substring->param_type)
		nbr = (int)(va_arg(*ap, void *));
	else
		nbr = read_int_param(substring->param_type->type, ap);
	s = ft_ltoa_base(nbr, 10);
	output_string = format_string(s, substring);
	return (output_string);
}

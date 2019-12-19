/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   modify_octal.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkauppi <jkauppi@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/18 16:46:22 by jkauppi           #+#    #+#             */
/*   Updated: 2019/12/19 12:21:49 by jkauppi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static void				add_min_mum_of_digits(t_substring *substring, char c)
{
	char		*new_string;

	if (c == '@')
		substring->output_string = right_adjust('0', substring->output_string,
												substring->precision, "");
	else
	{
		new_string = ft_strnew(sizeof(*new_string) * (substring->precision));
		*new_string = c;
		ft_memset(new_string + 1, '0', substring->precision -
									ft_strlen(substring->output_string) + 1);
		ft_strcat(new_string, substring->output_string + 1);
		ft_strdel(&substring->output_string);
		substring->output_string = new_string;
	}
	return ;
}

static void				add_min_mum_of_chars(t_substring *substring, char c)
{
	if (substring->left_adjust)
		substring->output_string = left_adjust(substring->output_string,
												substring->width);
	else
	{
		if (substring->filler == ' ' || substring->precision != -1)
			substring->output_string = right_adjust(' ',
							substring->output_string, substring->width, "");
		else
		{
			if (c == '@')
				substring->output_string = right_adjust(substring->filler,
							substring->output_string, substring->width, "");
			else
				substring->output_string = right_adjust(substring->filler,
							substring->output_string, substring->width, "0");
		}
	}
}

void					adjust_unsigned_octal(t_substring *substring)
{
	char		*new_string;

	if ((int)ft_strlen(substring->output_string) < substring->precision)
		add_min_mum_of_digits(substring, '@');
	if ((int)ft_strlen(substring->output_string) < substring->width)
	{
		add_min_mum_of_chars(substring, '@');
	}
	if (substring->output_string[0] == '\0' && substring->precision == -1)
	{
		new_string = ft_strnew(1);
		*new_string = '0';
		ft_strdel(&substring->output_string);
		substring->output_string = new_string;
	}
	return ;
}

char					*conv_unsigned_octal(va_list *ap,
										t_substring *substring, int *attrs)
{
	unsigned int	nbr;
	char			*s;
	char			*output_string;
	char			*input_string;

	input_string = substring->input_string;
	(void)input_string;
	(*attrs)++;
	nbr = (unsigned int)(va_arg(*ap, void *));
	s = ft_ltoa_base(nbr, 8);
	if (!nbr)
		*s = '\0';
	output_string = format_string(s, substring);
	return (output_string);
}

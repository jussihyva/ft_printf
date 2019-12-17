/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_converters.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkauppi <jkauppi@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/05 13:12:58 by jkauppi           #+#    #+#             */
/*   Updated: 2019/12/17 20:43:17 by jkauppi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static void				add_converter(t_substring *substring,
										t_list **converter_list,
										t_list **formatter_list)
{
	char			character;
	char			*input_string;
	t_list			*elem;

	input_string = substring->input_string;
	character = input_string[ft_strlen(input_string) - 1];
	elem = *converter_list;
	while (elem)
	{
		if (character == ((t_converter *)(elem->content))->character)
		{
			substring->converter = (t_converter *)(elem->content);
			if (input_string[0] == '%' &&
							input_string[ft_strlen(input_string) - 1] != '%')
				substring->formatter_list = get_formatters(substring,
															formatter_list);
			break ;
		}
		elem = elem->next;
	}
}

static void				convert_substring(t_substring *substring, va_list *ap,
											int *attrs)
{
	if (!substring->converter || !substring->converter->function_ptr)
		substring->output_string = ft_strdup(substring->input_string);
	else
	{
		substring->output_string = substring->converter->function_ptr(ap,
									substring, attrs);
	}
	return ;
}

int						convert_substrings(t_list **list, va_list *ap,
											t_list **converter_list,
											t_list **formatter_list)
{
	t_list			*elem;
	t_substring		*substring;
	int				attrs;
	int				max_num_attrs;

	max_num_attrs = 190;
	attrs = 0;
	elem = *list;
	while (elem)
	{
		substring = (t_substring *)(elem->content);
		substring->converter = NULL;
		substring->flags = 0;
		add_converter(substring, converter_list, formatter_list);
		convert_substring(substring, ap, &attrs);
		width_adjust(substring);
		elem = elem->next;
	}
	return (attrs);
}

static char				*no_conv(va_list *ap, t_substring *substring,
															int *attrs)
{
	char			*input_string;

	input_string = substring->input_string;
	(void)ap;
	(void)*attrs;
	input_string[ft_strlen(input_string) - 1] = '\0';
	return (ft_strdup(input_string));
}

static char				*conv_character(va_list *ap, t_substring *substring,
															int *attrs)
{
	char			*s;
	char			*output_string;

	(*attrs)++;
	s = ft_strnew(1);
	s[0] = (char)va_arg(*ap, int);
	output_string = format_string(s, substring);
	return (output_string);
}

static char				*conv_string(va_list *ap, t_substring *substring,
															int *attrs)
{
	char			*s;
	char			*output_string;

	(*attrs)++;
	s = ft_strdup(va_arg(*ap, char *));
	output_string = format_string(s, substring);
	return (output_string);
}

static char				*conv_pointer(va_list *ap, t_substring *substring,
															int *attrs)
{
	uintptr_t		ptr;
	char			*ptr_string;
	char			*s;
	char			*output_string;

	(*attrs)++;
	ptr = (uintptr_t)(va_arg(*ap, void *));
	ptr_string = ft_ltoa_base(ptr, 16);
	s = ft_strjoin("0x", ptr_string);
	ft_strdel(&ptr_string);
	output_string = format_string(s, substring);
	return (output_string);
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

static char				*conv_int(va_list *ap, t_substring *substring,
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

static char				*conv_unsigned_octal(va_list *ap,
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
	output_string = format_string(s, substring);
	return (output_string);
}

static char				*conv_unsigned_int(va_list *ap, t_substring *substring,
															int *attrs)
{
	unsigned int	nbr;
	char			*s;
	char			*input_string;

	input_string = substring->input_string;
	(void)input_string;
	(*attrs)++;
	nbr = (unsigned int)(va_arg(*ap, void *));
	s = ft_ltoa_base(nbr, 10);
	return (s);
}

static char				*conv_unsigned_hex(va_list *ap, t_substring *substring,
															int *attrs)
{
	unsigned int	nbr;
	char			*s;
	char			*output_string;
	char			*input_string;

	input_string = substring->input_string;
	(void)input_string;
	(*attrs)++;
	nbr = (unsigned int)(va_arg(*ap, void *));
	s = ft_ltoa_base(nbr, 16);
	output_string = format_string(s, substring);
	return (output_string);
}

static char				*conv_unsigned_hex_up(va_list *ap,
										t_substring *substring, int *attrs)
{
	unsigned int	nbr;
	char			*s;
	char			*output_string;
	int				i;
	char			*input_string;

	input_string = substring->input_string;
	(void)input_string;
	(*attrs)++;
	nbr = (unsigned int)(va_arg(*ap, void *));
	s = ft_ltoa_base(nbr, 16);
	output_string = format_string(s, substring);
	i = -1;
	while (*(output_string + ++i))
		*(output_string + i) = ft_toupper(*(output_string + i));
	return (output_string);
}

static char				*conv_float(va_list *ap, t_substring *substring,
															int *attrs)
{
	unsigned long	tmp;
	double			nbr;
	char			*s;
	char			*input_string;

	input_string = substring->input_string;
	(void)input_string;
	(*attrs)++;
	nbr = (double)(va_arg(*ap, double));
	tmp = (unsigned long)nbr;
	nbr *= 1000000;
	s = ft_ltoa_base(nbr, 10);
	return (s);
}

static char				*no_adjust(t_substring *substring)
{
	return (substring->output_string);
}

static char				*adjust_character(t_substring *substring)
{
	return (substring->output_string);
}

static char				*adjust_string(t_substring *substring)
{
	return (substring->output_string);
}

static char				*adjust_pointer(t_substring *substring)
{
	return (substring->output_string);
}

static char				*adjust_int(t_substring *substring)
{
	return (substring->output_string);
}

static char				*adjust_unsigned_octal(t_substring *substring)
{
	return (substring->output_string);
}

static char				*adjust_unsigned_int(t_substring *substring)
{
	return (substring->output_string);
}

static char				*adjust_unsigned_hex(t_substring *substring)
{
	return (substring->output_string);
}

static char				*adjust_unsigned_hex_up(t_substring *substring)
{
	return (substring->output_string);
}

static char				*adjust_float(t_substring *substring)
{
	return (substring->output_string);
}

static t_list			*new_conv(void *function, char character,
									void *adjust_width_prediction_ptr)
{
	t_converter		*converter;
	size_t			converter_size;
	t_list			*elem;
	int				valid_flags;

	valid_flags = 0;
	converter_size = sizeof(*converter);
	converter = (t_converter *)ft_memalloc(converter_size);
	converter->character = character;
	converter->valid_flags = valid_flags;
	converter->function_ptr = function;
	converter->adjust_width_prediction_ptr = adjust_width_prediction_ptr;
	elem = (t_list *)ft_memalloc(sizeof(*elem));
	elem->content = (void *)converter;
	elem->content_size = converter_size;
	elem->next = NULL;
	return (elem);
}

t_list					**create_converters(void)
{
	t_list			**conv_list;

	conv_list = (t_list **)ft_memalloc(sizeof(*conv_list));
	ft_lstadd_e(conv_list, new_conv((void *)no_conv, '%', no_adjust));
	ft_lstadd_e(conv_list, new_conv((void *)conv_character, 'c',
													adjust_character));
	ft_lstadd_e(conv_list, new_conv((void *)conv_string, 's', adjust_string));
	ft_lstadd_e(conv_list, new_conv((void *)conv_pointer, 'p', adjust_pointer));
	ft_lstadd_e(conv_list, new_conv((void *)conv_int, 'd', adjust_int));
	ft_lstadd_e(conv_list, new_conv((void *)conv_int, 'i', adjust_int));
	ft_lstadd_e(conv_list, new_conv((void *)conv_unsigned_octal, 'o',
													adjust_unsigned_octal));
	ft_lstadd_e(conv_list, new_conv((void *)conv_unsigned_int, 'u',
													adjust_unsigned_int));
	ft_lstadd_e(conv_list, new_conv((void *)conv_unsigned_hex, 'x',
													adjust_unsigned_hex));
	ft_lstadd_e(conv_list, new_conv((void *)conv_unsigned_hex_up, 'X',
													adjust_unsigned_hex_up));
	ft_lstadd_e(conv_list, new_conv((void *)conv_float, 'f', adjust_float));
	return (conv_list);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcelsa <dcelsa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/04 02:40:50 by dcelsa            #+#    #+#             */
/*   Updated: 2022/03/04 02:40:51 by dcelsa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

t_ui	paintpxl(t_color *color)
{
	return (color->t << 24 | color->r << 16 | color->g << 8 | color->b);
}

static char	converter(char *line)
{
	t_uchar	rgb;

	if ('0' <= line[0] && line[0] <= '9')
		rgb = (line[0] - '0') * 16;
	if ('A' <= line[0] && line[0] <= 'F')
		rgb = (line[0] - 'A' + 10) * 16;
	if ('a' <= line[0] && line[0] <= 'f')
		rgb = (line[0] - 'a' + 10) * 16;
	if ('0' <= line[1] && line[1] <= '9')
		rgb += line[1] - '0';
	if ('a' <= line[1] && line[1] <= 'f')
		rgb += line[1] - 'a' + 10;
	return (rgb);
}

void	colorreader(char *line, t_color *color)
{
	int		i;
	int		j;
	t_uchar	trgb[4];

	j = 1;
	i = 4;
	while (--i >= 0 + (ft_strlen(line) == 6) && line[0])
		trgb[i] = converter(line + ft_strlen(line) - 2 * j++);
	color->t = trgb[0];
	color->r = trgb[1];
	color->g = trgb[2];
	color->b = trgb[3];
}

void	gradientdef(t_cart *dot1, t_cart *dot2, t_gradient *gradient,
			t_gradient *cumgrad)
{
	t_axis	ref;

	vectorbuilder(dot2->x - dot1->x, dot2->y - dot1->y, dot2->z - dot1->z,
		&ref);
	if (ref.length < 1)
		ref.length = 1;
	gradient->t = (dot2->color.t - dot1->color.t) / ref.length;
	gradient->r = (dot2->color.r - dot1->color.r) / ref.length;
	gradient->g = (dot2->color.g - dot1->color.g) / ref.length;
	gradient->b = (dot2->color.b - dot1->color.b) / ref.length;
	cumgrad->t = 0;
	cumgrad->r = 0;
	cumgrad->g = 0;
	cumgrad->b = 0;
}

t_ui	paintgrad(t_cart *dot, t_gradient *cumgrad, t_gradient *gradient)
{
	t_color	color;

	color.t = dot->color.t + cumgrad->t;
	color.r = dot->color.r + cumgrad->r;
	color.g = dot->color.g + cumgrad->g;
	color.b = dot->color.b + cumgrad->b;
	if (color.t + gradient->t < 255 && color.t + gradient->t > 0)
		cumgrad->t += gradient->t;
	if (color.r + gradient->r < 255 && color.r + gradient->r > 0)
		cumgrad->r += gradient->r;
	if (color.g + gradient->g < 255 && color.g + gradient->g > 0)
		cumgrad->g += gradient->g;
	if (color.b + gradient->b < 255 && color.b + gradient->b > 0)
		cumgrad->b += gradient->b;
	return (paintpxl(&color));
}

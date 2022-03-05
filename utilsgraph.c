/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilsgraph.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcelsa <dcelsa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/04 02:42:45 by dcelsa            #+#    #+#             */
/*   Updated: 2022/03/04 02:42:45 by dcelsa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

t_bool	pxlinbounds(t_res *res, t_cart *dot)
{
	if (round(dot->x) < 0 || round(dot->x) > res->x - 0.5)
		return (FALSE);
	if (round(dot->y) < 0 || round(dot->y) > res->y - 0.5)
		return (FALSE);
	return (TRUE);
}

t_ui	*pxl(int x, int y, t_img *img)
{
	char	*pixel;

	pixel = img->addr + y * img->line + x * (img->bpp / 8);
	return ((t_ui *)pixel);
}

void	cartcopy(t_cart **ref, t_cart **cpy, int rows, int cols)
{
	int	i;
	int	j;

	i = -1;
	while (++i < rows)
	{
		j = -1;
		while (++j < cols)
		{
			cpy[i][j].x = ref[i][j].x;
			cpy[i][j].y = ref[i][j].y;
			cpy[i][j].z = ref[i][j].z;
			cpy[i][j].color.t = ref[i][j].color.t;
			cpy[i][j].color.r = ref[i][j].color.r;
			cpy[i][j].color.g = ref[i][j].color.g;
			cpy[i][j].color.b = ref[i][j].color.b;
		}
	}
}

void	dotalloc(t_dot *dots, char *prog, int i)
{
	dots->ref[i - 1] = perrhndlr(prog,
			malloc(sizeof(**dots->ref) * dots->cols));
	dots->dots[i - 1] = perrhndlr(prog,
			malloc(sizeof(**dots->dots) * dots->cols));
	dots->pos[i - 1] = perrhndlr(prog,
			malloc(sizeof(**dots->pos) * dots->cols));
}

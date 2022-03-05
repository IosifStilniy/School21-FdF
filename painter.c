/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   painter.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcelsa <dcelsa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/04 02:41:29 by dcelsa            #+#    #+#             */
/*   Updated: 2022/03/04 18:08:10 by dcelsa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static void	lineinit(t_res *incr, t_cart *start, t_cart *end, double *tang)
{
	incr->x = (end->x - start->x > 0.5) - (end->x - start->x < -0.5);
	incr->y = (end->y - start->y > 0.5) - (end->y - start->y < -0.5);
	*tang = INT32_MAX;
	if (incr->x)
		*tang = (end->y - start->y) / fabs(end->x - start->x);
}

static void	drawline(t_cart *start, t_cart *end, t_img *img, t_win *win)
{
	t_res	incr;
	double	tang;
	double	ref;

	lineinit(&incr, start, end, &tang);
	ref = 0;
	while (fabs(start->x - end->x) > 1.5 || fabs(start->y - end->y) > 1.5)
	{
		start->x += incr.x * (fabs(start->x - end->x) > 1.5);
		ref += tang;
		while (fabs(ref) > 0.5 && fabs(start->y - end->y) > 1.5)
		{
			start->y += incr.y;
			if (pxlinbounds(&win->res, start))
				(*pxl(round(start->x), round(start->y), img)) = paintgrad(start,
					&img->cumgrad, &img->gradient);
			ref += (ref < 0) - (ref > 0);
		}
		if (pxlinbounds(&win->res, start))
			(*pxl(round(start->x), round(start->y), img)) = paintgrad(start,
				&img->cumgrad, &img->gradient);
	}
}

static void	drawit(t_cart dot1, t_cart dot2, t_img *img, t_win *win)
{
	t_bool	dib[2];

	dib[0] = FALSE;
	dib[1] = FALSE;
	dot1.x += img->shift.sum.x;
	dot1.y += img->shift.sum.y;
	dot2.x += img->shift.sum.x;
	dot2.y += img->shift.sum.y;
	if (pxlinbounds(&win->res, &dot1) && ++dib[0])
		*pxl(round(dot1.x), round(dot1.y), img) = paintpxl(&dot1.color);
	if (fabs(dot1.x - dot2.x) < 1.5 && fabs(dot1.y - dot2.y) < 1.5)
		return ;
	if (pxlinbounds(&win->res, &dot2) && ++dib[1])
		*pxl(round(dot2.x), round(dot2.y), img) = paintpxl(&dot2.color);
	if (!dib[0] && !dib[1])
		return ;
	gradientdef(&dot1, &dot2, &img->gradient, &img->cumgrad);
	drawline(&dot1, &dot2, img, win);
}

void	paintpic(t_dot *dots, t_img *img, t_win *win, void *mlx)
{
	int		x;
	int		y;

	if (!dots->cols && !dots->rows)
		return ;
	y = -1;
	while (++y < dots->rows)
	{
		x = -1;
		while (++x < dots->cols)
		{
			drawit(dots->pos[y][x], dots->pos[y][x], img, win);
			if (x)
				drawit(dots->pos[y][x], dots->pos[y][x - 1], img, win);
			if (y)
				drawit(dots->pos[y][x], dots->pos[y - 1][x], img, win);
		}
	}
	mlx_put_image_to_window(mlx, win->win, img->img, 0, 0);
	mlx_string_put(mlx, win->win, 0.9 * win->res.x, 0.05 * win->res.y,
		0x00FFFF00, LGNDNT);
}

void	imgdefiner(t_img *img, t_win *win, void *mlx)
{
	mlx_destroy_image(mlx, img->img);
	img->shift.crdstm.x *= win->view.scale.cur / win->view.scale.old;
	img->shift.crdstm.y *= win->view.scale.cur / win->view.scale.old;
	if (win->view.scale.old < 0.001)
	{
		img->shift.crdstm.x = 0;
		img->shift.crdstm.y = 0;
	}
	img->shift.sum.x = win->cntr.x + img->shift.crdstm.x;
	img->shift.sum.y = win->cntr.y + img->shift.crdstm.y;
	win->view.scale.old = win->view.scale.cur;
	img->img = mlx_new_image(mlx, win->res.x, win->res.y);
	img->addr = mlx_get_data_addr(img->img, &img->bpp, &img->line,
			&img->endian);
}

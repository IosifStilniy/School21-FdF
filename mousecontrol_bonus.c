/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mousecontrol_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcelsa <dcelsa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/04 02:40:06 by dcelsa            #+#    #+#             */
/*   Updated: 2022/03/04 17:26:16 by dcelsa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void	mouserotating(t_cntrl *cntrl, int x, int y)
{
	vectorbuilder(x, y, cntrl->win->view.dstnc, &cntrl->mouse.vpos.v2);
	if (cntrl->keybrd.zrot)
		vectorbuilder(x, y, 0, &cntrl->mouse.vpos.v2);
	axisbuilder(&cntrl->mouse.vpos.v2, &cntrl->mouse.vpos.v1,
		&cntrl->win->view.axis);
	if (cntrl->keybrd.zrot)
		axisbuilder(&cntrl->mouse.vpos.v1, &cntrl->mouse.vpos.v2,
			&cntrl->win->view.axis);
	cntrl->win->view.axis.ang *= 3;
	engine(cntrl->win, cntrl->dots);
	imgdefiner(cntrl->img, cntrl->win, cntrl->mlx);
	paintpic(cntrl->dots, cntrl->img, cntrl->win, cntrl->mlx);
	vectorbuilder(cntrl->mouse.vpos.v2.vector.x, cntrl->mouse.vpos.v2.vector.y,
		cntrl->mouse.vpos.v2.vector.z, &cntrl->mouse.vpos.v1);
}

void	mouseshifting(t_cntrl *cntrl, int x, int y)
{
	engine(cntrl->win, cntrl->dots);
	imgdefiner(cntrl->img, cntrl->win, cntrl->mlx);
	paintpic(cntrl->dots, cntrl->img, cntrl->win, cntrl->mlx);
	cntrl->img->shift.crdstm.x = x - cntrl->mouse.pos.x;
	cntrl->img->shift.crdstm.y = y - cntrl->mouse.pos.y;
}

void	mousezooming(t_cntrl *cntrl, int y)
{
	if (cntrl->keybrd.zoom)
	{
		cntrl->win->view.scale.old = cntrl->win->view.scale.cur;
		cntrl->win->view.scale.cur += cntrl->mouse.pos.y - y
			+ cntrl->img->shift.crdstm.y;
	}
	if (cntrl->keybrd.focus)
		cntrl->win->view.focus += cntrl->mouse.pos.y - y
			+ cntrl->img->shift.crdstm.y;
	cntrl->win->view.scale.cur *= (cntrl->win->view.scale.cur > 0);
	cntrl->win->view.focus *= (cntrl->win->view.focus > 0);
	engine(cntrl->win, cntrl->dots);
	imgdefiner(cntrl->img, cntrl->win, cntrl->mlx);
	paintpic(cntrl->dots, cntrl->img, cntrl->win, cntrl->mlx);
	cntrl->mouse.pos.y = y - cntrl->img->shift.crdstm.y;
}

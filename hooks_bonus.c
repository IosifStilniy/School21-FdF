/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcelsa <dcelsa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/04 02:40:40 by dcelsa            #+#    #+#             */
/*   Updated: 2022/03/04 17:14:56 by dcelsa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

int	keydownhndlr(int keycode, t_cntrl *cntrl)
{
	if (keycode == K_CMND)
		cntrl->keybrd.zrot = (!cntrl->keybrd.zrot);
	if (keycode == K_SHIFT)
		cntrl->keybrd.zoom = (!cntrl->keybrd.zoom);
	if (keycode == K_CNTRL)
		cntrl->keybrd.focus = (!cntrl->keybrd.focus);
	keyshifting(keycode, cntrl);
	keyrotating(keycode, cntrl);
	return (0);
}

int	keyuphndlr(int keycode, t_cntrl *cntrl)
{
	if (keycode == K_CMND)
		cntrl->keybrd.zrot = (!cntrl->keybrd.zrot);
	if (keycode == K_SHIFT)
		cntrl->keybrd.zoom = (!cntrl->keybrd.zoom);
	if (keycode == K_CNTRL)
		cntrl->keybrd.focus = (!cntrl->keybrd.focus);
	if (keycode == K_ESC)
		exit(0);
	if (keycode == K_I)
	{
		mlx_destroy_image(cntrl->mlx, cntrl->img->img);
		restore(cntrl);
		paintpic(cntrl->dots, cntrl->img, cntrl->win, cntrl->mlx);
	}
	if (keycode == K_P)
	{
		cntrl->win->view.perspective = (!cntrl->win->view.perspective);
		engine(cntrl->win, cntrl->dots);
		imgdefiner(cntrl->img, cntrl->win, cntrl->mlx);
		paintpic(cntrl->dots, cntrl->img, cntrl->win, cntrl->mlx);
	}
	if (keycode == K_L)
		cntrl->keybrd.legend = (!cntrl->keybrd.legend);
	return (0);
}

int	btnpress(int btn, int x, int y, t_cntrl *cntrl)
{
	cntrl->mouse.pos.x = x - cntrl->img->shift.crdstm.x;
	cntrl->mouse.pos.y = y - cntrl->img->shift.crdstm.y;
	if (btn == LMB)
	{
		cntrl->mouse.pos.x -= cntrl->win->cntr.x;
		cntrl->mouse.pos.y -= cntrl->win->cntr.y;
		cntrl->mouse.rot = (!cntrl->mouse.rot);
		vectorbuilder(cntrl->mouse.pos.x, cntrl->mouse.pos.y,
			cntrl->win->view.dstnc, &cntrl->mouse.vpos.v1);
		if (cntrl->keybrd.zrot)
			vectorbuilder(cntrl->mouse.pos.x, cntrl->mouse.pos.y, 0,
				&cntrl->mouse.vpos.v1);
	}
	if (btn == MMB)
		cntrl->mouse.shift = (!cntrl->mouse.shift);
	if (btn == SCRL_UP || btn == SCRL_DOWN)
		scrolling(btn, cntrl);
	return (0);
}

int	btnup(int btn, int x, int y, t_cntrl *cntrl)
{
	x++;
	y++;
	if (btn == MMB)
		cntrl->mouse.shift = (!cntrl->mouse.shift);
	if (btn == LMB)
		cntrl->mouse.rot = (!cntrl->mouse.rot);
	return (0);
}

int	mousemove(int x, int y, t_cntrl *cntrl)
{
	if (!cntrl->keybrd.zoom && !cntrl->keybrd.focus && cntrl->mouse.shift)
		mouseshifting(cntrl, x, y);
	if (cntrl->mouse.rot)
	{
		x -= cntrl->img->shift.crdstm.x + cntrl->win->cntr.x;
		y -= cntrl->img->shift.crdstm.y + cntrl->win->cntr.y;
		mouserotating(cntrl, x, y);
	}
	if ((cntrl->keybrd.zoom || cntrl->keybrd.focus) && cntrl->mouse.shift)
		mousezooming(cntrl, y);
	return (0);
}

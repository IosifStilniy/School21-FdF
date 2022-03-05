/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keycontrol_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcelsa <dcelsa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/04 02:43:02 by dcelsa            #+#    #+#             */
/*   Updated: 2022/03/04 02:43:11 by dcelsa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void	keyshifting(int keycode, t_cntrl *cntrl)
{
	t_bool	shift;

	shift = FALSE;
	if (keycode == K_W && ++shift)
		cntrl->img->shift.crdstm.y -= SHIFT_SPEED;
	if (keycode == K_S && ++shift)
		cntrl->img->shift.crdstm.y += SHIFT_SPEED;
	if (keycode == K_A && ++shift)
		cntrl->img->shift.crdstm.x -= SHIFT_SPEED;
	if (keycode == K_D && ++shift)
		cntrl->img->shift.crdstm.x += SHIFT_SPEED;
	if (!shift)
		return ;
	engine(cntrl->win, cntrl->dots);
	imgdefiner(cntrl->img, cntrl->win, cntrl->mlx);
	paintpic(cntrl->dots, cntrl->img, cntrl->win, cntrl->mlx);
}

static t_bool	rothndlr(int keycode, t_win *win, t_keybrd *keybrd, t_dot *dots)
{
	t_bool	rot;

	rot = FALSE;
	if (keycode == K_UP && ++rot)
		vectorbuilder(-1, 0, 0, &win->view.axis);
	if (keycode == K_DOWN && ++rot)
		vectorbuilder(1, 0, 0, &win->view.axis);
	if (keycode == K_LEFT && !keybrd->zrot && ++rot)
		vectorbuilder(0, 1, 0, &win->view.axis);
	if (keycode == K_RIGHT && !keybrd->zrot && ++rot)
		vectorbuilder(0, -1, 0, &win->view.axis);
	if (keycode == K_LEFT && keybrd->zrot && ++rot)
		vectorbuilder(0, 0, -1, &win->view.axis);
	if (keycode == K_RIGHT && keybrd->zrot && ++rot)
		vectorbuilder(0, 0, 1, &win->view.axis);
	if (keycode == K_Q && ++rot)
		vectorbuilder(dots->norm.vector.x, -dots->norm.vector.y,
			-dots->norm.vector.z, &win->view.axis);
	if (keycode == K_E && ++rot)
		vectorbuilder(dots->norm.vector.x, dots->norm.vector.y,
			dots->norm.vector.z, &win->view.axis);
	return (rot);
}

void	keyrotating(int keycode, t_cntrl *cntrl)
{
	if (!rothndlr(keycode, cntrl->win, &cntrl->keybrd, cntrl->dots))
		return ;
	engine(cntrl->win, cntrl->dots);
	imgdefiner(cntrl->img, cntrl->win, cntrl->mlx);
	paintpic(cntrl->dots, cntrl->img, cntrl->win, cntrl->mlx);
}

void	scrolling(int btn, t_cntrl *cntrl)
{
	if (btn == SCRL_UP && cntrl->win->view.scale.cur > 0.9 * cntrl->win->res.y)
		return ;
	cntrl->win->view.scale.old = cntrl->win->view.scale.cur;
	if (!cntrl->win->view.scale.cur && btn == SCRL_UP)
		cntrl->win->view.scale.cur = 0.1;
	cntrl->win->view.scale.cur *= 1 - 0.1 * (btn != SCRL_UP)
		+ 0.1 * (btn == SCRL_UP);
	cntrl->win->view.scale.cur *= (cntrl->win->view.scale.cur > 0);
	engine(cntrl->win, cntrl->dots);
	imgdefiner(cntrl->img, cntrl->win, cntrl->mlx);
	paintpic(cntrl->dots, cntrl->img, cntrl->win, cntrl->mlx);
}

int	finish(void)
{
	exit(0);
}

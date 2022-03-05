/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcelsa <dcelsa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/04 02:42:38 by dcelsa            #+#    #+#             */
/*   Updated: 2022/03/04 17:15:37 by dcelsa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void	*perrhndlr(char *prog, void *p)
{
	if (p)
		return (p);
	perror(prog);
	exit(1);
}

int	errhndlr(char *prog, int funcres)
{
	if (funcres >= 0)
		return (funcres);
	perror(prog);
	exit(1);
}

void	customerr(char *prog, char *msg)
{
	ft_printf("%s: %s\n", prog, msg);
	exit (1);
}

void	charcleaner(char **lines)
{
	int	i;

	i = -1;
	while (lines && lines[++i])
		free(lines[i]);
	free(lines);
}

void	restorekeys(t_win *win, t_mouse *mouse, t_keybrd *keybrd, t_dot *dots)
{
	win->view.scale.def = win->res.y / dots->diag.length;
	win->view.scale.cur = win->view.scale.def;
	win->view.scale.old = win->view.scale.def;
	win->view.focus = dots->diag.length * win->view.scale.cur;
	win->view.dstnc = win->view.focus;
	win->view.perspective = FALSE;
	win->view.legend = FALSE;
	mouse->shift = FALSE;
	mouse->rot = FALSE;
	keybrd->zoom = FALSE;
	keybrd->zrot = FALSE;
	keybrd->legend = FALSE;
}

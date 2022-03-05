/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcelsa <dcelsa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/04 02:40:31 by dcelsa            #+#    #+#             */
/*   Updated: 2022/03/04 17:27:00 by dcelsa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static t_win	*wininit(char **argv, void *mlx)
{
	t_win	*win;
	char	*title;
	char	*buf;

	win = perrhndlr(argv[0], malloc(sizeof(*win)));
	win->res.x = RESX;
	win->res.y = RESY;
	buf = perrhndlr(argv[0], ft_strjoin(argv[0], ": "));
	title = perrhndlr(argv[0], ft_strjoin(buf, argv[1]));
	free(buf);
	win->win = mlx_new_window(mlx, RESX, RESY, title);
	free(title);
	win->cntr.x = win->res.x / 2;
	win->cntr.y = win->res.y / 2;
	return (win);
}

static int	keyup(int keycode)
{
	if (keycode == K_ESC)
		exit(0);
	return (0);
}

static int	finishhim(void)
{
	exit(0);
}

void	restore(t_cntrl *cntrl)
{
	cntrl->img->shift.crdstm.x = 0;
	cntrl->img->shift.crdstm.y = 0;
	cntrl->img->shift.sum.x = cntrl->win->cntr.x;
	cntrl->img->shift.sum.y = cntrl->win->cntr.y;
	restorekeys(cntrl->win, &cntrl->mouse, &cntrl->keybrd, cntrl->dots);
	cntrl->img->img = mlx_new_image(cntrl->mlx,
			cntrl->win->res.x, cntrl->win->res.y);
	cntrl->img->addr = mlx_get_data_addr(cntrl->img->img,
			&cntrl->img->bpp, &cntrl->img->line, &cntrl->img->endian);
	cartcopy(cntrl->dots->ref, cntrl->dots->dots,
		cntrl->dots->rows, cntrl->dots->cols);
	cartcopy(cntrl->dots->ref, cntrl->dots->pos,
		cntrl->dots->rows, cntrl->dots->cols);
	vectorbuilder(0, 0, 1, &cntrl->win->view.axis);
	cntrl->win->view.axis.ang = M_PI_4;
	engine(cntrl->win, cntrl->dots);
	vectorbuilder(-1, 0, 0, &cntrl->win->view.axis);
	cntrl->win->view.axis.ang = M_PI_4;
	engine(cntrl->win, cntrl->dots);
}

int	main(int argc, char **argv)
{
	t_cntrl	cntrl;

	argv[0] = ft_strrchr(argv[0], '/') + 1;
	if (argc == 1)
		customerr(argv[0], "Need file with .fdf extension");
	cntrl.prog = argv[0];
	cntrl.mlx = mlx_init();
	cntrl.dots = mapbuilder(argv[1], argv[0]);
	if (ft_strrchr(argv[1], '/'))
		argv[1] = ft_strrchr(argv[1], '/') + 1;
	cntrl.win = wininit(argv, cntrl.mlx);
	cntrl.img = perrhndlr(argv[0], malloc(sizeof(*cntrl.img)));
	restore(&cntrl);
	paintpic(cntrl.dots, cntrl.img, cntrl.win, cntrl.mlx);
	mlx_hook(cntrl.win->win, 3, 1L << 1, &keyup, NULL);
	mlx_hook(cntrl.win->win, 17, 0, &finishhim, NULL);
	mlx_loop(cntrl.mlx);
	return (0);
}

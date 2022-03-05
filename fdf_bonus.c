/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf_bonus.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcelsa <dcelsa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/04 02:41:04 by dcelsa            #+#    #+#             */
/*   Updated: 2022/03/04 17:14:15 by dcelsa           ###   ########.fr       */
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

static int	legend(t_cntrl *cntrl)
{
	if (!cntrl->keybrd.legend && cntrl->win->view.legend)
	{
		cntrl->win->view.legend = FALSE;
		mlx_put_image_to_window(cntrl->mlx, cntrl->win->win, cntrl->img->img,
			0, 0);
		mlx_string_put(cntrl->mlx, cntrl->win->win, 0.9 * cntrl->win->res.x,
			0.05 * cntrl->win->res.y, 0x00FFFF00, LGNDNT);
	}
	if (!cntrl->keybrd.legend)
		return (0);
	if (!cntrl->win->view.legend)
		cntrl->win->view.legend = TRUE;
	mlx_put_image_to_window(cntrl->mlx, cntrl->win->win, cntrl->legend->img,
		cntrl->lgndpos.x, cntrl->lgndpos.y);
	return (0);
}

static void	pudge(t_cntrl *cntrl)
{
	mlx_hook(cntrl->win->win, 2, 1L << 0, &keydownhndlr, cntrl);
	mlx_hook(cntrl->win->win, 3, 1L << 1, &keyuphndlr, cntrl);
	mlx_hook(cntrl->win->win, 4, 1L << 2, &btnpress, cntrl);
	mlx_hook(cntrl->win->win, 5, 1L << 3, &btnup, cntrl);
	mlx_hook(cntrl->win->win, 6, 1L << 6, &mousemove, cntrl);
	mlx_hook(cntrl->win->win, 17, 0, &finish, NULL);
	mlx_loop_hook(cntrl->mlx, &legend, cntrl);
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
	cntrl.legend = perrhndlr(argv[0], malloc(sizeof(*cntrl.legend)));
	cntrl.legend->img = mlx_xpm_file_to_image(cntrl.mlx, "legend.xpm",
			&cntrl.legend->res.x, &cntrl.legend->res.y);
	cntrl.lgndpos.x = cntrl.win->cntr.x - cntrl.legend->res.x / 2;
	cntrl.lgndpos.y = cntrl.win->cntr.y - cntrl.legend->res.y / 2;
	restore(&cntrl);
	pudge(&cntrl);
	paintpic(cntrl.dots, cntrl.img, cntrl.win, cntrl.mlx);
	mlx_loop(cntrl.mlx);
	return (0);
}

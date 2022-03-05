/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcelsa <dcelsa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/04 02:40:17 by dcelsa            #+#    #+#             */
/*   Updated: 2022/03/04 17:14:00 by dcelsa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FDF_H
# define FDF_H
# include <mlx.h>
# include <math.h>
# include <float.h>
# include <fcntl.h>
# include <stdio.h>
# include <sys/errno.h>
# include "ft_printf_bonus.h"
# include "get_next_line.h"

# ifndef TRUE
#  define TRUE 1
# endif

# ifndef FALSE
#  define FALSE 0
# endif

# ifndef RESX
#  define RESX 1920
# endif

# ifndef RESY
#  define RESY 1080
# endif

# ifndef OFST
#  define OFST 0.05
# endif

# ifndef CLEDGE
#  define CLEDGE 10
# endif

# ifndef SHIFT_SPEED
#  define SHIFT_SPEED 30
# endif

# ifndef DEFANG
#  define DEFANG 15
# endif

# ifndef DEFCLR
#  define DEFCLR "00FFFFFF\0"
# endif

# ifndef LGNDNT
#  define LGNDNT "L - show/hide legend"
# endif

# ifndef K_ESC
#  define K_ESC 53
# endif

# ifndef K_UP
#  define K_UP 126
# endif

# ifndef K_DOWN
#  define K_DOWN 125
# endif

# ifndef K_LEFT
#  define K_LEFT 123
# endif

# ifndef K_RIGHT
#  define K_RIGHT 124
# endif

# ifndef K_Q
#  define K_Q 12
# endif

# ifndef K_E
#  define K_E 14
# endif

# ifndef K_W
#  define K_W 13
# endif

# ifndef K_S
#  define K_S 1
# endif

# ifndef K_A
#  define K_A 0
# endif

# ifndef K_D
#  define K_D 2
# endif

# ifndef K_I
#  define K_I 34
# endif

# ifndef K_P
#  define K_P 35
# endif

# ifndef K_L
#  define K_L 37
# endif

# ifndef K_CMND
#  define K_CMND 259
# endif

# ifndef K_SHIFT
#  define K_SHIFT 257
# endif

# ifndef K_CNTRL
#  define K_CNTRL 256
# endif

# ifndef LMB
#  define LMB 1
# endif

# ifndef MMB
#  define MMB 3
# endif

# ifndef SCRL_UP
#  define SCRL_UP 5
# endif

# ifndef SCRL_DOWN
#  define SCRL_DOWN 4
# endif

typedef int				t_bool;
typedef unsigned int	t_ui;
typedef unsigned char	t_uchar;

typedef struct s_res {
	int	x;
	int	y;
}	t_res;

typedef struct s_color {
	t_uchar	t;
	t_uchar	r;
	t_uchar	g;
	t_uchar	b;
}	t_color;

typedef struct s_cart {
	double	x;
	double	y;
	double	z;
	t_color	color;
}	t_cart;

typedef struct s_axis {
	t_cart	vector;
	double	length;
	double	ang;
}	t_axis;

typedef struct s_scale {
	double	def;
	double	cur;
	double	old;
}	t_scale;

typedef struct s_gradient {
	double	t;
	double	r;
	double	g;
	double	b;
}	t_gradient;

typedef struct s_view {
	t_scale		scale;
	t_res		poscrd;
	t_axis		axis;
	double		dstnc;
	double		focus;
	t_bool		perspective;
	t_bool		legend;
}	t_view;

typedef struct s_shift {
	t_res	crdstm;
	t_res	sum;
}	t_shift;

typedef struct s_img {
	void		*img;
	char		*addr;
	t_shift		shift;
	t_gradient	gradient;
	t_gradient	cumgrad;
	t_res		res;
	int			bpp;
	int			line;
	int			endian;
}	t_img;

typedef struct s_win {
	void	*win;
	t_res	res;
	t_res	cntr;
	t_view	view;
}	t_win;

typedef struct s_dot {
	t_cart	**ref;
	t_cart	**dots;
	t_cart	**pos;
	t_cart	cntr;
	t_axis	diag;
	t_axis	norm;
	int		unit;
	int		rows;
	int		cols;
}	t_dot;

typedef struct s_trnaxs {
	t_axis	v1;
	t_axis	v2;
}	t_trnaxs;

typedef struct s_mouse {
	t_bool		shift;
	t_bool		rot;
	t_res		pos;
	t_trnaxs	vpos;
}	t_mouse;

typedef struct s_keybrd {
	t_bool	zrot;
	t_bool	zoom;
	t_bool	focus;
	t_bool	legend;
}	t_keybrd;

typedef struct s_cntrl {
	t_dot		*dots;
	t_win		*win;
	t_img		*img;
	void		*mlx;
	t_img		*legend;
	t_res		lgndpos;
	t_mouse		mouse;
	t_keybrd	keybrd;
	char		*prog;
}	t_cntrl;

void	customerr(char *prog, char *msg);
void	*perrhndlr(char *prog, void *p);
int		errhndlr(char *prog, int funcres);
t_dot	*mapbuilder(char *file, char *prog);
void	dotalloc(t_dot *dots, char *prog, int i);
void	charcleaner(char **lines);
void	restore(t_cntrl *cntrl);
void	restorekeys(t_win *win, t_mouse *mouse, t_keybrd *keybrd, t_dot *dots);
void	axisbuilder(t_axis *v1, t_axis *v2, t_axis *axis);
void	vectorbuilder(double x, double y, double z, t_axis *vector);
void	engine(t_win *win, t_dot *dots);
void	cartcopy(t_cart **ref, t_cart **cpy, int rows, int cols);
void	paintpic(t_dot *dots, t_img *img, t_win *win, void *mlx);
void	imgdefiner(t_img *img, t_win *win, void *mlx);
t_ui	paintpxl(t_color *color);
t_ui	paintgrad(t_cart *dot, t_gradient *cumgrad, t_gradient *gradient);
void	gradientdef(t_cart *dot1, t_cart *dot2, t_gradient *gradient,
			t_gradient *cumgrad);
void	colorreader(char *line, t_color *color);
t_ui	*pxl(int x, int y, t_img *img);
t_bool	pxlinbounds(t_res *res, t_cart *dot);
void	bounddefiner(t_res *winres, t_img *img);
int		keydownhndlr(int keycode, t_cntrl *cntrl);
int		keyuphndlr(int keycode, t_cntrl *cntrl);
int		btnpress(int btn, int x, int y, t_cntrl *cntrl);
int		btnup(int btn, int x, int y, t_cntrl *cntrl);
int		finish(void);
int		mousemove(int x, int y, t_cntrl *cntrl);
void	keyshifting(int keycode, t_cntrl *cntrl);
void	keyrotating(int keycode, t_cntrl *cntrl);
void	scrolling(int btn, t_cntrl *cntrl);
void	mouserotating(t_cntrl *cntrl, int x, int y);
void	mouseshifting(t_cntrl *cntrl, int x, int y);
void	mousezooming(t_cntrl *cntrl, int y);

#endif
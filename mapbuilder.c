/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mapbuilder.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcelsa <dcelsa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/04 02:42:53 by dcelsa            #+#    #+#             */
/*   Updated: 2022/03/04 02:43:00 by dcelsa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static void	cartinit(t_dot *dots)
{
	int	i;
	int	j;

	dots->cntr.z = 0;
	dots->cntr.x = dots->cols / 2;
	dots->cntr.y = dots->rows / 2;
	dots->diag.vector.z = 0;
	vectorbuilder(0, 0, 1, &dots->norm);
	i = -1;
	while (++i < dots->rows)
	{
		j = -1;
		while (++j < dots->cols)
		{
			dots->ref[i][j].x = j - dots->cntr.x;
			dots->ref[i][j].y = i - dots->cntr.y;
			dots->ref[i][j].z /= dots->unit;
			if (dots->ref[i][j].z > dots->diag.vector.z)
				dots->diag.vector.z = dots->ref[i][j].z;
		}
	}
	vectorbuilder(dots->cols, dots->rows, dots->diag.vector.z, &dots->diag);
}

static int	numchecker(char *line, t_color *color, char *prog)
{
	int	j;
	int	num;
	int	cpy;

	num = ft_atoi(line);
	cpy = num;
	if (ft_strchr(line, ','))
	{
		colorreader(ft_strchr(line, 'x') + 1, color);
		*ft_strchr(line, ',') = '\0';
	}
	j = 1 + (num < -0.001);
	while (abs(cpy) / 10 > 0 && ++j)
		cpy /= 10;
	if (j != (int)ft_strlen(line))
	{
		line = ft_strjoin("Invalid map: dots must be int: ", line);
		customerr(prog, line);
	}
	return (-num);
}

static int	dotfiller(t_cart *dots, int *unit, char **line, char *prog)
{
	int	i;

	i = -1;
	while (line[++i])
	{
		if (!dots)
			continue ;
		if (line[i][0] == '\n')
			continue ;
		if (line[i][ft_strlen(line[i]) - 1] == '\n')
			line[i][ft_strlen(line[i]) - 1] = '\0';
		colorreader(DEFCLR, &dots[i].color);
		dots[i].z = numchecker(line[i], &dots[i].color, prog);
		if (fabs(dots[i].z) > 0.001 && fabs(dots[i].z) < *unit)
			*unit = round(fabs(dots[i].z));
	}
	if (line[i - 1][0] == '\n')
		i--;
	return (i);
}

static int	parser(int fd, char *prog, t_dot *dots)
{
	char	**line;
	char	*buf;
	int		i;

	i = 0;
	buf = prog;
	while (buf && ++i)
	{
		buf = get_next_line(fd);
		line = ft_split(buf, ' ');
		free(buf);
		if (!buf || !dots)
			charcleaner(line);
		if ((!buf && i--) || !dots)
			continue ;
		dots->cols += (!dots->cols) * dotfiller(NULL, NULL, line, prog);
		if (dots->cols != dotfiller(NULL, NULL, line, prog))
			customerr(prog, "Invalid map: number of columns must be the same");
		dotalloc(dots, prog, i);
		dotfiller(dots->ref[i - 1], &dots->unit, line, prog);
		charcleaner(line);
	}
	return (i);
}

t_dot	*mapbuilder(char *file, char *prog)
{
	t_dot	*dots;
	int		fd;

	if ((!ft_strnstr(file, ".fdf", -1)
			|| ft_strlen(ft_strrchr(file, '.')) != 4))
		customerr(prog, "Need file with .fdf extension");
	fd = errhndlr(prog, open(file, O_RDONLY));
	dots = perrhndlr(prog, malloc(sizeof(*dots)));
	dots->rows = parser(fd, prog, NULL);
	if (!dots->rows)
		customerr(prog, "Invalid map: need at least 1 row");
	dots->ref = perrhndlr(prog, malloc(sizeof(*dots->dots) * dots->rows));
	dots->dots = perrhndlr(prog, malloc(sizeof(*dots->dots) * dots->rows));
	dots->pos = perrhndlr(prog, malloc(sizeof(*dots->pos) * dots->rows));
	errhndlr(prog, close(fd));
	fd = errhndlr(prog, open(file, O_RDONLY));
	dots->cols = 0;
	dots->unit = INT32_MAX;
	parser(fd, prog, dots);
	cartinit(dots);
	return (dots);
}

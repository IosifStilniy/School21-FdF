/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   engine.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcelsa <dcelsa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/04 02:39:57 by dcelsa            #+#    #+#             */
/*   Updated: 2022/03/04 17:15:24 by dcelsa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static void	quartrot(t_cart *pos, t_axis *axis)
{	
	double	mlts[3][3];
	t_cart	axsr;

	axsr.x = sin(axis->ang / 2) * axis->vector.x;
	axsr.y = sin(axis->ang / 2) * axis->vector.y;
	axsr.z = sin(axis->ang / 2) * axis->vector.z;
	mlts[0][0] = (1 - 2 * (pow(axsr.y, 2) + pow(axsr.z, 2))) * pos->x;
	mlts[0][1] = (2 * (axsr.x * axsr.y - axsr.z * cos(axis->ang / 2))) * pos->y;
	mlts[0][2] = (2 * (axsr.x * axsr.z + axsr.y * cos(axis->ang / 2))) * pos->z;
	mlts[1][0] = (2 * (axsr.x * axsr.y + axsr.z * cos(axis->ang / 2))) * pos->x;
	mlts[1][1] = (1 - 2 * (pow(axsr.x, 2) + pow(axsr.z, 2))) * pos->y;
	mlts[1][2] = (2 * (axsr.y * axsr.z - axsr.x * cos(axis->ang / 2))) * pos->z;
	mlts[2][0] = (2 * (axsr.x * axsr.z - axsr.y * cos(axis->ang / 2))) * pos->x;
	mlts[2][1] = (2 * (axsr.y * axsr.z + axsr.x * cos(axis->ang / 2))) * pos->y;
	mlts[2][2] = (1 - 2 * (pow(axsr.x, 2) + pow(axsr.y, 2))) * pos->z;
	pos->x = mlts[0][0] + mlts[0][1] + mlts[0][2];
	pos->y = mlts[1][0] + mlts[1][1] + mlts[1][2];
	pos->z = mlts[2][0] + mlts[2][1] + mlts[2][2];
}

void	engine(t_win *win, t_dot *dots)
{
	int		i;
	int		j;

	quartrot(&dots->norm.vector, &win->view.axis);
	i = -1;
	while (++i < dots->rows)
	{
		j = -1;
		while (++j < dots->cols)
		{
			quartrot(&dots->dots[i][j], &win->view.axis);
			dots->pos[i][j].x = dots->dots[i][j].x * win->view.scale.cur;
			dots->pos[i][j].y = dots->dots[i][j].y * win->view.scale.cur;
			dots->pos[i][j].z = dots->dots[i][j].z * win->view.scale.cur;
			if (win->view.perspective)
			{
				dots->pos[i][j].x *= win->view.focus
					/ (win->view.dstnc + dots->pos[i][j].z);
				dots->pos[i][j].y *= win->view.focus
					/ (win->view.dstnc + dots->pos[i][j].z);
			}
		}
	}
	win->view.axis.ang = 0;
}

void	vectorbuilder(double x, double y, double z, t_axis *vector)
{
	vector->length = sqrt(x * x + y * y + z * z);
	vector->vector.x = x / vector->length;
	vector->vector.y = y / vector->length;
	vector->vector.z = z / vector->length;
	if (!vector->ang)
		vector->ang = DEFANG * M_PI / 180;
}

void	axisbuilder(t_axis *v1, t_axis *v2, t_axis *axis)
{
	double	scalar;

	axis->vector.x = v1->vector.y * v2->vector.z - v1->vector.z * v2->vector.y;
	axis->vector.y = v1->vector.z * v2->vector.x - v1->vector.x * v2->vector.z;
	axis->vector.z = v1->vector.x * v2->vector.y - v1->vector.y * v2->vector.x;
	axis->length = sqrt(pow(axis->vector.x, 2) + pow(axis->vector.y, 2)
			+ pow(axis->vector.z, 2));
	scalar = axis->length;
	scalar /= (scalar * (scalar >= 1) + (scalar < 1));
	axis->ang = asin(scalar);
	scalar = v1->vector.x * v2->vector.x + v1->vector.y * v2->vector.y
		+ v1->vector.z * v2->vector.z;
	if (scalar < 0)
		axis->ang = M_PI - axis->ang;
	if (!axis->length)
		return ;
	axis->vector.x /= axis->length;
	axis->vector.y /= axis->length;
	axis->vector.z /= axis->length;
}

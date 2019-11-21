/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   app.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpoetess <rpoetess@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 21:41:50 by rpoetess          #+#    #+#             */
/*   Updated: 2019/11/21 21:43:32 by rpoetess         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"
#include "mlx.h"

/*
**https://github.com/keuhdall/images_example
*/

void	t_framebuffer_init(t_framebuffer *fb, void *mlx, int w, int h)
{
	fb->image = mlx_new_image(mlx, WIN_W, WIN_H);
	fb->data = (uint *)mlx_get_data_addr(
			fb->image,
			&fb->bpp,
			&fb->row_len,
			&fb->endian);
	fb->bpp /= 8;
	fb->row_len /= fb->bpp;
	fb->w = w;
	fb->h = h;
}

void	t_framebuffer_clear(t_framebuffer *fb)
{
	(void)fb;
	ft_bzero(fb->data, fb->row_len * WIN_H * sizeof(int));
}

void	t_framebuffer_upscale(t_framebuffer *fb, int scale)
{
	int i;
	int j;

	i = fb->w;
	while (--i >= 0)
	{
		j = fb->h;
		while (--j >= 0)
		{
			fb->data[fb->row_len * j + i] = fb->data[fb->row_len * (j / scale) + i / scale];
		}
	}
}

void	t_app_init(t_app *app)
{
	ft_bzero(app, sizeof(t_app));
	app->map_name = "maps/pylone.fdf";
	app->w = WIN_W;
	app->h = WIN_H;
	app->sidebar_w = 200;
	app->M = mlx_init();
	app->win = mlx_new_window(app->M, app->sidebar_w + app->w, app->h, "fdf");
	t_framebuffer_init(&app->framebuffer, app->M, app->w, app->h);
	app->objs = malloc(sizeof(t_mesh *) * 2);
	app->objs[0] = malloc(sizeof(t_mesh));
	app->objs[1] = 0;
	*(app->objs[0]) = t_mesh_landscape_from_file(app->map_name);
	app->controller.projection = 1;
	t_cam_init(&app->cam, (t_point){WIN_W, WIN_H});
}

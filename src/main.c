/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpoetess <rpoetess@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 21:32:59 by rpoetess          #+#    #+#             */
/*   Updated: 2019/11/21 21:38:44 by rpoetess         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mlx.h"
#include "libft.h"
#include "fdf.h"
#include <time.h>
#include "ft_linalg.h"

int		close_hook(void *param)
{
	(void)param;
	exit(0);
}

void	update_debug(t_app *app, double dt)
{
	int				i;
	static double	a;
	double			dx;
	double			dy;

	a = 0.0002;
	(void)dt;
	t_framebuffer_clear(&app->framebuffer);
	line(app,
		(t_vec){1., 1., 0},
		(t_vec){3., 1, 0}, WHITE);
	i = 0;
	while (i < 360)
	{
		dx = 34. * ft_cos(a + radians(1. * i));
		dy = 34. * ft_sin(a + radians(1. * i));
		line(app,
			(t_vec){50., 50., 0},
			(t_vec){50. + dx, 50. + dy, 0}, WHITE);
		line(app,
			(t_vec){50. + dx, 50. + dy, 0},
			(t_vec){50. + 1.4 * dx, 50. + 1.4 * dy, 0}, WHITE);
		i += 5;
	}
	t_framebuffer_upscale(&app->framebuffer, 8);
	mlx_put_image_to_window(app->M, app->win, app->framebuffer.image, 0, 0);
}

void	update(t_app *app, double dt)
{
	int		i;
	t_mesh	*obj;

	(void)dt;
	i = -1;
	t_framebuffer_clear(&app->framebuffer);
	if (app->controller.status_prj &&
		app->controller.status_prj != app->controller.projection)
		ft_change_projection(app);
	t_cam_move(&app->cam, &app->controller);
	while ((obj = app->objs[++i]))
	{
		t_cam_draw(&app->cam, app, obj);
	}
		mlx_put_image_to_window(app->M, app->win,
			app->framebuffer.image, app->sidebar_w, 0);
}

int		loop_hook(void *p)
{
	t_app	*app;
	double	dt;

	app = p;
	app->time = clock();
	dt = (double)(app->time - app->frame_time) / CLOCKS_PER_SEC;
	if (dt > FRAME_TIME)
	{
		app->frame_time = app->time;
		update(app, dt);
	}
	return (0);
}

int		mouse_hook(int button, int x, int y, void *param)
{
	(void)param;
	(void)button;
	ft_printf("mouse: %d %d\n", x, y);
	return (0);
}

int		main(int argc, char **argv)
{
	t_app	app;

	t_app_init(&app);
	print_info(&app);
	app.time = clock();
	app.frame_time = clock();
	mlx_loop_hook(app.M, loop_hook, &app);
	mlx_bind_keys(app.win, &app.controller);
	mlx_hook(app.win, MLX_EVENT_EXIT, 0, &close_hook, 0);
	mlx_loop(app.M);
	return (0);
}

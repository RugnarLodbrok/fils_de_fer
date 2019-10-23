#include "mlx.h"
#include "libft.h"
#include "fdf.h"
#include <stdio.h>
#include <time.h>
#include "ft_linalg.h"

int loop_hook(void *p);

int close_hook(void *param)
{
	(void)param;
	exit(0);
}

int main(void)
{
	t_app app;

	t_app_init(&app);
	line(&app, (t_vec){0, 0, 0}, (t_vec){50, 200, 0}, RED * 255);
	app.time = clock();
	app.frame_time = clock();

	mlx_loop_hook(app.M, loop_hook, &app);
	mlx_bind_keys(app.win, &app.controller);
	mlx_hook(app.win, MLX_EVENT_EXIT, 0, &close_hook, 0);
	mlx_loop(app.M);
}

void update(t_app *app, double dt)
{
	int i;
	t_mesh *obj;

	(void)dt;
	t_framebuffer_clear(&app->framebuffer);
	//todo: https://github.com/keuhdall/images_example
	t_cam_move(&app->cam, &app->controller);
	for (i = 0; (obj = app->objs[i]); ++i)
	{
		t_cam_draw(&app->cam, app, obj);
	}
	mlx_put_image_to_window(app->M, app->win, app->framebuffer.image, 0, 0);
}

int loop_hook(void *p)
{
	t_app *app;
	double dt;

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

int mouse_hook(int button, int x, int y, void *param)
{
	(void)param;
	(void)button;
	ft_printf("mouse: %d %d\n", x, y);
	return (0);
}

int key_hook(int keycode, void *p)
{
	t_app *s;

	s = (t_app *)p;
	if (keycode == KEY_LEFT)
	{
		s->momentum = t_vec_add(s->momentum, (t_vec){0, STEP, 0});
	}
	else if (keycode == KEY_RIGHT)
	{
		s->momentum = t_vec_add(s->momentum, (t_vec){0, -STEP, 0});
	}
	else if (keycode == KEY_UP)
	{
		s->momentum = t_vec_add(s->momentum, (t_vec){-STEP, 0, 0});
	}
	else if (keycode == KEY_DOWN)
	{
		s->momentum = t_vec_add(s->momentum, (t_vec){STEP, 0, 0});
	}
	return (0);
}

#include "mlx.h"
#include "libft.h"
#include "fdf.h"
#include <stdio.h>
#include "ft_linalg.h"

int loop_hook(void *p);
int mouse_hook(int button, int x, int y, void *param);
int key_hook(int keycode, void *p);

int main(void)
{
	void *M;
	void *win;
	t_scene s;
	t_mesh m;

	ft_printf("HELLO\n");
	M = mlx_init();
	win = mlx_new_window(M, 800, 600, "fdf");
	mlx_string_put(M, win, 150, 150, 255 * GREEN, "wake up, Neo!");

	ft_bzero(&s, sizeof(s));
	s.momentum = (t_vec){.1, 1, .3};
	t_vec_normalize(&(s.momentum));
	s.momentum = t_vec_mul(s.momentum, 0.2);
	s.M = M;
	s.win = win;
	s.objs = malloc(sizeof(t_wireframe *) * 2);
	s.objs[0] = &m;
	s.objs[1] = 0;
	m = t_mesh_cube(50);
	t_mat_translate(&m.m, (t_vec){200, 200, 0});

	mlx_loop_hook(M, loop_hook, &s);
	mlx_key_hook(win, key_hook, &s);
	mlx_loop(M);
}

int loop_hook(void *p)
{
	int i;
	t_mesh *obj;
	t_mat rot;
	t_scene *s;

	s = (t_scene *)p;
	if (!t_vec_len(s->momentum))
		return (0);
	obj = s->objs[0];
	rot = t_mat_rotation(s->momentum, t_vec_len(s->momentum), (t_vec){obj->m.data[0][3], obj->m.data[1][3], 0});
	t_vec_decay(&(s->momentum), 0.003);
	for (i = 0; (obj = s->objs[i]); ++i)
	{
		t_mesh_draw(obj, s, 0);
	}
	for (i = 0; (obj = s->objs[i]); ++i)
	{
		obj->m = t_mat_mul(rot, obj->m);
		t_mesh_draw(obj, s, 255 * GREEN);
	}
	mlx_pixel_put(((t_scene *)s)->M, ((t_scene *)s)->win, 200, 200, 255 * RED);
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
	t_scene *s;

	s = (t_scene *)p;
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

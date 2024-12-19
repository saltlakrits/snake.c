#pragma once

typedef enum {
	UP,
	RIGHT,
	DOWN,
	LEFT
} Heading;

typedef struct {
	int x;
	int y;
} Point;

typedef struct{
	int width;
	int height;
	int body_len;
	Point* body_arr;
  int arr_chunks;
	int fruits_eaten;
	Heading dir;
	Point fruit;

	int sleep_time;

} Snake;

Heading opposite_heading(Heading h);
Point create_point(int x, int y);
void generate_fruit(Snake* s);
void set_heading(Snake* s, char input);
void move_snake(Snake* s);
int _is_out_of_bounds(Snake* s);
int _has_collided_with_self(Snake* s);
int has_lost(Snake* s);
Snake* create_snake(int width, int height);
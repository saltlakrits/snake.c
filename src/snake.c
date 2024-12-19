#include <stdlib.h>
#include "snake.h"

// snake body allocates chunks of 32 indices
#define SNAKE_CHUNK 32

Heading opposite_heading(Heading h) {
	switch (h) {
		case UP:
			return DOWN;
		case RIGHT:
			return LEFT;
		case DOWN:
			return UP;
		case LEFT:
			return RIGHT;
	}
}

Point create_point(int x, int y) {
	// Point p = malloc(sizeof(*p));
	Point p;
	p.x = x;
	p.y = y;
	return p;
}

void generate_fruit(Snake* s) {

	int collision_found = 0;

	while (1) {
		s->fruit.x = rand() % (s->width);
		s->fruit.y = rand() % (s->height);
		collision_found = 0;

		for (int i = 0; i < s->body_len; i++) {
			if (s->fruit.x == s->body_arr[i].x && s->fruit.y == s->body_arr[i].y) {
				collision_found = 1;
				break;
			}
		}
		if (!collision_found) {
			break;
		}
	}
}

void set_heading(Snake* s, char input) {
	Heading new_heading;

	switch (input) {
		case 'w':
			new_heading = UP;
			break;
		case 'd':
			new_heading = RIGHT;
			break;
		case 's':
			new_heading = DOWN;
			break;
		case 'a':
			new_heading = LEFT;
			break;
		default:
			new_heading = s->dir;
	}

	if (new_heading != opposite_heading(s->dir)) {
		s->dir = new_heading;
	}
}

void move_snake(Snake* s) {

	// make new head from last head
	Point new_head = create_point(s->body_arr[0].x, s->body_arr[0].y);

	switch (s->dir) {
		case UP:
			new_head.y -= 1;
			break;
		case RIGHT:
			new_head.x += 1;
			break;
		case DOWN:
			new_head.y += 1;
			break;
		case LEFT:
			new_head.x -= 1;
	}

	if (new_head.x == s->fruit.x && new_head.y == s->fruit.y) {
		s->fruits_eaten += 1;
		generate_fruit(s);
		s->sleep_time = s->sleep_time * 0.99;
	}

	// if Snake should grow, increase len and realloc
	if (s->body_len < s->fruits_eaten + 3) {
		s->body_len += 1;
		// avoid frequent reallocs -- allocate more memory than
		// needed in chunks instead!
		if (s->body_len > s->arr_chunks * SNAKE_CHUNK) {
			s->arr_chunks += 1;
			s->body_arr = realloc(s->body_arr, s->arr_chunks * SNAKE_CHUNK * sizeof(*(s->body_arr)));
		}
	}

	// regardless, we shift all body parts back
	for (int i = s->body_len - 1; i > 0; i--) {
		s->body_arr[i] = s->body_arr[i-1];
	}

	// make first index our new head
	s->body_arr[0] = new_head;
	// free(new_head);
}

int _is_out_of_bounds(Snake* s) {
	Point head = s->body_arr[0];
	return (head.x < 0 || head.x == s->width || head.y < 0 || head.y == s->height);
}

int _has_collided_with_self(Snake* s) {
	Point head = s->body_arr[0];

	for (int i = 1; i < s->body_len; i++) {
		if (head.x == s->body_arr[i].x && head.y == s->body_arr[i].y) {
			return 1;
		}
	}
	return 0;
}

int has_lost(Snake* s) {
	return (_is_out_of_bounds(s) || _has_collided_with_self(s));
}

Snake* create_snake(int width, int height) {
	Snake* s = malloc(sizeof(*s));

	s->width = width;
	s->height = height;

	s->body_len = 1;
	s->arr_chunks = 1;
	s->body_arr = malloc(s->arr_chunks * SNAKE_CHUNK * sizeof(*(s->body_arr)));
	s->body_arr[0].x = s->width / 2;
	s->body_arr[0].y = s->height / 2;

	generate_fruit(s);

	s->fruits_eaten = 0;
	s->dir = UP;

	s->sleep_time = 100;

	return s;
}


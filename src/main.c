#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <ncurses.h>

#include "snake.h"

#define K 1000

#define GRAPHICAL_X_MULTIPLIER 2

#define WIDTH 20
#define HEIGHT 20

#define BLOCK_BORDER 1
#define BLOCK_SNAKE 2
#define BLOCK_FRUIT 3
#define SCORE_COLOR 4

#define MIN(A, B) ((A > B) ? B : A)

void mvwCharBlock(WINDOW *win, const char *s, int color, int y_mult,
									int x_coord, int y_coord)
{
	// Prints a string to a given coord, repats it for y_mult lines below.

	for (int i = 0; i < y_mult; i++)
	{
		wattron(win, COLOR_PAIR(color));
		mvwprintw(win, y_coord + i, x_coord, "%s", s);
		wattroff(win, COLOR_PAIR(color));
	}
}

int main(void)
{
	initscr();

	// check if terminal is too small to display the game at all
	if (LINES < HEIGHT + 3 || COLS < (WIDTH + 2) * GRAPHICAL_X_MULTIPLIER)
	{
		endwin();
		printf("Your terminal window is, somhow, too small.\n\
			For your own sake, and mine, make it bigger.");
		exit(1);
	}

	// dynamic sizing
	int width = COLS / WIDTH;
	int height = LINES / (HEIGHT + 1);
	int screen_size_multiple = MIN(width, height);

	// ensure screen_size_multiple is at least 1
	screen_size_multiple = (screen_size_multiple == 0) ? 1 : screen_size_multiple;
	// multipliers for y and x
	int y_multiple = screen_size_multiple;
	int x_multiple = screen_size_multiple * GRAPHICAL_X_MULTIPLIER;

	// seed the rng
	srand(time(NULL));

	// more ncurses init
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	curs_set(0);
	nodelay(stdscr, TRUE);
	start_color();

	// block char pairs
	init_pair(BLOCK_SNAKE, COLOR_BLACK, COLOR_GREEN);
	init_pair(BLOCK_FRUIT, COLOR_BLACK, COLOR_RED);

	// border color
	init_pair(BLOCK_BORDER, COLOR_BLUE, COLOR_BLUE);

	// score color
	init_pair(SCORE_COLOR, COLOR_BLACK, COLOR_BLUE);

	// dynamic window size
	int window_height = HEIGHT * y_multiple;
	int window_width = WIDTH * x_multiple;

	// horizontal string to print
	char *snake_str = malloc((x_multiple + 1) * sizeof(*snake_str));
	memset(snake_str, ' ', x_multiple);
	snake_str[x_multiple] = '\0';

	// approx. center of screen
	int start_x = (COLS - window_width) / 2;
	int start_y = (LINES - window_height) / 2;

	WINDOW *win = newwin(window_height + 2, window_width + 2, start_y, start_x);

	refresh();

	wrefresh(win);

	Snake *snake = create_snake(WIDTH, HEIGHT);
	char* score = malloc(50 * sizeof(*score));

	while (true)
	{
		// clear window
		werase(win);
		// draw border
		// if i draw this outside the window instead i wouldn't have
		// to redraw it every frame, oh well
		wattron(win, COLOR_PAIR(BLOCK_BORDER));
		box(win, 0, 0);
		wattroff(win, COLOR_PAIR(BLOCK_BORDER));

		int ch;
		ch = getch();

		set_heading(snake, ch);
		move_snake(snake);

		sprintf(score, "Fruits eaten: %d", snake->fruits_eaten);

		wattron(win, COLOR_PAIR(SCORE_COLOR));
		wattron(win, A_BOLD);
		mvwprintw(win, 0, 0 + 2 + GRAPHICAL_X_MULTIPLIER, "%s", score);
		wattroff(win, COLOR_PAIR(SCORE_COLOR));
		wattroff(win, A_BOLD);

		if (has_lost(snake))
		{
			break;
		}

		// draw fruit
		mvwCharBlock(win, snake_str, BLOCK_FRUIT, y_multiple,
								 (snake->fruit.x * x_multiple) + 1,
								 (snake->fruit.y * y_multiple) + 1);

		// draw snake
		for (int i = 0; i < snake->body_len; i++)
		{
			mvwCharBlock(win, snake_str, BLOCK_SNAKE, y_multiple,
									 (snake->body_arr[i].x * x_multiple) + 1,
									 (snake->body_arr[i].y * y_multiple) + 1);
		}

		wrefresh(win);
		usleep(snake->sleep_time * K);
	}

	curs_set(1);
	endwin();

	printf("\nYou lost :(\nYou ate %d fruit!\n\n", snake->fruits_eaten);
	return 0;
}

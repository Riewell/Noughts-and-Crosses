/*  Noughts and Crosses

  Крестики-нолики
  Version 0.2 - Alpha

  Copyright 2013 Konstantin Zyryanov <post.herzog@gmail.com>
  
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as published by
  the Free Software Foundation; either version 2.1 of the License, or
  (at your option) any later version.
  
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Lesser General Public License for more details.
  
  You should have received a copy of the GNU Lesser General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
  MA 02110-1301, USA.
*/

#include <ncurses.h>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <clocale>
#include <ctime>
using namespace std;

void player(char side), cpu(char side), winner(int y_1, int x_1, int y_2, int x_2, int y_3, int x_3, char side);
int main_menu();
bool check();

int x, y, bad_x, bad_y, i, j;
//TODO: Убрать в настройки
int size=3;
char array[3][3], *array_p;

int main()
{
//Инициализация ncurses
	setlocale(LC_CTYPE, "ru_RU.UTF8");
	initscr();
	curs_set(0);
	keypad(stdscr, true);
	echo();
//Инициализация генератора случайных чисел
	srand(time(0));
	while (main_menu())
	{
		clear();
		start_color();
		init_pair(1, COLOR_RED, COLOR_BLACK);
		init_pair(2, COLOR_BLUE, COLOR_BLACK);
//Инициализация и отрисовка первоначального игрового поля
		chtype space;
		space=' '|A_UNDERLINE;
		move(4,29);
		printw("x   1     2     3");
		move(5,27);
		printw("y");
		move(5,30);
		printw("___________________");
		move(6,30);
		printw("|     |     |     |");
		move(7,27);
		printw("1  |     |     |     |");
		move(8,30);
		addch('|');
		addch(space); addch(space); addch(space); addch(space); addch(space);
		addch('|');
		addch(space); addch(space); addch(space); addch(space); addch(space);
		addch('|');
		addch(space); addch(space); addch(space); addch(space); addch(space);
		addch('|');
		move(9,30);
		printw("|     |     |     |");
		move(10,27);
		printw("2  |     |     |     |");
		move(11,30);
		addch('|');
		addch(space); addch(space); addch(space); addch(space); addch(space);
		addch('|');
		addch(space); addch(space); addch(space); addch(space); addch(space);
		addch('|');
		addch(space); addch(space); addch(space); addch(space); addch(space);
		addch('|');
		move(12,30);
		printw("|     |     |     |");
		move(13,27);
		printw("3  |     |     |     |");
		move(14,30);
		addch('|');
		addch(space); addch(space); addch(space); addch(space); addch(space);
		addch('|');
		addch(space); addch(space); addch(space); addch(space); addch(space);
		addch('|');
		addch(space); addch(space); addch(space); addch(space); addch(space);
		addch('|');
		refresh();
//Инициализация массива значений квадратов игрового поля
		array_p=array[0];
		for (i=0; i < size; i++)
		{
			for (j=0; j < size; j++)
			{
				strcpy(array_p, " ");
				array_p++;
			}
		}
//Выбор игроком крестиков или ноликов
		char side[20];
		bool player_turn;
		move(17,5);
		printw("1 - X;  2 - O;  3 - Всё равно.");
		move(18,5);
		printw("Ваш выбор: ");
		refresh();
		scanw("%s", side);
		for (i=0; i<20; i++)
		{
			side[i]=tolower(side[i]);
		}
		if (!strncmp(side, "1", 1) || !strncmp(side, "x", 1) || !strncmp(side, "х", 1) || !strncmp(side, "Х", 1))
		{
			player_turn=true;
		}
		else if (!strncmp(side, "2", 1) || !strncmp(side, "o", 1) || !strncmp(side, "о", 1) || !strncmp(side, "О", 1) || !strncmp(side, "0", 1))
		{
			player_turn=false;
		}
		else
		{
			if (rand()%2)
			{
				player_turn=true;
			}
			else
			{
				player_turn=false;
			}
		}
//Игра
		bool win=false;
		while (!win)
		{
			if (player_turn)
			{
				player('X');
				win=check();
				if (win)
					break;
				cpu('O');
				win=check();
			}
			else
			{
				cpu('X');
				win=check();
				if (win)
					break;
				player('O');
				win=check();
			}
		}
		getch();
		attrset(A_NORMAL);
	}
	curs_set(1);
	endwin();
	return 0;
}

//Ход игрока
void player(char side)
{
	bool error=false;
	time_t timer, timer_2;
	do
	{
		move(16,0);
		clrtobot();
		if (error)
		{
			time(&timer);
			move (16,5);
			attron(A_BOLD);
			printw("Ошибка! Задайте координаты ещё раз!");
			refresh();
			while (difftime(time(&timer_2),timer) < 0.3)
			{
			}
			move (16,5);
			attroff(A_BOLD);
			attron(COLOR_PAIR(1));
			printw("Ошибка! Задайте координаты ещё раз!");
			refresh();
			time(&timer);
			while (difftime(time(&timer_2),timer) < 0.3)
			{
			}
			move (16,5);
			attron(A_BOLD);
			printw("Ошибка! Задайте координаты ещё раз!");
			refresh();
			attrset(A_NORMAL);
		}
		move(17,5);
		printw("Ваш ход.");
		move(18,11);
		printw("x:");
		refresh();
		bad_x=getch();
		while (!isdigit(bad_x) || bad_x < 49 || bad_x > 51)
		{
			move(18,0);
			clrtoeol();
			move(18,11);
			printw("x:");
			refresh();
			bad_x=getch();
		}
		move(19,11);
		printw("y:");
		refresh();
		bad_y=getch();
		while (!isdigit(bad_y) || bad_y < 49 || bad_y > 51)
		{
			move(19,0);
			clrtoeol();
			move(19,11);
			printw("y:");
			refresh();
			bad_y=getch();
		}
		error=true;
	} while (array[bad_y-49][bad_x-49] != ' ');
	x=bad_x-1;
	y=bad_y-1;
	array_p=&array[y-48][x-48];
	*array_p=side;
	int arr_y=7, arr_x=33;
	for (i=0; i<3; i++)
	{
		for (j=0; j<3; j++)
		{
			move(arr_y,arr_x);
			printw("%c", array[i][j]);
			arr_x=arr_x+6;
		}
		arr_y=arr_y+3;
		arr_x=33;
	}
	refresh();
}

//Ход компьютера
void cpu(char side)
{
	move(16,0);
	clrtobot();
	move(16,5);
	printw("Думаю...");
	refresh();
//Проверка на возможность победы игрока на следующем ходу
	char other_side;
	int local_x=0, local_y=0, repeat, space, space_x, space_y;
	int find_possible_fail=0;
	if (side == 'X')
		other_side='O';
	else
		other_side='X';
//Проверка по горизонтали
	for (i=0; i < size; i++)
	{
		local_x=0; repeat=0; space=0;
		if (array[local_y][local_x] == other_side && array[local_y][local_x] == array[local_y][size-1])
			repeat++;
		if (array[local_y][size-1] == ' ')
		{
			space++;
			space_x=size-1; space_y=local_y;
		}
		for (j=0; j < size-1; j++)
		{
			if (array[local_y][local_x] == other_side && array[local_y][local_x] == array[local_y][local_x+1])
				repeat++;
			if (array[local_y][local_x] == ' ')
			{
				space++;
				space_x=local_x; space_y=local_y;
			}
			local_x++;
		}
		if (size-repeat == 2 && space == 1)
		{
			y=space_y; x=space_x;
			find_possible_fail=1;
		}
		local_y++;
	}
//Проверка по вертикали
	local_x=0; local_y=0;
	for (i=0; i < size; i++)
	{
		local_y=0; repeat=0; space=0;
		if (array[local_y][local_x] == other_side && array[local_y][local_x] == array[size-1][local_x])
			repeat++;
		if (array[size-1][local_x] == ' ')
		{
			space++;
			space_x=local_x; space_y=size-1;
		}
		for (j=0; j < size-1; j++)
		{
			if (array[local_y][local_x] == other_side && array[local_y][local_x] == array[local_y+1][local_x])
				repeat++;
			if (array[local_y][local_x] == ' ')
			{
				space++;
				space_x=local_x; space_y=local_y;
			}
			local_y++;
		}
		if (size-repeat == 2 && space == 1)
		{
			x=space_x; y=space_y;
			find_possible_fail=2;
		}
		local_x++;
	}
//Проверка по диагоналям
	local_x=0; local_y=0; repeat=0; space=0;
	if (array[local_y][local_x] == other_side && array[local_y][local_x] == array[size-1][size-1])
			repeat++;
	if (array[size-1][size-1] == ' ')
	{
		space++;
		space_x=size-1; space_y=size-1;
	}
	for (i=0; i < size-1; i++)
	{
		if (array[local_y][local_x] == other_side && array[local_y][local_x] == array[local_y+1][local_x+1])
			repeat++;
		if (array[local_y][local_x] == ' ')
		{
			space++;
			space_x=local_x; space_y=local_y;
		}
		if (size-repeat == 2 && space == 1)
		{
			x=space_x; y=space_y;
			find_possible_fail=3;
		}
		local_x++; local_y++;
	}
	local_x=0; local_y=0; repeat=0; space=0;
	if (array[local_y][size-1] == other_side && array[local_y][size-1] == array[size-1][local_x])
			repeat++;
	if (array[size-1][local_x] == ' ')
	{
		space++;
		space_x=local_x; space_y=size-1;
	}
	local_x=2;
	for (i=0; i < size-1; i++)
	{
		if (array[local_y][local_x] == other_side && array[local_y][local_x] == array[local_y+1][local_x-1])
			repeat++;
		if (array[local_y][local_x] == ' ')
		{
			space++;
			space_x=local_x; space_y=local_y;
		}
		if (size-repeat == 2 && space == 1)
		{
			x=space_x; y=space_y;
			find_possible_fail=3;
		}
		local_x--; local_y++;
	}
//Проверка возможности победы компьютера на данном ходу
/*char other_side;
	int local_x=0, local_y=0, repeat, space, space_x, space_y;
	int find_possible_fail=0;
	if (side == 'X')
		other_side='O';
	else
		other_side='X';*/
//Проверка по горизонтали
	for (i=0; i < size; i++)
	{
		local_x=0; repeat=0; space=0;
		if (array[local_y][local_x] == side && array[local_y][local_x] == array[local_y][size-1])
			repeat++;
		if (array[local_y][size-1] == ' ')
		{
			space++;
			space_x=size-1; space_y=local_y;
		}
		for (j=0; j < size-1; j++)
		{
			if (array[local_y][local_x] == side && array[local_y][local_x] == array[local_y][local_x+1])
				repeat++;
			if (array[local_y][local_x] == ' ')
			{
				space++;
				space_x=local_x; space_y=local_y;
			}
			local_x++;
		}
		if (size-repeat == 2 && space == 1)
		{
			y=space_y; x=space_x;
			find_possible_fail=4;
		}
		local_y++;
	}
//Проверка по вертикали
	local_x=0; local_y=0;
	for (i=0; i < size; i++)
	{
		local_y=0; repeat=0; space=0;
		if (array[local_y][local_x] == side && array[local_y][local_x] == array[size-1][local_x])
			repeat++;
		if (array[size-1][local_x] == ' ')
		{
			space++;
			space_x=local_x; space_y=size-1;
		}
		for (j=0; j < size-1; j++)
		{
			if (array[local_y][local_x] == side && array[local_y][local_x] == array[local_y+1][local_x])
				repeat++;
			if (array[local_y][local_x] == ' ')
			{
				space++;
				space_x=local_x; space_y=local_y;
			}
			local_y++;
		}
		if (size-repeat == 2 && space == 1)
		{
			x=space_x; y=space_y;
			find_possible_fail=5;
		}
		local_x++;
	}
//Проверка по диагоналям
	local_x=0; local_y=0; repeat=0; space=0;
	if (array[local_y][local_x] == side && array[local_y][local_x] == array[size-1][size-1])
			repeat++;
	if (array[size-1][size-1] == ' ')
	{
		space++;
		space_x=size-1; space_y=size-1;
	}
	for (i=0; i < size-1; i++)
	{
		if (array[local_y][local_x] == side && array[local_y][local_x] == array[local_y+1][local_x+1])
			repeat++;
		if (array[local_y][local_x] == ' ')
		{
			space++;
			space_x=local_x; space_y=local_y;
		}
		if (size-repeat == 2 && space == 1)
		{
			x=space_x; y=space_y;
			find_possible_fail=6;
		}
		local_x++; local_y++;
	}
	local_x=0; local_y=0; repeat=0; space=0;
	if (array[local_y][size-1] == side && array[local_y][size-1] == array[size-1][local_x])
			repeat++;
	if (array[size-1][local_x] == ' ')
	{
		space++;
		space_x=local_x; space_y=size-1;
	}
	local_x=2;
	for (i=0; i < size-1; i++)
	{
		if (array[local_y][local_x] == side && array[local_y][local_x] == array[local_y+1][local_x-1])
			repeat++;
		if (array[local_y][local_x] == ' ')
		{
			space++;
			space_x=local_x; space_y=local_y;
		}
		if (size-repeat == 2 && space == 1)
		{
			x=space_x; y=space_y;
			find_possible_fail=6;
		}
		local_x--; local_y++;
	}
//Простой перебор случайных вариантов
	bool think=false;
	for (i=0; i < 3; i++)
	{
		if (x == (i+48))
			x=i;
		if (y == (i+48))
			y=i;
	}
	while (!think)
	{
		do
		{
			if (find_possible_fail == 0)
			{
				x=rand()%3;
				y=rand()%3;
			}
		} while (array[y][x] != ' ');
		think=true;
	}
	array_p=&array[y][x];
	*array_p=side;
	int arr_y=7, arr_x=33;
	for (i=0; i<3; i++)
	{
		for (j=0; j<3; j++)
		{
			move(arr_y,arr_x);
			printw("%c", array[i][j]);
			arr_x=arr_x+6;
		}
		arr_y=arr_y+3;
		arr_x=33;
	}
	refresh();
}

//Проверка на победу
bool check()
{
	bool win=false;
	if (array[0][0] != ' ' && (array[0][0] == array[0][1]) && (array[0][0] == array[0][2]))
	{
		win=true;
		winner(1, 1, 1, 2, 1, 3, array[0][0]);
	}
	if (array[1][0] != ' ' && (array[1][0] == array[1][1]) && (array[1][0] == array[1][2]))
	{
		win=true;
		winner(2, 1, 2, 2, 2, 3, array[1][0]);
	}
	if (array[2][0] != ' ' && (array[2][0] == array[2][1]) && (array[2][0] == array[2][2]))
	{
		win=true;
		winner(3, 1, 3, 2, 3, 3, array[2][0]);
	}
	if (array[0][0] != ' ' && (array[0][0] == array[1][0]) && (array[0][0] == array[2][0]))
	{
		win=true;
		winner(1, 1, 2, 1, 3, 1, array[0][0]);
	}
	if (array[0][1] != ' ' && (array[0][1] == array[1][1]) && (array[0][1] == array[2][1]))
	{
		win=true;
		winner(1, 2, 2, 2, 3, 2, array[0][1]);
	}
	if (array[0][2] != ' ' && (array[0][2] == array[1][2]) && (array[0][2] == array[2][2]))
	{
		win=true;
		winner(1, 3, 2, 3, 3, 3, array[0][2]);
	}
	if (array[0][0] != ' ' && (array[0][0] == array[1][1]) && (array[0][0] == array[2][2]))
	{
		win=true;
		winner(1, 1, 2, 2, 3, 3, array[0][0]);
	}
	if (array[2][0] != ' ' && (array[2][0] == array[1][1]) && (array[2][0] == array[0][2]))
	{
		win=true;
		winner(1, 3, 2, 2, 3, 1, array[0][2]);
	}
	if (win)
		return win;
//Проверка на ничью
	int free_slots=9;
	for (i=0; i < 3; i++)
	{
		for (j=0; j < 3; j++)
		{
			if (array[i][j] != ' ')
				free_slots--;
		}
	}
	if (free_slots == 0)
	{
		win=true;
		move(16,0);
		clrtobot();
		move(17,36);
		attron(COLOR_PAIR(1));
		attron(A_BOLD);
		printw("Ничья!");
		refresh();
	}
	return win;
}

//Поздравление победителя
void winner(int y_1, int x_1, int y_2, int x_2, int y_3, int x_3, char side)
{
	time_t timer, timer_2;
//Поздравляем победителя
	move(16,0);
	clrtobot();
	move(17,36);
	attron(COLOR_PAIR(1));
	attron(A_BOLD);
	printw("Победа!");
	refresh();
	time(&timer);
	while (difftime(time(&timer_2),timer) < 1)
	{
	}
//Уточняем координаты символов победителя
//Первый символ
	if (y_1 == 1)
		y_1=7;
	if (y_1 == 2)
		y_1=10;
	if (y_1 == 3)
		y_1=13;
	if (x_1 == 1)
		x_1=33;
	if (x_1 == 2)
		x_1=39;
	if (x_1 == 3)
		x_1=45;
//Второй символ
	if (y_2 == 1)
		y_2=7;
	if (y_2 == 2)
		y_2=10;
	if (y_2 == 3)
		y_2=13;
	if (x_2 == 1)
		x_2=33;
	if (x_2 == 2)
		x_2=39;
	if (x_2 == 3)
		x_2=45;
//Третий символ
	if (y_3 == 1)
		y_3=7;
	if (y_3 == 2)
		y_3=10;
	if (y_3 == 3)
		y_3=13;
	if (x_3 == 1)
		x_3=33;
	if (x_3 == 2)
		x_3=39;
	if (x_3 == 3)
		x_3=45;
//Отрисовываем символы победителя
	move(y_1,x_1);
	printw("%c", side);
	refresh();
	time(&timer);
	while (difftime(time(&timer_2),timer) < 1)
	{
	}
	move(y_2,x_2);
	printw("%c", side);
	refresh();
	time(&timer);
	while (difftime(time(&timer_2),timer) < 1)
	{
	}
	move(y_3,x_3);
	printw("%c", side);
	refresh();
	time(&timer);
	while (difftime(time(&timer_2),timer) < 1)
	{
	}
	attrset(COLOR_PAIR(2));
	move(y_1,x_1);
	printw("%c", side);
	move(y_2,x_2);
	printw("%c", side);
	move(y_3,x_3);
	printw("%c", side);
	refresh();
}

//Главное меню
int main_menu()
{
	int choose=0;
	while (true)
	{
		clear();
		attron(A_BOLD);
		if (!choose)
		{
			move(16,30);
			printw("> ");
			attron(A_REVERSE);
		}
		move(16,32);
		printw("Новая игра");
		attroff(A_REVERSE);
		if (choose)
		{
			move(18,30);
			printw("> ");
			attron(A_REVERSE);
		}
		move(18,32);
		printw("Выход");
		attroff(A_REVERSE);
		refresh();
		char key=getch();
		if (key == '\002' || key == '\003')
		{
			if (choose)
				choose=0;
			else
				choose=1;
		}
		if (key == '\n')
		{
			if (!choose)
			{
				attrset(A_NORMAL);
				return 1;
			}
			else
				return 0;	
		}
		if (key == '\033')
		{
			return 0;
		}
	}
}

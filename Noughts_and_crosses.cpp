/*  Noughts and Crosses

  Крестики-нолики
  Version 0.3.1 - Alpha

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

void cpu(char side, char **array), winner(int line, int line_number, int player_win, char symbol), single_game(), campaign();
int player(char side, char **array);
int main_menu(), sub_menu(int menu);
bool check(int player_win, char **array);

int x, y, x_begin, y_begin, i, j;
int size=3;
int difficalty=2;
int default_side=0;
char lang[3]="ru";
char *array_p;

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
//Вызов главного меню
	main_menu();
	curs_set(1);
	endwin();
	return 0;
}

//Ход игрока
int player(char side, char **array)
{
	bool error=false;
	time_t timer, timer_2;
	do
	{
		move(17,0);
		clrtobot();
		if (error)
		{
			time(&timer);
			move (17,5);
			attron(A_BOLD);
			printw("Ошибка! Задайте координаты ещё раз!");
			refresh();
			while (difftime(time(&timer_2),timer) < 0.3)
			{
			}
			move (17,5);
			attrset(COLOR_PAIR(3));
			printw("Ошибка! Задайте координаты ещё раз!");
			refresh();
			time(&timer);
			while (difftime(time(&timer_2),timer) < 0.3)
			{
			}
			move (17,5);
			attron(A_BOLD);
			attron(COLOR_PAIR(1));
			printw("Ошибка! Задайте координаты ещё раз!");
			refresh();
			attrset(A_NORMAL);
		}
		move(18,5);
		printw("Ваш ход.");
		do
		{
			move(19,0);
			clrtoeol();
			move(19,11);
			printw("x:");
			refresh();
			x=getch();
			if (x == 27)
				break;
		} while (!isdigit(x) || x < 49 || x > size+48);
		if (x == 27)
			return 1;
		do
		{
			move(20,0);
			clrtoeol();
			move(20,11);
			printw("y:");
			refresh();
			y=getch();
			if (y == 27)
				break;
		} while (!isdigit(y) || y < 49 || y > size+48);
		if (y == 27)
			return 1;
		error=true;
	} while (array[y-49][x-49] != ' ');
	array[y-49][x-49]=side;
	int arr_y=y_begin+3, arr_x=x_begin+3;
	for (i=0; i < size; i++)
	{
		for (j=0; j < size; j++)
		{
			move(arr_y,arr_x);
			printw("%c", array[i][j]);
			arr_x=arr_x+6;
		}
		arr_y=arr_y+3;
		arr_x=x_begin+3;
	}
	refresh();
	return 0;
}

//Ход компьютера
void cpu(char side, char **array)
{
	int find_possible_fail=0;
	move(17,0);
	clrtobot();
	move(17,5);
	printw("Думаю...");
	refresh();
//Проверка на возможность победы игрока на следующем ходу
	if (difficalty > 0)
	{
		char other_side;
		int local_x=0, local_y=0, repeat, space, space_x, space_y;
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
		local_x=size-1;
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
	}
//Проверка возможности победы компьютера на данном ходу
	if (difficalty == 2)
	{
//Проверка по горизонтали
		int local_x=0, local_y=0, repeat, space, space_x, space_y;
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
		local_x=size-1;
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
				x=rand()%size;
				y=rand()%size;
			}
		} while (array[y][x] != ' ');
		think=true;
	}
	array[y][x]=side;
	int arr_y=y_begin+3, arr_x=x_begin+3;
	for (i=0; i < size; i++)
	{
		for (j=0; j < size; j++)
		{
			move(arr_y,arr_x);
			printw("%c", array[i][j]);
			arr_x=arr_x+6;
		}
		arr_y=arr_y+3;
		arr_x=x_begin+3;
	}
	refresh();
}

//Проверка на победу
bool check(int player_win, char **array)
{
	bool win=false;
	int local_x=0, local_y=0, repeat=0;
//Проверка по горизонтали
	for (i=0; i < size; i++)
	{
		local_x=0; repeat=0;
		if (array[local_y][local_x] != ' ' && (array[local_y][local_x] == array[local_y][size-1]))
			repeat++;
		for (j=0; j < size-1; j++)
		{
			if (array[local_y][local_x] != ' ' && (array[local_y][local_x] == array[local_y][local_x+1]))
				repeat++;
			local_x++;
		}
		if (repeat == size)
		{
			win=true;
			winner(1, i, player_win, array[local_y][local_x]);
		}
		local_y++;
	}
//Проверка по вертикали
	local_x=0;
	for (i=0; i < size; i++)
	{
		local_y=0; repeat=0;
		if (array[local_y][local_x] != ' ' && array[local_y][local_x] == array[size-1][local_x])
			repeat++;
		for (j=0; j < size-1; j++)
		{
			if (array[local_y][local_x] != ' ' && array[local_y][local_x] == array[local_y+1][local_x])
				repeat++;
			local_y++;
		}
		if (repeat == size)
		{
			win=true;
			winner(2, i, player_win, array[local_y][local_x]);
		}
		local_x++;
	}
//Проверка по диагоналям
	local_x=0; local_y=0; repeat=0;
	if (array[local_y][local_x] != ' ' && array[local_y][local_x] == array[size-1][size-1])
			repeat++;
	for (i=0; i < size-1; i++)
	{
		if (array[local_y][local_x] != ' ' && array[local_y][local_x] == array[local_y+1][local_x+1])
			repeat++;
		if (repeat == size)
		{
			win=true;
			winner(3, i, player_win, array[local_y][local_x]);
		}
		local_x++; local_y++;
	}
	local_x=0; local_y=0; repeat=0;
	if (array[local_y][size-1] != ' ' && array[local_y][size-1] == array[size-1][local_x])
			repeat++;
	local_x=size-1;
	for (i=0; i < size-1; i++)
	{
		if (array[local_y][local_x] != ' ' && array[local_y][local_x] == array[local_y+1][local_x-1])
			repeat++;
		if (repeat == size)
		{
			win=true;
			winner(4, i, player_win, array[local_y][local_x]);
		}
		local_x--; local_y++;
	}
	if (win)
		return win;
//Проверка на ничью
	int free_slots=size*size;
	for (i=0; i < size; i++)
	{
		for (j=0; j < size; j++)
		{
			if (array[i][j] != ' ')
				free_slots--;
		}
	}
	if (free_slots == 0)
	{
		win=true;
		move(17,0);
		clrtobot();
		move(17,36);
		attron(A_BOLD);
		printw("Ничья!");
		refresh();
	}
	return win;
}

//Поздравление победителя
//FIXME: line и player_win как флаг?
void winner(int line, int line_number, int player_win, char symbol)
{
	time_t timer, timer_2;
//Текстовое поздравление
	move(17,0);
	clrtobot();
	attron(A_BOLD);
	if (player_win)
	{
		move(18,36);
		attron(COLOR_PAIR(1));
		printw("Победа!");
	}
	else
	{
		move(18,34);
		attron(COLOR_PAIR(4));
		printw("Поражение...");
		attron(COLOR_PAIR(1));
	}
	refresh();
	time(&timer);
	while (difftime(time(&timer_2),timer) < 1)
	{
	}
//Уточнение координат и отрисовка символов победителя
	y=y_begin+3; x=x_begin+3;
	switch (line)	
	{
		case 1:
			for (i=0; i < size; i++)
			{
				if (line_number == i)
				{
					for (j=0; j < size; j++)
					{
						move(y,x);
						printw("%c", symbol);
						refresh();
						time(&timer);
						while (difftime(time(&timer_2),timer) < 1)
						{
						}
						x=x+6;
					}
					x=x_begin+3;
					attrset(COLOR_PAIR(2));
					for (j=0; j < size; j++)
					{
						move(y,x);
						printw("%c", symbol);
						x=x+6;
					}
				}
				y=y+3;
			}
			break;
		case 2:
			for (i=0; i < size; i++)
			{
				if (line_number == i)
				{
					for (j=0; j < size; j++)
					{
						move(y,x);
						printw("%c", symbol);
						refresh();
						time(&timer);
						while (difftime(time(&timer_2),timer) < 1)
						{
						}
						y=y+3;
					}
					y=y_begin+3;
					attrset(COLOR_PAIR(2));
					for (j=0; j < size; j++)
					{
						move(y,x);
						printw("%c", symbol);
						y=y+3;
					}
				}
				x=x+6;
			}
			break;
		case 3:
			for (i=0; i < size; i++)
			{
				move(y,x);
				printw("%c", symbol);
				refresh();
				time(&timer);
				while (difftime(time(&timer_2),timer) < 1)
				{
				}
				x=x+6; y=y+3;
			}
			x=x_begin+3; y=y_begin+3;
			attrset(COLOR_PAIR(2));
			for (j=0; j < size; j++)
			{
				move(y,x);
				printw("%c", symbol);
				x=x+6; y=y+3;
			}
			break;
		case 4:
			x=x+6*(size-1);
			for (i=0; i < size; i++)
			{
				move(y,x);
				printw("%c", symbol);
				refresh();
				time(&timer);
				while (difftime(time(&timer_2),timer) < 1)
				{
				}
				x=x-6; y=y+3;
			}
			x=x_begin+6*(size-1)+3; y=y_begin+3;
			attrset(COLOR_PAIR(2));
			for (j=0; j < size; j++)
			{
				move(y,x);
				printw("%c", symbol);
				x=x-6; y=y+3;
			}
	}
	refresh();
}

//Главное меню
int main_menu()
{
	int choose=0;
	char key;
	while (true)
	{
		clear();
		attron(A_BOLD);
		if (choose == 0)
		{
			move(14,30);
			printw("> ");
			attron(A_REVERSE);
		}
		move(14,32);
		printw("Кампания");
		attroff(A_REVERSE);
		if (choose == 1)
		{
			move(16,30);
			printw("> ");
			attron(A_REVERSE);
		}
		move(16,32);
		printw("Одиночная партия");
		attroff(A_REVERSE);
		if (choose == 2)
		{
			move(18,30);
			printw("> ");
			attron(A_REVERSE);
		}
		move(18,32);
		printw("Настройки");
		attroff(A_REVERSE);
		if (choose == 3)
		{
			move(20,30);
			printw("> ");
			attron(A_REVERSE);
		}
		move(20,32);
		printw("Выход");
		attroff(A_REVERSE);
		refresh();
		key=getch();
		if (key == '\002')
		{
			if (choose == 3)
				choose=0;
			else
				choose++;
		}
		if (key == '\003')
		{
			if (choose == 0)
				choose=3;
			else
				choose--;
		}
		if (key == '\n')
		{
			if (choose == 0)
				sub_menu(0);
			if (choose == 1)
			{
				attrset(A_NORMAL);
				single_game();
			}
			if (choose == 2)
				sub_menu(2);
			if (choose == 3)
				return 0;	
		}
		if (key == '\033')
		{
			return 0;
		}
	}
}

//Подменю
int sub_menu(int menu)
{
//FIXME: флаги?
	int choose=0;
	char key;
	int local_x=(getmaxx(stdscr)-36)/2;
	int max_size=5;
	if (menu == 0)
	{
		while (true)
		{
			clear();
			if (choose == 0)
			{
				move(14,30);
				printw("> ");
				attron(A_REVERSE);
			}
			move(14,32);
			printw("Начать кампанию");
			attroff(A_REVERSE);
			if (choose == 1)
			{
				move(16,30);
				printw("> ");
				attron(A_REVERSE);
			}
			move(16,32);
			printw("Загрузить сохранённую игру");
			attroff(A_REVERSE);
			if (choose == 2)
			{
				move(18,30);
				printw("> ");
				attron(A_REVERSE);
			}
			move(18,32);
			printw("Назад");
			attroff(A_REVERSE);
			refresh();
			key=getch();
			if (key == '\002')
			{
				if (choose == 2)
					choose=0;
				else
					choose++;
			}
			if (key == '\003')
			{
				if (choose == 0)
					choose=2;
				else
					choose--;
			}
			if (key == '\n')
			{
				if (choose == 0)
				{
					campaign();
				}
				if (choose == 1)
				{
				}
				if (choose == 2)
					return 0;
			}
			if (key == '\033')
			{
				return 0;
			}
		}
	}
	if (menu == 2)
	{
		char difficalty_str[30], default_side_str[20], lang_str[20];
		if  (difficalty == 0)
			strcpy(difficalty_str, "Очень легко");
		if (difficalty == 1)
			strcpy(difficalty_str, "Легко");
		if (difficalty == 2)
			strcpy(difficalty_str, "Средне");
		if  (default_side == 0)
			strcpy(default_side_str, "На выбор");
		if (default_side == 1)
			strcpy(default_side_str, "Крестики");
		if (default_side == 2)
			strcpy(default_side_str, "Нолики");
		if (default_side == 3)
			strcpy(default_side_str, "Случайно");
		strcpy(lang_str, "Русский");
		while (true)
		{
			clear();
			move(9,local_x+5);
			printw("Настройки одиночной партии");
			if (choose == 0)
			{
				move(12,local_x-2);
				printw("> ");
				attron(A_REVERSE);
			}
			move(12,local_x);
			printw("Сложность");
			move(12,local_x+24);
			attroff(A_REVERSE);
			printw("< %s >", difficalty_str);
			if (choose == 1)
			{
				move(14,local_x-2);
				printw("> ");
				attron(A_REVERSE);
			}
			move(14,local_x);
			printw("Размер игрового поля");
			move(14,local_x+24);
			attroff(A_REVERSE);
			printw("< %i >", size);
			if (choose == 2)
			{
				move(16,local_x-2);
				printw("> ");
				attron(A_REVERSE);
			}
			move(16,local_x);
			printw("Игрок всегда получает");
			move(16,local_x+24);
			attroff(A_REVERSE);
			printw("< %s >", default_side_str);
			if (choose == 3)
			{
				move(18,local_x-2);
				printw("> ");
				attron(A_REVERSE);
			}
			move(18,local_x);
			printw("Язык");
			move(18,local_x+24);
			attroff(A_REVERSE);
			printw("< %s >", lang_str);
			if (choose == 4)
			{
				move(20,local_x-2);
				printw("> ");
				attron(A_REVERSE);
			}
			move(20,local_x);
			printw("Назад");
			attroff(A_REVERSE);
			refresh();
			key=getch();
			if (key == '\002')
			{
				if (choose == 4)
					choose=0;
				else
					choose++;
			}
			if (key == '\003')
			{
				if (choose == 0)
					choose=4;
				else
					choose--;
			}
			if (key == '\005')
			{
				if (choose == 0)
				{
					if (difficalty == 2)
						difficalty=0;
					else
						difficalty++;
					if  (difficalty == 0)
						strcpy(difficalty_str, "Очень легко");
					if (difficalty == 1)
						strcpy(difficalty_str, "Легко");
					if (difficalty == 2)
						strcpy(difficalty_str, "Средне");
				}
				if (choose == 1)
				{
					if (size == max_size)
						size=3;
					else
						size++;
					move(14,local_x+26);
					printw("%i", size);
				}
				if (choose == 2)
				{
					if (default_side == 3)
						default_side=0;
					else
						default_side++;
					if  (default_side == 0)
						strcpy(default_side_str, "На выбор");
					if (default_side == 1)
						strcpy(default_side_str, "Крестики");
					if (default_side == 2)
						strcpy(default_side_str, "Нолики");
					if (default_side == 3)
						strcpy(default_side_str, "Случайно");
				}
				if (choose == 3)
				{
				}
			}
			if (key == '\004')
			{
				if (choose == 0)
				{
					if (difficalty == 0)
						difficalty=2;
					else
						difficalty--;
					if  (difficalty == 0)
						strcpy(difficalty_str, "Очень легко");
					if (difficalty == 1)
						strcpy(difficalty_str, "Легко");
					if (difficalty == 2)
						strcpy(difficalty_str, "Средне");
				}
				if (choose == 1)
				{
					if (size == 3)
						size=max_size;
					else
						size--;
					move(14,local_x+26);
					printw("%i", size);
				}
				if (choose == 2)
				{
					if (default_side == 0)
						default_side=3;
					else
						default_side--;
					if  (default_side == 0)
						strcpy(default_side_str, "На выбор");
					if (default_side == 1)
						strcpy(default_side_str, "Крестики");
					if (default_side == 2)
						strcpy(default_side_str, "Нолики");
					if (default_side == 3)
						strcpy(default_side_str, "Случайно");
				}
				if (choose == 3)
				{
				}
			}
			if (key == '\033' || (key == '\n' && choose == 4))
			{
				return 0;
			}
		}
	}
}

void single_game()
{
	clear();
	start_color();
	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_BLUE, COLOR_BLACK);
	init_pair(3, COLOR_MAGENTA, COLOR_BLACK);
	init_pair(4, COLOR_CYAN, COLOR_BLACK);
//Инициализация и отрисовка первоначального игрового поля
	chtype space;
	space=' '|A_UNDERLINE;
	//int max_x=getmaxx(stdscr);
	x_begin=(getmaxx(stdscr)-6*size-1)/2;
	y_begin=4;
	for (i=3; i != size; i++)
		y_begin=y_begin-2;
	x=x_begin; y=y_begin;
	move(y,x-1);
	printw("x");
	move(y+1,x-3);
	printw("y  _");
	x=x+4;
	for (i=0; i < size; i++)
	{
		move(y,x-1);
		printw("%i", i+1);
		x=x+6;
	}
	y++; x=x_begin+1;
	for (i=0; i < size; i++)
	{
		move(y,x);
		printw("______");
		x=x+6;
	}
	y++;
	for (i=0; i < size; i++)
	{
		x=x_begin;
		move(y,x);
		printw("|");
		move(y+1,x-3);
		printw("%i  |", i+1);
		move(y+2,x);
		addch('|');
		for (j=0; j < size; j++)
		{
			move(y,x+1);
			printw("     |");
			move(y+1,x+1);
			printw("     |");
			move(y+2,x+1);
			addch(space); addch(space); addch(space); addch(space); addch(space);
			addch('|');
			x=x+6;
		}
		y=y+3;
	}
	refresh();
//Инициализация массива значений квадратов игрового поля
	char **array;
	array=new char*[size];
	for (i=0; i < size; i++)
	{
		array[i]=new char [size];
	}
	for (i=0; i < size; i++)
	{
		for (j=0; j < size; j++)
		{
			strcpy((*(array+i))+j, " ");
		}
	}
//Выбор игроком крестиков или ноликов
	bool player_turn;
	if (!default_side)
	{
		char side[20];
		move(18,5);
		printw("1 - X;  2 - O;  3 - Всё равно.");
		move(19,5);
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
				player_turn=true;
			else
				player_turn=false;
		}
	}
	if (default_side == 1)
		player_turn=true;
	if (default_side == 2)
		player_turn=false;
	if (default_side == 3)
	{
		if (rand()%2)
			player_turn=true;
		else
			player_turn=false;
	}
//Игра
	bool win=false, quit;
	while (!win)
	{
		if (player_turn)
		{
			quit=player('X', array);
			if (quit)
				break;
			win=check(1, array);
			if (win)
				break;
			cpu('O', array);
			win=check(0, array);
		}
		else
		{
			cpu('X', array);
			win=check(0, array);
			if (win)
				break;
			quit=player('O', array);
			if (quit)
				break;
			win=check(1, array);
		}
	}
//FIXME: Найти другой способ для быстрого выхода
	if (!quit)
		getch();
	attrset(A_NORMAL);
	for (i=0; i < size; i++)
	{
		delete[] array[i];
	}
	delete[] array;
}

void campaign()
{
	clear();
	move(getmaxy(stdscr)/2+1,(getmaxx(stdscr)-19)/2);
	printw("Ещё в разработке...");
	getch();
}

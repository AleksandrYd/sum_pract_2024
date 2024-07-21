#include "Game.h"
#include <Windows.h>


game::Game::Game(sf::RenderWindow& window, float fieldx, float fieldy, int pad, sf::Font& fon, int size_fon, int mode)
	:mywindow(window), pointY(fieldy), pointX(fieldx), padding(pad), font(fon), size_font(size_fon), gamemode(mode)
{

	chose_col = 0;
	chose_row = 0;

	text_tfield = new sf::Text * [26];
	for (int i = 0; i < 26; i++)
	{
		text_tfield[i] = new sf::Text[26];
	}

	text_field = new sf::Text *[26];
	for (int i = 0; i < 26; i++)
	{
		text_field[i] = new sf::Text[26];
	}

	sf::Vector2f field_size((float)padding * 9, (float)padding * 9);

	text_color = sf::Color::Black;

	squares = std::vector <sf::RectangleShape>(20);
	solved_squares = std::vector <sf::RectangleShape>(20);
	initRectangle(squares[0], sf::Color::Black, 5, (float)padding * 9, (float)padding * 9, 
		sf::Color(0, 0, 0, 0), pointX - 10, pointY + 7);
	initRectangle(solved_squares[0], sf::Color::Black, 5, (float)padding * 9, (float)padding * 9,
		sf::Color(0, 0, 0, 0), pointX - 10, pointY + 7);
	
	for (int i = 1, padx = padding, pady = padding; i < 9; i++, padx += padding, pady += padding)
	{
		if (i % 3 != 0)
		{
			initLine(squares[i], 1, field_size.y,
				sf::Color(0, 0, 0, 255), pointX - 10 + padx - 3, pointY + 7);
			initLine(solved_squares[i], 1, field_size.y,
				sf::Color(0, 0, 0, 255), pointX - 10 + padx - 3, pointY + 7);
		}
		else
		{
			initLine(squares[i], 5, field_size.y,
				sf::Color(0, 0, 0, 255), pointX - 10 + padx - 5, pointY + 7);
			initLine(solved_squares[i], 5, field_size.y,
				sf::Color(0, 0, 0, 255), pointX - 10 + padx - 5, pointY + 7);
		}

		if (i % 3 != 0)
		{
			initLine(squares[i + 8], field_size.x, 1,
				sf::Color(0, 0, 0, 255), pointX - 10, pointY + 7 + pady);
			initLine(solved_squares[i + 8], field_size.x, 1,
				sf::Color(0, 0, 0, 255), pointX - 10, pointY + 7 + pady);
		}
		else
		{
			initLine(squares[i + 8], field_size.x, 5,
				sf::Color(0, 0, 0, 255), pointX - 10, pointY + 7 + pady);
			initLine(solved_squares[i + 8], field_size.x, 5,
				sf::Color(0, 0, 0, 255), pointX - 10, pointY + 7 + pady);
		}
	}
	
	
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			field[i][j] = 0;
		}
	}

	for (int i = 0, ypos = pointY; i < 9; i++, ypos += padding)
	{
		for (int j = 0, xpos = pointX; j < 9; j++, xpos += padding)
		{
			number = std::to_string(field[i][j]);
			setInitText(text_field[i][j], number, size_font, xpos, ypos);
		}
	}
	if (gamemode == 1)
		text_field[chose_row][chose_col].setFillColor(sf::Color::Red);

	tfield = new int * [26];

	for (int i = 0; i < 26; i++)
	{
		tfield[i] = new int[26];
		for (int j = 0; j < 26; j++)
		{
			tfield[i][j] = field[i][j];
		}
	}
}

void game::Game::create_random_sudoku()
{
	int r;
	
	int** not_shuffled_field = new int* [26];
	for (int i = 0; i < 26; i++)
	{
		not_shuffled_field[i] = new int[26];
		for (int j = 0; j < 26; j++)
		{
			not_shuffled_field[i][j] = 0;
		}
	}
	int** arr = solve_sudoku(not_shuffled_field);

	int f[9][9];
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			f[i][j] = arr[i][j];
		}
	}
	int gg = 0;

	srand(time(0));
	for (int i = 0; i < 20; i++)
	{
		r = rand();
		switch (r%3)
		{
		case 0:
		{
			transposed_matrix(arr);
			break;
		}
		case 1:
		{
			swap_columns(arr);
			break;
		}
		case 2:
		{
			swap_rows(arr);
			break;
		}
		}
	}
	
	

	quant_tips = rand() % 5 + 5;
	elements_tips = std::vector <element>();
	int * nums;
	element el;
	for (int i = 0; i < quant_tips; i++)
	{
		nums = create_unique_elements();
		
		el.col = nums[1];
		el.row = nums[0];
		elements_tips.push_back(el);
	}

	for (int i = 0; i < quant_tips; i++)
	{
		field[elements_tips[i].row][elements_tips[i].col] = arr[elements_tips[i].row][elements_tips[i].col];
	}
	
	tfield = arr;
	update_text_field_mode0();
}

int** game::Game::solve_sudoku(int **arr)
{
	int** array = arr;
	bool allowed;
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			if (array[i][j] == 0)
			{
				for (int num = 1; num < 10; num++)
				{
					allowed = true;
					for (int k = 0; k < 9; k++)
					{
						if (array[k][j] == num || array[i][k] == num)
						{
							allowed = false;
							break;
						}
					}
					for (int k = 0; k < 3; k++)
					{
						for (int l = 0; l < 3; l++)
						{
							if (array[i - i % 3 + k][j - j % 3 + l] == num)
							{
								allowed = false;
								break;
							}
						}
					}
					if (allowed == true)
					{
						array[i][j] = num;
						int** itter = solve_sudoku(array);
						if (itter[25][25] != -1)
							return itter;
						else
						{
							array[25][25] = 0;
							array[i][j] = 0;
						}
					}
				}
				array[25][25] = -1;
				return array;
			}
		}
	}
	return array;
}

void game::Game::draw_mode0()
{
	
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			mywindow.draw(text_field[i][j]);
		}

	}

	for (int i = 0; i < 20; i++)
	{
		mywindow.draw(squares[i]);
	}
}

void game::Game::draw_mode1()
{
	update_text_field_mode1();
	for (int i = 0, ypos = pointY; i < 9; i++, ypos += padding)
	{
		for (int j = 0, xpos = pointX; j < 9; j++, xpos += padding)
		{
			mywindow.draw(text_field[i][j]);
		}
	}
	for (int i = 0; i < 20; i++)
	{
		mywindow.draw(squares[i]);
	}
}

void game::Game::move_cursor_right()
{
	chose_col++;
	if (chose_col > 8)
	{
		text_field[chose_row][chose_col-1].setFillColor(text_color);
		text_field[chose_row][0].setFillColor(sf::Color::Red);
		chose_col = 0;
	}
	else
	{
		text_field[chose_row][chose_col - 1].setFillColor(text_color);
		text_field[chose_row][chose_col].setFillColor(sf::Color::Red);
	}
}

void game::Game::move_cursor_left()
{
	chose_col--;
	if (chose_col < 0)
	{
		text_field[chose_row][chose_col + 1].setFillColor(text_color);
		text_field[chose_row][8].setFillColor(sf::Color::Red);
		chose_col = 8;
	}
	else
	{
		text_field[chose_row][chose_col + 1].setFillColor(text_color);
		text_field[chose_row][chose_col].setFillColor(sf::Color::Red);
	}
}

void game::Game::move_cursor_down()
{
	chose_row++;
	if (chose_row > 8)
	{
		text_field[chose_row - 1][chose_col].setFillColor(text_color);
		text_field[0][chose_col].setFillColor(sf::Color::Red);
		chose_row = 0;
	}
	else
	{
		text_field[chose_row - 1][chose_col].setFillColor(text_color);
		text_field[chose_row][chose_col].setFillColor(sf::Color::Red);
	}
}

void game::Game::move_cursor_up()
{
	chose_row--;
	if (chose_row < 0)
	{
		text_field[chose_row + 1][chose_col].setFillColor(text_color);
		text_field[8][chose_col].setFillColor(sf::Color::Red);
		chose_row = 8;
	}
	else
	{
		text_field[chose_row + 1][chose_col].setFillColor(text_color);
		text_field[chose_row][chose_col].setFillColor(sf::Color::Red);
	}
}

void game::Game::setInitText(sf::Text& text, sf::String str,int size_font, float xpos, float ypos)
{
	text.setFont(font);
	text.setFillColor(text_color);
	text.setString(str);
	text.setCharacterSize(size_font);
	//int b = text.getLocalBounds().width / 2 + padding / 2;
	text.setPosition(xpos, ypos);
}

void game::Game::initRectangle(sf::RectangleShape& rectangle, sf::Color linecolor, int linethick, float width,
	float height, sf::Color fillcolor, float posx, float posy)
{
	rectangle.setOutlineColor(linecolor);
	rectangle.setOutlineThickness(linethick);
	rectangle.setSize(sf::Vector2f(width, height));
	rectangle.setFillColor(fillcolor);
	rectangle.setPosition(sf::Vector2f(posx,posy));
}

void game::Game::initLine(sf::RectangleShape& rectangle, float width,
	float height, sf::Color fillcolor, float posx, float posy)
{
	rectangle.setSize(sf::Vector2f(width, height));
	rectangle.setFillColor(fillcolor);
	rectangle.setPosition(sf::Vector2f(posx, posy));
}

void game::Game::update_text_field_mode1()
{
	text_field[chose_row][chose_col].setString(std::to_string(field[chose_row][chose_col]));
}

void game::Game::update_text_field_mode0()
{
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			text_field[i][j].setString(std::to_string(field[i][j]));
		}
	}
	
}

void game::Game::set_field_num(int num)
{
	field[chose_row][chose_col] = num;
}

int** game::Game::get_temp_field()
{
	return tfield;
}

void game::Game::set_tfield(int** arr)
{
	tfield = arr;
}

void game::Game::update_tfield()
{
	for (int i = 0; i < 26; i++)
	{
		for (int j = 0; j < 26; j++)
		{
			tfield[i][j] = field[i][j];
		}
	}
}

void game::Game::reinitialize_fieldes()
{
	float newpositionX;
	float newpositionY;

	for (int i = 0, ypos = pointY; i < 9; i++, ypos += padding)
	{
		for (int j = 0; j < 9; j++)
		{
			newpositionX = text_field[i][j].getPosition().x -300;
			newpositionY = text_field[i][j].getPosition().y;
			text_field[i][j].setPosition(newpositionX, newpositionY);
			text_field[i][j].setFillColor(text_color);
		}
	}

	for (int i = 0, ypos = pointY; i < 9; i++, ypos += padding)
	{
		for (int j = 0, xpos = pointX + 300; j < 9; j++, xpos += padding)
		{
			number = std::to_string(tfield[i][j]);
			setInitText(text_tfield[i][j], number, size_font, xpos, ypos);
		}
	}

	for (int i = 0; i < 17; i++)
	{
		newpositionX = squares[i].getPosition().x;
		newpositionY = squares[i].getPosition().y;
		squares[i].setPosition(sf::Vector2f(newpositionX - 300, newpositionY));
		solved_squares[i].setPosition(sf::Vector2f(newpositionX + 300, newpositionY));
	}
}

void game::Game::show_result(int is_puzzle_solved) const
{
	float newposition;

	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			mywindow.draw(text_field[i][j]);
			mywindow.draw(text_tfield[i][j]);
		}
	}

	for (int i = 0; i < 18; i++)
	{
		newposition = squares[i].getPosition().x;
		mywindow.draw(squares[i]);
		mywindow.draw(solved_squares[i]);
	}

	if (is_puzzle_solved == 0)
	{
		sf::Text error = sf::Text(L"Судоку не решена", font, 60);
		error.setPosition(pointX + 300, pointY + 600);
		error.setFillColor(text_color);
		mywindow.draw(error);
	}
}

void game::Game::transposed_matrix(int**arr)
{
	int tempnum;
	for (int i = 0; i < 9; i++)
	{
		for (int j = i; j < 9; j++)
		{
			tempnum = arr[i][j];
			arr[i][j] = arr[j][i];
			arr[j][i] = tempnum;
		}
	}
	int f[9][9];
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			f[i][j] = arr[i][j];
		}
	}
	int gg = 0;
}
 
void game::Game::swap_rows(int**arr)
{
	int row1 = rand()%9;
	int row2 = rand()%9;
	while (row1 == row2)
	{
		row2 = rand()%9;
	}
	int tempnum;

	for (int i = 0; i < 9; i++)
	{
		tempnum = arr[row1][i];
		arr[row1][i] = arr[row2][i];
		arr[row2][i] = tempnum;
	}
	int f[9][9];
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			f[i][j] = arr[i][j];
		}
	}
	int gg = 0;
}

void game::Game::swap_columns(int** arr)
{
	int column1 = rand()%9;
	int column2 = rand()%9;
	int tempnum;
	while (column1 == column2)
	{
		column2 = rand()%9;
	}

	for (int i = 0; i < 9; i++)
	{
		tempnum = arr[i][column1];
		arr[i][column1] = arr[i][column2];
		arr[i][column2] = tempnum;
	}

	int f[9][9];
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			f[i][j] = arr[i][j];
		}
	}
	int gg = 0;
}

int* game::Game::create_unique_elements()
{
	int row = rand() % 9;
	int col = rand() % 9;
	if (elements_tips.size() == 0)
		return new int[2]{ row,col };
	int* arr = new int[2]{ row,col };
	for (int i = 0; i < elements_tips.size(); i++)
	{
		if (row == elements_tips[i].row && col == elements_tips[i].col)
			arr = create_unique_elements();
	}
	return arr;
}

bool game::Game::check_entered_field()
{
	int num;
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			num = field[i][j];
			if (num != 0)
			{
				for (int k = 0; k < 9; k++)
				{
					if ((field[k][j] == num && k != i || field[i][k] == num && k != j))
					{
						return false;
						break;
					}
				}
				for (int k = 0; k < 3; k++)
				{
					for (int l = 0; l < 3; l++)
					{
						if (field[i - i % 3 + k][j - j % 3 + l] == num && (i - i % 3 + k != i) && (j - j % 3 + l != j) )
						{
							return false;
							break;
						}
					}
				}
			}
		}
	}
	return true;
}
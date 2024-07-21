#pragma once
#include <SFML/Graphics.hpp>
#include <sstream>


namespace game
{
	struct element
	{
		int row, col;
	};

	class Game
	{
		
		int chose_col;
		int quant_tips;
		int chose_row;
		int padding;
		float pointX;
		float pointY;
		int field[26][26];
		int** tfield;
		sf::Text **text_tfield;
		sf::Text **text_field;
		std::vector <sf::RectangleShape> squares;
		std::vector <sf::RectangleShape> solved_squares;
		std::vector <element> elements_tips;
		sf::Color text_color;
		sf::Color chose_color;
		sf::Text text;
		sf::String number;
		int size_font;
		sf::Font& font;
		int gamemode;

		sf::RenderWindow& mywindow;

	public:
		


		Game(sf::RenderWindow& window, float fieldx, float fieldy, int pad, sf::Font& fon, int size_fon, int mode);
		~Game()
		{
			for (int i = 0; i < 9; i++)
			{
				for (int j = 0; j < 9; j++)
				{
					field[i][j] = NULL;
				}
			}
			delete[] text_field;
			for (int i = 0; i < 26; i++)
			{
				delete[] tfield[i];
				delete[] text_tfield[i];
			}
			delete[] tfield;
			delete[] text_tfield;

		}


		void create_random_sudoku();

		int ** solve_sudoku(int **arr);

		void draw_mode0();

		void draw_mode1();

		void move_cursor_right();

		void move_cursor_left();

		void move_cursor_down();

		void move_cursor_up();

		bool is_game_running();

		void setInitText(sf::Text& text, sf::String str, int size_font, float xpos, float ypos);

		void initRectangle(sf::RectangleShape& rect, sf::Color linecolor, int linethick, 
			float width, float height, sf::Color fillcolor, float posx, float posy);

		void initLine(sf::RectangleShape& rect,
			float width, float height, sf::Color fillcolor, float posx, float posy);

		void update_text_field_mode0();

		void update_text_field_mode1();

		void set_field_num(int num);

		int** get_temp_field();

		void set_tfield(int** arr);

		void show_result(int is_pazzle_solved) const;

		void update_tfield();

		void reinitialize_fieldes(); 

		int* create_unique_elements();

		void transposed_matrix(int** arr);

		void swap_rows(int** arr);

		void swap_columns(int** arr);

		bool check_entered_field();
	};
}


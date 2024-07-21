#pragma once
#include <SFML/Graphics.hpp>
namespace game 
{
	class GameMenu
	{
		float menu_X;
		float menu_Y;
		int menu_step;
		int max_menu;
		int size_font;
		int mainMenuSelected;
		sf::Font font;
		sf::Text* mainMenu;

		sf::Color menu_text_color = sf::Color(200, 0, 0, 255);
		sf::Color chose_text_color = sf::Color(0, 200, 0, 255);
		sf::Color border_color = sf::Color(0, 0, 200, 255);

		void setInitText(sf::Text& text, sf::String str, float xpos, float ypos);
		
		sf::RenderWindow& mywindow;
	
	public:

		GameMenu(sf::RenderWindow& window, float menux, float menuy, std::vector<sf::String>& name, int sizeFont = 50, int step = 60);
		~GameMenu()
		{
			delete[] mainMenu;
		}

		void draw();

		void MoveUp();

		void MoveDown();

		void setColorTextMenu(sf::Color menColor, sf::Color choColor);

		void AlignMenu(int posx);

		int getSelectedMeuNumber()
		{
			return mainMenuSelected;
		}
	};


}


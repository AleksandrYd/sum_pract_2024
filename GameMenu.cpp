#include "GameMenu.h"

void game::GameMenu::setInitText(sf::Text& text, sf::String str, float xpos, float ypos)
{
	text.setFont(font);
	text.setFillColor(menu_text_color);
	text.setString(str);
	text.setCharacterSize(size_font);
	text.setPosition(xpos, ypos);
	text.setOutlineThickness(3);
	text.setOutlineColor(border_color);
}

void game::GameMenu::AlignMenu(int posx)
{
	float nullx = 0;

	for (int i = 0; i < max_menu; i++)
	{
		switch (posx)
		{
		case 0:
			nullx = 0;
			break;
		case 1:
			nullx = mainMenu[i].getLocalBounds().width;
			break;
		case 2:
			nullx = mainMenu[i].getLocalBounds().width/2;
			break;
		}
		mainMenu[i].setPosition(mainMenu[i].getPosition().x - nullx, mainMenu[i].getPosition().y);
	}
}

game::GameMenu::GameMenu(sf::RenderWindow& window, float menux, float menuy, std::vector<sf::String>& name, int sizeFont, int step)
	:mywindow(window), menu_X(menux), menu_Y(menuy), size_font(sizeFont), menu_step(step)
{
	if (!font.loadFromFile("HomeVideo-Regular.otf")) exit(32);
	max_menu = name.size();
	mainMenu = new sf::Text[name.size()];

	for (int i = 0, ypos = menu_Y; i < max_menu; i++, ypos += menu_step)
		setInitText(mainMenu[i], name[i], menu_X, ypos);
	mainMenuSelected = 0;
	mainMenu[mainMenuSelected].setFillColor(chose_text_color);
}

void game::GameMenu::MoveUp()
{
	mainMenuSelected--;
	if (mainMenuSelected >= 0)
	{
		mainMenu[mainMenuSelected + 1].setFillColor(menu_text_color);
		mainMenu[mainMenuSelected].setFillColor(chose_text_color);
	}
	else
	{	
		mainMenu[0].setFillColor(menu_text_color);
		mainMenuSelected = max_menu - 1;
		mainMenu[mainMenuSelected].setFillColor(chose_text_color);
	}
}

void game::GameMenu::MoveDown()
{
	mainMenuSelected++;
	if (mainMenuSelected < max_menu)
	{
		mainMenu[mainMenuSelected - 1].setFillColor(menu_text_color);
		mainMenu[mainMenuSelected].setFillColor(chose_text_color);
	}
	else
	{
		mainMenu[max_menu - 1].setFillColor(menu_text_color);
		mainMenuSelected = 0;
		mainMenu[mainMenuSelected].setFillColor(chose_text_color);
	}
}

void game::GameMenu::draw()
{
	for (int i = 0; i < max_menu; i++)
		mywindow.draw(mainMenu[i]);
}

void game::GameMenu::setColorTextMenu(sf::Color menColor, sf::Color choColor)
{
	menu_text_color = menColor;
	chose_text_color = choColor;

	for (int i = 0; i < max_menu; i++)
		mainMenu[i].setFillColor(menu_text_color);

	mainMenu[mainMenuSelected].setFillColor(chose_text_color);
}

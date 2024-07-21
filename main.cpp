#include <SFML/Graphics.hpp>
#include "GameMenu.h"
#include "Game.h"
#include <conio.h>

using namespace sf;

void InitText(Text& text, float xpos, float ypos, String str, int size_font = 150,
    Color menu_text_color = Color(200, 0, 0, 255));
int GameStart(RenderWindow& window, int gamemode);
int AboutGame(RenderWindow &window);
bool check_solution(int** result);
const int show_unsolved_result(game::Game& mygame, RenderWindow& window);
const int show_solved_result(game::Game& mygame, RenderWindow& window);
void InitHeadingText(Text& text, float ypos, String str, int size_font,
    Color menu_text_color);

float padding = 60;
Font font = Font();

int choose_game_mode(RenderWindow &window, RectangleShape &bg)
{

    RenderWindow &mywindow = window;
    float width = VideoMode::getDesktopMode().width;
    float height = VideoMode::getDesktopMode().height;
    RectangleShape &background = bg;
    Text title;
    Event event;
    int gamemode;
    

    InitHeadingText(title, 50, L"Выберите судоку", 150, Color(255, 255, 0));
    

    std::vector <String> name_menu{ L"Создать случайную игру", L"Создать свою игру", L"Назад"};
    game::GameMenu mymenu(mywindow, 200, 350, name_menu, 100, 120);
    mymenu.setColorTextMenu(sf::Color(200, 0, 0, 255), sf::Color(0, 200, 0, 255));
    mymenu.AlignMenu(0);

    while (mywindow.isOpen())
    {
        while (mywindow.pollEvent(event))
        {
            if (event.type == Event::KeyReleased)
            {
                if (event.key.code == Keyboard::Up) mymenu.MoveUp();
                if (event.key.code == Keyboard::Down) mymenu.MoveDown();
                if (event.key.code == Keyboard::Escape) return 1;
                if (event.key.code == Keyboard::Enter)
                {
                    gamemode = mymenu.getSelectedMeuNumber();
                    switch (mymenu.getSelectedMeuNumber())
                    {
                    case 0: 
                    case 1: GameStart(mywindow, gamemode); break;
                    case 2: return 1; break;
                    }
                }
            }
        }

        mywindow.clear();
        mywindow.draw(background);
        mywindow.draw(title);
        mymenu.draw();
        mywindow.display();
    }

}

int GameStart(RenderWindow &window, int gamemode)
{
    RenderWindow& mywindow = window;
    Event event;
    Text interface;
    Texture texture_window;
    Text warning = Text();
    int mode = gamemode;
    bool is_game_run = 1;
    bool draw_warning = false;
    float width = VideoMode::getDesktopMode().width;
    float height = VideoMode::getDesktopMode().height;
    int size_font = 60;
    float half_screen = VideoMode::getDesktopMode().width / 2;
    float screen_vert = VideoMode::getDesktopMode().height;
    float size_field = (float)padding * 8;
    float coordx = half_screen-size_field/2;
    float coordy = 100;
    RectangleShape background(Vector2f(width, height));
    game::Game mygame(mywindow, coordx, coordy, padding, font, size_font, mode);
    Text control0, control1;

    InitText(control1, coordx-300, screen_vert - 300, L"Up, Down, Left, Right - перемещение курсора\nEnter - решить судоку\nВвод цифрами", 50, Color::Black);
    InitText(control0, coordx-50, screen_vert - 300, L"Enter - решить судоку", 50, Color::Black);
    InitText(warning, coordx-200, screen_vert - 430, L"Введены неверные значения \nЦифры не должны повторяться",
        60, Color::Red);

    if (!texture_window.loadFromFile("bg2.jpg")) return 4;
    background.setTexture(&texture_window);

    if (mode == 0)
    {
        mygame.create_random_sudoku();
        while (is_game_run == true)
        {
            while (mywindow.pollEvent(event))
            {
                if (event.type == Event::KeyReleased)
                {
                    if (event.key.code == Keyboard::Escape) return 1;
                    if (event.key.code == Keyboard::Enter)
                    {
                        show_solved_result(mygame, mywindow);
                        return 1;
                    }
                }

            }
            mywindow.clear();
            mywindow.draw(background);
            mywindow.draw(control0);
            mygame.draw_mode0();
            mywindow.display();
        }
    }
    else if (mode == 1)
    {
        
        while (is_game_run == true)
        {
            while (mywindow.pollEvent(event))
            {
                if (event.type == Event::KeyReleased)
                {
                    switch (event.key.code)
                    {
                    case Keyboard::Up: mygame.move_cursor_up(); break;
                    
                    case Keyboard::Down: mygame.move_cursor_down(); break;

                    case Keyboard::Right: mygame.move_cursor_right(); break;

                    case Keyboard::Left: mygame.move_cursor_left(); break;

                    case Keyboard::Escape: return 1;

                    case Keyboard::Enter:
                    {
                        mygame.update_tfield();
                        if (mygame.check_entered_field())
                        {
                            int** arr = mygame.get_temp_field();
                            int** result = mygame.solve_sudoku(arr);
                            mygame.set_tfield(result);
                            if (check_solution(result) == true)
                            {
                                show_solved_result(mygame, mywindow);
                                return 1;
                            }
                            else
                            {
                                show_unsolved_result(mygame, mywindow);
                                return 1;
                            }
                        }
                        else
                        {
                            draw_warning = true;
                            break;
                        }
                    }

                    case Keyboard::Numpad1: mygame.set_field_num(1); break;
                    case Keyboard::Numpad2: mygame.set_field_num(2); break;
                    case Keyboard::Numpad3: mygame.set_field_num(3); break;
                    case Keyboard::Numpad4: mygame.set_field_num(4); break;
                    case Keyboard::Numpad5: mygame.set_field_num(5); break;
                    case Keyboard::Numpad6: mygame.set_field_num(6); break;
                    case Keyboard::Numpad7: mygame.set_field_num(7); break;
                    case Keyboard::Numpad8: mygame.set_field_num(8); break;
                    case Keyboard::Numpad9: mygame.set_field_num(9); break;
                    }

                }

            }
            
            mywindow.clear();
            mywindow.draw(background);
            mywindow.draw(control1);
            mygame.draw_mode1();
            if (draw_warning)
            {
                mywindow.draw(warning);
            }
            mywindow.display();
            if (draw_warning)
            {
                sleep(seconds(3));
                draw_warning = false;
            }
        }
    }
}

int main()
{
    RenderWindow window(VideoMode::getDesktopMode(), L"Судоку", Style::Fullscreen);
    float width = VideoMode::getDesktopMode().width;
    float height = VideoMode::getDesktopMode().height;
    RectangleShape background(Vector2f(width, height));
    Texture texture_window;
    Text title;
    Event event;
    Image icon;

    if (!icon.loadFromFile("icon.jpg"))
    {
        return 1;
    }
    window.setIcon(48, 48, icon.getPixelsPtr());
    

    window.setVerticalSyncEnabled(true);
    window.setMouseCursorVisible(false);

    
    if (!texture_window.loadFromFile("bg.jpg")) return 4;
    background.setTexture(&texture_window);
    
    if (!font.loadFromFile("HomeVideo-Regular.otf")) return 5;
    
    title.setFont(font);
    
    InitHeadingText(title, 50, L"Судоку", 150, Color(255, 255, 0));

    std::vector <String> name_menu{ L"Решить", L"О игре", L"Выход" };
    game::GameMenu mymenu(window, 200, 350, name_menu, 100, 120);
    mymenu.setColorTextMenu(sf::Color(200, 0, 0, 255), sf::Color(0, 200, 0, 255));
    mymenu.AlignMenu(0);

    while (window.isOpen())
    {
        while (window.pollEvent(event))
        {
            if (event.type == Event::KeyReleased)
            {
                if (event.key.code == Keyboard::Up) mymenu.MoveUp();
                if (event.key.code == Keyboard::Down) mymenu.MoveDown();
                if (event.key.code == Keyboard::Enter)
                {
                    switch (mymenu.getSelectedMeuNumber())
                    {
                        case 0: choose_game_mode(window, background); break;
                        case 1: AboutGame(window); break;
                        case 2: window.close(); break;
                    }
                }
            }
        }

        window.clear();
        window.draw(background);
        window.draw(title);
        mymenu.draw();
        window.display();
    }
    return 0;
}

void InitHeadingText(Text& text, float ypos, String str, int size_font,
    Color menu_text_color)
{
    text.setFont(font);
    text.setCharacterSize(size_font);
    text.setString(str);
    text.setFillColor(menu_text_color);
    text.setPosition(VideoMode::getDesktopMode().width/2-text.getLocalBounds().width/2, ypos);
}

void InitText(Text& text, float xpos, float ypos, String str, int size_font,
    Color menu_text_color)
{
    text.setFont(font);
    text.setCharacterSize(size_font);
    text.setString(str);
    text.setFillColor(menu_text_color);
    text.setPosition(xpos, ypos);
}

int AboutGame(RenderWindow& window)
{
    Text about;
    Event event;
    float width = VideoMode::getDesktopMode().width;
    float height = VideoMode::getDesktopMode().height;
    RectangleShape background(Vector2f(width, height));
    RectangleShape frame(Vector2f(1610, 760));
    int counter = 0;
    Texture texture_window;
    if (!texture_window.loadFromFile("bg2.jpg")) return 4;
    background.setTexture(&texture_window);
    RectangleShape line_1(Vector2f(50, 8));
    RectangleShape line_2(Vector2f(33, 8));
    RectangleShape line_3(Vector2f(25, 8));
    RectangleShape line_4(Vector2f(50, 8));
    RectangleShape line_5(Vector2f(25, 8));
    RectangleShape line_6(Vector2f(33, 8));
    line_1.setPosition(1615+220, 850);
    line_1.rotate(90);
    line_2.setPosition(1606+220, 900);
    line_2.rotate(-45);
    line_3.setPosition(1606+220, 900);
    line_3.rotate(-135);
    line_4.setPosition(1615+220, 140);
    line_4.rotate(90);
    line_5.setPosition(1617+220, 140);
    line_6.setPosition(1617+220, 140);
    line_5.rotate(45);
    line_6.rotate(135);
    std::vector <RectangleShape> lines{ line_1,line_2,line_3,line_4,line_5, line_6 };
    for (int i = 0; i < 3; i++)
    {
        lines[i].setFillColor(Color(0,0,200));
    }
    for (int i = 3; i < 6; i++)
    {
        lines[i].setFillColor(Color(0, 0, 100));
    }

        String info_1 = L"\
    Игровое поле представляет собой квадрат\n\
    размером 9×9, разделённый на меньшие \n\
    квадраты со стороной в 3 клетки.\n\
    Таким образом, всё игровое поле \n\
    состоит из 81 клетки.\n\
    В них уже в начале игры стоят\n\
    некоторые числа (от 1 до 9),\n\
    называемые подсказками.\n\
    От игрока требуется заполнить\n\
    свободные клетки цифрами от 1 до 9 так, \n\
    чтобы в каждой строке, в каждом\n\
    столбце и в каждом малом квадрате 3×3";
        String info_2 = L"\
    размером 9×9, разделённый на меньшие \n\
    квадраты со стороной в 3 клетки.\n\
    Таким образом, всё игровое поле \n\
    состоит из 81 клетки.\n\
    В них уже в начале игры стоят\n\
    некоторые числа (от 1 до 9),\n\
    называемые подсказками.\n\
    От игрока требуется заполнить\n\
    свободные клетки цифрами от 1 до 9 так, \n\
    чтобы в каждой строке, в каждом\n\
    столбце и в каждом малом квадрате 3×3 \n\
    каждая цифра встречалась только один раз.";
        String info_3 = L"\
    квадраты со стороной в 3 клетки.\n\
    Таким образом, всё игровое поле \n\
    состоит из 81 клетки.\n\
    В них уже в начале игры стоят\n\
    некоторые числа (от 1 до 9),\n\
    называемые подсказками.\n\
    От игрока требуется заполнить\n\
    свободные клетки цифрами от 1 до 9 так, \n\
    чтобы в каждой строке, в каждом\n\
    столбце и в каждом малом квадрате 3×3 \n\
    каждая цифра встречалась только один раз.\n\
    Сложность судоку зависит от количества";

        String info_4 = L"\
    Таким образом, всё игровое поле \n\
    состоит из 81 клетки.\n\
    В них уже в начале игры стоят\n\
    некоторые числа(от 1 до 9), \n\
    называемые подсказками.\n\
    От игрока требуется заполнить\n\
    свободные клетки цифрами от 1 до 9 так, \n\
    чтобы в каждой строке, в каждом\n\
    столбце и в каждом малом квадрате 3×3 \n\
    каждая цифра встречалась только один раз.\n\
    Сложность судоку зависит от количества \n\
    изначально заполненных клеток и методов,";

        String info_5 = L"\
    состоит из 81 клетки.\n\
    В них уже в начале игры стоят\n\
    некоторые числа(от 1 до 9), \n\
    называемые подсказками.\n\
    От игрока требуется заполнить\n\
    свободные клетки цифрами от 1 до 9 так, \n\
    чтобы в каждой строке, в каждом\n\
    столбце и в каждом малом квадрате 3×3 \n\
    каждая цифра встречалась только один раз.\n\
    Сложность судоку зависит от количества \n\
    изначально заполненных клеток и методов,\n\
    которые нужно применять для её решения.";

        String info_6 = L"\
    В них уже в начале игры стоят\n\
    некоторые числа(от 1 до 9), \n\
    называемые подсказками.\n\
    От игрока требуется заполнить\n\
    свободные клетки цифрами от 1 до 9 так, \n\
    чтобы в каждой строке, в каждом\n\
    столбце и в каждом малом квадрате 3×3 \n\
    каждая цифра встречалась только один раз.\n\
    Сложность судоку зависит от количества \n\
    изначально заполненных клеток и методов, \n\
    которые нужно применять для её решения.\n\
    Самые простые решаются дедуктивно:";
        String info_7 = L"\
    некоторые числа(от 1 до 9),\n\
    называемые подсказками.\n\
    От игрока требуется заполнить\n\
    свободные клетки цифрами от 1 до 9 так, \n\
    чтобы в каждой строке, в каждом\n\
    столбце и в каждом малом квадрате 3×3 \n\
    каждая цифра встречалась только один раз.\n\
    Сложность судоку зависит от количества \n\
    изначально заполненных клеток и методов, \n\
    которые нужно применять для её решения.\n\
    Самые простые решаются дедуктивно:\n\
    всегда есть хотя бы одна клетка,";
        String info_8 = L"\
    называемые подсказками.\n\
    От игрока требуется заполнить\n\
    свободные клетки цифрами от 1 до 9 так, \n\
    чтобы в каждой строке, в каждом\n\
    столбце и в каждом малом квадрате 3×3 \n\
    каждая цифра встречалась только один раз.\n\
    Сложность судоку зависит от количества \n\
    изначально заполненных клеток и методов, \n\
    которые нужно применять для её решения.\n\
    Самые простые решаются дедуктивно:\n\
    всегда есть хотя бы одна клетка,\n\
    куда подходит только одно число.";
        String info_9 = L"\
    От игрока требуется заполнить\n\
    свободные клетки цифрами от 1 до 9 так, \n\
    чтобы в каждой строке, в каждом\n\
    столбце и в каждом малом квадрате 3×3 \n\
    каждая цифра встречалась только один раз.\n\
    Сложность судоку зависит от количества \n\
    изначально заполненных клеток и методов, \n\
    которые нужно применять для её решения.\n\
    Самые простые решаются дедуктивно:\n\
    всегда есть хотя бы одна клетка,\n\
    куда подходит только одно число.\n\
    Некоторые головоломки можно решить";
        String info_10 = L"\
    свободные клетки цифрами от 1 до 9 так, \n\
    чтобы в каждой строке, в каждом\n\
    столбце и в каждом малом квадрате 3×3 \n\
    каждая цифра встречалась только один раз.\n\
    Сложность судоку зависит от количества \n\
    изначально заполненных клеток и методов, \n\
    которые нужно применять для её решения.\n\
    Самые простые решаются дедуктивно:\n\
    всегда есть хотя бы одна клетка,\n\
    куда подходит только одно число.\n\
    Некоторые головоломки можно решить\n\
    за несколько минут, на другие можно";
        String info_11 = L"\
    чтобы в каждой строке, в каждом\n\
    столбце и в каждом малом квадрате 3×3 \n\
    каждая цифра встречалась только один раз.\n\
    Сложность судоку зависит от количества \n\
    изначально заполненных клеток и методов, \n\
    которые нужно применять для её решения.\n\
    Самые простые решаются дедуктивно:\n\
    всегда есть хотя бы одна клетка,\n\
    куда подходит только одно число.\n\
    Некоторые головоломки можно решить\n\
    за несколько минут, на другие можно\n\
    потратить несколько часов.";
        String info_12 = L"\
    столбце и в каждом малом квадрате 3×3 \n\
    каждая цифра встречалась только один раз.\n\
    Сложность судоку зависит от количества \n\
    изначально заполненных клеток и методов, \n\
    которые нужно применять для её решения.\n\
    Самые простые решаются дедуктивно:\n\
    всегда есть хотя бы одна клетка,\n\
    куда подходит только одно число.\n\
    Некоторые головоломки можно решить\n\
    за несколько минут, на другие можно\n\
    потратить несколько часов.\n\
    Правильно составленная головоломка";

        String info_13 = L"\
    каждая цифра встречалась только один раз.\n\
    Сложность судоку зависит от количества \n\
    изначально заполненных клеток и методов, \n\
    которые нужно применять для её решения.\n\
    Самые простые решаются дедуктивно : \n\
    всегда есть хотя бы одна клетка, \n\
    куда подходит только одно число.\n\
    Некоторые головоломки можно решить\n\
    за несколько минут, на другие можно\n\
    потратить несколько часов.\n\
    Правильно составленная головоломка\n\
    имеет только одно решение.";
        String info_14 = L"\
    Сложность судоку зависит от количества \n\
    изначально заполненных клеток и методов, \n\
    которые нужно применять для её решения.\n\
    Самые простые решаются дедуктивно : \n\
    всегда есть хотя бы одна клетка, \n\
    куда подходит только одно число.\n\
    Некоторые головоломки можно решить\n\
    за несколько минут, на другие можно\n\
    потратить несколько часов.\n\
    Правильно составленная головоломка\n\
    имеет только одно решение.\n\
    Тем не менее на некоторых сайтах";
        String info_15 = L"\
    изначально заполненных клеток и методов, \n\
    которые нужно применять для её решения.\n\
    Самые простые решаются дедуктивно : \n\
    всегда есть хотя бы одна клетка, \n\
    куда подходит только одно число.\n\
    Некоторые головоломки можно решить\n\
    за несколько минут, на другие можно\n\
    потратить несколько часов.\n\
    Правильно составленная головоломка\n\
    имеет только одно решение.\n\
    Тем не менее на некоторых сайтах \n\
    в интернете под видом усложнённых";
        String info_16 = L"\
    которые нужно применять для её решения.\n\
    Самые простые решаются дедуктивно : \n\
    всегда есть хотя бы одна клетка, \n\
    куда подходит только одно число.\n\
    Некоторые головоломки можно решить\n\
    за несколько минут, на другие можно\n\
    потратить несколько часов.\n\
    Правильно составленная головоломка\n\
    имеет только одно решение.\n\
    Тем не менее на некоторых сайтах\n\
    в интернете под видом усложнённых\n\
    головоломок пользователю предлагаются ";
        String info_17 = L"\
    Самые простые решаются дедуктивно : \n\
    всегда есть хотя бы одна клетка, \n\
    куда подходит только одно число.\n\
    Некоторые головоломки можно решить\n\
    за несколько минут, на другие можно\n\
    потратить несколько часов.\n\
    Правильно составленная головоломка\n\
    имеет только одно решение.\n\
    Тем не менее на некоторых сайтах\n\
    в интернете под видом усложнённых \n\
    головоломок пользователю предлагаются\n\
    варианты судоку с несколькими вариантами ";
        String info_18 = L"\
    всегда есть хотя бы одна клетка, \n\
    куда подходит только одно число.\n\
    Некоторые головоломки можно решить\n\
    за несколько минут, на другие можно\n\
    потратить несколько часов.\n\
    Правильно составленная головоломка\n\
    имеет только одно решение.\n\
    Тем не менее на некоторых сайтах \n\
    в интернете под видом усложнённых\n\
    головоломок пользователю предлагаются \n\
    варианты судоку с несколькими вариантами\n\
    решения, а также с ветвлениями";
        String info_19 = L"\
    куда подходит только одно число.\n\
    Некоторые головоломки можно решить\n\
    за несколько минут, на другие можно\n\
    потратить несколько часов.\n\
    Правильно составленная головоломка\n\
    имеет только одно решение.\n\
    Тем не менее на некоторых сайтах \n\
    в интернете под видом усложнённых\n\
    головоломок пользователю предлагаются \n\
    варианты судоку с несколькими вариантами\n\
    решения, а также с ветвлениями \n\
    самого хода решения.";
    std::vector <String> info{info_1,info_2,info_3,info_4,info_5,
    info_6,info_7,info_8,info_9,info_10,info_11,info_12,info_13,info_14,
    info_15,info_16,info_17,info_18, info_19};
    InitText(about, 40, 150, info_1, 65, Color::Black);
    frame.setPosition(180,140);
    frame.setFillColor(Color(0, 0, 0, 0));
    frame.setOutlineColor(Color(219,53,15));
    frame.setOutlineThickness(10);
    
    while (1)
    {
        while (window.pollEvent(event))
        {
            if (event.type == Event::KeyReleased)
            {
                if (event.key.code == Keyboard::Escape) return 1;
                if (event.key.code == Keyboard::Down)
                {
                    counter++;
                    
                    if (counter >= 18)
                    {
                        counter = 18;
                        for (int i = 0; i < 3; i++)
                        {
                            lines[i].setFillColor(Color(0, 0, 100));
                        }
                    }
                    else
                    {
                        for (int i = 3; i < 6; i++)
                        {
                            lines[i].setFillColor(Color(0, 0, 255));
                        }
                    }
                }
                if (event.key.code == Keyboard::Up)
                {
                    counter--;
                    
                    if (counter <= 0)
                    {
                        counter = 0;
                        for (int i = 3; i < 6; i++)
                        {
                            lines[i].setFillColor(Color(0, 0, 100));
                        }
                    }
                    else
                    {
                        for (int i = 0; i < 3; i++)
                        {
                            lines[i].setFillColor(Color(0, 0, 255));
                        }
                    }
                }
                if (event.key.code == Keyboard::Enter)
                {
                    return 1;
                }
            }
        }
        window.clear();
        window.draw(background);
        about.setString(info[counter]);
        window.draw(about);
        window.draw(frame);
        for (int i = 0; i < 6; i++)
        {
            window.draw(lines[i]);
        }
        window.display();
    }
}

bool check_solution(int** result)
{
    bool allowed = true;
    int** res = result;
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            int number = res[i][j];
            if (res[i][j] == 0)
            {
                allowed = false;
                return allowed;
            }
        }
    }
    return allowed;
}

const int show_solved_result(game::Game& mygame, RenderWindow &mywindow)
{
    sf::Event ev;
    bool stop = 0;
    int result_statement = 1;
    float width = VideoMode::getDesktopMode().width;
    float height = VideoMode::getDesktopMode().height;
    float screen_vert = VideoMode::getDesktopMode().height;
    float size_field = (float)padding * 8;
    float half_screen = VideoMode::getDesktopMode().width / 2;

    float coordx = half_screen - size_field / 2;
    RectangleShape background(Vector2f(width, height));
    Text control;
    InitText(control, coordx - 50, screen_vert - 300, L"Enter - Вернуться в меню", 50, Color::Black);
    
    Texture texture_window;
    if (!texture_window.loadFromFile("bg2.jpg")) return 4;
    background.setTexture(&texture_window);

    mygame.reinitialize_fieldes();
    mywindow.clear();
    mywindow.draw(background);
    mygame.show_result(result_statement);
    mywindow.draw(control);
    mywindow.display();
    while (!stop)
    {
        while (mywindow.pollEvent(ev))
        {
            if (ev.type == Event::KeyReleased)
            {
                if (ev.key.code == Keyboard::Enter)
                {
                    return 1;
                }

                if (ev.key.code == Keyboard::Escape) return 1;
            }
        }
    }

}

const int show_unsolved_result(game::Game& mygame, RenderWindow& mywindow)
{
    int result_statement = 0;
    sf::Event ev;
    bool stop = 0;
    float width = VideoMode::getDesktopMode().width;
    float height = VideoMode::getDesktopMode().height;
    RectangleShape background(Vector2f(width, height));
    Texture texture_window;
    if (!texture_window.loadFromFile("bg2.jpg")) return 4;
    background.setTexture(&texture_window);

    mygame.reinitialize_fieldes();
    mywindow.clear();
    mywindow.draw(background);
    mygame.show_result(result_statement);
    mywindow.display();
    while (!stop)
    {
        while (mywindow.pollEvent(ev))
        {
            if (ev.type == Event::KeyReleased)
            {
                if (ev.key.code == Keyboard::Enter)
                {
                    return 1;
                }

                if (ev.key.code == Keyboard::Escape) return 1;
            }
        }
    }
}
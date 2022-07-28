#include <SFML/Graphics.hpp>
#include "Animation.h"
#include "Box.h"
#include <list>
#include <windows.h>
#include "ExplotionAnimation.h"
#include <queue>
#include <iostream>

const int BoxWidth = 60;
const int BoxHeight = 60;
const int MaxColoumn = 10;
const int MaxRow=10;
const int StartFromX = 0;
const int StartFromY = 0;
int Score = 0;
int Moves = 40;
Box Map[MaxColoumn][MaxRow];
std::list<Animation> MoveAnimations;
std::list<ExplotionAnimation> ExplotionAnimations;

std::vector<sf::Vector2i> CheckHorizontal(int x, int y)
{
	std::vector<sf::Vector2i> result;
	int checkX = x;
	Box currentbox = Map[x][y];
	while (currentbox.kind == Map[checkX + 1][y].kind && checkX < MaxColoumn)
	{
		result.push_back(sf::Vector2i(checkX + 1, y));
		checkX++;
	}
	checkX = x;
	while (currentbox.kind == Map[checkX - 1][y].kind && checkX > 0)
	{
		result.push_back(sf::Vector2i(checkX - 1, y));
		checkX--;
	}
	return result;
}
std::vector<sf::Vector2i> CheckVertical(int x, int y)
{
	std::vector<sf::Vector2i> result;
	int checkY = y;
	Box currentbox = Map[x][y];
	while (currentbox.kind == Map[x][checkY + 1].kind && checkY < MaxRow)
	{
		result.push_back(sf::Vector2i(x, checkY + 1));
		checkY++;
	}
	checkY = y;
	while (currentbox.kind == Map[x][checkY - 1].kind && checkY > 0)
	{
		result.push_back(sf::Vector2i(x, checkY - 1));
		checkY--;
	}
	return result;
}
bool CanMatch()
{
	for (int i = 0; i < MaxColoumn; i++)
	{
		for (int j = 0; j < MaxRow; j++)
		{
			Box currentbox = Map[i][j];
			if (currentbox.kind == -1 || currentbox.moving)
				return false;
		}
	}
	return true;
}
bool CanExplode(int x, int y)
{
	return CheckHorizontal(x, y).size() >= 2 || CheckVertical(x, y).size() >= 2;
}
bool CanSwap(int x1,int y1 ,int x2,int y2)
{
	return CanExplode(x1, y1) || CanExplode(x2, y2);
}
void Explode(int x, int y)
{
	Map[x][y].kind = -1;
	Score += 1;
	
	//ExplotionAnimations.emplace_back(12, 49, 49, Map[x][y].x, Map[x][y].y, 0.03, "Explosion.png");
	ExplotionAnimations.emplace_back(4, 60, 60, Map[x][y].x, Map[x][y].y, 0.09, "1.png");
}
bool CheckBox(int x, int y)
{
	auto hor = CheckHorizontal(x, y);
	auto ver = CheckVertical(x, y);
	if (hor.size() < 2 && ver.size() < 2)
		return false;
	Explode(x, y);
	if (hor.size() >= 2)
	{
		for (const auto z : hor)
		{
			Explode(z.x, z.y);
		}
	}
	if (ver.size() >= 2)
	{
		for (const auto z : ver)
		{
			Explode(z.x, z.y);
		}
	}
	return true;
}
void SwapBox(int x1, int y1, int x2, int y2, bool userswap = false)
{
	if (std::abs(x1 - x2) > 1 || std::abs(y1 - y2) > 1 || (x1!=x2 && y2!=y1) || (x1==x2&&y1==y2))
		return;

	Box& oldBox = Map[x1][y1];
	Box& NewBox = Map[x2][y2];
	if (oldBox.moving || NewBox.moving)
		return;
	std::swap(oldBox.row, NewBox.row);
	std::swap(oldBox.col, NewBox.col);
	std::swap(oldBox, NewBox);
	if(userswap)
		Moves --;

	if (CanSwap(x1, y1, x2, y2) || !userswap)
	{
		MoveAnimations.push_back(Animation(sf::Vector2i(oldBox.x, oldBox.y), sf::Vector2i(NewBox.x, NewBox.y), 0.003, oldBox));
		MoveAnimations.push_back(Animation(sf::Vector2i(NewBox.x, NewBox.y), sf::Vector2i(oldBox.x, oldBox.y), 0.003, NewBox));
	}
	else if (userswap)
	{
		std::swap(oldBox.row, NewBox.row);
		std::swap(oldBox.col, NewBox.col);
		std::swap(oldBox, NewBox);
		MoveAnimations.push_back(Animation(sf::Vector2i(oldBox.x, oldBox.y), sf::Vector2i(NewBox.x, NewBox.y), 0.003, oldBox, true));
		MoveAnimations.push_back(Animation(sf::Vector2i(NewBox.x, NewBox.y), sf::Vector2i(oldBox.x, oldBox.y), 0.003, NewBox, true));
	}
}
void GenerateMap()
{
	for (int i = 0; i < MaxColoumn; i++)
	{
		for (int j = 0; j < MaxRow; j++)
		{
			Map[i][j].kind = rand() % 6;
			Map[i][j].col = i;
			Map[i][j].row = j;
			Map[i][j].x = StartFromX + i * BoxWidth;
			Map[i][j].y = StartFromY + j * BoxHeight;
		}
	}
	for (int i = 0; i < MaxColoumn; i++)
	{
		for (int j = 0; j < MaxRow; j++)
		{
			while (CanExplode(i, j))
				Map[i][j].kind = rand() % 6;
		}
	}
}
void DoAnimations(sf::RenderWindow &window,float secs)
{
	
		for (auto an = MoveAnimations.begin(); an != MoveAnimations.end();)
		{
			if (!an->Update(secs))
				an = MoveAnimations.erase(an);
			else
				++an;
		}
	for (auto an = ExplotionAnimations.begin(); an != ExplotionAnimations.end();)
	{
		if (!an->NextFrame(secs))
			an = ExplotionAnimations.erase(an);
		else
		{
			if (an->Sprite.getTextureRect().width == an->Sprite.getTexture()->getSize().x)
				continue;
			window.draw(an->Sprite);
			++an;
		}
	}
}
void start()
{
	bool GameOver=false;
	Moves = 40;
	Score = 0;
	sf::Vector2i Oldpos;
	sf::Vector2i Newpos;

	//sf::RenderWindow window(sf::VideoMode(BoxWidth*MaxColoumn + StartFromX, BoxHeight*MaxRow +StartFromY), "Candy 5arya");
	sf::RenderWindow window(sf::VideoMode(808, 600), "Candy Crush");

	sf::Texture t1;
	t1.loadFromFile("gems.png");
	sf::Sprite gems(t1);
	sf::Clock time;

	// loading font
	sf::Font font;
	font.loadFromFile("Matilda.ttf");
	// score
	sf::Text score;
	score.setFillColor(sf::Color::Red);
	score.setPosition(BoxWidth*MaxColoumn + 10, 280);
	score.setCharacterSize(24);
	score.setFont(font);
	// moves
	sf::Text moves;
	moves.setFillColor(sf::Color::Red);
	moves.setPosition(BoxWidth*MaxColoumn + 10, 310);
	moves.setCharacterSize(24);
	moves.setFont(font);
	//game over
	sf::Text gameOver;
	gameOver.setFillColor(sf::Color::Red);
	gameOver.setPosition(200, 100);
	gameOver.setCharacterSize(80);
	gameOver.setFont(font);
	gameOver.setString("Game Over");

	//background
	sf::Texture bgTexture;
	bgTexture.loadFromFile("background.png");
	sf::Sprite background(bgTexture);
	background.setPosition(0, 0);
	GenerateMap();
	while (window.isOpen())
	{

		sf::Event event;
		float secs = time.restart().asSeconds();
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::MouseButtonPressed)
			{
				if (event.key.code == sf::Mouse::Left)
				{
					Oldpos = sf::Mouse::getPosition(window);
				}
			}
			if (event.type == sf::Event::MouseButtonReleased)
			{
				if (event.key.code == sf::Mouse::Left)
				{
					Newpos = sf::Mouse::getPosition(window);
					int x1 = (Oldpos.x - StartFromX) / BoxWidth;
					int y1 = (Oldpos.y - StartFromY) / BoxHeight;
					int x2 = (Newpos.x - StartFromX) / BoxWidth;
					int y2 = (Newpos.y - StartFromY) / BoxHeight;
					SwapBox(x1, y1, x2, y2, true);
				}
			}
			if (event.key.code == sf::Keyboard::Space && GameOver)
			{
				return;
			}
		}
		window.clear();
		if (GameOver)
		{
			window.draw(gameOver);
			window.draw(score);
			window.draw(moves);
			window.display();
			continue;
		}
		window.draw(background);

		DoAnimations(window, secs);


		for (int i = 0; i < MaxColoumn; i++)
		{
			for (int j = 0; j < MaxRow; j++)
			{

				Box& currentbox = Map[i][j];

				if (!currentbox.moving)
				{
					if (currentbox.kind == -1 && j > 0)
					{
						Box upperbox = Map[i][j - 1];
						if (upperbox.kind == -1)
							continue;
						SwapBox(i, j, i, j - 1);
					}

					if (j == 0 && currentbox.kind == -1)
						currentbox.kind = rand() % 6;

					if (CanMatch())
						CheckBox(i, j);


				}
				gems.setTextureRect(sf::IntRect(currentbox.kind * BoxWidth, 0, BoxWidth, BoxHeight));
				gems.setPosition(currentbox.x, currentbox.y);
				window.draw(gems);
			}
		}
		score.setString("Score : " + std::to_string(Score));
		moves.setString("Moves left : " + std::to_string(Moves));
		window.draw(score);
		window.draw(moves);
		window.display();
		if (Moves <= 0)
		{
			GameOver = true;
		}
	}
}
int main()
{
	
		srand(time(nullptr));
		while (true)
		{
			start();
		}
		
	return 0;
}

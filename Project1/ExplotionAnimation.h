#pragma once
#include <SFML/Graphics/Sprite.hpp>
#include <string>
#include <SFML/Graphics/Texture.hpp>

class ExplotionAnimation
{
	private:
	int Frames, Height, Width,X,Y;
	float SwitchTime,CurrentTime;
	sf::Texture Texture;
	public:
	int CurrentFrame;
	sf::Sprite Sprite;
	ExplotionAnimation(int frames, int height, int width, int x, int y,float switchtime, std::string path);
	~ExplotionAnimation();
	bool NextFrame(float secs);
};


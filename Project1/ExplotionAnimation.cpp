#include "ExplotionAnimation.h"
#include <SFML/Graphics/Texture.hpp>


ExplotionAnimation::ExplotionAnimation(int frames, int height, int width, int x, int y, float switchtime, std::string path) : CurrentFrame(0), CurrentTime(0)
{
	Frames = frames;
	Height = height;
	Width = width;
	X = x;
	Y = y;
	Texture.loadFromFile(path);
	Sprite.setTexture(Texture);
	SwitchTime = switchtime;
}

bool ExplotionAnimation::NextFrame(float time)
{
	if (CurrentFrame / Width == Frames)
	{
		return false;
	}
	CurrentTime += time;
	if (SwitchTime > CurrentTime)
		return true;
	CurrentTime =0;
	Sprite.setTextureRect(sf::IntRect(CurrentFrame, 0, Width, Height));
	Sprite.setPosition(X, Y);
	CurrentFrame += Width;
	return true;
}

ExplotionAnimation::~ExplotionAnimation()
{
}


#pragma once
#include <SFML/System/Vector2.hpp>
#include "Box.h"


class Animation
{
	private:
	float Switchtime,CurrentTime;
	sf::Vector2i CurrentPos, NewPos;
	Box &Box;
	bool GoBack;
	public:
	bool Update(float);
	Animation(sf::Vector2i, sf::Vector2i,float, ::Box&,bool=false);
	~Animation();
};


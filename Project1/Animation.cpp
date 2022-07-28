#include "Animation.h"


Animation::Animation(sf::Vector2i oldPos, sf::Vector2i newPos, float switchtime, ::Box& box, bool goback) :Box(box), CurrentTime(0),GoBack(false)
{
	CurrentPos = oldPos;
	NewPos = newPos;
	Switchtime = switchtime;
	Box.moving = true;
	GoBack = goback;
}


Animation::~Animation()
{
}
bool Animation::Update(float Time)
{
	CurrentTime += Time;
	if (Switchtime > CurrentTime)
		return true;
	CurrentTime =0;
	if (Box.x != NewPos.x)
		Box.x > NewPos.x ? Box.x-- : Box.x++;
	if (Box.y != NewPos.y)
		Box.y > NewPos.y ? Box.y-- : Box.y++;
	
	if (Box.x == NewPos.x && Box.y == NewPos.y)
	{
		if (GoBack)
		{
			NewPos.x = CurrentPos.x;
			NewPos.y = CurrentPos.y;
			GoBack = false;
			return true;
		}
		Box.moving = false;
		return false;
	}
	return true;
	
	
}

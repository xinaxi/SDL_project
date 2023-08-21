#include <SDL2/SDL.h>

class BigPoint
{
public:
	// The size of array
	static const int size = 11;
	
	BigPoint()
	{
		int c = 10;
		points[0]={c,c};
		for (int i = -1; i <= 1; i++)
		{	
			points[i+2] = {c+i,c-1};
			points[i+5] = {c+i,c+1};
		}
		for (int i = 1; i<=2; i++)
		{
			points[6+i] = {c+i,c};
			points[8+i] = {c-i,c};
		}
	}

	BigPoint(int x, int y): BigPoint() {
		setPoint(x, y);
	}

	void move(int dirX, int dirY)
	{
		for (int i = 0; i<size; i++)
		{
			points[i].x += dirX*speed;
			points[i].y += dirY*speed;
		}
	}

	void moveTo(int x, int y) {
		int dirX = x - getCenter().x;
		int dirY = y - getCenter().y;
		move(dirX, dirY);
	}

	void setPoint(int x, int y) {
		int dirX = x - getCenter().x;
		int dirY = y - getCenter().y;
		for (int i = 0; i<size; i++)
		{
			points[i].x += dirX;
			points[i].y += dirY;
		}
		
	}

	bool touchingRect(SDL_Rect r) {
		if (getCenter().x > r.x - 3 &&
			getCenter().x < r.x + r.w + 3 &&
			getCenter().y > r.y - 2 &&
			getCenter().y < r.y + r.h + 2)
			return true;
		return false;
	}

	// Array of point to draw 
	SDL_Point* getPoints() {
		return points;
	}

	SDL_Point getCenter()
	{
		return points[0];
	}

	void setSpeed(int newSpeed)
	{
		speed = newSpeed;
	}

private:
	SDL_Point points[BigPoint::size];
	int speed = 1;
};


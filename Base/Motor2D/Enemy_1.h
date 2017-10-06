#ifndef __ENEMY_BROWNCOOKIE_H__
#define __ENEMY_BROWNCOOKIE_H__

#include "Enemy.h"

class Enemy_1 : public Enemy 
{

private:
	iPoint original_pos;
	Animation fly;
	
public:
	Enemy_1(int x, int y);
	~Enemy_1();
	void Move();
	int value_between(int,int);
	//anim
	Animation animation;
};
#endif

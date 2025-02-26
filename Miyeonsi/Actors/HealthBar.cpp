#include "HealthBar.h"
#include "Actors/Player.h"

HealthBar::HealthBar()
{
	image = "none";
	reverse = false;
	color = Color::Red;
	curHP = 0;
	owner = nullptr;
}

HealthBar::~HealthBar()
{
}


void HealthBar::Update(float deltaTime)
{
	Super::Update(deltaTime);
	int loop = 250;
	int hp = 0;
	if (owner)
	{
		if (owner->GetReverse())
			reverse = false;
		else
			reverse = true;
		hp = owner->GetHP();
	}
	std::string buf;
	if (loop > 0)
	{
		for (int i = 0; i < 10; i++)
		{
			for (int j = 0; j < loop; j++)
			{
				if (j > hp)
					buf += ' ';
				else
					buf += '#';
			}
			buf += '\n';
		}
	}

	image = buf;

}

void HealthBar::Draw()
{
	Super::Draw();
}

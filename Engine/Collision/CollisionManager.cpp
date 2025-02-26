#include "PreCompiledHeader.h"
#include "CollisionManager.h"
#include "Actor/Actor.h"
#include "Actor/BoundingBox.h"
#include "Level/Level.h"


CollisionManager::CollisionManager()
{
	boxes.resize((int)LAYER::NONE);
}

CollisionManager::~CollisionManager()
{

}

void CollisionManager::Update()
{
	for (int i = 0; i < boxes.size(); i++)
	{
		boxes[i].clear();
	}

	if (cur_level)
	{
		std::vector<Actor*> actors = cur_level->GetActorList();
		if (!actors.empty())
		{
			for (int i = 0; i < actors.size(); i++)
			{
				if (BoundingBox* box = actors[i]->As<BoundingBox>())
				{
					LAYER layer = box->GetLayerType();
					switch (layer)
					{
					case LAYER::RYU:
						boxes[(int)LAYER::RYU].push_back(box);
						break;
					case LAYER::KEN:
						boxes[(int)LAYER::KEN].push_back(box);
						break;
					case LAYER::GROUND:
						boxes[(int)LAYER::GROUND].push_back(box);
						break;
					case LAYER::RYUBODY:
						boxes[(int)LAYER::RYUBODY].push_back(box);
						break;
					case LAYER::KENBODY:
						boxes[(int)LAYER::KENBODY].push_back(box);
						break;
					case LAYER::KENPROJECTILE:
						boxes[(int)LAYER::KENPROJECTILE].push_back(box);
						break;
					case LAYER::RYUPROJECTILE:
						boxes[(int)LAYER::RYUPROJECTILE].push_back(box);
						break;
					case LAYER::NONE:
						break;
					default:
						break;
					}
				}
				else
				{
					continue;
				}
			}
		}

		for (int i = 0; i < (int)LAYER::NONE; i++)
		{
			for (int j = i; j < (int)LAYER::NONE; j++)
			{
				if (check_flag[i][j] == true)
				{
					CheckCollision((LAYER)i, LAYER(j));
				}

			}
		}

	}
}

void CollisionManager::SetCollision(LAYER _first, LAYER _second)
{
	check_flag[(int)_first][(int)_second] = true;
	check_flag[(int)_second][(int)_first] = true;
}

void CollisionManager::CheckCollision(LAYER _first, LAYER _second)
{
	if (boxes[(int)_first].empty() || boxes[(int)_second].empty())
		return;

	for (int i = 0; i < boxes[(int)_first].size(); i++)
	{
		for (int j = 0; j < boxes[(int)_second].size(); j++)
		{

			if (boxes[(int)_first][i]->isExpired || boxes[(int)_second][j]->isExpired ||
				boxes[(int)_first][i] == nullptr || boxes[(int)_second][j] == nullptr)
			{
				continue;
			}

			int a_x_max = boxes[(int)_first][i]->x_max;
			int a_x_min = boxes[(int)_first][i]->x_min;
			int a_y_max = boxes[(int)_first][i]->y_max;
			int a_y_min = boxes[(int)_first][i]->y_min;

			int b_x_max = boxes[(int)_second][j]->x_max;
			int b_x_min = boxes[(int)_second][j]->x_min;
			int b_y_max = boxes[(int)_second][j]->y_max;
			int b_y_min = boxes[(int)_second][j]->y_min;

			bool xInspector = (a_x_max < b_x_min || a_x_min > b_x_max);
			bool yInspector = (a_y_max < b_y_min || a_y_min > b_y_max);
			
			// 충돌 발생 상황
			if (!xInspector && !yInspector)
			{
				// 이전에도 충돌중이었다
				if (boxes[((int)_first)][i]->collided == true && boxes[(int)_second][j]->collided == true)
				{
					boxes[(int)_first][i]->collided = true;
					boxes[(int)_second][j]->collided = true;
					boxes[(int)_second][j]->OnCollision(boxes[(int)_first][i]);
					boxes[(int)_first][i]->OnCollision(boxes[(int)_second][j]);
					boxes[(int)_first][i]->color = Color::Red;
					boxes[(int)_second][j]->color = Color::Red;
				}

				// 이전엔 충돌하지 않았다
				else
				{
					boxes[(int)_first][i]->collided = true;
					boxes[(int)_second][j]->collided = true;
					boxes[(int)_second][j]->OnCollisionEnter(boxes[(int)_first][i]);
					boxes[(int)_first][i]->OnCollisionEnter(boxes[(int)_second][j]);
					boxes[(int)_first][i]->color = Color::Red;
					boxes[(int)_second][j]->color = Color::Red;
				}
			}

			// 충돌이 일어나지 않음
			else
			{
				// 이전에도 충돌하지 않음 (아무것도 호출하지 않음)
				if (boxes[((int)_first)][i]->collided == false || boxes[(int)_second][j]->collided == false)
				{
					boxes[(int)_first][i]->collided = false;
					boxes[(int)_second][j]->collided = false;
					boxes[(int)_first][i]->color = Color::Green;
					boxes[(int)_second][j]->color = Color::Green;
				}

				// 이전엔 충돌중이였음(충돌 종료 함수 호출)
				else if (boxes[((int)_first)][i]->collided == true || boxes[(int)_second][j]->collided == true)
				{
					boxes[(int)_first][i]->collided = false;
					boxes[(int)_second][j]->collided = false;
					boxes[(int)_second][j]->CollisionEnd(boxes[(int)_first][i]);
					boxes[(int)_first][i]->CollisionEnd(boxes[(int)_second][j]);
					boxes[(int)_first][i]->color = Color::Green;
					boxes[(int)_second][j]->color = Color::Green;
				}
				
			}
		}
	}
}



#include "Player.h"
#include "Engine/Engine.h"
#include "Math/Vector2.h"
#include "Actor/BoundingBox.h"
#include "Engine/Timer.h"
#include "Level/Level.h"
#include "Projectile.h"
#include <cmath>

Player::Player()
	: Super()
	, moveState(MOVE_STATE::IDLE)
	, attackState(ATTACK_STATE::NONE)
	, playerState(PLAYER_STATE::NONE)
	, specialMove(SPECIAL::NONE)
{
	AddAnimation("Assets\\imgs\\RyuAnimComplete\\Crawl\\images\\", 0.16f);
	AddAnimation("Assets\\imgs\\RyuAnimComplete\\DownHardKick\\images\\", 0.16f);
	AddAnimation("Assets\\imgs\\RyuAnimComplete\\DownHardPunch\\images\\", 0.16f);
	AddAnimation("Assets\\imgs\\RyuAnimComplete\\DownHit\\images\\", 0.16f);
	AddAnimation("Assets\\imgs\\RyuAnimComplete\\DownLightKick\\images\\", 0.16f);
	AddAnimation("Assets\\imgs\\RyuAnimComplete\\DownLightPunch\\images\\", 0.16f);
	AddAnimation("Assets\\imgs\\RyuAnimComplete\\DownMiddleKick\\images\\", 0.16f);
	AddAnimation("Assets\\imgs\\RyuAnimComplete\\DownMiddlePunch\\images\\", 0.16f);
	AddAnimation("Assets\\imgs\\RyuAnimComplete\\ForwardHardKick\\images\\", 0.16f);
	AddAnimation("Assets\\imgs\\RyuAnimComplete\\ForwardHardPunch\\images\\", 0.16f);
	AddAnimation("Assets\\imgs\\RyuAnimComplete\\ForwardJump\\images\\", 0.08f);
	AddAnimation("Assets\\imgs\\RyuAnimComplete\\ForwardJumpMiddleHardKick\\images\\", 0.16f);
	AddAnimation("Assets\\imgs\\RyuAnimComplete\\ForwardLightKick\\images\\", 0.16f);
	AddAnimation("Assets\\imgs\\RyuAnimComplete\\ForwardLightMiddleKick\\images\\", 0.16f);
	AddAnimation("Assets\\imgs\\RyuAnimComplete\\ForwardLightMiddlePunch\\images\\", 0.16f);
	AddAnimation("Assets\\imgs\\RyuAnimComplete\\ForwardLightPunch\\images\\", 0.16f);
	AddAnimation("Assets\\imgs\\RyuAnimComplete\\ForwardMiddleKick\\images\\", 0.16f);
	AddAnimation("Assets\\imgs\\RyuAnimComplete\\ForwardMiddlePunch\\images\\", 0.16f);
	AddAnimation("Assets\\imgs\\RyuAnimComplete\\Hadouken\\images\\", 0.16f);
	AddAnimation("Assets\\imgs\\RyuAnimComplete\\Hit\\images\\", 0.16f);
	AddAnimation("Assets\\imgs\\RyuAnimComplete\\HitFace\\images\\", 0.16f);
	AddAnimation("Assets\\imgs\\RyuAnimComplete\\Idle\\images\\", 0.3f);
	AddAnimation("Assets\\imgs\\RyuAnimComplete\\JumpHardKick\\images\\", 0.16f);
	AddAnimation("Assets\\imgs\\RyuAnimComplete\\JumpLightMiddleKick\\images\\", 0.5f);
	AddAnimation("Assets\\imgs\\RyuAnimComplete\\JumpPunch\\images\\", 0.16f);
	AddAnimation("Assets\\imgs\\RyuAnimComplete\\LightPunch\\images\\", 0.16f);
	AddAnimation("Assets\\imgs\\RyuAnimComplete\\MiddlePunch\\images\\", 0.16f);
	AddAnimation("Assets\\imgs\\RyuAnimComplete\\NormalJump\\images\\", 0.16f);
	AddAnimation("Assets\\imgs\\RyuAnimComplete\\Projectile\\images\\", 0.16f);
	AddAnimation("Assets\\imgs\\RyuAnimComplete\\RyuCatchBack\\images\\", 0.16f);
	AddAnimation("Assets\\imgs\\RyuAnimComplete\\RyuForwardCatch\\images\\", 0.16f);
	AddAnimation("Assets\\imgs\\RyuAnimComplete\\RyuGuard\\images\\", 0.16f);
	AddAnimation("Assets\\imgs\\RyuAnimComplete\\RyuHardKick\\images\\", 0.16f);
	AddAnimation("Assets\\imgs\\RyuAnimComplete\\RyuKO\\images\\", 0.4f);
	AddAnimation("Assets\\imgs\\RyuAnimComplete\\RyuStumbleAndRecover\\images\\", 0.16f);
	AddAnimation("Assets\\imgs\\RyuAnimComplete\\RyuWalking\\images\\", 0.16f);
	AddAnimation("Assets\\imgs\\RyuAnimComplete\\Senpuken\\images\\", 0.16f);
	AddAnimation("Assets\\imgs\\RyuAnimComplete\\Soryuken\\images\\", 0.16f);

	sounds[0] = new SoundObject();
	sounds[0]->LoadSound("Assets\\sound\\Move\\Blocked.mp3");
	sounds[1] = new SoundObject();
	sounds[1]->LoadSound("Assets\\sound\\Move\\HardAttack.mp3");
	sounds[2] = new SoundObject();
	sounds[2]->LoadSound("Assets\\sound\\Move\\JabHit.mp3");
	sounds[3] = new SoundObject();
	sounds[3]->LoadSound("Assets\\sound\\Move\\LightAttack.mp3");
	sounds[4] = new SoundObject();
	sounds[4]->LoadSound("Assets\\sound\\Move\\MediumAttack.mp3");
	sounds[5] = new SoundObject();
	sounds[5]->LoadSound("Assets\\sound\\Move\\StrongHit.mp3");
	sounds[6] = new SoundObject();
	sounds[6]->LoadSound("Assets\\sound\\Ryu\\Hadouken.mp3");
	sounds[7] = new SoundObject();
	sounds[7]->LoadSound("Assets\\sound\\Ryu\\Senpuken.mp3");
	sounds[8] = new SoundObject();
	sounds[8]->LoadSound("Assets\\sound\\Ryu\\Shoryuken.mp3");

	checkGravity = true;
	stunned_timer = new Timer(0.f);

	for (int i = 0; i < MAX_INPUT; i++)
	{
		inputTimer[i] = new Timer(0.f);
	}
}

Player::~Player()
{
	for (int i = 0; i < (int)PlayerSound::NONE; i++)
	{
		SafeDelete(sounds[i]);
	}
}

void Player::Init()
{
	Super::Init();

}

void Player::Update(float deltaTime)
{
	if (hp < 0)
		dead = true;

	if (playerState == PLAYER_STATE::FIRST)
		color = Color::Green;
	else
		color = Color::Red;

	int init_offset_x = 10;
	int init_offset_y = -8;

	if (reverse)
		init_offset_x = -init_offset_x;

	is_guard = false;

	if (dead && !dead_anim)
	{
		PlayAnimation((int)(ANIM_LIST::KO), false);
		dead_anim = true;
		return;
	}

	if(dead && dead_anim)
	{
		Super::Update(deltaTime);
		return;
	}

	for (int i = 0; i < inputBuffer.size(); i++)
	{
		inputTimer[i]->Update(deltaTime);
	}

	for (int i = 0; i < inputBuffer.size(); i++)
	{
		if (inputTimer[i]->IsTimeOut())
		{
			inputBuffer.erase(inputBuffer.begin() + i);
		}
	}

	if (hitBox && moveState != MOVE_STATE::CROUCH)
	{
		hitBox->SetSize(15, 30);
		hitBox->SetOffset(0, 0);
	}

	
	if (checkGravity)
	{
		position.y += (int)ceilf(9.8f * deltaTime);
	}


	Super::Update(deltaTime);
	stunned_timer->Update(deltaTime);
	stateChanged = false;

	if (attackState != ATTACK_STATE::NONE)
		EmptyInputStack();

	attackState = ATTACK_STATE::NONE;

	if (!stunned_timer->IsTimeOut())
	{
		return;
	}
	// 공격 박스 크기 초기화
	attackBox->SetSize(7, 7);

	// 1P 입력 관련
	if (Engine::Get().GetKey('S') &&
		attackState == ATTACK_STATE::NONE &&
		playerState == PLAYER_STATE::FIRST &&
		moveState != MOVE_STATE::JUMP)
	{
		moveState = MOVE_STATE::CROUCH;
		stateChanged = true;
	}

	if (Engine::Get().GetKeyDown('S'))
	{
		if (inputBuffer.size() < MAX_INPUT)
		{
			inputBuffer.push_back(DIRECTION::DOWN);
			inputTimer[inputBuffer.size() - 1]->SetTime(0.5f);
			inputTimer[inputBuffer.size() - 1]->Reset();
		}
		else
		{
			inputBuffer.pop_front();
			inputBuffer.push_back(DIRECTION::DOWN);
			inputTimer[inputBuffer.size() - 1]->SetTime(0.5f);
			inputTimer[inputBuffer.size() - 1]->Reset();
		}
	}


	if (playerState == PLAYER_STATE::FIRST)
	{
		if (Engine::Get().GetKey('D') &&
			attackState == ATTACK_STATE::NONE &&
			playerState == PLAYER_STATE::FIRST && position.x < 450 && moveState != MOVE_STATE::CROUCH)
		{
			moveState = MOVE_STATE::WALK_FRONT;
			position.x += (int)ceilf(10.f * deltaTime);
			stateChanged = true;

			if (reverse)
			{
				is_guard = true;
			}
		}


		if (Engine::Get().GetKeyDown('D'))
		{
			if (inputBuffer.size() < MAX_INPUT)
			{
				inputBuffer.push_back(DIRECTION::RIGHT);
				inputTimer[inputBuffer.size() - 1]->SetTime(0.5f);
				inputTimer[inputBuffer.size() - 1]->Reset();
			}
			else
			{
				inputBuffer.pop_front();
				inputBuffer.push_back(DIRECTION::RIGHT);
				inputTimer[inputBuffer.size() - 1]->SetTime(0.5f);
				inputTimer[inputBuffer.size() - 1]->Reset();
			}

		}


		if (Engine::Get().GetKey('A') &&
			attackState == ATTACK_STATE::NONE &&
			playerState == PLAYER_STATE::FIRST && position.x > (width / 2) + 20 && moveState != MOVE_STATE::CROUCH)
		{
			moveState = MOVE_STATE::WALK_BACK;
			position.x -= (int)ceilf(10.f * deltaTime);
			stateChanged = true;
			if (!reverse)
			{
				is_guard = true;
			}
		}



		if (Engine::Get().GetKeyDown('A'))
		{
			if (inputBuffer.size() < MAX_INPUT)
			{
				inputBuffer.push_back(DIRECTION::LEFT);
				inputTimer[inputBuffer.size() - 1]->SetTime(0.5f);
				inputTimer[inputBuffer.size() - 1]->Reset();
			}
			else
			{
				inputBuffer.pop_front();
				inputBuffer.push_back(DIRECTION::LEFT);
				inputTimer[inputBuffer.size() - 1]->SetTime(0.5f);
				inputTimer[inputBuffer.size() - 1]->Reset();
			}
		}

		if (Engine::Get().GetKeyDown('W') &&
			attackState == ATTACK_STATE::NONE &&
			playerState == PLAYER_STATE::FIRST &&
			moveState != MOVE_STATE::CROUCH && !checkGravity)
		{
			moveState = MOVE_STATE::JUMP;
			if(deltaTime > 0.016f)
				position.y -= (int)ceilf(1800.f * deltaTime);
			else
				position.y -= (int)ceilf(2500.f * 0.016f);
			checkGravity = true;
			stateChanged = true;
			stunned_timer->SetTime(0.05f);
			stunned_timer->Reset();
			if (inputBuffer.size() < MAX_INPUT)
			{
				inputBuffer.push_back(DIRECTION::UP);
				inputTimer[inputBuffer.size() - 1]->SetTime(0.5f);
				inputTimer[inputBuffer.size() - 1]->Reset();
			}
			else
			{
				inputBuffer.pop_front();
				inputBuffer.push_back(DIRECTION::UP);
				inputTimer[inputBuffer.size() - 1]->SetTime(0.5f);
				inputTimer[inputBuffer.size() - 1]->Reset();
			}
		}

		if (Engine::Get().GetKeyDown('J') || Engine::Get().GetKeyDown('K') || Engine::Get().GetKeyDown('L') ||
			Engine::Get().GetKeyDown('U') || Engine::Get().GetKeyDown('I') || Engine::Get().GetKeyDown('O'))
		{
			CheckSpecialMove();
		}


		if (specialMove != SPECIAL::NONE)
		{
			switch (specialMove)
			{
			case SPECIAL::HADOUKEN:
				if (Engine::Get().GetKeyDown('J') || Engine::Get().GetKeyDown('K') || Engine::Get().GetKeyDown('L'))
				{
					PlayAnimation((int)ANIM_LIST::HADOUKEN, false);
					stateChanged = true;
					stunned_timer->SetTime(1.2f);
					sounds[(int)PlayerSound::HADOUKEN]->Play(1, 1.f, true);
					stunned_timer->Reset();

					bool left = reverse;
					Projectile* proj = new Projectile(left, position, playerState);
					Level* mainLevel = Engine::Get().GetCurrentLevel();
					if (mainLevel)
					{
						mainLevel->AddActor(proj);
					}

					return;
				}
				break;
			case SPECIAL::SENPUKEN:
				if (Engine::Get().GetKeyDown('U') || Engine::Get().GetKeyDown('I') || Engine::Get().GetKeyDown('O'))
				{
					PlayAnimation((int)ANIM_LIST::SENPUKEN, false);
					stateChanged = true;
					stunned_timer->SetTime(0.8f);
					attackBox->SetSize(30, 30);
					init_offset_x = 0;
					init_offset_y = 0;
					sounds[(int)PlayerSound::SENPUKEN]->Play(1, 1.f, true);
					attackState = ATTACK_STATE::HARD_KICK;
					stunned_timer->Reset();
					return;
				}
				break;
			case SPECIAL::SORYUKEN:
				if (Engine::Get().GetKeyDown('J') || Engine::Get().GetKeyDown('K') || Engine::Get().GetKeyDown('L'))
				{
					PlayAnimation((int)ANIM_LIST::SORYUKEN, false);
					stateChanged = true;
					stunned_timer->SetTime(0.8f);
					attackBox->SetSize(20, 25);
					init_offset_x = reverse ? 8 : -8;
					sounds[(int)PlayerSound::SHORYUKEN]->Play(1, 1.f, true);
					attackState = ATTACK_STATE::HARD_PUNCH;
					stunned_timer->Reset();
					return;
				}
				break;
			case SPECIAL::NONE:
				break;
			default:
				break;
			}

			specialMove = SPECIAL::NONE;
		}


		// 약손
		if (Engine::Get().GetKeyDown('J') &&
			playerState == PLAYER_STATE::FIRST)
		{
			switch (moveState)
			{
			case MOVE_STATE::IDLE:
				PlayAnimation((int)ANIM_LIST::LIGHTPUNCH, false);
				break;
			case MOVE_STATE::CROUCH:
				PlayAnimation((int)ANIM_LIST::DOWNLIGHTPUNCH, false);
				break;
			case MOVE_STATE::WALK_BACK:
				PlayAnimation((int)ANIM_LIST::LIGHTPUNCH, false);
				break;
			case MOVE_STATE::WALK_FRONT:
				PlayAnimation((int)ANIM_LIST::LIGHTPUNCH, false);
				break;
			case MOVE_STATE::JUMP:
				PlayAnimation((int)ANIM_LIST::JUMPPUNCH, false);
				break;
			case MOVE_STATE::JUMP_BACK:
				PlayAnimation((int)ANIM_LIST::JUMPPUNCH, false);
				break;
			case MOVE_STATE::JUMP_FORWARD:
				PlayAnimation((int)ANIM_LIST::JUMPPUNCH, false);
				break;
			default:
				break;
			}
			stateChanged = true;
			if (attackBox)
			{
				attackBox->SetOffset(20, -8);
			}
			stunned_timer->SetTime(0.2f);
			sounds[(int)PlayerSound::LIGHTATTACK]->Play(1, 1.f, true);
			attackState = ATTACK_STATE::LIGHT_PUNCH;
			stunned_timer->Reset();
		}

		// 중손
		if (Engine::Get().GetKeyDown('K') &&
			playerState == PLAYER_STATE::FIRST)
		{
			switch (moveState)
			{
			case MOVE_STATE::IDLE:
				PlayAnimation((int)ANIM_LIST::MIDDLEPUNCH, false);
				stunned_timer->SetTime(0.3f);
				break;
			case MOVE_STATE::CROUCH:
				PlayAnimation((int)ANIM_LIST::DOWNMIDDLEPUNCH, false);
				stunned_timer->SetTime(0.3f);
				break;
			case MOVE_STATE::WALK_BACK:
				PlayAnimation((int)ANIM_LIST::MIDDLEPUNCH, false);
				stunned_timer->SetTime(0.3f);
				break;
			case MOVE_STATE::WALK_FRONT:
				PlayAnimation((int)ANIM_LIST::MIDDLEPUNCH, false);
				stunned_timer->SetTime(0.2f);
				break;
			case MOVE_STATE::JUMP:
				PlayAnimation((int)ANIM_LIST::JUMPPUNCH, false);
				stunned_timer->SetTime(0.2f);
				break;
			case MOVE_STATE::JUMP_BACK:
				PlayAnimation((int)ANIM_LIST::JUMPPUNCH, false);
				stunned_timer->SetTime(0.2f);
				break;
			case MOVE_STATE::JUMP_FORWARD:
				PlayAnimation((int)ANIM_LIST::JUMPPUNCH, false);
				stunned_timer->SetTime(0.2f);
				break;
			default:
				break;
			}

			stateChanged = true;
			if (attackBox)
			{
				attackBox->SetOffset(20, -8);
			}
			attackState = ATTACK_STATE::MIDDLE_PUNCH;
			sounds[(int)PlayerSound::MEDIUMATTACK]->Play(1, 1.f, true);
			stunned_timer->Reset();
		}


		// 강손 앉아 강손 애니메이션 오류남
		if (Engine::Get().GetKeyDown('L') &&
			playerState == PLAYER_STATE::FIRST)
		{
			switch (moveState)
			{
			case MOVE_STATE::IDLE:
				PlayAnimation((int)ANIM_LIST::FORWARDHARDPUNCH, false);
				stunned_timer->SetTime(0.5f);
				break;
			case MOVE_STATE::CROUCH:
				PlayAnimation((int)ANIM_LIST::DOWNHARDPUNCH, false);
				stunned_timer->SetTime(0.35f);
				break;
			case MOVE_STATE::WALK_BACK:
				PlayAnimation((int)ANIM_LIST::FORWARDHARDPUNCH, false);
				stunned_timer->SetTime(0.3f);
				break;
			case MOVE_STATE::WALK_FRONT:
				PlayAnimation((int)ANIM_LIST::FORWARDHARDPUNCH, false);
				stunned_timer->SetTime(0.2f);
				break;
			case MOVE_STATE::JUMP:
				PlayAnimation((int)ANIM_LIST::JUMPPUNCH, false);
				stunned_timer->SetTime(0.2f);
				break;
			case MOVE_STATE::JUMP_BACK:
				PlayAnimation((int)ANIM_LIST::JUMPPUNCH, false);
				stunned_timer->SetTime(0.2f);
				break;
			case MOVE_STATE::JUMP_FORWARD:
				PlayAnimation((int)ANIM_LIST::JUMPPUNCH, false);
				stunned_timer->SetTime(0.2f);
				break;
			default:
				break;
			}
			stateChanged = true;
			if (attackBox)
			{
				attackBox->SetOffset(20, -8);
			}
			attackState = ATTACK_STATE::HARD_PUNCH;
			sounds[(int)PlayerSound::HARDATTACK]->Play(1, 1.f, true);
			stunned_timer->Reset();
		}


		// 약발
		if (Engine::Get().GetKeyDown('U') &&
			playerState == PLAYER_STATE::FIRST)
		{
			switch (moveState)
			{
			case MOVE_STATE::IDLE:
				PlayAnimation((int)ANIM_LIST::FORWARDLIGHTKICK, false);
				stunned_timer->SetTime(0.2f);
				break;
			case MOVE_STATE::CROUCH:
				PlayAnimation((int)ANIM_LIST::DOWNLIGHTKICK, false);
				stunned_timer->SetTime(0.2f);
				break;
			case MOVE_STATE::WALK_BACK:
				PlayAnimation((int)ANIM_LIST::FORWARDLIGHTKICK, false);
				stunned_timer->SetTime(0.2f);
				break;
			case MOVE_STATE::WALK_FRONT:
				PlayAnimation((int)ANIM_LIST::FORWARDLIGHTKICK, false);
				stunned_timer->SetTime(0.2f);
				break;
			case MOVE_STATE::JUMP:
				PlayAnimation((int)ANIM_LIST::JUMPLIGHTMIDDLEKICK, false);
				stunned_timer->SetTime(0.5f);
				break;
			case MOVE_STATE::JUMP_BACK:
				PlayAnimation((int)ANIM_LIST::JUMPLIGHTMIDDLEKICK, false);
				stunned_timer->SetTime(0.5f);
				break;
			case MOVE_STATE::JUMP_FORWARD:
				PlayAnimation((int)ANIM_LIST::JUMPLIGHTMIDDLEKICK, false);
				stunned_timer->SetTime(0.5f);
				break;
			default:
				break;
			}
			attackState = ATTACK_STATE::LIGHT_KICK;
			stateChanged = true;
			sounds[(int)PlayerSound::LIGHTATTACK]->Play(1, 1.f, true);
			stunned_timer->Reset();
		}

		// 중발
		if (Engine::Get().GetKeyDown('I') &&
			playerState == PLAYER_STATE::FIRST)
		{
			switch (moveState)
			{
			case MOVE_STATE::IDLE:
				PlayAnimation((int)ANIM_LIST::FORWARDMIDDLEKICK, false);
				stunned_timer->SetTime(0.2f);
				break;
			case MOVE_STATE::CROUCH:
				PlayAnimation((int)ANIM_LIST::DOWNMIDDLEKICK, false);
				stunned_timer->SetTime(0.3f);
				break;
			case MOVE_STATE::WALK_BACK:
				PlayAnimation((int)ANIM_LIST::FORWARDMIDDLEKICK, false);
				stunned_timer->SetTime(0.3f);
				break;
			case MOVE_STATE::WALK_FRONT:
				PlayAnimation((int)ANIM_LIST::FORWARDMIDDLEKICK, false);
				stunned_timer->SetTime(0.3f);
				break;
			case MOVE_STATE::JUMP:
				PlayAnimation((int)ANIM_LIST::JUMPLIGHTMIDDLEKICK, false);
				stunned_timer->SetTime(0.2f);
				break;
			case MOVE_STATE::JUMP_BACK:
				PlayAnimation((int)ANIM_LIST::JUMPLIGHTMIDDLEKICK, false);
				stunned_timer->SetTime(0.2f);
				break;
			case MOVE_STATE::JUMP_FORWARD:
				PlayAnimation((int)ANIM_LIST::JUMPLIGHTMIDDLEKICK, false);
				stunned_timer->SetTime(0.2f);
				break;
			default:
				break;
			}
			attackState = ATTACK_STATE::MIDDLE_KICK;
			stateChanged = true;
			sounds[(int)PlayerSound::MEDIUMATTACK]->Play(1, 1.f, true);
			stunned_timer->Reset();
		}

		// 강발
		if (Engine::Get().GetKeyDown('O') &&
			playerState == PLAYER_STATE::FIRST)
		{
			switch (moveState)
			{
			case MOVE_STATE::IDLE:
				PlayAnimation((int)ANIM_LIST::HARDKICK, false);
				stunned_timer->SetTime(1.f);
				break;
			case MOVE_STATE::CROUCH:
				PlayAnimation((int)ANIM_LIST::DOWNHARDKICK, false);
				stunned_timer->SetTime(1.f);
				break;
			case MOVE_STATE::WALK_BACK:
				PlayAnimation((int)ANIM_LIST::HARDKICK, false);
				stunned_timer->SetTime(1.f);
				break;
			case MOVE_STATE::WALK_FRONT:
				PlayAnimation((int)ANIM_LIST::HARDKICK, false);
				stunned_timer->SetTime(1.f);
				break;
			case MOVE_STATE::JUMP:
				PlayAnimation((int)ANIM_LIST::JUMPHARDKICK, false);
				stunned_timer->SetTime(1.f);
				break;
			case MOVE_STATE::JUMP_BACK:
				PlayAnimation((int)ANIM_LIST::JUMPHARDKICK, false);
				stunned_timer->SetTime(1.f);
				break;
			case MOVE_STATE::JUMP_FORWARD:
				PlayAnimation((int)ANIM_LIST::JUMPHARDKICK, false);
				stunned_timer->SetTime(1.f);
				break;
			default:
				break;
			}

			attackState = ATTACK_STATE::HARD_KICK;
			stateChanged = true;
			sounds[(int)PlayerSound::HARDATTACK]->Play(1, 1.f, true);
			stunned_timer->Reset();
		}
	}


	// 2P 관련 조작 설정
	else if (playerState == PLAYER_STATE::SECOND)
	{
		if (Engine::Get().GetKey(VK_DOWN) &&
			attackState == ATTACK_STATE::NONE &&
			playerState == PLAYER_STATE::SECOND &&
			moveState != MOVE_STATE::JUMP)
		{
			moveState = MOVE_STATE::CROUCH;
			stateChanged = true;
		}

		if (Engine::Get().GetKeyDown(VK_DOWN))
		{
			if (inputBuffer.size() < MAX_INPUT)
			{
				inputBuffer.push_back(DIRECTION::DOWN);
				inputTimer[inputBuffer.size() - 1]->SetTime(0.5f);
				inputTimer[inputBuffer.size() - 1]->Reset();
			}
			else
			{
				inputBuffer.pop_front();
				inputBuffer.push_back(DIRECTION::DOWN);
				inputTimer[inputBuffer.size() - 1]->SetTime(0.5f);
				inputTimer[inputBuffer.size() - 1]->Reset();
			}
		}

		if (Engine::Get().GetKey(VK_RIGHT) &&
			attackState == ATTACK_STATE::NONE &&
			playerState == PLAYER_STATE::SECOND && position.x < 460 && moveState != MOVE_STATE::CROUCH)
		{
			moveState = MOVE_STATE::WALK_FRONT;
			position.x += (int)ceilf(10.f * deltaTime);
			stateChanged = true;
			if (reverse)
				is_guard = true;
		}


		if (Engine::Get().GetKeyDown(VK_RIGHT))
		{
			if (inputBuffer.size() < MAX_INPUT)
			{
				inputBuffer.push_back(DIRECTION::RIGHT);
				inputTimer[inputBuffer.size() - 1]->SetTime(0.5f);
				inputTimer[inputBuffer.size() - 1]->Reset();
			}
			else
			{
				inputBuffer.pop_front();
				inputBuffer.push_back(DIRECTION::RIGHT);
				inputTimer[inputBuffer.size() - 1]->SetTime(0.5f);
				inputTimer[inputBuffer.size() - 1]->Reset();
			}
		}

		if (Engine::Get().GetKey(VK_LEFT) &&
			attackState == ATTACK_STATE::NONE &&
			playerState == PLAYER_STATE::SECOND && position.x > (width / 2) + 20 && moveState != MOVE_STATE::CROUCH)
		{
			moveState = MOVE_STATE::WALK_BACK;
			position.x -= (int)ceilf(10.f * deltaTime);
			stateChanged = true;
			if (!reverse)
				is_guard = true;
		}


		if (Engine::Get().GetKeyDown(VK_LEFT))
		{
			if (inputBuffer.size() < MAX_INPUT)
			{
				inputBuffer.push_back(DIRECTION::LEFT);
				inputTimer[inputBuffer.size() - 1]->SetTime(0.5f);
				inputTimer[inputBuffer.size() - 1]->Reset();
			}
			else
			{
				inputBuffer.pop_front();
				inputBuffer.push_back(DIRECTION::LEFT);
				inputTimer[inputBuffer.size() - 1]->SetTime(0.5f);
				inputTimer[inputBuffer.size() - 1]->Reset();
			}
		}

		if (Engine::Get().GetKeyDown(VK_UP) &&
			attackState == ATTACK_STATE::NONE &&
			playerState == PLAYER_STATE::SECOND &&
			moveState != MOVE_STATE::CROUCH && !checkGravity)
		{
			moveState = MOVE_STATE::JUMP;
			position.y -= (int)ceilf(1800.f * deltaTime);
			checkGravity = true;
			stateChanged = true;
			if (inputBuffer.size() < MAX_INPUT)
			{
				inputBuffer.push_back(DIRECTION::UP);
				inputTimer[inputBuffer.size() - 1]->SetTime(0.5f);
				inputTimer[inputBuffer.size() - 1]->Reset();
			}
			else
			{
				inputBuffer.pop_front();
				inputBuffer.push_back(DIRECTION::UP);
				inputTimer[inputBuffer.size() - 1]->SetTime(0.5f);
				inputTimer[inputBuffer.size() - 1]->Reset();
			}
		}

		if (Engine::Get().GetKeyDown(VK_NUMPAD1) || Engine::Get().GetKeyDown(VK_NUMPAD2) || Engine::Get().GetKeyDown(VK_NUMPAD3) ||
			Engine::Get().GetKeyDown(VK_NUMPAD4) || Engine::Get().GetKeyDown(VK_NUMPAD5) || Engine::Get().GetKeyDown(VK_NUMPAD6))
		{
			CheckSpecialMove();
		}


		if (specialMove != SPECIAL::NONE)
		{
			switch (specialMove)
			{
			case SPECIAL::HADOUKEN:
				if (Engine::Get().GetKeyDown(VK_NUMPAD1) || Engine::Get().GetKeyDown(VK_NUMPAD2) || Engine::Get().GetKeyDown(VK_NUMPAD3))
				{
					PlayAnimation((int)ANIM_LIST::HADOUKEN, false);
					stateChanged = true;
					stunned_timer->SetTime(1.2f);
					sounds[(int)PlayerSound::HADOUKEN]->Play(1, 2.f, true);
					stunned_timer->Reset();

					Projectile* proj = new Projectile(reverse, position, playerState);
					Level* mainLevel = Engine::Get().GetCurrentLevel();
					if (mainLevel)
					{
						mainLevel->AddActor(proj);
					}

					return;
				}
				break;
			case SPECIAL::SENPUKEN:
				if (Engine::Get().GetKeyDown(VK_NUMPAD4) || Engine::Get().GetKeyDown(VK_NUMPAD5) || Engine::Get().GetKeyDown(VK_NUMPAD6))
				{
					PlayAnimation((int)ANIM_LIST::SENPUKEN, false);
					stateChanged = true;
					stunned_timer->SetTime(1.f);
					attackBox->SetSize(30, 30);
					init_offset_x = 0;
					init_offset_y = 0;
					sounds[(int)PlayerSound::SENPUKEN]->Play(1, 1.f, true);
					attackState = ATTACK_STATE::HARD_KICK;
					stunned_timer->Reset();
					return;
				}
				break;
			case SPECIAL::SORYUKEN:
				if (Engine::Get().GetKeyDown(VK_NUMPAD1) || Engine::Get().GetKeyDown(VK_NUMPAD2) || Engine::Get().GetKeyDown(VK_NUMPAD3))
				{
					PlayAnimation((int)ANIM_LIST::SORYUKEN, false);
					stateChanged = true;
					stunned_timer->SetTime(1.f);
					attackBox->SetSize(20, 25);
					init_offset_x = reverse ? 8 : -8;
					sounds[(int)PlayerSound::SHORYUKEN]->Play(1, 1.f, true);
					attackState = ATTACK_STATE::HARD_PUNCH;
					stunned_timer->Reset();
					return;
				}
				break;
			case SPECIAL::NONE:
				break;
			default:
				break;
			}

			specialMove = SPECIAL::NONE;
		}


		// 약손
		if (Engine::Get().GetKeyDown(VK_NUMPAD1) &&
			playerState == PLAYER_STATE::SECOND)
		{
			switch (moveState)
			{
			case MOVE_STATE::IDLE:
				PlayAnimation((int)ANIM_LIST::LIGHTPUNCH, false);
				break;
			case MOVE_STATE::CROUCH:
				PlayAnimation((int)ANIM_LIST::DOWNLIGHTPUNCH, false);
				break;
			case MOVE_STATE::WALK_BACK:
				PlayAnimation((int)ANIM_LIST::LIGHTPUNCH, false);
				break;
			case MOVE_STATE::WALK_FRONT:
				PlayAnimation((int)ANIM_LIST::LIGHTPUNCH, false);
				break;
			case MOVE_STATE::JUMP:
				PlayAnimation((int)ANIM_LIST::JUMPPUNCH, false);
				break;
			case MOVE_STATE::JUMP_BACK:
				PlayAnimation((int)ANIM_LIST::JUMPPUNCH, false);
				break;
			case MOVE_STATE::JUMP_FORWARD:
				PlayAnimation((int)ANIM_LIST::JUMPPUNCH, false);
				break;
			default:
				break;
			}
			stateChanged = true;
			stunned_timer->SetTime(0.3f);
			sounds[(int)PlayerSound::LIGHTATTACK]->Play(1, 1.f, true);
			attackState = ATTACK_STATE::LIGHT_PUNCH;
			stunned_timer->Reset();
		}

		// 중손
		if (Engine::Get().GetKeyDown(VK_NUMPAD2) &&
			playerState == PLAYER_STATE::SECOND)
		{
			switch (moveState)
			{
			case MOVE_STATE::IDLE:
				PlayAnimation((int)ANIM_LIST::MIDDLEPUNCH, false);
				stunned_timer->SetTime(0.5f);
				break;
			case MOVE_STATE::CROUCH:
				PlayAnimation((int)ANIM_LIST::DOWNMIDDLEPUNCH, false);
				stunned_timer->SetTime(0.5f);
				break;
			case MOVE_STATE::WALK_BACK:
				PlayAnimation((int)ANIM_LIST::MIDDLEPUNCH, false);
				stunned_timer->SetTime(0.5f);
				break;
			case MOVE_STATE::WALK_FRONT:
				PlayAnimation((int)ANIM_LIST::MIDDLEPUNCH, false);
				stunned_timer->SetTime(0.5f);
				break;
			case MOVE_STATE::JUMP:
				PlayAnimation((int)ANIM_LIST::JUMPPUNCH, false);
				stunned_timer->SetTime(0.3f);
				break;
			case MOVE_STATE::JUMP_BACK:
				PlayAnimation((int)ANIM_LIST::JUMPPUNCH, false);
				stunned_timer->SetTime(0.3f);
				break;
			case MOVE_STATE::JUMP_FORWARD:
				PlayAnimation((int)ANIM_LIST::JUMPPUNCH, false);
				stunned_timer->SetTime(0.3f);
				break;
			default:
				break;
			}

			stateChanged = true;
			attackState = ATTACK_STATE::MIDDLE_PUNCH;
			sounds[(int)PlayerSound::MEDIUMATTACK]->Play(1, 1.f, true);
			stunned_timer->Reset();
		}


		// 강손
		if (Engine::Get().GetKeyDown(VK_NUMPAD3) &&
			playerState == PLAYER_STATE::SECOND)
		{
			switch (moveState)
			{
			case MOVE_STATE::IDLE:
				PlayAnimation((int)ANIM_LIST::FORWARDHARDPUNCH, false);
				stunned_timer->SetTime(0.8f);
				break;
			case MOVE_STATE::CROUCH:
				PlayAnimation((int)ANIM_LIST::DOWNHARDPUNCH, false);
				stunned_timer->SetTime(0.8f);
				break;
			case MOVE_STATE::WALK_BACK:
				PlayAnimation((int)ANIM_LIST::FORWARDHARDPUNCH, false);
				stunned_timer->SetTime(0.8f);
				break;
			case MOVE_STATE::WALK_FRONT:
				PlayAnimation((int)ANIM_LIST::FORWARDHARDPUNCH, false);
				stunned_timer->SetTime(0.8f);
				break;
			case MOVE_STATE::JUMP:
				PlayAnimation((int)ANIM_LIST::JUMPPUNCH, false);
				stunned_timer->SetTime(0.3f);
				break;
			case MOVE_STATE::JUMP_BACK:
				PlayAnimation((int)ANIM_LIST::JUMPPUNCH, false);
				stunned_timer->SetTime(0.3f);
				break;
			case MOVE_STATE::JUMP_FORWARD:
				PlayAnimation((int)ANIM_LIST::JUMPPUNCH, false);
				stunned_timer->SetTime(0.3f);
				break;
			default:
				break;
			}
			stateChanged = true;
			if (attackBox)
			{
				attackBox->SetOffset(20, -8);
			}
			attackState = ATTACK_STATE::HARD_PUNCH;
			sounds[(int)PlayerSound::HARDATTACK]->Play(1, 1.f, true);
			stunned_timer->Reset();
		}


		// 약발
		if (Engine::Get().GetKeyDown(VK_NUMPAD4) &&
			playerState == PLAYER_STATE::SECOND)
		{
			switch (moveState)
			{
			case MOVE_STATE::IDLE:
				PlayAnimation((int)ANIM_LIST::FORWARDLIGHTKICK, false);
				stunned_timer->SetTime(0.5f);
				break;
			case MOVE_STATE::CROUCH:
				PlayAnimation((int)ANIM_LIST::DOWNLIGHTKICK, false);
				stunned_timer->SetTime(0.5f);
				break;
			case MOVE_STATE::WALK_BACK:
				PlayAnimation((int)ANIM_LIST::FORWARDLIGHTKICK, false);
				stunned_timer->SetTime(0.5f);
				break;
			case MOVE_STATE::WALK_FRONT:
				PlayAnimation((int)ANIM_LIST::FORWARDLIGHTKICK, false);
				stunned_timer->SetTime(0.5f);
				break;
			case MOVE_STATE::JUMP:
				PlayAnimation((int)ANIM_LIST::JUMPLIGHTMIDDLEKICK, false);
				stunned_timer->SetTime(0.5f);
				break;
			case MOVE_STATE::JUMP_BACK:
				PlayAnimation((int)ANIM_LIST::JUMPLIGHTMIDDLEKICK, false);
				stunned_timer->SetTime(0.5f);
				break;
			case MOVE_STATE::JUMP_FORWARD:
				PlayAnimation((int)ANIM_LIST::JUMPLIGHTMIDDLEKICK, false);
				stunned_timer->SetTime(0.5f);
				break;
			default:
				break;
			}
			attackState = ATTACK_STATE::LIGHT_KICK;
			stateChanged = true;
			sounds[(int)PlayerSound::LIGHTATTACK]->Play(1, 1.f, true);
			stunned_timer->Reset();
		}

		// 중발
		if (Engine::Get().GetKeyDown(VK_NUMPAD5) &&
			playerState == PLAYER_STATE::SECOND)
		{
			switch (moveState)
			{
			case MOVE_STATE::IDLE:
				PlayAnimation((int)ANIM_LIST::FORWARDMIDDLEKICK, false);
				stunned_timer->SetTime(0.7f);
				break;
			case MOVE_STATE::CROUCH:
				PlayAnimation((int)ANIM_LIST::DOWNMIDDLEKICK, false);
				stunned_timer->SetTime(0.7f);
				break;
			case MOVE_STATE::WALK_BACK:
				PlayAnimation((int)ANIM_LIST::FORWARDMIDDLEKICK, false);
				stunned_timer->SetTime(0.7f);
				break;
			case MOVE_STATE::WALK_FRONT:
				PlayAnimation((int)ANIM_LIST::FORWARDMIDDLEKICK, false);
				stunned_timer->SetTime(0.7f);
				break;
			case MOVE_STATE::JUMP:
				PlayAnimation((int)ANIM_LIST::JUMPLIGHTMIDDLEKICK, false);
				stunned_timer->SetTime(0.5f);
				break;
			case MOVE_STATE::JUMP_BACK:
				PlayAnimation((int)ANIM_LIST::JUMPLIGHTMIDDLEKICK, false);
				stunned_timer->SetTime(0.5f);
				break;
			case MOVE_STATE::JUMP_FORWARD:
				PlayAnimation((int)ANIM_LIST::JUMPLIGHTMIDDLEKICK, false);
				stunned_timer->SetTime(0.5f);
				break;
			default:
				break;
			}
			attackState = ATTACK_STATE::MIDDLE_KICK;
			stateChanged = true;
			sounds[(int)PlayerSound::MEDIUMATTACK]->Play(1, 1.f, true);
			stunned_timer->Reset();
		}

		// 강발
		if (Engine::Get().GetKeyDown(VK_NUMPAD6) &&
			playerState == PLAYER_STATE::SECOND)
		{
			switch (moveState)
			{
			case MOVE_STATE::IDLE:
				PlayAnimation((int)ANIM_LIST::HARDKICK, false);
				stunned_timer->SetTime(0.9f);
				break;
			case MOVE_STATE::CROUCH:
				PlayAnimation((int)ANIM_LIST::DOWNHARDKICK, false);
				stunned_timer->SetTime(0.9f);
				break;
			case MOVE_STATE::WALK_BACK:
				PlayAnimation((int)ANIM_LIST::HARDKICK, false);
				stunned_timer->SetTime(0.9f);
				break;
			case MOVE_STATE::WALK_FRONT:
				PlayAnimation((int)ANIM_LIST::HARDKICK, false);
				stunned_timer->SetTime(0.9f);
				break;
			case MOVE_STATE::JUMP:
				PlayAnimation((int)ANIM_LIST::JUMPHARDKICK, false);
				stunned_timer->SetTime(0.9f);
				break;
			case MOVE_STATE::JUMP_BACK:
				PlayAnimation((int)ANIM_LIST::JUMPHARDKICK, false);
				stunned_timer->SetTime(0.9f);
				break;
			case MOVE_STATE::JUMP_FORWARD:
				PlayAnimation((int)ANIM_LIST::JUMPHARDKICK, false);
				stunned_timer->SetTime(0.9f);
				break;
			default:
				break;
			}

			attackState = ATTACK_STATE::HARD_KICK;
			stateChanged = true;
			sounds[(int)PlayerSound::HARDATTACK]->Play(1, 1.f, true);
			stunned_timer->Reset();
		}
	}


	// 이동 관련 애니메이션 처리
	switch (moveState)
	{
	case MOVE_STATE::IDLE:
		if (GetCurrentPlaying() != (int)ANIM_LIST::IDLE && !stateChanged)
			PlayAnimation((int)ANIM_LIST::IDLE, true);
		break;
	case MOVE_STATE::CROUCH:
		if (GetCurrentPlaying() != (int)ANIM_LIST::CRAWL && attackState == ATTACK_STATE::NONE)
			PlayAnimation((int)ANIM_LIST::CRAWL, false);
		if (hitBox)
		{
			hitBox->SetSize(30, 10);
			hitBox->SetOffset(0, 12);
		}
		break;
	case MOVE_STATE::WALK_BACK:
		if (GetCurrentPlaying() != (int)ANIM_LIST::WALKING && attackState == ATTACK_STATE::NONE && !checkGravity)
			PlayAnimation((int)ANIM_LIST::WALKING, true);
		break;
	case MOVE_STATE::WALK_FRONT:
		if (GetCurrentPlaying() != (int)ANIM_LIST::WALKING && attackState == ATTACK_STATE::NONE && !checkGravity)
			PlayAnimation((int)ANIM_LIST::WALKING, true);
		break;
	case MOVE_STATE::JUMP:
		if (GetCurrentPlaying() != (int)ANIM_LIST::FORWARDJUMP && attackState == ATTACK_STATE::NONE && !stateChanged)
			PlayAnimation((int)ANIM_LIST::FORWARDJUMP, false);
		break;
	case MOVE_STATE::JUMP_BACK:
		if (GetCurrentPlaying() != (int)ANIM_LIST::FORWARDJUMP)
			PlayAnimation((int)ANIM_LIST::FORWARDJUMP, false);
		break;
	case MOVE_STATE::JUMP_FORWARD:
		if (GetCurrentPlaying() != (int)ANIM_LIST::FORWARDJUMP)
			PlayAnimation((int)ANIM_LIST::FORWARDJUMP, false);
		break;
	default:
		break;
	}

	// 공격 관련 히트박스 처리
	switch (attackState)
	{
	case ATTACK_STATE::LIGHT_PUNCH:
		if (reverse)
		{
			init_offset_x -= 8;
		}
		else
		{
			init_offset_x += 8;
		}
		break;
	case ATTACK_STATE::MIDDLE_PUNCH:
		if (reverse)
		{
			init_offset_x -= 9;
		}
		else
		{
			init_offset_x += 9;
		}
		break;
	case ATTACK_STATE::HARD_PUNCH:
		if (specialMove == SPECIAL::NONE)
		{
			if (reverse)
			{
				init_offset_x -= 10;
			}
			else
			{
				init_offset_x += 10;
			}
		}
		break;
	case ATTACK_STATE::LIGHT_KICK:
		if (reverse)
		{
			init_offset_x -= 12;
			init_offset_y += 7;
		}
		else
		{
			init_offset_x += 12;
			init_offset_y += 7;
		}
		break;
	case ATTACK_STATE::MIDDLE_KICK:
		if (reverse)
		{
			init_offset_x -= 14;
			init_offset_y += 7;
		}
		else
		{
			init_offset_x += 14;
			init_offset_y += 7;
		}
		break;
	case ATTACK_STATE::HARD_KICK:
		if (specialMove == SPECIAL::NONE)
		{
			if (reverse)
			{
				init_offset_x -= 16;
				init_offset_y += 7;
			}
			else
			{
				init_offset_x += 16;
				init_offset_y += 7;
			}
		}

		break;
	case ATTACK_STATE::NONE:
		break;
	default:
		break;
	}

	if (moveState == MOVE_STATE::CROUCH)
	{
		init_offset_y += 15;
	}


	if (!stateChanged && moveState != MOVE_STATE::JUMP && !checkGravity)
	{
		moveState = MOVE_STATE::IDLE;
	}

	if (checkGravity)
	{
		moveState = MOVE_STATE::JUMP;
	}


	if (attackBox)
	{
		attackBox->SetOffset(init_offset_x, init_offset_y);
	}
}

void Player::Draw()
{
	Super::Draw();
}

void Player::OnCollisionEnter(BoundingBox* _target)
{
	if (_target->As<BoundingBox>())
	{
		BoundingBox* target = (BoundingBox*)_target;
		if (target->GetLayerType() == LAYER::GROUND)
		{
			checkGravity = false;
			moveState = MOVE_STATE::IDLE;
		}
	}

	if (_target->As<BoundingBox>())
	{
		BoundingBox* target = (BoundingBox*)_target;
		if (target->GetOwner() && target->GetOwner()->As<Player>())
		{
			if ((attackBox->GetLayerType() == LAYER::RYU && (target->GetLayerType() == LAYER::KENBODY || target->GetLayerType() == LAYER::KEN)) ||
				(attackBox->GetLayerType() == LAYER::KEN && (target->GetLayerType() == LAYER::RYUBODY || target->GetLayerType() == LAYER::RYU)))
			{
				Player* enemy = (Player*)target->GetOwner();
				if (attackState == ATTACK_STATE::NONE && enemy->attackState == ATTACK_STATE::NONE)
					return;
				if (dead)
					return;
				if (enemy->dead)
					return;
				if (attackState == ATTACK_STATE::NONE)
				{
					switch (enemy->attackState)
					{
					case ATTACK_STATE::LIGHT_PUNCH:
						if (!is_guard)
						{
							hp -= 10;
							PlayAnimation((int)ANIM_LIST::HIT, false);
							sounds[(int)PlayerSound::JABHIT]->Play(1, 1.f, true);
							stunned_timer->SetTime(0.2f);
						}
						else
						{
							PlayAnimation((int)ANIM_LIST::GUARD, false);
							sounds[(int)PlayerSound::BLOCK]->Play(1, 1.f, true);
							stunned_timer->SetTime(0.1f);
						}
						break;
					case ATTACK_STATE::MIDDLE_PUNCH:
						if (!is_guard)
						{
							hp -= 20;
							PlayAnimation((int)ANIM_LIST::HIT, false);
							sounds[(int)PlayerSound::STRONGHIT]->Play(1, 1.f, true);
							stunned_timer->SetTime(0.3f);
						}
						else
						{
							PlayAnimation((int)ANIM_LIST::GUARD, false);
							sounds[(int)PlayerSound::BLOCK]->Play(1, 1.f, true);
							stunned_timer->SetTime(0.1f);
						}
						break;
					case ATTACK_STATE::HARD_PUNCH:
						if (!is_guard)
						{
							hp -= 50;
							PlayAnimation((int)ANIM_LIST::HIT, false);
							sounds[(int)PlayerSound::STRONGHIT]->Play(1, 1.f, true);
							stunned_timer->SetTime(0.3f);
						}
						else
						{
							PlayAnimation((int)ANIM_LIST::GUARD, false);
							sounds[(int)PlayerSound::BLOCK]->Play(1, 1.f, true);
							stunned_timer->SetTime(0.1f);
						}
						break;
					case ATTACK_STATE::LIGHT_KICK:
						if (!is_guard)
						{
							hp -= 10;
							PlayAnimation((int)ANIM_LIST::HIT, false);
							sounds[(int)PlayerSound::STRONGHIT]->Play(1, 1.f, true);
							stunned_timer->SetTime(0.3f);
						}
						else
						{
							PlayAnimation((int)ANIM_LIST::GUARD, false);
							sounds[(int)PlayerSound::BLOCK]->Play(1, 1.f, true);
							stunned_timer->SetTime(0.2f);
						}
						break;
					case ATTACK_STATE::MIDDLE_KICK:
						if (!is_guard)
						{
							hp -= 20;
							PlayAnimation((int)ANIM_LIST::HIT, false);
							sounds[(int)PlayerSound::STRONGHIT]->Play(1, 1.f, true);
							stunned_timer->SetTime(0.4f);
						}
						else
						{
							PlayAnimation((int)ANIM_LIST::GUARD, false);
							sounds[(int)PlayerSound::BLOCK]->Play(1, 1.f, true);
							stunned_timer->SetTime(0.2f);
						}
						break;
					case ATTACK_STATE::HARD_KICK:
						if (!is_guard)
						{
							hp -= 50;
							PlayAnimation((int)ANIM_LIST::HIT, false);
							sounds[(int)PlayerSound::STRONGHIT]->Play(1, 1.f, true);
							stunned_timer->SetTime(0.5f);
						}
						else
						{
							PlayAnimation((int)ANIM_LIST::GUARD, false);
							sounds[(int)PlayerSound::BLOCK]->Play(1, 1.f, true);
							stunned_timer->SetTime(0.2f);
						}
						break;
					default:
						break;
					}
					stunned_timer->Reset();
					enemy->attackState = ATTACK_STATE::NONE;
				}
				else
				{
					switch (attackState)
					{
					case ATTACK_STATE::LIGHT_PUNCH:
						if (!is_guard)
						{
							enemy->hp -= 10;
							enemy->PlayAnimation((int)ANIM_LIST::HIT, false);
							sounds[(int)PlayerSound::JABHIT]->Play(1, 1.f, true);
							enemy->stunned_timer->SetTime(0.2f);
						}
						else
						{
							enemy->PlayAnimation((int)ANIM_LIST::GUARD, false);
							sounds[(int)PlayerSound::BLOCK]->Play(1, 1.f, true);
							enemy->stunned_timer->SetTime(0.1f);
						}
						break;
					case ATTACK_STATE::MIDDLE_PUNCH:
						if (!is_guard)
						{
							enemy->hp -= 20;
							enemy->PlayAnimation((int)ANIM_LIST::HIT, false);
							sounds[(int)PlayerSound::STRONGHIT]->Play(1, 1.f, true);
							enemy->stunned_timer->SetTime(0.3f);
						}
						else
						{
							enemy->PlayAnimation((int)ANIM_LIST::GUARD, false);
							sounds[(int)PlayerSound::BLOCK]->Play(1, 1.f, true);
							enemy->stunned_timer->SetTime(0.1f);
						}
						break;
					case ATTACK_STATE::HARD_PUNCH:
						if (!is_guard)
						{
							enemy->hp -= 50;
							enemy->PlayAnimation((int)ANIM_LIST::HIT, false);
							sounds[(int)PlayerSound::STRONGHIT]->Play(1, 1.f, true);
							enemy->stunned_timer->SetTime(0.3f);
						}
						else
						{
							enemy->PlayAnimation((int)ANIM_LIST::GUARD, false);
							sounds[(int)PlayerSound::BLOCK]->Play(1, 1.f, true);
							enemy->stunned_timer->SetTime(0.1f);
						}
						break;
					case ATTACK_STATE::LIGHT_KICK:
						if (!is_guard)
						{
							enemy->hp -= 10;
							enemy->PlayAnimation((int)ANIM_LIST::HIT, false);
							sounds[(int)PlayerSound::STRONGHIT]->Play(1, 1.f, true);
							enemy->stunned_timer->SetTime(0.3f);
						}
						else
						{
							enemy->PlayAnimation((int)ANIM_LIST::GUARD, false);
							sounds[(int)PlayerSound::BLOCK]->Play(1, 1.f, true);
							enemy->stunned_timer->SetTime(0.2f);
						}
						break;
					case ATTACK_STATE::MIDDLE_KICK:
						if (!is_guard)
						{
							enemy->hp -= 20;
							enemy->PlayAnimation((int)ANIM_LIST::HIT, false);
							sounds[(int)PlayerSound::STRONGHIT]->Play(1, 1.f, true);
							enemy->stunned_timer->SetTime(0.4f);
						}
						else
						{
							enemy->PlayAnimation((int)ANIM_LIST::GUARD, false);
							sounds[(int)PlayerSound::BLOCK]->Play(1, 1.f, true);
							enemy->stunned_timer->SetTime(0.2f);
						}
						break;
					case ATTACK_STATE::HARD_KICK:
						if (!is_guard)
						{
							enemy->hp -= 50;
							enemy->PlayAnimation((int)ANIM_LIST::HIT, false);
							sounds[(int)PlayerSound::STRONGHIT]->Play(1, 1.f, true);
							enemy->stunned_timer->SetTime(0.5f);
						}
						else
						{
							enemy->PlayAnimation((int)ANIM_LIST::GUARD, false);
							sounds[(int)PlayerSound::BLOCK]->Play(1, 1.f, true);
							enemy->stunned_timer->SetTime(0.2f);
						}
						break;
					default:
						break;
					}
					enemy->stunned_timer->Reset();
					attackState = ATTACK_STATE::NONE;
				}
			}
		}
	}
}

void Player::OnCollision(BoundingBox* _target)
{
	if (_target->As<BoundingBox>())
	{
		BoundingBox* target = (BoundingBox*)_target;
		if (target->GetLayerType() == LAYER::GROUND)
		{
			if (target->Position().y - target->Height() / 2 < position.y)
			{
				position.y = target->Position().y - target->Height() + 1;
			}
			checkGravity = false;
			// moveState = MOVE_STATE::IDLE;
		}
	}
}

void Player::CollisionEnd(BoundingBox* _target)
{

}

void Player::EmptyInputStack()
{
	inputBuffer.clear();
}

void Player::CheckSpecialMove()
{
	DIRECTION first = DIRECTION::NONE;
	DIRECTION second = DIRECTION::NONE;

	if (inputBuffer.size() <= 1)
	{
		return;
	}

	second = inputBuffer.back();
	if (inputTimer[inputBuffer.size() - 1]->IsTimeOut())
		return;
	inputBuffer.pop_back();
	first = inputBuffer.back();
	if (inputTimer[inputBuffer.size() - 1]->IsTimeOut())
		return;
	inputBuffer.pop_back();

	// 왼쪽 특수 공격 판정
	if (!reverse)
	{
		if (first == DIRECTION::DOWN && second == DIRECTION::RIGHT)
		{
			specialMove = SPECIAL::HADOUKEN;
		}

		if (first == DIRECTION::DOWN && second == DIRECTION::LEFT)
		{
			specialMove = SPECIAL::SENPUKEN;
		}

		if (first == DIRECTION::DOWN && second == DIRECTION::UP)
		{
			specialMove = SPECIAL::SORYUKEN;
		}
	}
	
	else
	{
		if (first == DIRECTION::DOWN && second == DIRECTION::LEFT)
		{
			specialMove = SPECIAL::HADOUKEN;
		}

		if (first == DIRECTION::DOWN && second == DIRECTION::RIGHT)
		{
			specialMove = SPECIAL::SENPUKEN;
		}

		if (first == DIRECTION::DOWN && second == DIRECTION::UP)
		{
			specialMove = SPECIAL::SORYUKEN;
		}
	}


}

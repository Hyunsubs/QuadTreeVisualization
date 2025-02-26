#pragma once
#include <Actor/AnimatedActor.h>
#include <Engine/Timer.h>
#include <deque>
#include <vector>
#include <Sound/SoundObject.h>
class SoundObject;
class Timer;

#define MAX_INPUT 10

enum class ANIM_LIST
{
	CRAWL,
	DOWNHARDKICK,
	DOWNHARDPUNCH,
	DOWNHIT,
	DOWNLIGHTKICK,
	DOWNLIGHTPUNCH,
	DOWNMIDDLEKICK,
	DOWNMIDDLEPUNCH,
	FORWARDHARDKICK,
	FORWARDHARDPUNCH,
	FORWARDJUMP,
	FORWARDJUMPMIDDLEHARDKICK,
	FORWARDLIGHTKICK,
	FORWARDLIGHTMIDDLEKICK,
	FORWARDLIGHTMIDDLEPUNCH,
	FORWARDLIGHTPUNCH,
	FORWARDMIDDLEKICK,
	FORWARDMIDDLEPUNCH,
	HADOUKEN,
	HIT,
	HITFACE,
	IDLE,
	JUMPHARDKICK,
	JUMPLIGHTMIDDLEKICK,
	JUMPPUNCH,
	LIGHTPUNCH,
	MIDDLEPUNCH,
	NORMALJUMP,
	PROJECTILE,
	CATCHBACK,
	FORWARDCATCH,
	GUARD,
	HARDKICK,
	KO,
	STUMBLE,
	WALKING,
	SENPUKEN,
	SORYUKEN,
};


enum class MOVE_STATE
{
	IDLE,
	CROUCH,
	WALK_BACK,
	WALK_FRONT,
	JUMP,
	JUMP_BACK,
	JUMP_FORWARD,
};

enum class ATTACK_STATE
{
	LIGHT_PUNCH,
	MIDDLE_PUNCH,
	HARD_PUNCH,
	LIGHT_KICK,
	MIDDLE_KICK,
	HARD_KICK,
	NONE,
};

enum class PLAYER_STATE
{
	NONE,
	FIRST,
	SECOND,
};

enum class DIRECTION
{
	UP,
	DOWN,
	LEFT,
	RIGHT,
	RIGHTUP,
	LEFTUP,
	RIGHTDOWN,
	LEFTDOWN,
	NONE,
};

enum class SPECIAL
{
	HADOUKEN,
	SENPUKEN,
	SORYUKEN,
	NONE,
};

enum class PlayerSound
{
	BLOCK,
	HARDATTACK,
	JABHIT,
	LIGHTATTACK,
	MEDIUMATTACK,
	STRONGHIT,
	HADOUKEN,
	SENPUKEN,
	SHORYUKEN,
	NONE,
};

class BoundingBox;
class Player : public AnimatedActor
{
	RTTI_DECLARATIONS(Player, AnimatedActor)
public:
	Player();
	~Player();

	virtual void Init() override;
	virtual void Update(float deltaTime);
	virtual void Draw() override;

	virtual void OnCollisionEnter(BoundingBox* _target) override;
	virtual void OnCollision(BoundingBox* _target) override;
	virtual void CollisionEnd(BoundingBox* _target) override;

	inline void SetPlayerState(PLAYER_STATE state) { playerState = state; }
	void EmptyInputStack();
	inline void SetAttackBox(BoundingBox* box) { attackBox = box; }
	inline void SetHitBox(BoundingBox* box) { hitBox = box; }

	void CheckSpecialMove();

	inline void PlayPlayerSound(PlayerSound idx) 
	{ 
		if(sounds[(int)idx])
			sounds[(int)idx]->Play(1, 1.f, true);
	}

	inline int GetHP() { return hp; }
	inline void SetHP(int _hp) { hp = _hp; }
	inline bool GetReverse() { return reverse; }

	inline void SetStunnedTime(float _time) { stunned_timer->SetTime(_time); }
	inline void ResetTimer() { stunned_timer->Reset(); }
	inline bool IsDead() { return dead; }

	inline bool IsGuard() { return is_guard; }

private:
	MOVE_STATE moveState;
	ATTACK_STATE attackState;
	PLAYER_STATE playerState;
	SPECIAL specialMove;
	Timer* stunned_timer;

	BoundingBox* attackBox = nullptr;
	BoundingBox* hitBox = nullptr;

	int hp = 250;
	bool dead = false;
	bool dead_anim = false;

	bool is_guard = false;

	bool first_update = false;
	bool stateChanged = false;
	std::deque<DIRECTION> inputBuffer;
	Timer* inputTimer[MAX_INPUT];

	SoundObject* sounds[(int)PlayerSound::NONE] = {};
};
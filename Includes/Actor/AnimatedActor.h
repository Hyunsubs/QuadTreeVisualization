#pragma once
#include <Actor/DrawableActor.h>
#include <vector>
#include <string>

class Timer;
class ENGINE_API AnimatedActor : public DrawableActor
{
	RTTI_DECLARATIONS(AnimatedActor, DrawableActor)
public:
	AnimatedActor();
	~AnimatedActor();
	
	int AddAnimation(std::string _filepath, float anim_time);

	virtual void Update(float deltaTime) override;
	virtual void Draw() override;

	inline void SetAnimTime(float _time, int idx)
	{ 
		if(imageList.size() > idx)
			anim_time[idx] = _time;
	}
	
	void PlayAnimation(int idx, bool repeat);
	inline bool IsPlaying() { return is_playing; }
	inline int GetCurrentPlaying() { return current_anim_idx; }

	// Collision
	virtual void OnCollisionEnter(BoundingBox* _target) override;
	virtual void OnCollision(BoundingBox* _target) override;
	virtual void CollisionEnd(BoundingBox* _target) override;




protected:
	std::vector<std::vector<std::string>> imageList;
	std::vector<float> anim_time = {};
	int current_anim_idx = 0;
	int current_frame = 0;
	bool repeat = false;
	bool is_playing = false;
	Timer* anim_timer;

};
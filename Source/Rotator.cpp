#include "Common.h"
#include "Rotator.h"


Rotator::Rotator(Context* context) : LogicComponent(context)
{
	// Only the physics update event is needed: unsubscribe from the rest for optimization
	SetUpdateEventMask(USE_FIXEDUPDATE);

}

void Rotator::RegisterObject(Context* context)
{
	context->RegisterFactory<Rotator>();
}

void Rotator::Start()
{
	startPosition = GetNode()->GetWorldPosition();
	radius_ = 5.0f;
	speed_ = 10.0f;
}

void Rotator::Update(float timeStep) 
{

}


void Rotator::FixedUpdate(float timeStep)
{
	static float angle_ = 0.0f;

	if (radius_ > 0.0f) 
	{
		angle_ += (speed_ * timeStep);
		Vector3 finalPosition = startPosition + Vector3( cos(angle_) * radius_, 0.0f, sin(angle_) * radius_);
		GetNode()->SetWorldPosition(finalPosition);
	}

}

void Rotator::SetRotateParam(float radius, float speed) 
{
	radius_ = radius;
	speed_ = speed;
}

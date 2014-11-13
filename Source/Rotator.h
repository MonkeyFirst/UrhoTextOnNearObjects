#pragma once
#include "Common.h"
#include "LogicComponent.h"

namespace Urho3D
{
	class Node;
	class Scene;
	class Plane;
	class RigidBody;
	class LogicComponent;
	class Controls;
	class Drawable;

};

using namespace Urho3D;

class Rotator : public LogicComponent 
{
	OBJECT(Rotator);
public:
	/// Construct.
	Rotator(Context* context);

	/// Register object factory and attributes.
	static void RegisterObject(Context* context);

	/// Handle startup. Called by LogicComponent base class.
	virtual void Start();

	// גûחûגאועסÿ ךאזהûי ךאהנ ?
	void Update(float timeStep);

	/// Handle physics world update. Called by LogicComponent base class.
	virtual void FixedUpdate(float timeStep);

	void SetRotateParam(float radius, float speed);

private:
	float radius_;
	float speed_;
	Vector3 startPosition;




};
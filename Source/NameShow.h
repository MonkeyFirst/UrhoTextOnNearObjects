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
	class Text3D;

};

using namespace Urho3D;

class NameShow : public LogicComponent 
{
	OBJECT(NameShow);
public:
	/// Construct.
	NameShow(Context* context);
	static void RegisterObject(Context* context);
	virtual void Start();
	void Update(float timeStep);
	virtual void FixedUpdate(float timeStep);

	
	void SetTextViewDistance(float distance);
	void SetPlayerNode(Node* player);
	void SetAlwaysLookToPlayer(bool lookToPlayer);

private:
	SharedPtr<Node> player_;
	float distance_;
	SharedPtr<Node> text_;
	bool lookToPlayer_;

	void HandleNodeCollision(StringHash eventType, VariantMap& eventData);

};
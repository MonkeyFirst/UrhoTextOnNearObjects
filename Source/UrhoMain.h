#pragma once

#include "Application.h"
#include "Plane.h"
#include "RigidBody.h"

// My Scripts 
#include "NameShow.h"
#include "Character.h"


namespace Urho3D
{
	class Node;
	class Scene;
	class Plane;

}

using namespace Urho3D;

class UrhoMain : public Application
{
	OBJECT(UrhoMain);

public:
	UrhoMain(Context* context);
	virtual void Start();

protected:
	SharedPtr<Scene> scene_;
	SharedPtr<Node> cameraNode_;
	SharedPtr<Node> playerNode_;
	WeakPtr<Character> playerScript_;

private:
	void CreateConsole();
	void SubscribeToEvents();
	void LoadScene(Urho3D::String sceneFileName);
	void CameraSetup(Urho3D::String cameraNodeName);
	void OtherSetup();
	void CameraViewRotate(float timeStep);
	void GenerateNavMesh();
	bool Raycast(float maxDistance, Vector3& hitPos, Drawable*& hitDrawable);

	void HandleUpdate(StringHash eventType, VariantMap& eventData);
	void HandleKeyDown(StringHash eventType, VariantMap& eventData);
	void HandleSceneUpdate(StringHash eventType, VariantMap& eventData);
	void HandlePostRenderUpdate(StringHash eventType, VariantMap& eventData);

	bool paused_;
	float yaw_;
	float pitch_;

	bool drawDebug_;


};
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
	class VertexBuffer;
	class IndexBuffer;
	class Geometry;
	class Model;

};

using namespace Urho3D;

typedef struct DYN_VERTEX 
{
	Vector3 p;
	Vector3 n;
	Vector2 uv;
} DYN_VERTEX;

class LineGenerator : public LogicComponent 
{
	OBJECT(LineGenerator);
public:
	/// Construct.
	LineGenerator(Context* context);
	/// Register object factory and attributes.
	static void RegisterObject(Context* context);
	
	
	virtual void Start();

	void Update(float timeStep);

	virtual void FixedUpdate(float timeStep);
	virtual void PostUpdate(float timeStep);

private:
	PODVector<Vector3> tail_;
	PODVector<DYN_VERTEX> tailMesh_;

	int tailLength_;
	SharedPtr<Node> dynNode_; // scene parent


	SharedPtr<Model> fromScratchModel;
	SharedPtr<VertexBuffer> vb;
	SharedPtr<IndexBuffer> ib;
	SharedPtr<Geometry> geom;




};
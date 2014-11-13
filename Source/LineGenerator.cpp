#include "Common.h"
#include "LineGenerator.h"
//#include <vector>




LineGenerator::LineGenerator(Context* context) : LogicComponent(context)
{
	// Only the physics update event is needed: unsubscribe from the rest for optimization
	SetUpdateEventMask(USE_FIXEDUPDATE | USE_POSTUPDATE);
	
}

void LineGenerator::RegisterObject(Context* context)
{
	context->RegisterFactory<LineGenerator>();
}

void LineGenerator::Start()
{
	ResourceCache* cache = GetSubsystem<ResourceCache>();

	tailLength_ = 20;
	
	fromScratchModel = new Model(context_);
	vb = new VertexBuffer(context_);
	ib = new IndexBuffer(context_);
	geom = new Geometry(context_);
	
	const unsigned numVertices = tailLength_* 4;

	PODVector<DYN_VERTEX> vertexData;
	vertexData.Resize(numVertices);
	PODVector<unsigned short> indexData;
	indexData.Resize(numVertices);

	Vector3 xstep = Vector3(1.0f, 0.0f, 0.0f);
	Vector3 ystep = Vector3(0.0f, 1.0f, 0.0f);
	Vector3 curPos = Vector3(0,0.0f,0);

	for (int i =0; i < (numVertices); i += 4) 
	{
		vertexData[i].p = curPos;
		curPos += ystep;
		vertexData[i+1].p = curPos;
		curPos += xstep;
		curPos -= ystep;
		vertexData[i+2].p = curPos;
		curPos += ystep;
		vertexData[i+3].p = curPos;
		curPos -= ystep;

		vertexData[i].n = Vector3::UP;
		vertexData[i+1].n = Vector3::UP;
		vertexData[i+2].n = Vector3::UP;
		vertexData[i+3].n = Vector3::UP;

		vertexData[i].uv = Vector2(0.0f, 0.0);
		vertexData[i+1].uv = Vector2(0.0f, 1.0f);
		vertexData[i+2].uv = Vector2(1.0f, 0.0f);
		vertexData[i+3].uv = Vector2(1.0f, 1.0f);

	}
	//indexes
	for (int i =0; i < numVertices; ++i) 
	{
		indexData[i]=i;
	}

	

	//vb->SetShadowed(true);
	vb->SetSize(numVertices, MASK_POSITION|MASK_NORMAL|MASK_TEXCOORD1);
	vb->SetData(&vertexData[0]);
	

	ib->SetShadowed(true);
	ib->SetSize(numVertices, false);
	ib->SetData(&indexData[0]);

	geom->SetVertexBuffer(0, vb);
	geom->SetIndexBuffer(ib);
	geom->SetDrawRange(TRIANGLE_STRIP, 0, numVertices);
	

	fromScratchModel->SetNumGeometries(1);
	fromScratchModel->SetGeometry(0, 0, geom);
	fromScratchModel->SetBoundingBox(BoundingBox(Vector3(-5.0f, -5.0f, -5.0f), Vector3(5.0f, 5.0f, 5.0f)));

	dynNode_ = GetNode()->GetScene()->CreateChild("dynNode", LOCAL);
	StaticModel* object = dynNode_->CreateComponent<StaticModel>();
	object->SetModel(fromScratchModel);

	object->SetMaterial(cache->GetResource<Material>("Materials/LineGenerator.xml"));
	


}
void LineGenerator::Update(float timeStep) 
{
	dynNode_->SetWorldPosition(GetNode()->GetWorldPosition());
	

}

void LineGenerator:: PostUpdate(float timeStep)
{

	
	DebugRenderer* debug = GetNode()->GetScene()->GetComponent<DebugRenderer>();
	if (tail_.Size() > 0) 
	{
		debug->AddLine(GetNode()->GetWorldPosition(), tail_[tail_.Size()-1], Color(1.0f, 1.0f, 1.0f));
		
		for (unsigned i = 0; i < tail_.Size() - 1; ++i)
			debug->AddLine(tail_[i], tail_[i + 1], Color(1.0f, 1.0f, 1.0f));
				
	}

	if (tailMesh_.Size() > 0) 
	{
		//debug->AddLine(GetNode()->GetWorldPosition(), tailMesh_[tailMesh_.Size()-1].p, Color(0.0f, 1.0f, 0.0f));

		for (unsigned i = 0; i < tailMesh_.Size() - 1; ++i)
			debug->AddLine(tailMesh_[i].p, tailMesh_[i + 1].p, Color(0.0f, 1.0f, 0.0f));

	}



}


void LineGenerator::FixedUpdate(float timeStep)
{
	static float nextMark_ = 0.0f;
	nextMark_ += timeStep;
	static Vector3 lastPos = Vector3::ONE;
	Vector3 moveDir = lastPos - GetNode()->GetWorldPosition();
	static Vector3 lo = GetNode()->GetWorldPosition();
	moveDir.Normalize();

	if (nextMark_ > 0.25f) 
	{
		if (tail_.Size() >= tailLength_) 
		{
			tail_.Erase(0);
		}

		tail_.Push(GetNode()->GetWorldPosition());
		
		/// generate mesh
		//Vector3 dir = GetNode()->GetWorldDirection();
		//Quaternion quat = GetNode()->GetWorldRotation();
		//dir.Normalize();
		
		if (tailMesh_.Size() >= tailLength_ * 4) 
		{
			tailMesh_.Erase(0,4);	
		}


		Vector3 origin = lo;
		Vector3 xstep;
		xstep = moveDir;
		Vector3 ystep;
		ystep = Vector3::UP.Normalized();
		Vector3 curPos;

		DYN_VERTEX v1, v2, v3, v4;

		if (tailMesh_.Size() == 1) 
		{
			origin =  GetNode()->GetWorldPosition() + ystep/2.0f;
			curPos = origin;
			
			v1.p = curPos;
			v1.n = Vector3::FORWARD;
			v1.uv = Vector2(0,0);

			curPos +=ystep;
			v2.p = curPos;
			v2.n = Vector3::FORWARD;
			v2.uv = Vector2(0,1);


			curPos -=xstep;
			curPos -=ystep;
			v3.p = curPos;
			v3.n = Vector3::FORWARD;
			v3.uv = Vector2(1,0);


			curPos +=ystep;
			v4.p = curPos;
			v4.n = Vector3::FORWARD;
			v4.uv = Vector2(1,1);

			tailMesh_.Push(v1);
			tailMesh_.Push(v2);
			tailMesh_.Push(v3);
			tailMesh_.Push(v4);

			//lo = v4.p;

		
		}
		else 
		{
			curPos = GetNode()->GetWorldPosition() + ystep/2.0f;

			curPos -= xstep;
			curPos -= ystep;
			v3.p = curPos;
			v3.n = Vector3::FORWARD;
			v3.uv = Vector2(1,0);


			curPos +=ystep;
			v4.p = curPos;
			v4.n = Vector3::FORWARD;
			v4.uv = Vector2(1,1);

			tailMesh_.Push(v3);
			tailMesh_.Push(v4);

			//lo = v4.p;
		}

		

		nextMark_ = 0.0f;
		lastPos = GetNode()->GetWorldPosition();
		geom->SetDrawRange(TRIANGLE_STRIP, 0, tailMesh_.Size());


		if (tailMesh_.Size() < 1) return;

		//gen vb
		unsigned vertexSize = vb->GetVertexSize();
		unsigned numVertexes = vb->GetVertexCount();

		DYN_VERTEX* vertexData = (DYN_VERTEX*)vb->Lock(0, vb->GetVertexCount());
		if (vertexData) 
		{
			memcpy(vertexData, &tailMesh_[0], tailMesh_.Size() * vertexSize);
		}
		vb->Unlock();

	}





}

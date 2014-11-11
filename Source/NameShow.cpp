#include "Common.h"
#include "NameShow.h"


void NameShow::SetTextViewDistance(float distance) 
{
	distance_ = distance;
}

void NameShow::SetPlayerNode(Node* player) 
{
	player_ = SharedPtr<Node>(player);
}

void NameShow::SetAlwaysLookToPlayer(bool lookToPlayer)
{
	lookToPlayer_ = lookToPlayer;
}

NameShow::NameShow(Context* context) : LogicComponent(context)
{
	// Only the physics update event is needed: unsubscribe from the rest for optimization
	SetUpdateEventMask(USE_FIXEDUPDATE);

}

void NameShow::RegisterObject(Context* context)
{
	context->RegisterFactory<NameShow>();
}

void NameShow::Start()
{
	SubscribeToEvent(GetNode(), E_NODECOLLISION, HANDLER(NameShow, HandleNodeCollision));

	text_ = GetNode()->CreateChild("text", LOCAL);
	text_->SetPosition(Vector3(0.0f, 1.0f, 0.0f));
	text_->SetScale(Vector3::ONE);
	Text3D* titleText = text_->CreateComponent<Text3D>();
	
	Variant name_ = GetNode()->GetVar("name");
	String s = name_.GetString();
	
	if (s.Length() > 0)
		titleText->SetText(s);
	else
		titleText->SetText("no name");

	ResourceCache* cache = GetSubsystem<ResourceCache>();
	//titleText->SetFont(cache->GetResource<Font>("Fonts/BlueHighway.sdf"), 24);
	titleText->SetFont(cache->GetResource<Font>("Fonts/myfont.sdf"), 24);
	titleText->SetColor(Color::RED);
	
	//titleText->SetTextEffect(TE_);
	//titleText->SetEffectColor(Color(0.5f, 0.5f, 0.5f));
	titleText->SetAlignment(HA_CENTER, VA_CENTER);
	//text_->SetEnabledRecursive(false);

	lookToPlayer_ = true;

}

void NameShow::Update(float timeStep) 
{




}

void NameShow::FixedUpdate(float timeStep)
{
	Vector3 dir = (player_->GetWorldPosition() - GetNode()->GetWorldPosition());
	float curDistance = dir.Length();
	
	if (curDistance < distance_) //Show 
	{
		//text_->SetEnabledRecursive(true);

	}
	else // hide node
	{
		//text_->SetEnabledRecursive(false);
	}

	if (lookToPlayer_) 
	{
		//Vector3 norm = dir;
		//norm.Normalize();
		//text_->LookAt(dir, Vector3::UP, TS_WORLD);
	}
}

void NameShow::HandleNodeCollision(StringHash eventType, VariantMap& eventData)
{
	using namespace NodeCollision;


}

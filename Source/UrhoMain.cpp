#include "Common.h"

DEFINE_APPLICATION_MAIN(UrhoMain)

UrhoMain::UrhoMain(Context* context) : Application(context)
{
	engineParameters_["WindowTitle"] = GetTypeName();
	engineParameters_["FullScreen"]  = false;
	engineParameters_["Headless"]    = false;
	engineParameters_["WindowWidth"] = 1280;
	engineParameters_["WindowHeight"] = 720; 
	engineParameters_["LogName"]     = GetSubsystem<FileSystem>()->GetAppPreferencesDir("urho3d", "logs") + GetTypeName() + ".log";
	//engineParameters_["RenderPath"] = "Bin\CoreData\RenderPaths\Deferred.xml";
	//engineParameters_["FlushGPU"] = true;

	//Register my scripts
	NameShow::RegisterObject(context_);
	Character::RegisterObject(context);

}

void UrhoMain::Start()
{
	Graphics* graphics = GetSubsystem<Graphics>();
	Renderer* renderer = GetSubsystem<Renderer>();
	ResourceCache* cache = GetSubsystem<ResourceCache>();

	renderer->SetTextureFilterMode(FILTER_ANISOTROPIC);
	renderer->SetTextureAnisotropy(16);

	CreateConsole();
	SubscribeToEvents();
	LoadScene("Scene.xml");
	CameraSetup("cameraNode");
	OtherSetup();
	GenerateNavMesh();

}

void UrhoMain::GenerateNavMesh()
{
	NavigationMesh* navMesh = scene_->CreateComponent<NavigationMesh>();
	scene_->CreateComponent<Navigable>();
	navMesh->SetPadding(Vector3(0.0f, 1.0f, 0.0f));
	navMesh->SetAgentRadius(2.0f);
	navMesh->Build();
}


void UrhoMain::LoadScene(Urho3D::String sceneFileName) 
{
	scene_ = new Scene(context_);
	File sceneFile(context_, GetSubsystem<FileSystem>()->GetProgramDir() + "Data/Scenes/" + sceneFileName, FILE_READ);
	scene_->LoadXML(sceneFile);

	

}

void UrhoMain::CameraSetup(Urho3D::String cameraNodeName)
{
	Renderer* renderer = GetSubsystem<Renderer>();

	cameraNode_ = scene_->GetChild(cameraNodeName, true);
	Camera* camera = cameraNode_->GetComponent<Camera>();
	camera->SetFarClip(1000.0f);
	camera->SetFov(75.0f);
	// Вид
	SharedPtr<Viewport> viewport(new Viewport(context_, scene_, cameraNode_->GetComponent<Camera>()));
	renderer->SetViewport(0, viewport);

	yaw_ = 0.0f;
	pitch_ = 0.0f;	
}

bool UrhoMain::Raycast(float maxDistance, Vector3& hitPos, Drawable*& hitDrawable)
{
	hitDrawable = 0;

	UI* ui = GetSubsystem<UI>();
	IntVector2 pos = ui->GetCursorPosition();

	Graphics* graphics = GetSubsystem<Graphics>();
	Camera* camera = cameraNode_->GetComponent<Camera>();
	Ray cameraRay = camera->GetScreenRay((float)pos.x_ / graphics->GetWidth(), (float)pos.y_ / graphics->GetHeight());
	// Pick only geometry objects, not eg. zones or lights, only get the first (closest) hit
	PODVector<RayQueryResult> results;
	RayOctreeQuery query(results, cameraRay, RAY_TRIANGLE, maxDistance, DRAWABLE_GEOMETRY);
	scene_->GetComponent<Octree>()->RaycastSingle(query);
	if (results.Size())
	{
		RayQueryResult& result = results[0];
		hitPos = result.position_;
		hitDrawable = result.drawable_;
		return true;
	}

	return false;
}


void UrhoMain::OtherSetup()
{
	Graphics* graphics = GetSubsystem<Graphics>();
	Renderer* renderer = GetSubsystem<Renderer>();
	ResourceCache* cache = GetSubsystem<ResourceCache>();

	playerNode_ = scene_->GetChild("playerNode", true);
	playerScript_ = playerNode_->CreateComponent<Character>();

	PODVector<Node*> objects; 
	scene_->GetChildrenWithComponent<StaticModel>(objects, true);

	for (int i=0; i< objects.Size(); ++i)
	{
		Variant name_ = objects[i]->GetVar("name");
		if (name_.GetString().Length() > 0) 
		{
			NameShow* ns = objects[i]->CreateComponent<NameShow>();
			ns->SetTextViewDistance(5.0f);
			ns->SetPlayerNode(playerNode_);
		}
	}



}

void UrhoMain::CreateConsole()
{
	// Get default style
	ResourceCache* cache = GetSubsystem<ResourceCache>();
	XMLFile* xmlFile = cache->GetResource<XMLFile>("UI/DefaultStyle.xml");

	// Create console
	Console* console = engine_->CreateConsole();
	console->SetDefaultStyle(xmlFile);
	console->GetBackground()->SetOpacity(0.8f);
	
}


void UrhoMain::SubscribeToEvents()
{
	SubscribeToEvent(E_UPDATE, HANDLER(UrhoMain, HandleUpdate));
	SubscribeToEvent(E_KEYDOWN, HANDLER(UrhoMain, HandleKeyDown));
	SubscribeToEvent(E_SCENEUPDATE, HANDLER(UrhoMain, HandleSceneUpdate));
	SubscribeToEvent(E_POSTRENDERUPDATE, HANDLER(UrhoMain, HandlePostRenderUpdate));
}

void UrhoMain::CameraViewRotate(float timeStep) 
{
	Input* input = GetSubsystem<Input>();

	// Movement speed as world units per second
	const float MOVE_SPEED = 800.0f;
	// Mouse sensitivity as degrees per pixel
	const float MOUSE_SENSITIVITY = 0.1f;

	// Use this frame's mouse motion to adjust camera node yaw and pitch. Clamp the pitch between -90 and 90 degrees
	IntVector2 mouseMove = input->GetMouseMove();

	yaw_ += MOUSE_SENSITIVITY * mouseMove.x_;
	pitch_ += MOUSE_SENSITIVITY * mouseMove.y_;
	pitch_ = Clamp(pitch_, -90.0f, 90.0f);
	//yaw_ = Clamp(yaw_, -720.0f, 720.0f);

	// Construct new orientation for the camera scene node from yaw and pitch. Roll is fixed to zero
	cameraNode_->SetWorldRotation(Quaternion(pitch_, yaw_, 0.0f));

}


void UrhoMain::HandleUpdate(StringHash eventType, VariantMap& eventData)
{
	using namespace Update;

	// Take the frame time step, which is stored as a float
	float timeStep = eventData[P_TIMESTEP].GetFloat();

	CameraViewRotate(timeStep);
}


void UrhoMain::HandleKeyDown(StringHash eventType, VariantMap& eventData)
{
	using namespace KeyDown;
	int key = eventData[P_KEY].GetInt();
	
	if (key == KEY_ESC)
	{
		Console* console = GetSubsystem<Console>();
		if (console->IsVisible())
			console->SetVisible(false);
		else

			engine_->Exit();
	}
	else if (key == KEY_F1) 
	{
		Console* console = GetSubsystem<Console>();
		console->Toggle();
	}

	if (key == KEY_F2) drawDebug_ = !drawDebug_;

}

void UrhoMain::HandleSceneUpdate(StringHash eventType, VariantMap& eventData)
{


}

void UrhoMain::HandlePostRenderUpdate(StringHash eventType, VariantMap& eventData)
{

}
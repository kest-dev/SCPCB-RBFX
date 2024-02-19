#include <Urho3D/Scene/Node.h>
#include <Urho3D/Core/Context.h>

#include "GameWorld.h"

GameWorld::GameWorld(Context *context)
    : GameState(context)
{
    RMesh::RegisterObject(context);
    Room::RegisterObject(context);
    Player::RegisterObject(context);
}

void GameWorld::RegisterObject(Context *context)
{
    if (!context->IsReflected<GameWorld>())
        context->AddFactoryReflection<GameWorld>();
}

void GameWorld::Start()
{
    Node* roomNode = scene_->CreateChild();
    Room* intro173 = roomNode->CreateComponent<Room>();
    intro173->LoadXML("Rooms/173bright/room.xml");

    Node* playerNode = scene_->CreateChild();
    playerNode->SetPosition(Vector3(0, 0, 0));
    Player* player = playerNode->CreateComponent<Player>();
}

void GameWorld::Update(VariantMap &eventData)
{

}

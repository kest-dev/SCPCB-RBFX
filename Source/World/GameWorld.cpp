#include <Urho3D/Scene/Node.h>
#include <Urho3D/Core/Context.h>
#include <Urho3D/SystemUI/SystemUI.h>
#include <Urho3D/SystemUI/TransformGizmo.h>
#include <Urho3D/SystemUI/Console.h>

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
    ui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable | ImGuiConfigFlags_NavEnableKeyboard;
    ui::GetIO().BackendFlags |= ImGuiBackendFlags_HasMouseCursors;

    Node* roomNode = scene_->CreateChild();
    Room* intro173 = roomNode->CreateComponent<Room>();
    intro173->LoadXML("Rooms/173bright/room.xml");

    Node* playerNode = scene_->CreateChild();
    playerNode->SetPosition(Vector3(0, 0, 0));
    player_ = playerNode->CreateComponent<Player>();

    SubscribeToEvent(E_UPDATE, URHO3D_HANDLER(GameWorld, Update));
}

void GameWorld::Update(VariantMap &eventData)
{
    auto input = GetSubsystem<Input>();
    if(input->GetKeyPress(KEY_0))
        mouseEnabled_ = !mouseEnabled_;

    input->SetMouseMode(mouseEnabled_ ? MM_FREE : MM_ABSOLUTE); // TODO: Check if "MM_ABSOLUTE" is correct!
    input->SetMouseVisible(mouseEnabled_);

    ui::SetNextWindowSize(ImVec2(200, 300), ImGuiCond_FirstUseEver);
    ui::SetNextWindowPos(ImVec2(200, 300), ImGuiCond_FirstUseEver);
    if (ui::Begin("Debug", 0, ImGuiWindowFlags_NoSavedSettings))
    {
        ImGui::SliderFloat("Player Injury", &hp, 0.0f, 3.0f);

        player_->SetHealth(hp);
    }

    ui::End();
}

// Copyright (c) 2023-2023 the rbfx project.
// This work is licensed under the terms of the MIT license.
// For a copy, see <https://opensource.org/licenses/MIT> or the accompanying LICENSE file.

#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/Engine/Application.h>
#include <Urho3D/Engine/EngineDefs.h>
#include <Urho3D/Graphics/Octree.h>
#include <Urho3D/Graphics/Zone.h>
#include <Urho3D/Scene/Scene.h>

#include "Main.h"

#include "World/GameWorld.h"

// Define entry point.
URHO3D_DEFINE_APPLICATION_MAIN(SCPCB);

SCPCB::SCPCB(Context* context)
    : Application(context)
{
    GameWorld::RegisterObject(context);
}

void SCPCB::Setup()
{
    engineParameters_[EP_ORGANIZATION_NAME] = "SCP - Containment Breach";
    engineParameters_[EP_APPLICATION_NAME] = "SCP - Containment Breach RBFX";
    engineParameters_[EP_LOG_NAME] = "conf://SCPCB.log";
}

void SCPCB::Start()
{
    auto stateManager = GetSubsystem<StateManager>();

    stateManager->EnqueueState(GameWorld::GetTypeStatic());
}

void SCPCB::Stop()
{

}

void SCPCB::Update(Urho3D::VariantMap &eventData)
{

}

GameState::GameState(Context* context)
        : BaseClassName(context)
{
    mouseEnabled_ = false;
}

void GameState::Activate(Urho3D::StringVariantMap &bundle)
{
    // Create scene and octree.
    scene_ = new Scene(context_);
    scene_->CreateComponent<Octree>();

    // Create zone.
    Zone* zone = scene_->CreateComponent<Zone>();
    zone->SetFogColor(Color::BLACK);

    // Start the game state.
    Start();

    // Set the mouse to be enabled or disabled, depending on what is set for the game state.
    auto input = GetSubsystem<Input>();
    input->SetMouseMode(mouseEnabled_ ? MM_FREE : MM_ABSOLUTE); // TODO: Check if "MM_ABSOLUTE" is correct!
    input->SetMouseVisible(mouseEnabled_);
}
// Copyright (c) 2023-2023 the rbfx project.
// This work is licensed under the terms of the MIT license.
// For a copy, see <https://opensource.org/licenses/MIT> or the accompanying LICENSE file.

#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/Engine/Application.h>
#include <Urho3D/Engine/EngineDefs.h>
#include <Urho3D/Graphics/Camera.h>
#include <Urho3D/Graphics/Light.h>
#include <Urho3D/Graphics/Material.h>
#include <Urho3D/Graphics/Model.h>
#include <Urho3D/Graphics/Octree.h>
#include <Urho3D/Graphics/Renderer.h>
#include <Urho3D/Graphics/StaticModel.h>
#include <Urho3D/Graphics/Viewport.h>
#include <Urho3D/Graphics/Zone.h>
#include <Urho3D/Input/Input.h>
#include <Urho3D/Input/InputEvents.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Scene/Scene.h>

#include "Main.h"

SCPCB::SCPCB(Context *context)
    : Application(context)
{
}

void SCPCB::Setup()
{
    // Organization and application names are used to create writeable folder in OS-specific location.
    // For example, on Windows it would be C:/Users/<username>/AppData/Roaming/<orgname>/<appname>
    engineParameters_[EP_ORGANIZATION_NAME] = "SCP - Containment Breach";
    engineParameters_[EP_APPLICATION_NAME] = "SCP - Containment Breach RBFX";
    // conf:// directory is mapped to that writeable folder.
    engineParameters_[EP_LOG_NAME] = "conf://SCPCB.log";
}

void SCPCB::Start()
{
    auto cache = GetSubsystem<ResourceCache>();
    auto renderer = GetSubsystem<Renderer>();

    // Create scene.
    scene_ = MakeShared<Scene>(context_);
    scene_->CreateComponent<Octree>();

    // Create camera.
    Node* cameraNode = scene_->CreateChild("Camera");
    Camera* camera = cameraNode->CreateComponent<Camera>();

    // Create zone.
    Zone* zone = scene_->CreateComponent<Zone>();
    zone->SetFogColor(0xC9C0BB_rgb);

    // Create box geometry.
    geometryNode_ = scene_->CreateChild("Box");
    geometryNode_->SetPosition(Vector3{0.0f, 0.0f, 5.0f});
    geometryNode_->SetRotation(Quaternion{-30.0f, 60.0f, 50.0f});
    StaticModel* geometry = geometryNode_->CreateComponent<StaticModel>();
    geometry->SetModel(cache->GetResource<Model>("Models/Box.mdl"));
    geometry->SetMaterial(cache->GetResource<Material>("Materials/DefaultGrey.xml"));

    // Create light.
    Node* lightNode = scene_->CreateChild("Light");
    lightNode->SetDirection(Vector3(0.6f, -1.0f, 0.8f));
    Light* light = lightNode->CreateComponent<Light>();
    light->SetLightType(LIGHT_DIRECTIONAL);

    // Create viewport.
    const auto viewport = MakeShared<Viewport>(context_, scene_, camera);
    renderer->SetViewport(0, viewport);

    SubscribeToEvent(E_UPDATE, &SCPCB::Update);
}

void SCPCB::Stop()
{

}

void SCPCB::Update(VariantMap& eventData)
{
    const float timeStep = eventData[Update::P_TIMESTEP].GetFloat();
    geometryNode_->Rotate(Quaternion{10 * timeStep, Vector3::UP}, TS_WORLD);

    auto input = GetSubsystem<Input>();
    if (input->GetKeyPress(KEY_ESCAPE))
        SendEvent(E_EXITREQUESTED);
}

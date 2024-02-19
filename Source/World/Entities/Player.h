#pragma once

#include <Urho3D/Input/MoveAndOrbitComponent.h>
#include <Urho3D/Input/MoveAndOrbitController.h>
#include <Urho3D/Graphics/Camera.h>
#include <Urho3D/Physics/KinematicCharacterController.h>
#include <Urho3D/Input/FreeFlyController.h>
#include <Urho3D/Scene/Node.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Input/InputMap.h>
#include <Urho3D/Core/Context.h>

using namespace Urho3D;

class Player : public MoveAndOrbitComponent
{
    URHO3D_OBJECT(Player, MoveAndOrbitComponent);
public:
    explicit Player(Context* context);

    static void RegisterObject(Context* context);

    void Start() override;

    void FixedUpdate(float timeStep) override;

private:
    void Update(VariantMap& eventData);

    Node* cameraNode_;

    WeakPtr<Camera> camera_;
    WeakPtr<FreeFlyController> cameraController_;
    WeakPtr<KinematicCharacterController> characterController_;

    SharedPtr<InputMap> inputMap_;

    float shake_;
    float crouchState_;
    float health_;
};
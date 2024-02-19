#include <Urho3D/Audio/Audio.h>
#include <Urho3D/Audio/SoundListener.h>
#include <Urho3D/Graphics/Renderer.h>
#include <Urho3D/Core/CoreEvents.h>

#include "Player.h"

Player::Player(Context *context)
        : MoveAndOrbitComponent(context)
        , shake_(0.0f)
        , health_(0.0f)
        , crouchState_(0.0f)
        , mouseSensitivity_(0.75f)
        , up_(0.0f)
        , sprint_(1.0f)
{
}

void Player::RegisterObject(Context *context)
{
    if(!context->IsReflected<Player>())
        context->AddFactoryReflection<Player>();
}

void Player::Start()
{
    const auto moveAndOrbit= node_->CreateComponent<MoveAndOrbitController>();

    moveAndOrbit->LoadInputMap("Input/PlayerControls.inputmap");

    inputMap_ = moveAndOrbit->GetInputMap();

    cameraNode_ = node_->CreateChild();
    cameraNode_->SetPosition(Vector3::UP * 1.3f);
    cameraNode_->CreateComponent<Camera>();
    camera_ = cameraNode_->GetComponent<Camera>();

    SoundListener* listener = cameraNode_->CreateComponent<SoundListener>();
    GetSubsystem<Audio>()->SetListener(listener);

    // Create viewport.
    const auto viewport = MakeShared<Viewport>(context_, GetScene(), camera_);
    GetSubsystem<Renderer>()->SetViewport(0, viewport);

    cameraFlyController_ = cameraNode_->CreateComponent<FreeFlyController>();
    cameraFlyController_->SetSpeed(0.0f);
    cameraFlyController_->SetAcceleratedSpeed(0.0f);
    cameraFlyController_->SetEnabled(false);

    characterController_ = node_->CreateComponent<KinematicCharacterController>();
    characterController_->SetHeight(1.05f);
    characterController_->SetDiameter(0.7f);
    characterController_->SetOffset(Vector3(0.0f, 0.9f, 0.0f));
    characterController_->SetStepHeight(0.05f);

    SubscribeToEvent(E_UPDATE, URHO3D_HANDLER(Player, Update));
}

void Player::Update(VariantMap& eventData)
{
    using namespace Update;

    float timeStep = eventData[P_TIMESTEP].GetFloat();

    //If (Not UnableToMove%) Then Shake# = (Shake + FPSfactor * Min(Sprint, 1.5) * 7) Mod 720
    /*
     Local up# = (Sin(Shake) / (20.0+CrouchState*20.0))*0.6;, side# = Cos(Shake / 2.0) / 35.0
		Local roll# = Max(Min(Sin(Shake/2)*2.5*Min(Injuries+0.25,3.0),8.0),-8.0)
     */

    if(GetVelocity().LengthSquared() > 0.0f)
    {
        shake_ = Mod((shake_ + (timeStep * 70.0f) * Min(sprint_, 1.5f) * 7.0f), 720.0f);
        up_ = (Sin(shake_) / (20.0f + crouchState_ * 20.0f)) * 0.6f;
    }

    float roll = Max(Min(Sin(shake_  / 2.0f) * 2.5f * Min(health_ + 0.25, 3.0f), 8.0f), -8.0f);

    const auto* input = GetSubsystem<Input>();
    const IntVector2 mouseMove = input->GetMouseMove();
    mouseMovement_.y_ += mouseSensitivity_ * mouseMove.x_;
    mouseMovement_.x_ += mouseSensitivity_ * mouseMove.y_;
    mouseMovement_.z_ = roll;

    mouseMovement_.x_ = Clamp(mouseMovement_.x_, -75.0f, 75.0f);

    cameraNode_->SetRotation(Quaternion(mouseMovement_));
    cameraNode_->SetPosition(Vector3(cameraNode_->GetPosition().x_,
                                     (node_->GetPosition().y_ + 1.65f) + up_ ,
                                     cameraNode_->GetPosition().z_));

    URHO3D_LOGDEBUGF("%f", cameraNode_->GetPosition().y_);
}

void Player::FixedUpdate(float timeStep)
{
    const float moveSpeed = 1.5f + sprint_;

    if (!camera_ || !characterController_)
    {
        URHO3D_LOGDEBUG("failed");
        return;
    }

    // Get local move direction.
    Vector3 localDirection = GetVelocity();
    localDirection.Normalize();

    if(inputMap_->Evaluate("Sprint"))
    {
        sprint_ = 2.5f;
    }
    else
    {
        sprint_ = 1.0f;
    }

    // Get world move direction.
    const float yawAngle = cameraNode_->GetWorldRotation().YawAngle();
    const Vector3 worldDirection = Quaternion{ yawAngle, Vector3::UP } * localDirection;
    Vector3 pos = node_->GetPosition();
    characterController_->SetWalkIncrement(worldDirection * moveSpeed * timeStep);
}
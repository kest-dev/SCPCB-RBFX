#include <Urho3D/Audio/Audio.h>
#include <Urho3D/Audio/SoundListener.h>
#include <Urho3D/Audio/Sound.h>
#include <Urho3D/Graphics/Renderer.h>
#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Graphics/StaticModel.h>
#include <Urho3D/Physics/PhysicsEvents.h>
#include <Urho3D/Physics/PhysicsWorld.h>
#include <Urho3D/Physics/RigidBody.h>

#include "Player.h"

Player::Player(Context *context)
        : MoveAndOrbitComponent(context)
        , shake_(0.0f)
        , injuries_(3.0f)
        , crouchState_(0.0f)
        , mouseSensitivity_(0.75f)
        , sprint_(1.0f)
{
    stepSounds_.push_back(StepSound(8, 8, "Step", "Run"));
    stepSounds_.push_back(StepSound(8, 8, "StepMetal", "RunMetal"));
    stepSounds_.push_back(StepSound(3, 0, "StepPD", ""));
    stepSounds_.push_back(StepSound(3, 0, "StepForest", ""));
}

void Player::RegisterObject(Context *context)
{
    if(!context->IsReflected<Player>())
        context->AddFactoryReflection<Player>();
}

void Player::DelayedStart()
{
    const auto moveAndOrbit= node_->CreateComponent<MoveAndOrbitController>();

    moveAndOrbit->LoadInputMap("Input/PlayerControls.inputmap");

    auto cache = GetSubsystem<ResourceCache>();

    inputMap_ = moveAndOrbit->GetInputMap();

    cameraNode_ = node_->CreateChild();
    cameraNode_->SetPosition(Vector3::UP * 1.3f);
    cameraNode_->CreateComponent<Camera>();
    camera_ = cameraNode_->GetComponent<Camera>();
    camera_->SetFov(60.0f);

    footStep_ = cameraNode_->CreateComponent<SoundSource>();
    playerSource_ = cameraNode_->CreateComponent<SoundSource>();

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

    for(int i = 0; i < stepSounds_.size(); i++)
    {
        for (int j = 0; j < Max(stepSounds_[i].maxWalkSounds_, stepSounds_[i].maxRunSounds_); j++)
        {
            if (j < stepSounds_[i].maxWalkSounds_)
            {
                stepSounds_[i].walkSounds_.push_back(
                        cache->GetResource<Sound>(ToString("Sounds/Step/%s%d.ogg", stepSounds_[i].walkSuffix_.c_str(), (j + 1))));
            }

            if (j < stepSounds_[i].maxRunSounds_)
            {
                stepSounds_[i].runSounds_.push_back(
                        cache->GetResource<Sound>(ToString("Sounds/Step/%s%d.ogg", stepSounds_[i].runSuffix_.c_str(), (j + 1))));
            }
        }
    }

    stepType_ = REGULAR;
}

void Player::Start()
{
    SubscribeToEvent(E_UPDATE, URHO3D_HANDLER(Player, Update));

    SubscribeToEvent(E_PHYSICSCOLLISION, URHO3D_HANDLER(Player, HandleNodeCollision));
}

void Player::Update(VariantMap& eventData)
{
    using namespace Update;

    float timeStep = eventData[P_TIMESTEP].GetFloat();

    float lastShake = shake_;

    if(GetVelocity().LengthSquared() > 0.0f)
    {
        //This 70.0f is left over from Containment Breach's FPSFactor value.
        shake_ = Mod((shake_ + (timeStep * 70.0f) * Min(sprint_, 1.5f) * 7.0f), 720.0f);
    }

    float up = (Sin(shake_) / (20.0f + crouchState_ * 20.0f)) * 0.6f;

    float roll = Max(Min(Sin(shake_  / 2.0f) * 2.5f * Min(injuries_ + 0.25, 3.0f), 8.0f), -8.0f);

    const auto* input = GetSubsystem<Input>();

    if(input->GetMouseMode() != MM_FREE)
    {
        const IntVector2 mouseMove = input->GetMouseMove();
        mouseMovement_.y_ += mouseSensitivity_ * mouseMove.x_;
        mouseMovement_.x_ += mouseSensitivity_ * mouseMove.y_;
        mouseMovement_.z_ = roll;

        mouseMovement_.x_ = Clamp(mouseMovement_.x_, -75.0f, 75.0f);

        cameraNode_->SetRotation(Quaternion(mouseMovement_));
        cameraNode_->SetPosition(Vector3(cameraNode_->GetPosition().x_,
                                         (node_->GetPosition().y_ + 1.65f) + up,
                                         cameraNode_->GetPosition().z_));
    }

    if ((lastShake <= 270.0f && shake_ > 270.0f) || (lastShake <= 630.0f && shake_ > 630.0f))
    {
        if(sprint_ > 1.0f)
        {
            playerSource_->Play(stepSounds_[stepType_].runSounds_[Random(0, stepSounds_[stepType_].maxRunSounds_ - 1)]);
        }
        else
        {
            playerSource_->Play(stepSounds_[stepType_].walkSounds_[Random(0, stepSounds_[stepType_].maxWalkSounds_ - 1)]);
        }
    }
}

void Player::DamagePlayer(float damage)
{
    injuries_ += damage / 100.0f;
}

void Player::FixedUpdate(float timeStep)
{
    float moveSpeed = (1.5f + sprint_) / (1.0f + crouchState_);
    moveSpeed = moveSpeed / Max((injuries_ + 3.0) / 3.0,1.0);

    if (!camera_ || !characterController_)
    {
        URHO3D_LOGDEBUG("failed");
        return;
    }

    // Get local move direction.
    Vector3 localDirection = GetVelocity();
    localDirection.Normalize();

    if(inputMap_->Evaluate("Sprint") && crouchState_ < 10.0f)
    {
        sprint_ = 2.5f;
    }
    else
    {
        sprint_ = 1.0f;
    }

    if (inputMap_->Evaluate("Crouch") && sprint_ < 2.5f)
    {
        characterController_->SetHeight(0.7f);

        cameraNode_->SetPosition(Vector3::UP * 0.65f);
        crouchState_ = 10.0f;
    }
    else
    {
        characterController_->SetHeight(1.05f);
        characterController_->SetOffset(Vector3(0.0f, 0.9f, 0.0f));

        cameraNode_->SetPosition(Vector3::UP * 1.3f);
        crouchState_ = 0.0f;
    }

    // Get world move direction.
    const float yawAngle = cameraNode_->GetWorldRotation().YawAngle();
    const Vector3 worldDirection = Quaternion{ yawAngle, Vector3::UP } * localDirection;
    Vector3 pos = node_->GetPosition();
    characterController_->SetWalkIncrement(worldDirection * moveSpeed * timeStep);
}

void Player::HandleNodeCollision(StringHash eventType, VariantMap& eventData)
{
    URHO3D_LOGDEBUG("testcollision");
}
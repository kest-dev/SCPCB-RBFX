#pragma once

#include "../Main.h"

#include "Entities/Player.h"

#include "Room/RMesh.h"
#include "Room/Room.h"
#include "Room/Rooms/Room173Intro.h"

using namespace Urho3D;

class GameWorld : public GameState
{
    URHO3D_OBJECT(GameWorld, GameState);

public:
    explicit GameWorld(Context* context);

    static void RegisterObject(Context* context);

protected:
    /// Activate game state. Executed by StateManager.
    void Start() override;

    void Update(VariantMap& eventData);

private:
    SharedPtr<Player> player_;

    float hp = 3.0f;
};

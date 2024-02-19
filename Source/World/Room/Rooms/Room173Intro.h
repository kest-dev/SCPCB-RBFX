#pragma once
#include "../Room.h"

#include <Urho3D/Scene/Component.h>

using namespace Urho3D;

class Room173Intro : public Room
{
    URHO3D_OBJECT(Room173Intro, Room);
public:
    explicit Room173Intro(Context* context);

    static void RegisterObject(Context* context);

    void OnSceneSet(Scene* scene) override;

protected:
    void FillRoom() override;
    void UpdateEvent(VariantMap &eventData) override;

private:
    void CreateDoor(Vector3& pos);
};
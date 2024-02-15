#include "Room173Intro.h"

Room173Intro::Room173Intro(Context *context)
    : Room(context)
{
    FillRoom();

    SubscribeToEvent(E_UPDATE, URHO3D_HANDLER(Room173Intro, UpdateEvent));
}

void Room173Intro::RegisterObject(Context *context)
{
    if(!context->IsReflected<Room173Intro>())
        context->AddFactoryReflection<Room173Intro>();
}

void Room173Intro::FillRoom()
{
    URHO3D_LOGDEBUG("Help me boowamp boowamp");
}

void Room173Intro::UpdateEvent(VariantMap &eventData)
{

}

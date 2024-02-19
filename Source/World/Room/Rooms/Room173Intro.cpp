#include "Room173Intro.h"

#include <Urho3D/Resource/ResourceCache.h>

Room173Intro::Room173Intro(Context *context)
    : Room(context)
{
}

void Room173Intro::RegisterObject(Context *context)
{
    if(!context->IsReflected<Room173Intro>())
        context->AddFactoryReflection<Room173Intro>();
}

void Room173Intro::OnSceneSet(Urho3D::Scene *scene)
{
    FillRoom();

    SubscribeToEvent(E_UPDATE, URHO3D_HANDLER(Room173Intro, UpdateEvent));
}

void Room173Intro::FillRoom()
{
    /*
     For ztemp = 0 To 1
				d.Doors = CreateDoor(r\zone, r\x - 5760 * RoomScale, 0, r\z + (320+896*ztemp) * RoomScale, 0, r, False)
				d\locked = True
				d\DisableWaypoint = True

				d.Doors = CreateDoor(r\zone, r\x - 8288 * RoomScale, 0, r\z + (320+896*ztemp) * RoomScale, 0, r, False)
				d\locked = True
				If ztemp = 0 Then d\open = True Else d\DisableWaypoint = True

				For xtemp = 0 To 2
					d.Doors = CreateDoor(r\zone, r\x - (7424.0-512.0*xtemp) * RoomScale, 0, r\z + (1008.0-480.0*ztemp) * RoomScale, 180*(Not ztemp), r, False)
				Next
				For xtemp = 0 To 4
					d.Doors = CreateDoor(r\zone, r\x - (5120.0-512.0*xtemp) * RoomScale, 0, r\z + (1008.0-480.0*ztemp) * RoomScale, 180*(Not ztemp), r, False)

					If xtemp = 2 And ztemp = 1 Then r\RoomDoors[6] = d
				Next
			Next
     */

    for(int ztemp = 0; ztemp < 2; ztemp++)
    {
        Vector3 idk1 = Vector3(-5760, 0, 320+896*ztemp) * ROOMSCALE;
        CreateDoor(idk1);

        Vector3 idk2 = Vector3(-8288, 0, 320+896*ztemp) * ROOMSCALE;
        CreateDoor(idk2);

        for(int xtemp = 0; xtemp < 3; xtemp++)
        {
            Vector3 idk3 = Vector3(-(7424.0-512.0*xtemp), 0, 1008.0-480.0*ztemp) * ROOMSCALE;
            CreateDoor(idk3);
        }

        for(int xtemp = 0; xtemp < 5; xtemp++)
        {
            Vector3 idk3 = Vector3(-(5120.0-512.0*xtemp), 0, 1008.0-480.0*ztemp) * ROOMSCALE;
            CreateDoor(idk3);
        }
    }

    URHO3D_LOGDEBUG("Help me boowamp boowamp");
}

void Room173Intro::UpdateEvent(VariantMap &eventData)
{

}

void Room173Intro::CreateDoor(Vector3 &pos)
{
    auto cache = GetSubsystem<ResourceCache>();

    Node* doortemp = node_->CreateChild();
    doortemp->SetPosition(pos);

    StaticModel* tempModel = doortemp->CreateComponent<StaticModel>();
    tempModel->SetModel(cache->GetResource<Model>("Models/Box.mdl"));
}

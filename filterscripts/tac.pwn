#include <a_samp>
#include <izcmd>
#include <igta_color>

public OnFilterScriptInit()
{
    print("Tuise - AntiCheat Loaded!");
    SetTimer("CheckVehicleXYZ",100,1);
}

stock GetName(playerid)
{
    new name[64];
    GetPlayerName(playerid,name,sizeof(name));
    return name;
}

COMMAND:tanti(playerid)
{
    if(!GetPVarInt(playerid,"is_antitest"))
    {
        SetPVarInt(playerid,"is_antitest",GetPlayerVehicleID(playerid));
        printf("Player:%s Enter Tanti!",GetName(playerid));
    }
    else
    {
        DeletePVar(playerid, "is_antitest");
    }
}

public OnUnoccupiedVehicleUpdate(vehicleid, playerid, passenger_seat, Float:new_x, Float:new_y, Float:new_z, Float:vel_x, Float:vel_y, Float:vel_z)
{
    if(vel_x == 0.000000 && vel_y == 0.000000  && vel_z == 0.000000)
    {
        new string[256];
        format(string,sizeof(string),"[T-AntiCheat]玩家%s(%d)试图崩溃其他玩家,已被系统拦截并踢出",GetName(playerid),playerid);
        SendClientMessageToAll(COLOR_RED, string);
        Kick(playerid);
    }
}

COMMAND:rcds(playerid)
{
    new string[256];
    format(string,sizeof(string),"[T-AntiCheat]玩家%s(%d)试图崩溃其他玩家,已被系统拦截并踢出",GetName(playerid),playerid);
    SendClientMessageToAll(COLOR_RED, string);
    Kick(playerid);
    return 1;
}

/*
forward CheckVehicleXYZ();
public CheckVehicleXYZ()
{
    for(new i=0;i<MAX_PLAYERS;i++)
    {
        if(GetPVarInt(i,"is_antitest"))
        {
            new Float:qw,Float:qx,Float:qy,Float:qz,Float:rx,Float:ry,Float:rz;
            GetVehicleRotationQuat(GetPVarInt(i,"is_antitest"),qw,qx,qy,qz);
            rx = asin(2*qy*qz-2*qx*qw);
            ry = -atan2(qx*qz+qy*qw,0.5-qx*qx-qy*qy);
            rz = -atan2(qx*qy+qz*qw,0.5-qx*qx-qz*qz);
            printf("%s -  %f %f %f",GetName(i),rx,ry,rz);
            if(rx)
            {
                new string[256];
                format(string,sizeof(string),"[T-AntiCheat]玩家%s(%d)试图崩溃其他玩家,已被系统拦截",GetName(i),i);
                SendClientMessageToAll(COLOR_RED, string);
            }
        }
    }
}
*/
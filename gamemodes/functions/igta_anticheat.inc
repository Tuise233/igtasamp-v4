#define MAX_VEHICLE_ID_CHANGES 5
new sLdz;
static LastVehicleID[MAX_PLAYERS];
static VehicleIDChanges[MAX_PLAYERS];
static VehicleIDChangeTime[MAX_PLAYERS];

forward CheatTips(playerid,str[]);
public CheatTips(playerid,str[])
{
    new string[1024];
    format(string,sizeof(string),"[T-AntiCheat]%s",str);
    if(playerid == -1){
        for(new i=0;i<MAX_PLAYERS;i++){
            SendClientMessage(i, COLOR_RED, string);
        }
        return 1;
    }
    SendClientMessage(playerid,COLOR_RED,string);
    return 1;
}


forward anticheat_OnGameModeInit();
public anticheat_OnGameModeInit()
{
    SetTimer("AntiRPWeapon",100,1);
    SetTimer("AntiUnLogin",1000,1);
    //SetTimer("AntiBot",1000,1);
    sLdz = GangZoneCreate(-245.219146,1489.957397,-405.205963,1596.486938);
    SetTimer("VehicleAqq",500,1);
    return;
}

//载具安全区判定
forward VehicleAqq();
public VehicleAqq()
{
    new Float:x,Float:y,Float:z;
    new name[64],string[256];
    new banVehicle[] = {403,406,443,432,514,515,524,532,573,449,537,538,569,570,590,430,446,452,453,454,472,473,383,393,539,595,417,425,447,460,469,476,487,488,497,511,512,513,519,520,548,553,563,577,592,593};
    for(new i=0;i<MAX_PLAYERS;i++)
    {
        if(IsPlayerConnected(i))
        {
            if(GetPVarInt(i,"is_rp")) continue;
            if(GetPlayerVirtualWorld(i) == 0)
            {
                if(IsPlayerInAnyVehicle(i))
                {
                    if(GetPlayerVehicleSeat(i) == 0)
                    {
                        GetPlayerPos(i,x,y,z);
                        //雷达站
                        //-245.219146,1489.957397,75.229408
                        //-405.205963,1596.486938,71.170364
                        if(x>-405.205963 && x < -245.219146 && y > 1489.957397 && y < 1596.486938)
                        {
                            GetPlayerName(i,name,sizeof(name));
                            for(new k=0;k<sizeof(banVehicle);k++)
                            {
                                if(banVehicle[k] == GetVehicleModel(GetPlayerVehicleID(i)))
                                {
                                    format(string,sizeof(string),"[T-AntiCheat]玩家%s(%d)驾驶着ID为%d的安全区禁止驾驶的载具进入雷达站安全区,已被系统自动删除",name,i,GetVehicleModel(GetPlayerVehicleID(i)));
                                    SendClientMessageToAll(COLOR_RED,string);
                                    SendClientMessage(i,COLOR_RED,"[T-AntiCheat]您可以通过 /aqq 来查看地图的安全区");
                                    //获取车主
                                    for(new j=0;j<MAX_PLAYERS;j++)
                                    {
                                        if(PlayerVehicle[j]== GetPlayerVehicleID(i))
                                        {
                                            if(i != j)
                                            {
                                                format(string,sizeof(string),"[T-AntiCheat]你的载具由于被玩家%s(%d)驾驶着进入安全区,目前已被删除,您可以重新生成载具",name,i);
                                                SendClientMessage(j,COLOR_RED,string);
                                            }
                                            PlayerVehicle[j] = -1;
                                            DestroyVehicle(GetPlayerVehicleID(i));
                                        }
                                    }
                                }
                            } 
                        }
                    }
                }
            }
        }
    }
    return 1;
}

COMMAND:aqq(playerid)
{
    if(!GetPVarInt(playerid,"is_aqq"))
    {
        SetPVarInt(playerid,"is_aqq",233);
        SendClientMessage(playerid,COLOR_GREEN,"[服务器]已为您打开服务器安全区范围(黄绿光切换)");
        SendClientMessage(playerid,COLOR_GREEN,"[服务器]再次输入 /aqq 将关闭显示");
        GangZoneShowForPlayer(playerid,sLdz,0x1AE694C8);
        GangZoneFlashForPlayer(playerid,sLdz,0xFFFF80C8);
    }
    else
    {
        DeletePVar(playerid, "is_aqq");
        SendClientMessage(playerid,COLOR_GREEN,"[服务器]已为您关闭服务器安全区范围");
        SendClientMessage(playerid,COLOR_GREEN,"[服务器]再次输入 /aqq 将重新显示");
        GangZoneHideForPlayer(playerid, sLdz);
    }
    return 1;
}

forward AntiRPWeapon();
public AntiRPWeapon()
{
    new bangun[] = {35,36,37,38,39,40};
    for(new i=0;i<MAX_PLAYERS;i++)
    {
        if(GetPlayerWeapon(i) == 44 || GetPlayerWeapon(i) == 45)
        {
            new name[64],string[256];
            GetPlayerName(i,name,sizeof(name));
            format(string,sizeof(string),"玩家%s(%d)在RP模式中携带违规武器,已被系统拦截并踢出服务器",name,i);
            CheatTips(-1,string);
            ResetPlayerWeapons(i);
            Kick(i);
        }
        if(GetPVarInt(i,"is_rp"))
        {
            for(new j=0;j<5;j++)
            {
                if(GetPlayerWeapon(i) == bangun[j])
                {
                    new name[64],string[256];
                    GetPlayerName(i,name,sizeof(name));
                    format(string,sizeof(string),"玩家%s(%d)在RP模式中携带违规武器,已被系统拦截",name,i);
                    CheatTips(-1,string);
                    ResetPlayerWeapons(i);
                }
            }
        }
    }
    return 1;
}


forward OnAntiCheatLagTroll(playerid);
public OnAntiCheatLagTroll(playerid)
{
    new name[64],string[256];
    GetPlayerName(playerid,name,sizeof(name));
    format(string,sizeof(string),"玩家%s(%d)使用第三方软件吸车骚扰玩家,已被系统拦截并踢出服务器",name,playerid);
    CheatTips(-1,string);
    Kick(playerid);
    return 1;
}

forward anticheat_OnPlayerUpdate(playerid);
public anticheat_OnPlayerUpdate(playerid)
{
    new vid = GetPlayerVehicleID(playerid);
    if(vid > 0)
    {
        if(vid != LastVehicleID[playerid])
        {
            if(GetTickCount() - VehicleIDChangeTime[playerid] < 5000)
            {
                VehicleIDChanges[playerid]++;
                if(VehicleIDChanges[playerid] > MAX_VEHICLE_ID_CHANGES)
                {
                    CallLocalFunction("OnAntiCheatLagTroll", "i", playerid);
                    return 0;
                }
            }
            else
            {
                VehicleIDChanges[playerid] = 1;
            }
            LastVehicleID[playerid] = vid;
            VehicleIDChangeTime[playerid] = GetTickCount();
        }
    }
    return 1;
}

forward AntiUnLogin();
public AntiUnLogin()
{
    new name[64],string[256];
    for(new i=0;i<MAX_PLAYERS;i++)
    {
        if(!GetPVarInt(i,"is_login"))
        {
            if(!GetPVarInt(i,"is_unlogin"))
            {
                SetPVarInt(i,"is_unlogin",1);
            }
            else
            {
                if(GetPVarInt(i,"is_unlogin") == 20)
                {
                    GetPlayerName(i,name,sizeof(name));
                    format(string,sizeof(string),"玩家%s(%d)超过20秒未登录,已被系统踢出",name,i);
                    CheatTips(-1,string);
                    Kick(i);
                }
                else
                {
                    format(string,sizeof(string),"您必须在%d秒内登录服务器,否则将被系统踢出",20-GetPVarInt(i,"is_unlogin"));
                    SetPVarInt(i,"is_unlogin",GetPVarInt(i,"is_unlogin")+1);
                    CheatTips(i,string);
                }
            }
        }
    }
    return 1;
}

forward AntiVehicle();
public AntiVehicle()
{
    for(new i=0;i<MAX_PLAYERS;i++)
    {
        if(IsPlayerConnected(i))
        {
            if(GetPlayerVirtualWorld(i) == 0)
            {
                if(GetPlayerVehicleSeat(i) == 0 )
                {
                    new vehBan[] = {403, 406, 407, 408, 417, 425, 432, 433, 435, 443, 450, 455, 478, 486, 520, 524, 532, 544, 569, 570, 584, 590, 591, 592};
                    for(new j=0;j<24;j++)
                    {
                        if(GetVehicleModel(GetPlayerVehicleID(i)) == vehBan[j])
                        {
                            new name[64],string[256];
                            GetPlayerName(i,name,sizeof(name));
                            format(string,sizeof(string),"[T-AntiCheat]玩家%s(%d)在大世界驾驶违规载具(ID%d),已被系统拦截并踢出服务器",name,i,GetVehicleModel(GetPlayerVehicleID(i)));
                            SendClientMessageToAll(COLOR_RED, string);
                            Kick(i);
                        }
                    }
                }
            }
        }
    }
    return 1;
}

// igta_dm

stock SendClientMessageToAllDm(content[])
{
    for(new i=0;i<MAX_PLAYERS;i++)
    {
        if(IsPlayerConnected(i))
        {
            if(GetPVarInt(i,"is_dm"))
            {
                SendClientMessage(i,COLOR_BLUE,content);
            }
        }
    }
}

forward dm_OnGameModeInit();
public dm_OnGameModeInit()
{
    SetTimer("AntiEnterDMWorld",1000,1);
}

forward AntiEnterDMWorld();
public AntiEnterDMWorld()
{
    for(new i=0;i<MAX_PLAYERS;i++)
    {
        if(IsPlayerConnected(i))
        {
            if(GetPlayerVirtualWorld(i) == 201)
            {
                if(!GetPVarInt(i,"is_dm"))
                {
                    new name[64],string[256];
                    GetPlayerName(i,name,sizeof(name));
                    format(string,sizeof(string),"[T-AntiCheat]玩家%s(%d)通过第三方方式进入DM世界,已被系统拦截并踢出服务器",name,i);
                    SendClientMessageToAll(COLOR_RED,string);
                    Kick(i);
                }
            }
        }
    }
    return 1;
}

forward dm_OnPlayerTakeDamage(playerid, issuerid, Float:amount, weaponid, bodypart);
public dm_OnPlayerTakeDamage(playerid, issuerid, Float:amount, weaponid, bodypart)
{
    if(GetPVarInt(playerid,"is_dm"))
    {
        if(GetPVarInt(issuerid,"is_dm"))
        {
            new Float:health,Float:armour;
            GetPlayerHealth(issuerid,health);
            GetPlayerArmour(issuerid,armour);
            if(armour > 0)
            {
                SetPlayerArmour(issuerid,armour + amount);
                SetPlayerArmour(issuerid,armour);
            }
            else
            {
                SetPlayerHealth(issuerid,health+amount);
                SetPlayerHealth(issuerid,health);
            }
            new string[256],name[64],name1[64];
            GetPlayerName(playerid,name,sizeof(name));
            GetPlayerName(issuerid,name1,sizeof(name1));
            format(string,sizeof(string),"[DM模式]玩家%s(%d)对玩家%s(%d)造成%f伤害,武器ID:%d",name,playerid,name1,issuerid,amount,weaponid);
            SendClientMessageToAllDm(string);
        }
    }
    return 1;
}

COMMAND:dm(playerid)
{
    new name[64],string[256];
    if(GetPVarInt(playerid,"is_rp")) return SendClientMessage(playerid,COLOR_BLUE,"[DM系统]您处于RP状态下,不可以加入DM模式");
    GetPlayerName(playerid,name,sizeof(name));
    if(!GetPVarInt(playerid,"is_dm"))
    {
        SetPVarInt(playerid,"is_dm",!GetPVarInt(playerid,"is_dm"));
        ResetPlayerWeapons(playerid);
        SetPlayerHealth(playerid,100);
        SetPlayerArmour(playerid,100);
        DeletePVar(playerid,"is_wudi");
        format(string,sizeof(string),"[DM模式]玩家%s(%d)加入DM模式,加入DM模式: /dm",name,playerid);
        SendClientMessage(playerid,COLOR_BLUE,"[DM模式]您已加入DM模式,手机玩家加入DM模式将受到伤害!伤害同步将于2020年8月大更新后开启");
        SendClientMessageToAll(COLOR_BLUE,string);
        SetPlayerVirtualWorld(playerid,201);
    }
    else
    {
        DeletePVar(playerid,"is_dm");
        SetPVarInt(playerid,"is_wudi",!GetPVarInt(playerid,"is_wudi"));
        SetPlayerHealth(playerid,99999);
        SetPlayerArmour(playerid,99999);
        ResetPlayerWeapons(playerid);
        SendClientMessage(playerid,COLOR_BLUE,"[DM模式]您已退出DM模式");
    }
    return 1;
}
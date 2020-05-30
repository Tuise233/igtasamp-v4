


COMMAND:w(playerid,params[])
{
    if(GetPVarInt(playerid,"is_rp")) return SendClientMessage(playerid,COLOR_RED,"[T-AntiCheat]您在RP模式内,无法切换小世界");
    new wid[18],string[256];
    if(sscanf(params,"s[18]",wid))
    {
        SetPlayerVirtualWorld(playerid,0);
        ServerTips(playerid,"已切换至大世界");
        return 1;
    }
    if(strval(wid))
    {
        if(!IsPlayerConnected(strval(wid))) return ServerTips(playerid,"该玩家不在线");
        format(string,sizeof(string),"已切换至ID为%d的小世界",strval(wid));
        ServerTips(playerid,string);
        if(IsPlayerInAnyVehicle(playerid))
        {
            if(GetPlayerVehicleSeat(GetPlayerVehicleID(playerid)) == 0)
            {
                new vid = GetPlayerVehicleID(playerid);
                new sid = GetPlayerVehicleSeat(playerid);
                SetVehicleVirtualWorld(vid,strval(wid));
                SetPlayerVirtualWorld(playerid,strval(wid));
                PutPlayerInVehicle(playerid,vid,sid);
                return 1;
            }
        }
        SetPlayerVirtualWorld(playerid,strval(wid));
    }
    return 1;
}

COMMAND:ww(playerid)
{
    SetPlayerVirtualWorld(playerid,playerid);
    ServerTips(playerid,"您已切换至自己的小世界");
    return 1;
}

forward WorldTime();
public WorldTime()
{
    if(GetSVarInt("World_Time") == 24)
    {
        SetSVarInt("World_Time",1);
        SetWorldTime(1);
        return 1;
    }
    SetSVarInt("World_Time",GetSVarInt("World_Time")+1);
    SetWorldTime(GetSVarInt("World_Time"));  
    return 1;
}

COMMAND:pweather(playerid,params[])
{
    new wid[18];
    if(sscanf(params,"s[18]",wid)) return ServerTips(playerid,"修改玩家个人天气 /pweather [天气ID]");
    SetPlayerWeather(playerid,strval(wid));
    ServerTips(playerid,"修改个人天气成功");
    return 1;
}

COMMAND:ptime(playerid,params[])
{
    new hour[18];
    if(sscanf(params,"s[18]",hour)) return ServerTips(playerid,"设置玩家个人时间 /ptime [时间(1~24)]");
    if(strval(hour) < 1 || strval(hour) > 24) return ServerTips(playerid,"修改的时间限制在1~24");
    SetPlayerTime(playerid,strval(hour),0);
    ServerTips(playerid,"修改个人时间成功");
    return 1;
}


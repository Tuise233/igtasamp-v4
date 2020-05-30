#define COLOR_ORANGE 0xFF8040C8
#define COLOR_GREEN 0x00FF00C8
#define COLOR_WHITE 0xFFFFFFC8
#define COLOR_BLUE 0x0080FFC8
#define COLOR_RED 0xFF0000C8
#define COLOR_RP 0x8000FFC8
#define COLOR_YELLOW 0xFFFF80C8

forward ServerTips(id,string[]);
public ServerTips(id,string[])
{
    new strings[1024];
    format(strings,sizeof(strings),"[服务器提示]%s",string);
    if(id == -1)
    {
        SendClientMessageToAll(0xFF80C0C8,strings);
        return 1;
    }
    else
    {
        SendClientMessage(id,0xFFFF80C8,strings);
        return 1;
    }
}

forward Score();
public Score()
{
    new files[64],name[64];
    for(new i=0;i<MAX_PLAYERS;i++)
    {
        if(IsPlayerConnected(i))
        {
            if(GetPVarInt(i,"is_login"))
            {
                GetPlayerName(i,name,sizeof(name));
                format(files,sizeof(files),"\\Players\\%s.ini",name);
                dini_IntSet(files,"Score",dini_Int(files,"Score")+1);
                SetPlayerScore(i,GetPlayerScore(i)+1);
            }
            if(GetPVarInt(i,"is_afking")){
                GetPlayerName(i,name,sizeof(name));
                format(files,sizeof(files),"\\Players\\%s.ini",name);
                dini_IntSet(files,"Score",dini_Int(files,"Score")+1);
                SetPlayerScore(i,GetPlayerScore(i)+1);
                ServerTips(i,"您处于服务器挂机区域内,已获得双倍时间分奖励");
            }
        }
    }
    return 1;
}

forward CheckServerInfo();
public CheckServerInfo()
{
    new files[64],command[64];
    format(files,sizeof(files),"\\Server\\ServerInfo.ini");
    format(command,sizeof(command),"hostname %s",dini_Get(files,"ServerName"));
    SendRconCommand(command);
    SetGameModeText(dini_Get(files,"GameMode"));
    return 1;
}

COMMAND:ppos(playerid)
{
    new Float:x,Float:y,Float:z;
    GetPlayerPos(playerid,x,y,z);
    printf("%f,%f,%f",x,y,z);
    return 1;
}
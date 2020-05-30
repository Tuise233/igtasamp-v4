COMMAND:skin(playerid,params[])
{
    new skinid[18],files[64],name[64];
    GetPlayerName(playerid,name,sizeof(name));
    if(sscanf(params,"s[18]",skinid)) return ServerTips(playerid,"修改皮肤 /skin [皮肤ID]");
    if(strval(skinid) < 0|| strval(skinid) >299) return ServerTips(playerid,"皮肤ID限制在0~299内");
    format(files,sizeof(files),"\\Players\\%s.ini",name);
    dini_IntSet(files,"Skin",strval(skinid));
    SetPlayerSkin(playerid,strval(skinid));
    ServerTips(playerid,"您已成功修改皮肤");
    return 1;
}

COMMAND:pm(playerid,params[])
{
    new pid[18],name[64],name1[64],string[256],content[256];
    new year,month,day,hour,mini,sec;
    new time[64];
    if(GetPVarInt(playerid,"is_cpm")) return ServerTips(playerid,"您已关闭私聊系统,无法发送私聊,输入/cpm即可开启私聊系统");
    getdate(year,month,day);
    gettime(hour,mini,sec);
    format(time,sizeof(time),"[%d/%d/%d %d:%d:%d]",year,month,day,hour,mini,sec);
    if(sscanf(params,"s[64]s[256]",pid,content)) return ServerTips(playerid,"私聊玩家 /skin [玩家ID] [内容]"); 
    if(!IsPlayerConnected(strval(pid))) return ServerTips(playerid,"该玩家不在线");
    if(GetPVarInt(strval(pid),"is_cpm")) return ServerTips(playerid,"该玩家已关闭私聊系统,您无法对他发送私聊");
    if(GetPVarInt(playerid,"is_rp") || GetPVarInt(strval(pid),"is_rp")) return ServerTips(playerid,"RP模式内无法使用私聊系统");
    if(!GetPVarInt(strval(pid),"is_admin"))
    {
        if(GetPVarInt(playerid,"is_mute")) return AdminTips(playerid,"您已被禁言,无法使用私聊系统");
    }
    GetPlayerName(playerid,name,sizeof(name));
    GetPlayerName(strval(pid),name1,sizeof(name1));
    format(string,sizeof(string),"%s%s(%d):%s_Private to %s(%d)\n",time,name,playerid,content,name1,strval(pid));
    WriteChatLog(string);
    format(string,sizeof(string),"玩家%s(%d)私聊对你说:%s",name,playerid,content);
    ServerTips(strval(pid),string);
    AdminTips(strval(pid),"输入/cpm可关闭PM系统");
    format(string,sizeof(string),"你私聊对玩家%s(%d)说:%s",name1,strval(pid),content);
    ServerTips(playerid,string);
    format(string,sizeof(string),"玩家%s(%d)私聊对玩家%s(%d)说:%s",name,playerid,name1,strval(pid),content);
    for(new i=0;i<MAX_PLAYERS;i++)
    {
        if(GetPVarInt(i,"is_admin"))
        {
            ServerTips(i,string);
        }
    }
    return 1;
}

COMMAND:cpm(playerid)
{
    if(!GetPVarInt(playerid,"is_cpm"))
    {
        SetPVarInt(playerid,"is_cpm",!GetPVarInt(playerid,"is_cpm"));
        ServerTips(playerid,"您已关闭私聊系统,重新输入/cpm即可重新开启");
    }
    else
    {
        DeletePVar(playerid,"is_cpm");
        ServerTips(playerid,"您已经开启私聊系统,重新输入/cpm即可重新关闭");
    }
    return 1;
}

COMMAND:ctp(playerid)
{
    if(!GetPVarInt(playerid,"is_ctp"))
    {
        SetPVarInt(playerid,"is_ctp",!GetPVarInt(playerid,"is_ctp"));
        ServerTips(playerid,"已开启您的CTP权限");
        return 1;
    }
    DeletePVar(playerid,"is_ctp");
    ServerTips(playerid,"已关闭您的CTP权限");
    return 1;
}

COMMAND:tp(playerid,params[])
{
    new name[64],name1[64],string[256],pid[18];
    new Float:x,Float:y,Float:z;
    if(sscanf(params,"s[18]",pid)) return ServerTips(playerid,"传送到玩家附近: /tp [玩家ID]");
    if(!GetPVarInt(playerid,"is_ctp")) return ServerTips(playerid,"您未开启CTP权限");
    if(!IsPlayerConnected(strval(pid))) return ServerTips(playerid,"该玩家不在线");
    if(!GetPVarInt(strval(pid),"is_ctp")) return ServerTips(playerid,"该玩家未开启CTP权限");
    GetPlayerPos(strval(pid),x,y,z);
    SetPlayerPos(playerid,x,y,z);
    GetPlayerName(playerid,name,sizeof(name));
    GetPlayerName(strval(pid),name1,sizeof(name1));
    format(string,sizeof(string),"玩家%s(%d)传送至你的身边",name,playerid);
    ServerTips(strval(pid),string);
    format(string,sizeof(string),"你传送至玩家%s(%d)的身边",name1,strval(pid));
    ServerTips(playerid,string);
    return 1;
}

COMMAND:tv(playerid,params[])
{
    new params1[18],name[64],string[256];
    if(sscanf(params,"s[18]",params1)) return ServerTips(playerid,"观察功能使用指南 /tv help");

    if(strval(params1))
    {
        if(!IsPlayerConnected(strval(params1))) return ServerTips(playerid,"该玩家不在线");
        GetPlayerName(strval(params),name,sizeof(name));
        format(string,sizeof(string),"您已开始观察玩家%s(%d)",name,strval(params1));
        ServerTips(playerid,string);
        TogglePlayerSpectating(playerid, 1);
        SetPVarInt(playerid,"is_tv",strval(params1)+1);
        SetPlayerVirtualWorld(playerid,GetPlayerVirtualWorld(strval(params1)));
        if(IsPlayerInAnyVehicle(strval(params1)))
        {
            PlayerSpectateVehicle(playerid, GetPlayerVehicleID(strval(params1)));
            return 1;
        }
        PlayerSpectatePlayer(playerid, strval(params1),SPECTATE_MODE_NORMAL);
        return 1;
    }

    if(!strcmp(params1,"leave",true))
    {
        DeletePVar(playerid,"is_tv");
        SetPlayerVirtualWorld(playerid,0);
        TogglePlayerSpectating(playerid,0);
        ServerTips(playerid,"您已退出观察");
        return 1;
    }

    if(!strcmp(params1,"help",true))
    {
        ServerTips(playerid,"观察功能使用指南");
        ServerTips(playerid,"观察玩家 /tv [玩家ID] || 退出观察 /tv leave");
        return 1;
    }
    return 1;
}

forward AutoMessage();
public AutoMessage()
{
    new files[64],string[1024],num[18];
    new a=0;
    format(files,sizeof(files),"\\Server\\AutoMessage.ini");
    for(new i=0;i<50;i++)
    {
        format(num,sizeof(num),"Msg[%d]",i);
        if(dini_Isset(files,num))
        {
            a = a+1;
        }
    }
    format(num,sizeof(num),"Msg[%d]",random(a));
    format(string,sizeof(string),"[服务器公告]%s",dini_Get(files,num));
    SendClientMessageToAll(COLOR_GREEN,string);
    return 1;
}

COMMAND:kill(playerid)
{
    SetPlayerHealth(playerid, 0);
    ServerTips(playerid,"您选择了自杀...");
    return 1;   
}

COMMAND:skick(playerid,params[])
{
    if(!GetPVarInt(playerid,"is_login")) return ServerTips(playerid,"您还没有登录,无法进行任何操作");
    new pid[18],reason[64],name[64],name1[64],string[256];
    if(sscanf(params,"s[18]s[64]",pid,reason)) return ServerTips(playerid,"投票踢人 /skick [玩家ID] [原因]");
    for(new i=0;i<MAX_PLAYERS;i++){
        if(GetPVarInt(i,"is_skick_count")){
            ServerTips(playerid,"目前已有一项投票踢人正在进行,请稍后重试");
            return 1;
        }
    }
    if(!IsPlayerConnected(strval(pid))) return ServerTips(playerid,"该玩家不在线");
    GetPlayerName(playerid,name,sizeof(name));
    GetPlayerName(strval(pid),name1,sizeof(name1));
    if(GetPVarInt(strval(pid),"is_admin"))
    {
        format(string,sizeof(string),"[投票踢人]玩家%s(%d)尝试踢出管理员%s(%d),已被系统踢出服务器",name,playerid,name1,strval(pid));
        CheatTips(-1,string);
        Kick(playerid);
        return 1;
    }
    format(string,sizeof(string),"[投票踢人]玩家%s(%d)申请踢出玩家%s(%d),原因:%s",name,playerid,name1,strval(pid),reason);
    SendClientMessageToAll(COLOR_RED,string);
    SendClientMessageToAll(COLOR_RED,"[投票踢人]输入 /ty 同意踢出该玩家,输入 /bty 不同意踢出该玩家");
    SetPVarInt(playerid,"is_skick",123);
    SetPVarInt(strval(pid),"is_skick",!GetPVarInt(playerid,"is_skick"));
    SetPVarInt(strval(pid),"is_skick_count",30);
    SetPVarInt(strval(pid),"is_skick_ty",1);
    SetPVarInt(strval(pid),"is_skick_bty",1);
    return 1;
}

COMMAND:ty(playerid)
{
    if(!GetPVarInt(playerid,"is_login")) return ServerTips(playerid,"您还没有登录,无法进行任何操作");
    new a = 0,pid = 0;
    new name[64],name1[64],string[256];
    if(GetPVarInt(playerid,"is_skick")) return ServerTips(playerid,"您已经参与过投票,无法再次投票");
    for(new i=0;i<MAX_PLAYERS;i++){
        if(GetPVarInt(i,"is_skick_count")){
            a = a + 1;
            pid = i;
        }
    }
    if(a == 0) return ServerTips(playerid,"当前没有玩家正在进行投票踢人");
    else{
        GetPlayerName(playerid,name,sizeof(name));
        GetPlayerName(pid,name1,sizeof(name1));
        format(string,sizeof(string),"[投票踢人]玩家%s(%d)同意踢出%s(%d)",name,playerid,name1,pid);
        SendClientMessageToAll(COLOR_RED,string);
        SetPVarInt(playerid,"is_skick",!GetPVarInt(playerid,"is_skick"));
        SetPVarInt(pid,"is_skick_ty",GetPVarInt(pid,"is_skick_ty")+1);
        return 1;
    }
}

COMMAND:bty(playerid)
{
    if(!GetPVarInt(playerid,"is_login")) return ServerTips(playerid,"您还没有登录,无法进行任何操作");
    new a = 0,pid = 0;
    new name[64],name1[64],string[256];
    if(GetPVarInt(playerid,"is_skick")) return ServerTips(playerid,"您已经参与过投票,无法再次投票");
    for(new i=0;i<MAX_PLAYERS;i++){
        if(GetPVarInt(i,"is_skick_count")){
            a = a + 1;
            pid = i;
        }
    }
    if(a == 0) return ServerTips(playerid,"当前没有玩家正在进行投票踢人");
    else{
        GetPlayerName(playerid,name,sizeof(name));
        GetPlayerName(pid,name1,sizeof(name1));
        format(string,sizeof(string),"[投票踢人]玩家%s(%d)不同意踢出%s(%d)",name,playerid,name1,pid);
        SendClientMessageToAll(COLOR_RED,string);
        SetPVarInt(playerid,"is_skick",!GetPVarInt(playerid,"is_skick"));
        SetPVarInt(pid,"is_skick_bty",GetPVarInt(pid,"is_skick_bty")+1);
        return 1;
    }
}

forward SkickCheck();
public SkickCheck()
{
    new string[256],name[64];
    for(new i=0;i<MAX_PLAYERS;i++){
        if(GetPVarInt(i,"is_skick_count")){
            if(GetPVarInt(i,"is_skick_count") == 1){
                if(GetPVarInt(i,"is_skick_ty") > GetPVarInt(i,"is_skick_bty")){
                    GetPlayerName(i,name,sizeof(name));
                    format(string,sizeof(string),"[投票踢人]玩家%s(%d)被踢出服务器,投票结果 - 同意%d票 || 不同意%d票",name,i,GetPVarInt(i,"is_skick_ty"),GetPVarInt(i,"is_skick_bty"));
                    SendClientMessageToAll(COLOR_RED,string);
                    Kick(i);
                    for(new a=0;a<MAX_PLAYERS;a++){
                        if(GetPVarInt(a,"is_skick") || GetPVarInt(a,"is_skick_count") || GetPVarInt(i,"is_skick_ty") || GetPVarInt(i,"is_skick_bty")){
                            DeletePVar(a,"is_skick");
                            DeletePVar(a,"is_skick_count");
                            DeletePVar(a,"is_skick_ty");
                            DeletePVar(a,"is_skick_bty");
                        }
                    }
                    return 1;
                }
                if(GetPVarInt(i,"is_skick_ty") <= GetPVarInt(i,"is_skick_bty")){
                    GetPlayerName(i,name,sizeof(name));
                    format(string,sizeof(string),"[投票踢人]玩家%s(%d)未被踢出服务器,投票结果 - 同意%d票 || 不同意%d票",name,i,GetPVarInt(i,"is_skick_ty"),GetPVarInt(i,"is_skick_bty"));
                    SendClientMessageToAll(COLOR_RED,string);
                    for(new a=0;a<MAX_PLAYERS;a++){
                    if(GetPVarInt(a,"is_skick") || GetPVarInt(a,"is_skick_count") || GetPVarInt(i,"is_skick_ty") || GetPVarInt(i,"is_skick_bty")){
                            DeletePVar(a,"is_skick");
                            DeletePVar(a,"is_skick_count");
                            DeletePVar(a,"is_skick_ty");
                            DeletePVar(a,"is_skick_bty");
                        }
                    }
                }
                return 1;
            }
            else{
                SetPVarInt(i,"is_skick_count",GetPVarInt(i,"is_skick_count") - 1);
            }
        }
    }
    return 1;
}

COMMAND:mtp(playerid)
{
    if(GetPVarInt(playerid,"is_mtp")){
        DeletePVar(playerid,"is_mtp");
        ServerTips(playerid,"已关闭地图传送功能");
        return 1;
    }
    SetPVarInt(playerid,"is_mtp",!GetPVarInt(playerid,"is_mtp"));
    ServerTips(playerid,"已开启地图传送功能");
    return 1;
}

COMMAND:wudi(playerid)
{
    if(GetPVarInt(playerid,"is_rp")) return RpTips(playerid,"您正在RP模式中,无法开启无敌");
    if(GetPVarInt(playerid,"is_god")){
        DeletePVar(playerid,"is_god");
        ServerTips(playerid,"您已关闭无敌模式");
        SetPlayerHealth(playerid,100);
        SetPlayerArmour(playerid,100);
        return 1;
    }
    SetPVarInt(playerid,"is_god",99);
    ServerTips(playerid,"您已开启无敌模式");
    return 1;
}

forward God();
public God()
{
    for(new i=0;i<MAX_PLAYERS;i++){
        if(GetPVarInt(i,"is_god")){
            SetPlayerHealth(i,10000);
            SetPlayerArmour(i,10000);
        }
    }
    return 1;
}

COMMAND:qian(playerid)
{
    new Float:x,Float:y,Float:z;
    GetPlayerPos(playerid,x,y,z);
    SetPlayerPos(playerid,x+3,y,z);
    ServerTips(playerid,"已调整您的位置");
    return 1;
}

COMMAND:hou(playerid)
{
    new Float:x,Float:y,Float:z;
    GetPlayerPos(playerid,x,y,z);
    SetPlayerPos(playerid,x-3,y,z);
    ServerTips(playerid,"已调整您的位置");
    return 1;
}

COMMAND:zuo(playerid)
{
    new Float:x,Float:y,Float:z;
    GetPlayerPos(playerid,x,y,z);
    SetPlayerPos(playerid,x,y+3,z);
    ServerTips(playerid,"已调整您的位置");
    return 1;
}

COMMAND:you(playerid)
{
    new Float:x,Float:y,Float:z;
    GetPlayerPos(playerid,x,y,z);
    SetPlayerPos(playerid,x,y-3,z);
    ServerTips(playerid,"已调整您的位置");
    return 1;
}

COMMAND:shang(playerid)
{
    new Float:x,Float:y,Float:z;
    GetPlayerPos(playerid,x,y,z);
    SetPlayerPos(playerid,x,y,z+3);
    ServerTips(playerid,"已调整您的位置");
    return 1;
}

COMMAND:xia(playerid)
{
    new Float:x,Float:y,Float:z;
    GetPlayerPos(playerid,x,y,z);
    SetPlayerPos(playerid,x,y,z-3);
    ServerTips(playerid,"已调整您的位置");
    return 1;
}

COMMAND:point(playerid)
{
    new Float:x,Float:y,Float:z;
    GetPlayerPos(playerid,x,y,z);
    SetPlayerCheckpoint(playerid,x,y,z,20.0);
    return 1;
}

forward Afking();
public Afking()
{
    //128.362945,-1931.474975,-0.354136
    //185.009384,-1970.584350,-0.616092
    new Float:x,Float:y,Float:z;
    for(new i=0;i<MAX_PLAYERS;i++){
        GetPlayerPos(i,x,y,z);
        if(x > 128.362945 && x < 185.009384 && y < -1931.474975 && y > -1970.584350){
            if(!GetPVarInt(i,"is_afking")){
                SetPVarInt(i,"is_afking",!GetPVarInt(i,"is_afking"));
                ServerTips(i,"您正处于服务器挂机区,您将获得双倍的时间分");
            }
        }
        else{
            if(GetPVarInt(i,"is_afking")){
                DeletePVar(i,"is_afking");
                ServerTips(i,"您已离开服务器挂机区,您将不会获得双倍的时间分");
            }
        }
    }
    return 1;
}

COMMAND:logs(playerid)
{
    new num[64],files[64],string[256];
    format(files,sizeof(files),"\\Server\\AutoMessage.ini");
    for(new i=0;i<99;i++){
        format(num,sizeof(num),"Log[%d]",i);
        if(dini_Isset(files,num)){
            format(string,sizeof(string),"%s",dini_Get(files,num));
            ServerTips(playerid,string);
        }
    }
    return 1;
}

COMMAND:qd(playerid)
{
    new name[64],files[64],string[256];
    GetPlayerName(playerid,name,sizeof(name));
    format(files,sizeof(files),"\\Players\\%s.ini",name);
    new year,month,day;
    getdate(year,month,day);
    if(dini_Int(files,"Sign") == day){
        ServerTips(playerid,"您今天已经签过到,无法重新签到");
        return 1;
    }
    dini_IntSet(files,"Sign",day);
    format(string,sizeof(string),"玩家%s(%d)进行了签到,奖励10时间分",name,playerid);
    ServerTips(-1,string);
    SetPlayerScore(playerid,GetPlayerScore(playerid)+10);
    return 1;
}

COMMAND:stopmusic(playerid)
{
    StopAudioStreamForPlayer(playerid);
    SendClientMessage(playerid,COLOR_GREEN,"[系统]已为您关闭音乐");
    return 1;
}

stock GetName(playerid)
{
    new name[64];
    GetPlayerName(playerid,name,sizeof(name));
    return name;
}
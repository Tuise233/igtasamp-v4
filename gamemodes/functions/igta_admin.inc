forward AdminTips(playerid,params[]);
public AdminTips(playerid,params[])
{
    new string[256];
    format(string,sizeof(string),"[T-Admin]%s",params);
    if(playerid == -1){
        SendClientMessageToAll(COLOR_RED,string);
        return 1;
    }
    else{
        SendClientMessage(playerid,COLOR_RED,string);
    }
    return 1;
}

forward IsAdmin(playerid,level);
public IsAdmin(playerid,level)
{
    if(IsPlayerAdmin(playerid)) return true;
    if(!GetPVarInt(playerid,"is_admin")){
        AdminTips(playerid,"您不是服务器管理员");
        return false;
    }
    if(GetPVarInt(playerid,"is_admin") < level){
        AdminTips(playerid,"您的权限不足");
        return false;
    }
    if(GetPVarInt(playerid,"is_admin") >= level) return true;
    return false;
}

COMMAND:setadmin(playerid,params[])
{
    new pid[18],name[64],name1[64],string[256],files[64],level[18];
    if(sscanf(params,"s[18]s[18]",pid,level)) return AdminTips(playerid,"设置玩家管理员权限: /setadmin [玩家ID] [等级]");
    if(!IsPlayerConnected(strval(pid))) return AdminTips(playerid,"该玩家不在线");
    GetPlayerName(playerid,name,sizeof(name));
    GetPlayerName(strval(pid),name1,sizeof(name1));
    if(IsAdmin(playerid,5)){
        format(files,sizeof(files),"\\Admins\\%s.ini",name1);
        if(!fexist(files)) return AdminTips(playerid,"该玩家未注册管理员账号");
        if(dini_Int(files,"Level") == 0){
            format(string,sizeof(string),"高层%s(%d)将玩家%s(%d)设置为%d级管理员",name,playerid,name1,strval(pid),strval(level));
        }
        else{
            if(dini_Int(files,"Level") < strval(level)){
                format(string,sizeof(string),"高层%s(%d)将%d级管理员%s(%d)等级提升至%d级",name,playerid,dini_Int(files,"Level"),name1,strval(pid),strval(level));
            }
            if(dini_Int(files,"Level") >= strval(level)){
                format(string,sizeof(string),"高层%s(%d)将%d级管理员%s(%d)等级降级至%d级",name,playerid,dini_Int(files,"Level"),name1,strval(pid),strval(level));
            }
        } 
        AdminTips(-1,string);
        dini_IntSet(files,"Level",strval(level));
    }
    return 1;
}

COMMAND:aregister(playerid,params[])
{
    new password[128],name[64],string[256],files[64];
    if(sscanf(params,"s[128]",password)) return AdminTips(playerid,"注册管理员账号: /aregister [密码]");
    GetPlayerName(playerid,name,sizeof(name));
    format(files,sizeof(files),"\\Admins\\%s.ini",name);
    if(fexist(files)) return AdminTips(playerid,"您已注册过管理员账号,无需重新注册");
    else{
        dini_Create(files);
        dini_Set(files,"Name",name);
        dini_Set(files,"Password",password);
        dini_IntSet(files,"Level",0);
        format(string,sizeof(string),"成功注册管理员账号,请联系高层管理员上权限");
        AdminTips(playerid,string);
    }
    return 1;
}

COMMAND:alogin(playerid,params[])
{
    new name[64],password[128],string[256],files[64];
    new year,month,day,hour,mini,sec;
    gettime(hour,mini,sec);
    getdate(year,month,day);
    if(GetPVarInt(playerid,"is_admin")) return AdminTips(playerid,"您正在执勤状态,无需再次登陆,输入/logout退出执勤状态");
    GetPlayerName(playerid,name,sizeof(name));
    format(files,sizeof(files),"\\Admins\\%s.ini",name);
    if(sscanf(params,"s[128]",password)) return AdminTips(playerid,"进入执勤状态: /alogin [密码]");
    if(!strcmp(password,dini_Get(files,"password"))){
        if(!fexist(files)) return AdminTips(playerid,"您不是管理员,无法进行执勤");
        else{
            if(dini_Int(files,"Level") == 0) return AdminTips(playerid,"您的管理员账号没有权限,请联系高层管理员上权限后进行执勤");
            SetPVarInt(playerid,"is_admin",dini_Int(files,"Level"));
            SetPlayerColor(playerid,COLOR_RED);
            format(string,sizeof(string),"管理员%s(%d)进入执勤状态,管理权限等级:%d级",name,playerid,dini_Int(files,"Level"));
            AdminTips(-1,string);
            new times[64];
            format(times,sizeof(times),"%d.%d.%d %d:%d:%d",year,month,day,hour,mini,sec);
            dini_Set(files,"Time",times);
        }
    }
    else{
        AdminTips(playerid,"您输入的密码有误,请核对后重新输入");
    }
    return 1;
}

COMMAND:logout(playerid)
{
    if(!GetPVarInt(playerid,"is_admin")) return AdminTips(playerid,"您不在执勤状态,无需退出执勤");
    DeletePVar(playerid,"is_admin");
    new colors[6] = {0xFF80FFC8,0xFFFF80C8,0x00FF80C8,0x008000C8,0xFFFF80C8,0xFF8040C8};
    SetPlayerColor(playerid,colors[random(6)]);
    AdminTips(playerid,"您已退出执勤状态");
    return 1;
}

//通用
COMMAND:ip(playerid,params[])
{
    new pid[18],name[64],string[256];
    if(sscanf(params,"s[18]",pid)) return AdminTips(playerid,"查询玩家IP:/ip [玩家ID]");
    if(IsAdmin(playerid,1))
    {
        new ip[64];
        if(IsPlayerConnected(strval(pid)))
        {
            GetPlayerIp(strval(pid),ip,sizeof(ip));
            GetPlayerName(strval(pid),name,sizeof(name));
            format(string,sizeof(string),"玩家%s(%d)的IP地址:%s",name,strval(pid),ip);
            AdminTips(playerid,string);
        }
        else
        {
            AdminTips(playerid,"该玩家不在线");
        }
    }
    return 1;
}

stock fwrite_utf8(files[],string[])
{
    new File:fl = fopen(files,io_write);
    if(fl)
    {
        for(new i=0;string[i];i++)
        {
            fputchar(fl,string[i],false);
        }
        fclose(fl);
    }
}

COMMAND:jubao(playerid,params[])
{
    new pid[18],name[64],name1[64],string[256],reason[128];
    if(sscanf(params,"s[18]s[64]",pid,reason)) return AdminTips(playerid,"举报玩家:/jubao [玩家ID] [原因]");
    if(!IsPlayerConnected(strval(pid))) return AdminTips(playerid,"该玩家不在线");
    GetPlayerName(playerid,name,sizeof(name));
    GetPlayerName(strval(pid),name1,sizeof(name1));
    format(string,sizeof(string),"玩家%s(%d)举报玩家%s(%d),原因:%s",name,playerid,name1,strval(pid),reason);
    for(new i=0;i<MAX_PLAYERS;i++){
        if(GetPVarInt(i,"is_admin")){
            AdminTips(i,string);
        }
    }
    AdminTips(playerid,"您的消息已发送给在线管理员,等待管理员处理!");
    format(string,sizeof(string),"IGTASAMP - 举报通知\n举报人:%s(%d)\n被举报人:%s(%d)\n原因:%s\n\n请管理员尽快上线处理!",name,playerid,name1,strval(pid),reason);
    new files[64];
    format(files,sizeof(files),"\\Robot\\Report.txt");
    fwrite_utf8(files,string);
    return 1;
}

COMMAND:gms(playerid)
{
    new a=0;
    new name[64],string[256];
    AdminTips(playerid,"当前在线管理员");
    for(new i=0;i<MAX_PLAYERS;i++){
        if(GetPVarInt(i,"is_admin")){
            GetPlayerName(i,name,sizeof(name));
            format(string,sizeof(string),"%s(%d) - 权限等级:%d级",name,i,GetPVarInt(i,"is_admin"));
            AdminTips(playerid,string);
            a += 1;
        }
    }
    if(a == 0){
        AdminTips(playerid,"当前没有在线管理员");
    }
    return 1;
}

COMMAND:ac(playerid,params[])
{
    new content[256],string[256],name[64];
    if(sscanf(params,"s[256]",content)) return AdminTips(playerid,"管理员聊天频道:/ac [内容]");
    if(IsAdmin(playerid,1)){
        GetPlayerName(playerid,name,sizeof(name));
        format(string,sizeof(string),"%d级管理员%s(%d):%s",GetPVarInt(playerid,"is_admin"),name,playerid,content);
        for(new i=0;i<MAX_PLAYERS;i++){
            if(GetPVarInt(i,"is_admin")){
                AdminTips(i,string);
            }
        }
    }
    return 1;
}

COMMAND:goto(playerid,params[])
{
    new pid[18];
    if(IsAdmin(playerid,1))
    {
        new Float:x,Float:y,Float:z;
        if(sscanf(params,"s[18]",pid)) return AdminTips(playerid,"传送到玩家附近");
        if(!IsPlayerConnected(strval(pid))) return AdminTips(playerid,"该玩家不在线");
        GetPlayerPos(strval(pid),x,y,z);
        SetPlayerPos(playerid,x+5,y+5,z);
        return 1;
    }
    return 1;
}

COMMAND:get(playerid,params[])
{
    new pid[18];
    if(IsAdmin(playerid,1))
    {
        new Float:x,Float:y,Float:z;
        if(sscanf(params,"s[18]",pid)) return AdminTips(playerid,"将玩家传送到你附近");
        if(!IsPlayerConnected(strval(pid))) return AdminTips(playerid,"该玩家不在线");
        GetPlayerPos(playerid,x,y,z);
        SetPlayerPos(strval(pid),x+5,y+5,z);
        return 1;
    }
    return 1;
}

//1级
COMMAND:warn(playerid,params[])
{
    new name[64],name1[64],reason[128],string[256],pid[18];
    new Float:x,Float:y,Float:z;
    if(sscanf(params,"s[18]s[128]",pid,reason)) return AdminTips(playerid,"警告玩家: /warn [玩家ID] [原因]");
    if(IsAdmin(playerid,1)){
        if(!IsPlayerConnected(strval(pid))) return AdminTips(playerid,"该玩家不在线");
        GetPlayerName(playerid,name,sizeof(name));
        GetPlayerName(strval(pid),name1,sizeof(name1));
        format(string,sizeof(string),"管理员%s(%d)警告玩家%s(%d),原因:%s",name,playerid,name1,strval(pid),reason);
        AdminTips(-1,string);
        GetPlayerPos(strval(pid),x,y,z);
        SetPlayerPos(strval(pid),x,y,z+5);
    }
    return 1;
}

COMMAND:mute(playerid,params[])
{
    new name[64],name1[64],pid[18],string[256];
    if(sscanf(params,"s[18]",pid)) return AdminTips(playerid,"禁言玩家: /mute [玩家ID]");
    if(IsAdmin(playerid,1)){
        if(!IsPlayerConnected(strval(pid))) return AdminTips(playerid,"该玩家不在线");
        GetPlayerName(playerid,name,sizeof(name));
        GetPlayerName(strval(pid),name1,sizeof(name1));
        format(string,sizeof(string),"管理员%s(%d)禁言玩家%s(%d)",name,playerid,name1,strval(pid));
        AdminTips(-1,string);
        SetPVarInt(strval(pid),"is_mute",!GetPVarInt(strval(pid),"is_mute"));
    }
    return 1;
}

COMMAND:unmute(playerid,params[])
{
    new name[64],name1[64],string[256],pid[18];
    if(sscanf(params,"s[18]",pid)) return AdminTips(playerid,"解禁玩家: /unmute [玩家ID]");
    if(IsAdmin(playerid,1)){
        if(!IsPlayerConnected(strval(pid))) return AdminTips(playerid,"该玩家不在线");
        GetPlayerName(playerid,name,sizeof(name));
        GetPlayerName(strval(pid),name1,sizeof(name1));
        if(!GetPVarInt(strval(pid),"is_mute")) return AdminTips(playerid,"该玩家未被禁言");
        DeletePVar(strval(pid),"is_mute");
        format(string,sizeof(string),"管理员%s(%d)解禁言玩家%s(%d)",name,playerid,name1,strval(pid));
        AdminTips(-1,string);
    }
    return 1;
}

COMMAND:jail(playerid,params[])
{
    new name[64],name1[64],string[256],pid[18],sec[18];
    if(sscanf(params,"s[18]s[18]",pid,sec)) return AdminTips(playerid,"监禁玩家: /jail [玩家ID] [秒数]");
    if(IsAdmin(playerid,1)){
        if(!IsPlayerConnected(strval(pid))) return AdminTips(playerid,"该玩家不在线");
        GetPlayerName(playerid,name,sizeof(name));
        GetPlayerName(strval(pid),name1,sizeof(name1));
        format(string,sizeof(string),"管理员%s(%d)监禁玩家%s(%d)%d秒",name,playerid,name1,strval(pid),strval(sec));
        AdminTips(-1,string);
        if(IsPlayerInAnyVehicle(strval(pid))){
            RemovePlayerFromVehicle(strval(pid));
        }
        SetPVarInt(strval(pid),"is_jail",strval(sec));
    }
    return 1;
}

forward Jail();
public Jail()
{
    new Float:x,Float:y,Float:z;
    new string[256];
    for(new i=0;i<MAX_PLAYERS;i++){
        if(GetPVarInt(i,"is_jail"))
        {
            if(GetPVarInt(i,"is_jail") == 1){
                DeletePVar(i,"is_jail");
                AdminTips(i,"刑期已满,请好好做人!");
                SetPlayerHealth(i,0);
                return 1;
            }
            SetPVarInt(i,"is_jail",GetPVarInt(i,"is_jail") - 1);
            GetPlayerPos(i,x,y,z);
            if(x < -2189.426757 || x > -2174.765625 || y < -269.240661 || y > -256.060913 || z < 34.515625 || y > 38.515625){
                SetPlayerPos(i,-2182.973632,-263.378509,36.515625);
                format(string,sizeof(string),"您还剩%d后出狱",GetPVarInt(i,"is_jail"));
                AdminTips(i,string);
            }
        }
    }
    return 1;
}

COMMAND:moshou(playerid,params[])
{
    new pid[18],name[64],name1[64],string[256];
    if(sscanf(params,"s[18]",pid)) return AdminTips(playerid,"没收玩家武器:/moshou [玩家ID]");
    if(IsAdmin(playerid,1)){
        if(!IsPlayerConnected(strval(pid))) return AdminTips(playerid,"该玩家不在线");
        GetPlayerName(playerid,name,sizeof(name));
        GetPlayerName(strval(pid),name1,sizeof(name1));
        format(string,sizeof(string),"管理员%s(%d)没收了玩家%s(%d)的武器",name,playerid,name1,strval(pid));
        AdminTips(-1,string);
        ResetPlayerWeapons(strval(pid));
    }
    return 1;
}

COMMAND:clear(playerid)
{
    if(IsAdmin(playerid,1)){
        for(new i=0;i<20;i++){
            AdminTips(-1,"");
        }
        new name[64],string[256];
        GetPlayerName(playerid,name,sizeof(name));
        format(string,sizeof(string),"管理员%s(%d)清空了公屏聊天记录",name,playerid);
        AdminTips(-1,string);
    }
    return 1;
}

COMMAND:delc(playerid,params[])
{
    new name[64],name1[64],string[256],pid[18];
    if(IsAdmin(playerid,1))
    {
        if(sscanf(params,"s[18]",pid)) return AdminTips(playerid,"删除玩家载具: /delc [玩家ID]");
        if(!IsPlayerConnected(strval(pid))) return AdminTips(playerid,"该玩家不在线");
        if(PlayerVehicle[strval(pid)] == -1) return AdminTips(playerid,"该玩家没有生成任何载具");
        DestroyVehicle(PlayerVehicle[strval(pid)]);
        PlayerVehicle[strval(pid)] = -1;
        GetPlayerName(playerid,name,sizeof(name));
        GetPlayerName(strval(pid),name1,sizeof(name1));
        format(string,sizeof(string),"管理员%s(%d)删除了玩家%s(%d)生成的载具",name,playerid,name1,strval(pid));
        AdminTips(-1,string);
        return 1;
    }
    return 1;
}

COMMAND:racefix(playerid,params[])
{
    if(IsAdmin(playerid,1))
    {
        SendRconCommand("reloadfs race");
        SendClientMessage(playerid,COLOR_RED,"[T-Admin]已修复赛道系统");
    }
    return 1;
}

forward KickEx();
public KickEx()
{
    for(new i=0;i<MAX_PLAYERS;i++)
    {
        if(GetPVarInt(i,"is_kick_ex"))
        {
            if(GetPVarInt(i,"is_kick_ex") == 1)
            {
                Kick(i);
            }
            else
            {
                SetPVarInt(i,"is_kick_ex",GetPVarInt(i,"is_kick_ex") - 1);
            }
        }
    }
    return 1;
}

//2级
COMMAND:kick(playerid,params[])
{
    new name[64],name1[64],string[256],pid[18],reason[128];
    if(sscanf(params,"s[18]s[128]",pid,reason)) AdminTips(playerid,"踢出玩家: /kick [玩家ID] [原因]");
    if(IsAdmin(playerid,2)){
        if(!IsPlayerConnected(strval(pid))) return AdminTips(playerid,"该玩家不在线");
        GetPlayerName(playerid,name,sizeof(name));
        GetPlayerName(strval(pid),name1,sizeof(name1));
        format(string,sizeof(string),"管理员%s(%d)将玩家%s(%d)踢出服务器,原因:%s",name,playerid,name1,strval(pid),reason);
        AdminTips(-1,string);
        SetPVarInt(strval(pid),"is_kick_ex",5);
    }
    return 1;
}

COMMAND:say(playerid,params[])
{
    new name[64],content[256],string[256];
    if(sscanf(params,"s[256]",content)) return AdminTips(playerid,"发送服务器公告:/say [内容]");
    if(IsAdmin(playerid,2)){
        GetPlayerName(playerid,name,sizeof(name));
        format(string,sizeof(string),"管理员%s(%d)发布公告:%s",name,playerid,content);
        for(new i=0;i<3;i++){
            AdminTips(-1,string);
        }
    }
    return 1;
}

COMMAND:freeze(playerid,params[])
{
    new pid[18],name[64],name1[64],string[256];
    if(sscanf(params,"s[18]",pid)) return AdminTips(playerid,"禁止玩家行动:/freeze [玩家ID]");
    if(IsAdmin(playerid,2)){
        if(!IsPlayerConnected(strval(pid))) return AdminTips(playerid,"该玩家不在线");
        GetPlayerName(playerid,name,sizeof(name));
        GetPlayerName(strval(pid),name1,sizeof(name1));
        if(!GetPVarInt(strval(pid),"is_freezing")){
            format(string,sizeof(string),"管理员%s(%d)禁止玩家%s(%d)进行活动",name,playerid,name1,strval(pid));
            AdminTips(-1,string);
            TogglePlayerControllable(strval(pid),0);
            AdminTips(playerid,"您已冻结一名玩家,再次输入以解冻玩家");
            SetPVarInt(strval(pid),"is_freezing",!GetPVarInt(strval(pid),"is_freezing"));
            return 1;
        }
        if(GetPVarInt(strval(pid),"is_freezing")){
            format(string,sizeof(string),"管理员%s(%d)允许玩家%s(%d)进行活动",name,playerid,name1,strval(pid));
            TogglePlayerControllable(strval(pid),1);
            AdminTips(-1,string);
        }
    }
    return 1;
}

COMMAND:nocar(playerid,params[])
{
    new pid[18],name[64],name1[64],string[256];
    if(sscanf(params,"s[18]",pid)) return AdminTips(playerid,"禁止玩家进入载具:/nocar [玩家ID]");
    if(IsAdmin(playerid,2)){
        if(!IsPlayerConnected(strval(pid))) return AdminTips(playerid,"该玩家不在线");
        GetPlayerName(playerid,name,sizeof(name));
        GetPlayerName(strval(pid),name1,sizeof(name1));
        if(!GetPVarInt(strval(pid),"is_nocar")){
            SetPVarInt(strval(pid),"is_nocar",!GetPVarInt(strval(pid),"is_nocar"));
            format(string,sizeof(string),"管理员%s(%d)禁止玩家%s(%d)进入载具",name,playerid,name1,strval(pid));
            AdminTips(-1,string);
            return 1;
        }
        if(GetPVarInt(strval(pid),"is_nocar")){
            DeletePVar(strval(pid),"is_nocar");
            format(string,sizeof(string),"管理员%s(%d)允许玩家%s(%d)进入载具",name,playerid,name1,strval(pid));
            AdminTips(-1,string);
        }
    }
    return 1;
}

//3级
COMMAND:setname(playerid,params[])
{
    new pid[18],name[64],name1[64],nname[64],string[256];
    if(IsAdmin(playerid,3)){
        if(sscanf(params,"s[18]s[64]",pid,nname)) return AdminTips(playerid,"修改玩家名字:/setname [玩家ID] [新名字]");
        GetPlayerName(playerid,name,sizeof(name));
        GetPlayerName(strval(pid),name1,sizeof(name1));
        SetPlayerName(strval(pid),nname);
        format(string,sizeof(string),"管理员%s(%d)j将玩家%s(%d)的名字设置为:%s",name,playerid,name1,strval(pid),nname);
        AdminTips(-1,string);
    }
    return 1;
}

COMMAND:jiangli(playerid,params[])
{
    new pid[18],name[64],name1[64],string[256];
    if(IsAdmin(playerid,3)){
        if(sscanf(params,"s[18]",pid)) return AdminTips(playerid,"奖励玩家10时间分:/jiangli [玩家ID]");
        GetPlayerName(playerid,name,sizeof(name));
        GetPlayerName(strval(pid),name1,sizeof(name1));
        format(string,sizeof(string),"管理员%s(%d)奖励玩家%s(%d)10时间分",name,playerid,name1,strval(pid));
        AdminTips(-1,string);
        SetPlayerScore(strval(pid),GetPlayerScore(strval(pid))+10);        
    }
    return 1;
}

COMMAND:ban(playerid,params[])
{
    new pid[18],name[64],name1[64],string[256],reason[128];
    if(IsAdmin(playerid,3)){
        if(sscanf(params,"s[18]s[128]",pid,reason)) return AdminTips(playerid,"封禁玩家:/ban [玩家ID]");
        GetPlayerName(playerid,name,sizeof(name));
        GetPlayerName(strval(pid),name1,sizeof(name1));
        format(string,sizeof(string),"管理员%s(%d)永久封禁玩家%s(%d),原因:%s",name,playerid,name1,strval(pid),reason);
        AdminTips(-1,string);
        BanEx(strval(pid),reason);
    }
    return 1;
}

COMMAND:allmute(playerid)
{
    new name[64],string[256];
    if(IsAdmin(playerid,3))
    {
        GetPlayerName(playerid,name,sizeof(name));
        if(!GetSVarInt("is_allmute"))
        {
            format(string,sizeof(string),"管理员%s(%d)开启了全体禁言",playerid,name);
            SetSVarInt("is_allmute",1);
            for(new i=0;i<MAX_PLAYERS;i++)
            {
                if(GetPVarInt(i,"is_admin")) continue;
                SetPVarInt(i,"is_mute",!GetPVarInt(i,"is_mute"));
            }
        }
        else
        {
            DeleteSVar("is_allmute");
            for(new i=0;i<MAX_PLAYERS;i++)
            {
                DeletePVar(i,"is_allmute");
            }
            format(string,sizeof(string),"管理员%s(%d)关闭了全体禁言",playerid,name);
        }
        AdminTips(-1,string);
    }
    return 1;
}

/*
    T-Admin - 山城定制版
    本源代码由IGTA褪色开发，为山城车会服务器定制管理员脚本
    发现bug请及时反馈褪色
    请勿删除本行，如有侵权，后果自负！

    PS:使用该脚本请在scriptfiles文件夹下创建一个Admins空文件夹，否则将会导致服务器崩溃

    功能
    T-Admin

    通用
        注册管理员账号/ar
        登陆管理员账号/al
        退出执勤状态/tc
        查看在线管理员/gms
        举报玩家/jubao
        管理员频道聊天/ac

    1级
        警告玩家/jingao
        监禁玩家 /jianjin
        禁言玩家 /jinyan
        清除玩家武器/moshou
        清屏/qing

    2级
        将玩家踢出服务器/ti
        发布服务器管理员公告/say
        冻结玩家/dong
        禁止玩家进入载具 /nocar
        删除载具 /delc

    3级
        修改玩家名字/setname
        奖励玩家时间分/jiangli
        全员禁言/muteall

    4级
        封禁玩家/ban

    5级/Rcon级
        设置玩家管理员等级/setadmin
*/

#include <a_samp>
#include <sscanf>
#include <izcmd>
#include <dini>


#define COLOR_RED 0xFF0000C8

public OnFilterScriptInit()
{
    print("T - Admin By Tuise Loaded Successfully");
    SetTimer("Jail",1000,1);
    return 1;
}

public OnPlayerText(playerid, text[])
{
    if(GetPVarInt(playerid,"is_mute")){
        AdminTips(playerid,"您已被管理员禁言,请联系管理员解除禁言");
        return 0;
    }
    return 1;
}

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

public OnPlayerSpawn(playerid)
{
    AdminTips(playerid,"Tuise-Admin褪色管理员系统重拳出击!!");
    return 1;
}

COMMAND:ar(playerid,params[])
{
    new name[64],files[64],password[64];
    GetPlayerName(playerid,name,sizeof(name));
    format(files,sizeof(files),"\\Admins\\%s.ini",name);
    if(sscanf(params,"s[64]",password)) return AdminTips(playerid,"注册管理员账号:/azc [密码]");
    if(fexist(files)) return AdminTips(playerid,"您已注册过管理员账号");
    if(!fexist(files)){
        dini_Create(files);
        dini_Set(files,"Name",name);
        dini_Set(files,"Password",password);
        dini_IntSet(files,"Level",0);
        AdminTips(playerid,"注册管理员账号成功,请联系高层获取权限");
        return 1;
    }
    return 1;
}

COMMAND:al(playerid,params[])
{
    new password[64],name[64],files[64],string[256];
    if(sscanf(params,"s[64]",password)) return AdminTips(playerid,"登陆管理员账号:/al [密码]");
    if(GetPVarInt(playerid,"is_admin")) return AdminTips(playerid,"您已登陆,无需重新登陆");
    GetPlayerName(playerid,name,sizeof(name));
    format(files,sizeof(files),"\\Admins\\%s.ini",name);
    if(!fexist(files)) return AdminTips(playerid,"您未注册管理员账号,注册管理员账号:/ar");
    if(dini_Int(files,"Level") == 0) return AdminTips(playerid,"您没有权限,请联系高层获取权限");
    if(!strcmp(dini_Get(files,"Password"),password,true)){
        format(string,sizeof(string),"管理员%s(%d)上线执勤,权限等级:%d",name,playerid,dini_Int(files,"Level"));
        AdminTips(-1,string);
        SetPVarInt(playerid,"is_admin",dini_Int(files,"Level"));
        SetPlayerColor(playerid,COLOR_RED);
        return 1;
    }
    else{
        AdminTips(playerid,"您输入的密码有误,请核实后重新输入");
    }
    return 1;
}

COMMAND:tc(playerid)
{
    new name[64],string[256];
    if(!GetPVarInt(playerid,"is_admin")) return AdminTips(playerid,"您不在执勤状态,无法退出执勤");
    GetPlayerName(playerid,name,sizeof(name));
    format(string,sizeof(string),"管理员%s(%d)退出执勤状态",name,playerid);
    AdminTips(-1,string);
    DeletePVar(playerid,"is_admin");
    return 1;
}

COMMAND:gms(playerid)
{
    AdminTips(playerid,"当前在线管理员");
    for(new i=0;i<MAX_PLAYERS;i++){
        if(GetPVarInt(i,"is_admin")){
            new name[64],string[256];
            GetPlayerName(i,name,sizeof(name));
            format(string,sizeof(string),"%s(%d) - %d级",name,i,GetPVarInt(i,"is_admin"));
            AdminTips(playerid,string);
        }
    }
    return 1;
}

COMMAND:jubao(playerid,params[])
{
    new name[64],name1[64],reason[128],string[256];
    new pid;
    if(sscanf(params,"ds[128]",pid,reason)) return AdminTips(playerid,"举报玩家:/jubao [玩家ID] [原因]");
    GetPlayerName(playerid,name,sizeof(name));
    GetPlayerName(pid,name1,sizeof(name1));
    format(string,sizeof(string),"玩家%s(%d)举报玩家%s(%d),原因:%s",name,playerid,name1,pid,reason);
    for(new i=0;i<MAX_PLAYERS;i++){
        if(GetPVarInt(i,"is_admin")){
            AdminTips(i,string);
        }
    }
    AdminTips(playerid,"您的举报信息已发送至在线管理员");
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

//1级
COMMAND:jinggao(playerid,params[])
{
    new name[64],name1[64],reason[128],string[256],pid[18];
    new Float:x,Float:y,Float:z;
    if(sscanf(params,"s[18]s[128]",pid,reason)) return AdminTips(playerid,"警告玩家: /jinggao [玩家ID] [原因]");
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

COMMAND:jinyan(playerid,params[])
{
    new name[64],name1[64],pid[18],string[256];
    if(sscanf(params,"s[18]",pid)) return AdminTips(playerid,"禁言玩家: /jinyan [玩家ID]");
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

COMMAND:jiejinyan(playerid,params[])
{
    new name[64],name1[64],string[256],pid[18];
    if(sscanf(params,"s[18]",pid)) return AdminTips(playerid,"解禁玩家: /jiejinyan [玩家ID]");
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

COMMAND:jianyu(playerid,params[])
{
    new name[64],name1[64],string[256],pid[18],sec[18];
    if(sscanf(params,"s[18]s[18]",pid,sec)) return AdminTips(playerid,"监禁玩家: /jianyu [玩家ID] [秒数]");
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

COMMAND:qing(playerid)
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

//2级
COMMAND:ti(playerid,params[])
{
    new name[64],name1[64],string[256],pid[18],reason[128];
    if(sscanf(params,"s[18]s[128]",pid,reason)) AdminTips(playerid,"踢出玩家: /ti [玩家ID] [原因]");
    if(IsAdmin(playerid,2)){
        if(!IsPlayerConnected(strval(pid))) return AdminTips(playerid,"该玩家不在线");
        GetPlayerName(playerid,name,sizeof(name));
        GetPlayerName(strval(pid),name1,sizeof(name1));
        format(string,sizeof(string),"管理员%s(%d)将玩家%s(%d)踢出服务器,原因:%s",name,playerid,name1,strval(pid),reason);
        AdminTips(-1,string);
        Kick(strval(pid));
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

COMMAND:dong(playerid,params[])
{
    new pid[18],name[64],name1[64],string[256];
    if(sscanf(params,"s[18]",pid)) return AdminTips(playerid,"禁止玩家行动:/dong [玩家ID]");
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

//4级
COMMAND:ban(playerid,params[])
{
    new pid[18],name[64],name1[64],string[256],reason[128];
    if(IsAdmin(playerid,4)){
        if(sscanf(params,"s[18]s[128]",pid,reason)) return AdminTips(playerid,"封禁玩家:/ban [玩家ID]");
        GetPlayerName(playerid,name,sizeof(name));
        GetPlayerName(strval(pid),name1,sizeof(name1));
        format(string,sizeof(string),"管理员%s(%d)永久封禁玩家%s(%d),原因:%s",name,playerid,name1,strval(pid),reason);
        AdminTips(-1,string);
        BanEx(strval(pid),reason);
    }
    return 1;
}

//5级、Rcon级
COMMAND:setadmin(playerid,params[])
{
    new pid[18],name[64],name1[64],string[256],files[64],level[18];
    if(sscanf(params,"s[18]s[18]",pid,level)) return AdminTips(playerid,"设置玩家管理员权限: /setadmin [玩家ID] [等级]");
    if(!IsPlayerConnected(strval(pid))) return AdminTips(playerid,"该玩家不在线");
    GetPlayerName(playerid,name,sizeof(name));
    GetPlayerName(strval(pid),name1,sizeof(name1));
    if(IsAdmin(playerid,5)){
        format(files,sizeof(files),"\\Admins\\%s.ini",name);
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

/*
Script Variable
is_rp - 判定是否在RP模式
is_rp_taxi 判定是否为出租车司机,每分钟20美元
is_rp_wait_taxi 判定是否在等待出租车司机接单
is_rp_taxi_ing 判定是否正在进行一项订单
is_pd 判定是否为PD
is_fd 判定是否为FD 
is_wanted_level 判定通缉等级
is_wanted_count 判定通缉时间
is_wanted_jail 判定入狱时间 
*/

new rb[MAX_PLAYERS][10]; //RoadBlock


forward GetPlayerRpMoney(playerid);
public GetPlayerRpMoney(playerid)
{
    new name[64],files[64];
    GetPlayerName(playerid,name,sizeof(name));
    format(files,sizeof(files),"\\Players\\%s.ini",name);
    return dini_Int(files,"Money");
}

forward GivePlayerRpMoney(playerid,money);
public GivePlayerRpMoney(playerid,money)
{
    new name[64],files[64];
    GetPlayerName(playerid,name,sizeof(name));
    format(files,sizeof(files),"\\Players\\%s.ini",name);
    dini_IntSet(files,"Money",dini_Int(files,"Money")+money);
    ResetPlayerMoney(playerid);
    GivePlayerMoney(playerid, dini_Int(files,"Money"));
    return 1;
}


forward RpTips(playerid,str[]);
public RpTips(playerid,str[])
{
    new string[1024];
    format(string,sizeof(string),"[RP系统]%s",str);
    if(playerid == -1){
        for(new i=0;i<MAX_PLAYERS;i++){
            if(GetPVarInt(i,"is_rp")){
                SendClientMessage(i, 0x8080FFC8, string);
            }
        }
        return 1;
    }
    SendClientMessage(playerid,0x8080FFC8,string);
    return 1;
}

forward RpMoney();
public RpMoney()
{
    for(new i=0;i<MAX_PLAYERS;i++){
        if(GetPVarInt(i,"is_rp")){
            if(GetPVarInt(i,"is_rp_taxi")){
                GivePlayerRpMoney(i,20);
                RpTips(i,"您正在担任出租车司机职位,您的工资已到账:20美元");
                continue;
            }
            RpTips(i,"您的工资已到账:10美元");
            GivePlayerRpMoney(i,10);
        }
    }
    return 1;
}

COMMAND:rp(playerid,params[])
{
    new params1[64],string[256],name[64];
    if(!GetPVarInt(playerid,"is_login")) return RpTips(playerid,"您没有登录,无法进入RP模式");
    if(sscanf(params,"s[64]",params1)){
        if(GetPVarInt(playerid,"is_rp"))
        {
            if(GetPVarInt(playerid,"is_wanted_level")) return RpTips(playerid,"您不可以通过退出RP来逃避通缉RP");
            DeletePVar(playerid,"is_rp");
            for(new i=0;i<10;i++)
            {
                DestroyObject(rb[playerid][i]);
            }
            new colors[6] = {0xFF80FFC8,0xFFFF80C8,0x00FF80C8,0x008000C8,0xFFFF80C8,0xFF8040C8};
            SetPlayerColor(playerid,colors[random(6)]);
            SetPlayerHealth(playerid,99999);
            RpTips(playerid,"您已退出RP模式,欢迎下次再来~");
            RemovePlayerFromVehicle(playerid);
            SetPlayerVirtualWorld(playerid,0);
            if(PlayerVehicle[playerid] != -1)
            {
                DestroyVehicle(PlayerVehicle[playerid]);
            }
            PlayerVehicle[playerid] = -1;
            Delete3DTextLabel(PlayerVehicleText[playerid]);
            ResetPlayerMoney(playerid);
            return 1;
        }
        else
        {
            for(new i=0;i<10;i++)
            {
                rb[playerid][i] = -1;
            }
            if(IsRpBan(playerid)) return RpTips(playerid,"您已被禁止进入RP模式");
            new files[64];
            GetPlayerName(playerid,name,sizeof(name));
            format(files,sizeof(files),"\\Server\\RolePlay\\%s.ini",name);
            new pfiles[64];
            format(pfiles,sizeof(pfiles),"\\Players\\%s.ini",name);
            /*
            if(!fexist(files))
            {
                if(dini_Int(pfiles,"Money") > 50000)
                {   
                    dini_IntSet(pfiles,"Money",2000);
                    ResetPlayerMoney(playerid);
                    CheatTips(playerid,"您的金钱数据紊乱,清空金钱数据,并补偿2000美元");
                    dini_Create(files);
                }
            }
            */
            GetPlayerName(playerid,name,sizeof(name));
            format(string,sizeof(string),"玩家%s(%d)进入RP模式",name,playerid);
            RpTips(-1,string);
            new jailfiles[64];
            format(jailfiles,sizeof(jailfiles),"\\Server\\RolePlay\\%s.ini",name);
            if(dini_Isset(files,"Jail"))
            {
                SetPVarInt(playerid,"is_wanted_jail",dini_Int(files,"Jail"));
                format(string,sizeof(string),"系统检测到玩家%s(%d)还需要服刑%d秒,已送往监狱",name,playerid,dini_Int(files,"Jail"));
            }
            SetPVarInt(playerid,"is_rp",!GetPVarInt(playerid,"is_rp"));
            DeletePVar(playerid,"is_god");
            SetPlayerHealth(playerid,100);
            SetPlayerArmour(playerid,0);
            ResetPlayerMoney(playerid);
            ResetPlayerWeapons(playerid);
            RemovePlayerFromVehicle(playerid);
            SetPlayerVirtualWorld(playerid,981);
            RpTips(playerid,"您已进入RP模式,请遵守RP规则");
            RpTips(playerid,"服务器RP模式官方交流群:192805772");
            SetPlayerColor(playerid,COLOR_WHITE);
            if(PlayerVehicle[playerid] != -1){
                DestroyVehicle(PlayerVehicle[playerid]);
            }
            PlayerVehicle[playerid] = -1;
            format(files,sizeof(files),"\\Players\\%s.ini",name);
            GivePlayerMoney(playerid,dini_Int(files,"Money"));
            return 1;
        }
    }
    return 1;
}

COMMAND:taxi(playerid)
{
    new name[64],string[256];
    if(!GetPVarInt(playerid,"is_rp")) return RpTips(playerid,"您不在RP模式,无法使用该功能");
    if(GetPVarInt(playerid,"is_rp_taxi")){
        DeletePVar(playerid,"is_rp_taxi");
        RpTips(playerid,"您已辞去出租车司机的工作");
        return 1;
    }
    else{
        SetPVarInt(playerid,"is_rp_taxi",!GetPVarInt(playerid,"is_rp_taxi"));
        GetPlayerName(playerid,name,sizeof(name));
        format(string,sizeof(string),"出租车司机%s(%d)上班了,输入 /call taxi 呼叫出租车",name,playerid);
        RpTips(-1,string);
        RpTips(playerid,"您已成为一名出租车司机,出租车ID为420或438,每分钟获得20美元");
        return 1;
    }
}

COMMAND:call(playerid,params[])
{
    new pid[18],content[128],name[64],name1[64],string[256];
    if(sscanf(params,"s[18]s[128]",pid,content))
    {
        if(!strlen(pid)) return RpTips(playerid,"打电话给玩家 /call [玩家ID] [内容]");
    }
    if(strval(pid)){
        if(strval(pid) == 911){
            if(!strlen(content)) return RpTips(playerid,"报警求助 /call 911 [简述+地点]");
            GetPlayerName(playerid,name,sizeof(name));
            RpTips(playerid,"您已发出报警求助,PD将会派遣警员");
            format(string,sizeof(string),"警情通知 - 市民%s(%d) - %s",name,playerid,content);
            for(new i=0;i<MAX_PLAYERS;i++){
                if(GetPVarInt(i,"is_pd") || GetPVarInt(i,"is_fd")){
                    RpTips(i,string);
                }
            }
        }

        if(IsPlayerConnected(strval(pid))){
            if(!strlen(content)) return RpTips(playerid,"打电话给玩家 /call [玩家ID] [内容]");
            GetPlayerName(playerid,name,sizeof(name));
            GetPlayerName(strval(pid),name1,sizeof(name1));
            format(string,sizeof(string),"电话_%s(%d)->%s(%d):%s",name,playerid,name1,strval(pid),content);
            RpTips(playerid,string);
            RpTips(strval(pid),string);
        }
        else{
            RpTips(playerid,"您拨打的电话暂时无法接通,请稍后再拨...");
        }
        return 1;
    }
    if(!strcmp(pid,"taxi",true))
    {
        GetPlayerName(playerid,name,sizeof(name));
        format(string,sizeof(string),"玩家%s(%d)发来一则出租车订单,输入/jd进行接单",name,playerid);
        RpTips(playerid,"已将您的接送订单发送至出租车司机平台,请等待出租车司机接单");
        SetPVarInt(playerid,"is_rp_wait_taxi",!GetPVarInt(playerid,"is_rp_wait_taxi"));
        for(new i=0;i<MAX_PLAYERS;i++){
            if(GetPVarInt(i,"is_rp_taxi")){
                RpTips(i,string);
            }
        }
        return 1;
    }
    return 1;
}

COMMAND:jd(playerid)
{
    new name[64],string[256];
    new num =0;
    new Float:x,Float:y,Float:z;
    if(!GetPVarInt(playerid,"is_rp_taxi")) return RpTips(playerid,"您不是出租车司机,无法接单");
    if(GetPVarInt(playerid,"is_rp_taxi_ing")) return RpTips(playerid,"您已接下一单,在完成本单前无法接下一单");
    for(new i=0;i<MAX_PLAYERS;i++){
        if(GetPVarInt(i,"is_rp_wait_taxi")){
            num = num + 1;
            DeletePVar(i,"is_rp_wait_taxi");
            SetPVarInt(playerid,"is_rp_taxi_ing",!GetPVarInt(playerid,"is_rp_taxi_ing"));
            GetPlayerName(playerid,name,sizeof(name));
            format(string,sizeof(string),"出租车司机%s(%d)已接单,请在原地等待",name,playerid);
            RpTips(i,string);
            GetPlayerName(i,name,sizeof(name));
            GetPlayerPos(i,x,y,z);
            format(string,sizeof(string),"请前往红点接玩家%s(%d)",name,i);
            RpTips(playerid,string);
            RpTips(playerid,"您可以输入 /cjd 来取消本次订单");
            SetPlayerRaceCheckpoint(playerid, 0, x, y, z, x, y, z, 20.0);
        }
    }
    if(num == 0) return RpTips(playerid,"暂时没有人发起接送订单");
    return 1;
}

COMMAND:cjd(playerid)
{
    if(!GetPVarInt(playerid,"is_rp_taxi_ing")) return RpTips(playerid,"你没有接任何订单,无法取消订单");
    DisablePlayerRaceCheckpoint(playerid);
    DeletePVar(playerid,"is_rp_taxi_ing");
    RpTips(playerid,"您已取消本次订单");
    return 1;
}

COMMAND:pay(playerid,params[])
{
    new money[64],name[64],name1[64],string[256],pid[18];
    if(!GetPVarInt(playerid,"is_rp")) return RpTips(playerid,"您不在RP模式,无法使用该功能");
    if(sscanf(params,"s[18]s[64]",pid,money)) return RpTips(playerid,"向玩家进行转账 /pay [玩家ID] [转账金额]");
    if(!IsPlayerConnected(strval(pid))) return RpTips(playerid,"该玩家不在线");
    GetPlayerName(playerid,name,sizeof(name));
    GetPlayerName(strval(pid),name1,sizeof(name1));
    if(strval(money) < 0) return RpTips(playerid,"转账金额不得小于0");
    if(GetPlayerRpMoney(playerid) < strval(money))
    {
        format(string,sizeof(string),"您的余额为:%d美元,余额不足,无法进行转账",GetPlayerRpMoney(playerid));
        RpTips(playerid,string);
        return 1;
    }
    GivePlayerRpMoney(playerid,-strval(money));
    GivePlayerRpMoney(strval(pid),strval(money));
    format(string,sizeof(string),"玩家%s(%d)向你转账%d美元,目前余额:%d",name,playerid,strval(money),GetPlayerRpMoney(strval(pid)));
    RpTips(strval(pid),string);
    format(string,sizeof(string),"您向玩家%s(%d)转账%d美元,目前余额:%d",name1,strval(pid),strval(money),GetPlayerRpMoney(playerid));
    RpTips(playerid,string);
    return 1;
}

COMMAND:money(playerid)
{
    new string[256],name[64],files[64];
    GetPlayerName(playerid,name,sizeof(name));
    format(files,sizeof(files),"\\Players\\%s.ini",name);
    format(string,sizeof(string),"您目前的余额为:%d美元",dini_Int(files,"Money"));
    RpTips(playerid,string);
    return 1;
}

COMMAND:adv(playerid,params[])
{
    new pid[18],name[64],name1[64],string[256],files[64];
    if(!GetPVarInt(playerid,"is_login")) return RpTips(playerid,"请先登录后再使用权限");
    if(sscanf(params,"s[18]",pid)) return RpTips(playerid,"添加或删除玩家高级权限 /adv [玩家ID]");
    GetPlayerName(playerid,name,sizeof(name));
    format(files,sizeof(files),"\\Server\\RolePlay\\Admins\\%s.ini",name);
    if(!fexist(files)) return RpTips(playerid,"您没有权限");
    if(!IsPlayerConnected(strval(pid))) return RpTips(playerid,"该玩家不在线");
    GetPlayerName(strval(pid),name1,sizeof(name1));
    format(files,sizeof(files),"\\Server\\RolePlay\\Adv\\%s.ini",name1);
    if(fexist(files))
    {
        fremove(files);
        format(string,sizeof(string),"RP管理员%s(%d)将玩家%s(%d)的高级扮演权限移除",name,playerid,name1,strval(pid));
        RpTips(-1,string);
        return 1;
    }
    dini_Create(files);
    format(string,sizeof(string),"RP管理员%s(%d)允许玩家%s(%d)进行高级扮演",name,playerid,name1,strval(pid));
    RpTips(-1,string);
    return 1;
}

COMMAND:pd(playerid)
{
    new name[64],string[256],files[64];
    if(!GetPVarInt(playerid,"is_rp")) return RpTips(playerid,"您不在RP模式,无法使用该功能");
    GetPlayerName(playerid,name,sizeof(name));
    format(files,sizeof(files),"\\Server\\RolePlay\\Adv\\%s.ini",name);
    if(!fexist(files)) return RpTips(playerid,"您没有高级扮演权限");
    if(GetPVarInt(playerid,"is_pd"))
    {
        DeletePVar(playerid,"is_pd");
        ResetPlayerWeapons(playerid);
        SetPlayerHealth(playerid,100);
        SetPlayerArmour(playerid,0);
        format(string,sizeof(string),"警员%s(%d)退出执勤状态",name,playerid);
        RpTips(-1,string);
        SetPlayerColor(playerid,COLOR_WHITE);
        return 1;
    }
    SetPVarInt(playerid,"is_pd",6);
    format(string,sizeof(string),"警员%s(%d)进入执勤状态",name,playerid);
    RpTips(-1,string);
    SetPlayerHealth(playerid,100);
    SetPlayerArmour(playerid,100);
    SetPlayerColor(playerid,0x0D52F2C8);
    return 1; 
}

COMMAND:fd(playerid)
{
    new name[64],string[256],files[64];
    if(!GetPVarInt(playerid,"is_rp")) return RpTips(playerid,"您不在RP模式,无法使用该功能");
    GetPlayerName(playerid,name,sizeof(name));
    format(files,sizeof(files),"\\Server\\RolePlay\\Adv\\%s.ini",name);
    if(!fexist(files)) return RpTips(playerid,"您没有高级扮演权限");
    if(GetPVarInt(playerid,"is_fd"))
    {
        DeletePVar(playerid,"is_fd");
        ResetPlayerWeapons(playerid);
        SetPlayerHealth(playerid,100);
        SetPlayerArmour(playerid,0);
        format(string,sizeof(string),"FD人员%s(%d)退出执勤状态",name,playerid);
        RpTips(-1,string);
        SetPlayerColor(playerid,COLOR_WHITE);
        return 1;
    }
    SetPVarInt(playerid,"is_fd",6);
    format(string,sizeof(string),"FD人员%s(%d)进入执勤状态",name,playerid);
    RpTips(-1,string);
    SetPlayerColor(playerid,0xE11EE1C8);
    SetPlayerHealth(playerid,100);
    SetPlayerArmour(playerid,100);
    return 1; 
}

COMMAND:hhq(playerid,params[])
{
    new name[64],string[256],content[128];
    if(!GetPVarInt(playerid,"is_rp")) return RpTips(playerid,"您不在RP模式,无法使用该功能");
    if(!GetPVarInt(playerid,"is_pd")) return RpTips(playerid,"您不是警员,无法使用喊话器");
    if(sscanf(params,"s[128]",content)) return RpTips(playerid,"使用喊话器 /hhq [内容]");
    GetPlayerName(playerid,name,sizeof(name));
    format(string,sizeof(string),"警员%s(%d)使用喊话器:%s",name,playerid,content);
    RpTips(-1,string);
    return 1;
}

COMMAND:ra(playerid,params[])
{
    new name[64],string[256],content[128];
    if(!GetPVarInt(playerid,"is_rp")) return RpTips(playerid,"您不在RP模式,无法使用该功能");
    if(GetPVarInt(playerid,"is_pd") || GetPVarInt(playerid,"is_fd")){
        GetPlayerName(playerid,name,sizeof(name));
        if(sscanf(params,"s[128]",content)) return RpTips(playerid,"使用PD/FD公用通信频道 /ra [内容]");
        if(GetPVarInt(playerid,"is_fd")){
            format(string,sizeof(string),"无线电频道_FD_%s(%d):%s",name,playerid,content);
        }
        if(GetPVarInt(playerid,"is_pd")){
            format(string,sizeof(string),"无线电频道_PD_%s(%d):%s",name,playerid,content);
        }
        for(new i=0;i<MAX_PLAYERS;i++){
            if(GetPVarInt(i,"is_fd") || GetPVarInt(i,"is_pd")){
                RpTips(i,string);
            }
        }
    }
    else{
        RpTips(playerid,"您不是FD/PD,无法使用无线电对话频道");
    }
    return 1;
}

COMMAND:b(playerid,params[])
{
    new name[64],content[128],string[256];
    if(!GetPVarInt(playerid,"is_rp")) return RpTips(playerid,"您不在RP模式,无法使用该功能");
    if(sscanf(params,"s[128]",content)) return RpTips(playerid,"OOC对话 /b [内容]");
    GetPlayerName(playerid,name,sizeof(name));
    format(string,sizeof(string),"[OOC]%s(%d):%s",name,playerid,content);
    for(new i=0;i<MAX_PLAYERS;i++){
        if(GetPVarInt(i,"is_rp")){
            SendClientMessage(i, COLOR_WHITE, string);
        }
    }
    new chatlog[256],time[64];
    new year,month,day,hour,mini,sec;
	GetPlayerName(playerid,name,sizeof(name));
	getdate(year,month,day);
	gettime(hour,mini,sec);
	format(time,sizeof(time),"[%d/%d/%d %d:%d:%d]",year,month,day,hour,mini,sec);
    format(chatlog,sizeof(chatlog),"%s%s(%d):%s_RolePlayOOC\n",time,name,playerid,content);
	WriteChatLog(chatlog);
    return 1;
}

/*
stock CreateWantedTextDraw(playerid,second)
{
    new name[64],string[256];
    GetPlayerName(playerid,name,sizeof(name));
    format(string,sizeof(string),"%s: %ds",name,second);
    for(new i=0;i<9;i++)
    {
        if(wtxd[9] != -1)
        {
            wtxd[0][0] = second;
            wtxd[0] = TextDrawCreate(458,413,string);
            TextDrawAlignment(wtxd[0],1);
            TextDrawFont(wtxd[0],1);
            TextDrawLetterSize(wtxd[0],0.1,0.1);
            TextDrawColor(wtxd[0],0x8853F9FF);
            return 1;
        }

        if(wtxd[i] == -1)
        {
            wtxd[i][i] = second;
            wtxd[i] = TextDrawCreate(458,413,string);
            TextDrawAlignment(wtxd[i],1);
            TextDrawFont(wtxd[i],1);
            TextDrawLetterSize(wtxd[i],0.1,0.1);
            TextDrawColor(wtxd[i],0x8853F9FF);
            return 1;
        }
    }
    return 1;
}

stock WantedTextDrawRefresh()
{
    for(new i=0;i<9;i++)
    {
        if(wtxd[i] != -1)
        {
            wtxd[i][i] = wtxd[i][i] - 1;
        }
    }
    return 1;
}

*/

COMMAND:star(playerid,params[])
{
    new pid[18],reason[64],name[64],name1[64],string[256],level[18];
    if(!GetPVarInt(playerid,"is_pd")) return RpTips(playerid,"您不是警员,无法使用通缉功能");
    if(sscanf(params,"s[18]s[18]s[64]",pid,level,reason)) return RpTips(playerid,"通缉市民 /star [玩家ID] [等级] [原因]");
    if(!IsPlayerConnected(strval(pid))) return RpTips(playerid,"该玩家不在线");
    if(strval(level) < 0 || strval(level) > 6) return RpTips(playerid,"通缉等级范围在0~6");
    GetPlayerName(playerid,name,sizeof(name));
    GetPlayerName(strval(pid),name1,sizeof(name1));
    format(string,sizeof(string),"警员%s(%d)对市民%s(%d)下达了%d级通缉,原因:%s",name,playerid,name1,strval(pid),strval(level),reason);
    RpTips(-1,string);
    SetPVarInt(strval(pid),"is_wanted_level",strval(level));
    SetPVarInt(strval(pid),"is_wanted_count",strval(level)*100);
    SetPlayerWantedLevel(strval(pid),strval(level));
    SetPlayerColor(strval(pid),0x400000C8);
    new files[64];
    format(files,sizeof(files),"\\Server\\RolePlay\\%s.ini",name1);
    dini_IntSet(files,"Jail",strval(level)*100);
    return 1;
}

forward Wanted();
public Wanted()
{
    for(new i=0;i<MAX_PLAYERS;i++){
        if(GetPVarInt(i,"is_wanted_level")){
            SetPVarInt(i,"is_wanted_count",GetPVarInt(i,"is_wanted_count")-1);
            if(GetPVarInt(i,"is_wanted_count") == 501){
                SetPlayerWantedLevel(i,5);
                SetPVarInt(i,"is_wanted_level",GetPVarInt(i,"is_wanted_level")-1);
            }
            if(GetPVarInt(i,"is_wanted_count") == 401){
                SetPlayerWantedLevel(i,4);
                SetPVarInt(i,"is_wanted_level",GetPVarInt(i,"is_wanted_level")-1);
            }
            if(GetPVarInt(i,"is_wanted_count") == 301){
                SetPlayerWantedLevel(i,3);
                SetPVarInt(i,"is_wanted_level",GetPVarInt(i,"is_wanted_level")-1);
            }
            if(GetPVarInt(i,"is_wanted_count") == 201){
                SetPlayerWantedLevel(i,2);
                SetPVarInt(i,"is_wanted_level",GetPVarInt(i,"is_wanted_level")-1);
            }
            if(GetPVarInt(i,"is_wanted_count") == 101){
                SetPlayerWantedLevel(i,1);
                SetPVarInt(i,"is_wanted_level",GetPVarInt(i,"is_wanted_level")-1);
            }
            if(GetPVarInt(i,"is_wanted_count") == 1){
                SetPlayerWantedLevel(i,0);
                DeletePVar(i,"is_wanted_count");
                DeletePVar(i,"is_wanted_level");
                new string[256],name[64];
                GetPlayerName(i,name,sizeof(name));
                format(string,sizeof(string),"嫌疑犯%s(%d)已逃过警方的追捕",name,i);
                SetPlayerColor(i,COLOR_WHITE);
                RpTips(-1,string);
            }
        }
    }
    return 1;
}

forward RpJail();
public RpJail()
{
    new Float:x,Float:y,Float:z;
    new string[256];
    new name[64],files[64];
    for(new i=0;i<MAX_PLAYERS;i++){
        if(GetPVarInt(i,"is_wanted_jail")){
            GetPlayerName(i,name,sizeof(name));
            format(files,sizeof(files),"\\Server\\RolePlay\\%s.ini",name);
            SetPVarInt(i,"is_wanted_jail",GetPVarInt(i,"is_wanted_jail")-1);
            dini_IntSet(files,"Jail",GetPVarInt(i,"is_wanted_jail"));
            if(GetPVarInt(i,"is_wanted_jail") == 1){
                DeletePVar(i,"is_wanted_jail");
                dini_Unset(files,"Jail");
                RpTips(i,"刑期已满,已被释放,请好好做人!");
                SetPlayerColor(i,COLOR_WHITE);
                SetPlayerPos(i,1545.508300,-1675.517089,13.560499);
                return 1;
            }
            //-2174.765625,-256.060913,36.515625
            //-2189.426757,-269.240661,36.515625
            GetPlayerPos(i,x,y,z);
            if(x < -2189.426757 || x > -2174.765625 || y < -269.240661 || y > -256.060913 || z < 34.515625 || y > 38.515625){
                SetPlayerPos(i,-2182.973632,-263.378509,36.515625);
                format(string,sizeof(string),"您还剩%d后出狱",GetPVarInt(i,"is_wanted_jail"));
                RpTips(i,string);
            }
        }
    }
    return 1;
}

forward IsRpAdmin(playerid);
public IsRpAdmin(playerid)
{
    new name[64],files[64];
    GetPlayerName(playerid,name,sizeof(name));
    format(files,sizeof(files),"\\Server\\RolePlay\\Admins\\%s.ini",name);
    if(fexist(files)){
        return true;
    }
    else{
        return false;
    }
}

COMMAND:rpban(playerid,params[])
{
    if(!GetPVarInt(playerid,"is_login")) return ServerTips(playerid,"您还没有登录,无法进行任何操作");
    new pid[18],reason[64],string[256],name[64],name1[64],files[64];
    if(!IsRpAdmin(playerid)) return RpTips(playerid,"您不是RP管理员,无法进行该操作");
    if(sscanf(params,"s[18]s[64]",pid,reason)) return RpTips(playerid,"禁止玩家进入RP模式 /rpban [玩家ID] [原因]");
    if(!IsPlayerConnected(strval(pid))) return RpTips(playerid,"该玩家不在线");
    GetPlayerName(playerid,name,sizeof(name));
    GetPlayerName(strval(pid),name1,sizeof(name1));
    format(string,sizeof(string),"RP管理员%s(%d)禁止玩家%s(%d)进入RP模式,原因:%s",name,playerid,name1,strval(pid),reason);
    RpTips(-1,string);
    DeletePVar(strval(pid),"is_rp");
    SetPlayerVirtualWorld(strval(pid),0);
    format(files,sizeof(files),"\\Server\\RolePlay\\Ban\\%s.ini",name1);
    if(!fexist(files)){
        dini_Create(files);
    }
    return 1;
}

forward IsRpBan(playerid);
public IsRpBan(playerid)
{
    new name[64],files[64];
    GetPlayerName(playerid,name,sizeof(name));
    format(files,sizeof(files),"\\Server\\RolePlay\\Ban\\%s.ini",name);
    if(fexist(files)){
        return true;
    }
    else{
        return false;
    }
}


COMMAND:unrpban(playerid,params[])
{
    new pid[18],string[256],name[64],name1[64],files[64];
    if(!IsRpAdmin(playerid)) return RpTips(playerid,"您不是RP管理员,无法进行该操作");
    if(sscanf(params,"s[18]",pid)) return RpTips(playerid,"解除禁止玩家进入RP模式 /unrpban [玩家ID]");
    if(!IsPlayerConnected(strval(pid))) return RpTips(playerid,"该玩家不在线");
    GetPlayerName(playerid,name,sizeof(name));
    GetPlayerName(strval(pid),name1,sizeof(name1));
    format(files,sizeof(files),"\\Server\\RolePlay\\Ban\\%s.ini",name1);
    if(!fexist(files)){
        RpTips(playerid,"该玩家未被禁止进入RP模式");
        return 1;
    }
    else{
        fremove(files);
        format(string,sizeof(string),"RP管理员%s(%d)解除玩家%s(%d)禁止进入RP模式",name,playerid,name1,strval(pid));
        RpTips(-1,string);
    }
    return 1;
}

COMMAND:rpdelc(playerid,params[])
{
    new pid[18],name[64],name1[64],string[256];
    if(!IsRpAdmin(playerid)) return RpTips(playerid,"您不是RP管理员,无法进行该操作");
    if(sscanf(params,"s[18]",pid)) return RpTips(playerid,"删除玩家当前载具 /rpdelc [玩家ID]");
    if(!IsPlayerConnected(strval(pid))) return RpTips(playerid,"该玩家不在线");
    if(GetPlayerVehicleSeat(strval(pid)) != 0) return RpTips(playerid,"该玩家不是司机,无法删除");
    GetPlayerName(playerid,name,sizeof(name));
    GetPlayerName(strval(pid),name1,sizeof(name1));
    format(string,sizeof(string),"RP管理员%s(%d)删除玩家%s(%d)的载具",name,playerid,name1,strval(pid));
    RpTips(-1,string);
    DestroyVehicle(PlayerVehicle[strval(pid)]);
    PlayerVehicle[strval(pid)] = -1;
    return 1;
}

stock ProxDetectorRP(Float:dis,playerid,string[])
{
    new Float:x,Float:y,Float:z;
	GetPlayerPos(playerid,x,y,z);
    for(new i=0; i < MAX_PLAYERS; i++)
	{
		if(IsPlayerInRangeOfPoint(i, dis, x,y,z)) SendClientMessage(i,GetPlayerColor(playerid),string);
	}
}

stock ProxDetectorRPTips(Float:dis,playerid,string[])
{
    new Float:x,Float:y,Float:z;
	GetPlayerPos(playerid,x,y,z);
    for(new i=0; i < MAX_PLAYERS; i++)
	{
		if(IsPlayerInRangeOfPoint(i, dis, x,y,z)) SendClientMessage(i,0x8080FFC8,string);
	}
}

stock ProxDetector(Float:dis,playerid,string[])
{
    new Float:x,Float:y,Float:z;
	GetPlayerPos(playerid,x,y,z);
    for(new i=0; i < MAX_PLAYERS; i++)
	{
		if(IsPlayerInRangeOfPoint(i, dis, x,y,z)) SendClientMessage(i,-1,string);
	}
}

CMD:shout(playerid,params[])
{
    new string[256];
    if(sscanf(params, "s[256]", string)) return SendClientMessage(playerid, -1, "{9370DB}大喊 /shout [内容]");
    new pname[24];
    GetPlayerName(playerid,pname,24);
    format(string, sizeof(string), "{9370DB}%s 大喊: %s !!", pname, string);
	ProxDetector(30.0,playerid,string);
	return 1;
}

CMD:whisper(playerid,params[])
{
    new string[256];
    if(sscanf(params, "s[256]", string)) return SendClientMessage(playerid, -1, "{9370DB}耳语 /whisper [内容]");
    new pname[24];
    GetPlayerName(playerid,pname,24);
    format(string, sizeof(string), "{9370DB}%s 密语: %s", pname, string);
	ProxDetector(2.0,playerid,string);
	return 1;
}

CMD:me(playerid,params[])
{
    new string[256];
    if(sscanf(params, "s[256]", string)) return SendClientMessage(playerid, -1, "{9370DB}环境消息 /me [内容]");
    new pname[24];
    GetPlayerName(playerid,pname,24);
    format(string, sizeof(string), "{9370DB}* %s %s", pname, string);
	ProxDetector(20.0,playerid,string);
	return 1;
}

CMD:do(playerid,params[])
{
    new string[256];
    if(sscanf(params, "s[256]", string)) return SendClientMessage(playerid, -1, "{9370DB}动作 /do [内容]");
    new pname[24];
    GetPlayerName(playerid,pname,24);
    format(string, sizeof(string), "{9370DB}* %s (( %s ))", pname, string);
	ProxDetector(20.0,playerid,string);
	return 1;
}

COMMAND:rent(playerid,params[])
{
    new vid[18],string[256];
    new num = 0;
    new Float:x,Float:y,Float:z,Float:a;
    if(!GetPVarInt(playerid,"is_rp")) return RpTips(playerid,"您不在RP模式中,无法使用该命令");
    new BanCar[] = {427,432,433,427,523,596,597,598,599,497,416,407};
    if(sscanf(params,"s[18]",vid)) return RpTips(playerid,"租用一台临时载具 /rent [载具ID]");
    if(GetPlayerRpMoney(playerid) < 100){
        format(string,sizeof(string),"租用载具需要花费100美元,您的余额不足,无法租用载具,当前余额:%d",GetPlayerMoney(playerid));
        RpTips(playerid,string);
        return 1;
    }
    if(GetPVarInt(playerid,"is_pd") || GetPVarInt(playerid,"is_fd")){
        format(string,sizeof(string),"您征用一台ID为%d的载具,当前余额:%d",strval(vid),GetPlayerMoney(playerid));
    }
    else{
        for(new i=0;i<sizeof(BanCar);i++){
            if(strval(vid) == BanCar[i]){
                num += 1;
            }
        }
        if(num == 1) return RpTips(playerid,"您没有权限生成特种载具");
        GivePlayerRpMoney(playerid,-100);
        format(string,sizeof(string),"您花费100美元租用ID为%d的载具,当前余额:%d",strval(vid),GetPlayerMoney(playerid));
    }
    RpTips(playerid,string);
    if(PlayerVehicle[playerid] != -1){
        DestroyVehicle(PlayerVehicle[playerid]);
    }
    GetPlayerPos(playerid,x,y,z);
    GetPlayerFacingAngle(playerid,a);
    new PDCar[] = {427,523,596,597,598,599,497};
    new an = 0;
    for(new i =0;i<sizeof(PDCar);i++){
        if(strval(vid) == PDCar[i]){
            PlayerVehicle[playerid] = CreateVehicle(strval(vid),x,y,z,a,0,1,-1);
            an = 1;
        }
    }
    new FDCar[] = {416,407}
    for(new i =0;i<sizeof(FDCar);i++){
        if(strval(vid) == FDCar[i]){
            if(strval(vid) == 407){
                PlayerVehicle[playerid] = CreateVehicle(strval(vid),x,y,z,a,3,3,-1);
            }
            if(strval(vid) == 416){
                PlayerVehicle[playerid] = CreateVehicle(strval(vid),x,y,z,a,1,3,-1);
            }
            an = 1;
        }
    }
    if(an == 0){
        PlayerVehicle[playerid] = CreateVehicle(strval(vid),x,y,z,a,random(10),random(10),-1);
    }
    SetVehicleVirtualWorld(PlayerVehicle[playerid],GetPlayerVirtualWorld(playerid));
    PutPlayerInVehicle(playerid,PlayerVehicle[playerid],0);
    return 1;
}

COMMAND:repair(playerid)
{
    new string[256];
    if(!GetPVarInt(playerid,"is_rp")) return RpTips(playerid,"您不在RP模式中,无法使用该命令");
    if(!IsPlayerInAnyVehicle(playerid)) return RpTips(playerid,"您不在任何载具中,无法修复该载具");
    if(GetPlayerVehicleSeat(playerid) != 0) return RpTips(playerid,"您不是该载具的司机,无法修复该载具");
    if(GetPlayerMoney(playerid) < 20){
        format(string,sizeof(string),"修复载具需要花费20美元,您的余额不足,无法修复载具,当前余额:%d",GetPlayerMoney(playerid));
        RpTips(playerid,string);
    }
    GivePlayerRpMoney(playerid,-20);
    format(string,sizeof(string),"您花费20美元修复载具,当前余额:%d",GetPlayerMoney(playerid));
    RpTips(playerid,string);
    SetPVarInt(playerid,"is_rp_repair",30);
    return 1;
}

forward RpRepair();
public RpRepair()
{
    for(new i=0;i<MAX_PLAYERS;i++){
        if(GetPVarInt(i,"is_rp_repair")){
            if(GetPVarInt(i,"is_rp_repair") == 0){
                DeletePVar(i,"is_rp_repair");
                RepairVehicle(GetPlayerVehicleID(i));
                TogglePlayerControllable(i,1);
                break;
            }
            SetPVarInt(i,"is_rp_repair",GetPVarInt(i,"is_rp_repair")-1);
            new string[128];
            format(string,sizeof(string),"正在修复载具...剩余时间:%d秒",GetPVarInt(i,"is_rp_repair"));
            RpTips(i,string);
            TogglePlayerControllable(i, 0);
        }
    }
    return 1;
}

COMMAND:heal(playerid,params[])
{
    new pid[18],name[64],name1[64],string[256];
    if(!GetPVarInt(playerid,"is_fd")) return RpTips(playerid,"您没有权限使用该命令");
    if(sscanf(params,"s[18]",pid)) return RpTips(playerid,"治疗某人 /heal [玩家ID]");
    if(!IsPlayerConnected(strval(pid))) return RpTips(playerid,"该玩家不在线");
    if(!GetPVarInt(playerid,"is_rp")) return RpTips(playerid,"该玩家不在RP模式中,无法治疗");
    new Float:x,Float:y,Float:z;
    GetPlayerPos(playerid,x,y,z);
    if(IsPlayerInRangeOfPoint(strval(pid), 5, x, y, z)){
        GetPlayerName(playerid,name,sizeof(name));
        GetPlayerName(strval(pid),name1,sizeof(name1));
        SetPlayerHealth(strval(pid),100);
        format(string,sizeof(string),"医护人员%s(%d)治疗玩家%s(%d)",name,playerid,name1,strval(pid));
        RpTips(-1,string);
        return 1;
    }
    RpTips(playerid,"您要治疗的对象与您距离太远,无法进行治疗");
    return 1;
}

forward CodeColor();
public CodeColor()
{
    for(new i=0;i<MAX_PLAYERS;i++)
    {
        if(GetPVarInt(i,"is_code_color") == 1)
        {
            DeletePVar(i,"is_code_color");
            SetPlayerColor(i,0x0D52F2C8);
        }
        if(GetPVarInt(i,"is_code_color"))
        {
            if(GetPVarInt(i,"is_code_color") == 2 || GetPVarInt(i,"is_code_color") == 4 || GetPVarInt(i,"is_code_color") == 6 || GetPVarInt(i,"is_code_color") == 8 || GetPVarInt(i,"is_code_color") == 10 || GetPVarInt(i,"is_code_color") == 12 || GetPVarInt(i,"is_code_color") == 14 || GetPVarInt(i,"is_code_color") == 18 || GetPVarInt(i,"is_code_color") == 20 || GetPVarInt(i,"is_code_color") == 22 || GetPVarInt(i,"is_code_color") == 24 || GetPVarInt(i,"is_code_color") == 26 || 
            GetPVarInt(i,"is_code_color") == 28 || GetPVarInt(i,"is_code_color") == 30)
            {
                SetPlayerColor(i,0xFFFF00C8);
            }
            else
            {
                SetPlayerColor(i,0x0D52F2C8);
            }
            SetPVarInt(i,"is_code_color",GetPVarInt(i,"is_code_color")-1);
        }
    }
    return 1;
}

COMMAND:code(playerid,params[])
{
    //code3 4 5 7 8 11 99 100
    new string[256],name[64],code[18];
    if(!GetPVarInt(playerid,"is_pd")) return RpTips(playerid,"您不是警员,无法使用Code功能");
    if(sscanf(params,"s[18]",code)) return RpTips(playerid,"发送Code代码 /code [Code代码]");
    GetPlayerName(playerid,name,sizeof(name));

    if(!strcmp(code,"2",true))
    {
        SetPVarInt(playerid,"is_code_color",30);
        SetPlayerColor(playerid,0xFF00FFC8);
        for(new i=0;i<MAX_PLAYERS;i++)
        {
            if(GetPVarInt(i,"is_pd"))
            {
                format(string,sizeof(string),"警员%s(%d)发送Code %s  - 遇常规事件,需要支援 over.",name,playerid,code);
                RpTips(i,string);
            }
        }
        return 1;
    }
    
    if(!strcmp(code,"3",true))
    {
        SetPVarInt(playerid,"is_code_color",30);
        SetPlayerColor(playerid,0xFF00FFC8);
        for(new i=0;i<MAX_PLAYERS;i++)
        {
            if(GetPVarInt(i,"is_pd"))
            {
                format(string,sizeof(string),"警员%s(%d)发送Code %s  -  遇紧急事件,需要支援 over.",name,playerid,code);
                RpTips(i,string);
            }
        }
        return 1;
    }

    if(!strcmp(code,"5",true))
    {
        SetPVarInt(playerid,"is_code_color",5);
        SetPlayerColor(playerid,0xFF00FFC8);
        for(new i=0;i<MAX_PLAYERS;i++)
        {
            if(GetPVarInt(i,"is_pd"))
            {
                format(string,sizeof(string),"警员%s(%d)发送Code %s  - 正在监视,所有单位避免靠近附近区域 over.",name,playerid,code);
                RpTips(i,string);
            }
        }
        return 1;
    }

    if(!strcmp(code,"7",true))
    {
        SetPVarInt(playerid,"is_code_color",5);
        SetPlayerColor(playerid,0xFF00FFC8);
        for(new i=0;i<MAX_PLAYERS;i++)
        {
            if(GetPVarInt(i,"is_pd"))
            {
                format(string,sizeof(string),"警员%s(%d)发送Code %s  - 用餐及休息中 over.",name,playerid,code);
                RpTips(i,string);
            }
        }
        return 1;
    }

    if(!strcmp(code,"8",true))
    {
        SetPVarInt(playerid,"is_code_color",30);
        SetPlayerColor(playerid,0xFF00FFC8);
        for(new i=0;i<MAX_PLAYERS;i++)
        {
            if(GetPVarInt(i,"is_pd"))
            {
                format(string,sizeof(string),"警员%s(%d)发送Code %s  - 遇火灾事件,需要支援 over.",name,playerid,code);
                RpTips(i,string);
            }
        }
        return 1;
    }

    if(!strcmp(code,"11",true))
    {
        SetPVarInt(playerid,"is_code_color",30);
        SetPlayerColor(playerid,0xFF00FFC8);
        for(new i=0;i<MAX_PLAYERS;i++)
        {
            if(GetPVarInt(i,"is_pd"))
            {
                format(string,sizeof(string),"警员%s(%d)发送Code %s  - 需要SWAT支援 over.",name,playerid,code);
                RpTips(i,string);
            }
        }
        return 1;
    }

    if(!strcmp(code,"99",true))
    {
        SetPVarInt(playerid,"is_code_color",30);
        SetPlayerColor(playerid,0xFF00FFC8);
        for(new i=0;i<MAX_PLAYERS;i++)
        {
            if(GetPVarInt(i,"is_pd"))
            {
                format(string,sizeof(string),"警员%s(%d)发送Code %s  - 警员遭到枪击 over.",name,playerid,code);
                RpTips(i,string);
            }
        }
        return 1;
    }


    if(!strcmp(code,"100",true))
    {
        SetPVarInt(playerid,"is_code_color",5);
        SetPlayerColor(playerid,0xFF00FFC8);
        for(new i=0;i<MAX_PLAYERS;i++)
        {
            if(GetPVarInt(i,"is_pd"))
            {
                format(string,sizeof(string),"警员%s(%d)发送Code %s  - 准备进行拦截 over.",name,playerid,code);
                RpTips(i,string);
            }
        }
        return 1;
    }

    if(!strcmp(code,"20",true))
    {
        SetPVarInt(playerid,"is_code_color",30);
        SetPlayerColor(playerid,0xFF00FFC8);
        for(new i=0;i<MAX_PLAYERS;i++)
        {
            if(GetPVarInt(i,"is_pd"))
            {
                format(string,sizeof(string),"警员%s(%d)发送Code %s  - 通知媒体 over.",name,playerid,code);
                RpTips(i,string);
            }
        }
        return 1;
    }

    if(!strcmp(code,"4",true))
    {
        SetPVarInt(playerid,"is_code_color",5);
        SetPlayerColor(playerid,0xFF00FFC8);
        for(new i=0;i<MAX_PLAYERS;i++)
        {
            if(GetPVarInt(i,"is_pd"))
            {
                format(string,sizeof(string),"警员%s(%d)发送Code %s  - 无需进一步增援 over.",name,playerid,code);
                RpTips(i,string);
            }
        }
        return 1;
    }

    if(!strcmp(code,"6",true))
    {
        SetPVarInt(playerid,"is_code_color",5);
        SetPlayerColor(playerid,0xFF00FFC8);
        for(new i=0;i<MAX_PLAYERS;i++)
        {
            if(GetPVarInt(i,"is_pd"))
            {
                format(string,sizeof(string),"警员%s(%d)发送Code %s  - 离车进行调查 over.",name,playerid,code);
                RpTips(i,string);
            }
        }
        return 1;
    }

    if(!strcmp(code,"12",true))
    {
        SetPVarInt(playerid,"is_code_color",5);
        SetPlayerColor(playerid,0xFF00FFC8);
        for(new i=0;i<MAX_PLAYERS;i++)
        {
            if(GetPVarInt(i,"is_pd"))
            {
                format(string,sizeof(string),"警员%s(%d)发送Code %s  - 虚假报警 over.",name,playerid,code);
                RpTips(i,string);
            }
        }
        return 1;
    }

    if(!strcmp(code,"10",true))
    {
        SetPVarInt(playerid,"is_code_color",5);
        SetPlayerColor(playerid,0xFF00FFC8);
        for(new i=0;i<MAX_PLAYERS;i++)
        {
            if(GetPVarInt(i,"is_pd"))
            {
                format(string,sizeof(string),"警员%s(%d)发送Code %s  - 请清除罪案广播的广播频率 over.",name,playerid,code);
                RpTips(i,string);
            }
        }
        return 1;
    }

    else
    {
        RpTips(playerid,"其他Code暂时不可用,请等待更新!");
    }

    /*
    if(!strcmp(code,"",true))
    {
        SetPVarInt(playerid,"is_code_color",30);
        SetPlayerColor(playerid,0xFF00FFC8);
        for(new i=0;i<MAX_PLAYERS;i++)
        {
            if(GetPVarInt(i,"is_pd"))
            {
                format(string,sizeof(string),"警员%s(%d)发送Code %s  - ",name,playerid,code);
                RpTips(i,string);
            }
        }
        return 1;
    }
    */
    return 1;
}

COMMAND:new(playerid,params[])
{
    new name[64],string[256],content[256],files[64];
    GetPlayerName(playerid,name,sizeof(name));
    format(files,sizeof(files),"\\Server\\RolePlay\\Adv\\%s.ini",name);
    if(!fexist(files)) return RpTips(playerid,"您没有高级扮演权限");
    if(sscanf(params,"s[256]",content)) return RpTips(playerid,"发送新闻报道: /new [新闻内容]");
    format(string,sizeof(string),"记者%s(%d)报道一则新闻:%s",name,playerid,content);
    RpTips(-1,string);
    return 1;
}

COMMAND:lz(playerid)
{
    new Float:x,Float:y,Float:z,Float:a;
    GetPlayerPos(playerid,x,y,z);
    GetPlayerFacingAngle(playerid,a);
    if(!GetPVarInt(playerid,"is_pd")) return RpTips(playerid,"只有警员才可以放置路障");
    if(rb[playerid][9] != -1) return RpTips(playerid,"您最多只能放置10个路障,请输入 /clz 清除之前放置的路障");
    for(new i=0;i<9;i++)
    {
        if(rb[playerid][i] == -1)
        {
            rb[playerid][i] = CreateObject(1459,x,y,z-0.3,0,0,a,300);
            new string[256],name[64];
            GetPlayerName(playerid,name,sizeof(name));
            format(string,sizeof(string),"警员%s(%d)放置了一个路障,位置: x:%f y:%f z:%f",name,playerid,x,y,z);
            for(new aa=0;aa<MAX_PLAYERS;aa++)
            {
                if(GetPVarInt(aa,"is_pd"))
                {
                    RpTips(i,string);
                }
            }
            return 1;
        }
    }
    return 1;
}

COMMAND:clz(playerid)
{
    if(!GetPVarInt(playerid,"is_pd")) return RpTips(playerid,"只有警员才可以放置路障");
    for(new i=0;i<9;i++)
    {
        if(rb[playerid][i] != -1)
        {
            DestroyObject(rb[playerid][i]);
            rb[playerid][i] = -1;
        }
    }
    new string[256],name[64];
    GetPlayerName(playerid,name,sizeof(name));
    format(string,sizeof(string),"警员%s(%d)清除了他放置的路障",name,playerid);
    for(new i=0;i<MAX_PLAYERS;i++)
    {
        if(GetPVarInt(i,"is_pd"))
        {
            RpTips(i,string);
        }
    }
    return 1;
}

COMMAND:unit(playerid,params[])
{
    //new PlayerVehicle[MAX_PLAYERS];
    //new PlayerVehicleText[MAX_PLAYERS];
    new id[64];
    if(sscanf(params,"s[64]",id)) return RpTips(playerid,"添加载具标识: /unit [标识]");
    if(!GetPVarInt(playerid,"is_pd") && !GetPVarInt(playerid,"is_fd")) return RpTips(playerid,"您没有权限");
    PlayerVehicleText[playerid] = Create3DTextLabel(id,0xFFFFFFFF,0.0,0.0,0.0,50.0,0,1);
    if(PlayerVehicleTextNumber[playerid] != -1)
    {
        Delete3DTextLabel(PlayerVehicleText[playerid]);
        Attach3DTextLabelToVehicle(PlayerVehicleText[playerid],PlayerVehicle[playerid],0.0,0.0,2.0);
    }
    else
    {
        PlayerVehicleTextNumber[playerid] = 1;
        Attach3DTextLabelToVehicle(PlayerVehicleText[playerid],PlayerVehicle[playerid],0.0,0.0,2.0);
    }
    RpTips(playerid,"已为您的载具添加上标识");
    return 1;
}

COMMAND:rpwarn(playerid,params[])
{
    new name[64],name1[64],reason[128],string[256],pid[18];
    new Float:x,Float:y,Float:z;
    if(!GetPVarInt(playerid,"is_login")) return ServerTips(playerid,"您还没有登录,无法进行任何操作");
    if(!IsRpAdmin(playerid)) return RpTips(playerid,"您不是RP管理员,无法进行该操作");
    if(sscanf(params,"s[18]s[128]",pid,reason)) return RpTips(playerid,"警告玩家: /rpwarn [玩家ID] [原因]");
    if(!IsPlayerConnected(strval(pid))) return RpTips(playerid,"该玩家不在线");
    if(!GetPVarInt(strval(pid),"is_rp")) return RpTips(playerid,"该玩家不在RP模式中");
    GetPlayerName(playerid,name,sizeof(name));
    GetPlayerName(strval(pid),name1,sizeof(name1));
    format(string,sizeof(string),"RP管理员%s(%d)警告玩家%s(%d),原因:%s",name,playerid,name1,strval(pid),reason);
    RpTips(-1,string);
    GetPlayerPos(strval(pid),x,y,z);
    SetPlayerPos(strval(pid),x,y,z+5);
    return 1;
}

COMMAND:rpmoshou(playerid,params[])
{
    new pid[18],name[64],name1[64],string[256];
    if(sscanf(params,"s[18]",pid)) return RpTips(playerid,"没收玩家武器:/moshou [玩家ID]");
    if(!GetPVarInt(playerid,"is_login")) return ServerTips(playerid,"您还没有登录,无法进行任何操作");
    if(!IsRpAdmin(playerid)) return RpTips(playerid,"您不是RP管理员,无法进行该操作");
    if(!GetPVarInt(strval(pid),"is_rp")) return RpTips(playerid,"该玩家不在RP模式中");
    if(!IsPlayerConnected(strval(pid))) return RpTips(playerid,"该玩家不在线");
    GetPlayerName(playerid,name,sizeof(name));
    GetPlayerName(strval(pid),name1,sizeof(name1));
    format(string,sizeof(string),"RP管理员%s(%d)没收了玩家%s(%d)的武器",name,playerid,name1,strval(pid));
    RpTips(-1,string);
    ResetPlayerWeapons(strval(pid));
    return 1;
}

COMMAND:onfire(playerid)
{
    if(!GetPVarInt(playerid,"is_rp")) return RpTips(playerid,"您不在RP模式中,无法使用该指令");
    if(GetPVarInt(playerid,"is_onfire")) return RpTips(playerid,"您正在进行范围攻击,无法重复使用");
    if(IsPlayerInAnyVehicle(playerid)) return RpTips(playerid,"在载具上无法使用范围攻击");
    SetPVarInt(playerid,"is_onfire",6);
    new string[256],name[64];
    GetPlayerName(playerid,name,sizeof(name));
    format(string,sizeof(string),"[RP系统]玩家%s(%d)进行范围攻击,持续时间5秒,在他十五米内的玩家将有几率会受到伤害",name,playerid);
    RpTips(-1,string);
    return 1;
}

forward OnFireTimer();
public OnFireTimer()
{
    for(new i=0;i<MAX_PLAYERS;i++)
    {
        if(!GetPVarInt(i,"is_onfire")) continue;
        SetPVarInt(i,"is_onfire",GetPVarInt(i,"is_onfire") - 1);
        if(GetPVarInt(i,"is_onfire") == 1)
        {
            DeletePVar(i,"is_onfire");
            RpTips(i,"范围攻击已结束");
        }
        for(new j=0;j<MAX_PLAYERS;j++)
        {
            if(i == j) continue;
            if(GetPVarInt(i,"is_rp"))
            {
                if(GetPVarInt(j,"is_rp"))
                {
                    if(GetPVarInt(i,"is_onfire"))
                    {
                        SetPVarInt(i,"is_onfire",GetPVarInt(i,"is_onfire") - 1);
                        new Float:x,Float:y,Float:z;
                        GetPlayerPos(i,x,y,z);
                        if(IsPlayerInRangeOfPoint(j,10.0,x,y,z))
                        {
                            if(random(100) %2 == 0)
                            {
                                new name[64],name1[64],string[256];
                                GetPlayerName(i,name,sizeof(name));
                                GetPlayerName(j,name1,sizeof(name1));
                                format(string,sizeof(string),"%s使用范围攻击对%s造成15点伤害",name,name1);
                                RpTips(-1,string);
                                new Float:armour,Float:health;
                                armour = GetPlayerArmour(j, armour);
                                health = GetPlayerHealth(j, health);
                                if(armour != 0)
                                {
                                    if(armour - 15 != 0)
                                    {
                                        SetPlayerArmour(j,armour - 15);
                                    }
                                    else
                                    {
                                        armour = armour - 15;
                                        SetPlayerArmour(j,0);
                                        SetPlayerHealth(j,health + armour);
                                    }
                                }
                                else
                                {
                                    SetPlayerHealth(j,health - 15);
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

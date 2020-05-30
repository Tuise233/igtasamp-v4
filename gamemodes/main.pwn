#include <a_samp>
#include <izcmd>
#include <dini>
#include <sscanf>

//IGTA Server Includings
#include "\\functions\\igta_settings.inc"
#include "\\functions\\igta_world.inc"
#include "\\functions\\igta_login.inc"
#include "\\functions\\igta_misc.inc"
#include "\\functions\\igta_vehicles.inc"
#include "\\functions\\igta_roleplay.inc"
#include "\\functions\\igta_weapons.inc"
#include "\\functions\\igta_admin.inc"
#include "\\functions\\igta_anticheat.inc"
#include "\\functions\\igta_color.inc"
#include "\\functions\\igta_vip.inc"
#include "\\functions\\igta_dm.inc"
#include "\\functions\\igta_robot.inc"

//Dialog ID Defind
#define CMain 1201
#define CName 1202
#define CScore 1203
#define CPos 1204
#define CCh 1205
#define CXwb 1206

main()
{
    SendRconCommand("hostname [憨陆小镇]国内首家GTA电脑手机跨平台互通服务器");
    SetGameModeText("憨陆小镇 v4.0");
    return 1;
}

public OnGameModeInit()
{
    print("_______憨陆小镇 v0915_______");
    print("      Powered By Tuise    ")
    print("_______憨陆小镇 v0915_______")

    //ServerVariable
    SetSVarInt("World_Time",1);
    SetWorldTime(1);
	EnableStuntBonusForAll(0);
	anticheat_OnGameModeInit();
	dm_OnGameModeInit();

    //Timer
    SetTimer("CheckLogin",500,1); //检查是否登陆，未登陆弹出提示
    SetTimer("Score",60000,1); //玩家时间分
    SetTimer("Autofix",1000,1); //载具自动修复
    SetTimer("AddComponent",3000,1); //载具添加氮气
    SetTimer("WorldTime",300000,1); //世界时间 现实5分钟=游戏1小时 
    SetTimer("AutoMessage",60000,1); //服务器随机公告 1分钟推送1次
    SetTimer("CheckServerInfo",1000,1); //服务器动态信息设置
    SetTimer("SkickCheck",1000,1); //玩家投票踢人时间循环
    SetTimer("RpMoney",60000,1); //角色扮演世界工资
    SetTimer("Wanted",1000,1); //通缉系统时间循环
    SetTimer("God",100,1); //玩家无敌模式
    SetTimer("RpJail",1000,1); //RP监狱判定
	SetTimer("Afking",500,1); //判定是否在挂机
	SetTimer("Jail",1000,1); //玩家监狱 
	SetTimer("SignInit",1000,1); //签到数据库初始化
	SetTimer("QueryServer",100,1); //判定是否查询服务器状态
	SetTimer("QueryPlayers",100,1); //判定是否查询玩家信息
	SetTimer("QueryPlayerList",100,1); //判定是否查询在线玩家
	SetTimer("QueryGMList",100,1); //判定是否查询在线玩家
	SetTimer("AntiMoney",500,1); //判定玩家是否刷钱
	SetTimer("SetPlayerCh",100,1); //设置玩家称号
	SetTimer("SetPlayerXwb",100,1); //设置玩家小尾巴
	SetTimer("CodeColor",1000,1); //设置警员Code时颜色
	SetTimer("AntiRolling",2000,1); //防刷屏
	SetTimer("AntiVehicle",1000,1); //防违禁载具
	SetTimer("OnFireTimer",1000,1); //范围攻击
	SetTimer("KickEx",1000,1); //延迟踢人
	SetTimer("SosReceive",1000,1); //线下管理员反馈
	SetTimer("RGBCar",100,1); //VIP彩虹车
    return 1;
}

public OnPlayerConnect(playerid)
{
    new name[64],string[256];
    //main.pwn
    GetPlayerName(playerid,name,sizeof(name));
    format(string,sizeof(string),"{ffffff}欢迎玩家 [{15BFEA}%s(%d){ffffff}] 进入服务器",name,playerid);
	new files[64];
	format(files,sizeof(files),"\\Players\\%s.ini",name);
	if(!fexist(files))
	{
		format(string,sizeof(string),"{ffffff}欢迎新玩家 [{15BFEA}%s(%d){ffffff}] 进入服务器",name,playerid);
	}
    ServerTips(-1,string);
    new colors[6] = {0xFF80FFC8,0xFFFF80C8,0x00FF80C8,0x008000C8,0xFFFF80C8,0xFF8040C8};
    SetPlayerColor(playerid,colors[random(6)]);
	EnableStuntBonusForPlayer(playerid, 0);//关闭特技奖励
	//SetPlayerCheckpoint(playerid,154.365493,-1940.971069,3.773437,20.0);
	

    //igta_vehicle
    SetPVarInt(playerid,"is_autofix",!GetPVarInt(playerid,"is_autofix"));
    PlayerVehicle[playerid] = -1;
	PlayerVehicleTextNumber[playerid] = -1;

	//igta_anticheat
	//AntiBot(playerid);

	//igta_admin
	//igta_admins
	new time[64];
	new year,month,day,hour,mini,sec;
	getdate(year,month,day);
	gettime(hour,mini,sec);
	format(time,sizeof(time),"%d.%d.%d %d:%d:%d",year,month,day,hour,mini,sec);
	dini_Set(files,"LoginTime",time);
	if(GetSVarInt("is_allmute"))
	{
		SetPVarInt(playerid,"is_mute",1);
	}

	//igta_vip
	vip_OnPlayerConnect(playerid);
    return 1;
}

public OnPlayerDisconnect(playerid, reason)
{
    new name[64],string[256],files[64];
    GetPlayerName(playerid,name,sizeof(name));
    format(files,sizeof(files),"\\Players\\%s.ini",name);
    format(string,sizeof(string),"{ffffff}玩家 [{FF80FF}%s(%d){ffffff}] 离开服务器",name,playerid)
    ServerTips(-1,string);
    dini_IntSet(files,"Skin",GetPlayerSkin(playerid));

    //igta_vehicle
    if(PlayerVehicle[playerid] != -1)
    {
        DestroyVehicle(PlayerVehicle[playerid]);
		Delete3DTextLabel(PlayerVehicleText[playerid]);
    }

	//igta_admins
	new time[64];
	new year,month,day,hour,mini,sec;
	getdate(year,month,day);
	gettime(hour,mini,sec);
	format(time,sizeof(time),"%d.%d.%d %d:%d:%d",year,month,day,hour,mini,sec);
	dini_Set(files,"LogoutTime",time);

	//igta_rp
	for(new i=0;i<9;i++)
    {
        DestroyObject(rb[playerid][i]);
		rb[playerid][i] = -1;
		for(new j=0;j<MAX_PLAYERS;j++)
		{
			if(GetPVarInt(j,"is_tv") == playerid + 1)
			{
				TogglePlayerSpectating(j,0);
				SetPlayerVirtualWorld(j,0);
				ServerTips(j,"由于被观察的玩家退出服务器,您已返回服务器大世界");
			}
		}
    }
    return 1;
}

public OnPlayerSpawn(playerid)
{
    new name[64],files[64],ip[64],string[256];
    GetPlayerName(playerid,name,sizeof(name));
    GetPlayerIp(playerid,ip,sizeof(ip));
    format(files,sizeof(files),"\\Players\\%s.ini",name);
    //igta_login.inc
    if(!GetPVarInt(playerid,"is_login"))
    {
        if(fexist(files))
        {
            if(!strcmp(ip,dini_Get(files,"Ip"),true))
            {
                SetPVarInt(playerid,"is_login",!GetPVarInt(playerid,"is_login"));
                ServerTips(playerid,"检测到当前IP与上次登陆IP相同,已自动登陆,欢迎回来!");
                InitLoginPlayer(playerid);
            }
            else
            {
                TogglePlayerControllable(playerid, 0);
            }
        }
        else
        {
            TogglePlayerControllable(playerid, 0);
        }
    }
    new Float:posArray[3][3]={
        {-1222.885498,44.032390,14.135156},
        {2316.307128,-1527.405517,25.343750},
        {-697.464904,952.303466,12.298262}
    }
    new posnameArray[3][256]={
        "SF机场","篮球公园","乡村小别墅"
    }
    new a = random(3);
    format(string,sizeof(string),"您复活在 - %s",posnameArray[a]);
    SetPlayerPos(playerid,posArray[a][0],posArray[a][1],posArray[a][2]); //设置重生点
    ServerTips(playerid,string);
    SetPlayerSkin(playerid,dini_Int(files,"Skin"));
	ServerTips(playerid,"输入/logs查看服务器现版本更新内容");
    return 1;
}

forward WriteChatLog(text[]);
public WriteChatLog(text[])
{
	new files[64];
	format(files,sizeof(files),"\\Server\\Chatlog.txt");
	new File:fl = fopen(files,io_append);
	for(new i=0;i<strlen(text);i++)
	{
		fputchar(fl, text[i], false);
	}
	fclose(fl);
	return 1;
}

forward AntiRolling();
public AntiRolling()
{
	for(new i=0;i<MAX_PLAYERS;i++)
	{
		if(GetPVarInt(i,"is_rolling"))
		{
			SetPVarInt(i,"is_rolling",1);
		}
	}
	return 1;
}

public OnPlayerText(playerid, text[])
{
	new chatlog[256],time[64],name[64];
	new year,month,day,hour,mini,sec;
	GetPlayerName(playerid,name,sizeof(name));
	getdate(year,month,day);
	gettime(hour,mini,sec);
	format(time,sizeof(time),"[%d/%d/%d %d:%d:%d]",year,month,day,hour,mini,sec);
	if(!GetPVarInt(playerid,"is_login"))
	{
		ServerTips(playerid,"您还没有登录,无法进行任何操作");
		format(chatlog,sizeof(chatlog),"%s%s(%d):%s_UnLogin\n",time,name,playerid,text);
		WriteChatLog(chatlog);
		return 0;
	}
    new string[1024],files[64];
    new color = GetPlayerColor(playerid);
    format(files,sizeof(files),"\\Players\\%s.ini",name);
	if(GetPVarInt(playerid,"is_mute")){
	    AdminTips(playerid,"您已被管理员禁言,请联系管理员解除禁言");
		format(chatlog,sizeof(chatlog),"%s%s(%d):%s_Muted\n",time,name,playerid,text);
		WriteChatLog(chatlog);
		return 0;
	}
	if(GetPVarInt(playerid,"is_mute")) return 1;
	if(!GetPVarInt(playerid,"is_rolling"))
	{
		SetPVarInt(playerid,"is_rolling",1);
	}
	else
	{
	    if(GetPVarInt(playerid,"is_rolling") == 3)
	    {
			SendClientMessage(playerid,COLOR_RED,"[T-AntiCheat]您讲话的速度太快了,请放慢语速重新发送");
			return 0;
		}
		else
		{
            SetPVarInt(playerid,"is_rolling",GetPVarInt(playerid,"is_rolling")+1);
		}
	}
    if(!strcmp(dini_Get(files,"Ch"),"none",true))
    {
        if(!strcmp(dini_Get(files,"Xwb"),"none",true))
        {
            format(string,sizeof(string),"%s(%d):{ffffff}%s",name,playerid,text);
        }
        else
        {
            format(string,sizeof(string),"%s(%d):{ffffff}%s   {FF80C0}%s",name,playerid,text,dini_Get(files,"Xwb"));
        }
    }
    else
    {
        if(!strcmp(dini_Get(files,"Xwb"),"none",true))
        {
            format(string,sizeof(string),"[%s]%s(%d):%s",dini_Get(files,"Ch"),name,playerid,text);
        }
        else
        {
            format(string,sizeof(string),"[%s]%s(%d):%s   {FF80C0}%s",dini_Get(files,"Ch"),name,playerid,text,dini_Get(files,"Xwb"));
        }
    }
	if(GetPVarInt(playerid,"is_rp")){
		format(string,sizeof(string),"%s(%d):{ffffff}%s",name,playerid,text);
		ProxDetectorRP(13.0,playerid,string);
		format(chatlog,sizeof(chatlog),"%s%s(%d):%s_RolePlay\n",time,name,playerid,text);
		WriteChatLog(chatlog);
		return 0;
	}
    for(new i=0;i<MAX_PLAYERS;i++)
    {
        if(GetPlayerVirtualWorld(playerid) == GetPlayerVirtualWorld(i))
        {
            SendClientMessage(i,color,string);
			format(chatlog,sizeof(chatlog),"%s%s(%d):%s_Global\n",time,name,playerid,text);	
        }
    }
	printf("%s(%d):%s",name,playerid,text);
	WriteChatLog(chatlog);
	new filess[64];
	format(filess,sizeof(filess),"\\Robot\\ChatLive.txt");
	format(string,sizeof(string),"%s(%d):%s",name,playerid,text);
	new File:fl = fopen(filess,io_write);
	for(new i=0;string[i];i++)
	{
		fputchar(fl,string[i],false);
	}
	fclose(fl);
    return 0;
}

public OnPlayerEnterVehicle(playerid, vehicleid, ispassenger)
{
    new name[64],string[256],name1[64];
	if(GetPVarInt(playerid,"is_nocar")){
		RemovePlayerFromVehicle(playerid);
		AdminTips(playerid,"您已被禁止进入载具,请联系管理员进行解封");
		return 1;
	}
    if(!GetPVarInt(playerid,"is_rp")){
        GetPlayerName(GetVehicleHost(vehicleid),name,sizeof(name));
        GetPlayerName(playerid,name1,sizeof(name1));
        format(string,sizeof(string),"您进入了%s(%d)生成的载具,载具ID:%d",name,GetVehicleHost(vehicleid),GetVehicleModel(vehicleid));
        if(vehicleid != PlayerVehicle[playerid]){
            if(GetPVarInt(GetVehicleHost(vehicleid),"is_car_lock")){
                RemovePlayerFromVehicle(playerid);
                format(string,sizeof(string),"车主%s(%d)已将该载具上锁",name,GetVehicleHost(vehicleid));
                ServerTips(playerid,string);
                return 1;
            }
            ServerTips(playerid,string);
            format(string,sizeof(string),"玩家%s(%d)进入了您的专属载具",name1,playerid);
            ServerTips(GetVehicleHost(vehicleid),string);
            return 1;
        }
        else{
            format(string,sizeof(string),"您已进入您的专属载具,载具ID:%d",GetVehicleModel(vehicleid));
            ServerTips(playerid,string);
        }
    }
    return 1;
}

public  OnPlayerStateChange(playerid, newstate, oldstate)
{
    if(newstate == PLAYER_STATE_ONFOOT )
    {
        for(new i=0;i<MAX_PLAYERS;i++)
        {
            if(GetPVarInt(i,"is_tv") == playerid+1)
            {
                PlayerSpectatePlayer(i, playerid);
            }
        }

        if(newstate == PLAYER_STATE_DRIVER  || newstate == PLAYER_STATE_PASSENGER)
        {
            for(new i=0;i<MAX_PLAYERS;i++)
            {
                if(GetPVarInt(i,"is_tv") == playerid+1)
                {
                    PlayerSpectateVehicle(i,GetPlayerVehicleID(playerid));
                }
            }
        }
        return 1;
    }

    return 1;
}

public OnPlayerDeath(playerid, killerid, reason)
{
    SendDeathMessage(killerid, playerid, reason);
	if(GetPVarInt(playerid,"is_rp"))
	{
		if(GetPlayerMoney(playerid) < 500)
		{
			GivePlayerRpMoney(playerid,-GetPlayerMoney(playerid));
			RpTips(playerid,"您在医院治疗需要500美元,由于您的余额不足,您已破产");
			new name[64],string[256];
			GetPlayerName(playerid,name,sizeof(name));
			format(string,sizeof(string),"玩家%s(%d)破产了",name,playerid);
			RpTips(-1,string);
			return 1;
		}
		else
		{
			RpTips(playerid,"您在医院治疗花费了500美元");
			GivePlayerRpMoney(playerid,-400);
			return 1;
		}
	}
    return 1;
}

public OnPlayerClickPlayer(playerid, clickedplayerid, source)
{
    new name[64],style[1024],ip[64],files[64];
    new Float:x,Float:y,Float:z;
    GetPlayerName(clickedplayerid,name,sizeof(name));
    GetPlayerIp(clickedplayerid,ip,sizeof(ip));
    GetPlayerPos(clickedplayerid,x,y,z);
    format(files,sizeof(files),"\\Players\\%s.ini",name);
    if(IsPlayerAdmin(playerid)){
        format(style,sizeof(style),"玩家昵称:%s\n玩家ID:%d\n玩家时间分:%d\n玩家IP:%s\n玩家坐标:%f,%f,%f\n玩家称号:%s\n玩家小尾巴:%s",name,clickedplayerid,GetPlayerScore(clickedplayerid),ip,x,y,z,dini_Get(files,"Ch"),dini_Get(files,"Xwb"));
        ShowPlayerDialog(playerid,CMain,DIALOG_STYLE_LIST,"玩家信息",style,"确定","取消");
        SetPVarInt(playerid,"is_clickedid",clickedplayerid+1);
        return 1;
    }
    return 1;
}

public OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
    new string[256],name[64],name1[64],files[64];
    new Float:x,Float:y,Float:z;
    GetPlayerName(playerid,name,sizeof(name));
    if(dialogid == CMain){
        if(response){
            switch(listitem){
                //CName CScore CPos CCh CXwb
                case 0: ShowPlayerDialog(playerid, CName, DIALOG_STYLE_INPUT, "修改昵称", "修改该玩家的昵称","修改", "取消");
                case 2: ShowPlayerDialog(playerid, CScore, DIALOG_STYLE_INPUT, "修改时间分", "修改该玩家的时间分","修改", "取消");
                case 4: ShowPlayerDialog(playerid, CPos, DIALOG_STYLE_MSGBOX, "传送至玩家附近", "您确定要传送至该玩家附近","确定", "取消");
                case 5: ShowPlayerDialog(playerid, CCh, DIALOG_STYLE_INPUT, "修改称号", "修改该玩家的称号","修改", "取消");
                case 6: ShowPlayerDialog(playerid, CXwb, DIALOG_STYLE_INPUT, "修改小尾巴", "修改该玩家的小尾巴","修改", "取消");
            }
        }
        else{
            DeletePVar(playerid, "is_clickedid");
        }
    }

    if(dialogid == CName){
        GetPlayerName(GetPVarInt(playerid,"is_clickedid")-1,name1,sizeof(name1));
        SetPlayerName(GetPVarInt(playerid,"is_clickedid")-1,inputtext);
        format(string,sizeof(string),"[T-Admin]管理员%s(%d)修改玩家%s(%d)昵称为:%s",name,playerid,name1,GetPVarInt(playerid,"is_clickedid")-1,inputtext);
        SendClientMessageToAll(COLOR_RED, string);
        DeletePVar(playerid, "is_clickedid");
        return 1;
    }

    if(dialogid == CScore){
        SetPlayerScore(GetPVarInt(playerid,"is_clickedid")-1, strval(inputtext))
        GetPlayerName(GetPVarInt(playerid,"is_clickedid")-1,name1,sizeof(name1));
        format(string,sizeof(string),"[T-Admin]管理员%s(%d)修改玩家%s(%d)的时间分为:%d分",name,playerid,name1,GetPVarInt(playerid,"is_clickedid")-1,strval(inputtext));
        SendClientMessageToAll(COLOR_RED, string);
        DeletePVar(playerid, "is_clickedid");
        return 1;
    }

    if(dialogid == CPos){
        GetPlayerPos(GetPVarInt(playerid,"is_clickedid")-1,x,y,z);
        SetPlayerPos(playerid,x+3,y+3,z);
        SendClientMessage(playerid,COLOR_RED,"[T-Admin]传送成功");
        DeletePVar(playerid, "is_clickedid");
        return 1;
    }

    if(dialogid == CCh){
        GetPlayerName(GetPVarInt(playerid,"is_clickedid")-1,name1,sizeof(name1));
        format(files,sizeof(files),"\\Players\\%s.ini",name1);
        dini_Set(files,"Ch",inputtext);
        format(string,sizeof(string),"[T-Admin]管理员%s(%d)修改玩家%s(%d)的称号为:%s",name,playerid,name1,GetPVarInt(playerid,"is_clickedid")-1,inputtext);
        SendClientMessageToAll(COLOR_RED,string);
        DeletePVar(playerid, "is_clickedid");
        return 1;
    }

    if(dialogid == CXwb){
        GetPlayerName(GetPVarInt(playerid,"is_clickedid")-1,name1,sizeof(name1));
        format(files,sizeof(files),"\\Players\\%s.ini",name1);
        dini_Set(files,"Xwb",inputtext);
        format(string,sizeof(string),"[T-Admin]管理员%s(%d)修改玩家%s(%d)的小尾巴为:%s",name,playerid,name1,GetPVarInt(playerid,"is_clickedid")-1,inputtext);
        SendClientMessageToAll(COLOR_RED,string);
        DeletePVar(playerid, "is_clickedid");
        return 1;
    }

    //武器系统
	if(dialogid == 3100)
	{
	    if(response)
	    {
			switch(listitem)
			{
				case 0:ShowPlayerDialog(playerid,3101,DIALOG_STYLE_LIST,"手持类","指虎\n高尔夫\n警棍\n小刀\n棒球棍\n铁铲\n台球杆\n武士刀\n电锯\n花\n拐杖","确定","取消");
				case 1:ShowPlayerDialog(playerid,3102,DIALOG_STYLE_LIST,"投掷类","手雷\n催泪弹\n燃烧弹","确定","取消");
				case 2:ShowPlayerDialog(playerid,3103,DIALOG_STYLE_LIST,"手枪类","双持手枪\n消音手枪\n沙漠之鹰","确定","取消");
				case 3:ShowPlayerDialog(playerid,3104,DIALOG_STYLE_LIST,"冲锋枪","Tec9\nUZI\nMP5","确定","取消");
				case 4:ShowPlayerDialog(playerid,3105,DIALOG_STYLE_LIST,"散弹枪","普通散弹枪\n双持散弹枪\n七连发散弹枪","确定","取消");
				case 5:ShowPlayerDialog(playerid,3106,DIALOG_STYLE_LIST,"步枪类","AK47\nM4A1\n精准射手步枪\n狙击步枪","确定","取消");
				case 6:ShowPlayerDialog(playerid,3107,DIALOG_STYLE_LIST,"特殊类","火箭筒\n热追踪火箭筒\n火焰喷射器\n加特林机关枪\nC4炸药","确定","取消");
				case 7:ShowPlayerDialog(playerid,3108,DIALOG_STYLE_LIST,"其他类","喷漆\n照相机\n灭火器\n降落伞","确定","取消");
				case 8:ShowPlayerDialog(playerid,3109,DIALOG_STYLE_LIST,"血甲类","血量补满\n护甲补满\n血量和护甲补满","确定","取消");
			}
		}
	}

	if(dialogid == 3101)
	{
	    if(response)
	    {
	        switch(listitem)
	        {
				//指虎\n高尔夫\n警棍\n小刀\n棒球棍\n铁铲\n台球杆\n武士刀\n电锯\n花\n拐杖
				case 0:
				{
					GivePlayerWeapon(playerid,1,9999);
				    ServerTips(playerid,"[服务器]您已生成指虎");
				}
				case 1:
				{
					GivePlayerWeapon(playerid,2,9999);
				    ServerTips(playerid,"[服务器]您已生成高尔夫");
				}
				case 2:
				{
					GivePlayerWeapon(playerid,3,9999);
				    ServerTips(playerid,"[服务器]您已生成警棍");
				}
				case 3:
				{
					GivePlayerWeapon(playerid,4,9999);
				    ServerTips(playerid,"[服务器]您已生成小刀");
				}
				case 4:
				{
					GivePlayerWeapon(playerid,5,9999);
				    ServerTips(playerid,"[服务器]您已生成棒球棍");
				}
				case 5:
				{
					GivePlayerWeapon(playerid,6,9999);
				    ServerTips(playerid,"[服务器]您已生成铁铲");
				}
				case 6:
				{
					GivePlayerWeapon(playerid,7,9999);
				    ServerTips(playerid,"[服务器]您已生成台球杆");
				}
				case 7:
				{
					GivePlayerWeapon(playerid,8,9999);
				    ServerTips(playerid,"[服务器]您已生成武士刀");
				}
				case 8:
				{
					GivePlayerWeapon(playerid,9,9999);
				    ServerTips(playerid,"[服务器]您已生成电锯");
				}
				case 9:
				{
					GivePlayerWeapon(playerid,14,9999);
				    ServerTips(playerid,"[服务器]您已生成花");
				}
				case 10:
				{
					GivePlayerWeapon(playerid,15,9999);
				    ServerTips(playerid,"[服务器]您已生成拐杖");
				}
	        }
		 }
	}
	if(dialogid == 3102)
	{
	    if(response)
	    {
	        switch(listitem)
	        {
				//[服务器]您已生成
	            case 0:
	            {
					GivePlayerWeapon(playerid,16,9999);
				    ServerTips(playerid,"[服务器]您已生成手雷");
				}
				case 1:
				{
					GivePlayerWeapon(playerid,17,9999);
				    ServerTips(playerid,"[服务器]您已生成催泪弹");
				}
				case 2:
				{
					GivePlayerWeapon(playerid,18,9999);
				    ServerTips(playerid,"[服务器]您已生成燃烧弹");
				}
	        }
		}
	}
	if(dialogid == 3103)
	{
	    if(response)
	    {
			switch(listitem)
			{
				//双持手枪\n消音手枪\n沙漠之鹰
			    case 0:
			    {
					GivePlayerWeapon(playerid,22,9999);
				    ServerTips(playerid,"[服务器]您已生成双持手枪");
				}
				case 1:
				{
					GivePlayerWeapon(playerid,23,9999);
				    ServerTips(playerid,"[服务器]您已生成消音手枪");
				}
				case 2:
				{
					GivePlayerWeapon(playerid,24,9999);
				    ServerTips(playerid,"[服务器]您已生成沙漠之鹰");
				}
			}
		}
	}
	if(dialogid == 3104)
	{
	    if(response)
	    {
	        switch(listitem)
	        {
				//Tec9\nUZI\nMP5 32 28 29
				case 0:
				{
					GivePlayerWeapon(playerid,32,9999);
				    ServerTips(playerid,"[服务器]您已生成Tec9");
				}
				case 1:
				{
					GivePlayerWeapon(playerid,28,9999);
				    ServerTips(playerid,"[服务器]您已生成Uzi");
				}
				case 2:
				{
					GivePlayerWeapon(playerid,29,9999);
				    ServerTips(playerid,"[服务器]您已生成MP5");
				}
			}
		}
	}
	if(dialogid == 3105)
	{
		if(response)
		{
		    switch(listitem)
			{
			    //普通散弹枪\n双持散弹枪\n七连发散弹枪
				case 0:
				{
					GivePlayerWeapon(playerid,25,9999);
				    ServerTips(playerid,"[服务器]您已生成普通散弹枪");
				}
				case 1:
				{
					GivePlayerWeapon(playerid,26,9999);
				    ServerTips(playerid,"[服务器]您已生成双持散弹枪");
				}
				case 2:
				{
					GivePlayerWeapon(playerid,27,9999);
				    ServerTips(playerid,"[服务器]您已生成七连发散弹枪");
				}
			}
		}
	}
	if(dialogid == 3106)
	{
	    if(response)
	    {
	        switch(listitem)
	        {
	            //AK47\nM4A1\n精准射手步枪\n狙击步枪
	            case 0:
	            {
					GivePlayerWeapon(playerid,30,9999);
				    ServerTips(playerid,"[服务器]您已生成AK47"); 
				}
				case 1:
				{
					GivePlayerWeapon(playerid,31,9999);
				    ServerTips(playerid,"[服务器]您已生成M4A1");
				}
				case 2:
				{
					GivePlayerWeapon(playerid,33,9999);
				    ServerTips(playerid,"[服务器]您已生成精准射手步枪");
				}
				case 3:
				{
					GivePlayerWeapon(playerid,34,9999);
				    ServerTips(playerid,"[服务器]您已生成狙击步枪");
				}
	        }
		}
	}
	if(dialogid == 3107)
	{
	    if(response)
	    {
	        switch(listitem)
	        {
         		//火箭筒\n热追踪火箭筒\n火焰喷射器\n加特林机关枪\nC4炸药
				case 0:
				{
					GivePlayerWeapon(playerid,35,9999);
				    ServerTips(playerid,"[服务器]您已生成火箭筒");
				}
				case 1:
				{
					GivePlayerWeapon(playerid,36,9999);
				    ServerTips(playerid,"[服务器]您已生成热追踪火箭筒");
				}
				case 2:
				{
					GivePlayerWeapon(playerid,37,9999);
				    ServerTips(playerid,"[服务器]您已生成火焰喷射器");
				}
				case 3:
				{
					GivePlayerWeapon(playerid,38,9999);
				    ServerTips(playerid,"[服务器]您已生成加特林机关枪");
				}
				case 4:
				{
					GivePlayerWeapon(playerid,40,9999);
					GivePlayerWeapon(playerid,39,9999);
				    ServerTips(playerid,"[服务器]您已生成C4炸药");
				}

	        }
		}
	}
	if(dialogid == 3108)
	{
		if(response)
		{
		    switch(listitem)
		    {
		        //喷漆\n照相机\n灭火器\n降落伞
		        case 0:
		        {
					GivePlayerWeapon(playerid,41,9999);
				    ServerTips(playerid,"[服务器]您已生成喷漆");
				}
				case 1:
				{
					GivePlayerWeapon(playerid,43,9999);
				    ServerTips(playerid,"[服务器]您已生成照相机");
				}
				case 2:
				{
					GivePlayerWeapon(playerid,42,9999);
				    ServerTips(playerid,"[服务器]您已生成灭火器");
				}
				case 3:
				{
					GivePlayerWeapon(playerid,46,9999);
				    ServerTips(playerid,"[服务器]您已生成降落伞");
				}
		    }
		}
	}
	if(dialogid == 3109)
	{
	    if(response)
	    {
	        switch(listitem)
	        {
				//血量补满\n护甲补满\n血量和护甲补满
	            case 0:
	            {	
					SetPlayerHealth(playerid,100);
	                ServerTips(playerid,"[服务器]您已补满您的血量");
	            }
	            case 1:
				{
				    SetPlayerArmour(playerid,100);
				    ServerTips(playerid,"[服务器]您已补满您的护甲");
				}
				case 2:
				{
				    SetPlayerHealth(playerid,100);
				    SetPlayerArmour(playerid,100);
				    ServerTips(playerid,"[服务器]您已补满您的血量和护甲");
				}
	        }
		}
	}
    return 1;
}

public OnPlayerEnterRaceCheckpoint(playerid)
{
    if(GetPVarInt(playerid,"is_rp")){
        if(GetPVarInt(playerid,"is_rp_taxi_ing")){
            DisablePlayerRaceCheckpoint(playerid);
            RpTips(playerid,"您已到达目的地,请四周环顾寻找您的顾客");
            RpTips(playerid,"若您的顾客不在附近,您可以输入 /cjd 取消本次的接送订单");
            return 1;
        }
    }
    return 1;
}

public OnPlayerClickMap(playerid, Float:fX, Float:fY, Float:fZ)
{
    if(GetPVarInt(playerid,"is_mtp")){
        SetPlayerPos(playerid,fX,fY,fZ);
        ServerTips(playerid,"已将您传送至地图标记点");
        ServerTips(playerid,"关闭或开启地图传送功能 /mtp");
    }
    return 1;
}

public OnPlayerGiveDamage(playerid, damagedid, Float:amount, weaponid, bodypart)
{
    new name[64],name1[64],string[256];
    if(GetPVarInt(playerid,"is_rp")){
        if(GetPVarInt(playerid,"is_pd")){
            if(GetPVarInt(damagedid,"is_wanted_level")){
                if(weaponid == 0){
                    SetPVarInt(damagedid,"is_wanted_jail",GetPVarInt(damagedid,"is_wanted_count"));
                    DeletePVar(damagedid,"is_wanted_count");
                    DeletePVar(damagedid,"is_wanted_level");
                    GetPlayerName(playerid,name,sizeof(name));
                    GetPlayerName(damagedid,name1,sizeof(name1));
                    format(string,sizeof(string),"警员%s(%d)成功抓捕嫌犯%s(%d),入狱%d秒处置",name,playerid,name1,damagedid,GetPVarInt(damagedid,"is_wanted_jail"));
                    RpTips(-1,string);
					SetPlayerWantedLevel(damagedid,0);
                }
            }
        }

		/*
		GetPlayerName(damagedid,name,sizeof(name));
		GetPlayerName(playerid,name1,sizeof(name1));
		new Float:heal;
		GetPlayerHealth(damagedid, heal);
		heal = heal - amount;
		SetPlayerHealth(damagedid,heal);
		format(string,sizeof(string),"[RP模式]玩家%s(%d)对玩家%s(%d)造成伤害%fHP [武器:%d 命中部位:%d]",name1,playerid,name,damagedid,amount,weaponid,bodypart);
		ProxDetectorRPTips(10,playerid,string);
		*/
    }
    return 0;
}


public OnPlayerTakeDamage(playerid, issuerid, Float:amount, weaponid, bodypart)
{
	dm_OnPlayerTakeDamage(playerid, issuerid, amount, weaponid, bodypart);
    if(GetPVarInt(playerid,"is_god")){
        SetPlayerHealth(playerid,9999);
    }
	new name[64],name1[64],string[256];
    if(GetPVarInt(issuerid,"is_rp")){
        if(GetPVarInt(issuerid,"is_pd")){
            if(GetPVarInt(playerid,"is_wanted_level")){
                if(weaponid == 0){
                    SetPVarInt(playerid,"is_wanted_jail",GetPVarInt(playerid,"is_wanted_count"));
                    DeletePVar(playerid,"is_wanted_count");
                    DeletePVar(playerid,"is_wanted_level");
                    GetPlayerName(issuerid,name,sizeof(name));
                    GetPlayerName(playerid,name1,sizeof(name1));
                    format(string,sizeof(string),"警员%s(%d)成功抓捕嫌犯%s(%d),入狱%d秒处置",name,issuerid,name1,playerid,GetPVarInt(playerid,"is_wanted_jail"));
                    RpTips(-1,string);
					SetPlayerWantedLevel(playerid,0);
                }
            }
        }

		//玩家被爆头
		if(weaponid  == 0 && bodypart == 9)
		{
			GetPlayerName(playerid,name,sizeof(name));
			GetPlayerName(issuerid,name1,sizeof(name1));
			if(GetPlayerWeapon(playerid) == 0)
			{
				return 1;
			}
			format(string,sizeof(string),"玩家%s(%d)用武器击中玩家%s(%d)的头部,玩家%s(%d)当场死亡",issuerid,name1,name,playerid,name,playerid);
			RpTips(-1,string);SetPlayerHealth(playerid,0);
		}
		/*
		GetPlayerName(playerid,name,sizeof(name));
		GetPlayerName(issuerid,name1,sizeof(name1));
		new Float:heal;
		GetPlayerHealth(playerid, heal);
		heal = heal - amount;
		SetPlayerHealth(playerid,heal);
		format(string,sizeof(string),"[RP模式]玩家%s(%d)对玩家%s(%d)造成伤害%fHP [武器:%d 命中部位:%d]",name1,issuerid,name,playerid,amount,weaponid,bodypart);
		ProxDetectorRPTips(10,playerid,string);
		*/
    }
    return 0;
}

public OnPlayerUpdate(playerid)
{
	anticheat_OnPlayerUpdate(playerid);
	return 1;
}
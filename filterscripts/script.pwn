//定制脚本 By IGTA-Tuise
#include <a_samp>
#include <izcmd>
#include <sscanf>
#include <dini>
#include <igta_color>

new PlayerVehicle[MAX_PLAYERS];

public OnFilterScriptInit()
{
    printf("Script Loaded!");
    SetTimer("AddComponent",5000,1);
    SetTimer("AddScore",3600000,1);
    return 1;
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

public OnPlayerText(playerid, text[])
{
    new name[64],string[256];
    GetPlayerName(playerid,name,sizeof(name));
    format(string,sizeof(string),"[附近]%s(%d):%s",name,playerid,text);
    ProxDetector(50.0,playerid,string);
    return 0;
}

public OnPlayerConnect(playerid)
{
    for(new i=0;i<10;i++)
    {
        SendClientMessage(playerid,-1," ");
    }
    new colors[6] = {0xFF80FFC8,0xFFFF80C8,0x00FF80C8,0x008000C8,0xFFFF80C8,0xFF8040C8};
    SetPlayerColor(playerid,colors[random(6)]);
    PlayerVehicle[playerid] = -1;

	new name[64],string[256];
	GetPlayerName(playerid,name,sizeof(name));
	format(string,sizeof(string),"[服务器]玩家%s(%d)进入服务器",name,playerid);
	SendClientMessageToAll(COLOR_RED,string);
}

public OnPlayerDisconnect(playerid, reason)
{
    if(PlayerVehicle[playerid] != -1)
    {
        DestroyVehicle(PlayerVehicle[playerid]);
    }
    new Float:x,Float:y,Float:z;
    GetPlayerPos(playerid,x,y,z);
    new name[64],files[64];
    GetPlayerName(playerid,name,sizeof(name));
    format(files,sizeof(files),"\\Players\\%s.ini",name);
    dini_FloatSet(files,"pos_X",x);
    dini_FloatSet(files,"pos_Y",y);
    dini_FloatSet(files,"pos_Z",z);
    dini_IntSet(files,"Skin",GetPlayerSkin(playerid));
    dini_IntSet(files,"Score",GetPlayerScore(playerid));
}

public OnPlayerSpawn(playerid)
{
    SetPlayerPos(playerid,-316.446716,1510.098632,75.562500);
    SetPlayerArmour(playerid,100);
    SetPlayerHealth(playerid,100);
    if(!GetPVarInt(playerid,"is_login"))
    {
        new name[64],files[64];
	    GetPlayerName(playerid,name,sizeof(name));
	    format(files,sizeof(files),"\\Players\\%s.ini");
	    SetPlayerScore(playerid,dini_Int(files,"Score"));
	    SetPlayerSkin(playerid,dini_Int(files,"Skin"));
	    SetPlayerPos(playerid,dini_Float(files,"pos_X"),dini_Float(files,"pos_Y"),dini_Float(files,"pos_Z"));
		return 1;
    }
    return 1;
}

COMMAND:ppos(playerid)
{
    new Float:x,Float:y,Float:z;
    GetPlayerPos(playerid,x,y,z);
    printf("%f,%f,%f",x,y,z);
}

public OnPlayerRequestClass(playerid, classid)
{
    SpawnPlayer(playerid);
    new name[64],files[64];
    GetPlayerName(playerid,name,sizeof(name));
    format(files,sizeof(files),"\\Players\\%.s.ini",name);
    if(!fexist(files))
    {
        if(!GetPVarInt(playerid,"is_login"))
        {
            ShowPlayerDialog(playerid,1000,DIALOG_STYLE_INPUT,"注册系统","欢迎来到服务器,请输入密码进行注册","确定","");
        }
    }
    else
    {
        if(!GetPVarInt(playerid,"is_login"))
        {
            ShowPlayerDialog(playerid,1001,DIALOG_STYLE_INPUT,"登录系统","欢迎回来,请输入密码进行登录","登录","取消");
        }
    }
}

public OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
    new name[64],files[64],string[256];
    GetPlayerName(playerid,name,sizeof(name));
    format(files,sizeof(files),"\\Players\\%s.ini",name);
    //注册对话框
    if(dialogid == 1000)
    {
        if(response)
        {
            if(!strlen(inputtext))
            {
                SendClientMessage(playerid,-1,"[登录系统]您输入的密码不能为空");
                ShowPlayerDialog(playerid,1000,DIALOG_STYLE_INPUT,"注册系统","欢迎来到服务器,请输入密码进行注册","确定","");
            }
            else
            {
                dini_Create(files);
                dini_Set(files,"Name",name);
                dini_Set(files,"Password",inputtext);
                format(string,sizeof(string),"[注册系统]注册完毕,您的密码是:%s,下次登录时要使用到的哦~",inputtext);
                SendClientMessage(playerid,COLOR_RED,string);
                SetPVarInt(playerid,"is_login",!GetPVarInt(playerid,"is_login"));
                SpawnPlayer(playerid);
            }
        }
        else
        {
            Kick(playerid);
        }
    }

    //登录对话框
    if(dialogid == 1001)
    {
        if(response)
        {
            if(!strlen(inputtext))
            {
                SendClientMessage(playerid,-1,"[登录系统]您输入的密码不能为空");
                ShowPlayerDialog(playerid,1001,DIALOG_STYLE_INPUT,"登录系统","欢迎回来,请输入密码进行登录","登录","取消");
            }
            else
            {
                new pwd[128];
                format(pwd,sizeof(pwd),"%s",dini_Get(files,"Password"));
                if(!strcmp(inputtext,pwd,false))
                {
                    SendClientMessage(playerid,-1,"[登录系统]登陆成功,欢迎回来!");
                    SetPVarInt(playerid,"is_login",!GetPVarInt(playerid,"is_login"));
                    SetPlayerSkin(playerid,dini_Int(files,"Skin"));
                    SetPlayerPos(playerid,dini_Float(files,"pos_X"),dini_Float(files,"pos_Y"),dini_Float(files,"pos_Z"));
                    SpawnPlayer(playerid);
                }
                else
                {
                    SendClientMessage(playerid,-1,"[登录系统]您输入的密码有无,请核对后重新输入");
                    ShowPlayerDialog(playerid,1001,DIALOG_STYLE_INPUT,"登录系统","欢迎回来,请输入密码进行登录","登录","取消");
                }
            }
        }
        else
        {
            Kick(playerid);
        }
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
				    SendClientMessage(playerid,-1,"[服务器]您已生成指虎");
				}
				case 1:
				{
					GivePlayerWeapon(playerid,2,9999);
				    SendClientMessage(playerid,-1,"[服务器]您已生成高尔夫");
				}
				case 2:
				{
					GivePlayerWeapon(playerid,3,9999);
				    SendClientMessage(playerid,-1,"[服务器]您已生成警棍");
				}
				case 3:
				{
					GivePlayerWeapon(playerid,4,9999);
				    SendClientMessage(playerid,-1,"[服务器]您已生成小刀");
				}
				case 4:
				{
					GivePlayerWeapon(playerid,5,9999);
				    SendClientMessage(playerid,-1,"[服务器]您已生成棒球棍");
				}
				case 5:
				{
					GivePlayerWeapon(playerid,6,9999);
				    SendClientMessage(playerid,-1,"[服务器]您已生成铁铲");
				}
				case 6:
				{
					GivePlayerWeapon(playerid,7,9999);
				    SendClientMessage(playerid,-1,"[服务器]您已生成台球杆");
				}
				case 7:
				{
					GivePlayerWeapon(playerid,8,9999);
				    SendClientMessage(playerid,-1,"[服务器]您已生成武士刀");
				}
				case 8:
				{
					GivePlayerWeapon(playerid,9,9999);
				    SendClientMessage(playerid,-1,"[服务器]您已生成电锯");
				}
				case 9:
				{
					GivePlayerWeapon(playerid,14,9999);
				    SendClientMessage(playerid,-1,"[服务器]您已生成花");
				}
				case 10:
				{
					GivePlayerWeapon(playerid,15,9999);
				    SendClientMessage(playerid,-1,"[服务器]您已生成拐杖");
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
				    SendClientMessage(playerid,-1,"[服务器]您已生成手雷");
				}
				case 1:
				{
					GivePlayerWeapon(playerid,17,9999);
				    SendClientMessage(playerid,-1,"[服务器]您已生成催泪弹");
				}
				case 2:
				{
					GivePlayerWeapon(playerid,18,9999);
				    SendClientMessage(playerid,-1,"[服务器]您已生成燃烧弹");
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
				    SendClientMessage(playerid,-1,"[服务器]您已生成双持手枪");
				}
				case 1:
				{
					GivePlayerWeapon(playerid,23,9999);
				    SendClientMessage(playerid,-1,"[服务器]您已生成消音手枪");
				}
				case 2:
				{
					GivePlayerWeapon(playerid,24,9999);
				    SendClientMessage(playerid,-1,"[服务器]您已生成沙漠之鹰");
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
				    SendClientMessage(playerid,-1,"[服务器]您已生成Tec9");
				}
				case 1:
				{
					GivePlayerWeapon(playerid,28,9999);
				    SendClientMessage(playerid,-1,"[服务器]您已生成Uzi");
				}
				case 2:
				{
					GivePlayerWeapon(playerid,29,9999);
				    SendClientMessage(playerid,-1,"[服务器]您已生成MP5");
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
				    SendClientMessage(playerid,-1,"[服务器]您已生成普通散弹枪");
				}
				case 1:
				{
					GivePlayerWeapon(playerid,26,9999);
				    SendClientMessage(playerid,-1,"[服务器]您已生成双持散弹枪");
				}
				case 2:
				{
					GivePlayerWeapon(playerid,27,9999);
				    SendClientMessage(playerid,-1,"[服务器]您已生成七连发散弹枪");
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
				    SendClientMessage(playerid,-1,"[服务器]您已生成AK47"); 
				}
				case 1:
				{
					GivePlayerWeapon(playerid,31,9999);
				    SendClientMessage(playerid,-1,"[服务器]您已生成M4A1");
				}
				case 2:
				{
					GivePlayerWeapon(playerid,33,9999);
				    SendClientMessage(playerid,-1,"[服务器]您已生成精准射手步枪");
				}
				case 3:
				{
					GivePlayerWeapon(playerid,34,9999);
				    SendClientMessage(playerid,-1,"[服务器]您已生成狙击步枪");
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
				    SendClientMessage(playerid,-1,"[服务器]您已生成火箭筒");
				}
				case 1:
				{
					GivePlayerWeapon(playerid,36,9999);
				    SendClientMessage(playerid,-1,"[服务器]您已生成热追踪火箭筒");
				}
				case 2:
				{
					GivePlayerWeapon(playerid,37,9999);
				    SendClientMessage(playerid,-1,"[服务器]您已生成火焰喷射器");
				}
				case 3:
				{
					GivePlayerWeapon(playerid,38,9999);
				    SendClientMessage(playerid,-1,"[服务器]您已生成加特林机关枪");
				}
				case 4:
				{
					GivePlayerWeapon(playerid,40,9999);
					GivePlayerWeapon(playerid,39,9999);
				    SendClientMessage(playerid,-1,"[服务器]您已生成C4炸药");
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
				    SendClientMessage(playerid,-1,"[服务器]您已生成喷漆");
				}
				case 1:
				{
					GivePlayerWeapon(playerid,43,9999);
				    SendClientMessage(playerid,-1,"[服务器]您已生成照相机");
				}
				case 2:
				{
					GivePlayerWeapon(playerid,42,9999);
				    SendClientMessage(playerid,-1,"[服务器]您已生成灭火器");
				}
				case 3:
				{
					GivePlayerWeapon(playerid,46,9999);
				    SendClientMessage(playerid,-1,"[服务器]您已生成降落伞");
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
	                SendClientMessage(playerid,-1,"[服务器]您已补满您的血量");
	            }
	            case 1:
				{
				    SetPlayerArmour(playerid,100);
				    SendClientMessage(playerid,-1,"[服务器]您已补满您的护甲");
				}
				case 2:
				{
				    SetPlayerHealth(playerid,100);
				    SetPlayerArmour(playerid,100);
				    SendClientMessage(playerid,-1,"[服务器]您已补满您的血量和护甲");
				}
	        }
		}
	}
    return 1;
}

COMMAND:o(playerid,params[])
{
    new content[256],string[256],name[64];
    GetPlayerName(playerid,name,sizeof(name));
    if(sscanf(params,"s[256]",content)) return SendClientMessage(playerid,COLOR_RED,"[服务器]发送全服玩家可看见的消息: /o [内容]");
    format(string,sizeof(string),"[世界频道]%s(%d):%s",name,playerid,content);
    SendClientMessageToAll(GetPlayerColor(playerid),string);
    return 1;
}

COMMAND:skin(playerid,params[])
{
    new skinid[18];
    if(sscanf(params,"s[18]",skinid)) return SendClientMessage(playerid,COLOR_RED,"[服务器]修改皮肤 /skin [皮肤ID]");
    if(strval(skinid) < 0|| strval(skinid) >299) return SendClientMessage(playerid,COLOR_RED,"[服务器]皮肤ID限制在0~299内");
    SetPlayerSkin(playerid,strval(skinid));
    SendClientMessage(playerid,COLOR_RED,"[服务器]]您已成功修改皮肤");
    return 1;
}

COMMAND:v(playerid,params[])
{
    new params1[64],params2[64],params3[64];
    if(sscanf(params,"s[64]s[64]s[64]",params1,params2,params3))
    {
        if(!strlen(params1)) return SendClientMessage(playerid,COLOR_RED,"[服务器]载具系统帮助 /v help");
    }

    if(strval(params1))
    {
        new Float:x,Float:y,Float:z,Float:a;
        GetPlayerPos(playerid,x,y,z);
        GetPlayerFacingAngle(playerid,a);
        if(strval(params1) < 400 || strval(params1) > 611) return SendClientMessage(playerid,COLOR_RED,"[服务器]载具ID在400~611之间");
        if(PlayerVehicle[playerid] != -1)
        {
            DestroyVehicle(PlayerVehicle[playerid]);
            PlayerVehicle[playerid] = -1;
        }
        new PDCar[] = {427,523,596,597,598,599,497};
        new an = 0;
        for(new i =0;i<sizeof(PDCar);i++){
            if(strval(params1) == PDCar[i]){
                PlayerVehicle[playerid] = CreateVehicle(strval(params1),x,y,z,a,0,1,-1);
                an = 1;
            }
        }
        new FDCar[] = {416,407};
        for(new i =0;i<sizeof(FDCar);i++){
            if(strval(params1) == FDCar[i]){
                if(strval(params1) == 407){
                    PlayerVehicle[playerid] = CreateVehicle(strval(params1),x,y,z,a,3,3,-1);
                }
                if(strval(params1) == 416){
                    PlayerVehicle[playerid] = CreateVehicle(strval(params1),x,y,z,a,1,3,-1);
                }
                an = 1;
            }
        }
        if(an == 0){
            PlayerVehicle[playerid] = CreateVehicle(strval(params1),x,y,z,a,random(10),random(10),-1);
        }
        SetVehicleVirtualWorld(PlayerVehicle[playerid],GetPlayerVirtualWorld(playerid));
        PutPlayerInVehicle(playerid,PlayerVehicle[playerid],0);
        AddVehicleComponent(PlayerVehicle[playerid], 1010);
        SendClientMessage(playerid,COLOR_RED,"[服务器]载具生成成功");
        return 1;
    }

    if(!strcmp(params1,"help",true))
    {
        SendClientMessage(playerid,COLOR_RED,"[服务器]载具系统帮助");
        SendClientMessage(playerid,COLOR_RED,"[服务器]生成载具 /v [载具ID] || 召回载具 /v wode || 修改载具颜色 /v color");
        SendClientMessage(playerid,COLOR_RED,"[服务器]上锁/解锁载具 /v suo || 修复载具 /xiu");
        return 1;
    }

    if(!strcmp(params1,"suo",true))
    {
        if(PlayerVehicle[playerid] == -1) return SendClientMessage(playerid,COLOR_RED,"[服务器]您还没有专属载具");
        if(!GetPVarInt(playerid,"is_car_lock")){
            SetPVarInt(playerid,"is_car_lock",!GetPVarInt(playerid,"is_car_lock"));
            SendClientMessage(playerid,COLOR_RED,"[服务器]您已上锁您的专属载具");
            return 1;
        }
        DeletePVar(playerid,"is_car_lock");
        SendClientMessage(playerid,COLOR_RED,"[服务器]您已解锁您的专属载具");
        return 1;
    }

    if(!strcmp(params1,"color",true))
    {
        if(!IsPlayerInAnyVehicle(playerid)) return SendClientMessage(playerid,COLOR_RED,"[服务器]您不在任何载具上");
        if(GetPlayerVehicleSeat(playerid) != 0) return SendClientMessage(playerid,COLOR_RED,"[服务器]您不是该载具的司机");
        if(!strlen(params2)) return SendClientMessage(playerid,COLOR_RED,"[服务器]修改载具颜色 /v color [颜色ID1] [颜色ID2(可不填)]");
        SendClientMessage(playerid,COLOR_RED,"[服务器]修改载具颜色成功");
        if(!strlen(params3))
        {
            ChangeVehicleColor(GetPlayerVehicleID(playerid),strval(params2),strval(params2));
            return 1;
        }
        ChangeVehicleColor(GetPlayerVehicleID(playerid),strval(params2),strval(params3));
        return 1;
    }

    if(!strcmp(params1,"wode",true))
    {
        if(PlayerVehicle[playerid] == -1) return SendClientMessage(playerid,COLOR_RED,"[服务器]您还没有专属载具");
        new Float:x,Float:y,Float:z,Float:a;
        GetPlayerPos(playerid,x,y,z);
        GetPlayerFacingAngle(playerid,a);
        SetVehiclePos(PlayerVehicle[playerid],x,y,z);
        SetVehicleZAngle(PlayerVehicle[playerid],a);
        SetVehicleVirtualWorld(PlayerVehicle[playerid],GetPlayerVirtualWorld(playerid));
        PutPlayerInVehicle(playerid,PlayerVehicle[playerid],0);
        SendClientMessage(playerid,COLOR_RED,"[服务器]载具召回成功");
        return 1;
    }
    return 1;
}

COMMAND:xiu(playerid)
{
    if(!IsPlayerInAnyVehicle(playerid)) return SendClientMessage(playerid,COLOR_RED,"[服务器]您不在任何载具上");
    if(GetPlayerVehicleSeat(playerid) != 0) return SendClientMessage(playerid,COLOR_RED,"[服务器]您不是该载具的司机");
    RepairVehicle(GetPlayerVehicleID(playerid));
    SendClientMessage(playerid,COLOR_RED,"[服务器]载具修复成功");
    return 1;
}

COMMAND:gun(playerid)
{
	ShowPlayerDialog(playerid,3100,DIALOG_STYLE_LIST,"武器系统","手持类\n投掷类\n手枪类\n冲锋枪\n散弹枪\n步枪类\n特殊类\n其他类\n血甲类","确定","取消");
	return 1;
}

COMMAND:goto(playerid,params[])
{
    new pid[18],name[64],name1[64],string[256];
    new Float:x,Float:y,Float:z;
    if(sscanf(params,"s[18]",pid)) return SendClientMessage(playerid,COLOR_RED,"[服务器]传送至玩家附近: /goto [玩家ID]");
    if(!IsPlayerConnected(strval(pid))) return SendClientMessage(playerid,COLOR_RED,"[服务器]该玩家不在线");
    GetPlayerPos(strval(pid),x,y,z);
    GetPlayerName(playerid,name,sizeof(name));
    GetPlayerName(strval(pid),name1,sizeof(name1));
    if(IsPlayerInAnyVehicle(playerid))
    {
        if(GetPlayerVehicleSeat(playerid) == 0)
        {
            new vid = GetPlayerVehicleID(playerid);
            SetPlayerPos(playerid,x-3,y-3,z+1);
            SetVehiclePos(vid,x-3,y-3,z+1);
            PutPlayerInVehicle(playerid,vid,0);
        }
    }
    else
    {
        SetPlayerPos(playerid,x-2,y-2,z);
    }
    format(string,sizeof(string),"[系统]玩家%s(%d)通过传送至您的附近 ",name,playerid);
    SendClientMessage(playerid,COLOR_RED,string);
    format(string,sizeof(string),"[系统]你通过传送至玩家%s(%d)的附近",name1,strval(pid));
    SendClientMessage(playerid,COLOR_RED,string);
    return 1;
}

forward AddComponent();
public AddComponent()
{
    for(new i=0;i<MAX_PLAYERS;i++)
    {
        if(GetPlayerVehicleSeat(i) == 0)
        {
            if(!GetPVarInt(i,"is_rp"))
            {
                AddVehicleComponent(GetPlayerVehicleID(i), 1010);
            }
        }
    }
    return 1;
}

public OnPlayerDeath(playerid, killerid, reason)
{
    SendDeathMessage(killerid, playerid, reason);
    return 1;
}

forward AddScore();
public AddScore()
{
    for(new i=0;i<MAX_PLAYERS;i++)
    {
        if(IsPlayerConnected(i))
        {
            SendClientMessage(i,-1,"[系统]您在线满一小时,时间分自动加一");
            SetPlayerScore(i,GetPlayerScore(i)+1);
        }
    }
    return 1;
}

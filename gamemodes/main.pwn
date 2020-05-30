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
    SendRconCommand("hostname [��½С��]�����׼�GTA�����ֻ���ƽ̨��ͨ������");
    SetGameModeText("��½С�� v4.0");
    return 1;
}

public OnGameModeInit()
{
    print("_______��½С�� v0915_______");
    print("      Powered By Tuise    ")
    print("_______��½С�� v0915_______")

    //ServerVariable
    SetSVarInt("World_Time",1);
    SetWorldTime(1);
	EnableStuntBonusForAll(0);
	anticheat_OnGameModeInit();
	dm_OnGameModeInit();

    //Timer
    SetTimer("CheckLogin",500,1); //����Ƿ��½��δ��½������ʾ
    SetTimer("Score",60000,1); //���ʱ���
    SetTimer("Autofix",1000,1); //�ؾ��Զ��޸�
    SetTimer("AddComponent",3000,1); //�ؾ���ӵ���
    SetTimer("WorldTime",300000,1); //����ʱ�� ��ʵ5����=��Ϸ1Сʱ 
    SetTimer("AutoMessage",60000,1); //������������� 1��������1��
    SetTimer("CheckServerInfo",1000,1); //��������̬��Ϣ����
    SetTimer("SkickCheck",1000,1); //���ͶƱ����ʱ��ѭ��
    SetTimer("RpMoney",60000,1); //��ɫ�������繤��
    SetTimer("Wanted",1000,1); //ͨ��ϵͳʱ��ѭ��
    SetTimer("God",100,1); //����޵�ģʽ
    SetTimer("RpJail",1000,1); //RP�����ж�
	SetTimer("Afking",500,1); //�ж��Ƿ��ڹһ�
	SetTimer("Jail",1000,1); //��Ҽ��� 
	SetTimer("SignInit",1000,1); //ǩ�����ݿ��ʼ��
	SetTimer("QueryServer",100,1); //�ж��Ƿ��ѯ������״̬
	SetTimer("QueryPlayers",100,1); //�ж��Ƿ��ѯ�����Ϣ
	SetTimer("QueryPlayerList",100,1); //�ж��Ƿ��ѯ�������
	SetTimer("QueryGMList",100,1); //�ж��Ƿ��ѯ�������
	SetTimer("AntiMoney",500,1); //�ж�����Ƿ�ˢǮ
	SetTimer("SetPlayerCh",100,1); //������ҳƺ�
	SetTimer("SetPlayerXwb",100,1); //�������Сβ��
	SetTimer("CodeColor",1000,1); //���þ�ԱCodeʱ��ɫ
	SetTimer("AntiRolling",2000,1); //��ˢ��
	SetTimer("AntiVehicle",1000,1); //��Υ���ؾ�
	SetTimer("OnFireTimer",1000,1); //��Χ����
	SetTimer("KickEx",1000,1); //�ӳ�����
	SetTimer("SosReceive",1000,1); //���¹���Ա����
	SetTimer("RGBCar",100,1); //VIP�ʺ糵
    return 1;
}

public OnPlayerConnect(playerid)
{
    new name[64],string[256];
    //main.pwn
    GetPlayerName(playerid,name,sizeof(name));
    format(string,sizeof(string),"{ffffff}��ӭ��� [{15BFEA}%s(%d){ffffff}] ���������",name,playerid);
	new files[64];
	format(files,sizeof(files),"\\Players\\%s.ini",name);
	if(!fexist(files))
	{
		format(string,sizeof(string),"{ffffff}��ӭ����� [{15BFEA}%s(%d){ffffff}] ���������",name,playerid);
	}
    ServerTips(-1,string);
    new colors[6] = {0xFF80FFC8,0xFFFF80C8,0x00FF80C8,0x008000C8,0xFFFF80C8,0xFF8040C8};
    SetPlayerColor(playerid,colors[random(6)]);
	EnableStuntBonusForPlayer(playerid, 0);//�ر��ؼ�����
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
    format(string,sizeof(string),"{ffffff}��� [{FF80FF}%s(%d){ffffff}] �뿪������",name,playerid)
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
				ServerTips(j,"���ڱ��۲������˳�������,���ѷ��ط�����������");
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
                ServerTips(playerid,"��⵽��ǰIP���ϴε�½IP��ͬ,���Զ���½,��ӭ����!");
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
        "SF����","����԰","���С����"
    }
    new a = random(3);
    format(string,sizeof(string),"�������� - %s",posnameArray[a]);
    SetPlayerPos(playerid,posArray[a][0],posArray[a][1],posArray[a][2]); //����������
    ServerTips(playerid,string);
    SetPlayerSkin(playerid,dini_Int(files,"Skin"));
	ServerTips(playerid,"����/logs�鿴�������ְ汾��������");
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
		ServerTips(playerid,"����û�е�¼,�޷������κβ���");
		format(chatlog,sizeof(chatlog),"%s%s(%d):%s_UnLogin\n",time,name,playerid,text);
		WriteChatLog(chatlog);
		return 0;
	}
    new string[1024],files[64];
    new color = GetPlayerColor(playerid);
    format(files,sizeof(files),"\\Players\\%s.ini",name);
	if(GetPVarInt(playerid,"is_mute")){
	    AdminTips(playerid,"���ѱ�����Ա����,����ϵ����Ա�������");
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
			SendClientMessage(playerid,COLOR_RED,"[T-AntiCheat]���������ٶ�̫����,������������·���");
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
		AdminTips(playerid,"���ѱ���ֹ�����ؾ�,����ϵ����Ա���н��");
		return 1;
	}
    if(!GetPVarInt(playerid,"is_rp")){
        GetPlayerName(GetVehicleHost(vehicleid),name,sizeof(name));
        GetPlayerName(playerid,name1,sizeof(name1));
        format(string,sizeof(string),"��������%s(%d)���ɵ��ؾ�,�ؾ�ID:%d",name,GetVehicleHost(vehicleid),GetVehicleModel(vehicleid));
        if(vehicleid != PlayerVehicle[playerid]){
            if(GetPVarInt(GetVehicleHost(vehicleid),"is_car_lock")){
                RemovePlayerFromVehicle(playerid);
                format(string,sizeof(string),"����%s(%d)�ѽ����ؾ�����",name,GetVehicleHost(vehicleid));
                ServerTips(playerid,string);
                return 1;
            }
            ServerTips(playerid,string);
            format(string,sizeof(string),"���%s(%d)����������ר���ؾ�",name1,playerid);
            ServerTips(GetVehicleHost(vehicleid),string);
            return 1;
        }
        else{
            format(string,sizeof(string),"���ѽ�������ר���ؾ�,�ؾ�ID:%d",GetVehicleModel(vehicleid));
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
			RpTips(playerid,"����ҽԺ������Ҫ500��Ԫ,������������,�����Ʋ�");
			new name[64],string[256];
			GetPlayerName(playerid,name,sizeof(name));
			format(string,sizeof(string),"���%s(%d)�Ʋ���",name,playerid);
			RpTips(-1,string);
			return 1;
		}
		else
		{
			RpTips(playerid,"����ҽԺ���ƻ�����500��Ԫ");
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
        format(style,sizeof(style),"����ǳ�:%s\n���ID:%d\n���ʱ���:%d\n���IP:%s\n�������:%f,%f,%f\n��ҳƺ�:%s\n���Сβ��:%s",name,clickedplayerid,GetPlayerScore(clickedplayerid),ip,x,y,z,dini_Get(files,"Ch"),dini_Get(files,"Xwb"));
        ShowPlayerDialog(playerid,CMain,DIALOG_STYLE_LIST,"�����Ϣ",style,"ȷ��","ȡ��");
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
                case 0: ShowPlayerDialog(playerid, CName, DIALOG_STYLE_INPUT, "�޸��ǳ�", "�޸ĸ���ҵ��ǳ�","�޸�", "ȡ��");
                case 2: ShowPlayerDialog(playerid, CScore, DIALOG_STYLE_INPUT, "�޸�ʱ���", "�޸ĸ���ҵ�ʱ���","�޸�", "ȡ��");
                case 4: ShowPlayerDialog(playerid, CPos, DIALOG_STYLE_MSGBOX, "��������Ҹ���", "��ȷ��Ҫ����������Ҹ���","ȷ��", "ȡ��");
                case 5: ShowPlayerDialog(playerid, CCh, DIALOG_STYLE_INPUT, "�޸ĳƺ�", "�޸ĸ���ҵĳƺ�","�޸�", "ȡ��");
                case 6: ShowPlayerDialog(playerid, CXwb, DIALOG_STYLE_INPUT, "�޸�Сβ��", "�޸ĸ���ҵ�Сβ��","�޸�", "ȡ��");
            }
        }
        else{
            DeletePVar(playerid, "is_clickedid");
        }
    }

    if(dialogid == CName){
        GetPlayerName(GetPVarInt(playerid,"is_clickedid")-1,name1,sizeof(name1));
        SetPlayerName(GetPVarInt(playerid,"is_clickedid")-1,inputtext);
        format(string,sizeof(string),"[T-Admin]����Ա%s(%d)�޸����%s(%d)�ǳ�Ϊ:%s",name,playerid,name1,GetPVarInt(playerid,"is_clickedid")-1,inputtext);
        SendClientMessageToAll(COLOR_RED, string);
        DeletePVar(playerid, "is_clickedid");
        return 1;
    }

    if(dialogid == CScore){
        SetPlayerScore(GetPVarInt(playerid,"is_clickedid")-1, strval(inputtext))
        GetPlayerName(GetPVarInt(playerid,"is_clickedid")-1,name1,sizeof(name1));
        format(string,sizeof(string),"[T-Admin]����Ա%s(%d)�޸����%s(%d)��ʱ���Ϊ:%d��",name,playerid,name1,GetPVarInt(playerid,"is_clickedid")-1,strval(inputtext));
        SendClientMessageToAll(COLOR_RED, string);
        DeletePVar(playerid, "is_clickedid");
        return 1;
    }

    if(dialogid == CPos){
        GetPlayerPos(GetPVarInt(playerid,"is_clickedid")-1,x,y,z);
        SetPlayerPos(playerid,x+3,y+3,z);
        SendClientMessage(playerid,COLOR_RED,"[T-Admin]���ͳɹ�");
        DeletePVar(playerid, "is_clickedid");
        return 1;
    }

    if(dialogid == CCh){
        GetPlayerName(GetPVarInt(playerid,"is_clickedid")-1,name1,sizeof(name1));
        format(files,sizeof(files),"\\Players\\%s.ini",name1);
        dini_Set(files,"Ch",inputtext);
        format(string,sizeof(string),"[T-Admin]����Ա%s(%d)�޸����%s(%d)�ĳƺ�Ϊ:%s",name,playerid,name1,GetPVarInt(playerid,"is_clickedid")-1,inputtext);
        SendClientMessageToAll(COLOR_RED,string);
        DeletePVar(playerid, "is_clickedid");
        return 1;
    }

    if(dialogid == CXwb){
        GetPlayerName(GetPVarInt(playerid,"is_clickedid")-1,name1,sizeof(name1));
        format(files,sizeof(files),"\\Players\\%s.ini",name1);
        dini_Set(files,"Xwb",inputtext);
        format(string,sizeof(string),"[T-Admin]����Ա%s(%d)�޸����%s(%d)��Сβ��Ϊ:%s",name,playerid,name1,GetPVarInt(playerid,"is_clickedid")-1,inputtext);
        SendClientMessageToAll(COLOR_RED,string);
        DeletePVar(playerid, "is_clickedid");
        return 1;
    }

    //����ϵͳ
	if(dialogid == 3100)
	{
	    if(response)
	    {
			switch(listitem)
			{
				case 0:ShowPlayerDialog(playerid,3101,DIALOG_STYLE_LIST,"�ֳ���","ָ��\n�߶���\n����\nС��\n�����\n����\n̨���\n��ʿ��\n���\n��\n����","ȷ��","ȡ��");
				case 1:ShowPlayerDialog(playerid,3102,DIALOG_STYLE_LIST,"Ͷ����","����\n���ᵯ\nȼ�յ�","ȷ��","ȡ��");
				case 2:ShowPlayerDialog(playerid,3103,DIALOG_STYLE_LIST,"��ǹ��","˫����ǹ\n������ǹ\nɳĮ֮ӥ","ȷ��","ȡ��");
				case 3:ShowPlayerDialog(playerid,3104,DIALOG_STYLE_LIST,"���ǹ","Tec9\nUZI\nMP5","ȷ��","ȡ��");
				case 4:ShowPlayerDialog(playerid,3105,DIALOG_STYLE_LIST,"ɢ��ǹ","��ͨɢ��ǹ\n˫��ɢ��ǹ\n������ɢ��ǹ","ȷ��","ȡ��");
				case 5:ShowPlayerDialog(playerid,3106,DIALOG_STYLE_LIST,"��ǹ��","AK47\nM4A1\n��׼���ֲ�ǹ\n�ѻ���ǹ","ȷ��","ȡ��");
				case 6:ShowPlayerDialog(playerid,3107,DIALOG_STYLE_LIST,"������","���Ͳ\n��׷�ٻ��Ͳ\n����������\n�����ֻ���ǹ\nC4ըҩ","ȷ��","ȡ��");
				case 7:ShowPlayerDialog(playerid,3108,DIALOG_STYLE_LIST,"������","����\n�����\n�����\n����ɡ","ȷ��","ȡ��");
				case 8:ShowPlayerDialog(playerid,3109,DIALOG_STYLE_LIST,"Ѫ����","Ѫ������\n���ײ���\nѪ���ͻ��ײ���","ȷ��","ȡ��");
			}
		}
	}

	if(dialogid == 3101)
	{
	    if(response)
	    {
	        switch(listitem)
	        {
				//ָ��\n�߶���\n����\nС��\n�����\n����\n̨���\n��ʿ��\n���\n��\n����
				case 0:
				{
					GivePlayerWeapon(playerid,1,9999);
				    ServerTips(playerid,"[������]��������ָ��");
				}
				case 1:
				{
					GivePlayerWeapon(playerid,2,9999);
				    ServerTips(playerid,"[������]�������ɸ߶���");
				}
				case 2:
				{
					GivePlayerWeapon(playerid,3,9999);
				    ServerTips(playerid,"[������]�������ɾ���");
				}
				case 3:
				{
					GivePlayerWeapon(playerid,4,9999);
				    ServerTips(playerid,"[������]��������С��");
				}
				case 4:
				{
					GivePlayerWeapon(playerid,5,9999);
				    ServerTips(playerid,"[������]�������ɰ����");
				}
				case 5:
				{
					GivePlayerWeapon(playerid,6,9999);
				    ServerTips(playerid,"[������]������������");
				}
				case 6:
				{
					GivePlayerWeapon(playerid,7,9999);
				    ServerTips(playerid,"[������]��������̨���");
				}
				case 7:
				{
					GivePlayerWeapon(playerid,8,9999);
				    ServerTips(playerid,"[������]����������ʿ��");
				}
				case 8:
				{
					GivePlayerWeapon(playerid,9,9999);
				    ServerTips(playerid,"[������]�������ɵ��");
				}
				case 9:
				{
					GivePlayerWeapon(playerid,14,9999);
				    ServerTips(playerid,"[������]�������ɻ�");
				}
				case 10:
				{
					GivePlayerWeapon(playerid,15,9999);
				    ServerTips(playerid,"[������]�������ɹ���");
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
				//[������]��������
	            case 0:
	            {
					GivePlayerWeapon(playerid,16,9999);
				    ServerTips(playerid,"[������]������������");
				}
				case 1:
				{
					GivePlayerWeapon(playerid,17,9999);
				    ServerTips(playerid,"[������]�������ɴ��ᵯ");
				}
				case 2:
				{
					GivePlayerWeapon(playerid,18,9999);
				    ServerTips(playerid,"[������]��������ȼ�յ�");
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
				//˫����ǹ\n������ǹ\nɳĮ֮ӥ
			    case 0:
			    {
					GivePlayerWeapon(playerid,22,9999);
				    ServerTips(playerid,"[������]��������˫����ǹ");
				}
				case 1:
				{
					GivePlayerWeapon(playerid,23,9999);
				    ServerTips(playerid,"[������]��������������ǹ");
				}
				case 2:
				{
					GivePlayerWeapon(playerid,24,9999);
				    ServerTips(playerid,"[������]��������ɳĮ֮ӥ");
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
				    ServerTips(playerid,"[������]��������Tec9");
				}
				case 1:
				{
					GivePlayerWeapon(playerid,28,9999);
				    ServerTips(playerid,"[������]��������Uzi");
				}
				case 2:
				{
					GivePlayerWeapon(playerid,29,9999);
				    ServerTips(playerid,"[������]��������MP5");
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
			    //��ͨɢ��ǹ\n˫��ɢ��ǹ\n������ɢ��ǹ
				case 0:
				{
					GivePlayerWeapon(playerid,25,9999);
				    ServerTips(playerid,"[������]����������ͨɢ��ǹ");
				}
				case 1:
				{
					GivePlayerWeapon(playerid,26,9999);
				    ServerTips(playerid,"[������]��������˫��ɢ��ǹ");
				}
				case 2:
				{
					GivePlayerWeapon(playerid,27,9999);
				    ServerTips(playerid,"[������]��������������ɢ��ǹ");
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
	            //AK47\nM4A1\n��׼���ֲ�ǹ\n�ѻ���ǹ
	            case 0:
	            {
					GivePlayerWeapon(playerid,30,9999);
				    ServerTips(playerid,"[������]��������AK47"); 
				}
				case 1:
				{
					GivePlayerWeapon(playerid,31,9999);
				    ServerTips(playerid,"[������]��������M4A1");
				}
				case 2:
				{
					GivePlayerWeapon(playerid,33,9999);
				    ServerTips(playerid,"[������]�������ɾ�׼���ֲ�ǹ");
				}
				case 3:
				{
					GivePlayerWeapon(playerid,34,9999);
				    ServerTips(playerid,"[������]�������ɾѻ���ǹ");
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
         		//���Ͳ\n��׷�ٻ��Ͳ\n����������\n�����ֻ���ǹ\nC4ըҩ
				case 0:
				{
					GivePlayerWeapon(playerid,35,9999);
				    ServerTips(playerid,"[������]�������ɻ��Ͳ");
				}
				case 1:
				{
					GivePlayerWeapon(playerid,36,9999);
				    ServerTips(playerid,"[������]����������׷�ٻ��Ͳ");
				}
				case 2:
				{
					GivePlayerWeapon(playerid,37,9999);
				    ServerTips(playerid,"[������]�������ɻ���������");
				}
				case 3:
				{
					GivePlayerWeapon(playerid,38,9999);
				    ServerTips(playerid,"[������]�������ɼ����ֻ���ǹ");
				}
				case 4:
				{
					GivePlayerWeapon(playerid,40,9999);
					GivePlayerWeapon(playerid,39,9999);
				    ServerTips(playerid,"[������]��������C4ըҩ");
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
		        //����\n�����\n�����\n����ɡ
		        case 0:
		        {
					GivePlayerWeapon(playerid,41,9999);
				    ServerTips(playerid,"[������]������������");
				}
				case 1:
				{
					GivePlayerWeapon(playerid,43,9999);
				    ServerTips(playerid,"[������]�������������");
				}
				case 2:
				{
					GivePlayerWeapon(playerid,42,9999);
				    ServerTips(playerid,"[������]�������������");
				}
				case 3:
				{
					GivePlayerWeapon(playerid,46,9999);
				    ServerTips(playerid,"[������]�������ɽ���ɡ");
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
				//Ѫ������\n���ײ���\nѪ���ͻ��ײ���
	            case 0:
	            {	
					SetPlayerHealth(playerid,100);
	                ServerTips(playerid,"[������]���Ѳ�������Ѫ��");
	            }
	            case 1:
				{
				    SetPlayerArmour(playerid,100);
				    ServerTips(playerid,"[������]���Ѳ������Ļ���");
				}
				case 2:
				{
				    SetPlayerHealth(playerid,100);
				    SetPlayerArmour(playerid,100);
				    ServerTips(playerid,"[������]���Ѳ�������Ѫ���ͻ���");
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
            RpTips(playerid,"���ѵ���Ŀ�ĵ�,�����ܻ���Ѱ�����Ĺ˿�");
            RpTips(playerid,"�����Ĺ˿Ͳ��ڸ���,���������� /cjd ȡ�����εĽ��Ͷ���");
            return 1;
        }
    }
    return 1;
}

public OnPlayerClickMap(playerid, Float:fX, Float:fY, Float:fZ)
{
    if(GetPVarInt(playerid,"is_mtp")){
        SetPlayerPos(playerid,fX,fY,fZ);
        ServerTips(playerid,"�ѽ�����������ͼ��ǵ�");
        ServerTips(playerid,"�رջ�����ͼ���͹��� /mtp");
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
                    format(string,sizeof(string),"��Ա%s(%d)�ɹ�ץ���ӷ�%s(%d),����%d�봦��",name,playerid,name1,damagedid,GetPVarInt(damagedid,"is_wanted_jail"));
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
		format(string,sizeof(string),"[RPģʽ]���%s(%d)�����%s(%d)����˺�%fHP [����:%d ���в�λ:%d]",name1,playerid,name,damagedid,amount,weaponid,bodypart);
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
                    format(string,sizeof(string),"��Ա%s(%d)�ɹ�ץ���ӷ�%s(%d),����%d�봦��",name,issuerid,name1,playerid,GetPVarInt(playerid,"is_wanted_jail"));
                    RpTips(-1,string);
					SetPlayerWantedLevel(playerid,0);
                }
            }
        }

		//��ұ���ͷ
		if(weaponid  == 0 && bodypart == 9)
		{
			GetPlayerName(playerid,name,sizeof(name));
			GetPlayerName(issuerid,name1,sizeof(name1));
			if(GetPlayerWeapon(playerid) == 0)
			{
				return 1;
			}
			format(string,sizeof(string),"���%s(%d)�������������%s(%d)��ͷ��,���%s(%d)��������",issuerid,name1,name,playerid,name,playerid);
			RpTips(-1,string);SetPlayerHealth(playerid,0);
		}
		/*
		GetPlayerName(playerid,name,sizeof(name));
		GetPlayerName(issuerid,name1,sizeof(name1));
		new Float:heal;
		GetPlayerHealth(playerid, heal);
		heal = heal - amount;
		SetPlayerHealth(playerid,heal);
		format(string,sizeof(string),"[RPģʽ]���%s(%d)�����%s(%d)����˺�%fHP [����:%d ���в�λ:%d]",name1,issuerid,name,playerid,amount,weaponid,bodypart);
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
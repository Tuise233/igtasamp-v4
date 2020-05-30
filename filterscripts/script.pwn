//���ƽű� By IGTA-Tuise
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
    format(string,sizeof(string),"[����]%s(%d):%s",name,playerid,text);
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
	format(string,sizeof(string),"[������]���%s(%d)���������",name,playerid);
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
            ShowPlayerDialog(playerid,1000,DIALOG_STYLE_INPUT,"ע��ϵͳ","��ӭ����������,�������������ע��","ȷ��","");
        }
    }
    else
    {
        if(!GetPVarInt(playerid,"is_login"))
        {
            ShowPlayerDialog(playerid,1001,DIALOG_STYLE_INPUT,"��¼ϵͳ","��ӭ����,������������е�¼","��¼","ȡ��");
        }
    }
}

public OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
    new name[64],files[64],string[256];
    GetPlayerName(playerid,name,sizeof(name));
    format(files,sizeof(files),"\\Players\\%s.ini",name);
    //ע��Ի���
    if(dialogid == 1000)
    {
        if(response)
        {
            if(!strlen(inputtext))
            {
                SendClientMessage(playerid,-1,"[��¼ϵͳ]����������벻��Ϊ��");
                ShowPlayerDialog(playerid,1000,DIALOG_STYLE_INPUT,"ע��ϵͳ","��ӭ����������,�������������ע��","ȷ��","");
            }
            else
            {
                dini_Create(files);
                dini_Set(files,"Name",name);
                dini_Set(files,"Password",inputtext);
                format(string,sizeof(string),"[ע��ϵͳ]ע�����,����������:%s,�´ε�¼ʱҪʹ�õ���Ŷ~",inputtext);
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

    //��¼�Ի���
    if(dialogid == 1001)
    {
        if(response)
        {
            if(!strlen(inputtext))
            {
                SendClientMessage(playerid,-1,"[��¼ϵͳ]����������벻��Ϊ��");
                ShowPlayerDialog(playerid,1001,DIALOG_STYLE_INPUT,"��¼ϵͳ","��ӭ����,������������е�¼","��¼","ȡ��");
            }
            else
            {
                new pwd[128];
                format(pwd,sizeof(pwd),"%s",dini_Get(files,"Password"));
                if(!strcmp(inputtext,pwd,false))
                {
                    SendClientMessage(playerid,-1,"[��¼ϵͳ]��½�ɹ�,��ӭ����!");
                    SetPVarInt(playerid,"is_login",!GetPVarInt(playerid,"is_login"));
                    SetPlayerSkin(playerid,dini_Int(files,"Skin"));
                    SetPlayerPos(playerid,dini_Float(files,"pos_X"),dini_Float(files,"pos_Y"),dini_Float(files,"pos_Z"));
                    SpawnPlayer(playerid);
                }
                else
                {
                    SendClientMessage(playerid,-1,"[��¼ϵͳ]���������������,��˶Ժ���������");
                    ShowPlayerDialog(playerid,1001,DIALOG_STYLE_INPUT,"��¼ϵͳ","��ӭ����,������������е�¼","��¼","ȡ��");
                }
            }
        }
        else
        {
            Kick(playerid);
        }
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
				    SendClientMessage(playerid,-1,"[������]��������ָ��");
				}
				case 1:
				{
					GivePlayerWeapon(playerid,2,9999);
				    SendClientMessage(playerid,-1,"[������]�������ɸ߶���");
				}
				case 2:
				{
					GivePlayerWeapon(playerid,3,9999);
				    SendClientMessage(playerid,-1,"[������]�������ɾ���");
				}
				case 3:
				{
					GivePlayerWeapon(playerid,4,9999);
				    SendClientMessage(playerid,-1,"[������]��������С��");
				}
				case 4:
				{
					GivePlayerWeapon(playerid,5,9999);
				    SendClientMessage(playerid,-1,"[������]�������ɰ����");
				}
				case 5:
				{
					GivePlayerWeapon(playerid,6,9999);
				    SendClientMessage(playerid,-1,"[������]������������");
				}
				case 6:
				{
					GivePlayerWeapon(playerid,7,9999);
				    SendClientMessage(playerid,-1,"[������]��������̨���");
				}
				case 7:
				{
					GivePlayerWeapon(playerid,8,9999);
				    SendClientMessage(playerid,-1,"[������]����������ʿ��");
				}
				case 8:
				{
					GivePlayerWeapon(playerid,9,9999);
				    SendClientMessage(playerid,-1,"[������]�������ɵ��");
				}
				case 9:
				{
					GivePlayerWeapon(playerid,14,9999);
				    SendClientMessage(playerid,-1,"[������]�������ɻ�");
				}
				case 10:
				{
					GivePlayerWeapon(playerid,15,9999);
				    SendClientMessage(playerid,-1,"[������]�������ɹ���");
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
				    SendClientMessage(playerid,-1,"[������]������������");
				}
				case 1:
				{
					GivePlayerWeapon(playerid,17,9999);
				    SendClientMessage(playerid,-1,"[������]�������ɴ��ᵯ");
				}
				case 2:
				{
					GivePlayerWeapon(playerid,18,9999);
				    SendClientMessage(playerid,-1,"[������]��������ȼ�յ�");
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
				    SendClientMessage(playerid,-1,"[������]��������˫����ǹ");
				}
				case 1:
				{
					GivePlayerWeapon(playerid,23,9999);
				    SendClientMessage(playerid,-1,"[������]��������������ǹ");
				}
				case 2:
				{
					GivePlayerWeapon(playerid,24,9999);
				    SendClientMessage(playerid,-1,"[������]��������ɳĮ֮ӥ");
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
				    SendClientMessage(playerid,-1,"[������]��������Tec9");
				}
				case 1:
				{
					GivePlayerWeapon(playerid,28,9999);
				    SendClientMessage(playerid,-1,"[������]��������Uzi");
				}
				case 2:
				{
					GivePlayerWeapon(playerid,29,9999);
				    SendClientMessage(playerid,-1,"[������]��������MP5");
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
				    SendClientMessage(playerid,-1,"[������]����������ͨɢ��ǹ");
				}
				case 1:
				{
					GivePlayerWeapon(playerid,26,9999);
				    SendClientMessage(playerid,-1,"[������]��������˫��ɢ��ǹ");
				}
				case 2:
				{
					GivePlayerWeapon(playerid,27,9999);
				    SendClientMessage(playerid,-1,"[������]��������������ɢ��ǹ");
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
				    SendClientMessage(playerid,-1,"[������]��������AK47"); 
				}
				case 1:
				{
					GivePlayerWeapon(playerid,31,9999);
				    SendClientMessage(playerid,-1,"[������]��������M4A1");
				}
				case 2:
				{
					GivePlayerWeapon(playerid,33,9999);
				    SendClientMessage(playerid,-1,"[������]�������ɾ�׼���ֲ�ǹ");
				}
				case 3:
				{
					GivePlayerWeapon(playerid,34,9999);
				    SendClientMessage(playerid,-1,"[������]�������ɾѻ���ǹ");
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
				    SendClientMessage(playerid,-1,"[������]�������ɻ��Ͳ");
				}
				case 1:
				{
					GivePlayerWeapon(playerid,36,9999);
				    SendClientMessage(playerid,-1,"[������]����������׷�ٻ��Ͳ");
				}
				case 2:
				{
					GivePlayerWeapon(playerid,37,9999);
				    SendClientMessage(playerid,-1,"[������]�������ɻ���������");
				}
				case 3:
				{
					GivePlayerWeapon(playerid,38,9999);
				    SendClientMessage(playerid,-1,"[������]�������ɼ����ֻ���ǹ");
				}
				case 4:
				{
					GivePlayerWeapon(playerid,40,9999);
					GivePlayerWeapon(playerid,39,9999);
				    SendClientMessage(playerid,-1,"[������]��������C4ըҩ");
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
				    SendClientMessage(playerid,-1,"[������]������������");
				}
				case 1:
				{
					GivePlayerWeapon(playerid,43,9999);
				    SendClientMessage(playerid,-1,"[������]�������������");
				}
				case 2:
				{
					GivePlayerWeapon(playerid,42,9999);
				    SendClientMessage(playerid,-1,"[������]�������������");
				}
				case 3:
				{
					GivePlayerWeapon(playerid,46,9999);
				    SendClientMessage(playerid,-1,"[������]�������ɽ���ɡ");
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
	                SendClientMessage(playerid,-1,"[������]���Ѳ�������Ѫ��");
	            }
	            case 1:
				{
				    SetPlayerArmour(playerid,100);
				    SendClientMessage(playerid,-1,"[������]���Ѳ������Ļ���");
				}
				case 2:
				{
				    SetPlayerHealth(playerid,100);
				    SetPlayerArmour(playerid,100);
				    SendClientMessage(playerid,-1,"[������]���Ѳ�������Ѫ���ͻ���");
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
    if(sscanf(params,"s[256]",content)) return SendClientMessage(playerid,COLOR_RED,"[������]����ȫ����ҿɿ�������Ϣ: /o [����]");
    format(string,sizeof(string),"[����Ƶ��]%s(%d):%s",name,playerid,content);
    SendClientMessageToAll(GetPlayerColor(playerid),string);
    return 1;
}

COMMAND:skin(playerid,params[])
{
    new skinid[18];
    if(sscanf(params,"s[18]",skinid)) return SendClientMessage(playerid,COLOR_RED,"[������]�޸�Ƥ�� /skin [Ƥ��ID]");
    if(strval(skinid) < 0|| strval(skinid) >299) return SendClientMessage(playerid,COLOR_RED,"[������]Ƥ��ID������0~299��");
    SetPlayerSkin(playerid,strval(skinid));
    SendClientMessage(playerid,COLOR_RED,"[������]]���ѳɹ��޸�Ƥ��");
    return 1;
}

COMMAND:v(playerid,params[])
{
    new params1[64],params2[64],params3[64];
    if(sscanf(params,"s[64]s[64]s[64]",params1,params2,params3))
    {
        if(!strlen(params1)) return SendClientMessage(playerid,COLOR_RED,"[������]�ؾ�ϵͳ���� /v help");
    }

    if(strval(params1))
    {
        new Float:x,Float:y,Float:z,Float:a;
        GetPlayerPos(playerid,x,y,z);
        GetPlayerFacingAngle(playerid,a);
        if(strval(params1) < 400 || strval(params1) > 611) return SendClientMessage(playerid,COLOR_RED,"[������]�ؾ�ID��400~611֮��");
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
        SendClientMessage(playerid,COLOR_RED,"[������]�ؾ����ɳɹ�");
        return 1;
    }

    if(!strcmp(params1,"help",true))
    {
        SendClientMessage(playerid,COLOR_RED,"[������]�ؾ�ϵͳ����");
        SendClientMessage(playerid,COLOR_RED,"[������]�����ؾ� /v [�ؾ�ID] || �ٻ��ؾ� /v wode || �޸��ؾ���ɫ /v color");
        SendClientMessage(playerid,COLOR_RED,"[������]����/�����ؾ� /v suo || �޸��ؾ� /xiu");
        return 1;
    }

    if(!strcmp(params1,"suo",true))
    {
        if(PlayerVehicle[playerid] == -1) return SendClientMessage(playerid,COLOR_RED,"[������]����û��ר���ؾ�");
        if(!GetPVarInt(playerid,"is_car_lock")){
            SetPVarInt(playerid,"is_car_lock",!GetPVarInt(playerid,"is_car_lock"));
            SendClientMessage(playerid,COLOR_RED,"[������]������������ר���ؾ�");
            return 1;
        }
        DeletePVar(playerid,"is_car_lock");
        SendClientMessage(playerid,COLOR_RED,"[������]���ѽ�������ר���ؾ�");
        return 1;
    }

    if(!strcmp(params1,"color",true))
    {
        if(!IsPlayerInAnyVehicle(playerid)) return SendClientMessage(playerid,COLOR_RED,"[������]�������κ��ؾ���");
        if(GetPlayerVehicleSeat(playerid) != 0) return SendClientMessage(playerid,COLOR_RED,"[������]�����Ǹ��ؾߵ�˾��");
        if(!strlen(params2)) return SendClientMessage(playerid,COLOR_RED,"[������]�޸��ؾ���ɫ /v color [��ɫID1] [��ɫID2(�ɲ���)]");
        SendClientMessage(playerid,COLOR_RED,"[������]�޸��ؾ���ɫ�ɹ�");
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
        if(PlayerVehicle[playerid] == -1) return SendClientMessage(playerid,COLOR_RED,"[������]����û��ר���ؾ�");
        new Float:x,Float:y,Float:z,Float:a;
        GetPlayerPos(playerid,x,y,z);
        GetPlayerFacingAngle(playerid,a);
        SetVehiclePos(PlayerVehicle[playerid],x,y,z);
        SetVehicleZAngle(PlayerVehicle[playerid],a);
        SetVehicleVirtualWorld(PlayerVehicle[playerid],GetPlayerVirtualWorld(playerid));
        PutPlayerInVehicle(playerid,PlayerVehicle[playerid],0);
        SendClientMessage(playerid,COLOR_RED,"[������]�ؾ��ٻسɹ�");
        return 1;
    }
    return 1;
}

COMMAND:xiu(playerid)
{
    if(!IsPlayerInAnyVehicle(playerid)) return SendClientMessage(playerid,COLOR_RED,"[������]�������κ��ؾ���");
    if(GetPlayerVehicleSeat(playerid) != 0) return SendClientMessage(playerid,COLOR_RED,"[������]�����Ǹ��ؾߵ�˾��");
    RepairVehicle(GetPlayerVehicleID(playerid));
    SendClientMessage(playerid,COLOR_RED,"[������]�ؾ��޸��ɹ�");
    return 1;
}

COMMAND:gun(playerid)
{
	ShowPlayerDialog(playerid,3100,DIALOG_STYLE_LIST,"����ϵͳ","�ֳ���\nͶ����\n��ǹ��\n���ǹ\nɢ��ǹ\n��ǹ��\n������\n������\nѪ����","ȷ��","ȡ��");
	return 1;
}

COMMAND:goto(playerid,params[])
{
    new pid[18],name[64],name1[64],string[256];
    new Float:x,Float:y,Float:z;
    if(sscanf(params,"s[18]",pid)) return SendClientMessage(playerid,COLOR_RED,"[������]��������Ҹ���: /goto [���ID]");
    if(!IsPlayerConnected(strval(pid))) return SendClientMessage(playerid,COLOR_RED,"[������]����Ҳ�����");
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
    format(string,sizeof(string),"[ϵͳ]���%s(%d)ͨ�����������ĸ��� ",name,playerid);
    SendClientMessage(playerid,COLOR_RED,string);
    format(string,sizeof(string),"[ϵͳ]��ͨ�����������%s(%d)�ĸ���",name1,strval(pid));
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
            SendClientMessage(i,-1,"[ϵͳ]��������һСʱ,ʱ����Զ���һ");
            SetPlayerScore(i,GetPlayerScore(i)+1);
        }
    }
    return 1;
}

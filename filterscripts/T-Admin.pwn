/*
    T-Admin - ɽ�Ƕ��ư�
    ��Դ������IGTA��ɫ������Ϊɽ�ǳ�����������ƹ���Ա�ű�
    ����bug�뼰ʱ������ɫ
    ����ɾ�����У�������Ȩ������Ը���

    PS:ʹ�øýű�����scriptfiles�ļ����´���һ��Admins���ļ��У����򽫻ᵼ�·���������

    ����
    T-Admin

    ͨ��
        ע�����Ա�˺�/ar
        ��½����Ա�˺�/al
        �˳�ִ��״̬/tc
        �鿴���߹���Ա/gms
        �ٱ����/jubao
        ����ԱƵ������/ac

    1��
        �������/jingao
        ������ /jianjin
        ������� /jinyan
        ����������/moshou
        ����/qing

    2��
        ������߳�������/ti
        ��������������Ա����/say
        �������/dong
        ��ֹ��ҽ����ؾ� /nocar
        ɾ���ؾ� /delc

    3��
        �޸��������/setname
        �������ʱ���/jiangli
        ȫԱ����/muteall

    4��
        ������/ban

    5��/Rcon��
        ������ҹ���Ա�ȼ�/setadmin
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
        AdminTips(playerid,"���ѱ�����Ա����,����ϵ����Ա�������");
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
        AdminTips(playerid,"�����Ƿ���������Ա");
        return false;
    }
    if(GetPVarInt(playerid,"is_admin") < level){
        AdminTips(playerid,"����Ȩ�޲���");
        return false;
    }
    if(GetPVarInt(playerid,"is_admin") >= level) return true;
    return false;
}

public OnPlayerSpawn(playerid)
{
    AdminTips(playerid,"Tuise-Admin��ɫ����Աϵͳ��ȭ����!!");
    return 1;
}

COMMAND:ar(playerid,params[])
{
    new name[64],files[64],password[64];
    GetPlayerName(playerid,name,sizeof(name));
    format(files,sizeof(files),"\\Admins\\%s.ini",name);
    if(sscanf(params,"s[64]",password)) return AdminTips(playerid,"ע�����Ա�˺�:/azc [����]");
    if(fexist(files)) return AdminTips(playerid,"����ע�������Ա�˺�");
    if(!fexist(files)){
        dini_Create(files);
        dini_Set(files,"Name",name);
        dini_Set(files,"Password",password);
        dini_IntSet(files,"Level",0);
        AdminTips(playerid,"ע�����Ա�˺ųɹ�,����ϵ�߲��ȡȨ��");
        return 1;
    }
    return 1;
}

COMMAND:al(playerid,params[])
{
    new password[64],name[64],files[64],string[256];
    if(sscanf(params,"s[64]",password)) return AdminTips(playerid,"��½����Ա�˺�:/al [����]");
    if(GetPVarInt(playerid,"is_admin")) return AdminTips(playerid,"���ѵ�½,�������µ�½");
    GetPlayerName(playerid,name,sizeof(name));
    format(files,sizeof(files),"\\Admins\\%s.ini",name);
    if(!fexist(files)) return AdminTips(playerid,"��δע�����Ա�˺�,ע�����Ա�˺�:/ar");
    if(dini_Int(files,"Level") == 0) return AdminTips(playerid,"��û��Ȩ��,����ϵ�߲��ȡȨ��");
    if(!strcmp(dini_Get(files,"Password"),password,true)){
        format(string,sizeof(string),"����Ա%s(%d)����ִ��,Ȩ�޵ȼ�:%d",name,playerid,dini_Int(files,"Level"));
        AdminTips(-1,string);
        SetPVarInt(playerid,"is_admin",dini_Int(files,"Level"));
        SetPlayerColor(playerid,COLOR_RED);
        return 1;
    }
    else{
        AdminTips(playerid,"���������������,���ʵ����������");
    }
    return 1;
}

COMMAND:tc(playerid)
{
    new name[64],string[256];
    if(!GetPVarInt(playerid,"is_admin")) return AdminTips(playerid,"������ִ��״̬,�޷��˳�ִ��");
    GetPlayerName(playerid,name,sizeof(name));
    format(string,sizeof(string),"����Ա%s(%d)�˳�ִ��״̬",name,playerid);
    AdminTips(-1,string);
    DeletePVar(playerid,"is_admin");
    return 1;
}

COMMAND:gms(playerid)
{
    AdminTips(playerid,"��ǰ���߹���Ա");
    for(new i=0;i<MAX_PLAYERS;i++){
        if(GetPVarInt(i,"is_admin")){
            new name[64],string[256];
            GetPlayerName(i,name,sizeof(name));
            format(string,sizeof(string),"%s(%d) - %d��",name,i,GetPVarInt(i,"is_admin"));
            AdminTips(playerid,string);
        }
    }
    return 1;
}

COMMAND:jubao(playerid,params[])
{
    new name[64],name1[64],reason[128],string[256];
    new pid;
    if(sscanf(params,"ds[128]",pid,reason)) return AdminTips(playerid,"�ٱ����:/jubao [���ID] [ԭ��]");
    GetPlayerName(playerid,name,sizeof(name));
    GetPlayerName(pid,name1,sizeof(name1));
    format(string,sizeof(string),"���%s(%d)�ٱ����%s(%d),ԭ��:%s",name,playerid,name1,pid,reason);
    for(new i=0;i<MAX_PLAYERS;i++){
        if(GetPVarInt(i,"is_admin")){
            AdminTips(i,string);
        }
    }
    AdminTips(playerid,"���ľٱ���Ϣ�ѷ��������߹���Ա");
    return 1;
}

COMMAND:ac(playerid,params[])
{
    new content[256],string[256],name[64];
    if(sscanf(params,"s[256]",content)) return AdminTips(playerid,"����Ա����Ƶ��:/ac [����]");
    if(IsAdmin(playerid,1)){
        GetPlayerName(playerid,name,sizeof(name));
        format(string,sizeof(string),"%d������Ա%s(%d):%s",GetPVarInt(playerid,"is_admin"),name,playerid,content);
        for(new i=0;i<MAX_PLAYERS;i++){
            if(GetPVarInt(i,"is_admin")){
                AdminTips(i,string);
            }
        }
    }
    return 1;
}

//1��
COMMAND:jinggao(playerid,params[])
{
    new name[64],name1[64],reason[128],string[256],pid[18];
    new Float:x,Float:y,Float:z;
    if(sscanf(params,"s[18]s[128]",pid,reason)) return AdminTips(playerid,"�������: /jinggao [���ID] [ԭ��]");
    if(IsAdmin(playerid,1)){
        if(!IsPlayerConnected(strval(pid))) return AdminTips(playerid,"����Ҳ�����");
        GetPlayerName(playerid,name,sizeof(name));
        GetPlayerName(strval(pid),name1,sizeof(name1));
        format(string,sizeof(string),"����Ա%s(%d)�������%s(%d),ԭ��:%s",name,playerid,name1,strval(pid),reason);
        AdminTips(-1,string);
        GetPlayerPos(strval(pid),x,y,z);
        SetPlayerPos(strval(pid),x,y,z+5);
    }
    return 1;
}

COMMAND:jinyan(playerid,params[])
{
    new name[64],name1[64],pid[18],string[256];
    if(sscanf(params,"s[18]",pid)) return AdminTips(playerid,"�������: /jinyan [���ID]");
    if(IsAdmin(playerid,1)){
        if(!IsPlayerConnected(strval(pid))) return AdminTips(playerid,"����Ҳ�����");
        GetPlayerName(playerid,name,sizeof(name));
        GetPlayerName(strval(pid),name1,sizeof(name1));
        format(string,sizeof(string),"����Ա%s(%d)�������%s(%d)",name,playerid,name1,strval(pid));
        AdminTips(-1,string);
        SetPVarInt(strval(pid),"is_mute",!GetPVarInt(strval(pid),"is_mute"));
    }
    return 1;
}

COMMAND:jiejinyan(playerid,params[])
{
    new name[64],name1[64],string[256],pid[18];
    if(sscanf(params,"s[18]",pid)) return AdminTips(playerid,"������: /jiejinyan [���ID]");
    if(IsAdmin(playerid,1)){
        if(!IsPlayerConnected(strval(pid))) return AdminTips(playerid,"����Ҳ�����");
        GetPlayerName(playerid,name,sizeof(name));
        GetPlayerName(strval(pid),name1,sizeof(name1));
        if(!GetPVarInt(strval(pid),"is_mute")) return AdminTips(playerid,"�����δ������");
        DeletePVar(strval(pid),"is_mute");
        format(string,sizeof(string),"����Ա%s(%d)��������%s(%d)",name,playerid,name1,strval(pid));
        AdminTips(-1,string);
    }
    return 1;
}

COMMAND:jianyu(playerid,params[])
{
    new name[64],name1[64],string[256],pid[18],sec[18];
    if(sscanf(params,"s[18]s[18]",pid,sec)) return AdminTips(playerid,"������: /jianyu [���ID] [����]");
    if(IsAdmin(playerid,1)){
        if(!IsPlayerConnected(strval(pid))) return AdminTips(playerid,"����Ҳ�����");
        GetPlayerName(playerid,name,sizeof(name));
        GetPlayerName(strval(pid),name1,sizeof(name1));
        format(string,sizeof(string),"����Ա%s(%d)������%s(%d)%d��",name,playerid,name1,strval(pid),strval(sec));
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
                AdminTips(i,"��������,��ú�����!");
                SetPlayerHealth(i,0);
                return 1;
            }
            SetPVarInt(i,"is_jail",GetPVarInt(i,"is_jail") - 1);
            GetPlayerPos(i,x,y,z);
            if(x < -2189.426757 || x > -2174.765625 || y < -269.240661 || y > -256.060913 || z < 34.515625 || y > 38.515625){
                SetPlayerPos(i,-2182.973632,-263.378509,36.515625);
                format(string,sizeof(string),"����ʣ%d�����",GetPVarInt(i,"is_jail"));
                AdminTips(i,string);
            }
        }
    }
    return 1;
}

COMMAND:moshou(playerid,params[])
{
    new pid[18],name[64],name1[64],string[256];
    if(sscanf(params,"s[18]",pid)) return AdminTips(playerid,"û���������:/moshou [���ID]");
    if(IsAdmin(playerid,1)){
        if(!IsPlayerConnected(strval(pid))) return AdminTips(playerid,"����Ҳ�����");
        GetPlayerName(playerid,name,sizeof(name));
        GetPlayerName(strval(pid),name1,sizeof(name1));
        format(string,sizeof(string),"����Ա%s(%d)û�������%s(%d)������",name,playerid,name1,strval(pid));
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
        format(string,sizeof(string),"����Ա%s(%d)����˹��������¼",name,playerid);
        AdminTips(-1,string);
    }
    return 1;
}

//2��
COMMAND:ti(playerid,params[])
{
    new name[64],name1[64],string[256],pid[18],reason[128];
    if(sscanf(params,"s[18]s[128]",pid,reason)) AdminTips(playerid,"�߳����: /ti [���ID] [ԭ��]");
    if(IsAdmin(playerid,2)){
        if(!IsPlayerConnected(strval(pid))) return AdminTips(playerid,"����Ҳ�����");
        GetPlayerName(playerid,name,sizeof(name));
        GetPlayerName(strval(pid),name1,sizeof(name1));
        format(string,sizeof(string),"����Ա%s(%d)�����%s(%d)�߳�������,ԭ��:%s",name,playerid,name1,strval(pid),reason);
        AdminTips(-1,string);
        Kick(strval(pid));
    }
    return 1;
}

COMMAND:say(playerid,params[])
{
    new name[64],content[256],string[256];
    if(sscanf(params,"s[256]",content)) return AdminTips(playerid,"���ͷ���������:/say [����]");
    if(IsAdmin(playerid,2)){
        GetPlayerName(playerid,name,sizeof(name));
        format(string,sizeof(string),"����Ա%s(%d)��������:%s",name,playerid,content);
        for(new i=0;i<3;i++){
            AdminTips(-1,string);
        }
    }
    return 1;
}

COMMAND:dong(playerid,params[])
{
    new pid[18],name[64],name1[64],string[256];
    if(sscanf(params,"s[18]",pid)) return AdminTips(playerid,"��ֹ����ж�:/dong [���ID]");
    if(IsAdmin(playerid,2)){
        if(!IsPlayerConnected(strval(pid))) return AdminTips(playerid,"����Ҳ�����");
        GetPlayerName(playerid,name,sizeof(name));
        GetPlayerName(strval(pid),name1,sizeof(name1));
        if(!GetPVarInt(strval(pid),"is_freezing")){
            format(string,sizeof(string),"����Ա%s(%d)��ֹ���%s(%d)���л",name,playerid,name1,strval(pid));
            AdminTips(-1,string);
            TogglePlayerControllable(strval(pid),0);
            AdminTips(playerid,"���Ѷ���һ�����,�ٴ������Խⶳ���");
            SetPVarInt(strval(pid),"is_freezing",!GetPVarInt(strval(pid),"is_freezing"));
            return 1;
        }
        if(GetPVarInt(strval(pid),"is_freezing")){
            format(string,sizeof(string),"����Ա%s(%d)�������%s(%d)���л",name,playerid,name1,strval(pid));
            TogglePlayerControllable(strval(pid),1);
            AdminTips(-1,string);
        }
    }
    return 1;
}

COMMAND:nocar(playerid,params[])
{
    new pid[18],name[64],name1[64],string[256];
    if(sscanf(params,"s[18]",pid)) return AdminTips(playerid,"��ֹ��ҽ����ؾ�:/nocar [���ID]");
    if(IsAdmin(playerid,2)){
        if(!IsPlayerConnected(strval(pid))) return AdminTips(playerid,"����Ҳ�����");
        GetPlayerName(playerid,name,sizeof(name));
        GetPlayerName(strval(pid),name1,sizeof(name1));
        if(!GetPVarInt(strval(pid),"is_nocar")){
            SetPVarInt(strval(pid),"is_nocar",!GetPVarInt(strval(pid),"is_nocar"));
            format(string,sizeof(string),"����Ա%s(%d)��ֹ���%s(%d)�����ؾ�",name,playerid,name1,strval(pid));
            AdminTips(-1,string);
            return 1;
        }
        if(GetPVarInt(strval(pid),"is_nocar")){
            DeletePVar(strval(pid),"is_nocar");
            format(string,sizeof(string),"����Ա%s(%d)�������%s(%d)�����ؾ�",name,playerid,name1,strval(pid));
            AdminTips(-1,string);
        }
    }
    return 1;
}

//3��
COMMAND:setname(playerid,params[])
{
    new pid[18],name[64],name1[64],nname[64],string[256];
    if(IsAdmin(playerid,3)){
        if(sscanf(params,"s[18]s[64]",pid,nname)) return AdminTips(playerid,"�޸��������:/setname [���ID] [������]");
        GetPlayerName(playerid,name,sizeof(name));
        GetPlayerName(strval(pid),name1,sizeof(name1));
        SetPlayerName(strval(pid),nname);
        format(string,sizeof(string),"����Ա%s(%d)j�����%s(%d)����������Ϊ:%s",name,playerid,name1,strval(pid),nname);
        AdminTips(-1,string);
    }
    return 1;
}

COMMAND:jiangli(playerid,params[])
{
    new pid[18],name[64],name1[64],string[256];
    if(IsAdmin(playerid,3)){
        if(sscanf(params,"s[18]",pid)) return AdminTips(playerid,"�������10ʱ���:/jiangli [���ID]");
        GetPlayerName(playerid,name,sizeof(name));
        GetPlayerName(strval(pid),name1,sizeof(name1));
        format(string,sizeof(string),"����Ա%s(%d)�������%s(%d)10ʱ���",name,playerid,name1,strval(pid));
        AdminTips(-1,string);
        SetPlayerScore(strval(pid),GetPlayerScore(strval(pid))+10);        
    }
    return 1;
}

//4��
COMMAND:ban(playerid,params[])
{
    new pid[18],name[64],name1[64],string[256],reason[128];
    if(IsAdmin(playerid,4)){
        if(sscanf(params,"s[18]s[128]",pid,reason)) return AdminTips(playerid,"������:/ban [���ID]");
        GetPlayerName(playerid,name,sizeof(name));
        GetPlayerName(strval(pid),name1,sizeof(name1));
        format(string,sizeof(string),"����Ա%s(%d)���÷�����%s(%d),ԭ��:%s",name,playerid,name1,strval(pid),reason);
        AdminTips(-1,string);
        BanEx(strval(pid),reason);
    }
    return 1;
}

//5����Rcon��
COMMAND:setadmin(playerid,params[])
{
    new pid[18],name[64],name1[64],string[256],files[64],level[18];
    if(sscanf(params,"s[18]s[18]",pid,level)) return AdminTips(playerid,"������ҹ���ԱȨ��: /setadmin [���ID] [�ȼ�]");
    if(!IsPlayerConnected(strval(pid))) return AdminTips(playerid,"����Ҳ�����");
    GetPlayerName(playerid,name,sizeof(name));
    GetPlayerName(strval(pid),name1,sizeof(name1));
    if(IsAdmin(playerid,5)){
        format(files,sizeof(files),"\\Admins\\%s.ini",name);
        if(!fexist(files)) return AdminTips(playerid,"�����δע�����Ա�˺�");
        if(dini_Int(files,"Level") == 0){
            format(string,sizeof(string),"�߲�%s(%d)�����%s(%d)����Ϊ%d������Ա",name,playerid,name1,strval(pid),strval(level));
        }
        else{
            if(dini_Int(files,"Level") < strval(level)){
                format(string,sizeof(string),"�߲�%s(%d)��%d������Ա%s(%d)�ȼ�������%d��",name,playerid,dini_Int(files,"Level"),name1,strval(pid),strval(level));
            }
            if(dini_Int(files,"Level") >= strval(level)){
                format(string,sizeof(string),"�߲�%s(%d)��%d������Ա%s(%d)�ȼ�������%d��",name,playerid,dini_Int(files,"Level"),name1,strval(pid),strval(level));
            }
        } 
        AdminTips(-1,string);
        dini_IntSet(files,"Level",strval(level));
    }
    return 1;
}
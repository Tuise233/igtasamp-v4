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

COMMAND:setadmin(playerid,params[])
{
    new pid[18],name[64],name1[64],string[256],files[64],level[18];
    if(sscanf(params,"s[18]s[18]",pid,level)) return AdminTips(playerid,"������ҹ���ԱȨ��: /setadmin [���ID] [�ȼ�]");
    if(!IsPlayerConnected(strval(pid))) return AdminTips(playerid,"����Ҳ�����");
    GetPlayerName(playerid,name,sizeof(name));
    GetPlayerName(strval(pid),name1,sizeof(name1));
    if(IsAdmin(playerid,5)){
        format(files,sizeof(files),"\\Admins\\%s.ini",name1);
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

COMMAND:aregister(playerid,params[])
{
    new password[128],name[64],string[256],files[64];
    if(sscanf(params,"s[128]",password)) return AdminTips(playerid,"ע�����Ա�˺�: /aregister [����]");
    GetPlayerName(playerid,name,sizeof(name));
    format(files,sizeof(files),"\\Admins\\%s.ini",name);
    if(fexist(files)) return AdminTips(playerid,"����ע�������Ա�˺�,��������ע��");
    else{
        dini_Create(files);
        dini_Set(files,"Name",name);
        dini_Set(files,"Password",password);
        dini_IntSet(files,"Level",0);
        format(string,sizeof(string),"�ɹ�ע�����Ա�˺�,����ϵ�߲����Ա��Ȩ��");
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
    if(GetPVarInt(playerid,"is_admin")) return AdminTips(playerid,"������ִ��״̬,�����ٴε�½,����/logout�˳�ִ��״̬");
    GetPlayerName(playerid,name,sizeof(name));
    format(files,sizeof(files),"\\Admins\\%s.ini",name);
    if(sscanf(params,"s[128]",password)) return AdminTips(playerid,"����ִ��״̬: /alogin [����]");
    if(!strcmp(password,dini_Get(files,"password"))){
        if(!fexist(files)) return AdminTips(playerid,"�����ǹ���Ա,�޷�����ִ��");
        else{
            if(dini_Int(files,"Level") == 0) return AdminTips(playerid,"���Ĺ���Ա�˺�û��Ȩ��,����ϵ�߲����Ա��Ȩ�޺����ִ��");
            SetPVarInt(playerid,"is_admin",dini_Int(files,"Level"));
            SetPlayerColor(playerid,COLOR_RED);
            format(string,sizeof(string),"����Ա%s(%d)����ִ��״̬,����Ȩ�޵ȼ�:%d��",name,playerid,dini_Int(files,"Level"));
            AdminTips(-1,string);
            new times[64];
            format(times,sizeof(times),"%d.%d.%d %d:%d:%d",year,month,day,hour,mini,sec);
            dini_Set(files,"Time",times);
        }
    }
    else{
        AdminTips(playerid,"���������������,��˶Ժ���������");
    }
    return 1;
}

COMMAND:logout(playerid)
{
    if(!GetPVarInt(playerid,"is_admin")) return AdminTips(playerid,"������ִ��״̬,�����˳�ִ��");
    DeletePVar(playerid,"is_admin");
    new colors[6] = {0xFF80FFC8,0xFFFF80C8,0x00FF80C8,0x008000C8,0xFFFF80C8,0xFF8040C8};
    SetPlayerColor(playerid,colors[random(6)]);
    AdminTips(playerid,"�����˳�ִ��״̬");
    return 1;
}

//ͨ��
COMMAND:ip(playerid,params[])
{
    new pid[18],name[64],string[256];
    if(sscanf(params,"s[18]",pid)) return AdminTips(playerid,"��ѯ���IP:/ip [���ID]");
    if(IsAdmin(playerid,1))
    {
        new ip[64];
        if(IsPlayerConnected(strval(pid)))
        {
            GetPlayerIp(strval(pid),ip,sizeof(ip));
            GetPlayerName(strval(pid),name,sizeof(name));
            format(string,sizeof(string),"���%s(%d)��IP��ַ:%s",name,strval(pid),ip);
            AdminTips(playerid,string);
        }
        else
        {
            AdminTips(playerid,"����Ҳ�����");
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
    if(sscanf(params,"s[18]s[64]",pid,reason)) return AdminTips(playerid,"�ٱ����:/jubao [���ID] [ԭ��]");
    if(!IsPlayerConnected(strval(pid))) return AdminTips(playerid,"����Ҳ�����");
    GetPlayerName(playerid,name,sizeof(name));
    GetPlayerName(strval(pid),name1,sizeof(name1));
    format(string,sizeof(string),"���%s(%d)�ٱ����%s(%d),ԭ��:%s",name,playerid,name1,strval(pid),reason);
    for(new i=0;i<MAX_PLAYERS;i++){
        if(GetPVarInt(i,"is_admin")){
            AdminTips(i,string);
        }
    }
    AdminTips(playerid,"������Ϣ�ѷ��͸����߹���Ա,�ȴ�����Ա����!");
    format(string,sizeof(string),"IGTASAMP - �ٱ�֪ͨ\n�ٱ���:%s(%d)\n���ٱ���:%s(%d)\nԭ��:%s\n\n�����Ա�������ߴ���!",name,playerid,name1,strval(pid),reason);
    new files[64];
    format(files,sizeof(files),"\\Robot\\Report.txt");
    fwrite_utf8(files,string);
    return 1;
}

COMMAND:gms(playerid)
{
    new a=0;
    new name[64],string[256];
    AdminTips(playerid,"��ǰ���߹���Ա");
    for(new i=0;i<MAX_PLAYERS;i++){
        if(GetPVarInt(i,"is_admin")){
            GetPlayerName(i,name,sizeof(name));
            format(string,sizeof(string),"%s(%d) - Ȩ�޵ȼ�:%d��",name,i,GetPVarInt(i,"is_admin"));
            AdminTips(playerid,string);
            a += 1;
        }
    }
    if(a == 0){
        AdminTips(playerid,"��ǰû�����߹���Ա");
    }
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

COMMAND:goto(playerid,params[])
{
    new pid[18];
    if(IsAdmin(playerid,1))
    {
        new Float:x,Float:y,Float:z;
        if(sscanf(params,"s[18]",pid)) return AdminTips(playerid,"���͵���Ҹ���");
        if(!IsPlayerConnected(strval(pid))) return AdminTips(playerid,"����Ҳ�����");
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
        if(sscanf(params,"s[18]",pid)) return AdminTips(playerid,"����Ҵ��͵��㸽��");
        if(!IsPlayerConnected(strval(pid))) return AdminTips(playerid,"����Ҳ�����");
        GetPlayerPos(playerid,x,y,z);
        SetPlayerPos(strval(pid),x+5,y+5,z);
        return 1;
    }
    return 1;
}

//1��
COMMAND:warn(playerid,params[])
{
    new name[64],name1[64],reason[128],string[256],pid[18];
    new Float:x,Float:y,Float:z;
    if(sscanf(params,"s[18]s[128]",pid,reason)) return AdminTips(playerid,"�������: /warn [���ID] [ԭ��]");
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

COMMAND:mute(playerid,params[])
{
    new name[64],name1[64],pid[18],string[256];
    if(sscanf(params,"s[18]",pid)) return AdminTips(playerid,"�������: /mute [���ID]");
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

COMMAND:unmute(playerid,params[])
{
    new name[64],name1[64],string[256],pid[18];
    if(sscanf(params,"s[18]",pid)) return AdminTips(playerid,"������: /unmute [���ID]");
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

COMMAND:jail(playerid,params[])
{
    new name[64],name1[64],string[256],pid[18],sec[18];
    if(sscanf(params,"s[18]s[18]",pid,sec)) return AdminTips(playerid,"������: /jail [���ID] [����]");
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

COMMAND:clear(playerid)
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

COMMAND:delc(playerid,params[])
{
    new name[64],name1[64],string[256],pid[18];
    if(IsAdmin(playerid,1))
    {
        if(sscanf(params,"s[18]",pid)) return AdminTips(playerid,"ɾ������ؾ�: /delc [���ID]");
        if(!IsPlayerConnected(strval(pid))) return AdminTips(playerid,"����Ҳ�����");
        if(PlayerVehicle[strval(pid)] == -1) return AdminTips(playerid,"�����û�������κ��ؾ�");
        DestroyVehicle(PlayerVehicle[strval(pid)]);
        PlayerVehicle[strval(pid)] = -1;
        GetPlayerName(playerid,name,sizeof(name));
        GetPlayerName(strval(pid),name1,sizeof(name1));
        format(string,sizeof(string),"����Ա%s(%d)ɾ�������%s(%d)���ɵ��ؾ�",name,playerid,name1,strval(pid));
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
        SendClientMessage(playerid,COLOR_RED,"[T-Admin]���޸�����ϵͳ");
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

//2��
COMMAND:kick(playerid,params[])
{
    new name[64],name1[64],string[256],pid[18],reason[128];
    if(sscanf(params,"s[18]s[128]",pid,reason)) AdminTips(playerid,"�߳����: /kick [���ID] [ԭ��]");
    if(IsAdmin(playerid,2)){
        if(!IsPlayerConnected(strval(pid))) return AdminTips(playerid,"����Ҳ�����");
        GetPlayerName(playerid,name,sizeof(name));
        GetPlayerName(strval(pid),name1,sizeof(name1));
        format(string,sizeof(string),"����Ա%s(%d)�����%s(%d)�߳�������,ԭ��:%s",name,playerid,name1,strval(pid),reason);
        AdminTips(-1,string);
        SetPVarInt(strval(pid),"is_kick_ex",5);
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

COMMAND:freeze(playerid,params[])
{
    new pid[18],name[64],name1[64],string[256];
    if(sscanf(params,"s[18]",pid)) return AdminTips(playerid,"��ֹ����ж�:/freeze [���ID]");
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

COMMAND:ban(playerid,params[])
{
    new pid[18],name[64],name1[64],string[256],reason[128];
    if(IsAdmin(playerid,3)){
        if(sscanf(params,"s[18]s[128]",pid,reason)) return AdminTips(playerid,"������:/ban [���ID]");
        GetPlayerName(playerid,name,sizeof(name));
        GetPlayerName(strval(pid),name1,sizeof(name1));
        format(string,sizeof(string),"����Ա%s(%d)���÷�����%s(%d),ԭ��:%s",name,playerid,name1,strval(pid),reason);
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
            format(string,sizeof(string),"����Ա%s(%d)������ȫ�����",playerid,name);
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
            format(string,sizeof(string),"����Ա%s(%d)�ر���ȫ�����",playerid,name);
        }
        AdminTips(-1,string);
    }
    return 1;
}
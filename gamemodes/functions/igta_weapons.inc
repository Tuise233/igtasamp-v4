
//For Android Players
COMMAND:wq(playerid,params[])
{
    if(GetPVarInt(playerid,"is_rp")) return RpTips(playerid,"RPģʽ���޷���������");
    if(IsPlayerConnected(playerid)) return ServerTips(playerid,"��׿����ϵͳ��ʱȡ��");
    new wid[64];
    new Float:x,Float:y,Float:z;
    GetPlayerPos(playerid,x,y,z);
    if(sscanf(params,"s[64]",wid)) return ServerTips(playerid,"[����ϵͳ]����ϵͳָ���ȫ:/wq help");
    if(!strval(wid))
    {
        if(!strcmp(wid,"help",true))
        {
            ServerTips(playerid,"[����ϵͳ]����һ������:/wq [�������] || �鿴�������:/wq list");
            return 1;
        }
        if(!strcmp(wid,"list",true))
        {
            ServerTips(playerid,"������Ŵ�ȫ");
            ServerTips(playerid,"348[ɳӥ] || 355[AK47] || 355[M4A1] || 358[�ѻ���ǹ]");
            ServerTips(playerid,"359[RPG] || 349[����ǹ] || 342[����] || 335[С��]");
            ServerTips(playerid,"���ڸ�ϵͳ��Ϊ���԰汾,Ŀǰֻ������������");
            return 1;
        }
        return 1;
    }
    if(strval(wid))
    {
        if(strval(wid) == 348)
        {
            CreatePickup(strval(wid),3,x,y,z,GetPlayerVirtualWorld(playerid));
            CreatePickup(strval(wid),3,x,y,z,GetPlayerVirtualWorld(playerid));
            CreatePickup(strval(wid),3,x,y,z,GetPlayerVirtualWorld(playerid));
            ServerTips(playerid,"[����ϵͳ]��Ϊ������ɳӥ");
            return 1;
        }
        if(strval(wid) == 355)
        {
            CreatePickup(strval(wid),3,x,y,z,GetPlayerVirtualWorld(playerid));
            CreatePickup(strval(wid),3,x,y,z,GetPlayerVirtualWorld(playerid));
            CreatePickup(strval(wid),3,x,y,z,GetPlayerVirtualWorld(playerid));
            ServerTips(playerid,"[����ϵͳ]��Ϊ������AK47");
            return 1;
        }
        if(strval(wid) == 355)
        {
            CreatePickup(strval(wid),3,x,y,z,GetPlayerVirtualWorld(playerid));
            CreatePickup(strval(wid),3,x,y,z,GetPlayerVirtualWorld(playerid));
            CreatePickup(strval(wid),3,x,y,z,GetPlayerVirtualWorld(playerid));
            ServerTips(playerid,"[����ϵͳ]��Ϊ������M4A1");
            return 1;
        }
        if(strval(wid) == 358)
        {
            CreatePickup(strval(wid),3,x,y,z,GetPlayerVirtualWorld(playerid));
            CreatePickup(strval(wid),3,x,y,z,GetPlayerVirtualWorld(playerid));
            CreatePickup(strval(wid),3,x,y,z,GetPlayerVirtualWorld(playerid));
            ServerTips(playerid,"[����ϵͳ]��Ϊ�����ɾѻ���ǹ");
            return 1;
        }
        if(strval(wid) == 359)
        {
            CreatePickup(strval(wid),3,x,y,z,GetPlayerVirtualWorld(playerid));
            CreatePickup(strval(wid),3,x,y,z,GetPlayerVirtualWorld(playerid));
            CreatePickup(strval(wid),3,x,y,z,GetPlayerVirtualWorld(playerid));
            ServerTips(playerid,"[����ϵͳ]��Ϊ������RPG");
            return 1;
        }
        if(strval(wid) == 349)
        {
            CreatePickup(strval(wid),3,x,y,z,GetPlayerVirtualWorld(playerid));
            CreatePickup(strval(wid),3,x,y,z,GetPlayerVirtualWorld(playerid));
            CreatePickup(strval(wid),3,x,y,z,GetPlayerVirtualWorld(playerid));
            ServerTips(playerid,"[����ϵͳ]��Ϊ����������ǹ");
            return 1;
        }
        if(strval(wid) == 342)
        {
            CreatePickup(strval(wid),3,x,y,z,GetPlayerVirtualWorld(playerid));
            CreatePickup(strval(wid),3,x,y,z,GetPlayerVirtualWorld(playerid));
            CreatePickup(strval(wid),3,x,y,z,GetPlayerVirtualWorld(playerid));
            ServerTips(playerid,"[����ϵͳ]��Ϊ����������");
            return 1;
        }
        if(strval(wid) == 335)
        {
            CreatePickup(strval(wid),3,x,y,z,GetPlayerVirtualWorld(playerid));
            CreatePickup(strval(wid),3,x,y,z,GetPlayerVirtualWorld(playerid));
            CreatePickup(strval(wid),3,x,y,z,GetPlayerVirtualWorld(playerid));
            ServerTips(playerid,"[����ϵͳ]��Ϊ������С��");
            return 1;
        }
        else
        {
            ServerTips(playerid,"[����ϵͳ]����������ʱ������,Ŀǰ���ŵ�����:/wq list");
        }
        return 1;
    }
    return 1;
}


//For PC Players
COMMAND:wuqi(playerid)
{
    if(GetPVarInt(playerid,"is_rp")) return RpTips(playerid,"RPģʽ���޷���������");
	ShowPlayerDialog(playerid,3100,DIALOG_STYLE_LIST,"IGTA����ϵͳ","�ֳ���\nͶ����\n��ǹ��\n���ǹ\nɢ��ǹ\n��ǹ��\n������\n������\nѪ����","ȷ��","ȡ��");
	return 1;
}
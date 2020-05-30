
//For Android Players
COMMAND:wq(playerid,params[])
{
    if(GetPVarInt(playerid,"is_rp")) return RpTips(playerid,"RP模式内无法生成武器");
    if(IsPlayerConnected(playerid)) return ServerTips(playerid,"安卓武器系统暂时取消");
    new wid[64];
    new Float:x,Float:y,Float:z;
    GetPlayerPos(playerid,x,y,z);
    if(sscanf(params,"s[64]",wid)) return ServerTips(playerid,"[武器系统]武器系统指令大全:/wq help");
    if(!strval(wid))
    {
        if(!strcmp(wid,"help",true))
        {
            ServerTips(playerid,"[武器系统]生成一个武器:/wq [武器编号] || 查看武器编号:/wq list");
            return 1;
        }
        if(!strcmp(wid,"list",true))
        {
            ServerTips(playerid,"武器编号大全");
            ServerTips(playerid,"348[沙鹰] || 355[AK47] || 355[M4A1] || 358[狙击步枪]");
            ServerTips(playerid,"359[RPG] || 349[霰弹枪] || 342[手雷] || 335[小刀]");
            ServerTips(playerid,"由于该系统仅为测试版本,目前只开放以上武器");
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
            ServerTips(playerid,"[武器系统]已为您生成沙鹰");
            return 1;
        }
        if(strval(wid) == 355)
        {
            CreatePickup(strval(wid),3,x,y,z,GetPlayerVirtualWorld(playerid));
            CreatePickup(strval(wid),3,x,y,z,GetPlayerVirtualWorld(playerid));
            CreatePickup(strval(wid),3,x,y,z,GetPlayerVirtualWorld(playerid));
            ServerTips(playerid,"[武器系统]已为您生成AK47");
            return 1;
        }
        if(strval(wid) == 355)
        {
            CreatePickup(strval(wid),3,x,y,z,GetPlayerVirtualWorld(playerid));
            CreatePickup(strval(wid),3,x,y,z,GetPlayerVirtualWorld(playerid));
            CreatePickup(strval(wid),3,x,y,z,GetPlayerVirtualWorld(playerid));
            ServerTips(playerid,"[武器系统]已为您生成M4A1");
            return 1;
        }
        if(strval(wid) == 358)
        {
            CreatePickup(strval(wid),3,x,y,z,GetPlayerVirtualWorld(playerid));
            CreatePickup(strval(wid),3,x,y,z,GetPlayerVirtualWorld(playerid));
            CreatePickup(strval(wid),3,x,y,z,GetPlayerVirtualWorld(playerid));
            ServerTips(playerid,"[武器系统]已为您生成狙击步枪");
            return 1;
        }
        if(strval(wid) == 359)
        {
            CreatePickup(strval(wid),3,x,y,z,GetPlayerVirtualWorld(playerid));
            CreatePickup(strval(wid),3,x,y,z,GetPlayerVirtualWorld(playerid));
            CreatePickup(strval(wid),3,x,y,z,GetPlayerVirtualWorld(playerid));
            ServerTips(playerid,"[武器系统]已为您生成RPG");
            return 1;
        }
        if(strval(wid) == 349)
        {
            CreatePickup(strval(wid),3,x,y,z,GetPlayerVirtualWorld(playerid));
            CreatePickup(strval(wid),3,x,y,z,GetPlayerVirtualWorld(playerid));
            CreatePickup(strval(wid),3,x,y,z,GetPlayerVirtualWorld(playerid));
            ServerTips(playerid,"[武器系统]已为您生成霰弹枪");
            return 1;
        }
        if(strval(wid) == 342)
        {
            CreatePickup(strval(wid),3,x,y,z,GetPlayerVirtualWorld(playerid));
            CreatePickup(strval(wid),3,x,y,z,GetPlayerVirtualWorld(playerid));
            CreatePickup(strval(wid),3,x,y,z,GetPlayerVirtualWorld(playerid));
            ServerTips(playerid,"[武器系统]已为您生成手雷");
            return 1;
        }
        if(strval(wid) == 335)
        {
            CreatePickup(strval(wid),3,x,y,z,GetPlayerVirtualWorld(playerid));
            CreatePickup(strval(wid),3,x,y,z,GetPlayerVirtualWorld(playerid));
            CreatePickup(strval(wid),3,x,y,z,GetPlayerVirtualWorld(playerid));
            ServerTips(playerid,"[武器系统]已为您生成小刀");
            return 1;
        }
        else
        {
            ServerTips(playerid,"[武器系统]其他武器暂时不开放,目前开放的武器:/wq list");
        }
        return 1;
    }
    return 1;
}


//For PC Players
COMMAND:wuqi(playerid)
{
    if(GetPVarInt(playerid,"is_rp")) return RpTips(playerid,"RP模式内无法生成武器");
	ShowPlayerDialog(playerid,3100,DIALOG_STYLE_LIST,"IGTA武器系统","手持类\n投掷类\n手枪类\n冲锋枪\n散弹枪\n步枪类\n特殊类\n其他类\n血甲类","确定","取消");
	return 1;
}

// igta_vip

forward vip_OnPlayerConnect(playerid);
public vip_OnPlayerConnect(playerid)
{
    new name[64],files[64];
    GetPlayerName(playerid,name,sizeof(name));
    format(files,sizeof(files),"\\Server\\VIP\\Members\\%s.ini",name);
    if(fexist(files))
    {
        SetPVarInt(playerid,"is_vip",!GetPVarInt(playerid,"is_vip"));
        new string[256];
        format(string,sizeof(string),"[VIP系统]尊贵的VIP玩家%s(%d)上线了,各位欢迎~",name,playerid);
        SendClientMessageToAll(COLOR_VIP,string);
    }

    if(dini_Unset(files,"Ch"))
    {
        dini_IntSet(files,"Ch",0);
    }
    if(dini_Unset(files,"Xwb"))
    {
        dini_IntSet(files,"Xwb",0);
    }
    return 1;
}

forward OrderSongEx();
public OrderSongEx()
{
    for(new i=0;i<MAX_PLAYERS;i++)
    {
        
    }
}

COMMAND:vip(playerid,params[])
{
    new params1[256],params2[256],params3[256],name[64],string[256];
    GetPlayerName(playerid,name,sizeof(name));
    if(sscanf(params,"s[256]s[256]s[256]",params1,params2,params3))
    {
        if(!strlen(params1)) return SendClientMessage(playerid,COLOR_VIP,"[VIP系统]会员系统使用说明: /vip help");
    }

    if(!strcmp(params1,"help",true))
    {
        SendClientMessage(playerid,COLOR_VIP,"VIP系统帮助大全");
        SendClientMessage(playerid,COLOR_VIP,"卡密购买地址:http://u6.gg/sUncz");
        SendClientMessage(playerid,COLOR_VIP,"VIP充值续费:/vip add");
        SendClientMessage(playerid,COLOR_RED,"目前开放的只有VIP传送与VIP踢人!");
        SendClientMessage(playerid,COLOR_VIP,"VIP传送:/vip tp  ||  VIP踢人:/vip kick  ||  全服点歌:/vip music");
        SendClientMessage(playerid,COLOR_VIP,"彩虹车:/vip car  ||  VIP头衔: /vip show");
        SendClientMessage(playerid,COLOR_VIP,"修改称号:/vip ch  ||  修改小尾巴:/vip xwb");
        SendClientMessage(playerid,COLOR_VIP,"更多VIP功能敬请期待!");
        return 1;
    }

    if(!strcmp(params1,"add",true))
    {
        if(!strlen(params2)) return SendClientMessage(playerid,COLOR_VIP,"[VIP系统]VIP充值续费: /vip add [卡密]");
        new files[64],vfiles[64];
        //判断是否已经充值
        format(files,sizeof(files),"\\Server\\VIP\\Members\\%s.ini",name);
        format(vfiles,sizeof(vfiles),"\\Server\\VIP\\%s.ini",params2);
        if(!fexist(vfiles)) return SendClientMessage(playerid,COLOR_VIP,"[VIP系统]您输入的卡密有误,请核对后重新输入");
        if(!fexist(files))
        {
            dini_Create(files);
            dini_Set(files,"Name",name);
            dini_IntSet(files,"Time",dini_Int(vfiles,"Time"));
            format(string,sizeof(string),"[VIP系统]充值成功,您使用了一张[%s]卡,剩余时长:%d秒",dini_Int(vfiles,name),dini_Int(files,"Time"));
            SendClientMessage(playerid,COLOR_VIP,string);
            format(string,sizeof(string),"[VIP系统]玩家%s(%d)VIP充值成功,使用了一张[%s]卡,剩余时长:%d秒",name,playerid,dini_Int(vfiles,"Name"),dini_Int(files,"Time"));
            SendClientMessageToAll(COLOR_VIP,string);
            fremove(vfiles);
            SetPVarInt(playerid,"is_vip",!GetPVarInt(playerid,"is_vip"));
            return 1;
        }
        else
        {
            dini_IntSet(files,"Time",dini_Int(vfiles,"Time"));
            format(string,sizeof(string),"[VIP系统]充值成功,您使用了一张[%s]卡,剩余时长:%d秒",dini_Get(vfiles,name),dini_Int(files,"Time"));
            SendClientMessage(playerid,COLOR_VIP,string);
            format(string,sizeof(string),"[VIP系统]玩家%s(%d)VIP充值成功,使用了一张[%s]卡,剩余时长:%d秒",name,playerid,dini_Get(vfiles,"Name"),dini_Int(files,"Time"));
            SendClientMessageToAll(COLOR_VIP,string);
            fremove(vfiles);
            SetPVarInt(playerid,"is_vip",!GetPVarInt(playerid,"is_vip"));
            return 1;
        }
    }

    if(!GetPVarInt(playerid,"is_vip")) return SendClientMessage(playerid,COLOR_VIP,"[VIP系统]您不是VIP,您无权使用");
    if(!strcmp(params1,"tp",true))
    {
        if(!strlen(params2)) return SendClientMessage(playerid,COLOR_VIP,"[VIP系统]VIP传送: /vip tp [玩家ID]");
        new pid = strval(params2);
        if(!IsPlayerConnected(pid)) return SendClientMessage(playerid,COLOR_VIP,"[VIP系统]该玩家不在线");
        new name1[64];
        if(GetPVarInt(pid,"is_rp")) return SendClientMessage(playerid,COLOR_RED,"[T-AntiCheat]您不可以通过VIP传送传送至RP世界");
        if(GetPVarInt(pid,"is_dm")) return SendClientMessage(playerid,COLOR_RED,"[T-AntiCheat]您不可以通过VIP传送传送至DM世界");
        GetPlayerName(pid,name1,sizeof(name1));
        format(string,sizeof(string),"[VIP系统]玩家%s(%d)使用VIP传送传送至你的当前世界与位置",name,playerid);
        SendClientMessage(pid,COLOR_VIP,string);
        format(string,sizeof(string),"[VIP系统]你通过使用VIP传送传送至玩家%s(%d)的当前世界与位置",name1,pid);
        SendClientMessage(playerid,pid,string);
        new Float:x,Float:y,Float:z;
        GetPlayerPos(pid,x,y,z);
        SetPlayerVirtualWorld(playerid, GetPlayerVirtualWorld(pid));
        if(GetPlayerVehicleSeat(playerid) == 0)
        {
            new vid = GetPlayerVehicleID(playerid);
            SetVehiclePos(vid,x,y,z);
            SetVehicleVirtualWorld(vid, GetPlayerVirtualWorld(pid));
            PutPlayerInVehicle(playerid, vid, 0);
        }
        SetPlayerPos(playerid,x,y,z);
        return 1;
    }

    if(!strcmp(params1,"kick",true))
    {
        if(!strlen(params2)) return SendClientMessage(playerid,COLOR_VIP,"[VIP系统]VIP踢人; /vip kick [玩家ID] [理由]");
        if(!strlen(params3)) return SendClientMessage(playerid,COLOR_VIP,"[VIP系统]VIP踢人; /vip kick [玩家ID] [理由]");
        new pid = strval(params2);
        if(!IsPlayerConnected(pid)) return SendClientMessage(playerid,COLOR_VIP,"[VIP系统]该玩家不在线");
        if(GetPVarInt(pid,"is_admin"))
        {
            format(string,sizeof(string),"[T-Admin]VIP%s(%d)尝试踢出管理员,已被系统拦截并踢出服务器",name,playerid);
            SendClientMessageToAll(COLOR_RED,string);
            Kick(playerid);
            return 1;   
        }
        new name1[64];
        GetPlayerName(pid,name1,sizeof(name1));
        format(string,sizeof(string),"[VIP系统]玩家%s(%d)使用VIP踢人将玩家%s(%d)踢出服务器,理由:%s",name,playerid,name1,pid,params3);
        SendClientMessageToAll(COLOR_VIP,string);
        SendClientMessageToAll(COLOR_VIP,"[VIP系统]有玩家恶意使用VIP功能踢人?立刻向褪色反馈,查清后永久撤销VIP权限");
        SendClientMessageToAll(COLOR_VIP,"[VIP系统]并奖励举报者3天VIP");
        Kick(pid);
        format(string,sizeof(string),"IGTASMAP - VIP系统\nVIP用户:%s(%d)\n操作: VIP踢人\n被操作用户:%s(%d)\n原因:%s",name,playerid,name1,pid,params3);
        new files[64];
        format(files,sizeof(files),"\\Robot\\VIPSystem.txt");
        new File:fl = fopen(files,io_write);
        for(new i=0;string[i];i++)
        {
            fputchar(fl,string[i],false);
        }
        fclose(fl);
        return 1;
    }

    if(!strcmp(params1,"music",true))
    {
        if(!strlen(params2)) return SendClientMessage(playerid,COLOR_VIP,"[VIP系统]VIP全服点歌: /vip music [音乐外链]");
        format(string,sizeof(string),"[VIP系统]玩家%s(%d)使用VIP点歌为全服玩家点了一首歌",name,playerid);
        SendClientMessageToAll(COLOR_VIP,string);
        SendClientMessageToAll(COLOR_VIP,"[VIP系统]当前点歌仅电脑玩家听见,输入/stopmusic来关闭音乐");
        for(new i=0;i<MAX_PLAYERS;i++)
        {
            if(IsPlayerConnected(i))
            {
                PlayAudioStreamForPlayer(i, params2);
            }
        }
        return 1;
    }

    if(!strcmp(params1,"car",true))
    {
        if(!GetPVarInt(playerid,"is_vip_car"))
        {
            SetPVarInt(playerid,"is_vip_car",1);
            SendClientMessage(playerid,COLOR_VIP,"[VIP系统]您已开启彩虹车,您的载具会随机更换颜色");
            SendClientMessage(playerid,COLOR_VIP,"[VIP系统]手机玩家暂时无法显示载具的颜色更换");
        }
        else
        {
            DeletePVar(playerid,"is_vip_car");
            SendClientMessage(playerid,COLOR_VIP,"[VIP系统]您已关闭彩虹车");
        }
        return 1;
    }

    if(!strcmp(params1,"show",true))
    {
        if(!GetPVarInt(playerid,"is_vip_show"))
        {
            SendClientMessage(playerid,COLOR_VIP,"[VIP系统]VIP头衔展示在新版IGTASAMP才可以使用");
        }
        return 1;
    }

    if(!strcmp(params1,"clear",true))
    {
        for(new i=0;i<15;i++)
        {
            SendClientMessageToAll(COLOR_VIP," ");
        }
        format(string,sizeof(string),"[VIP系统]玩家%s(%d)使用VIP清屏清除了公屏对话",name,playerid);
        SendClientMessageToAll(COLOR_VIP,string);
    }

    if(!strcmp(params1,"ch",true))
    {
        new files[64];
        if(!strlen(params2)) return SendClientMessage(playerid,COLOR_VIP,"[VIP系统]自定义称号: /vip ch [称号] (成为即可获得两次修改机会)");
        format(files,sizeof(files),"\\Server\\VIP\\Members\\%s.ini",name);
        if(dini_Int(files,"Ch") == 2) return SendClientMessage(playerid,COLOR_VIP,"[VIP系统]您已使用完两次修改称号的机会");
        format(files,sizeof(files),"\\Players\\%s.ini",name);
        dini_Set(files,"Ch",params2);
        format(string,sizeof(string),"[VIP系统]VIP%s(%d)修改了自己的称号: %s",name,playerid,params2);
        SendClientMessageToAll(COLOR_VIP,string);
        format(files,sizeof(files),"\\Server\\VIP\\Members\\%s.ini",name);
        dini_IntSet(files,"Ch",dini_Int(files,"Ch") + 1);
    }

    if(!strcmp(params1,"xwb",true))
    {
        new files[64];
        if(!strlen(params2)) return SendClientMessage(playerid,COLOR_VIP,"[VIP系统]自定义称号: /vip xwb [小尾巴] (成为即可获得两次修改机会)");
        format(files,sizeof(files),"\\Server\\VIP\\Members\\%s.ini",name);
        if(dini_Int(files,"Xwb") == 2) return SendClientMessage(playerid,COLOR_VIP,"[VIP系统]您已使用完两次修改小尾巴的机会");
        format(files,sizeof(files),"\\Players\\%s.ini",name);
        dini_Set(files,"Xwb",params2);
        format(string,sizeof(string),"[VIP系统]VIP%s(%d)修改了自己的小尾巴: %s",name,playerid,params2);
        SendClientMessageToAll(COLOR_VIP,string);
        format(files,sizeof(files),"\\Server\\VIP\\Members\\%s.ini",name);
        dini_IntSet(files,"Xwb",dini_Int(files,"Xwb") + 1);
    }
    return 1;
}

forward RGBCar();
public RGBCar()
{
    for(new i=0;i<MAX_PLAYERS;i++)
    {
        if(GetPVarInt(i,"is_vip_car"))
        {
            if(GetPlayerVehicleSeat(i) == 0)
            {
                if(!GetPVarInt(i,"is_rp"))
                {
                    ChangeVehicleColor(GetPlayerVehicleID(i), random(20), random(20));
                }
            }
        }
    }
    return 1;
}
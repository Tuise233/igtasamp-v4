forward CheckLogin();
public CheckLogin()
{
    for(new i=0;i<MAX_PLAYERS;i++)
    {
        if(!GetPVarInt(i,"is_login"))
        {
            ServerTips(i,"输入/l [密码]进行登陆或注册");
        }
    }
    return 1;
}

forward InitLoginPlayer(playerid);
public InitLoginPlayer(playerid)
{
    new name[64],files[64],ip[64];
    GetPlayerName(playerid,name,sizeof(name));
    GetPlayerIp(playerid,ip,sizeof(ip));
    format(files,sizeof(files),"\\Players\\%s.ini",name);
    dini_Set(files,"IP",ip);
    SetPlayerScore(playerid,dini_Int(files,"Score"));
    SetPlayerSkin(playerid,dini_Int(files,"Skin"));
    SetPVarInt(playerid,"is_mtp",!GetPVarInt(playerid,"is_mtp"));
    SetPVarInt(playerid,"is_god",99);
    ServerTips(playerid,"以为您自动开启无敌模式,输入 /wudi 以关闭");
    return 1;
}

COMMAND:l(playerid,params[])
{
    new name[64],pwd[128],files[64],ip[64];
    GetPlayerName(playerid,name,sizeof(name));
    GetPlayerIp(playerid,ip,sizeof(ip));
    format(files,sizeof(files),"\\Players\\%s.ini",name);
    if(sscanf(params,"s[128]",pwd)) return ServerTips(playerid,"注册账号/l [密码]");
    if(!GetPVarInt(playerid,"is_login")){
        if(!fexist(files))
        {
            dini_Create(files);
            dini_Set(files,"Name",name);
            dini_Set(files,"Password",pwd);
            dini_Set(files,"IP",ip);
            dini_IntSet(files,"Score",0);
            dini_IntSet(files,"Skin",1);
            dini_IntSet(files,"Money",0);
            dini_Set(files,"Ch","none");
            dini_Set(files,"Xwb","none");
            ServerTips(playerid,"您已成功注册账号,请牢记密码,下次登陆时将会使用到密码");
            SetPVarInt(playerid,"is_login",!GetPVarInt(playerid,"is_login"));
            TogglePlayerControllable(playerid, 1);
            InitLoginPlayer(playerid);
            return 1;
        }
        else
        {
            if(!strcmp(pwd,dini_Get(files,"Password"),true))
            {
                SetPVarInt(playerid,"is_login",!GetPVarInt(playerid,"is_login"));
                ServerTips(playerid,"登陆成功,欢迎回来!");
                TogglePlayerControllable(playerid, 1);
                InitLoginPlayer(playerid);
            }
            else{
                ServerTips(playerid,"您输入的密码有误,请核对后重新输入");
                return 1;
            }
        }
    }
    return 1;
}
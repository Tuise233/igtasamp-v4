forward CheckLogin();
public CheckLogin()
{
    for(new i=0;i<MAX_PLAYERS;i++)
    {
        if(!GetPVarInt(i,"is_login"))
        {
            ServerTips(i,"����/l [����]���е�½��ע��");
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
    ServerTips(playerid,"��Ϊ���Զ������޵�ģʽ,���� /wudi �Թر�");
    return 1;
}

COMMAND:l(playerid,params[])
{
    new name[64],pwd[128],files[64],ip[64];
    GetPlayerName(playerid,name,sizeof(name));
    GetPlayerIp(playerid,ip,sizeof(ip));
    format(files,sizeof(files),"\\Players\\%s.ini",name);
    if(sscanf(params,"s[128]",pwd)) return ServerTips(playerid,"ע���˺�/l [����]");
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
            ServerTips(playerid,"���ѳɹ�ע���˺�,���μ�����,�´ε�½ʱ����ʹ�õ�����");
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
                ServerTips(playerid,"��½�ɹ�,��ӭ����!");
                TogglePlayerControllable(playerid, 1);
                InitLoginPlayer(playerid);
            }
            else{
                ServerTips(playerid,"���������������,��˶Ժ���������");
                return 1;
            }
        }
    }
    return 1;
}
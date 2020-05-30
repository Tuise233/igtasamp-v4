//400~611
new PlayerVehicle[MAX_PLAYERS];
new Text3D:PlayerVehicleText[MAX_PLAYERS];
new PlayerVehicleTextNumber[MAX_PLAYERS];

COMMAND:c(playerid,params[])
{
    new params1[64],params2[64],params3[64];
    if(sscanf(params,"s[64]s[64]s[64]",params1,params2,params3))
    {
        if(!strlen(params1)) return ServerTips(playerid,"载具系统帮助 /c help");
    }

    if(strval(params1))
    {
        if(GetPVarInt(playerid,"is_rp")) return RpTips(playerid,"RP模式禁用载具系统,请使用/rent租用载具");
        new vehBan[] = {403, 406, 407, 408, 417, 425, 432, 433, 435, 443, 450, 455, 478, 486, 520, 524, 532, 544, 569, 570, 584, 590, 591, 592};
        if(GetPlayerVirtualWorld(playerid) == 0 )
        {
            for(new i=0;i<24;i++)
            {
                if(strval(params1) == vehBan[i]) return ServerTips(playerid,"该载具为服务器大世界禁止载具,大世界不可用!");
            }
        }
        new Float:x,Float:y,Float:z,Float:a;
        GetPlayerPos(playerid,x,y,z);
        GetPlayerFacingAngle(playerid,a);
        if(strval(params1) < 400 || strval(params1) > 611) return ServerTips(playerid,"载具ID在400~611之间");
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
        new FDCar[] = {416,407}
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
        ServerTips(playerid,"载具生成成功");
        return 1;
    }

    if(!strcmp(params1,"help",true))
    {
        ServerTips(playerid,"载具系统帮助");
        ServerTips(playerid,"生成载具 /c [载具ID] || 召回载具 /c wode || 修改载具颜色 /c color");
        ServerTips(playerid,"上锁/解锁载具 /c suo || 开启/关闭自动修复 /autofix || 修复载具 /fix");
        return 1;
    }

    if(!strcmp(params1,"suo",true))
    {
        if(PlayerVehicle[playerid] == -1) return ServerTips(playerid,"您还没有专属载具");
        if(!GetPVarInt(playerid,"is_car_lock")){
            SetPVarInt(playerid,"is_car_lock",!GetPVarInt(playerid,"is_car_lock"));
            ServerTips(playerid,"您已上锁您的专属载具");
            return 1;
        }
        DeletePVar(playerid,"is_car_lock");
        ServerTips(playerid,"您已解锁您的专属载具");
        return 1;
    }

    if(!strcmp(params1,"color",true))
    {
        if(!IsPlayerInAnyVehicle(playerid)) return ServerTips(playerid,"您不在任何载具上");
        if(GetPlayerVehicleSeat(playerid) != 0) return ServerTips(playerid,"您不是该载具的司机");
        if(!strlen(params2)) return ServerTips(playerid,"修改载具颜色 /c color [颜色ID1] [颜色ID2(可不填)]");
        ServerTips(playerid,"修改载具颜色成功");
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
        if(GetPVarInt(playerid,"is_rp")) return RpTips(playerid,"RP模式禁用载具系统,请使用/rent租用载具");
        new vehBan[] = {403, 406, 407, 408, 417, 425, 432, 433, 435, 443, 450, 455, 478, 486, 520, 524, 532, 544, 569, 570, 584, 590, 591, 592};
        for(new i=0;i<24;i++)
        {
            if(PlayerVehicle[playerid] == vehBan[i]) return SendClientMessage(playerid,COLOR_RED,"[T-AntiCheat]您在小世界生成的载具是禁止在大世界使用的,无法召回!");
        }
        if(PlayerVehicle[playerid] == -1) return ServerTips(playerid,"您还没有专属载具");
        new Float:x,Float:y,Float:z,Float:a;
        GetPlayerPos(playerid,x,y,z);
        GetPlayerFacingAngle(playerid,a);
        SetVehiclePos(PlayerVehicle[playerid],x,y,z);
        SetVehicleZAngle(PlayerVehicle[playerid],a);
        SetVehicleVirtualWorld(PlayerVehicle[playerid],GetPlayerVirtualWorld(playerid));
        PutPlayerInVehicle(playerid,PlayerVehicle[playerid],0);
        ServerTips(playerid,"载具召回成功");
        return 1;
    }
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

forward Autofix();
public Autofix()
{
    for(new i=0;i<MAX_PLAYERS;i++)
    {
        if(GetPVarInt(i,"is_autofix"))
        {
            if(GetPlayerVehicleSeat(i) == 0)
            {
                if(!GetPVarInt(i,"is_rp"))
                {
                    RepairVehicle(GetPlayerVehicleID(i));
                }  
            }
        }
    }
    return 1;
}

COMMAND:f(playerid)
{
    new currentveh,states;
	new Float:angle;
	states = GetPlayerState(playerid);
	if(states != PLAYER_STATE_DRIVER) return ServerTips(playerid,"您不是载具的司机,无法翻正");
	currentveh = GetPlayerVehicleID(playerid);
	GetVehicleZAngle(currentveh, angle);
	SetVehicleZAngle(currentveh, angle);
	ServerTips(playerid,"您的载具已翻正");
    SetVehicleVelocity(currentveh,0,0,0);
	return 1;
}

COMMAND:fix(playerid)
{
    if(GetPVarInt(playerid,"is_rp")) return RpTips(playerid,"RP模式禁用载具修复,请使用/repair对载具进行修复");
    if(!IsPlayerInAnyVehicle(playerid)) return ServerTips(playerid,"您不在任何载具上");
    if(GetPlayerVehicleSeat(playerid) != 0) return ServerTips(playerid,"您不是该载具的司机");
    RepairVehicle(GetPlayerVehicleID(playerid));
    ServerTips(playerid,"载具修复成功");
    return 1;
}

COMMAND:autofix(playerid)
{
    if(GetPVarInt(playerid,"is_autofix"))
    {
        DeletePVar(playerid,"is_autofix");
        ServerTips(playerid,"已关闭载具自动修复");
        return 1;
    }
    SetPVarInt(playerid,"is_autofix",!GetPVarInt(playerid,"is_autofix"));
    ServerTips(playerid,"已开启载具自动修复");
    return 1;
}

forward GetVehicleHost(vid);
public GetVehicleHost(vid)
{
    for(new i=0;i<MAX_PLAYERS;i++)
    {
        if(PlayerVehicle[i] == vid)
        {
            return i;
        }
    }
    return 1;
}

forward GetDriverID(vid);
public GetDriverID(vid)
{
    for(new i=0;i<MAX_PLAYERS;i++)
    {
        if(IsPlayerInAnyVehicle(i))
        {
            if(GetPlayerVehicleSeat(i) == 0)
            {
                if(GetPlayerVehicleID(i) == vid)
                {
                    return i;
                }
            }
        }
    }
    return 1;
}
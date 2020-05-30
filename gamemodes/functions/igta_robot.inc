forward SosReceive();
public SosReceive()
{
    new files[64];
    format(files,sizeof(files),"\\Robot\\SosReceive.txt");
    if(fexist(files))
    {
        new string[256];
        format(string,sizeof(string),"[T-Admin]线下管理员反馈信息:%s",dini_Get(files,"Content"));
        SendClientMessage(dini_Int(files,"PlayerID"),COLOR_RED,string);
        fremove(files);
    }
    return 1;
}
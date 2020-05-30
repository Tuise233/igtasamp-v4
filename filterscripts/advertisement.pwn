#include <a_samp>
#include <dini>

forward SendAdvertisement();
public SendAdvertisement()
{
    new files[64];
    format(files,sizeof(files),"\\Server\\AutoMessage.ini");
    //判断有几条广告
    new count = 0;
    for(new i=0;i<99;i++)
    {
        new num[64];
        format(num,sizeof(num),"Adver[%d]",i);
        if(dini_Isset(files,num))
        {
            count = count + 1;
        }
    }
    new rnum[64];
    format(rnum,sizeof(rnum),"Adver[%d]",random(count));
    new string[1024];
    format(string,sizeof(string),"[服务器广告]%s",dini_Get(files,rnum));
    SendClientMessageToAll(0xCA22DDC8,string);
    return 1;
}

public OnFilterScriptInit()
{
    SetTimer("SendAdvertisement",60000,1);
    return 1;
}
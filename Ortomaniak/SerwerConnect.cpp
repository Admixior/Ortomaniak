#include "SerwerConnect.h"

SerwerConnect::SerwerConnect(void):
connected(false),hash(0),thread(0),active_thread(false)
{}

int SerwerConnect::LoginComputer(string computer)
{
	StartThread();
	if(!active_thread) return -1;
	if(connected)
	{
		ShowErr(NET_ERROR_COMP_ALREADY_LOGIN,"SewerConnect::LoginComputer()");
		return 0;
	}
	comp_name=computer;
	char* com_name_wsk=new char[comp_name.size()+1];
	memcpy(com_name_wsk,comp_name.data(),comp_name.size());
	com_name_wsk[comp_name.size()]=NULL;
	while(pack_send)::Sleep(10);
	pack_send=CreatePacket(0,hash,NET_MSG_COMPUTER,NET_COMTYPE_REG,0,comp_name.size(),(BYTE*)com_name_wsk);
	return pack_send==0;
}

int SerwerConnect::StartThread()
{
	if(active_thread) return -1;
	thread=CreateThread(0,0,(LPTHREAD_START_ROUTINE)Sender,this,0,0);
	if(thread==NULL)ShowErr(NET_ERROR_CREATE_THREAD,"SerwerConnect::StartThread();");
	else active_thread=true;
	return thread==NULL;
}

int SerwerConnect::StopThread()
{
	if(!active_thread)return -1;
	active_thread=false;
	DWORD exit_code=STILL_ACTIVE;
	while(exit_code==STILL_ACTIVE)
	{
		GetExitCodeThread(thread,&exit_code);
		::Sleep(10);
	}
	return 0;
}

int SerwerConnect::Logout()
{
	if(!active_thread) return -1;
	while(pack_send)::Sleep(10);
	pack_send=CreatePacket(cID,hash,NET_MSG_COMPUTER,NET_COMTYPE_UNREG,0,0,0);
	return pack_send!=0;
}

int SerwerConnect::SetConnOption(__int32 _ip,__int16 _port)
{
	if(connected){ShowErr(NET_ERROR_WRITE_ADDRESS,"SerwerConnect::SetConnOption()");return -1;}
	addres.sin_family=AF_INET;
	addres.sin_port=htons(_port);
	addres.sin_addr.s_addr=_ip;
	memset(addres.sin_zero,0,8);
	return 0;
}

int SerwerConnect::Interpret()
{
	if(!pack_recv) return 0;
	NetPacket* pack=pack_recv;
	pack_recv=NULL;
	switch(pack->typ)
	{
	case NET_COMTYPE_REG:
		if(!pack->err)
		{
			if(!pack->size) ShowErr(NET_ERROR_NEED_MSG,"SerwerConnect::Interpret()");
			else if(pack->size<(sizeof(cID)+sizeof(hash)+2)) ShowErr(NET_ERROR_INCOMPLETE_PACK,"SerwerConnect::Interpret()");
			else
			{
				unsigned char* rac_msg=pack->msg;
				memcpy((char*)&cID,rac_msg,sizeof(cID));
				rac_msg+=sizeof(cID);
				memcpy((char*)&hash,rac_msg,sizeof(hash));
				rac_msg+=sizeof(hash);
				edition_text=(char*)rac_msg;
				connected=true;
			}
		}
		break;
	case NET_COMTYPE_UNREG:
		if(!pack->err)
		{
			if(pack->size)ShowErr(NET_WARNING_NOT_NEED_MSG,"SerwerConnect::Interpret()");
			connected=false;
			if(user)
			{
				delete user;
				user=0;
			}
			StopThread();
		}
		break;
	default:
		ShowErr(NET_ERROR_BAD_TYPE_MSG,"SerwerConnect::Interpret()");
	}
	if(pack->err)ShowErr(pack->err,"SER->CLI::SerwerConnect::Interpret()");
	int err=pack->err;
	delete pack;
	return err;
}


// By Peter Nwanosike


#include <winsock2.h> // htons() htonl() and other helper functions
#pragma comment (lib,"WS2_32.lib") 
#include <windows.h> 
#include <Iphlpapi.h>// Used to find information about the device such as default gateway and hardware addresses
#pragma comment (lib,"Iphlpapi.lib") 

typedef unsigned int  ip_addr;

IP_ADAPTER_INFO getAdapterInfoForAddress(ip_addr in)
{
	IP_ADAPTER_INFO *pAdapterInfo = 0, found;
	ULONG dataSize = 0;
	
	memset(&found, 0, sizeof(IP_ADAPTER_INFO));

	//call GetAdaptersInfo to fill required size;

	if (GetAdaptersInfo(pAdapterInfo, &dataSize) == ERROR_BUFFER_OVERFLOW) {
		//we have the real size now so make another request

		pAdapterInfo = new IP_ADAPTER_INFO[dataSize];
		memset(pAdapterInfo, 0, sizeof(IP_ADAPTER_INFO)*dataSize);
	
		GetAdaptersInfo(pAdapterInfo, &dataSize);
	}
	
	//still nothing happened.
	if (!pAdapterInfo) return found;
	
	for (IP_ADAPTER_INFO* Current = pAdapterInfo; Current != NULL; Current = Current->Next)
	{
		if (in == inet_addr(Current->IpAddressList.IpAddress.String)) {
			memcpy(&found, Current, sizeof(IP_ADAPTER_INFO));
		}
	}
	//we didn't find it so return
	return found;
}


int main()
{
  in_addr addr;
  // from the previous post I made in github.com you will find this information
	addr.s_addr = get_local_ip();

	IP_ADAPTER_INFO *adapter = getAdapterInfoForAddress(addr.s_addr);
	if (adapter) {
		printf("ADAPTER NAME: %s\n", adapter->AdapterName);
		printf("IP MASK: %s\n", adapter->IpAddressList.IpMask.String);
		printf("Gateway: %s\n", adapter->GatewayList.IpAddress.String);
	}
  
  return 0;
}

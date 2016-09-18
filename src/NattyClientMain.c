/*
 *  Author : WangBoJing , email : 1989wangbojing@gmail.com
 * 
 *  Copyright Statement:
 *  --------------------
 *  This software is protected by Copyright and the information contained
 *  herein is confidential. The software may not be copied and the information
 *  contained herein may not be used or disclosed except with the written
 *  permission of Author. (C) 2016
 * 
 *
 
****       *****
  ***        *
  ***        *                         *               *
  * **       *                         *               *
  * **       *                         *               *
  *  **      *                        **              **
  *  **      *                       ***             ***
  *   **     *       ******       ***********     ***********    *****    *****
  *   **     *     **     **          **              **           **      **
  *    **    *    **       **         **              **           **      *
  *    **    *    **       **         **              **            *      *
  *     **   *    **       **         **              **            **     *
  *     **   *            ***         **              **             *    *
  *      **  *       ***** **         **              **             **   *
  *      **  *     ***     **         **              **             **   *
  *       ** *    **       **         **              **              *  *
  *       ** *   **        **         **              **              ** *
  *        ***   **        **         **              **               * *
  *        ***   **        **         **     *        **     *         **
  *         **   **        **  *      **     *        **     *         **
  *         **    **     ****  *       **   *          **   *          *
*****        *     ******   ***         ****            ****           *
                                                                       *
                                                                      *
                                                                  *****
                                                                  ****


 *
 */

#include <string.h>
#include <stdio.h>

#include "NattyProtoClient.h"



void ntyUserRecvCb(DEVID devid, int len) {
	//int i = 0;
	U8 *buffer = ntyGetRecvBuffer();

	//for (i = 0;i < len;i ++) {
	ntydbg(" devid:%lld, len:%d, %s ", devid, len, buffer);
	//}
	ntydbg("\n");
}

void ntySendSuccess(int arg) {
	ntydbg("Success\n");
}

void ntySendFailed(int arg) {
	if (arg == STATUS_TIMEOUT)
		ntydbg("STATUS_TIMEOUT\n");
	else if (arg == STATUS_NOEXIST)
		ntydbg("STATUS_NOEXIST\n");
}

void ntyDisconnect(int arg) {
	ntydbg("ntyDisconnect\n");
}

void ntyReconnected(int arg) {
	ntydbg("ntyReconnected\n");
}

void ntyBindResult(int arg) {
	ntydbg(" ntyBindResult --> arg: %d", arg);
}

void ntyUnBindResult(int arg) {
	ntydbg(" ntyUnBindResult --> arg: %d", arg);
}

int main() {
	DEVID AppId = 10794;
	DEVID did = 0x352315052834187;
	
	int n = 0, length, i;
	int ch;
	U8 tempBuf[CLIENT_BUFFER_SIZE] = {0};
	const char *url =  "GET http://apilocate.amap.com/position?accesstype=1&imei=352315052834187&macs=30:FC:68:B9:E6:E6,-48,TP-LINK_E6E6_JUGUI|50:BD:5F:00:5F:62,-79,TP-LINK_5F62|28:10:7B:F6:E1:BA,-80,joshen?D|C0:61:18:90:1D:64,-86,ttt222????|D4:EE:07:3E:C8:8A,-87,HiWiFi_3EC88A|&output=xml&key=81040f256992a218a8a20ffb7f13ba9f HTTP/1.1";
	
	//ntydbg(" Press DevId <1 or 2>: ");   	
	//n = scanf("%lld", &AppId);
	
	ntySetProxyCallback(ntyUserRecvCb);
	ntySetSendFailedCallback(ntySendFailed);
	ntySetSendSuccessCallback(ntySendSuccess);
	ntySetProxyDisconnect(ntyDisconnect);
	ntySetProxyReconnect(ntyReconnected);
	ntySetBindResult(ntyBindResult);
	ntySetUnBindResult(ntyUnBindResult);
	ntySetDevId(did);
	
	ntyStartupClient();
	sleep(5);

	//ntyUnBindClient(0xEDFF12342345613);

	int count = 0;
	DEVID *list = ntyGetFriendsList(&count);
	for (i = 0;i < count;i ++) {
		ntydbg(" %d --> %lld\n", i+1, *(list+i));
	}
	ntyReleaseFriendsList(&list);

	//while(1);
#if 1
	while(1) {
		ntydbg("Proxy Please send msg:");
		char *ptr = fgets(tempBuf, CLIENT_BUFFER_SIZE, stdin);

		if (ntyGetNetworkStatus() == -1) {
			ntydbg("Startup Client\n");
			ntyStartupClient();
			continue;
		}
		if (tempBuf[0] == 'c') {
			ntydbg("Shutdown Client\n");
			ntyShutdownClient();
			continue;
		}
		//memset(tempBuf, 0, RECV_BUFFER_SIZE);
		//memcpy(tempBuf, url, strlen(url));
		int len = strlen(tempBuf);
		ntydbg(" tempBuf:%s, len:%d\n", tempBuf, len);
		ntySendMassDataPacket(tempBuf, len-1);
	}
#endif
}



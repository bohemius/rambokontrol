/*
 * rpc_functions.cpp
 *
 *  Created on: Apr 18, 2017
 *      Author: bohemius
 */

#include "rpc_functions.h"
#include "mbed_rpc.h"
#include "EthernetInterface.h"
#include "MbedJSONValue/MbedJSONValue.h"
#include "hl_debug.h"

InterfaceManager* rpcIfMgr;

vector<string> getDirListing(char *dir) {
	vector<string> result;
	DIR* dp;
	struct dirent *dirp;
	dp = opendir(dir);
	if (dp != NULL) {
		//read all directory and file names in current directory into filename vector
		while ((dirp = readdir(dp)) != NULL) {
			result.push_back(dirp->d_name);
		}
	} else {
		ERR("Error opening directory %s", dir);
	}
	closedir(dp);
	return result;
}

void fSdRoot(Arguments* in, Reply* out) {
	vector<string> dirContent = getDirListing("/sd");

	INFO("Creating JSON response.");
	MbedJSONValue sdContent;
	std::string s;

	int i = 0;
	for (vector<string>::iterator dirIter = dirContent.begin();
			dirIter != dirContent.end(); ++dirIter) {
		sdContent["Files"][i++] = *dirIter;
	}
	s = sdContent.serialize();
	INFO("sdRoot RPC JSON response: %s\r\n", s.c_str());
	out->putData(s.c_str());
}

void fHello(Arguments* in, Reply* out) {
	//todo
	const char *greeting = in->getArg<const char*>();
	const char *name = in->getArg<const char*>();

	char buffer[200];
	sprintf(buffer, "%s there %s!", greeting, name);
	out->putData(buffer);
}

void fEthInfo(Arguments* in, Reply* out) {
	EthernetInterface eth = rpcIfMgr->getEthIf();

	INFO("Creating JSON response.");
	MbedJSONValue ethInfo;
	std::string s;

	ethInfo["IP Address"] = eth.getIPAddress();
	ethInfo["Gateway"] = eth.getGateway();
	ethInfo["Network Mask"] = eth.getNetworkMask();
	ethInfo["MAC Address"] = eth.getMACAddress();
	s = ethInfo.serialize();
	INFO("EthInfo RPC JSON response: %s\r\n", s.c_str());
	out->putData(s.c_str());
}

void setRpcInterfaceMgr(InterfaceManager* ifMgr) {
	rpcIfMgr = ifMgr;
}


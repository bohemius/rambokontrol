#include "mbed.h"
#include "HTTPServer.h"
#include "FsHandler.h"
#include "RpcHandler.h"
#include "LocalFileSystem.h"
#include "EthernetInterface.h"
#include "SDFileSystem.h"
#include "mbed_rpc.h"
#define DEBUG
#include "hl_debug.h"
#include "rpc_functions.h"
#include <vector>

//  Use LED1 to indicate that the main loop is still executing
DigitalOut loopLed(LED1);
DigitalOut remoteLed(LED3);
//  Use the serial connection 'pc' to dump debug information
HTTPServer svr;
//  Instantiate a local file system handler named 'local' which will be used later to access files on the mbed.
LocalFileSystem local("local");
//  Instantiate a sd file system handler named 'sd' which will be used later to access files on the SD card.
SDFileSystem sd(p11, p12, p13, p21, "sd", p15, SDFileSystem::SWITCH_NEG_NO, 80000000);
//  Create the EthernetInterface. This is optional, please see the documentation of HTTP Server's start method.

//EthernetInterface eth;

//Make these variables accessible over RPC by attaching them to an RPCVariable
int remoteLedRequest = 0;
RPCVariable<int> RPCRquest(&remoteLedRequest, "Request");

RPCFunction rpcHello(&fHello, "Hello");
RPCFunction rpcEth(&fEthInfo, "ethInfo");
RPCFunction rpcSdRoot(&fSdRoot, "sdRoot");

vector<string> read_file_names(char *dir) {
	vector<string> result;
	DirHandle *dirHandle = sd.opendir(dir);;
	struct dirent *dirEntry;
	if (dirHandle != NULL) {
		//read all directory and file names in current directory into filename vector
		while ((dirEntry = dirHandle->readdir()) != NULL) {
			result.push_back(string(dirEntry->d_name));
		}
	} else {
		ERR("Error opening directory %s", dir);
	}
	dirHandle->closedir();
	return result;
}

void dump_file_list(vector<string> fileList) {
	for (vector<string>::iterator it = fileList.begin(); it < fileList.end();
			it++) {
		INFO("%s", (*it).c_str());
	}
}

void try_index_file() {
	DIR *fp = opendir(".");
	if (fp == NULL) {
		ERR("Error trying index file.");
	} else {
		INFO("index.html file OK");
		closedir(fp);
	}
}

int main() {
	//indicate boot sequence
	//remoteLed=1;
	//wait(5);
	//remoteLed=0;
	INFO("Creating ifMgr");
	InterfaceManager ifMgr;
	INFO("Registering interface manager with RPC functions");
	setRpcInterfaceMgr(&ifMgr);

	INFO("Server starting.");
	//INFO("Mounting embedded flash as /local/");
	//vector<string> localFilesList = read_file_names("/local");

	INFO("Mounting SD Card as /sd/");
	//Try to mount the SD card
	if (sd.mount() != 0) {
	    ERR("failed!");
	    exit(1);
	}
	INFO("success!");

	//Display the card type
	INFO("\tCard type: ");
	SDFileSystem::CardType cardType = sd.card_type();
	if (cardType == SDFileSystem::CARD_NONE) {
		INFO("None");
	} else if (cardType == SDFileSystem::CARD_MMC) {
	    INFO("MMC");
	} else if (cardType == SDFileSystem::CARD_SD) {
	    INFO("SD");
	} else if (cardType == SDFileSystem::CARD_SDHC) {
	    INFO("SDHC");
	} else {
	   INFO("Unknown");
	}

	//Display the card capacity
	INFO("\tSectors: %u", sd.disk_sectors());
	INFO("\tCapacity: %.1fMB", sd.disk_sectors() / 2048.0);
	vector<string> sdFilesList = read_file_names("/");

	dump_file_list(sdFilesList);
	try_index_file();

	HTTPFsRequestHandler::mount("webfs/", "/");
	svr.addHandler<HTTPFsRequestHandler>("/");
	svr.addHandler<HTTPRpcRequestHandler>("/rpc");

	EthernetInterface eth = ifMgr.getEthIf();
	//eth.init();
	//eth.connect();

	// Now start the server on port 80.
	if (!svr.start(80, &eth)) {
		ERR("Server not starting !");
		exit(0);
	}

	// The whole server activity is performed in this Loop. You can also put it into a separate RTOS Task and let it run there all the time.
	while (1) {
		remoteLed = remoteLedRequest;
		svr.poll();
		loopLed = 1;
		wait(0.5);
		loopLed = 0;
		wait(0.5);
	}
}

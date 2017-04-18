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
Serial pc(USBTX, USBRX);
//  Instantiate a HTTPServer to handle incoming requests
HTTPServer  svr;
//  Instantiate a local file system handler named 'local' which will be used later to access files on the mbed.
LocalFileSystem local("local");
//  Instantiate a sd file system handler named 'sd' which will be used later to access files on the SD card.
SDFileSystem sd(p11, p12, p13, p21, "sd");
//  Create the EthernetInterface. This is optional, please see the documentation of HTTP Server's start method.
EthernetInterface eth;
//Make these variables accessible over RPC by attaching them to an RPCVariable
int remoteLedRequest = 0;
RPCVariable<int> RPCRquest(&remoteLedRequest, "Request");

RPCFunction rpcHello(&fHello, "Hello");

vector<string> read_file_names(char *dir)
{
    vector<string> result;
    DIR* dp;
    struct dirent *dirp;
    dp = opendir(dir);
    if (dp != NULL) {
        //read all directory and file names in current directory into filename vector
        while((dirp = readdir(dp)) != NULL) {
            result.push_back(string(dirp->d_name));
        }
    } else {
        ERR("Error opening directory %s",dir);
    }
    closedir(dp);
    return result;
}

void dump_file_list(vector<string> fileList)
{
    for(vector<string>::iterator it=fileList.begin(); it < fileList.end(); it++) {
        INFO("%s",(*it).c_str());
    }
}

int main()
{
    //indicate boot sequence
    remoteLed=1;
    wait(5);
    remoteLed=0;
    
    INFO("Server starting.");
    INFO("Mounting embedded flash as /local/");
    vector<string> localFilesList = read_file_names("/local");

    INFO("Mounting SD Card as /sd/");    
    vector<string> sdFilesList = read_file_names("/sd");
    
    dump_file_list(sdFilesList);
    
    HTTPFsRequestHandler::mount("/sd/webfs/", "/");
    svr.addHandler<HTTPFsRequestHandler>("/");
    svr.addHandler<HTTPRpcRequestHandler>("/rpc");

    //  Initialize the EthernetInterface and initiate a connection using DHCP.
    eth.init();
    eth.connect();

    // ***
    // TODO: Perform other Tasks using the ethernet connection here.
    // ****

    // Now start the server on port 80.
    if (!svr.start(80, &eth)) {
        ERR("Server not starting !");
        exit(0);
    }

    // The whole server activity is performed in this Loop. You can also put it into a separate RTOS Task and let it run there all the time.
    while(1) {
        remoteLed=remoteLedRequest;
        svr.poll();
        loopLed = 1;
        wait(0.5);
        loopLed = 0;
        wait(0.5);
    }
}

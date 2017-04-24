/*
 * rpc_functions.h
 *
 *  Created on: Apr 18, 2017
 *      Author: bohemius
 */
#ifndef RPC_FUNCTIONS_H
#define RPC_FUNCTIONS_H

#include "mbed_rpc.h"
#include "EthernetInterface.h"
#include "if_manager.h"
#include <vector>

vector<string> getDirListing(char *dir);
void fSdRoot(Arguments* in,Reply* out);
void fEthInfo(Arguments* in,Reply* out);
void fHello(Arguments* in,Reply* out);
void setRpcInterfaceMgr(InterfaceManager* ifMgr);

#endif

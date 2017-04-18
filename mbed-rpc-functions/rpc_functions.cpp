/*
 * rpc_functions.cpp
 *
 *  Created on: Apr 18, 2017
 *      Author: bohemius
 */

#include "rpc_functions.h"
#include "mbed_rpc.h"

void fSdRoot(Arguments* in,Reply* out) {
	//todo
}

void fHello(Arguments* in,Reply* out) {
	//todo
	const char *greeting = in->getArg<const char*>();
	const char *name = in->getArg<const char*>();

	char buffer[200];
	sprintf(buffer, "%s there %s!",greeting,name);
	out->putData(buffer);
}



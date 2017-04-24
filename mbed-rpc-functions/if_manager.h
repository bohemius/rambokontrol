#ifndef MBED_RPC_FUNCTIONS_IF_MANAGER_H_
#define MBED_RPC_FUNCTIONS_IF_MANAGER_H_

#include "mbed.h"
#include "EthernetInterface.h"
#include <map>

class InterfaceManager {
public:
	InterfaceManager();
	~InterfaceManager();

	EthernetInterface getEthIf();
	Serial getPrinterIf();
	void addDigitalOut(const PinName pin);

private:
	EthernetInterface _eth;
	Serial _printer; // tx, rx serial interface for printer
	map<PinName,DigitalOut> _digitalOutputs;

};

#endif /* MBED_RPC_FUNCTIONS_IF_MANAGER_H_ */

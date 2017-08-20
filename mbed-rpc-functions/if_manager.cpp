#include "if_manager.h"
#include "hl_debug.h"

InterfaceManager::InterfaceManager() {
	INFO("Initializing Ethernet adapter");
	_eth.init();
	_eth.connect();

	DigitalOut bootLed(LED4);
	addDigitalOut(LED4);

	bootLed=1;
	wait(5);
	bootLed=0;
}

InterfaceManager::~InterfaceManager() {
}

EthernetInterface InterfaceManager::getEthIf() {
	return _eth;
}

void InterfaceManager::addDigitalOut(const PinName pin) {
	_digitalOutputs.emplace(pin,DigitalOut(pin));
}

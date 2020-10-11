//
// Created by Rogiel Sulzbach on 9/15/17.
//

#include "Machine.hpp"

namespace System::Machine {

	void Machine::wait(unsigned int cycles) {
		for(int i = 0; i<cycles/2; i++) asm volatile("nop");
	}

	Device::Console* Machine::getStandardConsole() {
		return nullptr;
	}

}
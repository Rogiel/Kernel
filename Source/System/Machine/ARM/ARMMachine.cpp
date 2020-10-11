//
// Created by Rogiel Sulzbach on 9/24/17.
//

#include "ARMMachine.hpp"

#include <bitset>
#include <System/Memory/PhysicalMemoryPage.hpp>

namespace System::Machine::ARM {

	void ARMMachine::initialize() {

	}

	void ARMMachine::halt() {

	}

	Bus::Bus* ARMMachine::getRootBus() {
		return nullptr;
	}

	Device::Console* ARMMachine::getStandardConsole() {
		return nullptr;
	}

	Memory::VirtualMemoryMapper& ARMMachine::getVirtualMemoryMapper() {
	}

	Interrupt::InterruptController& ARMMachine::getInterruptController() {
	}

	Memory::MemoryLayout ARMMachine::getMemoryLayout() {
	}

	Memory::PhysicalMemoryPageSize ARMMachine::getPhysicalMemoryPageSize() {
		return 0;
	}

	const char* ARMMachine::getName() const {
		return "ARM";
	}

	void ARMMachine::disableInterrupt() {

	}

	void ARMMachine::enableInterrupt() {

	}

}
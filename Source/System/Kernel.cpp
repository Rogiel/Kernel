//
// Created by Rogiel Sulzbach on 9/15/17.
//

#include "Kernel.hpp"

#include <string>
#include <experimental/coroutine>

namespace System {

	Kernel* kernel;

	Kernel::Kernel(Memory::MemoryController* memoryController) :
			memoryController(memoryController) {
		kernel = this;
		driverManager = new Driver::DriverManager();
		deviceManager = new Device::DeviceManager(*driverManager);
	}

	Kernel::Kernel(Kernel&& other) noexcept = default;

	Kernel& Kernel::operator=(Kernel&& other) noexcept = default;

	// -----------------------------------------------------------------------------------------------------------------

	static const char* COPYRIGHT_NOTICE = R"(
 /----------------------------------------------------------------------------\
 | Rogiel Kernel  --  Version 0.1                                             |
 | Copyright (c) 2017 Rogiel Sulzbach. All rights reserved.                   |
 | -------------------------------------------------------------------------- |
 | This is a learning operating system written by me.                         |
 \----------------------------------------------------------------------------/
)";

	void Kernel::initialize() {
		machine->initialize();

		auto console = machine->getStandardConsole();

		console->clear();
		console->writeln(COPYRIGHT_NOTICE);

		console->write("Machine initialized: ");
		console->writeln(machine->getName());

		kernel->writeln("Found ", Driver::count, " dynamically registered drivers.");

		auto rootBus = machine->getRootBus();
		if(rootBus == nullptr) {
			console->writeln("The machine root bus is nullptr. Not probing.");
		} else {
			console->writeln("Probing devices on root bus...");
			deviceManager->probe(*rootBus);
			console->writeln("Devices probed.");
		}
	}

	void Kernel::print(const std::string& str) {
		if(machine == nullptr) {
			return;
		}

		auto console = machine->getStandardConsole();
		if(console == nullptr) {
			return;
		}

		console->write(str);
	}

//	void Kernel::writeln() {
//		if(machine == nullptr) {
//			return;
//		}
//
//		auto console = machine->getStandardConsole();
//		if(console == nullptr) {
//			return;
//		}
//
//		console->write("\n");
//	}

	// -----------------------------------------------------------------------------------------------------------------

	void Kernel::setMachine(Machine::Machine* machine) {
		Kernel::machine = machine;
	}

	Machine::Machine* Kernel::getMachine() {
		return Kernel::machine;
	}

	void Kernel::setMemoryController(Memory::MemoryController* memoryController) {
		Kernel::memoryController = memoryController;
	}

	Memory::MemoryController* Kernel::getMemoryController() {
		return Kernel::memoryController;
	}

	void Kernel::setDriverManager(Driver::DriverManager* driverManager) {
		Kernel::driverManager = driverManager;
	}

	Driver::DriverManager* Kernel::getDriverManager() {
		return Kernel::driverManager;
	}

	void Kernel::setDeviceManager(Device::DeviceManager* deviceManager) {
		Kernel::deviceManager = deviceManager;
	}

	Device::DeviceManager* Kernel::getDeviceManager() {
		return Kernel::deviceManager;
	}

}
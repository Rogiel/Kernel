//
// Created by Rogiel Sulzbach on 9/15/17.
//

#include "X86Machine.hpp"

#include <System/Kernel.hpp>
#include <System/Driver/Bus/PCIBus.hpp>

#include <System/Memory/PhysicalMemoryController.hpp>
#include <System/Memory/MemoryLayout.hpp>

namespace System::Machine::X86 {

	void X86Machine::initialize() {
		/*
		 * Add some default drivers...
		 */
		kernel->getDriverManager()->addDriver(new Driver::Console::VGAConsoleDriver());
		kernel->getDriverManager()->addDriver(new Driver::Bus::PCI::PCIBusDriver());

		/*
		 * And attach a fake console device
		 */
		Driver::Bus::FakeBus::MMIOFakeBusDevice fakeBusDevice((void*) 0xB8000, 4000);
		consoleDevice = dynamic_cast<Device::Console*>(kernel->getDeviceManager()->attach(fakeBusDevice));

		if(consoleDevice == nullptr) {
			halt();
		}
		consoleDevice->write("Console device loaded.\n");

		fakeBus.addDevice(new Driver::Bus::FakeBus::MMIOFakeBusDevice((void*) 0x01, 4000));
		fakeBus.addDevice(new Driver::Bus::FakeBus::MMIOFakeBusDevice((void*) 0x8000000, 4000));
	}

	void X86Machine::halt() {
		asm("hlt");
	}

	// -----------------------------------------------------------------------------------------------------------------

	Bus::Bus* X86Machine::getRootBus() {
		return &fakeBus;
	}

	Device::Console* X86Machine::getStandardConsole() {
		return consoleDevice;
	}

	// -----------------------------------------------------------------------------------------------------------------

	Memory::VirtualMemoryMapper& X86Machine::getVirtualMemoryMapper() {
//		return <#initializer#>;
	}

	Interrupt::InterruptController& X86Machine::getInterruptController() {
//		return <#initializer#>;
	}

	Memory::MemoryLayout X86Machine::getMemoryLayout() {
		return Memory::MemoryLayout {
				{Memory::MemoryRegion(0x01000000, 0x01000000 + 512 * 1024 * 1024)}
		};
	}

	Memory::PhysicalMemoryPageSize X86Machine::getPhysicalMemoryPageSize() {
		return 4 * 1024;
	}

	// -----------------------------------------------------------------------------------------------------------------

	const char* X86Machine::getName() const {
		return "Intel x86 Platform";
	}

	// -----------------------------------------------------------------------------------------------------------------

	void X86Machine::disableInterrupt() {
		asm("cli");
	}

	void X86Machine::enableInterrupt() {
		asm("cli");
	}

}
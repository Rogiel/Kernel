//
// Created by Rogiel Sulzbach on 9/15/17.
//

#include <System/Kernel.hpp>

#include <System/Driver/DriverManager.hpp>

#include "PCIBus.hpp"
#include "FakeBus.hpp"

namespace System::Driver::Bus::PCI {

	PCIBus::PCIBus() = default;
	PCIBus::~PCIBus() = default;

	// -----------------------------------------------------------------------------------------------------------------

	void PCIBus::enumerateChildren(Bus::Bus::BusEnumerator enumerator) {
		for(auto& busDevice : devices) {
			enumerator(*busDevice);
		}
	}

	std::string PCIBus::getName() const {
		return "PCI";
	}

	// -----------------------------------------------------------------------------------------------------------------

	PCIBusDevice::PCIBusDevice(PCIBus* parentBus, uint32_t vendorID, uint32_t deviceID) :
			parentBus(parentBus),
			vendorID(vendorID),
			deviceID(deviceID) {}

	// -----------------------------------------------------------------------------------------------------------------

	uint32_t PCIBusDevice::getVendorID() const {
		return vendorID;
	}

	uint32_t PCIBusDevice::getDeviceID() const {
		return deviceID;
	}

	// -----------------------------------------------------------------------------------------------------------------

	System::Bus::Bus& PCIBusDevice::getParentBus() {
		return *parentBus;
	}

	System::Bus::MemoryResource<uint8_t> PCIBusDevice::allocateMemoryResource() {
		return System::Bus::MemoryResource<uint8_t>();
	}

	std::string PCIBusDevice::getDeviceName() {
		return "PCI(" + std::to_string(deviceID) + "," + std::to_string(vendorID) + ")";
	}

	// -----------------------------------------------------------------------------------------------------------------

	Driver::ProbeStatus PCIBusDriver::probe(System::Bus::BusDevice& busDevice) {
		if(auto fakeBusDevice = dynamic_cast<FakeBus::MMIOFakeBusDevice*>(&busDevice)) {
			if(fakeBusDevice->getPhysicalAddress() == (void*) 0x01) {
				return ProbeStatus::SUPPORTED;
			}
		} else {
			kernel->writeln("Not a MMIOFakeBusDevice");
		}
		return ProbeStatus::NOT_SUPPORTED;
	}

	Device::Device* PCIBusDriver::attach(System::Bus::BusDevice& busDevice) {
		return new PCIBus();
	}

	const char* PCIBusDriver::getName() {
		return "PCIBus";
	}

	KERNEL_REGISTER_DRIVER(PCIBus, PCIBusDriver);

}
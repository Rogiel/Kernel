//
// Created by Rogiel Sulzbach on 9/15/17.
//

#include "FakeBus.hpp"
#include <System/Driver/DriverManager.hpp>

namespace System::Driver::Bus::FakeBus {

	FakeBus::FakeBus() {

	}

	FakeBus::~FakeBus() {

	}

	// -----------------------------------------------------------------------------------------------------------------

	void FakeBus::enumerateChildren(BusEnumerator enumerator) {
		for(auto& busDevice : devices) {
			enumerator(*busDevice);
		}
	}

	// -----------------------------------------------------------------------------------------------------------------

	void FakeBus::addDevice(FakeBusDevice* busDevice) {
		devices.push_back(std::unique_ptr<FakeBusDevice>(busDevice));
	}

	// -----------------------------------------------------------------------------------------------------------------

	std::string FakeBus::getName() const {
		return "FakeBus";
	}

	// -----------------------------------------------------------------------------------------------------------------

	std::string FakeBusDevice::getDeviceName() {
		return "FakeBus(0,0)";
	}

	// -----------------------------------------------------------------------------------------------------------------

	MMIOFakeBusDevice::MMIOFakeBusDevice(void* addr, unsigned int size) :
			addr(addr),
			size(size) {}

	System::Bus::MemoryResource<uint8_t> MMIOFakeBusDevice::allocateMemoryResource() {
		return System::Bus::MemoryResource(
				addr, size
		);
	}

	bool MMIOFakeBusDevice::isMMIODevice() const {
		return true;
	}

	bool MMIOFakeBusDevice::isPMIODevice() const {
		return false;
	}

	void* MMIOFakeBusDevice::getPhysicalAddress() const {
		return addr;
	}

	unsigned int MMIOFakeBusDevice::getSize() const {
		return size;
	}

	// -----------------------------------------------------------------------------------------------------------------

	Driver::ProbeStatus FakeBusDriver::probe(System::Bus::BusDevice& busDevice) {
		return ProbeStatus::SUPPORTED;
	}

	Device::Device* FakeBusDriver::attach(System::Bus::BusDevice& busDevice) {
		return new FakeBus();
	}

	const char* FakeBusDriver::getName() {
		return "FakeBus";
	}

	KERNEL_REGISTER_DRIVER(FakeBus, FakeBusDriver);

}

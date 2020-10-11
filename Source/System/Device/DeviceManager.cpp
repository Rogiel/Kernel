//
// Created by Rogiel Sulzbach on 9/15/17.
//

#include "DeviceManager.hpp"

#include <System/Kernel.hpp>

namespace System::Device {

	DeviceManager::DeviceManager(Driver::DriverManager& driverManager) :
			driverManager(driverManager) {}

	// -----------------------------------------------------------------------------------------------------------------

	void DeviceManager::probe(Bus::Bus& bus) {
		bus.enumerateChildren([this, &bus](Bus::BusDevice& busDevice) {
			kernel->writeln("Found device ", busDevice.getDeviceName(), " on bus ", bus.getName());
			attach(busDevice);
		});
	}

	Device* DeviceManager::attach(Bus::BusDevice& busDevice) {
		kernel->writeln("Attaching bus device ", busDevice.getDeviceName());

		auto driver = driverManager.findDriver(busDevice);
		if(driver == nullptr) {
			kernel->writeln("No driver found for ", busDevice.getDeviceName());
			return nullptr;
		}

		kernel->writeln("Found ", driver->getName(), " driver for device ", busDevice.getDeviceName());

		auto device = driver->attach(busDevice);
		busDevice.setAttachedDevice(device);
		device->onAttach();

		devices.push_back(std::unique_ptr<Device>(device));

		return device;
	}

}

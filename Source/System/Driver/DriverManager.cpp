//
// Created by Rogiel Sulzbach on 9/15/17.
//

#include "DriverManager.hpp"

#include <System/Kernel.hpp>

namespace System::Driver {

	void DriverManager::addDriver(Driver* driver) {
		auto found = std::find_if(drivers.begin(), drivers.end(),
								  [driver](const auto& entry) { return entry.get() == driver; });
		if(found != drivers.end()) {
			kernel->debug("Adding a new driver (", driver->getName(), "), but the driver was already registered.");
			return;
		}
		kernel->debug("Adding driver ", driver->getName());
		drivers.push_back(std::unique_ptr<Driver>(driver));
	}

	void DriverManager::removeDriver(Driver* driver) {
		auto found = std::find_if(drivers.begin(), drivers.end(),
								  [driver](const auto& entry) { return entry.get() == driver; });
		if(found == drivers.end()) {
			kernel->debug("Removing a driver (", driver->getName(), "), but the driver was not registered.");
			return;
		}

		kernel->debug("Removing driver ", driver->getName());
		drivers.erase(found);
	}

	// -----------------------------------------------------------------------------------------------------------------

	Driver* DriverManager::findDriver(System::Bus::BusDevice& busDevice) {
		for(std::unique_ptr<Driver>& driver : drivers) {
			auto status = driver->probe(busDevice);
			if(status == Driver::ProbeStatus::SUPPORTED) {
				return driver.get();
			}
		}
		return nullptr;
	}

	Driver* DriverManager::findDriverByName(const std::string& name) {
		auto found = std::find_if(drivers.begin(), drivers.end(), [&name](const auto& driver) {
			return driver->getName() == name;
		});
		if(found == drivers.end()) {
			return nullptr;
		}
		return found->get();
	}

	Driver* DriverManager::findDriverByType(const std::type_info& typeInfo) {
		auto found = std::find_if(drivers.begin(), drivers.end(), [&typeInfo](const auto& driver) {
			return typeid(*driver) == typeInfo;
		});
		if(found == drivers.end()) {
			return nullptr;
		}
		return found->get();
	}


	size_t count = 0;
	void dynamicDriverRegistering(Driver* driver) {
		count++;
		kernel->getDriverManager()->addDriver(driver);
	}

}

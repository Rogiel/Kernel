//
// Created by Rogiel Sulzbach on 9/15/17.
//

#pragma once

#include <System/Driver/DriverManager.hpp>
#include <vector>
#include <system_error>

namespace System::Device {

	class DeviceManager {
	private:
		Driver::DriverManager& driverManager;

		std::vector<std::unique_ptr<Device>> devices;

	public:
		explicit DeviceManager(Driver::DriverManager& driverManager);

		DeviceManager(const DeviceManager& other) = delete;
		DeviceManager& operator=(const DeviceManager& other) = delete;

		DeviceManager(DeviceManager&& other) = delete;
		DeviceManager& operator=(DeviceManager&& other) = delete;

	public:
		void probe(Bus::Bus& bus);
		Device* attach(Bus::BusDevice& busDevice);



	};

}



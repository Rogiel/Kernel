//
// Created by Rogiel Sulzbach on 9/15/17.
//

#pragma once

#include <System/Device/Device.hpp>
#include <System/Bus/BusDevice.hpp>

#include <memory>

namespace System::Driver {

	class Driver {
	public:
		enum class ProbeStatus {
			SUPPORTED,
			NOT_SUPPORTED
		};

		virtual ProbeStatus probe(Bus::BusDevice& busDevice) = 0;

		/**
		 * Attaches the device.
		 *
		 * @param busDevice 	the bus device to attach
		 *
		 * @return a newly created device from the driver
		 */
		virtual Device::Device* attach(Bus::BusDevice& busDevice) = 0;

		/**
		 * @return a constant string with the driver name
		 */
		virtual const char* getName() = 0;

	};

	using DriverPtr = std::unique_ptr<Driver>;

}



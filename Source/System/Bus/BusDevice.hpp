//
// Created by Rogiel Sulzbach on 9/15/17.
//

#pragma once

#include <System/Bus/Bus.hpp>
#include <System/Bus/MemoryResource.hpp>

#include <string>

namespace System::Bus {

	class BusDevice {
	private:
		/**
		 * The attached device
		 */
		Device::Device* attachedDevice;

	public:
		/**
		 * @return the parent bus
		 */
		virtual Bus& getParentBus() = 0;

		/**
		 * @return the attached device
		 */
		virtual Device::Device* getAttachedDevice();

		/**
		 * @param device the attached device
		 */
		virtual void setAttachedDevice(Device::Device* device);

	public:
		/**
		 * @return a allocated memory resource
		 */
		virtual MemoryResource<uint8_t> allocateMemoryResource() = 0;

	public:
		/**
		 * @return a string representing the device name
		 */
		virtual std::string getDeviceName() = 0;


	};

}


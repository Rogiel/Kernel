//
// Created by Rogiel Sulzbach on 9/16/17.
//

#include "BusDevice.hpp"

namespace System::Bus {

	Device::Device* BusDevice::getAttachedDevice() {
		return attachedDevice;
	}

	void BusDevice::setAttachedDevice(Device::Device* attachedDevice) {
		BusDevice::attachedDevice = attachedDevice;
	}

}
//
// Created by Rogiel Sulzbach on 9/15/17.
//

#include "Device.hpp"

#include <System/Kernel.hpp>

namespace System::Device {

	void Device::onAttach() {
		kernel->debug("Default Device ", __func__, " method called.");
	}

	void Device::onDetach() {
		kernel->debug("Default Device ", __func__, " method called.");
	}

	void Device::onInterrupt() {
		kernel->debug("Default Device ", __func__, " method called.");
	}

	// -----------------------------------------------------------------------------------------------------------------

	System::Device::Device* System::Device::Device::getParent() {
		return nullptr;
	}

}
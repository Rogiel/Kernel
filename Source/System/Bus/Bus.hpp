//
// Created by Rogiel Sulzbach on 9/15/17.
//

#pragma once

#include <System/Device/Device.hpp>
#include <functional>

namespace System::Bus {

	class BusDevice;

	class Bus : public Device::Device {

	public:
		/**
		 * A callback that gets called for every device on the bus.
		 *
		 * The callback function should not take ownership of the
		 * bus device, but it is safe to keep a pointer to it for
		 * as long as the device's {#onDetach} method is not called.
		 *
		 * @param busDevice	the device found on the bus
		 */
		using BusEnumerator = std::function<void(BusDevice& busDevice)>;

		/**
		 * Enumerates all children on the bus.
		 *
		 * @param enumerator a callback that will be called for each device
		 * found on the bus.
		 */
		virtual void enumerateChildren(BusEnumerator enumerator) = 0;

	};

}

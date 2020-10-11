//
// Created by Rogiel Sulzbach on 9/15/17.
//

#pragma once

#include <System/Device/Device.hpp>

namespace System::Device {

	/**
	 * A device that represents a AHCI bus
	 */
	class AHCI : public Device,
				 public Bus::Bus {
	public:


	};

	class AHCIDevice : public Bus::BusDevice {
	protected:
		AHCI& controller;

	public:
		explicit AHCIDevice(AHCI& parentBus) :
				controller(controller) {}

	public:
		AHCI& getParentBus() final { return controller; }

	public:
		virtual void write() = 0;

		virtual void read() = 0;
	};

}
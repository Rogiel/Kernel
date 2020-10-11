//
// Created by Rogiel Sulzbach on 9/15/17.
//

#pragma once

#include <System/Bus/Bus.hpp>
#include <System/Bus/BusDevice.hpp>
#include <System/Driver/Driver.hpp>

#include <vector>
#include <memory>
#include <stdint.h>

namespace System::Driver::Bus::FakeBus {

	class FakeBusDevice;

	class FakeBus : public System::Bus::Bus {
	private:
		std::vector<std::unique_ptr<FakeBusDevice>> devices;

	public:
		FakeBus();
		~FakeBus();

	public:
		void enumerateChildren(BusEnumerator enumerate) override;

	public:
		void addDevice(FakeBusDevice* busDevice);

	public:
		/**
		 * @return a string representing the device name
		 */
		std::string getName() const final;

	};

	class FakeBusDevice : public System::Bus::BusDevice {
	public:
		FakeBus& getParentBus() override { return *(FakeBus*)(0x00); }

		/**
		 * @return a string representing the device name
		 */
		std::string getDeviceName() final;

	public:
		/**
		 * @return true if the device is a MMIO device
		 */
		virtual bool isMMIODevice() const = 0;

		/**
		 * @return true if the device is a MMIO device
		 */
		virtual bool isPMIODevice() const = 0;

	};

	class MMIOFakeBusDevice : public FakeBusDevice {
	private:
		void* addr;
		unsigned int size;

	public:
		MMIOFakeBusDevice(void* addr, unsigned int size);

	public:
		virtual System::Bus::MemoryResource<uint8_t> allocateMemoryResource();

	public:
		/**
		 * @return true if the device is a MMIO device
		 */
		bool isMMIODevice() const;

		/**
		 * @return true if the device is a MMIO device
		 */
		bool isPMIODevice() const;

	public:
		void* getPhysicalAddress() const;
		unsigned int getSize() const;

	};

	class FakeBusDriver : public Driver {
	public:
		ProbeStatus probe(System::Bus::BusDevice& busDevice) final;

		/**
		 * Attaches the device.
		 *
		 * @param busDevice 	the bus device to attach
		 *
		 * @return a newly created device from the driver
		 */
		Device::Device* attach(System::Bus::BusDevice& busDevice) final;

		/**
		 * @return a constant string with the driver name
		 */
		const char* getName() final;

	};

}

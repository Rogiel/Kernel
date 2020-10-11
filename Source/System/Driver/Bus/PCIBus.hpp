//
// Created by Rogiel Sulzbach on 9/15/17.
//

#pragma once

#include <System/Bus/Bus.hpp>
#include <System/Bus/BusDevice.hpp>
#include <System/Driver/Driver.hpp>

#include <vector>
#include <memory>

namespace System::Driver::Bus::PCI {

	class PCIBusDevice;

	class PCIBus : public System::Bus::Bus {
	private:
		/**
		 * A list of currently active PCI bus devices
		 */
		std::vector<std::unique_ptr<PCIBusDevice>> devices;

	public:
		/**
		 * Creates a new PCI bus
		 */
		PCIBus();

		/**
		 * Destroys the PCI bus device
		 */
		~PCIBus();

	public:
		/**
		 * Enumerates all children on the bus.
		 *
		 * @param enumerator a callback that will be called for each device
		 * found on the bus.
		 */
		void enumerateChildren(BusEnumerator enumerator) final;

	public:
		/**
		 * @return a string representing the device name
		 */
		std::string getName() const final;
	};

	class PCIBusDevice : public System::Bus::BusDevice {
	private:
		/**
		 * The parent PCI bus
		 */
		PCIBus* parentBus;

	private:
		/**
		 * The PCI vendor ID
		 */
		uint32_t vendorID;

		/**
		 * The PCI device ID
		 */
		uint32_t deviceID;

	public:
		/**
		 * Creates a new PCIBusDevice
		 *
		 * @param parentBus 	the parent PCI bus
		 * @param vendorID 		the PCI vendor ID
		 * @param deviceID 		the PCI device ID
		 */
		PCIBusDevice(PCIBus* parentBus, uint32_t vendorID, uint32_t deviceID);

	public:
		/**
		 * @return the PCI vendor ID
		 */
		uint32_t getVendorID() const;

		/**
		 * @return the PCI device ID
		 */
		uint32_t getDeviceID() const;

	public:
		/**
		 * @return the parent bus
		 */
		System::Bus::Bus& getParentBus() final;

	public:
		/**
		 * @return a allocated memory resource
		 */
		System::Bus::MemoryResource<uint8_t> allocateMemoryResource() final;

	public:
		/**
		 * @return a string representing the device name
		 */
		std::string getDeviceName() final;

	};

	class PCIBusDriver : public Driver {
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
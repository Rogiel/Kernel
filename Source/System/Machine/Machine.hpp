//
// Created by Rogiel Sulzbach on 9/15/17.
//

#pragma once

#include <System/Bus/Bus.hpp>
#include <System/Device/Device.hpp>
#include <System/Device/Console.hpp>
#include <System/Memory/VirtualMemoryMapper.hpp>
#include <System/Interrupt/InterruptController.hpp>
#include <System/Memory/MemoryLayout.hpp>

namespace System::Machine {

	class Machine {
	public:
		/**
		 * Initializes the machine.
		 */
		virtual void initialize() = 0;
		virtual void halt() = 0;

		virtual void wait(unsigned int cycles);

	public:
		virtual Bus::Bus* getRootBus() = 0;
		virtual Device::Console* getStandardConsole();

	public:
		/**
		 * @return the machine virtual memory mapper
		 */
		virtual Memory::VirtualMemoryMapper& getVirtualMemoryMapper() = 0;

		/**
		 * @return the machine interrupt controller
		 */
		virtual Interrupt::InterruptController& getInterruptController() = 0;

	public:
		/**
		 * @return the physical memory layout
		 */
		virtual Memory::MemoryLayout getMemoryLayout() = 0;

		/**
		 * @return the physical memory page size
		 */
		virtual Memory::PhysicalMemoryPageSize getPhysicalMemoryPageSize() = 0;

	public:
		/**
		 * @return a string representing the machine name
		 */
		virtual const char* getName() const = 0;

	};

}


//
// Created by Rogiel Sulzbach on 9/24/17.
//

#pragma once

#include <System/Machine/Machine.hpp>

namespace System::Machine::ARM {
	class ARMMachine : public Machine {
	public:
		/**
		 * Initializes the machine.
		 */
		void initialize() override;

		void halt() override;

	public:
		Bus::Bus* getRootBus() override;
		Device::Console* getStandardConsole() override;

	public:
		/**
		 * @return the machine virtual memory mapper
		 */
		Memory::VirtualMemoryMapper& getVirtualMemoryMapper() override;

		/**
		 * @return the machine interrupt controller
		 */
		Interrupt::InterruptController& getInterruptController() override;

	public:
		/**
		 * @return the physical memory layout
		 */
		Memory::MemoryLayout getMemoryLayout() override;

		/**
		 * @return the physical memory page size
		 */
		Memory::PhysicalMemoryPageSize getPhysicalMemoryPageSize() override;

	public:
		/**
		 * @return a string representing the machine name
		 */
		const char* getName() const override;

	public:
		void disableInterrupt();
		void enableInterrupt();

	};
}

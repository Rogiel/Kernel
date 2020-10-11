//
// Created by Rogiel Sulzbach on 9/18/17.
//

#pragma once

#include <System/Memory/VirtualMemoryMapper.hpp>

namespace System::Machine::X86::Memory {

	class X86VirtualMemoryMapper : public System::Memory::VirtualMemoryMapper {
	public:
		/**
		 * Creates a new virtual memory page in the platform
		 *
		 * @param page 	the memory page being created
		 */
		void createPage(System::Memory::VirtualMemoryPage& page) override;

		/**
		 * Updates the platform virtual memory mapping
		 *
		 * @param page 	the page to be updated
		 */
		void updatePage(System::Memory::VirtualMemoryPage& page) override;

		/**
		 * Destroys virtual memory page in the platform
		 *
		 * @param page 	the memory page being destroyed
		 */
		void destroyPage(System::Memory::VirtualMemoryPage& page) override;
	};


}


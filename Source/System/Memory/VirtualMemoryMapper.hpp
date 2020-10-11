//
// Created by Rogiel Sulzbach on 9/16/17.
//

#pragma once

#include <System/Memory/VirtualMemoryPage.hpp>

namespace System::Memory {

	/**
	 * A platform-specific virtual memory mapper.
	 *
	 * Implementation of this class are responsible for updating
	 * the processor memory map to match the in-kernel representation
	 * of the virtual memory.
	 */
	class VirtualMemoryMapper {
	public:
		/**
		 * Creates a new virtual memory page in the platform
		 *
		 * @param page 	the memory page being created
		 */
		virtual void createPage(VirtualMemoryPage& page) = 0;

		/**
		 * Updates the platform virtual memory mapping
		 *
		 * @param page 	the page to be updated
		 */
		virtual void updatePage(VirtualMemoryPage& page) = 0;

		/**
		 * Destroys virtual memory page in the platform
		 *
		 * @param page 	the memory page being destroyed
		 */
		virtual void destroyPage(VirtualMemoryPage& page) = 0;

	};

}


//
// Created by Rogiel Sulzbach on 9/16/17.
//

#pragma once

#include <System/Memory/VirtualMemorySpace.hpp>
#include <list>

namespace System::Memory {

	class VirtualMemoryController {
	private:
		/**
		 * A list of currently active memory spaces
		 */
		std::list<VirtualMemorySpaceWeakPtr> memorySpaces;

	public:
		/**
		 * Creates a new virtual memory controller
		 */
		VirtualMemoryController();

		/**
		 * Destroys the virtual memory controller
		 */
		~VirtualMemoryController();

	public:
		/**
		 * Creates a new virtual memory space. This memory space can be
		 * used by both the kernel and user-space applications.
		 *
		 * The memory space will be automatically reclaimed once there are
		 * no more active references to the space. All physical pages
		 * allocated by the virtual memory space will be released and can
		 * be reutilized by other processes.
		 *
		 * @return a newly created virtual memory space
		 */
		VirtualMemorySpacePtr createVirtualMemorySpace(size_t startingSize);

	private:
		/**
		 * Disposes a virtual memory space
		 *
		 * @param virtualMemorySpace 	the virtual memory space being disposed
		 */
		void disposeVirtualMemorySpace(VirtualMemorySpace* virtualMemorySpace);

	};

}

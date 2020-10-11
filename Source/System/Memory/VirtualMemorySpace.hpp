//
// Created by Rogiel Sulzbach on 9/16/17.
//

#pragma once

#include <System/Memory/VirtualMemoryPage.hpp>

#include <memory>
#include <list>
#include <vector>

namespace System::Memory {

	class VirtualMemorySpace {
	private:
		/**
		 * All pages of virtual memory that belong to this virtual
		 * memory space
		 */
		std::list<VirtualMemoryPagePtr> pages;

	public:
		/**
		 * Creates a new empty virtual memory space
		 */
		VirtualMemorySpace();

		/**
		 * Destroys the virtual memory space
		 */
		~VirtualMemorySpace();

	public:
		/**
		 * Checks if the memory space contains the address given by
		 * <tt>address</tt>.
		 *
		 * @param address 	the address to check for bounds
		 *
		 * @return true if the virtual memory space contains the given
		 * address
		 */
		bool contains(size_t address);

		/**
		 * Gets the virtual memory page to which <tt>address</tt>
		 * belongs to. If <tt>address</tt> is not contained in the
		 * virtual space, a <tt>nullptr</tt> is returned.
		 *
		 * @param address	the address to fetch the page for
		 *
		 * @return the virtual memory page in which <tt>address</tt>
		 * can be accessed or <tt>nullptr</tt> if the address is
		 * out of bounds.
		 */
		VirtualMemoryPagePtr getPage(size_t address);

		/**
		 * Gets a list of virtual memory pages in which the memory
		 * region starting from <tt>address</tt> with <tt>length</tt>
		 * bytes in length can be accessed.
		 *
		 * @param address	the address to fetch the page for
		 * @param length	the length of the memory region to
		 * 					fetch pages for
		 *
		 * @return a vector of memory pages in which the given memory
		 * region can be accessed. A empty vector is returned if the
		 * region is out of bounds.
		 */
		std::vector<VirtualMemoryPagePtr> getPage(size_t address, size_t length);

	public:
		/**
		 * @param address	the address to check if it is readable
		 *
		 * @return true if the memory address is readable
		 */
		bool canRead(size_t address);

		/**
		 * @param address	the address to check if it is writeable
		 *
		 * @return true if the memory address is writeable
		 */
		bool canWrite(size_t address);

		/**
		 * @param address	the address to check if it is executable
		 *
		 * @return true if the memory address is executable
		 */
		bool canExecute(size_t address);

	};

	using VirtualMemorySpacePtr = std::shared_ptr<VirtualMemorySpace>;
	using VirtualMemorySpaceWeakPtr = std::weak_ptr<VirtualMemorySpace>;

}

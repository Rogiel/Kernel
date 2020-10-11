//
// Created by Rogiel Sulzbach on 9/16/17.
//

#pragma once

#include <System/Memory/PhysicalMemoryPage.hpp>
#include <System/Memory/MemoryLayout.hpp>

#include <list>

namespace System::Memory {

	class PhysicalMemoryController {
	private:
		/**
		 * A list of all used memory pages
		 */
		std::list<PhysicalMemoryPagePtr> usedPages;

		/**
		 * A list of all free memory pages
		 */
		std::list<PhysicalMemoryPagePtr> freePages;

	public:
		/**
		 * Create a new physical memory controller from the given
		 * memory layout.
		 *
		 * @param memoryLayout 	the memory layout to create a new
		 * physical memory controller from
		 */
		explicit PhysicalMemoryController(const MemoryLayout& memoryLayout);

		/**
		 * Destroys the physical memory controller
		 */
		~PhysicalMemoryController();

	public:
		/**
		 * Takes a free memory page and returns it. The page
		 * will be automatically flaged as used until released
		 * by a call to {#releasePage}.
		 *
		 * @return a allocated memory page or <tt>nullptr</tt>
		 * if there is no more memory available.
		 */
		PhysicalMemoryPagePtr allocatePage();

		/**
		 * Releases a physical memory page.
		 *
		 * @param page 	the physical memory page to be released.
		 */
		void releasePage(const PhysicalMemoryPagePtr& page);

	public:
		/**
		 * Find a memory page by its physical memory address
		 *
		 * @param address 	the physical memory page address
		 *
		 * @return the physical memory page or <tt>nullptr</tt>
		 * if the memory does not point to a valid memory address
		 */
		PhysicalMemoryPagePtr getPage(size_t address);

	public:
		/**
		 * @return the number of free pages
		 */
		size_t getFreePageCount() const;

		/**
		 * @return the number of used pages
		 */
		size_t getUsedPageCount() const;

	};

}

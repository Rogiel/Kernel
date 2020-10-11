//
// Created by Rogiel Sulzbach on 9/16/17.
//

#pragma once

#include <memory>

namespace System::Memory {

	using PhysicalMemoryPageSize = size_t;

	class PhysicalMemoryPage {
	private:
		/**
		 * The memory page starting address
		 */
		size_t address;

		/**
		 * The memory page size
		 */
		size_t size;

	public:
		/**
		 * Creates a new physical memory page
		 *
		 * @param address 	the memory page starting address
		 * @param size 		the memory page size
		 */
		PhysicalMemoryPage(size_t address, size_t size);

	public:
		/**
		 * @return the memory page starting address
		 */
		size_t getAddress() const;

		/**
		 * @return the memory page size
		 */
		size_t getSize() const;

	};

	using PhysicalMemoryPagePtr = PhysicalMemoryPage*;

}

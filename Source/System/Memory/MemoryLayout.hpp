//
// Created by Rogiel Sulzbach on 9/16/17.
//

#pragma once

#include <vector>

namespace System::Memory {

	class MemoryRegion {
	private:
		/**
		 * The memory region starting address
		 */
		size_t start;

		/**
		 * The memory region ending address
		 */
		size_t end;

	public:
		/**
		 * Creates a new memory region from starting and ending addresses
		 *
		 * @param start	the memory region starting address
		 * @param end	the memory region ending address
		 */
		MemoryRegion(size_t start, size_t end);

	public:
		/**
		 * Checks if the region contains the given memory address
		 *
		 * @param address 	the address to check for
		 *
		 * @return true if the region contains the address given by
		 * <tt>address</tt>.
		 */
		bool contains(size_t address) const;

	public:
		/**
		 * @return the memory region starting address
		 */
		size_t getStart() const;

		/**
		 * @return the memory region ending address
		 */
		size_t getEnd() const;

	};

	class MemoryLayout {
	private:
		/**
		 * A vector containing all known memory regions
		 */
		std::vector<MemoryRegion> regions;

	public:
		/**
		 * Creates a new memory layout description with the given
		 * <tt>regions</tt>
		 *
		 * @param regions 	the regions to create a new memory layout
		 * from
		 */
		explicit MemoryLayout(std::vector<MemoryRegion> regions);

	public:
		/**
		 * Simplifies the memory layout description.
		 *
		 * A simplification will collapse are adjacent regions into
		 * a single large region.
		 */
		void simplify();

		/**
		 * Checks if any of the regions in the memory contains the
		 * given memory address
		 *
		 * @param address 	the address to check for
		 *
		 * @return true if any region contains the address given by
		 * <tt>address</tt>.
		 */
		bool contains(size_t address) const;

	public:
		/**
		 * @return a vector containing all known memory regions
		 */
		const std::vector<MemoryRegion>& getRegions() const;


	};

}


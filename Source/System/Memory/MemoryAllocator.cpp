//
// Created by Rogiel Sulzbach on 9/17/17.
//

#include "MemoryAllocator.hpp"

namespace System::Memory {

	MemoryAllocator::MemoryAllocator(PhysicalMemoryController& physicalMemoryController) :
			physicalMemoryController(&physicalMemoryController) {

	}

	MemoryAllocator::MemoryAllocator(MemoryAllocator&& other) noexcept = default;
	MemoryAllocator& MemoryAllocator::operator=(MemoryAllocator&& other) noexcept = default;

	// -----------------------------------------------------------------------------------------------------------------

	struct AllocationHeader {
		size_t length;
	} __attribute__((packed));

	// -----------------------------------------------------------------------------------------------------------------

	void* MemoryAllocator::allocate(uint64_t size) {


		return nullptr;
	}

	void MemoryAllocator::deallocate(void* ptr) {

	}

}

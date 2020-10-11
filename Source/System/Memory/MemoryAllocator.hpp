//
// Created by Rogiel Sulzbach on 9/17/17.
//

#pragma once

#include <System/Memory/PhysicalMemoryController.hpp>

#include <list>

namespace System::Memory {

	class MemoryAllocator {
	private:
		PhysicalMemoryController* physicalMemoryController;
		std::list<PhysicalMemoryPage> pages;

	public:
		MemoryAllocator(PhysicalMemoryController& physicalMemoryController);

		MemoryAllocator(const MemoryAllocator& other) = delete;
		MemoryAllocator& operator=(const MemoryAllocator& other) = delete;

		MemoryAllocator(MemoryAllocator&& other) noexcept;
		MemoryAllocator& operator=(MemoryAllocator&& other) noexcept;

	public:
		void* allocate(uint64_t size);
		void deallocate(void* ptr);
	};

}

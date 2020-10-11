//
// Created by Rogiel Sulzbach on 9/15/17.
//

#include "MemoryController.hpp"

#include "System/Kernel.hpp"
#include <string.h>

namespace System::Memory {

	MemoryController::MemoryController(void* starting, uint64_t length) :
			starting((uint8_t*) starting),
			size(length) {
		current = (uint8_t*) starting;
		memset(starting, 0, 512);
	}

	MemoryController::MemoryController(MemoryController&& other) noexcept = default;
	MemoryController& MemoryController::operator=(MemoryController&& other) noexcept = default;

	// -----------------------------------------------------------------------------------------------------------------

	void* MemoryController::alloc(uint64_t size) {
		if((current - starting) + size + sizeof(uint64_t) < size) {
			return nullptr;
		}

		auto ptr = current;
		auto length = (uint64_t*) ptr;
		*length = size;

		current += size + sizeof(uint64_t);
		return ptr + sizeof(uint64_t);
	}

	void MemoryController::dealloc(void* ptr) {
		uint64_t* length = (uint64_t*) ptr - sizeof(uint64_t);
	}

}

void* malloc(size_t size) {
	return System::kernel->getMemoryController()->alloc(size);
}

void free(void* ptr) {
	System::kernel->getMemoryController()->dealloc(ptr);
}

void* realloc(void* ptr, size_t size) {
	return System::kernel->getMemoryController()->alloc(size);
}

void* operator new(size_t size) {
	return System::kernel->getMemoryController()->alloc(size);
}

void* operator new[](size_t size) {
	return System::kernel->getMemoryController()->alloc(size);
}

void operator delete(void* ptr) {
	return System::kernel->getMemoryController()->dealloc(ptr);
}

void operator delete[](void* ptr) {
	return System::kernel->getMemoryController()->dealloc(ptr);
}

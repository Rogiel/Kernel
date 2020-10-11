//
// Created by Rogiel Sulzbach on 9/15/17.
//

#pragma once

#include <stdint.h>

namespace System::Memory {

	class MemoryController {
	private:
		uint8_t* starting;
		uint8_t* current;
		uint64_t size;

	public:
		MemoryController(void* starting, uint64_t length);

		MemoryController(const MemoryController& other) = delete;
		MemoryController& operator=(const MemoryController& other) = delete;

		MemoryController(MemoryController&& other) noexcept;
		MemoryController& operator=(MemoryController&& other) noexcept;
		
	public:
		void* alloc(uint64_t size);
		void dealloc(void* ptr);

	};

}


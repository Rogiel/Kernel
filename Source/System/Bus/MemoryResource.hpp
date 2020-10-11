//
// Created by Rogiel Sulzbach on 9/15/17.
//

#pragma once

#include <cstdint>

namespace System::Bus {

	template<typename T = uint8_t>
	class MemoryResource {
	private:
		volatile T* addr = nullptr;
		unsigned int size = 0;

	public:
		MemoryResource() = default;

		MemoryResource(volatile void* addr, unsigned int size);

		template<typename TT>
		MemoryResource(MemoryResource<TT> other);

	public:
		volatile void* address() const { return addr; }
		unsigned int length() const { return size; }

	public:
		template<typename V>
		volatile V* view() {
			return reinterpret_cast<volatile V*>(addr);
		}

		explicit operator bool() const {
			return addr != nullptr;
		}

		volatile T& operator[](unsigned int offset) {
			return *reinterpret_cast<volatile T*>(addr + offset);
		}

		volatile const T& operator[](unsigned int offset) const {
			return *reinterpret_cast<volatile const T*>(addr + offset);
		}
	};

	template<typename T>
	MemoryResource<T>::MemoryResource(volatile void* addr, unsigned int size) :
			addr(reinterpret_cast<volatile T*>(addr)),
			size(size) {}

	template<typename T>
	template<typename TT>
	MemoryResource<T>::MemoryResource(MemoryResource<TT> other):
			MemoryResource(other.address(),
						   other.length() * sizeof(T) / sizeof(TT)) {}

}




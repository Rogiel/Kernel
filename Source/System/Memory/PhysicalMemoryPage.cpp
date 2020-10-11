//
// Created by Rogiel Sulzbach on 9/16/17.
//

#include "PhysicalMemoryPage.hpp"

namespace System::Memory {

	PhysicalMemoryPage::PhysicalMemoryPage(size_t address, size_t size) :
			address(address),
			size(size) {

	}

	// -----------------------------------------------------------------------------------------------------------------

	size_t PhysicalMemoryPage::getAddress() const {
		return address;
	}

	size_t PhysicalMemoryPage::getSize() const {
		return size;
	}

}
//
// Created by Rogiel Sulzbach on 9/16/17.
//

#include "VirtualMemorySpace.hpp"

namespace System::Memory {

	VirtualMemorySpace::VirtualMemorySpace() {

	}

	VirtualMemorySpace::~VirtualMemorySpace() {

	}

	// -----------------------------------------------------------------------------------------------------------------

	bool VirtualMemorySpace::contains(size_t address) {
		return false;
	}

	VirtualMemoryPagePtr VirtualMemorySpace::getPage(size_t address) {
		return System::Memory::VirtualMemoryPagePtr();
	}

	std::vector<VirtualMemoryPagePtr> VirtualMemorySpace::getPage(size_t address, size_t length) {
		return std::vector<VirtualMemoryPagePtr>();
	}

	// -----------------------------------------------------------------------------------------------------------------

	bool VirtualMemorySpace::canRead(size_t address) {
		auto page = getPage(address);
		if(page == nullptr) {
			return false;
		}
		return page->canRead();
	}

	bool VirtualMemorySpace::canWrite(size_t address) {
		auto page = getPage(address);
		if(page == nullptr) {
			return false;
		}
		return page->canWrite();
	}

	bool VirtualMemorySpace::canExecute(size_t address) {
		auto page = getPage(address);
		if(page == nullptr) {
			return false;
		}
		return page->canExecute();
	}


}

//
// Created by Rogiel Sulzbach on 9/18/17.
//

#include "X86VirtualMemoryMapper.hpp"

namespace System::Machine::X86::Memory {

	struct PlatformVirtualPage {
		uint8_t id;
	};

	void X86VirtualMemoryMapper::createPage(System::Memory::VirtualMemoryPage& page) {
		page.setPlatformData(new PlatformVirtualPage());
	}

	void X86VirtualMemoryMapper::updatePage(System::Memory::VirtualMemoryPage& page) {
		auto platformPage = reinterpret_cast<PlatformVirtualPage*>(page.getPlatformData());
		platformPage->id = 100;
	}

	void X86VirtualMemoryMapper::destroyPage(System::Memory::VirtualMemoryPage& page) {
		delete reinterpret_cast<PlatformVirtualPage*>(page.getPlatformData());
	}

}

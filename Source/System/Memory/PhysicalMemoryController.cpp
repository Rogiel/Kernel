//
// Created by Rogiel Sulzbach on 9/16/17.
//

#include <System/Kernel.hpp>
#include "PhysicalMemoryController.hpp"

namespace System::Memory {

	PhysicalMemoryController::PhysicalMemoryController(const MemoryLayout& memoryLayout) {
		// TODO externalize the page size...
		for(auto& region : memoryLayout.getRegions()) {
			size_t starting = region.getStart();
			size_t length = 4 * 1024;

			while(region.contains(starting) && region.contains(starting + length)) {
				freePages.push_back(new PhysicalMemoryPage(
						starting, length
				));
				starting += length;
			}
		}
	}

	PhysicalMemoryController::~PhysicalMemoryController() {

	}

	// -----------------------------------------------------------------------------------------------------------------

	PhysicalMemoryPagePtr PhysicalMemoryController::allocatePage() {
		if(freePages.empty()) {
			return nullptr;
		}

		auto page = freePages.front();
		freePages.pop_front();
		usedPages.push_back(page);

		return page;
	}

	void PhysicalMemoryController::releasePage(const PhysicalMemoryPagePtr& page) {
		auto found = std::find(usedPages.begin(), usedPages.end(), page);
		if(found == usedPages.end()) {
			/*
			 * The page is already free? weird... I think we should panic here
			 */
			return;
		}

		usedPages.erase(found);
		freePages.push_back(page);
	}

	// -----------------------------------------------------------------------------------------------------------------

	PhysicalMemoryPagePtr PhysicalMemoryController::getPage(size_t address) {
		return System::Memory::PhysicalMemoryPagePtr();
	}

	// -----------------------------------------------------------------------------------------------------------------

	size_t PhysicalMemoryController::getFreePageCount() const {
		return freePages.size();
	}

	size_t PhysicalMemoryController::getUsedPageCount() const {
		return usedPages.size();
	}

}

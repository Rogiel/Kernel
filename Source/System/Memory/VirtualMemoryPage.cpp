//
// Created by Rogiel Sulzbach on 9/16/17.
//

#include "VirtualMemoryPage.hpp"

namespace System::Memory {

	VirtualMemoryPage::VirtualMemoryPage() {

	}

	VirtualMemoryPage::~VirtualMemoryPage() {

	}

	// -----------------------------------------------------------------------------------------------------------------

	VirtualMemoryPagePermission VirtualMemoryPage::getPermissions() const {
		return permissions;
	}

	bool VirtualMemoryPage::canRead() {
		return permissions & VirtualMemoryPagePermission::READ;
	}

	bool VirtualMemoryPage::canWrite() {
		return permissions & VirtualMemoryPagePermission::WRITE;
	}

	bool VirtualMemoryPage::canExecute() {
		return permissions & VirtualMemoryPagePermission::EXECUTE;
	}

	const PhysicalMemoryPagePtr& VirtualMemoryPage::getPhysicalMemoryPage() const {
		return physicalMemoryPage;
	}

	void VirtualMemoryPage::setPhysicalMemoryPage(const PhysicalMemoryPagePtr& physicalMemoryPage) {
		if(VirtualMemoryPage::physicalMemoryPage != nullptr) {
//			VirtualMemoryPage::physicalMemoryPage->removeVirtualPage(this);
		}
		VirtualMemoryPage::physicalMemoryPage = physicalMemoryPage;
		if(VirtualMemoryPage::physicalMemoryPage != nullptr) {
//			VirtualMemoryPage::physicalMemoryPage->addVirtualPage(this);
		}
	}

	VirtualMemoryPagePlatformData VirtualMemoryPage::getPlatformData() const {
		return platformData;
	}

	void VirtualMemoryPage::setPlatformData(VirtualMemoryPagePlatformData platformData) {
		VirtualMemoryPage::platformData = platformData;
	}

}

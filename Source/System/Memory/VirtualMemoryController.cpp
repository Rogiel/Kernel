//
// Created by Rogiel Sulzbach on 9/16/17.
//

#include "VirtualMemoryController.hpp"

#include <functional>

namespace System::Memory {

	VirtualMemoryController::VirtualMemoryController() {

	}

	VirtualMemoryController::~VirtualMemoryController() {

	}

	// -----------------------------------------------------------------------------------------------------------------

	VirtualMemorySpacePtr VirtualMemoryController::createVirtualMemorySpace(size_t startingSize) {
		return std::shared_ptr<VirtualMemorySpace>(
				new VirtualMemorySpace(),
				std::bind(&VirtualMemoryController::disposeVirtualMemorySpace,
						  this, std::placeholders::_1)
		);
	}

	void VirtualMemoryController::disposeVirtualMemorySpace(VirtualMemorySpace* virtualMemorySpace) {
		// we are now disposing the memory space
		delete virtualMemorySpace;
	}

	// -----------------------------------------------------------------------------------------------------------------



}


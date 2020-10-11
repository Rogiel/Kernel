//
// Created by Rogiel Sulzbach on 9/17/17.
//

#include "ProcessController.hpp"

namespace System::Process {

	ProcessController::ProcessController() = default;
	ProcessController::~ProcessController() = default;

	// -----------------------------------------------------------------------------------------------------------------

	ProcessPtr ProcessController::createProcess() {
		return nullptr;
	}

	void ProcessController::destroyProcess(ProcessPtr process) {

	}

	// -----------------------------------------------------------------------------------------------------------------

	const std::list<Process>& ProcessController::getProcesses() const {
		return processes;
	}

}

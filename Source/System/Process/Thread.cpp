//
// Created by Rogiel Sulzbach on 9/17/17.
//

#include "Thread.hpp"

namespace System::Process {

	Thread::Thread(Process& process) :
			process(&process) {}

	Thread::~Thread() = default;

	// -----------------------------------------------------------------------------------------------------------------

	Process* Thread::getProcess() const {
		return process;
	}

	ThreadState Thread::getState() const {
		return state;
	}
}

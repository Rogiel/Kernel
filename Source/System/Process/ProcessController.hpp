//
// Created by Rogiel Sulzbach on 9/17/17.
//

#pragma once

#include <System/Process/Process.hpp>

#include <list>

namespace System::Process {

	class ProcessController {
	private:
		/**
		 * A list of all processes currently being managed by the system
		 */
		std::list<Process> processes;

	public:
		/**
		 * Creates a new process controller
		 */
		ProcessController();

		/**
		 * Destroys the process controller
		 */
		~ProcessController();

	public:
		/**
		 * Creates a new process
		 *
		 * @return a pointer to the newly created process
		 */
		ProcessPtr createProcess();

		/**
		 * Destroys a process
		 *
		 * @param process 	the process to be destroyed
		 */
		void destroyProcess(ProcessPtr process);

	public:
		/**
		 * @return a list of all processes currently being managed by
		 * the system
		 */
		const std::list<Process>& getProcesses() const;


	};

}

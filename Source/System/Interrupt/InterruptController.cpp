//
// Created by Rogiel Sulzbach on 9/17/17.
//

#include "InterruptController.hpp"

namespace System::Interrupt {

	InterruptController::InterruptController() = default;
	InterruptController::~InterruptController() = default;

	// -----------------------------------------------------------------------------------------------------------------

	InterruptDescriptorPtr InterruptController::getInterrupt(IRQ irq) {
		auto found = interrupts.find(irq);
		if(found == interrupts.end()) {
			return nullptr;
		}

		return found->second.get();
	}

	void InterruptController::registerInterrupt(InterruptDescriptorPtr interruptDescriptor) {
		auto found = interrupts.find(interruptDescriptor->getIRQ());
		if(found != interrupts.end()) {
			return;
		}

		interrupts.emplace(interruptDescriptor->getIRQ(), std::unique_ptr<InterruptDescriptor>(interruptDescriptor));
	}

}

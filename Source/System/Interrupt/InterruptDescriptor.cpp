//
// Created by Rogiel Sulzbach on 9/17/17.
//

#include "InterruptDescriptor.hpp"

namespace System::Interrupt {

	InterruptDescriptor::InterruptDescriptor(IRQ irq) :
			irq(irq) {};

	InterruptDescriptor::~InterruptDescriptor() = default;

	// -----------------------------------------------------------------------------------------------------------------

	IRQ InterruptDescriptor::getIRQ() const {
		return irq;
	}

	const InterruptHandler& InterruptDescriptor::getHandler() const {
		return handler;
	}

	void InterruptDescriptor::setHandler(const InterruptHandler& handler) {
		InterruptDescriptor::handler = handler;
	}

}

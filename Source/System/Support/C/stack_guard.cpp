//
// Created by Rogiel Sulzbach on 9/16/17.
//

#include <System/Kernel.hpp>

#if UINT32_MAX == UINTPTR_MAX
#define STACK_CHK_GUARD 0xe2dee396
#else
#define STACK_CHK_GUARD 0x595e9fbd94fda766
#endif

extern "C" {

__attribute__((unused))
uintptr_t __stack_chk_guard = STACK_CHK_GUARD;

__attribute__((noreturn))
__attribute__((noinline))
void __stack_chk_fail(void) {
	System::kernel->writeln("Stack corrupted!!!");
	System::kernel->getMachine()->halt();
}

}
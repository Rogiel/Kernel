/* The bootloader will look at this image and start execution at the symbol
   designated as the entry point. */
ENTRY(KernelMain)
/* OUTPUT_FORMAT(binary) */

PAGE_SIZE = 4K;

/* Tell where the various sections of the object files will be put in the final
   kernel image. */
SECTIONS {
	/* Begin putting sections at 1 MiB, a conventional place for kernels to be
	   loaded at by the bootloader. */
	. = 1M;

	/* First put the multiboot header, as it is required to be put very early
	   early in the image or the bootloader won't recognize the file format.
	   Next we'll put the .text section. */
	/*.multiboot (.) : ALIGN(4) {
	    *(.multiboot)
	}*/

	.init (.) : ALIGN(4K) {
        *(.init)
	}

	.text (.) : ALIGN(4K) {
	    KEEP (*(SORT_NONE(.text)))
	}

	/* Read-only data. */
	.rodata  : ALIGN(4K) {
		*(.rodata)
	}

	/* Read-write data (initialized) */
	.data  :  {
		*(.data)
	}

	/* Read-write data (uninitialized) and stack */
	.bss  : ALIGN(4K) {
		*(COMMON)
		*(.bss)
		*(.bootstrap_stack)
	}

/*
    .debug_info     . : { *(.debug_info)     }
    .debug_abbrev   . : { *(.debug_abbrev)   }
    .debug_aranges  . : { *(.debug_aranges)  }
    .debug_line     . : { *(.debug_line)     }
    .debug_str      . : { *(.debug_str)      }
    .debug_ranges   . : { *(.debug_ranges)   }
    .debug_macinfo  . : { *(.debug_macinfo)  }
    .debug_pubnames . : { *(.debug_pubnames) }
    .debug_pubtypes . : { *(.debug_pubtypes) }
    .comment        . : { *(.comment)        }
    .debug_frame    . : { *(.debug_frame)    }

	.symtab     . : { *(.symtab)   }
	.shstrtab   . : { *(.shstrtab) }
	.strtab     . : { *(.strtab)   }
*/
	/* The compiler may produce other sections, by default it will put them in
	   a segment with the same name. Simply add stuff here as needed. */
}

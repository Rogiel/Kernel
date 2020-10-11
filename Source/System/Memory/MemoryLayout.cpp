//
// Created by Rogiel Sulzbach on 9/16/17.
//

#include "MemoryLayout.hpp"

namespace System::Memory {

	MemoryRegion::MemoryRegion(size_t start, size_t end) :
			start(start),
			end(end) {}
	// -----------------------------------------------------------------------------------------------------------------

	bool MemoryRegion::contains(size_t address) const {
		return (
				address >= start &&
				address <= end
		);
	}

	// -----------------------------------------------------------------------------------------------------------------

	size_t MemoryRegion::getStart() const {
		return start;
	}

	size_t MemoryRegion::getEnd() const {
		return end;
	}

	// -----------------------------------------------------------------------------------------------------------------
	// -----------------------------------------------------------------------------------------------------------------

	MemoryLayout::MemoryLayout(std::vector<MemoryRegion> regions_) :
			regions(std::move(regions_)) {
		// ensure the regions are sorted by starting addresses
		std::sort(regions.begin(), regions.end(), [](const MemoryRegion& region1, const MemoryRegion& region2) {
			return region1.getStart() < region2.getStart();
		});
	}

	// -----------------------------------------------------------------------------------------------------------------

	void MemoryLayout::simplify() {
		// TODO
	}

	bool MemoryLayout::contains(size_t address) const {
		for(auto& region : regions) {
			if(region.contains(address)) {
				return true;
			}
		}
		return false;
	}

	// -----------------------------------------------------------------------------------------------------------------

	const std::vector<MemoryRegion>& MemoryLayout::getRegions() const {
		return regions;
	}

}
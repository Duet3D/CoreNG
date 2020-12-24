/*
 * Cache.cpp
 *
 *  Created on: 22 Nov 2019
 *      Author: David
 */

#include "Cache.h"

#if SAM4E || SAME70 || SAME5x

#if SAME70
# include <core_cm7.h>

#define USE_MPU		1

extern uint32_t _nocache_ram_start;
extern uint32_t _nocache_ram_end;

# if USE_MPU
#  include <mpu_armv7.h>

// Macro ARM_MPU_RASR_EX is incorrectly defined in CMSIS 5.4.0, see https://github.com/ARM-software/CMSIS_5/releases. Redefine it here.

#  undef ARM_MPU_RASR_EX

/**
* MPU Region Attribute and Size Register Value
*
* \param DisableExec       Instruction access disable bit, 1= disable instruction fetches.
* \param AccessPermission  Data access permissions, allows you to configure read/write access for User and Privileged mode.
* \param AccessAttributes  Memory access attribution, see \ref ARM_MPU_ACCESS_.
* \param SubRegionDisable  Sub-region disable field.
* \param Size              Region size of the region to be configured, for example 4K, 8K.
*/
#  define ARM_MPU_RASR_EX(DisableExec, AccessPermission, AccessAttributes, SubRegionDisable, Size)    \
  ((((DisableExec)      << MPU_RASR_XN_Pos)   & MPU_RASR_XN_Msk)                                  | \
   (((AccessPermission) << MPU_RASR_AP_Pos)   & MPU_RASR_AP_Msk)                                  | \
   (((AccessAttributes) & (MPU_RASR_TEX_Msk | MPU_RASR_S_Msk | MPU_RASR_C_Msk | MPU_RASR_B_Msk))) | \
   (((SubRegionDisable) << MPU_RASR_SRD_Pos)  & MPU_RASR_SRD_Msk)                                 | \
   (((Size)             << MPU_RASR_SIZE_Pos) & MPU_RASR_SIZE_Msk)                                | \
   (((MPU_RASR_ENABLE_Msk))))

# endif

#else

// SAM4E and SAME5x use fairly similar cache controllers

inline bool is_cache_enabled() noexcept
{
#if SAME5x
	return CMCC->SR.bit.CSTS;
#elif SAM4E
	return (CMCC->CMCC_SR & CMCC_SR_CSTS) != 0;
#endif
}

inline void cache_invalidate_all() noexcept
{
#if SAME5x
	CMCC->MAINT0.reg = CMCC_MAINT0_INVALL;
#elif SAM4E
	CMCC->CMCC_MAINT0 = CMCC_MAINT0_INVALL;
#endif
	__ISB();
	__DSB();
}

inline void cache_disable() noexcept
{
	while (is_cache_enabled())
	{
#if SAME5x
		CMCC->CTRL.reg = 0;
#elif SAM4E
		CMCC->CMCC_CTRL = 0;
#endif
		__ISB();
		__DSB();
	}
}

inline void cache_enable() noexcept
{
	if (!is_cache_enabled())
	{
		cache_invalidate_all();
#if SAME5x
		CMCC->CTRL.reg = CMCC_CTRL_CEN;
#elif SAM4E
		CMCC->CMCC_CTRL = CMCC_CTRL_CEN;
#endif
		__ISB();
		__DSB();
	}
}

#endif

void Cache::Init() noexcept
{

#if SAME70 && USE_MPU

# if 0
// For debugging
#  define CACHE_MODE	ARM_MPU_CACHEP_WT_NWA
# else
// Normal operation
#  define CACHE_MODE	ARM_MPU_CACHEP_WB_WRA
# endif

	// Set up the MPU so that we can have a non-cacheable RAM region, and so that we can trap accesses to non-existent memory
	// Where regions overlap, the region with the highest region number takes priority
	constexpr ARM_MPU_Region_t regionTable[] =
	{
		// Flash memory: read-only, execute allowed, cacheable
		{
			ARM_MPU_RBAR(0, IFLASH_ADDR),
			ARM_MPU_RASR_EX(0u, ARM_MPU_AP_RO, ARM_MPU_ACCESS_NORMAL(ARM_MPU_CACHEP_WB_WRA, ARM_MPU_CACHEP_WB_WRA, 0u), 0u, ARM_MPU_REGION_SIZE_1MB)
		},
		// First 512b of the flash memory is also the flash write page buffer, which we need to write to when writing the user page
		{
			ARM_MPU_RBAR(1, IFLASH_ADDR),
			ARM_MPU_RASR_EX(0u, ARM_MPU_AP_FULL, ARM_MPU_ACCESS_NORMAL(ARM_MPU_CACHEP_WT_NWA, ARM_MPU_CACHEP_WT_NWA, 0u), 0u, ARM_MPU_REGION_SIZE_512B)
		},
		// First 256kb RAM, read-write, cacheable, execute disabled (parts of this are overridden later)
		{
			ARM_MPU_RBAR(2, IRAM_ADDR),
			ARM_MPU_RASR_EX(1u, ARM_MPU_AP_FULL, ARM_MPU_ACCESS_NORMAL(CACHE_MODE, CACHE_MODE, 0u), 0u, ARM_MPU_REGION_SIZE_256KB)
		},
		// First 64kb RAM, read-write, shared, execute disabled
		{
			ARM_MPU_RBAR(3, IRAM_ADDR),
			ARM_MPU_RASR_EX(1u, ARM_MPU_AP_FULL, ARM_MPU_ACCESS_NORMAL(ARM_MPU_CACHEP_NOCACHE, ARM_MPU_CACHEP_NOCACHE, 1u), 0u, ARM_MPU_REGION_SIZE_64KB)
		},
		// Next 32kb RAM, read-write, shared, execute disabled
		{
			ARM_MPU_RBAR(4, IRAM_ADDR + 0x00010000),
			ARM_MPU_RASR_EX(1u, ARM_MPU_AP_FULL, ARM_MPU_ACCESS_NORMAL(ARM_MPU_CACHEP_NOCACHE, ARM_MPU_CACHEP_NOCACHE, 1u), 0u, ARM_MPU_REGION_SIZE_32KB)
		},
		// RAMFUNC memory. Read-only (the code has already been written to it), execution allowed. The initialised data memory follows, so it must be RW.
		// 256 bytes is enough at present (check the linker memory map if adding more RAMFUNCs).
		{
			ARM_MPU_RBAR(5, IRAM_ADDR + 0x00018000),
			ARM_MPU_RASR_EX(0u, ARM_MPU_AP_FULL, ARM_MPU_ACCESS_NORMAL(CACHE_MODE, CACHE_MODE, 0u), 0u, ARM_MPU_REGION_SIZE_256B)
		},
		// Final 128kb RAM, read-write, cacheable, execute disabled
		{
			ARM_MPU_RBAR(6, IRAM_ADDR + 0x00040000),
			ARM_MPU_RASR_EX(1u, ARM_MPU_AP_FULL, ARM_MPU_ACCESS_NORMAL(CACHE_MODE, CACHE_MODE, 0u), 0u, ARM_MPU_REGION_SIZE_128KB)
		},
		// Peripherals
		{
			ARM_MPU_RBAR(7, 0x40000000),
			ARM_MPU_RASR_EX(1u, ARM_MPU_AP_FULL, ARM_MPU_ACCESS_DEVICE(1u), 0u, ARM_MPU_REGION_SIZE_16MB)
		},
		// USBHS
		{
			ARM_MPU_RBAR(8, 0xA0100000),
			ARM_MPU_RASR_EX(1u, ARM_MPU_AP_FULL, ARM_MPU_ACCESS_DEVICE(1u), 0u, ARM_MPU_REGION_SIZE_1MB)
		},
		// ROM
		{
			ARM_MPU_RBAR(9, IROM_ADDR),
			ARM_MPU_RASR_EX(0u, ARM_MPU_AP_RO, ARM_MPU_ACCESS_NORMAL(ARM_MPU_CACHEP_WT_NWA, ARM_MPU_CACHEP_WT_NWA, 0u), 0u, ARM_MPU_REGION_SIZE_4MB)
		},
		// ARM Private Peripheral Bus
		{
			ARM_MPU_RBAR(10, 0xE0000000),
			ARM_MPU_RASR_EX(1u, ARM_MPU_AP_FULL, ARM_MPU_ACCESS_ORDERED, 0u, ARM_MPU_REGION_SIZE_1MB)
		}
	};

	static_assert(ARRAY_SIZE(regionTable) <= 16);		// SAME70 supports 16 regions

	// Ensure MPU is disabled
	ARM_MPU_Disable();

	// Clear all regions
	const uint32_t numRegions = (MPU->TYPE & MPU_TYPE_DREGION_Msk) >> MPU_TYPE_DREGION_Pos;
	for (unsigned int region = 0; region < numRegions; ++region)
	{
		ARM_MPU_ClrRegion(region);
	}

	// Load regions from our table
	ARM_MPU_Load(regionTable, ARRAY_SIZE(regionTable));

	// Enable the MPU, disabling the default map but allowing exception handlers to use it
	ARM_MPU_Enable(0x01);
#elif SAME5x
	CMCC->MCFG.reg = CMCC_MCFG_MODE_DHIT_COUNT;		// data hit mode
	CMCC->MEN.bit.MENABLE = 1;
#elif SAM4E
	CMCC->CMCC_MCFG = 2;							// data hit mode
	CMCC->CMCC_MEN |= CMCC_MEN_MENABLE;
#endif
}

void Cache::Enable() noexcept
{
#if SAME70
	if ((SCB->CCR & SCB_CCR_IC_Msk) == 0)			// if instruction cache is not enabled
	{
		SCB_EnableICache();
	}

	if ((SCB->CCR & SCB_CCR_DC_Msk) == 0)			// if data cache is not enabled
	{
		SCB_EnableDCache();
	}
#else
	cache_enable();
#endif
}

// Disable the cache, returning true if it was enabled
bool Cache::Disable() noexcept
{
#if SAME70
	if ((SCB->CCR & SCB_CCR_IC_Msk) != 0)			// if instruction cache is enabled
	{
		SCB_DisableICache();
	}

	const bool wasEnabled = (SCB->CCR & SCB_CCR_DC_Msk) != 0;
	if (wasEnabled)									// if data cache is enabled
	{
		// Warning: this code is fragile! There must be no memory writes while flushing the data cache, hence we must disable interrupts.
		const irqflags_t flags = cpu_irq_save();
		SCB_DisableDCache();						// this cleans it as well as disabling it
		cpu_irq_restore(flags);
	}
#else
	const bool wasEnabled = is_cache_enabled();
	cache_disable();
#endif
	return wasEnabled;
}

#if SAME70

extern "C" [[noreturn]] void vAssertCalled(uint32_t line, const char *file) noexcept;

void Cache::Flush(const volatile void *start, size_t length) noexcept
{
	if ((SCB->CCR & SCB_CCR_DC_Msk) != 0)			// if data cache is enabled
	{
		// The DMA buffer should be entirely inside the non-cached RAM area
		if ((const char *)start < (const char *)&_nocache_ram_start || (const char *)start + length >= (const char *)&_nocache_ram_end)
		{
			vAssertCalled(__LINE__, __FILE__);
		}
	}
}

#endif

void Cache::Invalidate(const volatile void *start, size_t length) noexcept
{
#if SAME70
	if ((SCB->CCR & SCB_CCR_DC_Msk) != 0)			// if data cache is enabled
	{
		// The DMA buffer should be entirely inside the non-cached RAM area
		if ((const char *)start < (const char *)&_nocache_ram_start || (const char *)start + length >= (const char *)&_nocache_ram_end)
		{
			vAssertCalled(__LINE__, __FILE__);
		}
	}
#else
	// We just invalidate the whole cache
	if (is_cache_enabled())
	{
		// Disable interrupts to prevent a task switch, otherwise we may end up with the cache disabled in another task
		const irqflags_t flags = cpu_irq_save();
		cache_disable();
		cache_invalidate_all();
		cache_enable();
		cpu_irq_restore(flags);
	}
	else
	{
		cache_invalidate_all();
	}
#endif
}

#if SAM4E || SAME5x

uint32_t Cache::GetHitCount() noexcept
{
#if SAM4E
	return CMCC->CMCC_MSR;
#elif SAME5x
	return CMCC->MSR.reg;
#endif
}

#endif

#endif	// SAM4E || SAME70 || SAME5x

// Entry points that can be called from ASF C code
void CacheFlushBeforeDMAReceive(const volatile void *start, size_t length) noexcept { Cache::FlushBeforeDMAReceive(start, length); }
void CacheInvalidateAfterDMAReceive(const volatile void *start, size_t length) noexcept { Cache::InvalidateAfterDMAReceive(start, length); }
void CacheFlushBeforeDMASend(const volatile void *start, size_t length) noexcept { Cache::FlushBeforeDMASend(start, length); }

// End

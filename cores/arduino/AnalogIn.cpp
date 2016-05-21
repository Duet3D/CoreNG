/*
 * AnalogInput.cpp
 *
 *  Created on: 2 Apr 2016
 *      Author: David
 */

#include "Core.h"
#include "AnalogIn.h"

#if SAM3XA
# include "adc/adc.h"
#elif SAM4E
# include "afec/afec.h"
#else
# error Analog input module not written for this processor
#endif

#include "pmc.h"

#if SAM3XA
const unsigned int numChannels = 16;
#endif
#if SAM4E
const unsigned int numChannels = 32;
#endif

static uint32_t activeChannels = 0;
static AnalogCallback_t callbackFn = nullptr;

#if SAM3XA
static inline adc_channel_num_t GetAdcChannel(AnalogChannelNumber channel)
{
	return static_cast<adc_channel_num_t>((unsigned int)channel);
}
#endif
#if SAM4E
static inline Afec *GetAfec(AnalogChannelNumber channel)
{
	return (channel >= 16) ? AFEC1 : AFEC0;
}

static inline afec_channel_num GetAfecChannel(AnalogChannelNumber channel)
{
	return static_cast<afec_channel_num>((unsigned int)channel & 15);
}
#endif

// Module initialisation
void AnalogInInit()
{
#if SAM3XA
	pmc_enable_periph_clk(ID_ADC);
	adc_init(ADC, SystemCoreClock, ADC_FREQ_MIN, ADC_STARTUP_TIME_12);	// Reduce clock rate
	adc_configure_timing(ADC, 3, ADC_SETTLING_TIME_3, 1);			// Add transfer time
	adc_configure_trigger(ADC, ADC_TRIG_SW, 0);						// Disable hardware trigger
	adc_disable_interrupt(ADC, 0xFFFFFFFF);							// Disable all ADC interrupts
	adc_disable_all_channel(ADC);
#endif
#if SAM4E
	afec_enable(AFEC0);
	afec_enable(AFEC1);
	afec_config cfg;
	afec_get_config_defaults(&cfg);

#if 0	// these are probably not needed, the defaults should be OK
//	cfg.afec_clock = 2000000UL;						// reduce clock frequency
//	cfg.settling_time = AFEC_SETTLING_TIME_3;
#endif

	while (afec_init(AFEC0, &cfg) != STATUS_OK)
	{
		(void)afec_get_latest_value(AFEC0);
	}
	while (afec_init(AFEC1, &cfg) != STATUS_OK)
	{
		(void)afec_get_latest_value(AFEC1);
	}
	afec_disable_interrupt(AFEC0, AFEC_INTERRUPT_ALL);
	afec_disable_interrupt(AFEC1, AFEC_INTERRUPT_ALL);
	afec_set_trigger(AFEC0, AFEC_TRIG_SW);
	afec_set_trigger(AFEC1, AFEC_TRIG_SW);
#endif
}

// Enable or disable a channel. Use AnalogCheckReady to make sure the ADC is ready before calling this.
void AnalogInEnableChannel(AnalogChannelNumber channel, bool enable)
{
	if ((unsigned int)channel < numChannels)
	{
		if (enable)
		{
			activeChannels |= (1 << channel);
#if SAM3XA
			adc_enable_channel(ADC, GetAdcChannel(channel));
#endif
#if SAM4E
			afec_ch_config cfg;
			afec_ch_get_config_defaults(&cfg);
			afec_ch_set_config(GetAfec(channel), GetAfecChannel(channel), &cfg);
			afec_channel_set_analog_offset(GetAfec(channel), GetAfecChannel(channel), 2048);	// need this to get the full ADC range
			afec_channel_enable(GetAfec(channel), GetAfecChannel(channel));
			afec_start_calibration(GetAfec(channel));					// do automatic calibration
#endif
		}
		else
		{
			activeChannels &= ~(1 << channel);
#if SAM3XA
			adc_disable_channel(ADC, GetAdcChannel(channel));
#endif
#if SAM4E
			afec_channel_disable(GetAfec(channel), GetAfecChannel(channel));
#endif
		}
	}
}

// Read the most recent 12-bit result from a channel
uint16_t AnalogInReadChannel(AnalogChannelNumber channel)
{
	if ((unsigned int)channel < numChannels)
	{
#if SAM3XA
		return adc_get_channel_value(ADC, GetAdcChannel(channel));
#endif
#if SAM4E
		return afec_channel_get_value(GetAfec(channel), GetAfecChannel(channel));
#endif
	}
	return 0;
}

// Set up a callback for when all conversions have been completed. Returns the previous callback pointer.
AnalogCallback_t AnalogInSetCallback(AnalogCallback_t fn)
{
	AnalogCallback_t oldFn = fn;
	callbackFn = fn;
	if (fn == nullptr)
	{
		//TODO
	}
	else
	{
		//TODO
	}
	return oldFn;
}

#if SAM4E
static void StartConversion(Afec *afec)
{
	// Clear out any existing conversion complete bits in the status register
	for (uint32_t chan = 0; chan < 16; ++chan)
	{
		if ((afec_get_interrupt_status(afec) & (1 << chan)) != 0)
		{
			(void)afec_channel_get_value(afec, static_cast<afec_channel_num>(chan));
		}
	}
	afec_start_software_conversion(afec);
}
#endif

// Start converting the enabled channels
void AnalogInStartConversion(uint32_t channels)
{
#if SAM3XA
	// Clear out any existing conversion complete bits in the status register
	for (uint32_t chan = 0; chan < 16; ++chan)
	{
		if ((adc_get_status(ADC) & (1 << chan)) != 0)
		{
			(void)adc_get_channel_value(ADC, static_cast<adc_channel_num_t>(chan));
		}
	}
	adc_start(ADC);
#endif
#if SAM4E
	channels &= activeChannels;
	if (channels & 0x0000FFFF)
	{
		StartConversion(AFEC0);
	}
	if (channels & 0xFFFF0000)
	{
		StartConversion(AFEC1);
	}
#endif
}

// Check whether all conversions have been completed since the last call to AnalogStartConversion
bool AnalogInCheckReady(uint32_t channels)
{
#if SAM3XA
	const uint32_t mask = channels & activeChannels;
	return (adc_get_status(ADC) & mask) == mask;
#endif
#if SAM4E
	channels &= activeChannels;
	const uint32_t afec0Mask = channels & 0x0000FFFF;
	const uint32_t afec1Mask = (channels >> 16) & 0x0000FFFF;
	return (afec_get_interrupt_status(AFEC0) & afec0Mask) == afec0Mask
		&& (afec_get_interrupt_status(AFEC1) & afec1Mask) == afec1Mask;
#endif
}

// Convert an Arduino Due analog pin number to the corresponding ADC channel number
AnalogChannelNumber PinToAdcChannel(uint32_t pin)
{
#if SAM3XA
	// Arduino Due uses separate analog pin numbers
	if (pin < A0)
	{
		pin += A0;
	}
#endif
	return g_APinDescription[pin].ulADCChannelNumber;
}

// End

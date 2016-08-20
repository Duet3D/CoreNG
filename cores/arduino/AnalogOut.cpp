/*
 Copyright (c) 2011 Arduino.  All right reserved.

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 See the GNU Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "Core.h"
#include "AnalogOut.h"

#include "pwm/pwm.h"
#include "tc/tc.h"
#include "dacc/dacc.h"

// Initialise this module
extern void AnalogOutInit()
{
	// Nothing to do yet
}

// Convert a float in 0..1 to unsigned integer in 0..N
static inline uint32_t ConvertRange(float f, uint32_t top)
pre(0.0 <= ulValue; ulValue <= 1.0)
post(result <= top)
{
	return lround(f * (float)top);
}

// AnalogWrite to a DAC pin
// Return true if successful, false if we need to fall back to digitalWrite
static bool AnalogWriteDac(const PinDescription& pinDesc, float ulValue)
pre(0.0 <= ulValue; ulValue <= 1.0)
pre((pinDesc.ulAttribute & PIN_ATTR_DAC) != 0)
{
	const AnalogChannelNumber channel = pinDesc.ulADCChannelNumber;
	const uint32_t chDACC = ((channel == DA0) ? 0 : 1);
	if (dacc_get_channel_status(DACC_INTERFACE) == 0)
	{
		// Enable clock for DACC_INTERFACE
		pmc_enable_periph_clk(DACC_INTERFACE_ID);
		// Reset DACC registers
		dacc_reset(DACC_INTERFACE);
		// Half word transfer mode
		dacc_set_transfer_mode(DACC_INTERFACE, 0);
#if SAM4E
		// The data sheet says we must also set this bit when using a peripheral clock frequency >100MHz
		DACC->DACC_MR |= DACC_MR_CLKDIV_DIV_4;
#endif

#if (SAM3S) || (SAM3XA)
		/* Power save:
		 * sleep mode - 0 (disabled)
		 * fast wakeup - 0 (disabled)
		 */
		dacc_set_power_save(DACC_INTERFACE, 0, 0);
		/* Timing:
		 * refresh - 0x08 (1024*8 dacc clocks)
		 * max speed mode - 0 (disabled)
		 * startup time - 0x10 (1024 dacc clocks)
		 */
		dacc_set_timing(DACC_INTERFACE, 0x08, 0, 0x10);
#endif
		// Set up analog current
		dacc_set_analog_control(DACC_INTERFACE, DACC_ACR_IBCTLCH0(0x02) | DACC_ACR_IBCTLCH1(0x02) | DACC_ACR_IBCTLDACCORE(0x01));
	}

	// Disable TAG and select output channel chDACC
	dacc_set_channel_selection(DACC_INTERFACE, chDACC);
	if ((dacc_get_channel_status(DACC_INTERFACE) & (1 << chDACC)) == 0)
	{
		dacc_enable_channel(DACC_INTERFACE, chDACC);
	}

	// Write user value - need to convert it from 8 to 12 bit resolution
	dacc_write_conversion_data(DACC_INTERFACE, ConvertRange(ulValue, (1 << DACC_RESOLUTION) - 1));
	while ((dacc_get_interrupt_status(DACC_INTERFACE) & DACC_ISR_EOC) == 0) {}
	return true;
}

#if SAM3XA
const unsigned int numPwmChannels = 8;
#endif
#if SAM4E
const unsigned int numPwmChannels = 4;
#endif

static bool PWMEnabled = false;
static uint16_t PWMChanFreq[numPwmChannels] = {0};
static uint16_t PWMChanPeriod[numPwmChannels];

// AnalogWrite to a PWM pin
// Return true if successful, false if we need to fall back to digitalWrite
static bool AnalogWritePwm(const PinDescription& pinDesc, float ulValue, uint16_t freq)
pre(0.0 <= ulValue; ulValue <= 1.0)
pre((pinDesc.ulAttribute & PIN_ATTR_PWM) != 0)
{
	const uint32_t chan = pinDesc.ulPWMChannel;
	if (freq == 0)
	{
		PWMChanFreq[chan] = freq;
		return false;
	}
	else if (PWMChanFreq[chan] != freq)
	{
		if (!PWMEnabled)
		{
			// PWM Startup code
			pmc_enable_periph_clk(PWM_INTERFACE_ID);
			pwm_clock_t clockConfig;
			clockConfig.ul_clka = PwmSlowClock;
			clockConfig.ul_clkb = PwmFastClock;
			clockConfig.ul_mck = VARIANT_MCK;
			pwm_init(PWM, &clockConfig);
			PWMEnabled = true;
		}

		pwm_channel_disable(PWM, chan);
		bool useFastClock = (freq >= PwmFastClock/65535);
		uint32_t period = ((useFastClock) ? PwmFastClock : PwmSlowClock)/freq;
		// Setup PWM for this PWM channel
		pio_configure(pinDesc.pPort,
				pinDesc.ulPinType,
				pinDesc.ulPin,
				pinDesc.ulPinConfiguration);
		pwm_channel_t channelConfig;
		memset(&channelConfig, 0, sizeof(channelConfig));		// clear unused fields
		channelConfig.ul_prescaler = (useFastClock) ? PWM_CMR_CPRE_CLKB : PWM_CMR_CPRE_CLKA;
		channelConfig.ul_period = period;
		channelConfig.ul_duty = ConvertRange(ulValue, period);
		channelConfig.channel = chan;
		pwm_channel_init(PWM, &channelConfig);
		pwm_channel_enable(PWM, chan);

		PWMChanFreq[chan] = freq;
		PWMChanPeriod[chan] = (uint16_t)period;
	}
	else
	{
		// We have to pass a pwm_channel_t struct to pwm_channel_update duty, but the only fields it reads are 'chan' and 'ul_period'.
		pwm_channel_t channelConfig;
		channelConfig.channel = chan;
		channelConfig.ul_period = (uint32_t)PWMChanPeriod[chan];
		pwm_channel_update_duty(PWM, &channelConfig, ConvertRange(ulValue, channelConfig.ul_period));
	}
	return true;
}

const unsigned int numTcChannels = 9;

// Map from timer channel to TC channel number
static const uint8_t channelToChNo[] = { 0, 1, 2, 0, 1, 2, 0, 1, 2 };

// Map from timer channel to TC number
static Tc * const channelToTC[] = { TC0, TC0, TC0,
									TC1, TC1, TC1,
									TC2, TC2, TC2 };

// Map from timer channel to TIO number
static const uint8_t channelToId[] = { ID_TC0, ID_TC1, ID_TC2,
									   ID_TC3, ID_TC4, ID_TC5,
									   ID_TC6, ID_TC7, ID_TC8 };

// Current frequency of each TC channel
static uint16_t TCChanFreq[numTcChannels] = {0};

static inline void TC_SetCMR_ChannelA(Tc *tc, uint32_t chan, uint32_t v)
{
	tc->TC_CHANNEL[chan].TC_CMR = (tc->TC_CHANNEL[chan].TC_CMR & 0xFFF0FFFF) | v;
}

static inline void TC_SetCMR_ChannelB(Tc *tc, uint32_t chan, uint32_t v)
{
	tc->TC_CHANNEL[chan].TC_CMR = (tc->TC_CHANNEL[chan].TC_CMR & 0xF0FFFFFF) | v;
}

// AnalogWrite to a TC pin
// Return true if successful, false if we need to fall back to digitalWrite
// WARNING: this will screw up big time if you try to use both the A and B outputs of the same timer at different frequencies.
// The DuetNG board uses only A outputs, so this is OK.
static bool AnalogWriteTc(const PinDescription& pinDesc, float ulValue, uint16_t freq)
pre(0.0 <= ulValue; ulValue <= 1.0)
pre((pinDesc.ulAttribute & PIN_ATTR_TIMER) != 0)
{
	const uint32_t chan = (uint32_t)pinDesc.ulTCChannel >> 1;
	if (freq == 0)
	{
		TCChanFreq[chan] = freq;
		return false;
	}
	else
	{
		Tc * const chTC = channelToTC[chan];
		const uint32_t chNo = channelToChNo[chan];
		bool doInit = (TCChanFreq[chan] != freq);

		if (doInit)
		{
			TCChanFreq[chan] = freq;

			// Enable the peripheral clock to this timer
			pmc_enable_periph_clk(channelToId[chan]);

			// Set up the timer mode and top count
			tc_init(chTC, chNo,
							TC_CMR_TCCLKS_TIMER_CLOCK2 |		// clock is MCLK/8 to save a little power and avoid overflow later on
							TC_CMR_WAVE |         				// Waveform mode
							TC_CMR_WAVSEL_UP_RC | 				// Counter running up and reset when equals to RC
							TC_CMR_EEVT_XC0 |     				// Set external events from XC0 (this setup TIOB as output)
							TC_CMR_ACPA_CLEAR | TC_CMR_ACPC_CLEAR |
							TC_CMR_BCPB_CLEAR | TC_CMR_BCPC_CLEAR);
			const uint32_t top = (VARIANT_MCK/8)/(uint32_t)freq;	// with 120MHz clock this varies between 228 (@ 65.535kHz) and 15 million (@ 1Hz)
			// The datasheet doesn't say how the period relates to the RC value, but from measurement it seems that we do not need to subtract one from top
			tc_write_rc(chTC, chNo, top);
		}

		const uint32_t threshold = ConvertRange(ulValue, tc_read_rc(chTC, chNo));
		if (threshold == 0)
		{
			if (((uint32_t)pinDesc.ulTCChannel & 1) == 0)
			{
				tc_write_ra(chTC, chNo, 1);
				TC_SetCMR_ChannelA(chTC, chNo, TC_CMR_ACPA_CLEAR | TC_CMR_ACPC_CLEAR);
			}
			else
			{
				tc_write_rb(chTC, chNo, 1);
				TC_SetCMR_ChannelB(chTC, chNo, TC_CMR_BCPB_CLEAR | TC_CMR_BCPC_CLEAR);
			}

		}
		else
		{
			if (((uint32_t)pinDesc.ulTCChannel & 1) == 0)
			{
				tc_write_ra(chTC, chNo, threshold);
				TC_SetCMR_ChannelA(chTC, chNo, TC_CMR_ACPA_CLEAR | TC_CMR_ACPC_SET);
			}
			else
			{
				tc_write_rb(chTC, chNo, threshold);
				TC_SetCMR_ChannelB(chTC, chNo, TC_CMR_BCPB_CLEAR | TC_CMR_BCPC_SET);
			}
		}

		if (doInit)
		{
			ConfigurePin(pinDesc);
			tc_start(chTC, chNo);
		}
	}
	return true;
}

// Analog write to DAC, PWM, TC or plain output pin
void AnalogOut(Pin pin, float ulValue, uint16_t freq)
{
	if (pin > MaxPinNumber || std::isnan(ulValue))
	{
		return;
	}

	ulValue = constrain<float>(ulValue, 0.0, 1.0);

	const PinDescription& pinDesc = g_APinDescription[pin];
	const uint32_t attr = pinDesc.ulPinAttribute;
	if ((attr & PIN_ATTR_DAC) != 0)
	{
		if (AnalogWriteDac(pinDesc, ulValue))
		{
			return;
		}
	}
	else if ((attr & PIN_ATTR_PWM) != 0)
	{
		if (AnalogWritePwm(pinDesc, ulValue, freq))
		{
			return;
		}
	}
	else if ((attr & PIN_ATTR_TIMER) != 0)
	{
		if (AnalogWriteTc(pinDesc, ulValue, freq))
		{
			return;
		}
	}

	// Fall back to digital write
	pinMode(pin, (ulValue < 0.5) ? OUTPUT_LOW : OUTPUT_HIGH);
}

// End

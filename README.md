# CoreNG
New hardware abstraction layer for RepRapFirmware based on ASF 3.x (originally 3.31, currently 3.45).

This is a new hardware abstraction layer for RepRapFirmware. It replaces the CoreDuet project in releases 1.11 and later of my fork of RepRapFirmware. It is based on Atmel Software Framework version 3.x and vestiges of the Arduino Due core.

The main advantages over the previous CoreDuet project are support for more recent ATSAM processors (including the one used in the Duet WiFi), support for PWM outputs via timer/counter channels, and the use of the on-chip hardware ADC scheduler. Also the USB interface is interrupt and DMA driven instead of polled.

To build it, import the project into a recent version of Eclipse, select the desired configuration (SAM3X8E, SAM4E8E, SAM4S etc.), and press Build. The build depends on the Eclipse workspace variable 'ArmGccPath" being set to the directory where your arm-none-eabi-g++ compiler resides, for example "C:\Program Files (x86)\GNU Tools ARM Embedded\8 2018-q4-major\bin" on Windows. To set it, in Eclipse go to Windows -> Preferences -> C/C++ -> Build -> Build Variables and click "Add..."

# License

Code written by Duet3D and Arduino is licensed under GPLv3, see http://www.gnu.org/licenses/gpl-3.0.en.html. The files from the ASF have a more retrictive license that prohibits their use in software for non-Atmel/Microchip processors.

# Notes on the API
Prior to 2019-05-03 separate builds of CoreNG were used for non-RTOS projects (e.g. DuetIAP, and RepRapFirmware for older boards/processors) and FreeRTOS-based projects (e.g. RepRapFirmware for the Duet 2 series of electronics). Now we use a single build of CoreNG for both types of projects. The new startup and systick API is as follows, with all API functions declared extern "C".

**Functions that must be provided by the application**

UrgentInit() - this is called by CoreNG early on during startup, before global C++ objects have been initialised. Use it to perform urgent hardware initialisation. Avoid using global C++ objects in this function.

AppMain() - this is called by CoreNG at the end of startup to run the application. It should not return.

SysTick_Handler() or vApplicationTickHook() - for non-RTOS systems, SysTick_Handler is the tick interrupt service routine. For RTOS-bases systems, vApplicationTickHook is called from the tick interrupt service routine within FreeRTOS. In either case, it must call CoreSysTick() and reset the watchdog(s). It may then do anything else needed by the application.

SVC_Handler() and PendSV_Handler() - a non-RTOS project must provide dummy handlers for these exceptions.

**Functions provided by CoreNG**

InitSysTick() - in a non-RTOS project, call this at/near the start of AppMain to initialise the system tick hardware. In a FreeRTOS-based project, the FreeRTOS initialisation initialises the system tick hardware

CoreSysTick() - call this from the systick interrupt handler or systick interrupt hook, to keep the milliseconds timer up to date.

CoreNG also provides functions for driving various aspects of the microcontroller peripherals.

DC, 2019-05-03.

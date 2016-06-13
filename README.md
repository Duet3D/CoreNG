# CoreNG
New hardware abstraction layer for RepRapFirmware based on ASF 3.31

This is a new hardware abstraction layer for RepRapFirmware. It replaces the CoreDuet project in releases 1.11 and later of my fork of RepRapFirmware. It is based on Atmel Software Framework version 3.31 and also includes modules from the Arduino Due core.

The main advantages over the previous CoreDuet project are support for more recent ATSAM processors (including the one used in the next-generation Duet), support for PWM outputs via timer/counter channels, and the use of the on-chip hardware ADC scheduler. Also the USB interface is interrupt and DMA driven instead of polled.

To build it, import the project into Eclipse Mars 2, select the desired configuration (SAM3X8E or SAM4E8E), and press Build.

License: GPLv3, see http://www.gnu.org/licenses/gpl-3.0.en.html.

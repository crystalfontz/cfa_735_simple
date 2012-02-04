Crystalfontz CFA-735 Example User Firmware Source Code
------------------------------------------------------

Crystalfontz America, Inc. makes this firmware availabe to our users
and developers to demonstrate how to access the various components
that make up the CFA-735 module.

Additional modules can be purchased from:
http://www.crystalfontz.com/search.php?q=CFA735

Support can be found through our forums:
http://forum.crystalfontz.com/forumdisplay.php?37-CFA-735-User-Code

License
-------

Copyright 2012 Crystalfontz America, Inc.

Licensed under the Apache License, Version 2.0 (the "License"); you
may not use this software except in compliance with the License.  You
may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
implied.  See the License for the specific language governing
permissions and limitations under the License.

Warning
-------

Installation of custom firmware on a CFA-735 will remove the firmware
supported by Crystalfontz.  There is no method for reinstalling the
Crystalfontz firmware on a CFA-735 without returning the module to
Crystalfontz.


Recommended compiler
--------------------

Sourcery G++ Lite 2011.03-42 for ARM EABI from Mentor Graphics
https://sourcery.mentor.com/sgpp/lite/arm/portal/release1802

Recommended On Chip Debugger
----------------------------

OpenOCD
http://openocd.sourceforge.net/getting-openocd/

Known working JTAG interfaces:
http://www.olimex.com/dev/arm-usb-ocd.html

JTAG Interface
--------------

The JTAG interface is most easily accessed through the H1 connector.

The pin connections between the H1 connector and the 20 pin connector
found on most ARM JTAG adapters:

ARM JTAG          Signal                H1
--------          ------                --
1                 PWR                   16
20                GND                   15
3                 NTRST                 7
5                 TDI                   9
7                 TMS                   11
9                 TCK                   13
13                TDO                   10
15                NRST                  14


Known Issues
------------

When using some JTAG adapters, minor ESD events may cause the LCD
controller to reset.  After that event the LCD controller will need to
be reinitialized.  This has only been experianced when the JTAG is
connected.

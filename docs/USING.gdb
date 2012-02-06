The GNU Debugger, GDB, works easily with OpenOCD in order to debug
running code on an STM32F103 microcontroller.  GDB is included in
Sourcery G++ Lite as arm-none-eabi-gdb.

We have included a small GDB script to set up GDB and connect it to a
running OpenOCD daemon on localhost at the standard port.

Running GDB
-----------

Usually GDB will be run like this from the cfa_735_simple repository
directory.

$ arm-none-eabi-gdb -x gdb-remote-prep.gdb src/cfa735.elf

The gdb-remote-prep.gdb script connects to openocd and disables
interrupts on the microprocessor while the code is halted in the
debugger.  The ELF image src/cfa735.elf is loaded into GDB for it to
use as its symbols.

Accessing OpenOCD
-----------------

Any command that is usable on the OpenOCD telnet console is accessible
from GDB using the "monitor" or "mon" command.

(gdb) mon halt
target state: halted
target halted due to debug-request, current mode: Thread 
xPSR: 0x81000000 pc: 0x0800333e msp: 0x2000bfb8

Disconnect
----------

You may need to disconnect from OpenOCD.  You can use the "disconnect"
or "disc" command to do so.

(gdb) disc
Ending remote debugging.


Target
------

To manually connect to a GDB remote (in our case OpenOCD) you can use
the "target" command with the parameters of "remote" and a hostname
and port in the form of "hostname:portnumber".  You may leave out the
hostname if you are connecting to the localhost.

(gdb) target remote :3333
Remote debugging using :3333
cortex_m3 interrupt mask on
0x0800333e in ST7529_writeDataByte (ST7529_Byte=0 '\000') at st7529_core.c:548
548	    ST7529_DATA_GPIO->BSRR = (uint32_t)(ST7529_Byte) | (uint32_t)(ST7529_DATA_RESET_MASK);

Continue
--------

Continure will resume the program in the debugger until a breakpoint
if hit or another break condition is met.  "continue", "cont", or even
"c" can be used to continue.

(gdb) c
cortex_m3 interrupt mask off
Continuing.


Manual Break
------------

If your code is running and you wish break in the debugger, press
Control-C.

Breakpoints
-----------

Breakpoints can be added with the "break" command.  You can specify
file names and line numbers, addresses, function names, etc.

(gdb) break main
Breakpoint 1 at 0x8002984: file main.c, line 58.
(gdb) break leds.c:20
Breakpoint 2 at 0x8003744: file leds.c, line 20.

Deleting breakpoints can be done with "delete break".  "delete break"
alone deletes all breakpoints or you can specify a breakpoint number
to delete.

(gdb) delete break 1

Loading new code
----------------

While the debugger and processor are halted you can use the load
command to reprogram the flash in the chip.  This is easier if you are
dubugging and testing changes than using the OpenOCD letnet interface.

(gdb) load
Loading section .isr_vector, size 0x1e4 lma 0x8000000
Loading section .text, size 0x47d4 lma 0x80001e4
Loading section .data, size 0x110 lma 0x80049b8
Start address 0x80001e4, load size 19144
Transfer rate: 13 KB/sec, 4786 bytes/write.



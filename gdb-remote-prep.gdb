target remote localhost:3333
mon gdb_sync
#turn off timers and peripherals while we are stopped
mon mww 0xe0042004 0x1fff00
#mask interupts when we stop, re-enable when we turn back on.
define hook-stop
mon cortex_m3 maskisr on
end
define hook-continue
mon cortex_m3 maskisr off
end
define restart
mon reset init
c
end



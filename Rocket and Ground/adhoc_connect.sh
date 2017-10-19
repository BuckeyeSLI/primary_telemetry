#!/bin/bash
sudo ip link set wls2 down
sudo iw dev wls2 set type ibss
sudo ip link set wls2 up
sudo iw dev wls2 ibss join Telemetry_Test 2412

# Note the frequency is the last number on line 5. This should be determined using
# the following command:
# iw dev wlp0s29f7u3 scan | less

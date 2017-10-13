# ForgeGhost
Basic concept : to listen the press blows with a piezo cell linked to an UNO.
UNO send the points to a Raspberry (serial com).
Raspberry sends data to a linux server by ethernet (WIFI)
Raspberry does a FFT, stores the value (modal frequency, cycle time) and dispatch them to control systems
Control systems :
- internal web page with high refreshing rate
- Control screen
- light board (workshop visual management)

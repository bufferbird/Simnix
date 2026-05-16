´´´  


============================================
Simnix Kernel and OS
============================================


Simnix is a custom Kernel and Operating System, wich is made for AArch64, for an Raspberry Pi 3,
but i plan support for other architectures too, after AArch64 is finished. 

-------------------------------------
WLAN & Keyboard
-------------------------------------


At this time, there isnt any Bluetooth oder Ethernet Driver, (that does not mean that it is not planned or in work), so the plan is, 
to put a Bluetooth Keyboard into a ESP32 with native USB, this ESP32 could also handle WLAN / Ethernet. 

-------------------------------------
Monolith or modular?
-------------------------------------


For beginning this Kernel will be a monolith with some modular support, it is planned to let the user delete useless Drivers. 
At the End, if everything turns perfect, it should be, that you have a minmal installtion, and then select drivers, and can load new ones, using the simnix package manager.


´´´


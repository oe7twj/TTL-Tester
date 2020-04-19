# TTL-Tester
yet another TTL tester for Arduino Mega 

Project Homepage: https://oe7twj.at/index.php?title=Projekte/TTL_Tester

This is a Tester for common TTL Chips (74xx).
The Hardware is very simple and based on Arduino MEGA.




### Nessecary Hardware:

* Arduino MEGA
* Prototype Shield for Mega
* a ZIF socket 
* a button 
* optionally an LCD display (HD44780 kompatibel)
* optionally an P-FET and some resistors


### DRAM Tester vom vom 8Bit-Museum.de

Alternativly to build the hardware yourself, you also can use the DRAM tester Revision 4.1 from 8Bit-Museum.de

https://8bit-museum.de/speichertester-fuer-dram-chips-fuer-arduino-mega-2560-rev-4/


This DRAM Tester has same Hardware and can used as TTL tester without any modification.



### Unterstützte TTL Chips

Zur Zeit werden folgende Chips erkannt und getestet:

<u>14 Pin</u>: 7400,7402,7408,7410,7414,7421,7430,7432,7474,7486,74125,74126,74164

(7414 <-- 7404,7405) -- selber Typ, werden als 7414 angezeigt

(7400 <-- 74132)

16 Pin: 74138,74139,74157,74163,74165,74595

20 Pin: 74541,74573





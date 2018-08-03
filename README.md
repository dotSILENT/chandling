# CHandling
unfinished, garbage code, abandoned

This client-sided plugin was supposed to add the ability of creating custom per-vehicle (not per-model!!) handlings in SA-MP, just like in MTA.

It uses the fact that GTA's vehicle class has a pointer to it's handling data (which is stored in another class).
The game stores a fixed amount of handlings, but we can overwrite the memory of a specific car to change the pointer to our own, custom handling class. This way we can have as many handlings as we want.

I've created a HandlingModifiers class which allows to apply certain modifiers to the handling instead of completely overwriting the values.
This way you can, for example decrease the drag multiplier a little bit.
```c
handlingMgr.vehicleHandlings[createdVehicleID].modifiers.AddModifier(HANDL_FDRAGMULTIPLIER, "*0.97");

handlingMgr.vehicleHandlings[createdVehicleID].modifiers.ApplyModifiersToHandling(&handlingMgr.vehicleHandlings[createdVehicleID].calcHandling);
handlingMgr.vehicleHandlings[createdVehicleID].Recalculate();
```
This code is provided as is, it's not finished. It started out as a complete test with random parts of code copied from other projects, therefore it's total garbage. It's up to anyone to re-write it and make it fully functional.
Obviously, it requires a server-sided plugin which will communicate with the client (by hooking raknet, or by using some custom networking code).
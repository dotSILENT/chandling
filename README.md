# CHandling
**Undergoing a proper rewrite**, this isn't working yet!

This client sided SA:MP plugin adds the ability to assign vehicle handlings to **individual** vehicles.

It uses the fact that GTA's vehicle class has a pointer to it's handling data (which is stored in another class).
The game stores a fixed amount of handlings, but we can overwrite the memory of a specific car to change the pointer to our own, custom handling class. This way we can have as many handlings as we want.

This plugin works together with a server sided plugin which controls all the handling modifications.
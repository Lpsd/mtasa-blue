# Client Deathmatch Headless

This directory is the ownership boundary for production code that can run without launching GTA or loading the Client Deathmatch DLL. Every C++ source below this directory belongs to the `Client Deathmatch Headless` static library.

Move a class here only when it compiles without the Deathmatch precompiled header, process globals, GTA objects, DLL lifecycle, rendering, GUI, or the Lua runtime. When only part of a class qualifies, extract that state or algorithm here and leave the existing class in `logic/` as the runtime adapter.

Service-dependent logic may use narrow injected interfaces declared here. Implementations that access the game or other process-wide services remain outside this directory.

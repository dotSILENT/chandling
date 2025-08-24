# CHandling plugin for SA:MP

This client side SA:MP plugin adds the ability to assign vehicle handling lines to **individual** vehicles by modifying the default behaviour of the game which uses per-model handling lines.

This plugin needs the server to use server side version of the plugin which controls all the handling modifications: [chandling-svr](https://github.com/dotSILENT/chandling-svr)

## Supported SA:MP versions
The plugin uses libcurl to fetch a file with memory offsets from the GitHub repo, meaning that unless there are significant changes, there should be no need for the end-user to download anything when a new version of SA:MP is released.

Currently supported versions:  
* 0.3.7
* 0.3.7 R2
* 0.3.7 R3
* 0.3.7 R4
* 0.3.7 R5
* 0.3DL

## Installation
In order to be injected into the game, this plugin requires ASI Loader

1. Download the latest [release](https://github.com/dotSILENT/chandling/releases)
2. Extract the files to your GTA SA installation directory

## Thanks to
* DK22Pac - for his [plugin sdk](https://github.com/DK22Pac/plugin-sdk) that I took some structs from
* ini - for helping with various problems that occured along the way

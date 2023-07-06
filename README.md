#  STM32 EXPERIMENTAL PLATFORM ( IO )

_( The project is a work in progress, there may be bugs... please, report )_

## Why PlatformIO
* I generally use it at work ( I will only add chips those that I use and can test )
* Full Open Source
* Full PlatformIO support
* Full Python build logic
* Full STLink OpenOCD support
* Do you need anything else ?!?

## [HOW TO](https://www.youtube.com/watch?v=BzYFqrIYYx8)
* Create STM32CubeMX ( STM32L051K8U3 ) and export to MAKE project
* Create PlatformIO project ( as example: STM32L051K8U3 ... [boards](https://github.com/Wiz-IO/wizio-stm/tree/main/boards) )
* Move ALL **stm** files ( that contain "stm" in the filename ) <br>from CubeMX Project **Root**, **Inc** and **Src** to project folder **stm** <br> ( STM32L051K8Ux_FLASH.ld, startup_stm32l051xx.S ...etc )
* Move H files from **Inc** to **include**
* Move C files from **Src** to **src**
* Drivers and CMSIS **not need** - exist with framework
* Add function **void _init(void) { }** to main.c
* [Documetnation](https://github.com/Wiz-IO/wizio-stm/wiki/Documentation)
* [Examples](https://github.com/Wiz-IO/wizio-stm/tree/main/examples)
* [Framework](https://github.com/Wiz-IO/framework-wizio-stm)

![stm](https://raw.githubusercontent.com/Wiz-IO/LIB/master/STM/STM32CubeMX-PlatformIO.png)

## SUPPORT
* [ **MCU ( Boards )** ](https://github.com/Wiz-IO/wizio-stm/tree/main/boards)
* **STM32VIRTUAL Board - for ALL chips from this list**  ( [Read WIKI - The STM32VIRTUAL Board](https://github.com/Wiz-IO/wizio-stm/wiki/The-STM32VIRTUAL--Board) )
* * STM32L0x
* * STM32L1x
* * STM32L4x
* * STM32F0x
* * STM32F1x ( [Example](https://github.com/Wiz-IO/wizio-stm/tree/main/examples/TEST-VIRTUAL) )
* * STM32F3x
* * STM32F4x
* * STM32C0x
* * STM32G0x
* * STM32G4x
* * STM32WLx

## UPLOAD
* STM32 Cube Programmer ( by default, maybe only Windows )
* STM32 ST-LINK CLI Programmer ( stlinkv1, maybe only Windows )
* STLink @ OpenOCD ( by default )
* J-LINK ( TODO )

## DEBUG
* STLink @ OpenOCD
* J-LINK ( TODO )

## TODO
* Documentation

>If you want to help / support or treat me to Coffee  [![paypal](https://www.paypalobjects.com/en_US/i/btn/btn_donate_SM.gif)](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=ESUP9LCZMZTD6)

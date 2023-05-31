#  STM32 EXPERIMENTAL PLATFORM ( IO )

## Why PlatformIO
* I generally use it at work ( I will only add chips those that I use and can test )
* Full Open Source
* Full PlatformIO support
* Full Python build logic
* Full STLink OpenOCD support
* Do you need anything else ?!?

## HOW TO
* Create STM32CubeMX ( STM32L051K8U3 ) and export to MAKE project
* Create PlatformIO project ( as example: STM32L051K8U3 ... [boards](https://github.com/Wiz-IO/wizio-stm/tree/main/boards) )
* Move ALL **stm** files ( that contain "stm" in the filename ) <br>from CubeMX Project **Root**, **Inc** and **Src** to project folder **stm** <br> ( STM32L051K8Ux_FLASH.ld, startup_stm32l051xx.S ...etc )
* Move H files from **Inc** to **include**
* Move C files from **Src** to **src**
* Drivers and CMSIS not need - exist with framework
* [Examples](https://github.com/Wiz-IO/wizio-stm/tree/main/examples)
* [Framework](https://github.com/Wiz-IO/framework-wizio-stm)

![stm](https://raw.githubusercontent.com/Wiz-IO/LIB/master/STM/STM32CubeMX-PlatformIO.png)

## SUPPORT
**Will support all L0, L4 ... etc as "virtual" board** ( later )<br><br>
_For now_
* STM32L051K8U3 
* STM32L431RBT6 ( not tested as hardware )
* STM32L431RCY6TR ( not tested as hardware )
* STM32F100RBT6B
* STM32F100R8T6B
* STM32VIRTUAL Board ( TODO ... load params from project ini )

## UPLOAD
* STM32 Cube Programmer ( maybe only Windows )
* STM32 ST-LINK CLI Programmer ( maybe only Windows )
* STLink @ OpenOCD

## DEBUG
* STLink @ OpenOCD
* J-LINK ( TODO )

## TODO
* Add other chips ( boards )
* Add Hal Drivers & CMSIS
* Support for jlink

>If you want to help / support or treat me to Coffee  [![paypal](https://www.paypalobjects.com/en_US/i/btn/btn_donate_SM.gif)](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=ESUP9LCZMZTD6)

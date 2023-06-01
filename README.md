#  STM32 EXPERIMENTAL PLATFORM ( IO )

_( The project is a work in progress, there may be bugs... please, report )_

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
* STM32L051K8U3 
* STM32L431RBT6 ( not tested as hardware )
* STM32L431RCY6TR ( not tested as hardware )
* STM32F100RBT6B
* STM32F100R8T6B
* **STM32VIRTUAL Board** - Load chip params from Project INI ( [TODO Documentation](https://github.com/Wiz-IO/wizio-stm/blob/main/examples/TEST-VIRTUAL/platformio.ini#L16) )
* * STM32L0x
* * STM32L1x
* * STM32L4x
* * STM32F0x
* * STM32F1x
* * STM32F4x
* * TODO etc

## UPLOAD
* STM32 Cube Programmer ( maybe only Windows )
* STM32 ST-LINK CLI Programmer ( maybe only Windows )
* STLink @ OpenOCD
* J-LINK ( TODO )

## DEBUG
* STLink @ OpenOCD
* J-LINK ( TODO )

>If you want to help / support or treat me to Coffee  [![paypal](https://www.paypalobjects.com/en_US/i/btn/btn_donate_SM.gif)](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=ESUP9LCZMZTD6)

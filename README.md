#  STM32 EXPERIMENTAL PLATFORM 
## 2023 WizIO ( Georgi Angelov )

( is not ready ... don`t install )


* Create STM32CubeMX ( STM32L051K8U3 ... todo other ) and export to MAKE project
* Create PlatformIO project ( STM32L051K8U3 "board" ) 
* Move ALL *stm* files ( that contain "stm" in the name ) from **Inc** and **Src** to project folder **stm** ( STM32L051K8Ux_FLASH.ld, startup_stm32l051xx.S ...etc )
* Move H files from **Inc** to **include**
* Move C files from **Src** to **src**
* Drivers and CMSIS not need - exist with framework ( todo other )

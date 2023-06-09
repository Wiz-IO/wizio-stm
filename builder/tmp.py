'''
Copyright 2023 (c) WizIO ( Georgi Angelov )

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following
   disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
   disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products
   derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
'''

import sys, os, time
from os import listdir
from os.path import join, exists, normpath, dirname, basename
from shutil import copyfile
from platformio import proc, fs
from colorama import Fore

def ERROR(s):
    print(Fore.RED + "[ERROR]", s)
    time.sleep(.1)
    exit(-1) 

def create_linker_script(env):
    print('create_linker()')
    dir = join(env.framework_dir, 'Drivers', 'LD')
    tmp = join(dir, '_FLASH.ld' )
    if not exists( tmp ):
        ERROR("This template '_FLASH.ld' not exists !!!")
    f = open(tmp, 'r')
    txt = f.read(); 
    txt = txt.replace('%stack%', 'ORIGIN(RAM) + LENGTH(RAM)')
    ram_size = env.BoardConfig().get('upload.maximum_ram_size')
    rom_size = env.BoardConfig().get('upload.maximum_size')
    txt = txt.replace('FLASH (rx)      : ORIGIN = null, LENGTH = 0K', 
        'FLASH (rx)  : ORIGIN = 0x%08X, LENGTH = 0x%X' % ( 0x8000000, rom_size ) )
    txt = txt.replace('RAM (xrw)      : ORIGIN = null, LENGTH = 0K', 
        'RAM (xrw)   : ORIGIN = 0x%08X, LENGTH = 0x%X' % ( 0x20000000, ram_size ) )
    f = open( join(env.subst('$PROJECT_DIR'), 'stm', '%s_FLASH.ld' % env.MCU ), 'w' )
    f.write( txt )

def create_hal_config(env):
    print('create_hal_config()')
    # Drivers/STM32L0xx_HAL_Driver/Inc/stm32l0xx_hal_conf_template.h
    file = '%sxx_hal_conf_template.h' % env.DIV.lower()
    tmp = join( env.framework_dir, 'Drivers', '%sxx_HAL_Driver' % env.DIV.upper(), 'Inc', file )
    print(tmp)
    if not exists( tmp ):
        ERROR("Template '%s' not exists !!!" % file)
    file = file.replace('_template', '')
    copyfile( tmp,  join( env.subst('$PROJECT_DIR') , 'stm', file ) )

def CreateProject(env):
    stm = join( env.subst('$PROJECT_DIR') , 'stm' )
    if not exists( stm ):
        ERROR("The 'Project/stm' folder not exists !!!")
        pass
    if listdir( stm ):
        ERROR("The 'Project/stm' folder is not empty !!!")
        pass

    print(env.DIV)
    print(env.SUB)
    print(env.MCU)

    ### LINKER
    dir = join(env.framework_dir, 'Drivers', 'LD')
    print( join(dir, '%s_FLASH.ld' % env.MCU ) )
    if exists( join(dir, '%s_FLASH.ld' % env.MCU ) ):
        print( 'LD' )
        # TODO COPY Drivers/LD/ '%s_FLASH.ld' % env.MCU
    else: 
        create_linker_script(env)

    ### COPY: Drivers/CMSIS/Device/ST/STM32L0xx/Source/Templates/system_stm32l0xx.c
    copyfile( 
        join( env.framework_dir, 'Drivers', 'CMSIS', 'Device', 'ST', env.DIV + 'xx', 'Source', 'Templates', 'system_%sxx.c' % env.DIV.lower()), 
        join( stm, 'system_%sxx.c' % env.DIV.lower() )       
    )

    ### COPY: Drivers/CMSIS/Device/ST/STM32L0xx/Source/Templates/gcc/startup_stm32l051xx.s
    copyfile( 
        join( env.framework_dir, 'Drivers', 'CMSIS', 'Device', 'ST', env.DIV + 'xx', 'Source', 'Templates', 'gcc', 'startup_%s.s' % env.SUB.lower()), 
        join( stm, 'startup_%s.S' % env.SUB.lower() )       
    )

    ### EDIT: Drivers/STM32L0xx_HAL_Driver/Inc/stm32l0xx_hal_conf_template.h
    create_hal_config(env)

    with open(join( env.subst('$PROJECT_DIR'), 'src', 'board.c' ), 'w') as w: 
        w.write('''
/*
 ******************************************************************************
 *
 * Copyright (c) 2023 WizIO.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
*/ 

#include "%sxx_hal.h"

void __attribute__((weak)) _init(void) { }      

#if 0 /* for printf */
int __attribute__((weak)) _write(int file, char *ptr, int len)
{
  for (int i = 0; i < len; i++)
    HAL_UART_Transmit(&huart_x, (const uint8_t *)ptr++, 1, -1);
  return len;
  // setvbuf(stdout, NULL, _IONBF, 0); // at uart init
}
#endif

///////////////////////////////////////////////////////////////////////////////

static void SystemClock_Config(void)
{
    // USER INIT
}

void board_config(void)
{
    HAL_Init();
    SystemClock_Config();

    // HAL INIT OTHER
}

'''  % env.DIV.lower() )

    with open(join( env.subst('$PROJECT_DIR'), 'src', 'main.c' ), 'w') as w: w.write(
'''
/*
 ******************************************************************************
 *
 * Copyright (c) 2023 WizIO.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
*/

#include "%sxx_hal.h"
void board_config(void);

int main(void)
{
    board_config();

    while (1)
    {
    
    }
}
''' % env.DIV.lower() )
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

import sys, os
from os.path import join, exists
from os import listdir
from platformio import proc
from SCons.Script import Builder

def dev_uploader(target, source, env): # TODO
    print("UPLOAD TODO !!!")
    exit(0)    
    # "C:/Program Files/STMicroelectronics/STM32Cube/STM32CubeProgrammer/bin/STM32_Programmer_CLI" -c port=SWD freq=4000 -w $< --start
    APP_PATH = dev_get_value(env, 'STM32_PRG_CLI', 'C:/Program Files/STMicroelectronics/STM32Cube/STM32CubeProgrammer/bin/STM32_Programmer_CLI') # INIDOC
    args = [ APP_PATH, '-c', 'port=SWD', 'freq=4000', '-w', 'ELF', '--start' ]
    res = proc.exec_command( args, stdout=sys.stdout, stderr=sys.stderr, stdin=sys.stdin, cwd='' )
    print(res)

def dev_get_value(env, name, default):
    res = env.GetProjectOption('custom_%s' % name, # ini user config
           env.BoardConfig().get('build.%s' % name, default) ) # from board
    if res == 'ERROR': 
        print('[ERROR] Cannot find setting', name)
        exit(1)
    return res

def dev_create_template(env, template=None):
    dir = join( env.subst('$PROJECT_DIR'), 'src' )
    if not listdir( dir ):
        if template:
            template(env)  
        dir = join( env.subst('$PROJECT_DIR'), 'stm' )
        if dir and not exists(dir):
            os.makedirs(dir, exist_ok=True)      

def dev_init_compiler(env, application_name = 'APPLICATION'):
    env.DIV = env.BoardConfig().get('build.div', 'ERROR')   # STM32L0
    env.SUB = env.BoardConfig().get('build.sub', 'ERROR')   # %51xx
    env.MCU = env.BoardConfig().get('build.mcu', 'ERROR')   # %51K8Ux
    env.SUB = env.SUB.replace("%", env.DIV)                 # STM32L051xx
    env.MCU = env.MCU.replace("%", env.DIV)                 # STM32L051K8Ux is STM32L051K8U3
    env.COR = env.BoardConfig().get('build.cor', 'ERROR')   # [ "-march=armv6-m", "-mcpu=cortex-m0plus", "-mthumb" ]    
    
    env.optimization = dev_get_value(env, 'optimization', '-Os') # INIDOC
    print('OPTIMIZATION  : %s' % env.optimization)

    linker = dev_get_value(env, 'linker', 'DEFAULT') # INIDOC
    if 'DEFAULT' == linker: linker = join('$PROJECT_DIR', 'stm' , env.MCU + '_FLASH.ld')
    print('LINKER        : %s' % linker )  

    env.Replace( 
        SIZETOOL = 'arm-none-eabi-size',
        SIZEPROGREGEXP = r"^(?:\.boot2|\.text|\.data|\.rodata|\.text.align|\.ARM.exidx)\s+(\d+).*",
        SIZEDATAREGEXP = r'^(?:\.data|\.bss|\.ram_vector_table)\s+(\d+).*',
        SIZECHECKCMD = '$SIZETOOL -A -d $SOURCES',
        #SIZEPRINTCMD = '$SIZETOOL --mcu=arm -C -d $SOURCES', # TODO check?
        PROGSUFFIX = '.elf',
        PROGNAME = env.GetProjectOption('custom_name', application_name) # INIDOC 
    )  

    env.Append( 
        ASFLAGS=[ env.COR, '-x', 'assembler-with-cpp' ],
        CPPDEFINES = [
            'USE_HAL_DRIVER',
            env.SUB, # STM32L051xx
        ],
        CPPPATH = [
            join("$PROJECT_DIR", "src"),
            join("$PROJECT_DIR", "lib"),
            join("$PROJECT_DIR", "include"),
            join("$PROJECT_DIR", "stm"),

            join("$FRAMEWORK_DIR", "Drivers", env.DIV + "xx_HAL_Driver", "Inc"),
            join("$FRAMEWORK_DIR", "Drivers", env.DIV + "xx_HAL_Driver", "Inc", "Legacy"),
            join("$FRAMEWORK_DIR", "Drivers", "CMSIS", "Device", "ST", env.DIV + "xx", "Include"),
            join("$FRAMEWORK_DIR", "Drivers", "CMSIS", "Include"),

        ],
        #CFLAGS = [],
        CCFLAGS = [ env.COR, env.optimization,
            "-fdata-sections",
            "-ffunction-sections",
            "-Wall",
            "-Wextra",
            "-Wfatal-errors",
            "-Wno-unused-parameter",
            "-Wno-unused-function",
            "-Wno-unused-variable",
            "-Wno-unused-value",
        ],
        CXXFLAGS = [
            '-fno-rtti',
            '-fno-exceptions',
            '-fno-threadsafe-statics',
            '-fno-non-call-exceptions',
            '-fno-use-cxa-atexit',
        ],
        LIBSOURCE_DIRS = [ 
            join('$PROJECT_DIR', 'lib'), 
        ],
        LIBPATH = [ 
            join('$PROJECT_DIR', 'lib'), 
        ],
        LIBS = [
            'm', 'c', '-lnosys',
        ], 
        LINKFLAGS = [ env.COR, env.optimization,
            '-nostartfiles',        
            '-specs=nano.specs',
            '-Wl,-Map=%s.map' % env.subst(join('$BUILD_DIR','$PROGNAME')),
            '-Wl,--gc-sections',
            '--entry=Reset_Handler',
        ],    
        LDSCRIPT_PATH = linker, 
        UPLOADCMD = dev_uploader,
        BUILDERS = dict(
            ELF2HEX = Builder(
                action = env.VerboseAction(' '.join([ '$OBJCOPY', '-O',  'ihex', '$SOURCES', '$TARGET', ]), 'Building HEX $TARGET'), 
                suffix = '.hex'
            ),            
            ELF2BIN = Builder(
                action = env.VerboseAction(' '.join([ '$OBJCOPY', '-O',  'binary', '-S', '$SOURCES', '$TARGET', ]), 'Building BIN $TARGET'), 
                suffix = '.bin'
            ),
        ),
    )

    env.BuildSources(
        join('$BUILD_DIR', env.DIV + "xx_HAL_Driver", "Src"),
        join("$FRAMEWORK_DIR", "Drivers", env.DIV + "xx_HAL_Driver", "Src"),
        '-<*> +<*_hal*>' # remove LL drivers
    )
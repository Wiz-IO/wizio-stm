'''
Copyright 2022 (c) 2022 WizIO ( Georgi Angelov )

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

from os.path import join, dirname, normpath, exists
from SCons.Script import (AlwaysBuild, Builder, COMMAND_LINE_TARGETS, Default, DefaultEnvironment)
from colorama import Fore

PLATFORM_NAME  = 'wizio-stm'
FRAMEWORK_NAME = 'framework-' + PLATFORM_NAME

env = DefaultEnvironment()

def cubemx_config(*args, **kwargs): # TODO
   print("TODO !!!")
   if exists( env.subst( join("$PROJECT_DIR", "Inc") ) ):
      print("Inc exists")
      # MOVE ALL TO INCLUDE & DELETE
   if exists( env.subst( join("$PROJECT_DIR", "Src") ) ): # case sensitive !?!
      print("Src exists")      
   exit(0)
env.AddCustomTarget(
    "config_from_cubemx", None, cubemx_config,
    title="Config Project from CubeMX",
    description="blah",
)

print( '\n<<< STM32 EXPERIMENTAL PLATFORM 2023 Georgi Angelov >>>\n' )
env['PLATFORM_DIR' ] = env.platform_dir  = dirname( env['PLATFORM_MANIFEST'] )
env['FRAMEWORK_DIR'] = env.framework_dir = env.PioPlatform().get_package_dir( FRAMEWORK_NAME )
env.Replace( 
    BUILD_DIR = env.subst('$BUILD_DIR'),
    ARFLAGS = ['rc'],        
    AR = 'arm-none-eabi-ar',
    AS = 'arm-none-eabi-as',
    CC = 'arm-none-eabi-gcc',
    GDB = 'arm-none-eabi-gdb',
    CXX = 'arm-none-eabi-g++',
    OBJCOPY = 'arm-none-eabi-objcopy',
    RANLIB = 'arm-none-eabi-ranlib',
    #SIZETOOL = 'arm-none-eabi-size',
) 

prg = None
if 'WizIO-STM32-SDK' in env['PIOFRAMEWORK']: 
   elf = env.BuildProgram()

# DEBUG ####################################################################### TODO
debug_tool = env.GetProjectOption('debug_tool')
if None == debug_tool:
    Default( prg )
else:   
    Default( prg )

# UPLOAD ###################################################################### TODO
upload = env.Alias('upload', prg, env.VerboseAction('$UPLOADCMD', ' - Uploading'), ) 
AlwaysBuild( upload )
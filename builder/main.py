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

import sys
from os.path import join, dirname
from SCons.Script import (ARGUMENTS, AlwaysBuild, Default, DefaultEnvironment)

PLATFORM_NAME  = 'wizio-stm'
FRAMEWORK_NAME = 'framework-' + PLATFORM_NAME

env = DefaultEnvironment()
platform = env.PioPlatform()
board = env.BoardConfig()

'''
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
'''

print( '\n<<< STM32 EXPERIMENTAL PLATFORM(IO) 2023 Georgi Angelov >>>\n' )
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
   bin = env.ELF2BIN( join('$BUILD_DIR', '${PROGNAME}'), elf )
   hex = env.ELF2HEX( join('$BUILD_DIR', '${PROGNAME}'), elf )
   prg = env.Alias( 'buildprog', hex)
   AlwaysBuild( bin, hex )

debug_tools     = board.get("debug.tools", {})
upload_protocol = env.GetProjectOption("upload_protocol")
upload_source   = join("$BUILD_DIR", "${PROGNAME}.elf")
upload_actions  = []

# UPLOAD ###################################################################### TODO
if upload_protocol == None or upload_protocol == "stlinkv1": # STM32CubeProgrammer if exists
   upload_actions = [env.VerboseAction("$UPLOADCMD", "Uploading $SOURCE")]
elif upload_protocol == "stlink":
   openocd_args = [ "-d%d" % (2 if int(ARGUMENTS.get("PIOVERBOSE", 0)) else 1) ]
   openocd_args.extend( debug_tools.get(upload_protocol).get("server").get("arguments", []) )
   if env.GetProjectOption("debug_speed", ""):
      openocd_args.extend( ["-c", "adapter speed %s" % env.GetProjectOption("debug_speed")] )
   openocd_args.extend( ["-c", "program {$SOURCE} %s verify reset; shutdown;" % board.get("upload.offset_address", "")] )
   openocd_args = [ f.replace("$PACKAGE_DIR", platform.get_package_dir("tool-openocd") or "") for f in openocd_args ]
   env.Replace(
      UPLOADER = "openocd",
      UPLOADERFLAGS = openocd_args,
      UPLOADCMD = "$UPLOADER $UPLOADERFLAGS")
   upload_actions = [env.VerboseAction("$UPLOADCMD", "Uploading $SOURCE")]
elif upload_protocol.startswith("jlink"):
   print('JLINK TODO')   
else:
   sys.stderr.write("Warning! Unknown upload protocol %s\n" % upload_protocol)
AlwaysBuild( env.Alias("upload", upload_source, upload_actions) )    

# DEBUG ####################################################################### TODO
debug_tool = env.GetProjectOption('debug_tool')
if None == debug_tool:
   Default( bin, hex ) 
else:   
   Default( bin, hex ) 


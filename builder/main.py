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
from os import listdir
from os.path import join, dirname
from SCons.Script import (ARGUMENTS, AlwaysBuild, Default, DefaultEnvironment)
from cube import ImportCube
from tmp import CreateProject

PLATFORM_NAME  = 'wizio-stm'
FRAMEWORK_NAME = 'framework-' + PLATFORM_NAME

env = DefaultEnvironment()
platform = env.PioPlatform()
board = env.BoardConfig()

def ImportCubeMX(*args, **kwargs): ImportCube(env)
if os.name == 'nt':
   env.AddCustomTarget( "import_cubemx", None, ImportCubeMX, title="IMPORT CubeMX", description="Import files from STM32CubeMX Pproject" )

def CreateProjectMX(*args, **kwargs): CreateProject(env)
env.AddCustomTarget( "create_project", None, CreateProjectMX, title="CREATE Project", description="TODO Create simple project" )

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
   AlwaysBuild( hex, bin )

debug_tools     = board.get("debug.tools", {})
upload_protocol = env.GetProjectOption("upload_protocol")
upload_actions  = [env.VerboseAction("$UPLOADCMD", "Uploading $SOURCE")]

# DEBUG ####################################################################### TODO
debug_tool = env.GetProjectOption('debug_tool')
if None == debug_tool:
   Default( hex, bin ) 
else:   
   Default( hex, bin ) 

# UPLOAD ###################################################################### TODO
if upload_protocol == None or upload_protocol == "stlinkv1":
   pass
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
elif upload_protocol.startswith("jlink"):
   print('JLINK TODO')   
else:
   sys.stderr.write("Warning! Unknown upload protocol %s\n" % upload_protocol)

AlwaysBuild( env.Alias("upload", prg, upload_actions) )    

# print(env.Dump())
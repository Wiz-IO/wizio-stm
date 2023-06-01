# Copyright 2023 WizIO ( Georgi Angelov )

from os.path import join, exists
from shutil import copyfile
from modules import PRINT_MODULE_INFO

def init(env, params=''):
    OBJ_DIR = join( '$BUILD_DIR', 'modules', 'freertos' )
    SRC_DIR = join( env.framework_dir, 'libraries', 'freertos')

    PRJ_DIR = join( env.subst('$PROJECT_DIR'), 'include' )
    if not exists( join(PRJ_DIR, 'FreeRTOSConfig.h') ): 
        copyfile(
            join(SRC_DIR, 'FreeRTOSConfig'),
            join(PRJ_DIR, 'FreeRTOSConfig.h')
        )

    env.Append(
        CPPDEFINES = [ 'FREERTOS' ],
        CPPPATH    = [
            join(SRC_DIR, 'include' ),
            join(SRC_DIR, 'ARM_CM0' ) if 'ARM_CM0'  in params else '' ,
            join(SRC_DIR, 'ARM_CM3' ) if 'ARM_CM3'  in params else '' ,
            join(SRC_DIR, 'ARM_CM4F') if 'ARM_CM4F' in params else '' ,
        ],
    )

    filter = [
        '-<*>',
        '+<src>',
        '+<portable/MemMang/heap_4.c>',
        '+<ARM_CM0>'  if 'ARM_CM0'  in params else '' ,
        '+<ARM_CM3>'  if 'ARM_CM3'  in params else '' ,
        '+<ARM_CM4F>' if 'ARM_CM4F' in params else '' ,
    ]

    if 'LIB' in params.upper():
        env.Append( LIBS = env.BuildLibrary( OBJ_DIR, SRC_DIR, src_filter = filter ) )
        PRINT_MODULE_INFO('FREERTOS (lib)')
    else:
        env.BuildSources( OBJ_DIR, SRC_DIR, src_filter = filter ) 
        PRINT_MODULE_INFO('FREERTOS (src)')

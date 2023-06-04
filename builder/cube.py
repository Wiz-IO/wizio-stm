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
from os.path import join, exists, normpath, dirname, basename
from shutil import copyfile
from platformio import proc, fs
from colorama import Fore

PYTHON = proc.get_pythonexe_path()

def ERROR(s):
    print(Fore.RED + "[ERROR]", s)
    exit(0) 

def CheckPath( LIST ):
    for p in LIST:
        if not exists(p): 
            ERROR( 'Path Not Exists: %s' % p)

def install_python_requirements():
    try:
        import win32gui, win32com
    except ImportError:
        print(' * Installing Python requirements')
        THIS_DIR = normpath( dirname(__file__) )
        print('install_python_requirements, THIS_DIR = %s' % THIS_DIR)
        args = [ PYTHON, '-m', 'pip', 'install', '-r', 'requirements.txt' ]
        res = proc.exec_command( args, stdout=sys.stdout, stderr=sys.stderr, stdin=sys.stdin, cwd=THIS_DIR )
        if 0 == res['returncode']:
            print(' * Requirements Done')
        else:
            ERROR('(PIP) Please, try later')
        print('install_python_requirements' , res['returncode'])

def ImportCube(env):
    if os.name != 'nt':
        print('WINDOWS ONLY')
        exit(0)
        
    install_python_requirements()
    import win32gui
    from win32com.shell import shell, shellcon

    PProject = env.subst('$PROJECT_DIR')  

    Cube, display_name, image_list = shell.SHBrowseForFolder (
        win32gui.GetDesktopWindow (), 
        shell.SHGetFolderLocation (0, shellcon.CSIDL_DRIVES, 0, 0),
        "Select STM32CubeMX Project", 0, None, None
    )
    if( Cube == None ):
        ERROR("STM32CubeMX project folder not selected")
    try:    
        CProject = shell.SHGetPathFromIDList(Cube).decode('utf-8').replace('\\','/')
    except: 
        ERROR("STM32CubeMX wrong path")

    print()
    print(Fore.GREEN +'[PROJECT] CONFIG:', CProject)
    CheckPath([
        join( PProject, 'include' ),
        join( PProject, 'src' ),
        join( PProject, 'stm' ),    
        join( PProject, 'platformio.ini' ),
        join( CProject, 'Core', 'Inc' ),
        join( CProject, 'Core', 'Src' ),         
    ])  

    # from /Core/Inc to /include
    print(Fore.GREEN +'\tPrepare : include')
    R = fs.match_src_files( CProject, ['-<*>', '+<Core/Inc/>'], ['h'] )
    for r in R:
        if 'STM32' in r.upper(): 
            copyfile( join( CProject, r), join( PProject, 'stm', basename(r)) ) 
        else:     
            copyfile( join( CProject, r), join( PProject, 'include', basename(r)) ) 

    # from /Core/Src to /src
    print(Fore.GREEN +'\tPrepare : src')
    R = fs.match_src_files( CProject, ['-<*>', '+<Core/Src/>'], ['c', 'cpp'] )
    for r in R:
        if 'STM32' in r.upper(): 
            copyfile( join( CProject, r), join( PProject, 'stm', basename(r)) ) 
        else:     
            copyfile( join( CProject, r), join( PProject, 'src', basename(r)) ) 

    # LD to /stm
    print(Fore.GREEN +'\tPrepare : linker sript')
    R = fs.match_src_files( CProject, ['+<*>', '-<Drivers>', '-<Middlewares>'], ['ld'] )
    for r in R:
        if 'STM32' in r.upper(): 
            copyfile( join( CProject, r), join( PProject, 'stm', basename(r)) )     

    # S to /stm
    print(Fore.GREEN +'\tPrepare : asm files')
    R = fs.match_src_files( CProject, ['+<*>', '-<Drivers>', '-<Middlewares>'], ['s'] )
    for r in R:
        if 'STM32' in r.upper(): 
            asm = join( PProject, 'stm', basename(r))
            copyfile( join( CProject, r), asm[:-1] + asm[-1].upper() )             

    print(Fore.GREEN +'[PROJECT] CONFIG DONE\n')
    time.sleep(.1)
    exit(0)
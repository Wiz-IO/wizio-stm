try:
    import win32gui, win32com
except:
    pass


import sys, os, time
from os.path import join, exists, normpath, dirname, basename
from shutil import copyfile
from platformio import proc, fs

PYTHON = proc.get_pythonexe_path()

def ERROR(s):
    print("[ERROR]", s)
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

    from win32com.shell import shell, shellcon
    PProject = env.subst('$PROJECT_DIR')  

    Cube, display_name, image_list = shell.SHBrowseForFolder (
        win32gui.GetDesktopWindow (), 
        shell.SHGetFolderLocation (0, shellcon.CSIDL_PERSONAL, 0, 0), # CSIDL_DRIVES
        "Select STM32CubeMX Project", 0, None, None
    )
    if( Cube == None ):
        ERROR("STM32CubeMX project folder not selected")
    try:    
        CProject = shell.SHGetPathFromIDList(Cube).decode('utf-8').replace('\\','/')
    except: 
        ERROR("STM32CubeMX wrong path")

    print('\nPROJECT CONFIG FOR:', CProject)
    CheckPath([
        join( PProject, 'include' ),
        join( PProject, 'src' ),
        join( PProject, 'stm' ),    
        join( PProject, 'platformio.ini' )
    ])
    CheckPath([
        join( CProject, 'Core', 'Inc' ),
        join( CProject, 'Core', 'Src' ), 
    ])    

    R = fs.match_src_files( CProject, ['-<*>', '+<Core/Inc/>'], ['h'] )
    for r in R:
        if 'STM32' in r.upper(): 
            copyfile( join( CProject, r), join( PProject, 'stm', basename(r)) ) 
        else:     
            copyfile( join( CProject, r), join( PProject, 'include', basename(r)) ) 

    R = fs.match_src_files( CProject, ['-<*>', '+<Core/Src/>'], ['c', 'cpp'] )
    for r in R:
        if 'STM32' in r.upper(): 
            copyfile( join( CProject, r), join( PProject, 'stm', basename(r)) ) 
        else:     
            copyfile( join( CProject, r), join( PProject, 'src', basename(r)) ) 

    R = fs.match_src_files( CProject, ['+<*>', '-<Drivers>'], ['ld'] )
    for r in R:
        if 'STM32' in r.upper(): 
            copyfile( join( CProject, r), join( PProject, 'stm', basename(r)) )     

    R = fs.match_src_files( CProject, ['+<*>', '-<Drivers>'], ['s'] )
    for r in R:
        if 'STM32' in r.upper(): 
            asm = join( PProject, 'stm', basename(r))
            copyfile( join( CProject, r), asm[:-1] + asm[-1].upper() )             

    print('DONE\n')
    time.sleep(.1)
    exit(0)
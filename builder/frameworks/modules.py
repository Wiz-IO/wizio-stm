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

import os, hashlib
from os.path import join, isabs, exists, isdir, basename
from importlib.machinery import SourceFileLoader

def PRINT_MODULE_INFO(txt):
    print('  * %s' % txt)

def dev_module_load(env, module_path, params=''):
    module_path = env.subst( module_path )
    name = hashlib.md5( module_path.encode() ).hexdigest()
    if name not in env.modules:
        SourceFileLoader(name, module_path).load_module().init( env, params )
        env.modules.append(name)

def dev_init_modules(env):
    env.modules = []
    env['MODULES'] = env.modules_dir = join( env.platform_dir, 'modules')
    lines = env.GetProjectOption('custom_module', None) # INIDOC
    if lines:
        print('PROJECT MODULES')
        for line in lines.split('\n'):
            if line == '':
                continue

            line = line.strip().replace('\r', '').replace('\t', '')
            delim = '='
            params = ''
            if delim in line:
                params = line[ line.index( delim ) + 1 : ].strip()
                params = ' '.join( params.split() )
                line = line.partition( delim )[0].strip()
            module_path = env.subst( line ).strip()

            if False == isabs( module_path ):
                module_path = env.subst( join( '$MODULES', env['PIOFRAMEWORK'][0], module_path ) )

            if False == exists( module_path ):
                print('[ERROR] Module File not found: %s' % module_path)
                exit(0)

            if True == isdir( module_path ):
                for root, dirs, files in os.walk( module_path ):
                    files = [ f for f in files if f.endswith('.py') ]
                    for file in files:
                        if not basename( file ).startswith('md-'):
                            continue
                        dev_module_load(env, join(root, file), params)
            else:
                if not basename( module_path ).startswith('md-'):
                    print('[ERROR] Unknown Module File: <%s> Must begin with "md-"' % basename(module_path))
                    exit(0)
                dev_module_load(env, module_path, params)

###############################################################################

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

# https://github.com/STMicroelectronics/stm32l0xx_hal_driver/tree/master

from os.path import join
from SCons.Script import DefaultEnvironment
from common import dev_init_compiler, dev_create_template
from modules import dev_init_modules, dev_add_source_folder

env = DefaultEnvironment()

def template(env):
    dir = join( env.subst('$PROJECT_DIR'), 'src' )
    open( join(dir, 'main.c'), 'w').write('''
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

int main(void)
{
    // setup
    while (1)
    {
        // loop
    }
}        
        ''')

dev_init_compiler(env)

dev_create_template(env, template)

env.BuildSources( join('$BUILD_DIR', 'stm'), join("$PROJECT_DIR", "stm") )

dev_add_source_folder(env)

dev_init_modules(env)
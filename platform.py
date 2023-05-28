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

from os.path import join, dirname, exists
from importlib.machinery import SourceFileLoader
from platformio.managers.platform import PlatformBase

FRAMEWORK_NAME = 'framework-wizio-stm'

class WiziostmPlatform(PlatformBase):

    def is_embedded(self):
        return True

    def get_boards(self, id_=None):
        board = PlatformBase.get_boards(self, id_)
        # TODO: Debuger
        return board

    def get_package_type(self, name):
        Type = self.packages[name].get('type')
        if 'framework' == Type:
            self.install(1)
        return Type

    def on_installed(self):
        return
        self.install(0)

    def install(self, mode):
        return
        filepath = join( dirname( __file__ ), 'builder', 'frameworks', 'install.py' )
        if exists( filepath ):
            SourceFileLoader('module_' +  str(abs(hash(filepath))), filepath).load_module().dev_install(
                join(self.config.get('platformio', 'core_dir'), 'packages', FRAMEWORK_NAME),
                self.packages[FRAMEWORK_NAME].get('url-sdk'),
                mode 
            )

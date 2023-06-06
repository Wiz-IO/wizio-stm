import sys, os, time
from os.path import join, exists, normpath, dirname, basename

def get_gcc(env):
    pp = env['ENV']['PATH'].split(';')
    for p in pp:
        if 'toolchain-gccarmnoneeabi' in p:
            return p.replace('\\', '/')
    return '# EDIT GCC PATH'

def MakeFile(env):
    print('TODO - NOT READY YET !!!')
    f = open( join( env.subst('$PROJECT_DIR'), 'Makefile' ), 'w' )
    f.write('# TODO Makefile ( from PlatformIO )\n')
    f.write('''
#######################################
# binaries
#######################################
GCC_PATH = "%s"
PREFIX = arm-none-eabi-
CC = $(GCC_PATH)/$(PREFIX)gcc
AS = $(GCC_PATH)/$(PREFIX)gcc -x assembler-with-cpp
CP = $(GCC_PATH)/$(PREFIX)objcopy
SZ = $(GCC_PATH)/$(PREFIX)size
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S
''' % get_gcc(env))
    f.write('TARGET = %s\n' % env['PIOENV']) ### ?
    f.write('DEBUG = 1\n')
    f.write('OPT = %s\n' % env.optimization)
    f.write('BUILD_DIR = build\n')
    f.write('CPU = ')
    for p in env.CORTEX: f.write(p); f.write(' ')
    f.write('\n' )
    f.write('''
######################################
# DEFINE
######################################
C_DEFS =  \\
''')
    for p in env['CPPDEFINES']:  
        if type(p) != str: continue
        if p.startswith('-O'): continue
        f.write('-D') 
        f.write(p) 
        f.write(' \\\n')   
    for p in env['CCFLAGS']:  
        if type(p) != str: continue
        if p.startswith('-O'): continue
        f.write(p) 
        f.write(' \\\n')
    f.write('\n')

    f.write('''
######################################
# INCLUDE
######################################
AS_INCLUDES = \n
C_INCLUDES =  \\
''')
    for p in env['CPPPATH']: 
        if p != "": 
            f.write('-I"' + env.subst(p) + '" \\\n'); 
    f.write('\n')


    f.write('''
######################################
# SOURCE
######################################
C_SOURCES = # TODO
''')
    # pfuuu ... there is not list
    #for p in env['PIOBUILDFILES']: f.write(str(p)); f.write('\n') # ... is O files
    f.write('\n')

    f.write('ASM_SOURCES = # TODO')
    f.write('\n')

    f.write('''
#######################################
# LDFLAGS
#######################################
LDSCRIPT = %s
''' % env.subst(env['LDSCRIPT_PATH']))



    f.write('\n')


    f.write('''
######################################################################################

# default action: build all
all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin

# list of objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
# list of ASM program objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR) 
	$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	$(AS) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SZ) $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(HEX) $< $@
	
$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(BIN) $< $@	
	
$(BUILD_DIR):
	mkdir $@		

#######################################
# clean up
#######################################
clean:
	-rm -fR $(BUILD_DIR)
  
#######################################
# dependencies
#######################################
-include $(wildcard $(BUILD_DIR)/*.d)

# *** EOF ***    
''')


    f.close()

    print('DONE')

    # print(env.Dump())
    print( env['File'] )
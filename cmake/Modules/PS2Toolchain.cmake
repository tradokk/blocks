# CMake toolchain file for PS2 EE processor
# Copyright (C) 2017 David Leiter
# based on the toolchain from Mathias Lafeldt <misfire@debugon.org>

set(PS2DEV "$ENV{PS2DEV}")
if (NOT PS2DEV)
    MESSAGE(FATAL_ERROR "PS2DEV env var not set.")
endif (NOT PS2DEV)

set(CMAKE_SYSTEM_NAME "Generic")
set(CMAKE_SYSTEM_VERSION 1)
set(CMAKE_SYSTEM_PROCESSOR ee)

set(CMAKE_C_COMPILER ${PS2DEV}/ee/bin/ee-gcc)
set(CMAKE_CXX_COMPILER ${PS2DEV}/ee/bin/ee-g++)

set(CMAKE_FIND_ROOT_PATH "${PS2DEV}/ee")
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

set(PS2SDK "${PS2DEV}/ps2sdk")
include_directories("${PS2SDK}/ee/include")
include_directories("${PS2SDK}/common/include")

set(EE_CFLAGS "-D_EE -O2 -G0 -Wall" CACHE STRING "EE C compiler flags" FORCE)
set(EE_CXXFLAGS "${EE_CFLAGS} -fno-exceptions")
set(EE_LDFLAGS "" CACHE STRING "EE linker flags" FORCE)
set(EE_ASFLAGS "-G0" CACHE STRING "EE assembler flags" FORCE)

set(EE_CRT0 "${PS2SDK}/ee/startup/crt0.o" CACHE STRING "EE crt0 file" FORCE)

set_source_files_properties(${EE_CRT0} PROPERTIES EXTERNAL_OBJECT TRUE)

set(EE_LINKFILE "${PS2SDK}/ee/startup/linkfile" CACHE STRING "EE linkfile" FORCE)

set(CMAKE_C_FLAGS_INIT ${EE_CFLAGS})
set(CMAKE_CXX_FLAGS_INIT ${EE_CFLAGS})
set(CMAKE_EXE_LINKER_FLAGS_INIT ${EE_LDFLAGS})

set(CMAKE_STATIC_LIBRARY_PREFIX "lib")
set(CMAKE_STATIC_LIBRARY_SUFFIX ".a")
set(CMAKE_EXECUTABLE_SUFFIX ".elf")

set(CMAKE_FIND_LIBRARY_PREFIXES "lib")
set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")

# build EE_BIN
set(CMAKE_C_LINK_EXECUTABLE
	"<CMAKE_C_COMPILER> <FLAGS> <CMAKE_C_LINK_FLAGS> -L${PS2SDK}/ee/lib <LINK_FLAGS> -mno-crt0 -T${EE_LINKFILE} ${EE_CRT0} <OBJECTS> -o <TARGET> <LINK_LIBRARIES> -lc -lkernel"
)

# build EE_LIB
set(CMAKE_C_CREATE_STATIC_LIBRARY
    "<CMAKE_AR> cru <TARGET> <LINK_FLAGS> <OBJECTS>"
)

set(PLATFORM_IS_PS2 TRUE)

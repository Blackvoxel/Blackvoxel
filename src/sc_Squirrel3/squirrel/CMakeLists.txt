set(SQUIRREL ..)

get_filename_component(OUT ${SQUIRREL}/lib ABSOLUTE)
set(INCZ "-I${SQUIRREL}/include -I. -Iinclude")
set(SQUIRREL_SRC_FILES
                 sqapi.cpp
                 sqbaselib.cpp
                 sqfuncstate.cpp
                 sqdebug.cpp
                 sqlexer.cpp
                 sqobject.cpp
                 sqcompiler.cpp
                 sqstate.cpp
                 sqtable.cpp
                 sqmem.cpp
                 sqvm.cpp
                 sqclass.cpp)

add_library(squirrel STATIC ${SQUIRREL_SRC_FILES})

if(PANDORA)
	set_target_properties(squirrel PROPERTIES COMPILE_FLAGS "${INCZ} -O2 -mcpu=cortex-a8 -mfpu=neon -mfloat-abi=softfp -ftree-vectorize -fno-exceptions -fno-rtti -Wall -fno-strict-aliasing")
else(PANDORA)
	if("${CPU_BIT}" MATCHES "32")
		set_target_properties(squirrel PROPERTIES COMPILE_FLAGS "${INCZ} -O2 -fno-exceptions -fno-rtti -Wall -fno-strict-aliasing")
	elseif("${CPU_BIT}" MATCHES "prof")
		set_target_properties(squirrel PROPERTIES COMPILE_FLAGS "${INCZ} -O2 -pg -fno-exceptions -fno-rtti -pie -gstabs -g3 -Wall -fno-strict-aliasing")
	elseif("${CPU_BIT}" MATCHES "64d")
		set_target_properties(squirrel PROPERTIES COMPILE_FLAGS "${INCZ} -O0 -g3 -m64 -D_SQ64 -fno-exceptions -fno-rtti -Wall -fno-strict-aliasing")
	elseif("${CPU_BIT}" MATCHES "64")
		set_target_properties(squirrel PROPERTIES COMPILE_FLAGS "${INCZ} -O2 -m64 -D_SQ64 -fno-exceptions -fno-rtti -Wall -fno-strict-aliasing")
	endif("${CPU_BIT}" MATCHES "32")
endif(PANDORA)

add_custom_command(TARGET squirrel POST_BUILD COMMAND cp libsquirrel.a ${OUT}/libsquirrel.a COMMENT "Copying CXX static library libsquirrel.a to ${OUT}")

set(SQUIRREL ..)

get_filename_component(OUT ${SQUIRREL}/lib ABSOLUTE)
set(INCZ "-I${SQUIRREL}/include -I. -Iinclude")
set(SQSTDLIB_SRC_FILES
                 sqstdblob.cpp
				 sqstdio.cpp
				 sqstdstream.cpp
				 sqstdmath.cpp
				 sqstdsystem.cpp
				 sqstdstring.cpp
				 sqstdaux.cpp
				 sqstdrex.cpp)

add_library(sqstdlib STATIC ${SQSTDLIB_SRC_FILES})

if(PANDORA)
	set_target_properties(sqstdlib PROPERTIES COMPILE_FLAGS "${INCZ} -O2 -mcpu=cortex-a8 -mfpu=neon -mfloat-abi=softfp -ftree-vectorize -fno-exceptions -fno-rtti -Wall -fno-strict-aliasing")
else(PANDORA)
	if("${CPU_BIT}" MATCHES "32")
		set_target_properties(sqstdlib PROPERTIES COMPILE_FLAGS "${INCZ} -O2 -fno-exceptions -fno-rtti -Wall -fno-strict-aliasing")
	elseif("${CPU_BIT}" MATCHES "prof")
		set_target_properties(sqstdlib PROPERTIES COMPILE_FLAGS "${INCZ} -O2 -pg -fno-exceptions -fno-rtti -pie -gstabs -g3 -Wall -fno-strict-aliasing")
	elseif("${CPU_BIT}" MATCHES "64d")
		set_target_properties(sqstdlib PROPERTIES COMPILE_FLAGS "${INCZ} -O0 -g3 -m64 -D_SQ64 -fno-exceptions -fno-rtti -Wall -fno-strict-aliasing")
	elseif("${CPU_BIT}" MATCHES "64")
		set_target_properties(sqstdlib PROPERTIES COMPILE_FLAGS "${INCZ} -O2 -m64 -D_SQ64 -fno-exceptions -fno-rtti -Wall -fno-strict-aliasing")
	endif("${CPU_BIT}" MATCHES "32")
endif(PANDORA)

add_custom_command(TARGET sqstdlib POST_BUILD COMMAND cp libsqstdlib.a ${OUT}/libsqstdlib.a COMMENT "Copying CXX static library libsqstdlib.a to ${OUT}")

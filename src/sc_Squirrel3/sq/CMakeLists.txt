set(SQUIRREL ..)

get_filename_component(OUT ${SQUIRREL}/lib ABSOLUTE)
set(INCZ "-I${SQUIRREL}/include -I. -Iinclude")
set(LIBZ "-L${SQUIRREL}/lib")
set(LIB "-lsquirrel -lsqstdlib")
set(SQ_SRC_FILES
           sq.cpp)

add_library(sq STATIC ${SQ_SRC_FILES})
set_target_properties(sq PROPERTIES LINK_FLAGS "${LIBZ} ${LIB}")

if(PANDORA)
	set_target_properties(sq PROPERTIES COMPILE_FLAGS "${INCZ} -O2 -mcpu=cortex-a8 -mfpu=neon -mfloat-abi=softfp -ftree-vectorize -fno-exceptions -fno-rtti")
else(PANDORA)
	if("${CPU_BIT}" MATCHES "32")
		set_target_properties(sq PROPERTIES COMPILE_FLAGS "${INCZ} -O2 -s -fno-exceptions -fno-rtti")
	elseif("${CPU_BIT}" MATCHES "prof")
		set_target_properties(sq PROPERTIES COMPILE_FLAGS "${INCZ} -O2 -pg -fno-exceptions -fno-rtti -pie -gstabs -g3")
	elseif("${CPU_BIT}" MATCHES "64d")
		set_target_properties(sq PROPERTIES COMPILE_FLAGS "${INCZ} -O0 -g3 -m64 -fno-exceptions -fno-rtti -D_SQ64")
	elseif("${CPU_BIT}" MATCHES "64")
		set_target_properties(sq PROPERTIES COMPILE_FLAGS "${INCZ} -O2 -s -m64 -fno-exceptions -fno-rtti -D_SQ64")
	endif("${CPU_BIT}" MATCHES "32")
endif(PANDORA)

add_custom_command(TARGET sq POST_BUILD COMMAND cp libsq.a ${OUT}/libsq.a COMMENT "Copying CXX static library libsq.a to ${OUT}")

# The code is open source under the MIT license.
# Copyright 2019-2020, Phillip Keldenich, TU Braunschweig, Algorithms Group
# https://ibr.cs.tu-bs.de/alg
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy of
# this software and associated documentation files (the "Software"), to deal in
# the Software without restriction, including without limitation the rights to
# use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
# of the Software, and to permit persons to whom the Software is furnished to do
# so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
include(CheckLanguage)
check_language(CUDA)

add_library(__ivarp_cuda_support INTERFACE)
add_library(ivarp::cuda_support ALIAS __ivarp_cuda_support)

if(CMAKE_CUDA_COMPILER AND NOT IVARP_NO_CUDA)
	enable_language(CUDA)
	set(CMAKE_CUDA_STANDARD 14)
	set(CMAKE_CUDA_STANDARD_REQUIRED ON)
    target_compile_definitions(__ivarp_cuda_support INTERFACE IVARP_CUDA_SUPPORTED=1)
	target_compile_options(__ivarp_cuda_support INTERFACE "$<$<COMPILE_LANGUAGE:CUDA>:--generate-code=arch=compute_50,code=sm_50;--generate-code=arch=compute_61,code=sm_61>")
	target_include_directories(__ivarp_cuda_support INTERFACE "${CMAKE_CURRENT_LIST_DIR}/../include/ivarp/with_cuda")
	target_include_directories(__ivarp_cuda_support INTERFACE ${CMAKE_CUDA_TOOLKIT_INCLUDE_DIRECTORIES})
	find_library(CUDART_LIBRARY cudart ${CMAKE_CUDA_IMPLICIT_LINK_DIRECTORIES})
	if(CUDART_LIBRARY)
		target_link_libraries(__ivarp_cuda_support INTERFACE ${CUDART_LIBRARY})
	endif()
else()
	message(STATUS "Could not find CUDA - building without CUDA support.")
endif()

function(util_make_flags_cuda_compatible EXISTING_TARGET)
    if(CMAKE_CUDA_COMPILER)
        get_property(iface_flags TARGET ${EXISTING_TARGET} PROPERTY INTERFACE_COMPILE_OPTIONS)
        if(NOT "${iface_flags}" STREQUAL "")
            string(REPLACE ";" "," cuda_wrapped_flags "${iface_flags}")
            set_property(TARGET ${EXISTING_TARGET} PROPERTY INTERFACE_COMPILE_OPTIONS
                    "$<$<BUILD_INTERFACE:$<COMPILE_LANGUAGE:CXX>>:${iface_flags}>$<$<BUILD_INTERFACE:$<COMPILE_LANGUAGE:CUDA>>:-Xcompiler=${cuda_wrapped_flags}>"
                    )
        endif()
    endif()
endfunction()

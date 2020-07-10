# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.


#add_library(relocate    STATIC relocate.c)
#add_library( # Sets the name of the library.
#             InlineHook
#
#             # Sets the library as a shared library.
#        STATIC
#
#             # Provides a relative path to your source file(s).
#        InlineHook.c)
#set(CMAKE_CXX_STANDARD 11)
#set(CMAKE_CXX_FLAGS "-std=c++11 -g")
#
set(LIB_SRC  HookerX86.cpp HookerX86.h hooker.cpp hooker.h)
add_library(inlinehook STATIC ${LIB_SRC})
add_library(dexdump SHARED dexdump.cpp)
target_link_libraries(dexdump inlinehook)
find_library(log-lib log)
target_link_libraries(dexdump ${log-lib})
target_link_libraries(inlinehook ${log-lib})



#add_library(dexdump SHARED dexdump.cpp)
#add_library(hooker STATIC  hooker.cpp)
#add_library(HookerX86 STATIC HookerX86.cpp)
#target_link_libraries(dexdump HookerX86)
#target_link_libraries(HookerX86 hooker)
#find_library(log-lib log)
#target_link_libraries(dexdump ${log-lib})
#target_link_libraries(HookerX86 ${log-lib})
#target_link_libraries(hooker ${log-lib})



# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

#find_library( # Sets the name of the path variable.
#        log-lib
#
#        # Specifies the name of the NDK library that
#        # you want CMake to locate.
#        log )

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

#target_link_libraries( # Specifies the target library.
#        dexdump
#
#                       # Links the target library to the log library
#                       # included in the NDK.
#                       ${log-lib} )
#target_link_libraries(InlineHook ${log-lib})
#target_link_libraries(hooker $(log-lib))
#target_link_libraries(dexdump hooker)
#target_link_libraries(dexdump InlineHook)
#target_link_libraries(InlineHook    relocate)



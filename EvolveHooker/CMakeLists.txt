set(SOURCES
  Dll1/wintrust.cpp
  Dll1/logging/easylogging++.cc
  Dll1/simpleini/ConvertUTF.c
  Dll1/Dll1.rc
)

add_library(BINTRUST SHARED ${SOURCES})

target_include_directories(BINTRUST PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/Dll1
  ${CMAKE_CURRENT_SOURCE_DIR}/Dll1/logging
  ${CMAKE_CURRENT_SOURCE_DIR}/Dll1/simpleini
  ${CMAKE_SOURCE_DIR}/server_core/include
)

if(TARGET minhook)
  get_target_property(_minhook_include_dirs minhook INTERFACE_INCLUDE_DIRECTORIES)
  if(_minhook_include_dirs)
    target_include_directories(BINTRUST PRIVATE ${_minhook_include_dirs})
  endif()
endif()

target_link_libraries(BINTRUST PRIVATE
  minhook
  ws2_32 winhttp kernel32 advapi32 imm32 user32 shlwapi winpthread
)

target_compile_definitions(BINTRUST PRIVATE
  NDEBUG
  _WINDOWS
  _CRT_SECURE_NO_WARNINGS
  _WINSOCK_DEPRECATED_NO_WARNINGS
  UNICODE
  _UNICODE
)

if(NOT DEFINED EVOLVE_BINTRUST_INTERNAL_SERVER)
  set(EVOLVE_BINTRUST_INTERNAL_SERVER ON)
endif()

if(EVOLVE_BINTRUST_INTERNAL_SERVER)
  target_link_libraries(BINTRUST PRIVATE evolve_server_core)
  target_compile_definitions(BINTRUST PRIVATE EVOLVE_INTERNAL_SERVER)
endif()

set_target_properties(BINTRUST PROPERTIES
  OUTPUT_NAME BINTRUST
  PREFIX ""
  SUFFIX ".dll"
)

if(MINGW)
  target_link_options(BINTRUST PRIVATE -static-libgcc -static-libstdc++)
endif()

cmake_minimum_required(VERSION 3.21)

project(EvolveLegacyReborn LANGUAGES C CXX)

option(EVOLVE_BUILD_TESTING "Build tests" ON)
option(EVOLVE_BUILD_HARNESS "Build the standalone server harness" OFF)
option(EVOLVE_BUILD_BINTRUST "Build BINTRUST.dll (Windows hook DLL)" OFF)
option(EVOLVE_BINTRUST_INTERNAL_SERVER "Enable embedded CivetWeb HTTPS server in BINTRUST.dll" OFF)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
  add_compile_options(/W4 /permissive-)
else()
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

include(FetchContent)

# ---- MbedTLS ----
set(ENABLE_PROGRAMS OFF CACHE BOOL "" FORCE)
set(ENABLE_TESTING OFF CACHE BOOL "" FORCE)
FetchContent_Declare(mbedtls
  GIT_REPOSITORY https://github.com/Mbed-TLS/mbedtls.git
  GIT_TAG v3.6.6
)

# ---- CivetWeb ----
set(CIVETWEB_BUILD_TESTING OFF CACHE BOOL "" FORCE)
FetchContent_Declare(civetweb
  GIT_REPOSITORY https://github.com/civetweb/civetweb.git
  GIT_TAG v1.16
)

# ---- MinHook ----
FetchContent_Declare(minhook
  GIT_REPOSITORY https://github.com/TsudaKageyu/minhook.git
  GIT_TAG v1.3.3
)

# Determine whether server runtime dependencies are needed
set(EVOLVE_NEED_SERVER_DEPS OFF)
if(EVOLVE_BUILD_HARNESS OR EVOLVE_BINTRUST_INTERNAL_SERVER)
  set(EVOLVE_NEED_SERVER_DEPS ON)
endif()

if(EVOLVE_NEED_SERVER_DEPS)
  FetchContent_MakeAvailable(mbedtls)
  if(NOT TARGET MbedTLS::mbedtls)
    add_library(MbedTLS::mbedtls ALIAS mbedtls)
    add_library(MbedTLS::mbedcrypto ALIAS mbedcrypto)
    add_library(MbedTLS::mbedx509 ALIAS mbedx509)
  endif()

  FetchContent_GetProperties(civetweb)
  if(NOT civetweb_POPULATED)
    FetchContent_Populate(civetweb)
  endif()

  add_library(civetweb_obj OBJECT
    ${civetweb_SOURCE_DIR}/src/civetweb.c
  )
  target_include_directories(civetweb_obj PUBLIC ${civetweb_SOURCE_DIR}/include)
  if(WIN32)
    target_compile_definitions(civetweb_obj PRIVATE _CRT_SECURE_NO_WARNINGS)
  endif()
  target_compile_definitions(civetweb_obj PRIVATE USE_MBEDTLS NOT_OPENSSL)
  target_link_libraries(civetweb_obj PUBLIC mbedtls mbedcrypto mbedx509)
  if(MINGW)
    target_link_libraries(civetweb_obj PUBLIC winpthread)
  endif()
  add_library(civetweb::civetweb ALIAS civetweb_obj)
endif()

if(EVOLVE_BUILD_BINTRUST)
  FetchContent_Populate(minhook)

  add_library(minhook STATIC
    ${minhook_SOURCE_DIR}/src/hook.c
    ${minhook_SOURCE_DIR}/src/trampoline.c
    ${minhook_SOURCE_DIR}/src/buffer.c
    ${minhook_SOURCE_DIR}/src/hde/hde32.c
    ${minhook_SOURCE_DIR}/src/hde/hde64.c
  )
  target_include_directories(minhook PUBLIC ${minhook_SOURCE_DIR}/include)
  target_include_directories(minhook PRIVATE ${minhook_SOURCE_DIR}/src)
endif()

include(CTest)
add_subdirectory(server_core)

if(EVOLVE_BUILD_BINTRUST AND WIN32)
  add_subdirectory(EvolveHooker)
endif()

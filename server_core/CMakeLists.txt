add_library(evolve_server_logic STATIC
  src/server_logic.cpp
  src/resource_store.cpp
)

target_include_directories(evolve_server_logic
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

if(EVOLVE_BUILD_TESTING)
  add_executable(server_logic_tests
    tests/server_logic_tests.cpp
    tests/resource_store_tests.cpp
  )

  target_link_libraries(server_logic_tests
    PRIVATE
      evolve_server_logic
  )

  add_test(NAME server_logic_tests COMMAND $<TARGET_FILE:server_logic_tests>)
endif()

if(EVOLVE_BUILD_RUNTIME)
  find_package(civetweb CONFIG REQUIRED)
  find_package(OpenSSL REQUIRED)

  add_library(evolve_server_core STATIC
    src/server_core.cpp
  )

  target_include_directories(evolve_server_core
    PUBLIC
      ${CMAKE_CURRENT_SOURCE_DIR}/include
  )

  target_link_libraries(evolve_server_core
    PUBLIC
      evolve_server_logic
      civetweb::civetweb
      OpenSSL::SSL
      OpenSSL::Crypto
  )

  if(EVOLVE_BUILD_HARNESS)
    add_executable(server_harness
      ../server_harness/main.cpp
    )

    target_link_libraries(server_harness
      PRIVATE
        evolve_server_core
    )
  endif()
endif()

include(GoogleTest)
option(INSTALL_GTEST OFF)
option(BUILD_GMOCK OFF)

add_subdirectory("extern/googletest")
add_subdirectory(units)
add_subdirectory(python)

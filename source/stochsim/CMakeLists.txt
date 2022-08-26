set(sources
  factory.cpp
  problem.cpp
  reaction.cpp
  reaction_parser.cpp
  simulation.cpp
  )

add_library(stochsim STATIC ${sources})

add_subdirectory(diagnostics)
add_subdirectory(solvers)
add_subdirectory(utils)

target_compile_features(stochsim PUBLIC cxx_std_20)
target_include_directories(stochsim PUBLIC ${stochsim_includes})

target_compile_options(stochsim PUBLIC -g -O3 -Wall -Wextra)

target_link_libraries(stochsim PUBLIC nlohmann_json::nlohmann_json)

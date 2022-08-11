set(sources
  factory.cpp
  reaction.cpp
  reaction_parser.cpp
  ssa.cpp
  stochastic_simulation_method.cpp
  tau_leaping.cpp
  )

add_library(ssm SHARED ${sources})

add_subdirectory(utils)

target_compile_features(ssm PUBLIC cxx_std_20)
target_include_directories(ssm PUBLIC ${ssm_includes})

target_compile_options(ssm PUBLIC -g -O3 -Wall -Wextra)

target_link_libraries(ssm PUBLIC nlohmann_json::nlohmann_json)

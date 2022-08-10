set(sources
  reaction.cpp
  ssa.cpp
  stochastic_simulation_method.cpp
  tau_leaping.cpp
  )

add_library(ssm SHARED ${sources})


target_compile_features(ssm PUBLIC cxx_std_20)
target_include_directories(ssm PUBLIC ${ssm_includes})

target_compile_options(ssm PUBLIC -g -O3 -Wall -Wextra)

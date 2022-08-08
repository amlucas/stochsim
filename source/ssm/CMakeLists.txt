set(sources
  reaction.cpp
  )

add_library(ssm SHARED ${sources})


target_compile_features(ssm PUBLIC cxx_std_20)
target_include_directories(ssm PUBLIC ${ssm_includes})

target_compile_options(ssm PUBLIC -g -O3 -Wall -Wextra)

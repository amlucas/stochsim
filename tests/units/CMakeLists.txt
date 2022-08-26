add_executable(test_stochsim
  main.cpp
  diagnostics.cpp
  exceptions.cpp
  factory.cpp
  R_leaping.cpp
  reaction.cpp
  reaction_parser.cpp
  ssa.cpp
  tau_leaping.cpp
  utils.cpp)

target_link_libraries(test_stochsim gtest stochsim)
add_test(NAME test_stochsim COMMAND test_stochsim)

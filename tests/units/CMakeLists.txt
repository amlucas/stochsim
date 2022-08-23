add_executable(test_ssm
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

target_link_libraries(test_ssm gtest ssm)
add_test(NAME test_ssm COMMAND test_ssm)

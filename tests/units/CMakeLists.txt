add_executable(test_ssm
  main.cpp
  reaction.cpp)

target_link_libraries(test_ssm gtest ssm)
add_test(NAME test_ssm COMMAND test_ssm)

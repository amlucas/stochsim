target_sources(stochsim PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/R0_leaping.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/R1_leaping.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/R_leaping.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/solver.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ssa.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/tau_leaping.cpp
  )

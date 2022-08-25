set(binding_sources
  bindings.cpp
  problem.cpp
  run.cpp
  solvers.cpp
  )

pybind11_add_module(pystochsim MODULE ${binding_sources})
target_link_libraries(pystochsim PRIVATE ssm)
set_property(TARGET ssm PROPERTY POSITION_INDEPENDENT_CODE ON)

set(binding_sources
  bindings.cpp
  problem.cpp
  solvers.cpp
  )

pybind11_add_module(pyssm MODULE ${binding_sources})
target_link_libraries(pyssm PRIVATE ssm)
set_property(TARGET ssm PROPERTY POSITION_INDEPENDENT_CODE ON)

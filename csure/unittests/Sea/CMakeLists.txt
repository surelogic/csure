set(LLVM_LINK_COMPONENTS
  Support
  )

add_csure_unittest(SeaTests
  DropTest.cpp
  HintDropTest.cpp
  MetricDropTest.cpp
  ProposedPromiseDropTest.cpp
  SeaTest.cpp
  )

target_link_libraries(SeaTests
  csureSea
  csureCommon
  )

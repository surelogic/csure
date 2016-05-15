set(LLVM_LINK_COMPONENTS
  Support
  )

add_csure_unittest(CommonTests
  SLUtilTest.cpp
  )

target_link_libraries(CommonTests
  csureCommon
  )

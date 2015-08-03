add_custom_target(CSureUnitTests)
set_target_properties(CSureUnitTests PROPERTIES FOLDER "CSure tests")

# add_csure_unittest(test_dirname file1.cpp file2.cpp)
#
# Will compile the list of files together and link against csure
# Produces a binary named 'basename(test_dirname)'.
function(add_csure_unittest test_dirname)
  add_unittest(CSureUnitTests ${test_dirname} ${ARGN})
endfunction()

add_subdirectory(Common)
add_subdirectory(Sea)

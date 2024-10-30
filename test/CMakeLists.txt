# Parent project does not export its library target, so this CML implicitly
# depends on being added from it, i.e. the testing is done only from the build
# tree and is not feasible from an install location

project(CassouScriptTests LANGUAGES CXX)

# ---- Tests ----

add_executable(CassouScript_test source/CassouScript_test.cpp)
target_link_libraries(CassouScript_test PRIVATE CassouScript_lib)
target_compile_features(CassouScript_test PRIVATE cxx_std_17)

add_test(NAME CassouScript_test COMMAND CassouScript_test)

# ---- End-of-file commands ----

add_folders(Test)

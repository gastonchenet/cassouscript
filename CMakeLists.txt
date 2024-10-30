cmake_minimum_required(VERSION 3.14)

include(cmake/prelude.cmake)

project(
    CassouScript
    VERSION 0.0.1
    DESCRIPTION "AAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
    HOMEPAGE_URL "https://cspt-maiscenomestdejaprisducoupjallongeunpeu.com/"
    LANGUAGES CXX
)

include(cmake/project-is-top-level.cmake)
include(cmake/variables.cmake)

file(GLOB_RECURSE cassouscript_SRC CONFIGURE_DEPENDS
    "source/*.cpp"
)

list(FILTER cassouscript_SRC EXCLUDE REGEX "source/lib.cpp$")

# ---- Declare library ----

add_library(
    CassouScript_lib OBJECT
    source/lib.cpp
)

target_include_directories(
    CassouScript_lib ${warning_guard}
    PUBLIC
    "\$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/source>"
)

target_compile_features(CassouScript_lib PUBLIC cxx_std_17)

# ---- Declare executable ----

add_executable(CassouScript_exe ${cassouscript_SRC})
add_executable(CassouScript::exe ALIAS CassouScript_exe)

set_property(TARGET CassouScript_exe PROPERTY OUTPUT_NAME CassouScript)

target_compile_features(CassouScript_exe PRIVATE cxx_std_17)

target_link_libraries(CassouScript_exe PRIVATE CassouScript_lib)

# ---- Install rules ----

if(NOT CMAKE_SKIP_INSTALL_RULES)
  include(cmake/install-rules.cmake)
endif()

# ---- Developer mode ----

if(NOT CassouScript_DEVELOPER_MODE)
  return()
elseif(NOT PROJECT_IS_TOP_LEVEL)
  message(
      AUTHOR_WARNING
      "Developer mode is intended for developers of CassouScript"
  )
endif()

include(cmake/dev-mode.cmake)

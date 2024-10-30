install(
    TARGETS CassouScript_exe
    RUNTIME COMPONENT CassouScript_Runtime
)

if(PROJECT_IS_TOP_LEVEL)
  include(CPack)
endif()

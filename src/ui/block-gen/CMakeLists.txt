add_executable(block-gen main
                         ../common/cmd_arguments_base
                         ../common/string_util
                         cmd_arguments)
target_link_libraries(block-gen block igraphpp mtwister ${igraph_LIBRARIES})

install(TARGETS block-gen
	    RUNTIME DESTINATION bin)

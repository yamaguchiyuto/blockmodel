add_executable(block-pred main
                          ../common/cmd_arguments_base
                          ../common/graph_util
                          ../common/string_util
                          cmd_arguments)
target_link_libraries(block-pred block igraphpp mtwister ${igraph_LIBRARIES})

install(TARGETS block-pred
	    RUNTIME DESTINATION bin)

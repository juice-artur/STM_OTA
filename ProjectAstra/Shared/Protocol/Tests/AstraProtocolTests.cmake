include("${CMAKE_CURRENT_LIST_DIR}/../../../ThirdParty/utest.cmake")

add_executable(RunProtocolTests
	"${CMAKE_CURRENT_LIST_DIR}/main_test.c"
	"${CMAKE_CURRENT_LIST_DIR}/Cobs_tests.c"
)

target_link_libraries(RunProtocolTests PRIVATE AstraProtocol utest)

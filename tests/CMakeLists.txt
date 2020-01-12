set(TESTS_DIR ${CMAKE_SOURCE_DIR}/tests)

set(TEST_SOURCES
${TESTS_DIR}/tests.cpp
${TESTS_DIR}/tensor_tests.cpp
${TESTS_DIR}/canvas_tests.cpp
${TESTS_DIR}/matrix_tests.cpp
${TESTS_DIR}/transform_tests.cpp
${TESTS_DIR}/ray_tests.cpp
${TESTS_DIR}/sphere_tests.cpp
${TESTS_DIR}/plane_tests.cpp
${TESTS_DIR}/cube_tests.cpp
${TESTS_DIR}/cylinder_tests.cpp
${TESTS_DIR}/cone_tests.cpp
${TESTS_DIR}/triangle_tests.cpp
${TESTS_DIR}/intersection_tests.cpp
${TESTS_DIR}/light_tests.cpp
${TESTS_DIR}/material_tests.cpp
${TESTS_DIR}/world_tests.cpp
${TESTS_DIR}/camera_tests.cpp
${TESTS_DIR}/pattern_tests.cpp
${TESTS_DIR}/group_tests.cpp
${TESTS_DIR}/model_parser_tests.cpp
)

configure_file(${TESTS_DIR}/test_objs/gibberish.obj gibberish.obj COPYONLY)
configure_file(${TESTS_DIR}/test_objs/groups.obj groups.obj COPYONLY)
configure_file(${TESTS_DIR}/test_objs/polygon.obj polygon.obj COPYONLY)
configure_file(${TESTS_DIR}/test_objs/triangles.obj triangles.obj COPYONLY)
configure_file(${TESTS_DIR}/test_objs/vertices.obj vertices.obj COPYONLY)
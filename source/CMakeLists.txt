set(CORE_DIR ${CMAKE_SOURCE_DIR}/source/lightspace)

set(CORE_SOURCES
${CORE_DIR}/public/common.hpp
${CORE_DIR}/public/tensor.hpp
${CORE_DIR}/private/tensor.cpp
${CORE_DIR}/public/canvas.hpp
${CORE_DIR}/private/canvas.cpp
${CORE_DIR}/public/matrix.hpp
${CORE_DIR}/private/matrix.cpp
${CORE_DIR}/public/transform.hpp
${CORE_DIR}/private/transform.cpp
)

set(CORE_INCLUDES
${CORE_DIR}
${CORE_DIR}/public
)
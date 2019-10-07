set(CORE_DIR ${CMAKE_SOURCE_DIR}/source/lightspace)

set(CORE_SOURCES
${CORE_DIR}/public/common.hpp
${CORE_DIR}/public/tensor.hpp
${CORE_DIR}/private/tensor.cpp
${CORE_DIR}/public/canvas.hpp
${CORE_DIR}/private/canvas.cpp
)

set(CORE_INCLUDES
${CORE_DIR}
${CORE_DIR}/public
)
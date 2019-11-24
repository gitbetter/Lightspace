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
${CORE_DIR}/public/ray.hpp
${CORE_DIR}/private/ray.cpp
${CORE_DIR}/public/shapes.hpp
${CORE_DIR}/private/shapes.cpp
${CORE_DIR}/public/intersection.hpp
${CORE_DIR}/private/intersection.cpp
${CORE_DIR}/public/lights.hpp
${CORE_DIR}/private/lights.cpp
${CORE_DIR}/public/materials.hpp
${CORE_DIR}/private/materials.cpp
${CORE_DIR}/public/world.hpp
${CORE_DIR}/private/world.cpp
${CORE_DIR}/public/camera.hpp
${CORE_DIR}/private/camera.cpp
)

set(CORE_INCLUDES
${CORE_DIR}
${CORE_DIR}/public
)
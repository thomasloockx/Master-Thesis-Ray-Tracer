#
# CMake file for the ray-tracer.
#

SET(RENDER_CORE
    rtcore/image.cpp
    rtcore/boundingbox.cpp
    rtcore/triangle.cpp
    rtcore/sahpartitioner.cpp
    rtcore/bvh.cpp
    rtcore/bboxbvh.cpp
    rtcore/hitrecord.cpp
    rtcore/raypacket.cpp
    rtcore/packetraytracer.cpp
    rtcore/color.cpp
    rtcore/partitionalgorithms.cpp
    rtcore/raytracerbase.cpp
    rtcore/twolevelraytracer.cpp
    rtcore/simplebvh.cpp
    rtcore/fuzzybvh.cpp
    rtcore/camera.cpp
    rtcore/ray.cpp
    )

SET(STATIC_RT
    staticrt/rtstatic.cpp
    staticrt/ra2parser.cpp
    )

SET(CHARACTER_ANIMATION
    character_animation/cfgparser.cpp
    character_animation/fuzzy.cpp
    character_animation/main.cpp
    character_animation/cal3dadapter.cpp
    character_animation/testcase.cpp
    character_animation/fastrebuild.cpp
    character_animation/twolevelrefit.cpp
    character_animation/refit.cpp
    character_animation/cluster.cpp
    )    

SET(TESTSUITE
    testsuite/interval_test.cpp
    testsuite/testrunner.cpp
    testsuite/boundingbox_test.cpp
    testsuite/partitionalgorithms_test.cpp
    testsuite/camera_test.cpp
    testsuite/point_test.cpp
    testsuite/matrix4_test.cpp
    testsuite/matrix_test.cpp
    testsuite/vector_test.cpp
    testsuite/triangle_test.cpp
    testsuite/utils_test.cpp
    testsuite/bboxbvh_test.cpp
    testsuite/raypacket_test.cpp
    testsuite/cal3dadapter_test.cpp   
   )

include_directories(
    rtcore
    staticrt
    testsuite
    character_animation
    )

SET(CMAKE_CXX_FLAGS -g)
SET(EXECUTABLE static-rt)
add_executable(${EXECUTABLE} ${RENDER_CORE} ${STATIC_RT})
target_link_libraries(${EXECUTABLE} freeimage)

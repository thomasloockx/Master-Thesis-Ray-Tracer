SRC_FILES = boundingbox.cpp camera.cpp color.cpp hitrecord.cpp image.cpp \
            partitionalgorithms.cpp ray.cpp raytracerbase.cpp \
            packetraytracer.cpp twolevelraytracer.cpp sahpartitioner.cpp simplebvh.cpp \
            triangle.cpp raypacket.cpp bboxbvh.cpp bvh.cpp fuzzybvh.cpp \

OBJ_FILES = $(SRC_FILES:%.cpp=%.o)

all : librtcore.a

librtcore.a : $(OBJ_FILES)
	rm -f $@
	ar cq $@ $(OBJ_FILES) -I../rtcore/

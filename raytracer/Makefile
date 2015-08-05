CXX_FLAGS = -Wall

SRC_DIR = src
TST_DIR = test
GT_DIR = gtest-1.5.0

# Boost library location
BOOST_INC = /usr/include/boost

# Name of test image that is produced from PPM image of test scene.
IMG_NAME = testimg.png

# Name of gtest object file that we need to link with unit tests.
GT_OBJ = gtest-all.o

# Name of test scene description file and image dimensions
TEST_DATA = example.dat
WIDTH = 1280
HEIGHT = 800

# Makes the raytracer binary.
rt: $(SRC_DIR)/driver.o
	$(CXX) $(CXX_FLAGS) $(CPP_FLAGS) $(SRC_DIR)/driver.o -o rt

all: rt unit_tests

# Views the test image with Eye of Gnome.
view: $(IMG_NAME)
	eog $(IMG_NAME)

# Makes test image. Pipes PPM image to PNG converter, outputs to test image.
$(IMG_NAME): rt $(TEST_DATA)
	./rt $(WIDTH) $(HEIGHT) -s < $(TEST_DATA) | pnmtopng > $(IMG_NAME)

# Makes debugging enabled raytracer binary.
drt: $(SRC_DIR)/ddriver.o
	$(CXX) $(CXX_FLAGS) $(CPP_FLAGS) $(SRC_DIR)/ddriver.o -o drt

# This depends on driver.o to force a re-compile when driver.o's deps change.
$(SRC_DIR)/ddriver.o: $(SRC_DIR)/driver.o
	$(CXX) $(CXX_FLAGS) $(CPP_FLAGS) -I$(BOOST_INC) \
	-g -O0 -c $(SRC_DIR)/driver.cc -o $(SRC_DIR)/ddriver.o

$(SRC_DIR)/driver.o: $(SRC_DIR)/driver.cc 
	$(CXX) $(CXX_FLAGS) $(CPP_FLAGS) -I$(BOOST_INC) \
	-c $(SRC_DIR)/driver.cc -o $(SRC_DIR)/driver.o

# Makes the unit tests binary.
unit_tests: $(TST_DIR)/alltests.o $(GT_DIR)/make/$(GT_OBJ)
	$(CXX) $(CXX_FLAGS) $(CPP_FLAGS) $(GT_DIR)/make/$(GT_OBJ) \
	$(TST_DIR)/alltests.o -lpthread -o unit_tests

$(TST_DIR)/alltests.o: $(TST_DIR)/alltests.cc
	$(CXX) $(CXX_FLAGS) $(CPP_FLAGS) -I$(SRC_DIR) -I$(TST_DIR) \
	-I$(GT_DIR)/include -I$(BOOST_INC) -g -O0 -c $(TST_DIR)/alltests.cc \
	-o $(TST_DIR)/alltests.o

# Makes the gtest object file that must be linked to the unit tests.
$(GT_DIR)/make/$(GT_OBJ):
	make -C $(GT_DIR)/make

.PHONY: clean view docs depend

clean:
	rm -rf *~ *.o rt drt unit_tests docs $(IMG_NAME) $(TST_DIR)/*.o \
	$(TST_DIR)/*~ $(SRC_DIR)/*~ $(SRC_DIR)/*.o
	make clean -C $(GT_DIR)/make

docs:
	doxygen

depend:
	makedepend $(CXX_FLAGS) $(CPP_FLAGS) -Y -Isrc -Itest \
	$(SRC_DIR)/driver.cc $(TST_DIR)/alltests.cc

# DO NOT DELETE

src/driver.o: src/sceneobj.hh src/rgbcolor.hh src/mvector.hh src/ray.hh
src/driver.o: src/scene.hh src/light.hh src/spotlight.hh src/arealight.hh
src/driver.o: src/shape.hh src/camera.hh src/infplane.hh src/sphere.hh
src/driver.o: src/cylinder.hh
test/alltests.o: test/test_light.cc src/light.hh src/sceneobj.hh
test/alltests.o: src/rgbcolor.hh src/mvector.hh src/ray.hh test/test_ray.cc
test/alltests.o: test/test_mvector.cc test/test_rgbcolor.cc
test/alltests.o: test/test_infplane.cc src/infplane.hh src/shape.hh
test/alltests.o: test/test_sphere.cc src/sphere.hh test/test_scene.cc
test/alltests.o: src/scene.hh src/spotlight.hh src/arealight.hh src/camera.hh
test/alltests.o: test/test_arealight.cc

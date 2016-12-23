OPENCV_LIBS=-L/Users/fyquah/software/opencv/build/lib -lopencv_calib3d -lopencv_core -lopencv_features2d -lopencv_flann -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lopencv_ml -lopencv_objdetect -lopencv_photo -lopencv_shape -lopencv_stitching -lopencv_superres -lopencv_video -lopencv_videoio -lopencv_videostab
LDFLAGS=${OPENCV_LIBS}
CXXFLAGS=-I/Users/fyquah/software/opencv/include/ -std=c++0x -stdlib=libc++ -g3 -Wall -O0
LINK.o = $(LINK.cc)

watermark: watermark.o

clean:
	find *.o | xargs rm watermark

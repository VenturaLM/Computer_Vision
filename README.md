# Computer_Vision
Introduction to image processing and computer vision.

Intall OpenCV: https://linuxize.com/post/how-to-install-opencv-on-ubuntu-20-04/

- $ sudo apt update
- $ sudo apt install libopencv-dev python3-opencv
- python3 -c "import cv2; print(cv2.__version__)"
- $ sudo apt install build-essential cmake git pkg-config libgtk-3-dev \
    libavcodec-dev libavformat-dev libswscale-dev libv4l-dev \
    libxvidcore-dev libx264-dev libjpeg-dev libpng-dev libtiff-dev \
    gfortran openexr libatlas-base-dev python3-dev python3-numpy \
    libtbb2 libtbb-dev libdc1394-22-dev libopenexr-dev \
    libgstreamer-plugins-base1.0-dev libgstreamer1.0-dev
- $ mkdir ~/opencv_build && cd ~/opencv_build
- $ git clone https://github.com/opencv/opencv.git
- $ git clone https://github.com/opencv/opencv_contrib.git
- $ cd ~/opencv_build/opencv
- $ mkdir -p build && cd build
- $ cmake -D CMAKE_BUILD_TYPE=RELEASE \
    -D CMAKE_INSTALL_PREFIX=/usr/local \
    -D INSTALL_C_EXAMPLES=ON \
    -D INSTALL_PYTHON_EXAMPLES=ON \
    -D OPENCV_GENERATE_PKGCONFIG=ON \
    -D OPENCV_EXTRA_MODULES_PATH=~/opencv_build/opencv_contrib/modules \
    -D BUILD_EXAMPLES=ON ..
- $ make -j8 (Modify the -j flag according to your processor. If you do not know the number of cores your processor, you can find it by typing nproc. The compilation may take several minutes or more, depending on your system configuration.)
- $ sudo make install

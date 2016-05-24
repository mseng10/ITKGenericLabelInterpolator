FROM debian:8
MAINTAINER Insight Software Consortium <community@itk.org>

RUN apt-get update && apt-get install -y \
  build-essential \
  curl \
  cmake \
  git \
  libexpat1-dev \
  libhdf5-dev \
  libjpeg-dev \
  libpng12-dev \
  libpython3-dev \
  libtiff5-dev \
  python \
  ninja-build \
  wget \
  vim \
  zlib1g-dev

RUN mkdir -p /usr/src/GenericLabelInterpolator-build
WORKDIR /usr/src

# v4.10rc02 (2016-05-18)
ENV ITK_GIT_TAG 417d6a36d01aa3b835855349cc481a9b3dca8506
RUN git clone git://itk.org/ITK.git && \
  cd ITK && \
  git checkout ${ITK_GIT_TAG} && \
  cd ../ && \
  mkdir ITK-build && \
  cd ITK-build && \
  cmake \
    -G Ninja \
    -DCMAKE_INSTALL_PREFIX:PATH=/usr \
    -DBUILD_EXAMPLES:BOOL=OFF \
    -DBUILD_TESTING:BOOL=OFF \
    -DBUILD_SHARED_LIBS:BOOL=ON \
    -DCMAKE_POSITION_INDEPENDENT_CODE:BOOL=ON \
    -DITK_LEGACY_REMOVE:BOOL=ON \
    -DITK_WRAP_PYTHON:BOOL=OFF \
    -DITK_LEGACY_SILENT:BOOL=ON \
    -DITK_BUILD_DEFAULT_MODULES:BOOL=OFF \
    -DITK_USE_SYSTEM_LIBRARIES:BOOL=OFF \
    -DModule_ITKImageGrid:BOOL=ON \
    -DModule_ITKSmoothing:BOOL=ON \
    -DModule_ITKTestKernel:BOOL=ON \
    -DModule_ITKImageAdaptors:BOOL=ON \
    ../ITK && \
  ninja && \
  find . -name '*.o' -delete

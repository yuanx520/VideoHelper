CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(videohelper)

INCLUDE_DIRECTORIES(${videohelper_SOURCE_DIR}/include)
SET(ALL_SRC src/avilib.c src/jpgd.cpp)

ADD_LIBRARY(videohelper STATIC ${ALL_SRC})

# Project Name

### st_videohelper

- This is a AVI video reader which can be used in all platforms.
- We only support the avi video type(each frame is encoded by JPEG).

## Installation

- git clone
- mkdir build && cd build
- cmake ..
- make

## Basic Code Example
```
#include <st_videohelper.hpp>
using namespace yy;
static VideoHelperBackend vhelper;
Video* video = vhelper.LoadAVIVideo(video_path_to);

// Each frame has been stored in video->data for you
// Total frame num is video->frame_num

vhelper.FreeVideo(video);
```

## Run Memory Test

- cd samples
- make
- sh ./memory_test.sh

## Contributing

- git checkout dev
- add your new features
- commit your changes: `git commit -am 'Add some feature'`
- push to the branch: `git push origin dev`

## TODO
- using the third-party ffmpeg lib to support all video formats
- test speed in mobiles

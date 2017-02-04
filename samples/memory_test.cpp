#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <st_videohelper.hpp>

using namespace std;
using namespace yy;

static VideoHelperBackend vhelper;
int main(int argc, char *argv[]) {

	const string fn = "./1.avi";
	Video* video = vhelper.LoadAVIVideo(fn.c_str());//load video
	int frame_num = video->frame_num;
	cout << "memory test:"<<endl;
	for (int i=0; i<frame_num; i++){
		cout << "-";
	}
	cout << endl;
	vhelper.FreeVideo(video);
}


#ifndef FRAME_PRODUCER_HPP
#define FRAME_PRODUCER_HPP

#include "opencv2/opencv.hpp"
#include <stdexcept>
#include <vector>

using namespace std;
using namespace cv;

enum LoopKeyCtrl{
	VIDEO,
	SLIDES,
	ONE_FRAME
};

class FrameProducer {
public:
	FrameProducer()
		: update_bcs_settings(false) {
		
	}
	
	virtual Mat getFrame() = 0;
	virtual void seekForward() = 0;
	virtual void seekBackward() = 0;
	
	void settingsChanged() {
		update_bcs_settings = true;
	}
	bool loopKeyCtrl(LoopKeyCtrl ctrl);
protected:
	bool update_bcs_settings;
};

class ImageFrameProducer : public FrameProducer {
public:
	ImageFrameProducer(const string& img_file_name_pattern);
	
	virtual Mat getFrame();
	virtual void seekForward();
	virtual void seekBackward();
protected:
	vector<string> img_file_names;
	int img_idx;
};

class CameraFrameProducer : public FrameProducer {
public:
	CameraFrameProducer(int index = 0) {
		cap.open(index);
		if(!cap.isOpened()){
			throw runtime_error("Cannot open camera!");
		}
	}
	
	virtual Mat getFrame();
	virtual void seekForward(){}
	virtual void seekBackward(){}
protected:
	VideoCapture cap;
};



#endif // FRAME_PRODUCER_HPP

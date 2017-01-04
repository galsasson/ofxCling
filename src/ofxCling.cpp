//
//  ofxCling.cpp
//  ClingOFPlayground
//
//  Created by Gal Sasson on 1/4/17.
//
//

#include "ofxCling.h"

cling::Interpreter* ofxCling::createInterpreter(const string& ofPath, const string& ofxClingPath, const string& sysrootPath)
{
	vector<string> args;
	args.push_back("cling");
	args.push_back("-nobuiltininc");
	args.push_back("-isysroot"+sysrootPath);
	args.push_back("-I"+ofxClingPath+"/include");
	args.push_back("-I"+ofxClingPath+"/lib/clang/3.9.0/include");

	// add OF include paths
	args.push_back("-I"+ofPath+"/libs/openFrameworks");
	args.push_back("-I"+ofPath+"/libs/openFrameworks/3d");
	args.push_back("-I"+ofPath+"/libs/openFrameworks/communication");
	args.push_back("-I"+ofPath+"/libs/openFrameworks/gl");
	args.push_back("-I"+ofPath+"/libs/openFrameworks/math");
	args.push_back("-I"+ofPath+"/libs/openFrameworks/sound");
	args.push_back("-I"+ofPath+"/libs/openFrameworks/utils");
	args.push_back("-I"+ofPath+"/libs/openFrameworks/app");
	args.push_back("-I"+ofPath+"/libs/openFrameworks/events");
	args.push_back("-I"+ofPath+"/libs/openFrameworks/graphics");
	args.push_back("-I"+ofPath+"/libs/openFrameworks/types");
	args.push_back("-I"+ofPath+"/libs/openFrameworks/video");
	args.push_back("-I"+ofPath+"/libs/FreeImage/include");
	args.push_back("-I"+ofPath+"/libs/freetype/include");
	args.push_back("-I"+ofPath+"/libs/rtaudio/include");
	args.push_back("-I"+ofPath+"/libs/boost/include");
	args.push_back("-I"+ofPath+"/libs/tess2/include");
	args.push_back("-I"+ofPath+"/libs/cairo/include");
	args.push_back("-I"+ofPath+"/libs/cairo/include/cairo");
	args.push_back("-I"+ofPath+"/libs/glfw/include");
	args.push_back("-I"+ofPath+"/libs/glew/include");
	args.push_back("-I"+ofPath+"/libs/fmodex/include");
	args.push_back("-I"+ofPath+"/libs/poco/include");
	args.push_back("-I"+ofPath+"/libs/glm/include");
	args.push_back("-I"+ofPath+"/libs/utf8/include");
	args.push_back("-I"+ofPath+"/libs/json/include");
	args.push_back("-I"+ofPath+"/libs/pugixml/include");

	// create argc and argv
	int argc=args.size();
	char** argv = new char*[args.size()];
	for (int i=0; i<args.size(); i++) {
		argv[i] = new char[2048];
		sprintf(argv[i], "%s", args[i].c_str());
	}

	cling::Interpreter* interp = new cling::Interpreter(argc, argv, ofxClingPath.c_str());

	// free up memory
	for (int i=0; i<args.size(); i++) {
		delete argv[i];
	}
	delete [] argv;

	return interp;
}

void ofxCling::forceLinkWithSymbols()
{
	ofDrawRectangle(0, 0, 0, 0);
	ofDrawLine(0, 0, 0, 0);
	ofDrawCircle(0, 0, 0);
	ofDrawEllipse(0, 0, 0, 0);
	ofSetColor(255);
	ofNoFill();
	ofFill();
	ofRandom(1);
	ofRandom(0,1);
	ofDrawSphere(100);
	ofDrawSphere(0, 0, 100);
	ofDrawSphere(0, 0, 0, 100);
	ofEasyCam cam;
	ofColor(0);
	ofColor(0,0);
	ofColor(0,0,0);
	ofColor(0,0,0,0);
	ofVideoPlayer player;
	ofAVFoundationPlayer avplayer;
	ofLog();
}

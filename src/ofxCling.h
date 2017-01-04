//
//  ofxCling.h
//  ClingOFPlayground
//
//  Created by Gal Sasson on 1/4/17.
//
//

#ifndef ofxCling_h
#define ofxCling_h

#include "ofMain.h"
#include "cling/Interpreter/Interpreter.h"

class ofxCling
{
public:
	// ofPath: root path of openframeworks
	// ofxClingPath: root path of ofxCling
	// sysrootPath: Mac OS X SDK path (usually under /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/...)
	static cling::Interpreter* createInterpreter(const string& ofPath, const string& ofxClingPath, const string& sysrootPath);

	// this call is required in setup to force the linker to add some symbols
	static void forceLinkWithSymbols();
};

#endif /* ofxCling_h */

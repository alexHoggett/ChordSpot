#pragma once

#include "ofMain.h"
#include "ofxMaxim.h"
#define SAMPLE_LENGTH 262144*2 // must be a power of 2 -> ~12secs

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
        void audioIn(float * input, int bufferSize, int nChannels);
        void audioOut(float * output, int bufferSize, int nChannels);
    
        ofSoundStream soundStreamInput;
        ofSoundStream soundStreamOutput;
        ofPolyline line;
    
        std::array<float, SAMPLE_LENGTH> recording;
        int recordingBufferOffset;
        int playingBufferOffset;
        int sampleRate;
        int FFTSize;
        
        bool isPlaying;
        bool isRecording;
        bool analysed;
        
        int bufferSize;
    
        maxiFFT myFFT;
        blackmanHarrisWinFunctor blackMan;
        std::vector<float> currentFreqs;
        std::vector<int> top3(std::vector<float> chroma);
        float octaveSum(float baseFreq, int numOctaves);
    
        // Chord 'Database'
        //                       C  C# D  D# E  F  F# G  G# A  A# B
        std::vector<int> CMaj = {1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0}; // checked
        std::vector<int> DMaj = {0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0}; // checked
        std::vector<int> EMaj = {1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1}; // checked
        std::vector<int> FMaj = {1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0}; // checked
        std::vector<int> GMaj = {0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1}; // checked
        std::vector<int> AMaj = {0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0}; // checked
        std::vector<int> BMaj = {0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1}; // checked
};

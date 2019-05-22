#include "ofApp.h"

//--------------------------------------------------------------
// Function with 2 parameters: a base frequency & the number of octaves,
// the function will sum the bin magnitudes of the bins that correspond to the given
// base frequency and it's octaves
float ofApp::octaveSum(float baseFreq, int numOctaves){
    // Generate the octave frequencies
    std::vector<float> octaves;
    octaves.push_back(baseFreq);
    for(int i = 1; i < numOctaves + 1; i++){
        octaves.push_back(octaves[i - 1]*2);
    }
    
    // Summate the bin magnitudes that correspond to the octave frequencies
    float freqRes = (float)sampleRate / FFTSize;
    float chromaBin = 0;
    for(int i = 0; i < octaves.size(); i++){
        int targetBin = octaves[i]/freqRes;
        chromaBin += myFFT.magnitudes[targetBin];
    }
    return chromaBin;
}

//--------------------------------------------------------------
// Function that returns 3 index positions that correspond to the 3 greatest elements
std::vector<int> ofApp::top3(std::vector<float> chroma){
    float first, second, third = -99999999;
    int index1, index2, index3 = 0;
    
    
    for (int i = 0; i < chroma.size(); i++){
        if(chroma[i] > first){
            third = second;
            index3 = index2;
            second = first;
            index2 = index1;
            first = chroma[i];
            index1 = i;
        }
        else if(chroma[i] > second){
            third = second;
            index3 = index2;
            second = chroma[i];
            index2 = i;
        }
        else if(chroma[i] > third){
            third = chroma[i];
            index3 = i;
        }
    }
    std::vector<int> top3 = {index1, index2, index3};
    return top3;
}

//--------------------------------------------------------------
// Generate the frequencies for the FFT
std::vector<float> fftFreqs(int fftSize, int sampleRate){
    std::vector<float> theFreqs;
    for (int i = 0; i < fftSize; ++i){
        theFreqs.push_back((sampleRate/float(fftSize)) * i);
    }
    return theFreqs;
}

//--------------------------------------------------------------
void ofApp::setup(){
    analysed = false;
    
    ofSetVerticalSync(true);
    
    soundStreamInput.printDeviceList();
    
    bufferSize = 512;
    sampleRate = 44100;
    
    ofxMaxiSettings::setup(sampleRate, 2, bufferSize);
    
    soundStreamInput.setup(this, 0, 1, 44100, bufferSize, 4);
    soundStreamOutput.setup(this, 2, 0, 44100, bufferSize, 4);
    
    // FFT size, window size, hop size
    FFTSize = 8192*2;
    myFFT.setup(FFTSize, 8192, 256);
    
    //Calculate frequencies per bin
    currentFreqs = fftFreqs(FFTSize, sampleRate);
    
    playingBufferOffset = 0;
    recordingBufferOffset = 0;
    soundStreamOutput.stop();
    soundStreamInput.stop();
    
    ofSetBackgroundColor(157, 193, 131);
    
}

//--------------------------------------------------------------
void ofApp::update(){
    if(playingBufferOffset * bufferSize >= SAMPLE_LENGTH - bufferSize){
        isPlaying = false;
        soundStreamOutput.stop();
    }
    if(recordingBufferOffset * bufferSize >= SAMPLE_LENGTH - bufferSize){
        isRecording = false;
        soundStreamInput.stop();
    }
    
//    cout << "current" << currentFreqs[81] << endl;
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofSetColor(0);
    
    ofDrawBitmapString("PRESS 'R' To Record", 31, 20);
    ofDrawBitmapString("PRESS 'P' To Play", 31, 34);
    ofDrawBitmapString("PRESS 'F' To Analyze", 31, 48);
    
    string reportString = "recordBuffer: "+ofToString(recordingBufferOffset)+"\nplayBuffer: "+ofToString(playingBufferOffset);
    ofDrawBitmapString(reportString, 32, 200);
    
    if(isPlaying){
        ofDrawBitmapString("PLAYING", 31, 92);
    }
    if(isRecording){
        ofDrawBitmapString("RECORDING", 31, 130);
    }
    if(analysed){
        
        int maxBin = 0;
        float maxMag = 0;
        float maxFreq = 0;
        //find max frequency bin
        for(int i = 0; i < myFFT.bins; ++i){
            if(myFFT.magnitudes[i] > maxMag){
                maxMag = myFFT.magnitudes[i];
                maxFreq = currentFreqs[i];
                maxBin = i;
            }
        }
        cout << "max mag: " << maxMag << endl;
        cout << "max freq: " << maxFreq  << endl;
        cout << "max bin: " << maxBin << endl;
        
        cout << myFFT.magnitudes[129] << endl;
        cout << myFFT.magnitudes[130] << endl;
        cout << myFFT.magnitudes[131] << endl;
        
        // Draw spectrum
        ofTranslate(0, ofGetHeight()/2);
        // Spread the bins over the width of the window
        float binWidth = ofGetWidth()/myFFT.bins;
        // Horizontal line
        ofDrawLine(0, 0, ofGetWidth(), 0);
        
        int drawFreq = 0;
        // one line per bin
        for(int i = 0; i < myFFT.bins; ++i) {
            ofPoint pt;
            pt.set(i*binWidth, myFFT.magnitudes[i] * -5.);
            line.addVertex(pt);
            
            // print frequencies every 45 bins
            if(drawFreq <= 0){
                ofDrawBitmapString(currentFreqs[i + 3], i*binWidth, 20);
                drawFreq = 45;
            }
            --drawFreq;
        }
        line.end();
        line.draw();
        
        // calculate chroma bins
        // lowest octaves (in Hz)
        float baseC = 16.35;
        float baseDb = 17.325;
        float baseD = 18.355;
        float baseEb = 19.445;
        float baseE = 20.6;
        float baseF = 21.83;
        float baseGb = 23.125;
        float baseG = 24.5;
        float baseAb = 25.955;
        float baseA = 27.5;
        float baseBb = 29.135;
        float baseB = 30.87;
        
        //
        float C = octaveSum(baseC, 8);
        float Db = octaveSum(baseDb, 8);
        float D = octaveSum(baseD, 8);
        float Eb = octaveSum(baseEb, 8);
        float E = octaveSum(baseE, 8);
        float F = octaveSum(baseF, 8);
        float Gb = octaveSum(baseGb, 8);
        float G = octaveSum(baseG, 8);
        float Ab = octaveSum(baseAb, 8);
        float A = octaveSum(baseA, 8);
        float Bb = octaveSum(baseBb, 8);
        float B = octaveSum(baseB, 8);
        
//        cout << "C: " << C << endl;
//        cout << "Db: " << Db << endl;
//        cout << "D: " << D << endl;
//        cout << "Eb: " << Eb << endl;
//        cout << "E: " << E << endl;
//        cout << "F: " << F << endl;
//        cout << "Gb: " << Gb << endl;
//        cout << "G: " << G << endl;
//        cout << "Ab: " << Ab << endl;
//        cout << "A: " << A << endl;
//        cout << "Bb: " << Bb << endl;
//        cout << "B: " << B << endl;

        // create a vector to store the results and find the top 3 occuring 'notes'
        std::vector<float> chroma = {C, Db, D, Eb, E, F, Gb, G, Ab, A, Bb, B};
        std::vector<int> notes = top3(chroma);

        // create a prediction vector for the chord
        std::vector<int> prediction = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        for(int i = 0; i < notes.size(); i++){
            prediction[notes[i]] =  1;
        }

        for(int i = 0; i < prediction.size(); i++){
            cout << prediction[i] << " ";
        }
        cout << endl;

        string final;

        if(prediction == CMaj){
            final = "C";
        } else if(prediction == DMaj){
            final = "D";
        } else if(prediction == EMaj){
            final = "E";
        } else if(prediction == FMaj){
            final = "F";
        } else if(prediction == GMaj){
            final = "G";
        } else if(prediction == AMaj){
            final = "A";
        } else if(prediction == BMaj){
            final = "B";
        } else{
            final =  "Sorry that chord could not be found";
        }

        ofDrawBitmapString(final, 31, 62);
    }
}

//--------------------------------------------------------------
void ofApp::audioIn(float * input, int bufferSize, int nChannels){
    if(isRecording){
        for (int i = 0; i < bufferSize; i++){
            recording[i+recordingBufferOffset*bufferSize] = input[i];
        }
        recordingBufferOffset++;
    }
}

//--------------------------------------------------------------
void ofApp::audioOut(float * output, int bufferSize, int nChannels){
    if (isPlaying) {
        for (int i = 0; i < bufferSize; i++){
            output[ i*2+0 ] = recording[i + playingBufferOffset*bufferSize];
            output[ i*2+1 ] = recording[i + playingBufferOffset*bufferSize];
        }
        playingBufferOffset++;
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed  (int key){
    if( key == 'r' ){
        if(isRecording) {
            isRecording = false;
            soundStreamInput.stop();
        }
        else{
            isRecording = true;
            soundStreamInput.start();
            recordingBufferOffset = 0;
            analysed = false;
            recording.fill(0.0);
            line.clear();
        }
    }
    if( key == 'p' ){
        if(isPlaying) {
            isPlaying = false;
            soundStreamOutput.stop();
        }
        else{
            isPlaying = true;
            soundStreamOutput.start();
            playingBufferOffset = 0;
        }
    }
    if( key == 'f' ){
        for(int i = 0; i < recordingBufferOffset*bufferSize; i++){
            myFFT.process(recording[i]);
        }
        analysed = true;
    }
}



//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}


# ChordSpot
An Open Frameworks application that recognises guitar chords.

---

### Introduction

Using openFrameworks in addition to the addon Maximilian, found [here](https://github.com/falcon4ever/ofxMaxim), Chordspot is an application that takes audio recordings of up to 12 seconds and compares its frequency content with that of known chords. This project has been intended for, and tested on, guitar chords however instrument should work in theory.

### Description

For the most accurate results the chord should be played for the full duration of the 12 seconds, strumming clearly every 3 - 4 seconds and never letting the sound 'die out'. This provides the maximum amount of frequency content for the FFT to work with and reduces the chance of anomalous results.

You may adjust the values of the FFT size, window size & hop size however i belive i have found the most accurate results without implementing Frame blocking or Windowing.

higher quality recording equiment is preferred, most in-built laptop microphones work fine however.

This project was aided by the research conducted in:

*Chord Identification Using Pitch Class Profile Method with Fast Fourier Transform Feature Extraction, Mulundi, Aristoteles, Loupatty, IJCSI International Journal of Computer Science Issues, Vol. 11, Issue 3, No 1, May 2014*.

# Timbre-Generator
Play midi with your timbre specified by waveform.

## Requirements
1. Python 2
2. Pillow library
3. Python-midi library

## Setup
Compile .cpp files with speed optimization.
For example, using g++,
```
g++ file.cpp -o file -O3
```
## Usage
1. Draw your own waveshape picture!  
   (We gave you a default picture, so feel free to skip this step.)  
   The file must be saved in 1-bit bmp format, with resolution 1000*200.  
   Put it in ```./pic```.
2. Generate the timbre file.  
   ```from Timbre import *
   create_timbre("wav", "myTimbre")```
   Where the first argument is the name (without extension) of the bmp file,  
   and the second is the name of the outputted timbre file.
   Then you will be able to see ```myTimbre.tone``` in ```./timbre```.
3. Play your midi!
   Put your midi in ```./midi```, then execute
   ```play_midi("name_of_midi", "name_of_timbre")```
   For example, if you want to play ```music.mid``` with ```myTimbre```, then you should run
   ```play_midi("music", "myTimbre")```
4. Collect the output ```wave.wav``` in the current directory.  
   (Note that we do not need to generate a new timbre each time we play a new midi.)

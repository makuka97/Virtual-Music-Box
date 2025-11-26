```
================================================================================
                            MUSIC BOX
                        Design Documentation
                         Terminal Edition
================================================================================

TABLE OF CONTENTS
-----------------
1. Overview
2. Design Philosophy
3. Architecture
4. File Format Specification
5. Usage Guide
6. Implementation Details
7. Extending the System


================================================================================
1. OVERVIEW
================================================================================

Music Box is a programmable music sequencer that runs in the terminal. It 
reads simple text files containing musical notation and plays them using sine
wave synthesis. The program is inspired by mechanical music boxes - physical
devices that play music by rotating a cylinder with pins that pluck metal 
tines.

Key Features:
-------------
  * Simple text-based music notation
  * Real-time playback with visual feedback
  * Interactive command-line interface
  * Portable - runs on any system with C++11
  * Extensible - easy to add new songs


================================================================================
2. DESIGN PHILOSOPHY
================================================================================

Mechanical Metaphor
-------------------
The entire system is designed around the metaphor of a physical music box:

  Physical Component          Software Equivalent
  ------------------          -------------------
  Metal cylinder              .txt file
  Pins on cylinder            Characters in file
  Metal tines (comb)          Frequency table
  Cylinder rotation           Main playback loop
  Spring motor                Tempo control
  Plucking action             Sine wave generation


Constraints as Features
-----------------------
Like real music boxes, this program embraces limitations:

  * Fixed note durations (one character = one beat)
  * Limited note range (C-B in two octaves)
  * Simple tones (pure sine waves)
  * Sequential playback (no random access)

These constraints make the system:
  * Easy to learn
  * Quick to compose
  * Predictable and reliable
  * Authentic to the music box aesthetic


Data-Driven Design
------------------
The program is a player, not a composer. Music is data, not code.

  Song File (DATA)  -->  Music Box (PROGRAM)  -->  Sound (OUTPUT)

This separation means:
  * Non-programmers can create songs
  * Songs are portable text files
  * Easy to share and modify
  * No recompilation needed


================================================================================
3. ARCHITECTURE
================================================================================

Component Structure
-------------------

music.h
-------
  Header file containing the MusicBox class interface.
  Defines public methods and private data members.

music.cpp
---------
  Implementation of the MusicBox class.
  Contains:
    * Frequency table initialization
    * File parsing logic
    * Sound generation (sine waves)
    * Tempo control
    * Visual display formatting

main.cpp
--------
  Entry point and user interface.
  Contains:
    * Command loop (REPL)
    * File listing functionality
    * User input parsing
    * Visual feedback (loading animations)


Class Design: MusicBox
-----------------------

Private Members:
  map<char, double> frequencies   // Note name -> frequency (Hz)
  int tempo                        // Beats per minute
  string title                     // Current song title

Public Interface:
  MusicBox()                       // Constructor
  void play(filename)              // Load and play a song
  void setTempo(bpm)               // Change playback speed
  int getTempo()                   // Get current tempo

Private Methods:
  void initNotes()                 // Build frequency table
  void playTone(freq, duration)    // Generate single note
  void printNote(char)             // Visual feedback
  int getBeatDuration()            // Convert BPM to milliseconds


Data Flow
---------

1. User Input
   |
   v
2. File Reading (parse .txt file)
   |
   v
3. Character Processing (loop through each character)
   |
   v
4. Frequency Lookup (map character to Hz)
   |
   v
5. Sound Generation (sine wave at frequency)
   |
   v
6. Audio Output (system beep or audio library)
   |
   v
7. Visual Display (show note being played)


================================================================================
4. FILE FORMAT SPECIFICATION
================================================================================

Structure
---------
Every song file must have exactly this structure:

Line 1:  Song Title (any text)
Line 2:  Tempo (T followed by number, e.g., T120)
Line 3+: Musical notation (characters representing notes)


Character Mapping
-----------------

Musical Notes (Lower Octave - C4 to B4):
  C = 261.63 Hz
  D = 293.66 Hz
  E = 329.63 Hz
  F = 349.23 Hz
  G = 392.00 Hz
  A = 440.00 Hz
  B = 493.88 Hz

Musical Notes (Higher Octave - C5 to E5):
  c = 523.25 Hz
  d = 587.33 Hz
  e = 659.25 Hz

Special Characters:
  - = Rest (silence)
  | = Bar line (visual separator, no sound)
  Space = Ignored
  
Sharps/Flats:
  # after a note = Sharp (e.g., C# = C-sharp)
  b after a note = Flat (e.g., Bb = B-flat)


Timing
------
Duration is controlled by tempo:

  * Each character = 1 beat
  * Tempo = beats per minute (BPM)
  * Beat duration = 60000 / BPM milliseconds

Examples:
  T60  = 1 beat per second (slow)
  T120 = 2 beats per second (moderate)
  T180 = 3 beats per second (fast)


Example File
------------

twinkle_little_star.txt:
------------------------
Twinkle Twinkle Little Star
T120
CCGGAA|G---|
FFEEDDC---|
GGFFEE|D---|
GGFFEE|D---|
CCGGAA|G---|
FFEEDDC---|


Translation:
  Line 1: Title = "Twinkle Twinkle Little Star"
  Line 2: Tempo = 120 BPM (500ms per beat)
  Line 3: Play C C G G A A, pause, G, rest (3 beats)
  Line 4: Play F F E E D D, pause, C, rest (3 beats)
  ...and so on


================================================================================
5. USAGE GUIDE
================================================================================

Compilation
-----------

Standard compile:
  g++ -std=c++11 main.cpp music.cpp -o musicbox

With optimizations:
  g++ -std=c++11 -O2 main.cpp music.cpp -o musicbox

Make executable:
  chmod +x musicbox


Running
-------

Start the music box:
  ./musicbox

You'll see:
  +====================================+
  |      ** MUSIC BOX **               |
  |      Terminal Edition              |
  |      Written By Micajah Nordyke    |
  +====================================+

  Commands:
  ------------------------------------
  play <file>  - Insert and play cylinder
  list         - Show available cylinders
  help         - Show this help
  quit         - Close the music box

  >


Commands
--------

list
----
  Shows all .txt files in the current directory.
  Files starting with _ are hidden (templates).
  
  Example:
    > list
    
    Available Cylinders:
    ------------------------------------
   * twinkle_little_star.txt

play <filename>
---------------
  Loads and plays a song file.
  Shows loading animation, then waits for ENTER to start.
  
  Example:
    > play twinkle_little_star.txt
    
    Inserting cylinder... OK
    
    >> Wind the spring (press ENTER)...
    [User presses ENTER]
    
    [Music plays with visual display]

help
----
  Displays command list.

quit
----
  Exits the program.


Creating Songs
--------------

1. Create a new .txt file:
   nano my_song.txt

2. Add the three required parts:
   My Song Title
   T120
   CDEFGABC

3. Save and play:
   > play my_song.txt


Tips for Composing
-------------------

Start Simple:
  Test with: CDEFGABC
  This plays a C major scale

Use Bar Lines:
  CCCC|DDDD|EEEE|FFFF
  Makes it easier to read (4 beats per bar)

Add Rests for Rhythm:
  C-C-|E-E-|G-G-|C---
  Creates bouncy, staccato feel

Experiment with Tempo:
  T60  = Slow, peaceful
  T120 = Normal, moderate
  T180 = Fast, energetic

Higher Notes (lowercase):
  CDEFGABcde
  Ascends from low to high


================================================================================
6. IMPLEMENTATION DETAILS
================================================================================

Sound Generation
----------------

The program uses sine wave synthesis to create pure tones:

  amplitude = sin(2 * π * frequency * time)

This mimics the physics of a vibrating metal tine in a real music box.

Platform-Specific Audio:
  Windows: Beep() function (winmm.lib)
  Linux/Mac: Terminal bell (\a) or PortAudio (if available)


Frequency Calculation
---------------------

Musical notes follow the equal temperament scale:

  frequency = base_frequency * 2^(semitones/12)

Standard tuning:
  A4 = 440 Hz (concert pitch)
  
Each semitone is 2^(1/12) ≈ 1.059463 times the previous note.


Timing Precision
----------------

Beat duration calculated from tempo:

  milliseconds_per_beat = 60000 / BPM

Examples:
  120 BPM = 500 ms per beat
  180 BPM = 333 ms per beat
  60 BPM = 1000 ms per beat

Uses std::this_thread::sleep_for() for delays.


Character Encoding
------------------

Uses ASCII-only characters for maximum compatibility:
  * No Unicode box-drawing characters
  * No special musical symbols
  * Works on any terminal

Original design used UTF-8 but caused issues on some systems.


File Parsing Strategy
---------------------

Sequential, line-by-line parsing:

1. Read line 1 → Store as title
2. Read line 2 → If starts with 'T', parse as tempo
3. Read remaining lines → Process each character

Character processing:
  * Skip whitespace
  * Lookup in frequency table
  * If found: play tone
  * If not found: ignore silently

This allows flexible formatting (spaces, newlines don't matter).


Visual Feedback
---------------

During playback, shows:
  * Current note being played
  * Bar lines as separators
  * Dots for rests
  * Auto-wraps every 16 characters

Example display:
   C  C  G  G  A  A  |  G  .  .  . 
   F  F  E  E  D  D  |  C  .  .  . 

This mimics watching pins strike tines on a real music box.


================================================================================
7. EXTENDING THE SYSTEM
================================================================================

Adding More Notes
-----------------

To extend the range, edit music.cpp initNotes():

  frequencies['C3'] = 130.81;   // Lower octave
  frequencies['C6'] = 1046.50;  // Higher octave

Then update documentation to tell users about new notes.


Adding Effects
--------------

Current implementation uses pure sine waves. To add richness:

Harmonics (overtones):
  double sound = sin(2*π*f*t) * 0.6;        // Fundamental
  sound += sin(2*π*f*2*t) * 0.15;           // 2nd harmonic
  sound += sin(2*π*f*3*t) * 0.1;            // 3rd harmonic

ADSR Envelope:
  Attack:  Fade in over first 20ms
  Decay:   Drop to 70% over next 50ms
  Sustain: Hold at 70%
  Release: Fade out over last 100ms

Vibrato:
  frequency_mod = frequency * (1 + 0.01 * sin(6 * t))


Adding New Commands
-------------------

To add a command, edit main.cpp main() loop:

  else if (input == "random") {
      // Pick random song from list
      vector<string> songs = listCylinders();
      if (!songs.empty()) {
          int idx = rand() % songs.size();
          box.play(songs[idx]);
      }
  }


Supporting Subdirectories
-------------------------

Modify listCylinders() to search recursively:

  songs/
    classical/
      beethoven.txt
      mozart.txt
    pop/
      beatles.txt

Then update play command to handle paths:
  > play classical/beethoven.txt


Dynamic Tempo Changes
----------------------

Add support for tempo changes mid-song:

  My Song
  T120
  CCCC
  T180
  DDDD

Parse 'T' lines during playback, not just at start.


Chord Support
-------------

Allow multiple simultaneous notes:

  [CEG]   = C major chord (play C, E, G together)

Implementation:
  * Parse bracket notation
  * Mix multiple sine waves
  * Average amplitudes to prevent clipping


Loop Support
------------

Add repeat syntax:

  :start
  CDEFG
  :end 3

Play the section 3 times.


MIDI Import
-----------

For advanced users, could add MIDI file support:

1. Parse MIDI file
2. Extract note events
3. Convert to music box notation
4. Save as .txt file

This would allow importing existing music.


Real Audio Output
-----------------

For better sound quality on Linux/Mac, integrate PortAudio:

  #include <portaudio.h>
  
  // Generate audio buffer
  // Write to stream
  // Better control over waveform

This requires linking against PortAudio library.


================================================================================

APPENDIX A: FREQUENCY TABLE
----------------------------

Note    Frequency (Hz)    Octave
----    --------------    ------
C       261.63           4
C#/Db   277.18           4
D       293.66           4
D#/Eb   311.13           4
E       329.63           4
F       349.23           4
F#/Gb   369.99           4
G       392.00           4
G#/Ab   415.30           4
A       440.00           4 (concert pitch)
A#/Bb   466.16           4
B       493.88           4
c       523.25           5
c#/db   554.37           5
d       587.33           5
d#/eb   622.25           5
e       659.25           5


APPENDIX B: TEMPO GUIDE
------------------------

BPM     Description              Feel
---     -----------              ----
40      Grave                    Very slow
60      Largo                    Slow
76      Adagio                   Leisurely
100     Andante                  Walking pace
120     Moderato                 Moderate
144     Allegro                  Fast
168     Vivace                   Lively
200     Presto                   Very fast


APPENDIX C: COMMON PATTERNS
----------------------------

Major Scale:
  CDEFGABC

Minor Scale:
  ABCDEFGA

Arpeggio (C major):
  CEGC

Chromatic Scale:
  CC#DD#EFF#GG#AA#B

Twinkle Pattern:
  CCGGAA|G---|
  FFEEDDC---|


                              END OF DOCUMENTATION
================================================================================
```

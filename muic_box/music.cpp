#include "music.h"
#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>
#include <cmath>

#ifdef _WIN32
#include <windows.h>
#pragma comment(lib, "winmm.lib")
#endif

MusicBox::MusicBox() : tempo(120), title("") {
    initNotes();
}

void MusicBox::initNotes() {
    // The metal comb - each character maps to a frequency
    // Lower octave (C4-B4)
    frequencies['C'] = 261.63;
    frequencies['D'] = 293.66;
    frequencies['E'] = 329.63;
    frequencies['F'] = 349.23;
    frequencies['G'] = 392.00;
    frequencies['A'] = 440.00;
    frequencies['B'] = 493.88;
    
    // Higher octave (C5-E5) - lowercase letters
    frequencies['c'] = 523.25;
    frequencies['d'] = 587.33;
    frequencies['e'] = 659.25;
    
    // Rest (silence)
    frequencies['-'] = 0.0;
}

void MusicBox::playTone(double frequency, int durationMs) {
    if (frequency == 0.0) {
        // Rest - the cylinder rotates but no pin strikes
        std::this_thread::sleep_for(std::chrono::milliseconds(durationMs));
        return;
    }
    
    #ifdef _WIN32
    // Windows: Use Beep function (simple but effective)
    Beep(static_cast<DWORD>(frequency), static_cast<DWORD>(durationMs));
    #else
    // Unix/Linux/Mac: Simulate with sleep
    // In production, you would use PortAudio, ALSA, or similar
    std::this_thread::sleep_for(std::chrono::milliseconds(durationMs));
    std::cout << "\a"; // Terminal bell as fallback
    #endif
}

void MusicBox::printNote(char note) {
    if (note == '-') {
        std::cout << " . ";
    } else if (note == '|') {
        std::cout << " | ";
    } else {
        std::cout << " " << note << " ";
    }
    std::cout.flush();
}

int MusicBox::getBeatDuration() const {
    // Convert BPM to milliseconds per beat
    // 60 BPM = 1 beat per second = 1000ms
    // 120 BPM = 2 beats per second = 500ms
    return 60000 / tempo;
}

void MusicBox::setTempo(int bpm) {
    if (bpm > 0 && bpm <= 300) {
        tempo = bpm;
    }
}

int MusicBox::getTempo() const {
    return tempo;
}

void MusicBox::displayHeader() const {
    std::cout << "\n-----------------------------------\n";
    std::cout << "|   MUSIC BOX                    |\n";
    std::cout << "|------------------------------------|\n";
    std::cout << "|------------------------------------|  ";
    
    // Display title, padded to fit
    std::string displayTitle = title;
    if (displayTitle.length() > 34) {
        displayTitle = displayTitle.substr(0, 31) + "...";
    }
    std::cout << displayTitle;
    
    for (size_t i = displayTitle.length(); i < 34; i++) {
        std::cout << " ";
    }
    std::cout << "|\n";
    std::cout << "|---------------------------------|\n\n";
}

void MusicBox::displayFooter() const {
    std::cout << "\n\n  Music box finished.\n\n";
}

void MusicBox::play(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open file '" << filename << "'\n";
        std::cerr << "Make sure the file exists and the path is correct.\n";
        return;
    }
    
    // Read the title (first line)
    std::getline(file, title);
    if (title.empty()) {
        title = "Untitled";
    }
    
    // Display the music box header
    displayHeader();
    
    std::cout << "  Playing at " << tempo << " BPM...\n\n  ";
    
    // Read and play the tape (cylinder)
    std::string line;
    int noteCount = 0;
    int beatDuration = getBeatDuration();
    
    while (std::getline(file, line)) {
        // Check for tempo command
        if (!line.empty() && line[0] == 'T') {
            int newTempo = std::stoi(line.substr(1));
            setTempo(newTempo);
            beatDuration = getBeatDuration();
            std::cout << "\n  [Tempo: " << tempo << " BPM]\n  ";
            continue;
        }
        
        // Play each character as a note
        for (char c : line) {
            // Skip spaces
            if (c == ' ') continue;
            
            // Display the note
            printNote(c);
            
            // Play the note if it's in our frequency table
            if (frequencies.count(c)) {
                playTone(frequencies[c], beatDuration);
            } else if (c == '|') {
                // Bar line - visual only, no pause
            } else {
                // Unknown character - skip silently
            }
            
            // Line break every 16 notes for readability
            noteCount++;
            if (noteCount % 16 == 0) {
                std::cout << "\n  ";
            }
        }
    }
    
    // Display footer
    displayFooter();
    
    file.close();
}
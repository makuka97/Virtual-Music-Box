#ifndef MUSIC_H
#define MUSIC_H

#include <string>
#include <map>
#include <vector>

class MusicBox {
private:
    std::map<char, double> frequencies;
    int tempo;
    std::string title;
    
    // Initialize the frequency table (the "comb")
    void initNotes();
    
    // Generate and play a single tone (pluck a "tine")
    void playTone(double frequency, int durationMs);
    
    // Visual feedback - show the note being played
    void printNote(char note);
    
    // Calculate beat duration from tempo
    int getBeatDuration() const;
    
public:
    MusicBox();
    
    // Set the tempo (spring tension)
    void setTempo(int bpm);
    
    // Get current tempo
    int getTempo() const;
    
    // Load and play a song from file (load the "cylinder")
    void play(const std::string& filename);
    
    // Display the music box interface
    void displayHeader() const;
    void displayFooter() const;
};

#endif // MUSIC_H
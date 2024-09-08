//
//  main.cpp
//  p1_countess
//
//  Created by Bends on 9/4/24.
//

#include <iostream>
#include <fstream>
#include <string>
#include <getopt.h>
#include <vector>
#include <deque>

using namespace std;

class castleMap {
private:
    char inputMode;
    size_t numRooms;
    uint32_t lengthRoomSide;
    vector<vector<vector<char>>> castleMap;

public:
    void printOutput();
    
    void readFromInput(const string &filename);
    
};

void castleMap::printOutput() {
    for (size_t room = 0; room < numRooms; ++room) {
        for (size_t row = 0; row < lengthRoomSide; ++row) {
            for (size_t col = 0; col < lengthRoomSide; ++col) {
                cout << "(" << room << "," << row << "," << col << "," << castleMap[room][row][col] << ")" << "\n";
            }
        }
    }
}
void castleMap::readFromInput(const string &filename) {
    ifstream fin;
    fin.open(filename.c_str());
    if (!fin.is_open()) {
        std::cout << "open failed" << std::endl;
        exit(1);
    }
    
    fin >> inputMode;
    fin >> numRooms;
    fin >> lengthRoomSide;
    
    castleMap.resize(numRooms, vector<vector<char>>(lengthRoomSide, vector<char>(lengthRoomSide, '.'))); // Initialize with '.' or any default char

    // (0,0,1,C)
    string line;
    while (getline(fin,line)) {
        // skip comments
        if (line.find("//") == 0) {
            continue;
        }
        if (line[0] == '(') {
            
            size_t pos1 = line.find('(') + 1;
            size_t pos2 = line.find(',', pos1);
            size_t room = (size_t)stoi(line.substr(pos1,pos2-pos1));
            
            pos1 = pos2 + 1;   // index right after the comma
            pos2 = line.find(',', pos1);
            size_t row = (size_t)stoi(line.substr(pos1,pos2-pos1));
            
            pos1 = pos2 + 1;   // index right after the comma
            pos2 = line.find(',', pos1);
            size_t col = (size_t)stoi(line.substr(pos1,pos2-pos1));
            
            pos1 = pos2 + 1;
            char ch = line[pos1];
            
            castleMap[room][row][col] = ch;
        }
    }
    
    printOutput();
    
    fin.close();
}

//int argc, const char * argv[]

int main_2(/*int argc, const char * argv[]*/) {
    // turning off synchronzied C-C++ I/O - saves time
    std::ios_base::sync_with_stdio(false);
    try {
        castleMap c;
        string filename = "spec-L.txt";
        c.readFromInput(filename);
    }
    catch (std::runtime_error &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}

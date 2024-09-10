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
    // initialization variables
    char inputMode;
    size_t numRooms;
    uint32_t lengthRoomSide;
    vector<vector<vector<char>>> castleMap;
    
    // Marco (S) location
    size_t room_S;
    size_t row_S;
    size_t col_S;

    void processingListMode(string &line) {
        // skip comments
        if (line.find("//") == 0) {
            return;
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
            
            if (ch == 'S') {
                room_S = room;
                row_S = row;
                col_S = col;
            }
            
            castleMap[room][row][col] = ch;
        }
    }
    
    void processingMapMode(const string &line, size_t &room, size_t &row) {
        // skip comments
        if (line.find("//") == 0) {
            return;
        }
        // go through all columns for that specific row
        for (size_t col = 0; col < lengthRoomSide; ++col) {
            // avoiding rewriting '.' char
            if (line[col] != '.') {
                //cout << line[col] << endl;
                if (line[col]  == 'S') {
                    room_S = room;
                    row_S = row;
                    col_S = col;
                }
                castleMap[room][row][col] = line[col];
            }
        }
        // after going through each col in row, increment row
        ++row;
        // once row is at length, room increments and row resets for the next room
        if (row == lengthRoomSide) {
            row = 0;
            ++room;
        }
    }
    
    void printListOutput() {
        for (size_t room = 0; room < numRooms; ++room) {
            for (size_t row = 0; row < lengthRoomSide; ++row) {
                for (size_t col = 0; col < lengthRoomSide; ++col) {
                    cout << "(" << room << "," << row << "," << col << "," << castleMap[room][row][col] << ")" << "\n";
                }
            }
        }
    }
    
    void printMapOutput() {
        for (size_t room = 0; room < numRooms; ++room) {
            cout << "//castle room " << room << "\n";
            for (size_t row = 0; row < lengthRoomSide; ++row) {
                for (size_t col = 0; col < lengthRoomSide; ++col) {
                    cout << castleMap[room][row][col];
                }
                cout << "\n";
            }
        }
    }

public:
    void readFromInput(const string &filename);
    void searchAlgorithm();
};

void castleMap::searchAlgorithm() {
    cout << "Room S: " << room_S << endl;
    cout << "Row S: " << row_S << endl;
    cout << "Col S: " << col_S << endl;

    // N: current row - 1; check to see if it's out of bounds
    // E: current col + 1; check to see if it's out of bounds
    // S: current row + 1; check to see if it's out of bounds
    // W: current col - 1; check to see if it's out of bounds
}

void castleMap::readFromInput(const string &filename) {
    // creating stream object
    ifstream fin;
    fin.open(filename.c_str());
    if (!fin.is_open()) {
        std::cout << "open failed" << std::endl;
        exit(1);
    }
    
    // Reading in initialization data from input and resizing vectors appropriately
    fin >> inputMode;
    fin >> numRooms;
    // the ws is very important here to get rid of all the whitespace / other nonsense and skip to the next line
    fin >> lengthRoomSide >> ws;
    castleMap.resize(numRooms, vector<vector<char>>(lengthRoomSide, vector<char>(lengthRoomSide, '.'))); // Initialize with '.' or any default char
    
    // processing line by line based on input Mode
    if (inputMode == 'L') {
        string line = "";
        while (getline(fin,line)) {
            processingListMode(line);
        }
        //TODO: debugging by printing output
        //printListOutput();
    }
    else if (inputMode == 'M') {
        string line = "";
        size_t room = 0;
        size_t row = 0;
        while (getline(fin,line)) {
            cout << line << endl;
            processingMapMode(line, room, row);
        }
        //TODO: debugging by printing output
        //printMapOutput();
    }
    
    // close file once we've stored all data
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

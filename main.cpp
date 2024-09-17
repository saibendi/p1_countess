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
#include <algorithm>

using namespace std;

class castleMap {
private:
    // initialization variables
    char inputMode;
    size_t numRooms;
    uint32_t lengthRoomSide;
    
    // defining a struct Location to hold values
    struct Location {
        size_t room;
        size_t row;
        size_t col;
        
        bool operator==(const Location &rhs) const {
            return room == rhs.room && row == rhs.row && col == rhs.col;
        }
        
        //TODO: comment out; just for debug purposes until I find something to do it with it later
        friend ostream& operator<<(ostream& os, const Location &loc) {
            os << "(" << loc.room << "," << loc.row << "," << loc.col << ")" << endl;
            return os;
        }
    };
    
    // main data structures
    vector<vector<vector<char>>> castleMap;
    deque<Location> search;
    
    // Marco (S) and Countess (C) location
    Location S;
    Location C;

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
                S = {room, row, col};
            }
            else if (ch == 'C') {
                C = {room, row, col};
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
                    S = {room, row, col};
                }
                else if (line[col] == 'C') {
                    C = {room, row, col};
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

    bool locationIsMoveable(const Location &newLoc) {
        bool roomIsValid = (newLoc.room >= 0 && newLoc.room <= numRooms-1);
        bool rowIsValid = (newLoc.row >=0 && newLoc.row <= lengthRoomSide - 1);
        bool colIsValid = (newLoc.col >=0 && newLoc.col <= lengthRoomSide - 1);
        
        bool newLocIsWall = false;
        bool newLocIsMinion = false;
        
        // only enter if loc is Valid
        if (roomIsValid && rowIsValid && colIsValid) {
            newLocIsWall = (castleMap[newLoc.room][newLoc.row][newLoc.col] == '#');
            newLocIsMinion = (castleMap[newLoc.room][newLoc.row][newLoc.col] == '!');
        }
        return roomIsValid && rowIsValid && colIsValid && !newLocIsWall && !newLocIsMinion;
    }
    
    //TODO: need to replace check here; think of not doing O(N), maybe try binary / ternary search / need sorted vector for that
    bool locationNotVisited(const vector<Location> &locationHistory, const Location &input) {
        // check to see if current has been previously visited
        return (find(locationHistory.begin(), locationHistory.end(), input)) == locationHistory.end();
    }
    
public:
    void readFromInput(const string &filename);
    void searchAlgorithm();
};

void castleMap::searchAlgorithm() {
    search.push_back(S);
    Location current;
    vector<Location> locationVisitedHistory;
    vector<Location> locationMoveableHistory;
//    int i = 0;
    // Current Location isn't equal to Countess Location)
    while (!(current == C)) {
//        ++i;
        // 0. If search container is empty before you reach Countess, search has failed
        if (search.empty()) {
            cout << "no path to rescue Countess" << endl;
            break;
        }
        // 1. Remove the next position from search container
        current = search.back();
        //TODO: think about potentially doing out of bounds check here itself when we read the current value to avoid subtracting row -1 and getting random numbers like 184903099009309

        search.pop_back();      // deleting element once we remove it off stack
        locationVisitedHistory.push_back(current); // add it to location history now
        cout << "Current: (" << current.room << "," << current.row << "," << current.col << ")" << endl;

        // 2. if position has a warp pipe; probably need to do some level of char->int int-> char conversion here
        if (castleMap[current.room][current.row][current.col] >= 0 && castleMap[current.room][current.row][current.col] <= 9) {
            //TODO: replace continue w/ logic
            continue;
        }
        else {
            // N: current row - 1
            // if row is not equal to 0 
            Location N = {current.room, current.row - 1, current.col};
            cout << N << " " << locationIsMoveable(N) << endl;
            // E: current col + 1
            Location E = {current.room, current.row, current.col + 1};
            cout << E << " " << locationIsMoveable(E) << endl;
            // S: current row + 1; check to see if it's out of bounds
            Location S = {current.room, current.row + 1, current.col};
            cout << S << " " << locationIsMoveable(S) << endl;
            // W: current col - 1; check to see if it's out of bounds
            Location W = {current.room, current.row, current.col - 1};
            cout << W << " " << locationIsMoveable(W) << endl;
            cout << "step into else" << endl;

            // check to see if location is moveable
            if (locationIsMoveable(N) && locationNotVisited(locationVisitedHistory, N) && locationNotVisited(locationMoveableHistory, N)) {
                
                search.push_back(N);
                locationMoveableHistory.push_back(N); // add it to location history now
            }
            if (locationIsMoveable(E) && locationNotVisited(locationVisitedHistory, E) && locationNotVisited(locationMoveableHistory, E)) {
                search.push_back(E);
                locationMoveableHistory.push_back(E); // add it to location history now
            }
            if (locationIsMoveable(S) && locationNotVisited(locationVisitedHistory, S) && locationNotVisited(locationMoveableHistory, S)) {
                search.push_back(S);
                locationMoveableHistory.push_back(N); // add it to location history now
            }
            if (locationIsMoveable(W) && locationNotVisited(locationVisitedHistory, W) && locationNotVisited(locationMoveableHistory, W)) {
                search.push_back(W);
                locationMoveableHistory.push_back(N); // add it to location history now
            }
        }
        cout << "-----------Search----------" << endl;
        for (auto i : search) {
            cout << i;
        }
//        if (i == 3) {
//            break;
//        }
    }
    
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

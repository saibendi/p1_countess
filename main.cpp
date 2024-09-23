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
#include <map>

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
            os << "(" << loc.room << "," << loc.row << "," << loc.col << ")" << "\n";
            return os;
        }
    };
    
    // defining a struct pathLocation to hold values
    struct pathLocation {
        size_t room;
        size_t row;
        size_t col;
        char symbol;
        
        //custom ctor
        pathLocation(const Location loc_in, const char symbol_in) : room(loc_in.room), row(loc_in.row), col(loc_in.col), symbol(symbol_in) {};
        
        bool operator==(const pathLocation &rhs) const {
            return room == rhs.room && row == rhs.row && col == rhs.col && symbol == rhs.symbol;
        }
        
        //TODO: comment out; just for debug purposes until I find something to do it with it later
        friend ostream& operator<<(ostream& os, const pathLocation &loc) {
            os << "(" << loc.room << "," << loc.row << "," << loc.col << "," << loc.symbol << ")" << "\n";
            return os;
        }
    };

    // main data structures
    vector<vector<vector<char>>> castleMap;
    deque<Location> search;
    vector<vector<vector<char>>> backtrace;
    
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
                //cout << line[col] << "\n";
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
        
        // only check if loc is Valid
        if (roomIsValid && rowIsValid && colIsValid) {
            newLocIsWall = (castleMap[newLoc.room][newLoc.row][newLoc.col] == '#');
            newLocIsMinion = (castleMap[newLoc.room][newLoc.row][newLoc.col] == '!');
        }
        return roomIsValid && rowIsValid && colIsValid && !newLocIsWall && !newLocIsMinion;
    }
        
    bool locationNotVisited(const Location &input) {
        return (backtrace[input.room][input.row][input.col] == '-');
    }

public:
    void readFromInput(const string &filename);
    void searchAlgorithm();
    void outputPath();
};

void castleMap::outputPath() {
    deque<pathLocation> path;
    Location current = C;
//    cout << current << "\n";
//    int debug = 0;
    // loop until starting position
    while (!(current == S)) {
        cout << "Loop current: " << current << "\n";
//        debug++;
//        if (debug == 5) {
//            break;
//        }
        // if west exists, add to path and make that current
        Location W = {current.room, current.row, current.col - 1};
        if (locationIsMoveable(W) && backtrace[W.room][W.row][W.col] != '-') {
            pathLocation W_path(W, backtrace[W.room][W.row][W.col]);
            path.push_front(W_path);
            backtrace[W.room][W.row][W.col] = '-';
            current = W;
            continue;
        }
        // if south exists, add to path and make that current
        Location So = {current.room, current.row+1, current.col};
//        cout << locationIsMoveable(So) << "\n";
//        cout << backtrace[So.room][So.row+1][So.col] << "\n";
        if (locationIsMoveable(So) && backtrace[So.room][So.row][So.col] != '-') {
            pathLocation So_path(So, backtrace[So.room][So.row][So.col]);
            path.push_front(So_path);
            backtrace[So.room][So.row][So.col] = '-';
            current = So;
            continue;
        }
        // if east exists, add to path and make that current
        Location E = {current.room, current.row, current.col+1};
        if (locationIsMoveable(E) && backtrace[E.room][E.row][E.col] != '-') {
            pathLocation E_path(E, backtrace[E.room][E.row][E.col]);
            path.push_front(E_path);
            backtrace[E.room][E.row][E.col] = '-';
            current = E;
            continue;
        }
        // if north exists, add to path and make that current
        Location N = {current.room, current.row-1, current.col};
        if (locationIsMoveable(N) && backtrace[N.room][N.row][N.col] != '-') {
            pathLocation N_path(N, backtrace[N.room][N.row][N.col]);
            path.push_front(N_path);
            backtrace[N.room][N.row][N.col] = '-';
            current = N;
            continue;
        }
    }
    for (auto loc : path) {
        cout << loc << "\n";
    }

}
void castleMap::searchAlgorithm() {
    search.push_back(S);
    Location current;
    bool foundC = false;
    backtrace.resize(numRooms, vector<vector<char>>(lengthRoomSide, vector<char>(lengthRoomSide, '-'))); // Initialize with '.' or any default char
    backtrace[S.room][S.row][S.col] = 'S';
//    int i = 0;

    if (inputMode == 'L') {
        while (!search.empty()) {
//            ++i;

//            if(algorithm == "stack"){
                // Set Current
                current = search.back();
                search.pop_back();      // deleting element once we remove it off stack
//            }
//            if(algorithm == "queue"){
//                current = search.front();
//                search.pop_front();
//            }

            // found C
            if (current == C) {
                foundC = true;
                cout << "Found C: " << C << "\n";
                
                break;
            }
            cout << "Current: (" << current.room << "," << current.row << "," << current.col << "," << backtrace[current.room][current.row][current.col] << ")" << "\n";

            // TODO: include another if statement to not do this every iteration -- if (char != '.' etc.)
            // Warp Pipe; probably need to do some level of char->int int-> char conversion here
            uint32_t level = static_cast<uint32_t>(castleMap[current.room][current.row][current.col] - '0');
            // checks if level is between 0-9 and prevents infinite loop of warping back into same room
            if (level != current.room && level >= 0 && level <= 9) {
                // create new exit location
                Location exitLoc = {level, current.row, current.col};
                // check to see if warp pipe isn't taking you to a enemy or a solid wall and check if you've already been there
                if (locationIsMoveable(exitLoc) && locationNotVisited(exitLoc)) {
                    // TODO: do we need to do this? do we need to clear all elements from previous room?
                    //search.clear();
                    search.push_back(exitLoc);
                    backtrace[exitLoc.room][exitLoc.row][exitLoc.col] = 'p';
                    continue;
                }
                //do nothing
            }

            // Adding to Search
            if (current.row != 0) {
                // N: current row - 1
                // if row is not equal to 0
                Location N = {current.room, current.row - 1, current.col};
                // check to see if location is moveable
                if (locationIsMoveable(N) && locationNotVisited(N)) {
                    search.push_back(N);
                    backtrace[N.room][N.row][N.col] = 'n';
                }
            }
            if (current.row != lengthRoomSide-1) {
                // E: current col + 1
                Location E = {current.room, current.row, current.col + 1};
                if (locationIsMoveable(E) && locationNotVisited(E)) {
                    search.push_back(E);
                    backtrace[E.room][E.row][E.col] = 'e';
                }
            }
            if (current.row != lengthRoomSide-1) {
                // S: current row + 1; check to see if it's out of bounds
                Location So = {current.room, current.row + 1, current.col};
                if (locationIsMoveable(So) && locationNotVisited(So)) {
                    search.push_back(So);
                    backtrace[So.room][So.row][So.col] = 's';
                }
            }
            if (current.col != 0) {
                // W: current col - 1; check to see if it's out of bounds
                Location W = {current.room, current.row, current.col - 1};
                if (locationIsMoveable(W) && locationNotVisited(W)) {
                    search.push_back(W);
                    backtrace[W.room][W.row][W.col] = 'w';
                }
            }
//            cout << "-----------Search----------" << "\n";
//            for (auto i : search) {
//                cout << i;
//            }
//            if (i == 3) {
//                break;
//            }
        }
    }
    else if (inputMode == 'M') {
    }

    
    // Search has failed
    if (!foundC) {
        cout << "no path to rescue Countess" << "\n";
    }
}

void castleMap::readFromInput(const string &filename) {
    // creating stream object
    ifstream fin;
    fin.open(filename.c_str());
    if (!fin.is_open()) {
        std::cout << "open failed" << "\n";
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
            cout << line << "\n";
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

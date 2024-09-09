//
//  test.cpp
//  p1_countess
//
//  Created by Bends on 9/8/24.
//

#include <stdio.h>
#include "main.cpp"

using namespace std;

int main() {
    cout << "TESTING: readFromInput" << "\n";
    try {
        int switchval = 6;
        
        
        castleMap c;
        string filename;
        switch (switchval) {
            case 1:
                filename = "spec-L.txt";
                break;
            case 2:
                filename = "spec-pipe-L.txt";
                break;
            case 3:
                filename = "sample-Small-L.txt";
                break;
            case 4:
                filename = "spec-M.txt";
                break;
            case 5:
                filename = "spec-pipe-M.txt";
                break;
            case 6:
                filename = "sample-Small-M.txt";
                break;

        }
        c.readFromInput(filename);
    }
    catch (std::runtime_error &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    
    return 0;

}

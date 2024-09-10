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
        int switchval = 2;
        
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
                filename = "sample-Big-L.txt";
                break;
            case 5:
                filename = "sample-no-solution.txt";
                break;
            case 6:
                filename = "spec-M.txt";
                break;
            case 7:
                filename = "spec-pipe-M.txt";
                break;
            case 8:
                filename = "sample-Small-M.txt";
                break;
            case 9:
                filename = "sample-Big-M.txt";
                break;
        }
        c.readFromInput(filename);
        c.searchAlgorithm();
    }
    catch (std::runtime_error &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    
    return 0;

}

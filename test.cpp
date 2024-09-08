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
    try {
        castleMap c;
        string filename = "spec-pipe-L.txt";
        c.readFromInput(filename);
    }
    catch (std::runtime_error &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    
    return 0;

}

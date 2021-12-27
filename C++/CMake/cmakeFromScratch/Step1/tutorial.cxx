#include <iostream>

#include "lib/MathFunctions/Operation.hpp"

using namespace std;

int main(int argc, char **argv) {
    cout << "Howdy\n";
    if (argc > 1) {
        cout << argv[1] << endl;

        const double inputValue = stod(argv[1]);
        cout << inputValue << endl;
    }

    Operation anOp;
    int i = 3;
    int j;
    j = anOp.sum(i, i);
    cout << j << endl;

    return 0;
}

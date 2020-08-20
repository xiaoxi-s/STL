#include <iostream>
#include <vector>
#include <string>

using namespace std;

int hello() {
    vector<string> msg {"Hello", "C++", "World", "from", "VS Code", "and the C++ extension!"};

    for (const string& word : msg) {
        cout << word << " ";
    }

    cout << endl;

    return 0;
}
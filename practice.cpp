// includes all standard libraries
#include <iostream>
#include <stdio.h>
#include <vector>
#include <algorithm>
#define GET_VARIABLE_NAME(Variable) (#Variable)
using namespace std;

int main() {
    vector<int> my_list;
    for (int i = 0; i < 10; i ++) {
        my_list.push_back(i * 23);
    }
    auto to_be_rm = remove_if(my_list.begin(), my_list.end(), [](int ele) {return ele == 46; });
    my_list.erase(to_be_rm, my_list.end());
    for (int i = 0; i < my_list.size(); i++) {
        cout << my_list[i] << endl;

    }
    // char * time;
    // cin >> time;
    // string *pointer;
    // pointer = strtok(time, ":");
    // int hours = stoi(pointer)
    // pointer = strtok(NULL, ":");
    
    return 0;
}
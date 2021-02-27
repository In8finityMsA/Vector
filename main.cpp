#include <iostream>
#include "Vector.h"
#include <vector>
#include <string>

using namespace std;

int main() {

    const int n = 10000;
    try {
        Vector<int> intv0{1,3,5,6,2}; //Constr init list

        ///Pushback check
        Vector<int> intv1; //Constr empty
        for (int i = 0; i < n; i++) {
            intv1.push_back(i); //Copy pushback
        }

        cout << "Size: " << intv1.size() << endl;
        cout << "Capacity: " << intv1.capacity() << endl;
        cout << "Insert PopBack" << endl;
        intv1.insert(n,-1);
        cout << "Insert from index" << endl;
        intv1.insert(2, 6384,-100);
        cout << "Capacity: " << intv1.capacity() << endl;
        cout << "Elements index 1: " << intv1[1] << endl;
        cout << "Elements index 2: " << intv1[2] << endl;
        cout << "Elements index 1337: " << intv1[1337] << endl;
        cout << "Elements index 2 + 6384 - 1 (last inserted): " << intv1[2 + 6384 - 1] << endl;
        cout << "Elements index 2 + 6384 (next after last inserted): " << intv1[2 + 6384] << endl;
        cout << "Elements index (100th after last inserted): " << intv1[2 + 6384 + 100] << endl;
        cout << "Elements index 10000: " << intv1[10000] << endl;
        cout << "Elements index 9999: " << intv1[10000 - 1] << endl;
        cout << "Elements index size - 2 (prev last): " << intv1[intv1.size() - 2] << endl;
        cout << "Elements index size - 1 (last): " << intv1[intv1.size() - 1] << endl;
        cout << "Size: " << intv1.size() << endl;
        cout << "PopBack" << endl;
        intv1.pop_back();
        cout << "Size: " << intv1.size() << endl;
        cout << "Elements index size - 2 (prev last): " << intv1[intv1.size() - 2] << endl;
        cout << "Elements index size - 1 (last): " << intv1[intv1.size() - 1] << endl;
        cout << "Elements index size (after last): " << intv1[intv1.size()] << endl;

        cout << "Erase 2 elemets (9996, 9997)" << endl;
        intv1.erase(intv1.size() - 4, 2);
        cout << "Elements index size - 4: " << intv1[intv1.size() - 4] << endl;
        cout << "Elements index size - 3: " << intv1[intv1.size() - 3] << endl;
        cout << "Elements index size - 2 (prev last): " << intv1[intv1.size() - 2] << endl;
        cout << "Elements index size - 1 (last): " << intv1[intv1.size() - 1] << endl;
        cout << "Elements index size (after last): " << intv1[intv1.size() - 2] << endl;

        cout << "Resize with + 10 elements (elem = 12)" << endl;
        cout << "Size: " << intv1.size() << endl;
        intv1.resize(intv1.size() + 10, 12);
        cout << "Size: " << intv1.size() << endl;
        cout << "Elements index size - 11 (prev before last inserted): " << intv1[intv1.size() - 11] << endl;
        cout << "Elements index size - 10 (last inserted): " << intv1[intv1.size() - 10] << endl;
        cout << "Elements index size - 2 (prev last): " << intv1[intv1.size() - 2] << endl;
        cout << "Elements index size - 1 (last): " << intv1[intv1.size() - 1] << endl;
        cout << "Capacity: " << intv1.capacity() << endl;
        cout << "Shrink to Fit" << endl;
        intv1.shrink_to_fit();
        cout << "Capacity: " << intv1.capacity() << endl;
        /*intv1.clear();
        //intv1.erase(0, intv1.size());
        cout << intv1.size() << endl;
        cout << intv1[16384 - 5] << endl;
*/
        ///Massive copy
        Vector<int> intv2(intv1); //Constr copy

        /*
        ///Pushback check nonIntegral
        Vector<string> st1; //Constr empty
        for (int i = 0; i < n; i++) {
            st1.push_back(std::to_string(i)); //Move pushback
        }
        ///Massive copy nonIntegral
        Vector<string> st2(st1); //Constr copy
        cout << st2[289] << endl;

        ///Move check
        Vector<int> intv3 = {1,3,4}; //Constr init list
        intv3 = Vector<int>(5, 167); //Assign move + n,elem
        auto intv4(std::move(intv3)); //Constr move
        intv4 = std::move(intv4); //Assign move (self)
        cout << intv4[0] << endl;
        //cout << "Size intv3: " << intv3.size() << endl;

        for (int i = 1; i < intv3.size(); i++) {
            cout << "Size intv3: " << intv3.size() << " " << i << endl;
            std::cout << intv3[i] << endl;
            intv3[i-1] = intv3[i];
            intv3[3] = 300;
        }
        std::cout << endl;
        for (int i = 0; i < intv3.size(); i++) {
            cout << "Size intv3: " << intv3.size() << endl;
            std::cout << intv3[i] << endl;
        }*/

        /*///Check of not integral types
        vector<string> vec0{"fhh","hfhhh","ghfdbfd","rtnnbt","bfbfbngfnn"};
        vector<string> vec1{"fhffh","hfjhhhh","gdbfd","rtbt","bfbhjhjgfnn"};
        vector<string> vec2{"fdhh","hfh","ghfdghjhbfd","rtnnbt","bfbffnn"};
        Vector<vector<string>> strv{vec0, vec1, vec2}; //init list
        Vector<vector<string>> strv1(strv); //
        for (int i = 0; i < strv1.size(); i++) {
            for (int j = 0; j < strv1[i].size(); j++) {
                std::cout << strv1[i][j] << endl;
            }
            cout << endl;
        }
        strv[0][0] = "hello";
        cout << strv[0][0] << endl;
        cout << vec0[0] << endl;*/
    }
    catch (std::exception& e) {
        std::cout << e.what() << std::endl;
    }
    catch (...) {
        std::cout << "Exception";
    }


    return 0;
}

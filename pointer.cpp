#include <iostream>
using namespace std;
int main(){
    int *x[10];
    int y = 34;
    x[0] = &y;
    cout << x[0] << " " << *x[0] << " " << &x[0] << endl;
    return 0;
}
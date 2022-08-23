#include <iostream>
#include <Windows.h>

using namespace std;

void theoryFormula(int x) {

    int i, n, buff_1 = 0, buff_2 = 0;

    for (i = 1; i <= x; i++) {

        n = i;
        cout << "Starting check of '3n + 1' theory for " << n << ":\n" << endl;

        do {

            cout << n << "\t --> \t";

            if (n % 2 != 0) {

                n = 3 * n + 1;
                buff_1++;
                cout << n << "\t | Odd, multiply by 3 and add 1." << endl;

            }
            else {

                n *= 0.5;
                buff_2++;
                cout << n << "\t | Even, divide by 2." << endl;

            }

        } while (n != 1);

        cout << "\nTheory of '3n + 1' for " << i << " ended!" << endl;
        cout << "Acts: " << buff_2 + buff_1 << ". Divides: " << buff_2 << ". Multiplies: " << buff_1 << ".\n" << endl;
        buff_1 = buff_2 = 0;

    }

}

int main() {

    int checkTo = 1;

    cout << "You want check numbers from 1 to... ";
    cin >> checkTo;
    cout << endl;

    theoryFormula(checkTo);

    system("pause");

}

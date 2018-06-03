#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <climits>
#include <iterator>

using namespace std;




int main()
{
    int N ;
    cin >> N; cin.ignore();
    int* strengths = new int[N];

    for (int i = 0; i < N; i++) {
        int strength; cin >> strength; cin.ignore();
        strengths[i] = strength;
    }

//    int* strengths = new int[3] {5, 8, 9};

    ostream_iterator<int> output(cerr, " ");
    std::copy(strengths, strengths + N, output);

    cerr << "\n--------------sorted-------------------\n";

    sort(strengths, strengths + N);
    std::copy(strengths, strengths + N, output);

    int closest_diff = strengths[1] - strengths[0];

    for (int i = 1; i < N - 1; ++i) {
        int diff = strengths[i + 1] - strengths[i];

        if(diff < closest_diff) {
            closest_diff = diff;
        }
    }


    cout << closest_diff << endl;
}
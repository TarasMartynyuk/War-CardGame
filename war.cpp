#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <queue>
#include <sstream>

using namespace std;
//region defs

bool lexical_cast(const std::string& s, int& i);

const string kCardPowers[13]{
    "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A"
};

int compare(const string&, const string&);

queue<string> readPlayerCards();

class Card
{
public:
    Card(const string& str)  {

    }

private:
    bool isDigit;
    int digitValue;
    char charValue;

};

//endregion

int main()
{

    int i = -1;
    cout << lexical_cast("10", i);
    cout << "\n" << i;

    return 0;

    int m; // the number of cards for player 2
    cin >> m; cin.ignore();
    for (int i = 0; i < m; i++) {
        string cardp2; // the m cards of player 2
        cin >> cardp2; cin.ignore();
    }

    // Write an action using cout. DON'T FORGET THE "<< endl"
    // To debug: cerr << "Debug messages..." << endl;

    cout << "PAT" << endl;
}


queue<string> readPlayerCards(){
    int n; // the number of cards for player 1
    cin >> n; cin.ignore();
    for (int i = 0; i < n; i++) {
        string cardp1; // the n cards of player 1
        cin >> cardp1;
        cin.ignore();
    }
}

bool lexical_cast(const std::string& s, int& i) {
    std::stringstream ss(s);

    int test;
    if((ss >> test).fail() || !(ss >> std::ws).eof()) {
        return false;
    }
    i = test;
    return true;
}
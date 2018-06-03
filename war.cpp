#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <queue>
#include <sstream>
#include <cassert>
#include <iterator>

using namespace std;
//region defs

class Card;
bool lexical_cast(const std::string& s, int& i);

const string kCardPowers[13]{
    "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A"
};

enum Value {
    Two, Three, Four, Five, Six, Seven, Eight, Nine, Ten, J, Q, K, A
};

string ValueToString(Value value);

deque<Card> readPlayerCards();

class Card
{
public:
    Card(const string& str)  {
        string valueStr = str.substr(0, str.size() -1);

//        cerr  <<  str << " : "  << valueStr << endl;
        assert(valueStr.size() <= 2);

        int digit_value = -1;
        bool is_digit = lexical_cast(valueStr, digit_value);

        assert(digit_value >= 2 && digit_value <= 10 || !is_digit);
        value = is_digit ? static_cast<Value>(digit_value - 2) : parseChar(valueStr[0]);
    }

    int compareTo(const Card other) {
    }

    const Value& getValue()const {return value; }

    Card& operator=(const Card& other) = default;
private:
    Value value;
    Value parseChar(char ch) {
        switch (ch) {
            case 'J':
                return Value::J;
            case 'Q':
                return Value::Q;
            case 'K':
                return Value::K;
            case 'A':
                return Value::A;
            default:
                throw invalid_argument("can't convert char {" + to_string(ch) + "} to Value");
        }
    }
};
ostream& operator<< (ostream&, const Card&);

//endregion

int main()
{

    auto first_deck = readPlayerCards();
    auto second_deck = readPlayerCards();

    ostream_iterator <Card> output(cerr, " ");
    cerr << "--------first-----------\n";
    std::copy(first_deck.begin(), first_deck.end(), output);
    cerr << "\n--------second-----------\n";
    std::copy(second_deck.begin(), second_deck.end(), output);


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


deque<Card> readPlayerCards(){
    int n; cin >> n; cin.ignore();

    deque<Card> cards;
    for (int i = 0; i < n; i++) {
        string card_str; cin >> card_str; cin.ignore();
        cards.push_back(Card(card_str));
    }

    return cards;
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

ostream& operator<<(ostream& os, const Card& card) {
    os << ValueToString(card.getValue());
}

string ValueToString(Value value){

    int int_val = (int) value;
    if(int_val <= 8) {
        return
        to_string(int_val
        + 2);
    } else {
        switch (value) {
            case J:
                return "J";
            case Q:
                return "Q";
            case K:
                return "K";
            case A:
                return "A";
            default:
                throw invalid_argument("ValueToString error");
        }
    }
}
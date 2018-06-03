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
// "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A"
enum Value {
    Two, Three, Four, Five, Six, Seven, Eight, Nine, Ten, J, Q, K, A
};
string ValueToString(Value value);
enum State {
    FirstWins, SecondWins, Pat, War, Battle
};
bool isEndGameState(State state);
string endGameToString(State state);

deque<Card> readPlayerCards();
void print (const deque<Card>&, const deque<Card>&);
Card pop_front(deque<Card>&);

deque<Card> mockFirst();
deque<Card> mockSecond();

//endregion
//region struct

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
        int this_val = (int) getValue();
        int other_val = (int) other.getValue();

        if(this_val == other_val) {
            return 0;
        } else if (this_val > other_val) {
            return 1;
        } else {
            return -1;
        }
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
//region func

State doBattle(
    deque<Card>& first_deck,
    deque<Card>& second_deck,
//    bool is_war,
    queue<Card>& first_war_cards,
    queue<Card>& second_war_cards)
{
    if(first_deck.empty()) {
         return State::SecondWins;
    }
    if(second_deck.empty()) {
        return State::FirstWins;
    }

    Card first_card = pop_front(first_deck);
    Card second_card = pop_front(second_deck);

    int comp = first_card.compareTo(second_card);

    //starging war
    if(comp == 0) {
        first_war_cards.push(first_card);
        second_war_cards.push(second_card);
        return State::War;
    } else {
        if(comp > 0) { // first player won the battle
            first_deck.push_back(first_card);
            first_deck.push_back(second_card);
        } else { // second won
            second_deck.push_back(first_card);
            second_deck.push_back(second_card);
        }
        return State::Battle;
    }
}

string playWar(deque<Card>& first_deck, deque<Card>& second_deck) {

    queue<Card> first_war_deck;
    queue<Card> second_war_deck;
    while(true) {
        // try battle
        auto postBattleState = doBattle(first_deck, second_deck,
            first_war_deck, second_war_deck);

        if(isEndGameState(postBattleState)) {
            return endGameToString(postBattleState);
        }
    }
}
//endregion

int main()
{
//    auto first_deck = readPlayerCards();
//    auto second_deck = readPlayerCards();
    auto first_deck = mockFirst();
    auto second_deck = mockSecond();

    print(first_deck, second_deck);
//    cerr << second_deck.front().compareTo(second_deck.front());

    cout << playWar(first_deck, second_deck)<< endl;
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

void print(const deque<Card>& deque1, const deque<Card>& deque2) {
    ostream_iterator<Card> output(cerr, " ");
    cerr << "--------first-----------\n";
    std::copy(deque1.begin(), deque1.end(), output);
    cerr << "\n--------second-----------\n";
    std::copy(deque2.begin(), deque2.end(), output);
    cerr << "\n";
}

Card pop_front(deque<Card>& deck) {
    assert(! deck.empty());

    Card top = deck.front();
    deck.pop_front();
    return top;
}

bool isEndGameState(State state) {
    switch (state) {
        case State::FirstWins:
        case State::SecondWins:
        case State::Pat:
            return true;
        default:
            return false;
    }
}

string endGameToString(State state) {
    switch (state) {
        case State::FirstWins:
            return "1";
        case State::SecondWins:
            return "2";
        case State::Pat:
            return "PAT";
        default:
            throw invalid_argument("not an end state");
    }
}

deque<Card> mockFirst() {
    return deque<Card> {
        Card("AZ"),
        Card("KZ"),
        Card("QZ")
    };
}

deque<Card> mockSecond() {
    return deque<Card>{
        Card("KZ"),
        Card("QZ"),
        Card("JZ")
    };
}
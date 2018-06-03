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
struct GameDecks;

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

void readPlayerDeck(deque<Card>&);

void print(GameDecks&);
Card pop_return(queue<Card>&);

void mockDecks(GameDecks&);
void mockDecksWar(GameDecks&);
bool checkIfNoMoreCards(GameDecks&, State& whoWon);

State drawToWarQueues(GameDecks& decks);
void pipe(deque<Card>&, queue<Card>&);
//endregion
//region struct

struct GameDecks
{
    queue<Card> first_deck;
    queue<Card> second_deck;
    queue<Card> first_war_cards;
    queue<Card> second_war_cards;
};

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
//region algorithm

// can return end or war states
State doBattle(GameDecks& decks)
{
    State whoWon = State::Battle;
    if(checkIfNoMoreCards(decks, whoWon)) {
        return whoWon;
    }

    Card first_card = pop_return(decks.first_deck);
    Card second_card = pop_return(decks.second_deck);

    int comp = first_card.compareTo(second_card);

    //starting war
    if(comp == 0) {
        decks.first_war_cards.push(first_card);
        decks.second_war_cards.push(second_card);
        return State::War;
    } else {
        if(comp > 0) { // first player won the battle
            decks.first_deck.push(first_card);
            decks.first_deck.push(second_card);
        } else { // second won the battle
            decks.second_deck.push(first_card);
            decks.second_deck.push(second_card);
        }
        return State::Battle;
    }
}
// can return end or battle states
State doWar(GameDecks& decks)
{
    while (true) {
        State after_draw = drawToWarQueues(decks);
        if (isEndGameState(after_draw)) {
            assert(after_draw == State::Pat);
            return State::Pat;
        }

        assert(after_draw == State::War);
        //battle
        State _;
        if (checkIfNoMoreCards(decks, _)) {
            return State::Pat;
        }

        Card first_card = pop_return(decks.first_deck);
        decks.first_war_cards.push(first_card);
        Card second_card = pop_return(decks.second_deck);
        decks.second_war_cards.push(second_card);

        int comp = first_card.compareTo(second_card);
        // continue the war
        if (comp > 0) {
            // add both war qs to first player

        } else if (comp < 0) {
            // add both war queues to second player
        }

        assert(after_draw == State::War);
    }
}

// can return end or war states
State drawToWarQueues(GameDecks& decks) {
    for (int i = 0; i < 3; ++i) {
        State _;
        if(checkIfNoMoreCards(decks, _)) {
            return State::Pat;
        }

        decks.first_war_cards.push(pop_return(decks.first_deck));
        decks.second_war_cards.push(pop_return(decks.second_deck));
    }
    return State::War;
}

string playWar(GameDecks& decks) {

    while(true) {
        // try battle
        auto postBattleState = doBattle(decks);

        if (isEndGameState(postBattleState)) {
            return endGameToString(postBattleState);
        }

        if(postBattleState == State::War) {
            doWar(decks);
        }
    }
}

//void drawToWarQueuesOnce(GameDecks& decks) {
//
//}
//endregion

int main()
{
    GameDecks decks;
//    readPlayerDeck(decks.first_deck);
//    readPlayerDeck(decks.second_deck);
//    mockDecks(decks);
    mockDecksWar(decks);


    print(decks);
//    cerr << second_deck.front().compareTo(second_deck.front());

    cout << playWar(decks) << endl;
}

//region impls

void readPlayerDeck(deque<Card>& deck) {
    int n; cin >> n; cin.ignore();

    for (int i = 0; i < n; i++) {
        string card_str; cin >> card_str; cin.ignore();
        deck.push_back(Card(card_str));
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

ostream& operator<<(ostream& os, const Card& card) {
    os << ValueToString(card.getValue());
    return os;
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

void print(GameDecks& decks) {

    queue<Card> first_copy, second_copy;
    cerr << "--------first-----------\n";
    while(! decks.first_deck.empty()) {
        cerr << decks.first_deck.front() << " ";
        first_copy.push(decks.first_deck.front());
        decks.first_deck.pop();
    }

    cerr << "\n--------second-----------\n";
    while (! decks.second_deck.empty()) {
        cerr << decks.second_deck.front() << " ";
        second_copy.push(decks.second_deck.front());
        decks.second_deck.pop();
    }
    cerr << "\n";
}

Card pop_return(queue<Card>& q) {
    assert(! q.empty());

    Card top = q.front();
    q.pop();
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

void mockDecks(GameDecks& decks) {
    decks.first_deck.push(Card("AZ"));
    decks.first_deck.push(Card("KZ"));
    decks.first_deck.push(Card("QZ"));

    decks.second_deck.push(Card("KZ"));
    decks.second_deck.push(Card("QZ"));
    decks.second_deck.push(Card("JZ"));
}
// passing the whole struct to minimize the code
bool checkIfNoMoreCards(GameDecks& decks, State& whoWon) {
    bool first_lost = decks.first_deck.empty();
    bool second_lost = decks.second_deck.empty();

    if(first_lost && second_lost) {
        whoWon = State::Pat;
        return true;
    } else if (second_lost) {
        whoWon = State::FirstWins;
        return true;
    } else if (first_lost) {
        whoWon = State::SecondWins;
        return true;
    }
    return false;
}

void mockDecksWar(GameDecks& decks) {
    //10D 9S 8D KH 7D 5H 6S
    decks.first_deck = queue<Card>({
        Card("10D"),
        Card("9S"),
        Card("8D"),
        Card("KH"),
        Card("7D"),
        Card("5H"),
        Card("6S")
    });
    //10H 7H 5C QC 2C 4H 6D
    decks.second_deck = queue<Card>({
        Card("10H"),
        Card("7H"),
        Card("5C"),
        Card("QC"),
        Card("2C"),
        Card("4H"),
        Card("6D")
    });
}

void pipe(deque<Card>& to, queue<Card>& from) {
    while(! from.empty()) {
        to.push_back(pop_return(from));
    }
}

//endregion
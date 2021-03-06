/* ****Card class and Hand derived class required for Poker game */

#ifndef CARDGAME_H_INCLUDED
#define CARDGAME_H_INCLUDED

#include <iomanip>
#include <vector>
#include <tuple>
#include <string>
#include <iostream>
#include <algorithm>

using namespace std;

class Card
{
public:
    int rankInt;
    int suitInt;
    //Constructor
    Card(){}

    //Mutator
    void add(int rank, int suit);

    //Accessor
    string getRank(); //Translate numbers into names for K, Q, J, A
    string getSuit();
    string getCard(); //This is simply getRank + getSuit
};

class Hand : public Card
{
public:
    Hand();

    //Mutator
    void addCard(Card);
    void clear();

    //Accessor
    void showHand(); //Loop through vector and display contents using the getCard function
    string getScore();
    void sortHand(); //Sorts the hand by rank

private:
    vector<Card> myHand;
};

//Card class functions
void Card::add(int rank, int suit){//Mutator: Add a card
    if(rank >13 || rank < 1 || suit > 3 || suit < 0){
        cout << "Invalid card! Ending program.";
        exit(1);
    }
    else{
        rankInt = rank;
        suitInt = suit;
    }
}

string Card::getRank(){//Accessor: Translate rankInt into name strings
    switch (rankInt) {
        case 1:
            return "Ace";
            break;
        case 2:
            return "2";
            break;
        case 3:
            return "3";
            break;
        case 4:
            return "4";
            break;
        case 5:
            return "5";
            break;
        case 6:
            return "6";
            break;
        case 7:
            return "7";
            break;
        case 8:
            return "8";
            break;
        case 9:
            return "9";
            break;
        case 10:
            return "10";
            break;
        case 11:
            return "Jack";
            break;
        case 12:
            return "Queen";
            break;
        case 13:
            return "King";
            break;
        default:
            return "Invalid Rank";
            break;
    }
}

string Card::getSuit(){//Accessor: Convert suitInt to suit string
    switch (suitInt) {
        case 0:
            return "Hearts";
            break;
        case 1:
            return "Clubs";
            break;
        case 2:
            return "Diamonds";
            break;
        case 3:
            return "Spades";
            break;
        default:
            return "Invalid Suit";
            break;
    }
}

string Card::getCard(){//This is simply getRank + getSuit
    return (getRank() + " of " + getSuit());
}

//Hand class functions
Hand::Hand(){ //initializes an empty vector myHand
    vector<Card> cards;
    myHand = cards;
}

void Hand::addCard(Card newCard){ //Adds a card to the hand
    myHand.push_back(newCard);

}


void Hand::showHand(){ //Loop through vector using the getCard function
    for(int i=0; i < myHand.size()-1; i++) //This needs some help
        cout << myHand[i].getRank() << " of " << myHand[i].getSuit() << ", ";
    cout << myHand.back().getRank() << " of " << myHand.back().getSuit() << endl << endl;
}

string Hand::getScore(){ //Massive function that returns the score for the hand
    int straight = 1; //A single card is a straight with length=1
    int ofKindInt = 1; //A single card is 1 of a kind. Just like you. :)
    int flush = 1; //A single card is a flush of 1
    vector<tuple<int, string>> ofAKind; //Vector to hold tuples that include ofKindInt and card rank
    string result;
    bool aceHigh = false;

    //Evaluate hand
    for(int i=0; i<myHand.size()-1; i++){
        if(myHand[i].suitInt == myHand[i+1].suitInt) //Check for suit match. Probably only accurate when =5 & flush is present!
            flush++;
        if(myHand[i].rankInt+1 == myHand[i+1].rankInt){ //Check for straight
            straight++;
            ofKindInt = 1; //No longer a matching rank so ofKindInt counter must be reset
        }
        else if (myHand[i].rankInt == myHand[i+1].rankInt){ //Evaluate for ofKind. Cannot be straight and cannot be flush
            ofKindInt++;
            if(ofKindInt == 2) //Adds an ofAKind tuple and sets as pair
                ofAKind.push_back(make_tuple(ofKindInt,myHand[i].getRank()));
            else if(ofKindInt == 3) //Sets last ofAKind tuple to 3 of a kind
                ofAKind.back() = make_tuple(ofKindInt,myHand[i].getRank());
            else if(ofKindInt == 4) //Sets last ofAKind tuple to 4 of a kind
                ofAKind.back() = make_tuple(ofKindInt,myHand[i].getRank());
        }
         else{ //No match & no straight? Reset ofKindInt counter!
            ofKindInt = 1;
        }
    }
    if (myHand.front().rankInt == 1 && myHand.back().rankInt ==13){ //Handling for Aces high. King and Ace increments straight by 1
        straight++;
        aceHigh = true;
    }

    //Evaluate scoring
    if(straight==5){//Straight
        if((myHand.back().rankInt == 13) && (aceHigh == true))
            result = "Ace-high straight!";
        else
            result = myHand.back().getRank() + "-high straight!";
        if (flush==5)
            result = result + " flush!";
    }
    else if(flush==5)//Flush
        result = myHand.back().getSuit() + " flush!";

    else if(ofAKind.size()>0){ //Check ofAKind size to make sure it's not empty. Bad things happen if it's empty and these checks happen...
        if ((get<0>(ofAKind[0]) == 2)&&(ofAKind.size()==1))//Pair
            result = "Pair of " + get<1>(ofAKind[0]) + "s!";
        else if ((get<0>(ofAKind[0])) + (get<0>(ofAKind[1])) == 4) //Two Pair
            result = "Pair of " + get<1>(ofAKind[0]) + "s and pair of " + get<1>(ofAKind[1]) + "s!";
        else if ((get<0>(ofAKind[0]) == 3)&&(ofAKind.size()==1))//Three of a kind
            result = "Three " + get<1>(ofAKind[0]) + "s!";
        else if ((get<0>(ofAKind[0])==2) && (get<0>(ofAKind[1])==3)) //Full House, pair rank is low
            result = "Full house: " + get<1>(ofAKind[1]) + "s over " + get<1>(ofAKind[0]) + "s!";
        else if ((get<0>(ofAKind[0])==3) && (get<0>(ofAKind[1])==2)) //Full House, pair rank is high
            result = "Full house: " + get<1>(ofAKind[0]) + "s over " + get<1>(ofAKind[1]) + "s!";
        else if (get<0>(ofAKind[0]) == 4) //Four of a kind
            result = "Four " + get<1>(ofAKind[0]) + "s!";
        else
            result = "Something has gone horribly wrong :(";
    }
    else
        result = "You got nothing. Sad panda. :(";
    return result;
}

bool operator<(const Card &c1, const Card &c2) {return (c1.rankInt < c2.rankInt);} //used for sort

void Hand::sortHand(){ //Sorts the hand
    sort(myHand.begin(), myHand.end());
}

void Hand::clear(){ // Clears hand vector
    myHand.clear();
}

#endif // CARDGAME_H_INCLUDED

// Wheel_Of_Fortune.cpp : This file contains the 'main' function. Program execution begins and ends there.
// Sotiris Stylianou


#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <vector>
#include <memory>

using namespace std;

//class for players including their name, bank money, round money and seond chances
class players {
public:
    string name;
    int bank_money;
    int second_chances;
    int round_money;

    players(const string& players_name) : name(players_name), bank_money(0), round_money(0), second_chances(0) {};
};
const players player1("Jhon");
const players player2("Maria");

//main function of cslice
class CSlice {
public:
    int Type;
    int ammount;
    string description;

    CSlice() : Type(0), ammount(0), description("") {}
    CSlice(int s, int a, const string d) : Type(s), ammount(a), description(d) {};

    virtual void slicetype(players& player) const
    {cout << "Player landed on a regular slice type.";}
};

//functions to override cslice
class lose_turn : public CSlice {
public:
    lose_turn() : CSlice(2, 0, "LoseTurn") {};
    void slicetype(players& player) const override
    {
        cout << player.name << " loses turn" << endl;
        cout << "Next player's turn to play!!" << endl;
    }
};

class bankrupt : public CSlice {
public:
    bankrupt() : CSlice(3, 0, "Bankrupt") {};
    void slicetype(players& player) const override
    {
        player.second_chances = 0;
        player.round_money = 0;
        cout << player.name << " current round balance is: " << player.round_money << endl;
    }
};

class bankrupt_plus : public CSlice {
public:
    bankrupt_plus() : CSlice(4, 0, "Bankrupt+") {};
    void slicetype(players& player) const override
    {
        player.round_money = 0;
        player.bank_money = 0;
        player.second_chances = 0;
        cout << player.name << " rolls Bankrupt+ and loses everything " << endl;
    }
};

class second_chance : public CSlice {
public:
    second_chance() : CSlice(5, 0, "SecondChance") {};
    void slicetype(players& player) const override
    {
        player.second_chances++;
        cout << player.name << " rolls SecondChance" << endl;
        cout << player.name << "'s number of SecondChance tokens earned: " << player.second_chances << endl;
    }
};

class steal : public CSlice {
public:
    steal() : CSlice(6, 0, "Steal") {};
    void slicetype(players& player) const override
    {
        cout << "Player steals half of other players bank money";
    }
};
class jackpot : public CSlice {
public:
    jackpot() : CSlice(7, 0, "Jackpot") {};
    void slicetype(players& player) const override
    {
        player.bank_money = player.bank_money * 2;
        cout << player.name << " rolls Jackpot" << endl;
        cout << player.name << " doubles banked money" << endl;
        cout << player.name << " new bank money balance is " << player.bank_money << endl;
    }
};

//function for slics types
class Slice {
public:
    static unique_ptr<CSlice> createSlice(int type, int ammount, const string& description) {
        switch (ammount) {
        case 2: return make_unique<lose_turn>();
        case 3: return make_unique<bankrupt>();
        case 4: return make_unique<bankrupt_plus>();
        case 5: return make_unique<second_chance>();
        case 6: return make_unique<steal>();
        case 7: return make_unique<jackpot>();
        default: return make_unique<CSlice>(type, ammount, description);
        }
    }
};

//funciton to play the game
class wheel_of_fortune_game {
private:
    const int slices_number = 16;
    vector<unique_ptr<CSlice>> Wheel_Of_Fortune;
    const int rounds_number = 4;
    vector <string> words;

public:

    wheel_of_fortune_game() {
        load_wheel_of_fortune();
        load_rounds();
    }

    //load the slices of wheel of fortune
    void load_wheel_of_fortune() {
        ifstream inputFile("wheel.txt");
        if (inputFile.is_open()) {
            for (int i = 0; i < slices_number; i++) {
                int type, ammount;
                string description;
                inputFile >> type >> ammount;
                getline(inputFile, description);
                Wheel_Of_Fortune.push_back(Slice::createSlice(type, ammount, description));
            }
            inputFile.close();
        }
        else {
            cout << "Unable to open file wheel.txt" << endl;
        }
    }

    //load the words of rounds.txt
    void load_rounds() {
        ifstream inFile("rounds.txt");
        string word;
        if (inFile.is_open()) {
            while (inFile >> word) {
                words.push_back(word);
            }
            inFile.close();
        }
        else {
            cout << "Unable to open file rounds.txt" << endl;
        }
    }
    //class for rounds to be played 4 times
    void round() {
        cout << endl << "Welcome to WheelOfFortune - ish";
        players player1("Jhon");
        players player2("Maria");
        for (int i = 0; i < rounds_number; i++)
        {
            play_round(i, player1, player2);
        }
    }
    //class to play each round
    void play_round(int round_number, players& player1, players& player2)
    {
        cout << endl << "Round " << round_number + 1 << " - Targeted word : " << words[round_number];
        cout << endl << "Players: " << player1.name << " and " << player2.name << endl;

        int current_player;
        if (round_number % 2 == 0) {
            current_player = 1;
        }
        else {
            current_player = 2;
        }

        string players_name = "";
        bool word_found = false;
        int last_rolled_slice = 0;
        player1.round_money = 0;
        player2.round_money = 0;

        vector<bool> letters_revealed(words[round_number].length(), false);
        vector<char> letters_used;

        srand(5);

        //while to check if the word is found
        while (!word_found) {
            if (current_player == 1)
            {
                players_name = player1.name;
            }
            else if (current_player == 2)
            {
                players_name = player2.name;
            }

            int rolled_number = rand() % (slices_number);
            cout << players_name << " rolls " << rolled_number + 1 << endl;
            cout << players_name << " rolls " << Wheel_Of_Fortune[rolled_number]->description << endl;

            //check if the slice type is lose turn so player can lose his/her turn
            if (Wheel_Of_Fortune[rolled_number]->Type == 2)
            {
                lose_turn loseTurnSlice;
                if (current_player == 1)
                {
                    loseTurnSlice.slicetype(player1);
                }
                else if (current_player == 2)
                {
                    loseTurnSlice.slicetype(player2);
                }
                current_player = 3 - current_player;
                continue;
            }

            //check if the slice type is bankrupt so the player loses his turn round money and account money
            if (Wheel_Of_Fortune[rolled_number]->Type == 3) {
                bankrupt bankruptSlice;
                if (current_player == 1)
                {
                    bankruptSlice.slicetype(player1);
                }
                else if (current_player == 2)
                {
                    bankruptSlice.slicetype(player2);
                }
                current_player = 3 - current_player;
                continue;
            }

            //check if the slice type is bankrupt+ so the player loses his turn round money, account balance money and account money
            if (Wheel_Of_Fortune[rolled_number]->Type == 4) {
                bankrupt_plus bankruptPlusSlice;
                if (current_player == 1)
                {
                    bankruptPlusSlice.slicetype(player1);
                    
                }
                else if (current_player == 2)
                {
                    bankruptPlusSlice.slicetype(player2);
                }
                current_player = 3 - current_player;
                continue;
            }
            // check if the letter is already chosen
            char chosen_letter;
            bool chosen_letter_found;
            do {
                chosen_letter = 'a' + rand() % 26;
                chosen_letter_found = false;
                for (char letter : letters_used)
                {
                    if (letter == chosen_letter)
                    {
                        chosen_letter_found = true;
                        break;
                    }
                }
            } while (chosen_letter_found);

            cout << players_name << " guesses " << chosen_letter << endl;
            letters_used.push_back(chosen_letter);


            bool letter_found = false;
            int revealed_letters_in_round = 0;
            //count the letters to turn them to revealed and count how many letters are revealed
            for (int j = 0; j < words[round_number].length(); j++) {
                if ((chosen_letter == words[round_number][j]) && !letters_revealed[j]) {
                    letters_revealed[j] = true;
                    letter_found = true;
                    revealed_letters_in_round++;
                }
            }

            int number_of_revealed = 0;
            //check if a letter isfound
            if (letter_found) {
                for (bool True : letters_revealed) {
                    if (True) {
                        number_of_revealed++;
                    }
                }
                cout << players_name << " reveals " << revealed_letters_in_round << endl;

                int round_earnings = revealed_letters_in_round * Wheel_Of_Fortune[rolled_number]->ammount;
                int money_earned_jhon = 0;
                int money_earned_maria = 0;

                //check if the slice type is second chance so the player can gain a secondchance token
                if (Wheel_Of_Fortune[rolled_number]->Type == 5)
                {
                    second_chance secondChanceSlice;
                    if (current_player == 1)
                    {
                        secondChanceSlice.slicetype(player1);
                    }
                    else if (current_player == 2)
                    {
                        secondChanceSlice.slicetype(player2);
                    }
                    continue;
                }

                //check if the slice type is steal so the player can steal half of the other players bank money
                if (Wheel_Of_Fortune[rolled_number]->Type == 6)
                {
                    if (current_player == 1)
                    {
                        int stolen_amount = player2.bank_money * 0.5;
                        player1.bank_money += stolen_amount;
                        player2.bank_money -= stolen_amount;
                        cout << players_name << " rolls Steal" << endl;
                        cout << players_name << " steals " << stolen_amount << " from " << player2.name << endl;
                    }
                    else if (current_player == 2)
                    {
                        int stolen_amount = player1.bank_money * 0.5;
                        player2.bank_money += stolen_amount;
                        player1.bank_money -= stolen_amount;
                        cout << players_name << " rolls Steal" << endl;
                        cout << players_name << " steals " << stolen_amount << " from " << player1.name << endl;
                    }
                }

                //check if the slice type is jackpot so the player doubles his bank money
                if (Wheel_Of_Fortune[rolled_number]->Type == 7)
                {
                    jackpot jackpotSlice;
                    if (current_player == 1)
                    {
                        jackpotSlice.slicetype(player1);
                    }
                    else if (current_player == 2)
                    {
                        jackpotSlice.slicetype(player2);
                    }
                    continue;
                }

                // add round earnings to round money
                if (current_player == 1)
                {
                    money_earned_jhon += round_earnings;
                    player1.round_money += round_earnings;
                    cout << player1.name << " earns " << money_earned_jhon << endl;
                }
                else if (current_player == 2)
                {
                    money_earned_maria += round_earnings;
                    player2.round_money += round_earnings;
                    cout << player2.name << " earns " << money_earned_maria << endl;
                }
            }

            //check if letter is not found
            if (!letter_found) {
                //check if player1 has any seondchance tokens
                if (current_player == 1 && player1.second_chances > 0)
                {
                    player1.second_chances--;
                    cout << player1.name << " uses SecondChance token" << endl;
                    cout << player1.name << "'s remaining SecondChance tokens: " << player1.second_chances << endl;
                    continue;
                }
                //check if player2 has any seondchance tokens
                else if (current_player == 2 && player2.second_chances > 0) {
                    player2.second_chances--;
                    cout << player2.name << " uses SecondChance token" << endl;
                    cout << player2.name << "'s remaining SecondChance tokens: " << player2.second_chances << endl;
                    continue;
                }
                cout << players_name << " loses turn due to inappropriate letter choice" << endl;
                cout << "Next player's turn to play!!" << endl;
                current_player = 3 - current_player;
            }

            //check if all the letters are revealed
            word_found = true;
            for (bool revealed : letters_revealed) {
                if (!revealed) {
                    word_found = false;
                    break;
                }
            }

            //check if the word is found
            if (word_found) {
                cout << "Game Over" << endl;
                cout << endl << players_name << " has won the round" << endl;
                //check the winner to add round money to his bank money
                if (current_player == 1)
                {
                    player1.bank_money += player1.round_money;
                    cout << endl << player1.name << " wins round and banks: " << player1.round_money << endl;
                }
                else if (current_player == 2)
                {
                    player2.bank_money += player2.round_money;
                    cout << endl << player2.name << " wins round and banks: " << player2.round_money << endl;
                }
                break;
            }
        }

        //output the total bank money results
        cout << endl << player1.name << "'s total banked amount is: " << player1.bank_money << endl;
        cout << player2.name << "'s total banked amount is: " << player2.bank_money << endl;

    }

};

int main()
{
    //calling the wheel_of_fortune and game functions
    wheel_of_fortune_game game;
    game.round();

}

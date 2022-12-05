#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

#define RPS_OPTIONS 3
/*
    1st Level is opponent's move:           0 = Rock , 1 = Paper , 2 = Scissor
    2nd Level is player's move:             0 = Rock , 1 = Paper , 2 = Scissor
    Contents refer to which side wins:      0 = opponent , 1 = draw , 2 = player
    Ex. opponent is Paper, player is Rock:  result = table[1][0] = 0 (Opponent Wins)
*/
int RPS_Scoring_Table[RPS_OPTIONS][RPS_OPTIONS] = { {1, 2, 0}, 
                                                    {0, 1, 2}, 
                                                    {2, 0, 1} };
#define OPPONENT_BASE 'A'
#define PLAYER_BASE   'X'
#define OUTCOME_BASE  'X'

// 1st entry was opponent's move, 2nd was player's
int assumption1(int opponent_move, int player_move) {
    switch (RPS_Scoring_Table[opponent_move][player_move]) {
        case 0:
            return (player_move+1 + 0);
        case 1:
            return (player_move+1 + 3);
        case 2:
            return (player_move+1 + 6);
    } return 0;   // to not get a compiler warning
}

// 1st entry was opponent's move, 2nd was the final outcome
int assumption2(int opponent_move, int outcome) {
    int i = 0;
    for (i; i < RPS_OPTIONS; i++) {
        if (RPS_Scoring_Table[opponent_move][i] == outcome) {
            break;
        }
    }
    switch (outcome) {
        case 0:
            return (i+1 + 0);
        case 1:
            return (i+1 + 3);
        case 2:
            return (i+1 + 6);
    } return 0;   // to not get a compiler warning
}

void totalScore(ifstream* stream) {
    int total_score1 = 0, total_score2 = 0;
    char first_entry, second_entry;
    while (*stream >> first_entry >> second_entry) {    // iterate over all rounds
        total_score1 += assumption1(first_entry-OPPONENT_BASE, second_entry-PLAYER_BASE);
        total_score2 += assumption2(first_entry-OPPONENT_BASE, second_entry-OUTCOME_BASE);
    }
    cout << "Score using Assumption 1: " << total_score1 << " & using Assumption 2: " << total_score2 << endl;
}

int main() {
    ifstream fstream;
    fstream.open("rock_paper_scissor_data.txt", ios::in);
    if (!fstream.is_open()) { return 1; }
    totalScore(&fstream);
    return 0;
}
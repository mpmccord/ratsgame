// randomly can find items on the board
// start off with one of each, can find others randomly
// shows stats
#include <iostream>
#include <ctime>
#include <cstdlib>

const int COL = 15;
struct Cheese {
    int x = 0;
    int y = 0;
    char symbol = 'C';
    const int RANGE = 2;
    const int STUNS = 65;
    const int STUNS_NOSEPLUGS = 30;
    bool ignore = false;
};
struct SmartRats {
    int x = 0;
    int y = 0;
    int prev_x = 0;
    int prev_y = 0;
    char symbol = 'S';
    const int ATTACK_RANGE = 1;
    const int BITE_NO_SHIELD = 85;
    const int BITE_SHIELD = 15;
    bool ignore = false;
};
struct DumbRats {
    int x = 0;
    int y = 0;
    int prev_x = 0;
    int prev_y = 0;
    char symbol = 'D';
    const int ATTACK_RANGE = 3;
    const int CHANCE_BITE = 30;
    bool ignore = false;
};
struct NoLs {
    const int MAX = 5;
    int numNOLs = 1;
    bool use = false;
};
struct HZ {
    const int MAX = 5;
    int numHZ = 1;
    bool use = false;
    const int RANGE = 1;
    const int CHANCE_WORKS = 70;
};
struct TrashCanShields {
    const int MAX = 1;
    const int MAX_BITES = 3;
    int numBites = 0;
    bool use = false;
    int numShields = 1;
};
struct ExplodingKitties {
    const int MAX = 6;
    const int RANGE = 3;
    int numEK = 1;
    bool use = false;
    const int doesHitSmartRat = 50;
    const int doesHitDumbRat = 80;
};
struct Stats {
    const int MAX = 5;
    int numDumbRats = MAX;
    int numSmartRats = MAX;
    int numCheeses = MAX;
};
struct Player {
    int x = 1;
    int y = 1;
    int prev_x = 1;
    int prev_y = 1;
    char symbol = 'P';
    NoLs noseplugs;
    HZ ziplocks;
    TrashCanShields shields;
    ExplodingKitties kitties;
    bool hasItem = true;
    Stats stats;
    const int MAX_NUM_HITS = 5;
    int numHits = 0;
};
void createTown(char[][COL], const int);
void showPlayer(char[][COL], Player);
void showTown(char[][COL], const int);
void getMove(Player *, const int);
void verifyMove(Player *, Cheese[], const int, SmartRats[], DumbRats[]);
void showCheeseRats(char town[][COL], const int R, Cheese ch[], DumbRats dr[], const int N, Player);
void showSmartRats(char[][COL], const int, SmartRats[], const int, Cheese[], DumbRats[], Player);
bool cheeseAttack(Player*, Cheese[], const int);
void findItem(Player*);
void playerAttackCheese(char[][COL], Player *, Cheese[], const int);
void playerAttackDumbRats(char[][COL], Player *, DumbRats[], const int);
void playerAttackSmartRats(char[][COL], Player *, SmartRats[], const int);
void getChoiceDefenseAttack(Player *);
void dumbRatMove(char[][COL], const int, DumbRats[], const int, Player, Cheese[]) ;
void smartRatMove(char[][COL], SmartRats[], const int, Player);
void dumbRatAttack(Player *, DumbRats[], const int);
void smartRatAttack(Player *, SmartRats[], const int);
void showPlayerStats(Player);
int main() {
    srand(time(0));

    // defining the town
    const int ROWS = 15;
    const int NUM_CHEESES = 5;
    char town[ROWS][COL] = {0};

    // player stats
    Player player;
    Player * pPtr = nullptr;
    pPtr = &player;
    player.numHits = 0;
    player.kitties.numEK = 2;
    player.shields.numShields = 1;
    player.ziplocks.numHZ = 2;
    player.noseplugs.numNOLs = 2;
    int won = -1;
    const int WIN = 1;
    const int LOSE = 0;
    const int MOVES_TURN = 3;
    // obstacles
    Cheese cheeses[NUM_CHEESES];
    SmartRats sRats[NUM_CHEESES];
    DumbRats dRats[NUM_CHEESES];

    createTown(town, ROWS);
    player.kitties.numEK = 1;
    player.shields.numShields = 1;
    player.ziplocks.numHZ = 1;
    player.noseplugs.numNOLs = 1;
    player.stats.numCheeses = player.stats.MAX;
    player.stats.numDumbRats = player.stats.MAX;
    player.stats.numSmartRats = player.stats.MAX;
    // initializing the first elements
    showPlayer(town, player);
    showCheeseRats(town, ROWS, cheeses, dRats, NUM_CHEESES, player);
    showSmartRats(town, ROWS, sRats, NUM_CHEESES, cheeses, dRats, player);
    while (won!= LOSE && won != WIN) {
        showTown(town, ROWS);
        getChoiceDefenseAttack(pPtr);
        std::cout << std::endl;
        for (int j = 0; j < MOVES_TURN; j++) {
            getMove(pPtr, ROWS);
            verifyMove(pPtr, cheeses, NUM_CHEESES, sRats, dRats);
            showPlayer(town, player);
            showTown(town, ROWS);
            bool stunned;
            stunned = cheeseAttack(pPtr, cheeses, NUM_CHEESES);
            if (stunned) {
                break;
            }
        }
        playerAttackCheese(town, pPtr, cheeses, NUM_CHEESES);
        playerAttackDumbRats(town, pPtr, dRats, NUM_CHEESES);
        playerAttackSmartRats(town, pPtr, sRats, NUM_CHEESES);
        smartRatAttack(pPtr, sRats, NUM_CHEESES);
        dumbRatAttack(pPtr, dRats, NUM_CHEESES);
        int smartRatsRemaining = player.stats.numSmartRats;
        int dumbRatsRemaining = player.stats.numDumbRats;
        int cheeseRemaining = player.stats.numCheeses;
        if (player.numHits == player.MAX_NUM_HITS) {
            won = LOSE;
            break;
        } else {
            if (smartRatsRemaining > 0 || dumbRatsRemaining > 0 || cheeseRemaining > 0) {
                won = WIN;
                break;
            }
        }
        std::cout << std::endl;
        findItem(pPtr);
        smartRatMove(town, sRats, NUM_CHEESES, player);
        dumbRatMove(town, ROWS, dRats, NUM_CHEESES, player, cheeses);
        if (player.stats.numSmartRats == 0 && player.stats.numCheeses == 0 && player.stats.numDumbRats == 0) {
            won = WIN;
        } else if (player.numHits == player.MAX_NUM_HITS) {
            won = LOSE;
        }
    }
    pPtr = nullptr;
    delete(pPtr);
    showPlayerStats(player);
    if (won == WIN) {
        std::cout << "You won!\n";
    } else {
        std::cout << "You lost\n";
    }
    std::cout << "Press enter to continue: ";
    getchar();
    getchar();
    return 0;
}
void createTown(char town[][COL], const int R) {
    for (int r = 0; r < R; r++) {
        for (int c = 0; c < COL; c++) {
            town[r][c] = '-';
        }
    }
    for (int r = 0; r < R; r++) {
        town[r][0] = '*';
        town[r][COL - 1] = '*';
    }
    for (int c = 0; c < COL; c++) {
        town[0][c] = '*';
        town[R-1][c] = '*';
    }
}

void showTown(char arr[][COL], const int R) {
    for (int r = 0; r < R; r++) {
        for (int c = 1; c < COL; c++) {
            std::cout << arr[r][c];
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}
void showPlayer(char town[][COL], Player p) {
    town[p.prev_y][p.prev_x] = '-';
    town[p.y][p.x] = p.symbol;
}
void getMove(Player *p, const int R) {
    char move = 0;
    p->prev_x = p->x;
    p->prev_y = p->y;
    std::cout << "Press j to go left\n";
    std::cout << "Press l to go right\n";
    std::cout << "Press i to go up\n";
    std::cout << "Press k to go down\n";
    std::cout << "Press n to stay in place\n";
    std::cout << "Pick your choice: ";
    std::cin >> move;
    move = tolower(move);
    while (move != 'j' && move != 'l' && move != 'i' && move != 'k' && move != 'n') {
        std::cout << "Error enter valid input: ";
        std::cin >> move;
        move = tolower(move);
    }
    switch (tolower(move)) {
        case 'j':
            if (p->x - 1 > 0) {
                p->x -= 1;
            }
            break;
        case 'l':
            if (p->x + 1 < (COL - 1)) {
                p->x += 1;
            }
            break;
        case 'i':
            if (p->y - 1 > 0) {
                p->y -= 1;
            }
            break;
        case 'k':
            if (p->y +1 < R-1) {
                p->y += 1;
            }
            break;
        default:
            break;
    }
}
void showCheeseRats(char town[][COL], const int R, Cheese ch[], DumbRats dr[], const int N, Player p) {
    for (int i = 0; i < N; i++) {
        ch[i].x = rand() % (COL - 3) + 2;
        ch[i].y = rand() % (R-3) + 2;
        while (ch[i].x == p.x && ch[i].y == p.y) {
            ch[i].x = rand() % (COL - 3) + 2;
            ch[i].y = rand() % (R - 3) +2;
        }
        dr[i].x = ch[i].x + 1;

        town[ch[i].y][ch[i].x] = ch->symbol;
        dr[i].x = ch[i].x;
        dr[i].y = ch[i].y + 1;
        town[dr[i].y][dr[i].x] = dr->symbol;
    }
}
void showSmartRats(char town[][COL], const int R, SmartRats sr[], const int N, Cheese ch[], DumbRats dr[], Player p) {
    for (int i = 0; i < N; i++) {
        sr[i].x = rand() % (COL - 3) + 1;
        sr[i].y = rand() % (R -3) + 1;
        while (sr[i].x == p.x && sr[i].y == p.y) {
            sr[i].x = rand() % (COL - 3) +1;
            sr[i].y = rand() % (COL -3) + 1;
        }
        for (int j = 0; j < N; j++) {
            while (dr[j].x == sr[i].x) {
                sr[i].x = rand() % (COL -2) + 1;
            }
            while (ch[j].x == sr[i].x) {
                sr[i].x = rand() % (COL -2) + 1;
            }
        }
        town[sr[i].y][sr[i].x] = sr->symbol;
    }
}
bool cheeseAttack(Player *p, Cheese ch[], const int N) {
    bool stunned;
    bool close = false;
    int location = 0;
    for (int i = 0; i < N; i++) {
        if (abs(p->x - ch[i].x) <= ch->RANGE && !ch[i].ignore) {
            close = true;
            location = i;
            break;
        } else if (abs(p->y - ch[i].y) <= ch->RANGE && !ch[i].ignore) {
            close = true;
            location = i;
            break;
        }
    }
    if (close) {
        if (p->noseplugs.use) {
            stunned = rand() % 100 < ch[location].STUNS_NOSEPLUGS;
            if (stunned) {
                std::cout << "Didn't work. You got stunned by the Smell of Doom!\n";
                ++p->numHits;
            } else {
                std::cout << "The cheese attacked\n"
                <<"You were saved by the noseplugs of Lavender!\n";
            }
        } else {
            stunned = rand() % 100 < ch[location].STUNS;
            if (stunned) {
                std::cout << "You got hit by the Smell of Doom!\n";
                ++p->numHits;
            } else {
                std::cout << "You're in luck! The cheese did not attack\n";
            }
        }
        std::cout << "You have " << p->MAX_NUM_HITS - p->numHits << " hits remaining\n";
    }
    return stunned;
}
void verifyMove(Player *p, Cheese ch[], const int N, SmartRats sr[], DumbRats dr[]) {
    for (int i = 0; i < N; i++) {
        if ((p->x == ch[i].x && p-> y == ch[i].y)
        || (p->x == sr[i].x && p->y == sr[i].y)
        || (p->x == dr[i].x && p->y == dr[i].y)) {
            p->x = p->prev_x;
            p->y = p->prev_y;
        }
    }
}
void findItem(Player *p) {
    bool found = false;
    if (!(p->x == p->prev_x && p->y == p->prev_y)) {
        found = rand() % 100 < 60;
    }
    int probability = rand() % 100;
    char choice = 0;
    if (found) {
        if (probability < 15) {
            std::cout << "You found a trash can shield!\n";
            std::cout << "You have " << p->shields.numShields << " shields\n";
            std::cout << "Would you like to take it? (Y/N)";
            std::cin >> choice;
            choice = toupper(choice);
            while (choice != 'Y' && choice != 'N') {
                std::cout << "Would you like to take it?(Y/N)";
                std::cin >> choice;
            }
            std::cout << std::endl;
            switch(toupper(choice)) {
                case 'Y':
                    if (p->shields.numShields == p->shields.MAX) {
                        std::cout << "You already have a shield\n";
                    } else {
                        ++p->shields.numShields;
                    }
                    break;
                case 'N':
                    std::cout << "You did not take the shield.\n";
                    break;
            }
        } else if (probability < 50) {
            std::cout << "You found an Exploding Kitty!\n";
            std::cout << "You have " << p->kitties.numEK << " kitties\n";
            std::cout << "Would you like to take it? (Y/N)";
            std::cin >> choice;
            choice = toupper(choice);
            while (choice != toupper('y') && choice != toupper('N')) {
                std::cout << "Would you like to take it?(Y/N)";
                std::cin >> choice;
                choice = toupper(choice);
            }
            switch(toupper(choice)) {
                case 'Y':
                    if (p->kitties.numEK == p->kitties.MAX) {
                        std::cout << "You cannot carry any more kitties\n";
                    } else {
                        std::cout << "You took the kitty\n";
                        ++p->kitties.numEK;
                    }
                    break;
                case 'N':
                    std::cout << "You did not take the kitty.\n";
                    break;
            }
        } else if (probability < 70) {
            std::cout << "You found a Huge ZipLock!\n";
            std::cout << "You have " << p->ziplocks.numHZ << "ziplocks\n";
            std::cout << "Would you like to take it? (Y/N)";
            std::cin >> choice;
            choice = toupper(choice);
            while (choice != 'Y' && choice != 'N') {
                std::cout << "Would you like to take it?(Y/N)";
                std::cin >> choice;
                choice = toupper(choice);
            }
            switch(toupper(choice)) {
                case 'Y':
                    if (p->ziplocks.numHZ == p->ziplocks.MAX) {
                        std::cout << "You cannot carry any more ziplocks\n";
                    } else {
                        std::cout << "You took the ziplock\n";
                        ++p->ziplocks.numHZ;
                    }
                    break;
                case 'N':
                    std::cout << "You did not take the ziplock.\n";
                    break;
            }
        } else {
            std::cout << "You found a Noseplug of Lavender!\n";
            std::cout << "You have " << p->noseplugs.numNOLs << " noseplugs\n";
            std::cout << "Would you like to take it? (Y/N)";
            std::cin >> choice;
            choice = toupper(choice);
            while (choice != 'Y' && choice != 'N') {
                std::cout << "Would you like to take it?(Y/N)";
                std::cin >> choice;
                choice = toupper(choice);
            }
            switch(toupper(choice)) {
                case 'Y':
                    if (p->noseplugs.numNOLs == p->noseplugs.MAX) {
                        std::cout << "You cannot carry any more\n";
                    } else {
                        std::cout << "You took the noseplug\n";
                        ++p->noseplugs.numNOLs;
                    }
                    break;
                case 'N':
                    std::cout << "You did not take the noseplug.\n";
                    break;
            }
        }
    }
}
void getChoiceDefenseAttack(Player *p) {
    char choice = 0;
    if (p->kitties.numEK > 0 || p->noseplugs.numNOLs > 0 || p->ziplocks.numHZ > 0
    || p->shields.numShields > 0) {
        p->hasItem = true;
    }
    if (p->hasItem) {
        std::cout << "Would you like to use \n" << std::endl;
        if (p->kitties.numEK > 0) {
            std::cout << "the Exploding Kitties (K) against the rats\n";
        }
        if (p->ziplocks.numHZ > 0) {
            std::cout << "or the Huge ZipLocks against the cheese\n";
        }
        if (p->noseplugs.numNOLs > 0) {
            std::cout << "or the noseplugs of Lavender (N) to defend against the cheese\n";
        }
        if (p->shields.numShields > 0 && (!p->shields.use)) {
            std::cout << "or the Trash Can Shields (S) to defend against the rats\n";
        }
        std::cout << "Press q to quit this dialogue: ";
        std::cin >> choice;
        choice = toupper(choice);
        while (choice != 'Z' && choice != 'K' && choice != 'N' && choice != 'S' && choice != 'Q') {
            std::cout << "Please enter a valid choice: ";
            std::cin >> choice;
            choice = toupper(choice);
        }
        switch (toupper(choice)) {
            case 'Q':
                std::cout << "All right. Nothing's been set\n";
                break;
            case 'Z':
                if (p->ziplocks.numHZ > 0) {
                    std::cout << "You are using the ziplocks\n";
                    if (!p->ziplocks.use) {
                        --p->ziplocks.numHZ;
                        p->ziplocks.use = true;
                    }
                } else {
                    std::cout << "You have no ziplocks\n";
                }
                break;
            case 'K':
                if (p->kitties.numEK > 0) {
                    std::cout << "You deployed the exploding kitty\n";
                    p->kitties.use = true;
                } else {
                    std::cout << "You have no kitties in your inventory\n";
                }
                break;
            case 'N':
                if (p->noseplugs.numNOLs > 0) {
                    std::cout << "You deployed the noseplugs of Lavender\n";
                    p->noseplugs.use = true;
                    --p->noseplugs.numNOLs;
                } else {
                    std::cout << "You have no more noseplugs\n";
                }
                break;
            case 'S':
                if (p->shields.numShields > 0) {
                    std::cout << "You deployed the Trash Can Shields\n";
                    if (!p->shields.use) {
                        p->shields.use = true;
                    }
                }
        }
    }

}
void showPlayerStats(Player p) {
    std::cout << "Rats remaining: " << p.stats.numSmartRats + p.stats.numSmartRats << std::endl;
    std::cout << "Cheese remaining: " << p.stats.numCheeses << std::endl;
    std::cout << "Hits: " << p.numHits << std::endl;
    std::cout << "Remaining hits: " << p.MAX_NUM_HITS - p.numHits << std::endl;
}
void playerAttackCheese(char town[][COL], Player *p, Cheese ch[], const int N) {
    bool close = false;
    int loc = 0;
    if (p->ziplocks.use) {
        for (int i = 0; i < N; i++) {
            if (abs(ch[i].x - p->x) <= (p->ziplocks.RANGE
            && !ch[i].ignore)) {
                close = true;
                loc = i;
                break;
            } else if (!ch[i].ignore && abs(ch[i].y - p->x) <= p->ziplocks.RANGE) {
                close = true;
                loc = i;
                break;
            }
        }
        if (close) {
            bool doesWork = (rand() % 100 < p->ziplocks.CHANCE_WORKS);
            if (doesWork) {
                std::cout << "You wrapped the cheese successfully!\n";
                --p->stats.numCheeses;
                town[ch[loc].y][ch[loc].x] = '-';
                ch[loc].ignore = true;
            } else {
                std::cout << "Didn't work. The bag tore when used\n";
            }
        } else {
            std::cout << "You are not in range of the cheese\n";
        }
    }
    p->ziplocks.use = false;
}
void playerAttackDumbRats(char town[][COL], Player *p, DumbRats dr[], const int N) {
    bool close = false;
    int x = 0;
    int y = 0;
    int loc = 0;
    if (p->kitties.use) {
        for (int i = 0; i < N; i++) {
            if (abs(dr[i].x - p->x) <= (p->kitties.RANGE)) {
                close = true;
                dr[i].x = x;
                dr[i].y = y;
                loc = i;
                break;
            } else if (abs(dr[i].y - p->x) <= p->kitties.RANGE && !dr[i].ignore) {
                close = true;
                dr[i].x = x;
                dr[i].y = y;
            }
        }
        if (close) {
            bool doesWork = (rand() % 100 < p->kitties.doesHitDumbRat);
            if (doesWork) {
                std::cout << "It worked! The cat destroyed the Dumb Rat\n";
                --p->stats.numDumbRats;
                town[y][x] = '-';
                dr[loc].ignore = true;
            } else {
                std::cout << "Didn't work. The bag tore when used\n";
            }
        } else {
            std::cout << "You are not in range of the rats\n";
        }
        p->kitties.use = false;
    }
}
void playerAttackSmartRats(char town[][COL], Player *p, SmartRats sr[], const int N) {
    bool close = false;
    int x = 0;
    int y = 0;
    int loc = 0;
    if (p->kitties.use) {
        for (int i = 0; i < N; i++) {
            if (abs(sr[i].x - p->x) <= (p->kitties.RANGE
                                        && abs(sr[i].y - p->x) <= p->kitties.RANGE
                                        && !sr[i].ignore)) {
                close = true;
                sr[i].x = x;
                sr[i].y = y;
                loc = i;
                break;
            }
        }
        if (close) {
            bool doesWork = (rand() % 100 < p->kitties.doesHitSmartRat);
            if (doesWork) {
                std::cout << "It worked! The cat destroyed the Smart Rat\n";
                --p->stats.numDumbRats;
                town[y][x] = '-';
                sr[loc].ignore = true;
            } else {
                std::cout << "Didn't work. The cat didn't destroy the Smart Rat\n";
            }
        } else {
            std::cout << "You are not in range of the rats\n";
        }
        p->kitties.use = false;
    }
}
void dumbRatMove(char town[][COL], const int R, DumbRats dr[], const int N, Player p, Cheese ch[]) {
    int numCalled = p.stats.numDumbRats;
    for (int j = 0; j < N; j++) {
        dr[j].prev_x = dr[j].x;
        dr[j].prev_y = dr[j].y;

        bool changesX = rand() % 100 < 50;
        int direction = rand() % 3 -1;
        if (changesX) {
            dr[j].x += direction;
        } else {
            dr[j].y += direction;
        }
        if (dr[j].x == p.x && dr[j].y == p.y) {
            dr[j].x -=direction;
        }
        for (int i = 0; i < numCalled; i++) {
            if (dr[j].x == ch[i].x && dr[j].y == ch[i].y) {
                dr[j].x -=direction;
            }
        }
    for (int i = 0; i < N; i++) {
        town[dr[j].prev_y][dr[j].prev_x] = '-';
        town[dr[j].y][dr[j].x] = dr->symbol;
    }

    }
}
void smartRatMove(char town[][COL], SmartRats sr[], const int N, Player p) {
    for (int i = 0; i < N; i++) {
        if (abs(p.x - sr[i].x) > sr[i].ATTACK_RANGE && abs(p.y - sr[i].y) > sr->ATTACK_RANGE) {
            if (sr[i].x > p.x) {
                sr[i].x -=1;
            } else if (sr[i].x < p.x) {
                sr[i].x +=1;
            } if (sr[i].y > p.y) {
                sr[i].y +=1;
            } else if (sr[i].y < p.y) {
                sr[i].y -=1;
            }
        }
        town[sr[i].y][sr[i].x] = sr->symbol;
        town[sr[i].prev_y][sr[i].prev_x] = '-';
    }
}
void dumbRatAttack(Player *p, DumbRats dr[], const int N) {
    bool hit;
    bool close = false;
    int location = 0;
    for (int i = 0; i < N; i++) {
        if (abs(p->x - dr[i].x) <= dr->ATTACK_RANGE && !dr[i].ignore) {
            close = true;
            location = i;
            break;
        } else if (abs(p->y - dr[i].y) <= dr->ATTACK_RANGE && !dr[i].ignore) {
            close = true;
            location = i;
            break;
        }
    }
    if (close) {
        hit = rand() % 100 < dr[location].CHANCE_BITE;
        if (p->shields.use) {
            if (hit) {
                std::cout << "Didn't work. You got bitten by the Smart Rat!\n";
                ++p->numHits;
            } else {
                std::cout << "The rat attacked, but you were saved by your shield!\n";
                ++p->shields.numBites;
                if (p->shields.numBites == p->shields.MAX_BITES) {
                    p->shields.use = false;
                    p->shields.numShields = 0;
                    p->shields.numBites = 0;
                }
            }
        } else {
            if (hit) {
                std::cout << "You got bitten by a rat!\n";
                ++p->numHits;
            } else {
                std::cout << "You're in luck! The rat did not attack\n";
            }
        }
        std::cout << "You have " << p->MAX_NUM_HITS - p->numHits << " hits remaining\n";
    }
}
void smartRatAttack(Player *p, SmartRats sr[], const int N) {
    bool hit;
    bool close = false;
    int location = 0;
    for (int i = 0; i < N; i++) {
        for (int i = 0; i < N; i++) {
            if (abs(p->x - sr[i].x) <= sr->ATTACK_RANGE && !sr[i].ignore) {
                close = true;
                location = i;
                break;
            } else if (abs(p->y - sr[i].y) <= sr->ATTACK_RANGE && !sr[i].ignore) {
                close = true;
                location = i;
                break;
            }
        }
    }
    if (close) {
        if (p->shields.use) {
            hit = rand() % 100 < sr[location].BITE_SHIELD;
            if (hit) {
                std::cout << "Didn't work. You got bitten by the Smart Rat!\n";
                ++p->numHits;
            } else {
                std::cout << "The rat attacked, but you were saved by your shield!\n";
                ++p->shields.numBites;
                if (p->shields.numBites == p->shields.MAX_BITES) {
                    p->shields.use = false;
                    std::cout << "You have used up your shield\n";
                    p->shields.numBites = 0;
                }
            }
        } else {
            hit = rand() % 100 < sr[location].BITE_NO_SHIELD;
            if (hit) {
                std::cout << "You got bitten by a rat!\n";
                ++p->numHits;
            } else {
                std::cout << "You're in luck! The rat did not attack\n";
            }
        }
        std::cout << "You have " << p->MAX_NUM_HITS - p->numHits << " hits remaining\n";
    }
}


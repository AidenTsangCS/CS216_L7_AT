// Aiden Tsang
// CS216 Lab 7
// clang-format off

#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <new>
#include <sstream>
#include <string>

using namespace std;

const int MIN_ARMY_STAT = 45;
const int MAX_ARMY_STAT = 275;
const int MIN_RESET_STAT = 30;
const int MAX_RESET_STAT = 150;
const int MIN_VALID_STRENGTH = 1;
const int MIN_VALID_HEALTH = 0;

const int PERCENT_ROLL = 100;
const int DEMON_BONUS_CHANCE = 15;
const int DEMON_BONUS_DAMAGE = 40;
const int BALROG_ATTACKS = 2;
const int ELF_BONUS_CHANCE = 20;
const int ELF_MULTIPLIER = 2;
const int CYBERELF_BONUS_CHANCE = 30;
const int CYBERELF_BONUS_DAMAGE = 50;

const string DEFAULT_NAME = "n/a";
const int DEFAULT_STAT = MIN_ARMY_STAT;
const string INVALID_NAME = "";
const int INVALID_STAT = -1;

const string DEFAULT_ARMY_NAME = "Unnamed";
const int DEFAULT_ARMY_SIZE = 0;
const int MIN_ARMY_SIZE = 1;
const int MAX_ARMY_SIZE = 12;
const int MIN_ARMY_NAME_ALPHA = 3;

const string CREATURE_TYPE_NAMES[] = {"demon", "balrog", "elf", "cyberelf"};
const int NUM_TYPES = 4;

const string NAMES_FILE = "in_creature_names.txt";
const int NAME_COL = 16;
const int TYPE_COL = 12;
const int NUM_COL = 10;
const int DUEL_NAME_COL = 24;
const int DUEL_ARMY_COL = 12;
const int DUEL_NUM_COL = 9;
const int DIVIDER_WIDTH = 78;
const char DIVIDER_CHAR = '=';

const unsigned int RANDOM_SEED = 216;
const int COIN_FLIP = 2;
const int LETTER_CASE_OFFSET = 'a' - 'A';

enum MenuOption { PLAY = 1, QUIT };
enum RematchOption { REMATCH_YES = 1, REMATCH_NO };
enum CreatureType { DEMON, BALROG, ELF, CYBERELF };

class Creature {
  private:
    string name = DEFAULT_NAME;
    int strength = DEFAULT_STAT;
    int health = DEFAULT_STAT;

  public:
    Creature();
    Creature(const string &newName, int newStrength, int newHealth);
    virtual ~Creature();

    void setCreature(const string &newName, int newStrength, int newHealth);
    void setName(const string &newName);
    void setStrength(int newStrength);
    void setHealth(int newHealth);
    void reset();

    string getId() const;
    int getStrength() const;
    int getHealth() const;

    virtual string getTypeName() const;
    virtual string getName() const;
    virtual int getDamage() const;
    virtual string toString() const;
    virtual Creature *clone() const;
};

class Demon : public Creature {
  public:
    Demon();
    Demon(const string &newName, int newStrength, int newHealth);
    ~Demon();

    string getTypeName() const;
    int getDamage() const;
    Creature *clone() const;
};

class Balrog : public Creature {
  public:
    Balrog();
    Balrog(const string &newName, int newStrength, int newHealth);
    ~Balrog();

    string getTypeName() const;
    int getDamage() const;
    Creature *clone() const;
};

class Elf : public Creature {
  public:
    Elf();
    Elf(const string &newName, int newStrength, int newHealth);
    ~Elf();

    string getTypeName() const;
    int getDamage() const;
    Creature *clone() const;
};

class Cyberelf : public Elf {
  public:
    Cyberelf();
    Cyberelf(const string &newName, int newStrength, int newHealth);
    ~Cyberelf();

    string getTypeName() const;
    int getDamage() const;
    Creature *clone() const;
};

class Army {
  private:
    string name = DEFAULT_ARMY_NAME;
    int size = DEFAULT_ARMY_SIZE;
    Creature **ppCreatures = nullptr;

    void setArmy(const string &newName, int newSize, Creature **ppNewCreatures);
    void releaseMemory();
    bool fillCreatures(Creature **&ppTemp, int count, const string *pNames, int startIndex) const;
    bool copyCreatures(Creature **&ppTemp, const Army &rhs) const;

  public:
    Army();
    Army(const Army &rhs);
    ~Army();

    Army &operator=(const Army &rhs);

    bool createArmy(const string &newName, int newSize, const string *pNames, int startIndex);
    void resetCreatures();

    string getName() const;
    int getSize() const;
    int getTotalHealth() const;
    Creature *getCreature(int index) const;
    void print(const string &label) const;
};

class Game {
  private:
    Army army1;
    Army army2;

    void runBattle(const string *pNames, int nameCount);
    void fightRound(int armySize);
    void runDuel(int position);
    void performStrike(Creature *pAttacker, const string &attackerArmy, Creature *pDefender, const string &defenderArmy) const;
    void printDuelHeader() const;
    void announceWinner() const;

  public:
    Game();

    void play();
};

void clearCin(const string &errorMessage);
int readInt(const string &prompt, int minVal, int maxVal);
string readArmyName(const string &prompt);
int countAlphabetic(const string &text);
string capitalizeFirst(const string &text);
int randomStat();
CreatureType randomType();
Creature *createCreature(CreatureType type, const string &creatureName, int newStrength, int newHealth);
Creature **allocatePointerArray(int count);
void releaseArray(Creature **&ppList, int count);
string *loadNames(const string &fileName, int &nameCount);
void printDivider();

int main() {
    srand(RANDOM_SEED);

    try {
        Game battleGame;
        int menuChoice = 0;
        string menuPrompt = "\n\nBattle Arena Menu:\n1. Play Game\n2. Quit\nEnter your choice: ";

        do {
            menuChoice = readInt(menuPrompt, numeric_limits<int>::min(), numeric_limits<int>::max());

            switch (menuChoice) {
            case PLAY:
                battleGame.play();
                break;
            case QUIT:
                cout << "\nThanks for playing. Goodbye!" << endl;
                break;
            default:
                clearCin("Invalid menu choice");
            }
        } while (menuChoice != QUIT);
    }
    catch (const bad_alloc &error) {
        cout << "\nThe program ran out of memory and cannot continue" << endl;
    }

    return 0;
}

Creature::Creature() {
    setCreature(DEFAULT_NAME, DEFAULT_STAT, DEFAULT_STAT);
}

Creature::Creature(const string &newName, int newStrength, int newHealth) {
    setCreature(newName, newStrength, newHealth);
}

Creature::~Creature() {
    name = INVALID_NAME;
    strength = INVALID_STAT;
    health = INVALID_STAT;
}

void Creature::setCreature(const string &newName, int newStrength, int newHealth) {
    bool isValid = newName.length() > 0 && newStrength >= MIN_VALID_STRENGTH && newHealth >= MIN_VALID_HEALTH;

    if (!isValid) {
        cout << "\nInvalid creature data; keeping current values" << endl;
    }
    else {
        name = newName;
        strength = newStrength;
        health = newHealth;
    }
}

void Creature::setName(const string &newName) {
    setCreature(newName, strength, health);
}

void Creature::setStrength(int newStrength) {
    setCreature(name, newStrength, health);
}

void Creature::setHealth(int newHealth) {
    setCreature(name, strength, newHealth);
}

void Creature::reset() {
    int newStrength = (rand() % (MAX_RESET_STAT - MIN_RESET_STAT + 1)) + MIN_RESET_STAT;
    int newHealth = (rand() % (MAX_RESET_STAT - MIN_RESET_STAT + 1)) + MIN_RESET_STAT;

    setCreature(name, newStrength, newHealth);
}

string Creature::getId() const {
    return name;
}

int Creature::getStrength() const {
    return strength;
}

int Creature::getHealth() const {
    return health;
}

string Creature::getTypeName() const {
    return "creature";
}

string Creature::getName() const {
    return name + " the " + capitalizeFirst(getTypeName());
}

int Creature::getDamage() const {
    return (rand() % strength) + 1;
}

string Creature::toString() const {
    ostringstream oss;

    oss << left << setw(NAME_COL) << name << setw(TYPE_COL) << getTypeName() << right << setw(NUM_COL) << strength << setw(NUM_COL) << health;

    return oss.str();
}

Creature *Creature::clone() const {
    return new Creature(*this);
}

Demon::Demon() : Creature() {
}

Demon::Demon(const string &newName, int newStrength, int newHealth) : Creature(newName, newStrength, newHealth) {
}

Demon::~Demon() {
}

string Demon::getTypeName() const {
    return CREATURE_TYPE_NAMES[DEMON];
}

int Demon::getDamage() const {
    int damage = Creature::getDamage();

    if ((rand() % PERCENT_ROLL) < DEMON_BONUS_CHANCE) {
        damage = damage + DEMON_BONUS_DAMAGE;
    }

    return damage;
}

Creature *Demon::clone() const {
    return new Demon(*this);
}

Balrog::Balrog() : Creature() {
}

Balrog::Balrog(const string &newName, int newStrength, int newHealth) : Creature(newName, newStrength, newHealth) {
}

Balrog::~Balrog() {
}

string Balrog::getTypeName() const {
    return CREATURE_TYPE_NAMES[BALROG];
}

int Balrog::getDamage() const {
    int damage = 0;

    for (int strike = 0; strike < BALROG_ATTACKS; ++strike) {
        damage = damage + Creature::getDamage();
    }

    return damage;
}

Creature *Balrog::clone() const {
    return new Balrog(*this);
}

Elf::Elf() : Creature() {
}

Elf::Elf(const string &newName, int newStrength, int newHealth) : Creature(newName, newStrength, newHealth) {
}

Elf::~Elf() {
}

string Elf::getTypeName() const {
    return CREATURE_TYPE_NAMES[ELF];
}

int Elf::getDamage() const {
    int damage = Creature::getDamage();

    if ((rand() % PERCENT_ROLL) < ELF_BONUS_CHANCE) {
        damage = damage * ELF_MULTIPLIER;
    }

    return damage;
}

Creature *Elf::clone() const {
    return new Elf(*this);
}

Cyberelf::Cyberelf() : Elf() {
}

Cyberelf::Cyberelf(const string &newName, int newStrength, int newHealth) : Elf(newName, newStrength, newHealth) {
}

Cyberelf::~Cyberelf() {
}

string Cyberelf::getTypeName() const {
    return CREATURE_TYPE_NAMES[CYBERELF];
}

int Cyberelf::getDamage() const {
    int damage = Elf::getDamage();

    if ((rand() % PERCENT_ROLL) < CYBERELF_BONUS_CHANCE) {
        damage = damage + CYBERELF_BONUS_DAMAGE;
    }

    return damage;
}

Creature *Cyberelf::clone() const {
    return new Cyberelf(*this);
}

Army::Army() {
    setArmy(DEFAULT_ARMY_NAME, DEFAULT_ARMY_SIZE, nullptr);
}

Army::Army(const Army &rhs) {
    if (rhs.size > 0 && rhs.ppCreatures != nullptr) {
        Creature **ppTemp = allocatePointerArray(rhs.size);

        if (ppTemp != nullptr && copyCreatures(ppTemp, rhs)) {
            setArmy(rhs.name, rhs.size, ppTemp);
        }
        else {
            cout << "\nCould not copy the army; an empty army was created instead" << endl;
        }
    }
}

Army::~Army() {
    releaseMemory();
    name = DEFAULT_ARMY_NAME;
}

Army &Army::operator=(const Army &rhs) {
    if (this != &rhs) {
        Creature **ppTemp = allocatePointerArray(rhs.size);

        if (ppTemp != nullptr && copyCreatures(ppTemp, rhs)) {
            releaseMemory();
            setArmy(rhs.name, rhs.size, ppTemp);
        }
        else {
            cout << "\nCould not copy the army; the original army was left unchanged" << endl;
        }
    }

    return *this;
}

void Army::setArmy(const string &newName, int newSize, Creature **ppNewCreatures) {
    name = newName;
    size = newSize;
    ppCreatures = ppNewCreatures;
}

void Army::releaseMemory() {
    releaseArray(ppCreatures, size);
    size = DEFAULT_ARMY_SIZE;
}

bool Army::fillCreatures(Creature **&ppTemp, int count, const string *pNames, int startIndex) const {
    bool isSuccess = true;

    for (int i = 0; i < count && isSuccess; ++i) {
        try {
            ppTemp[i] = createCreature(randomType(), pNames[startIndex + i], randomStat(), randomStat());
        }
        catch (const bad_alloc &error) {
            isSuccess = false;
        }
    }

    if (!isSuccess) {
        releaseArray(ppTemp, count);
    }

    return isSuccess;
}

bool Army::copyCreatures(Creature **&ppTemp, const Army &rhs) const {
    bool isSuccess = true;

    for (int i = 0; i < rhs.size && isSuccess; ++i) {
        try {
            ppTemp[i] = rhs.ppCreatures[i]->clone();
        }
        catch (const bad_alloc &error) {
            isSuccess = false;
        }
    }

    if (!isSuccess) {
        releaseArray(ppTemp, rhs.size);
    }

    return isSuccess;
}

bool Army::createArmy(const string &newName, int newSize, const string *pNames, int startIndex) {
    bool isSuccess = false;
    bool isValid = countAlphabetic(newName) >= MIN_ARMY_NAME_ALPHA && newSize >= MIN_ARMY_SIZE && newSize <= MAX_ARMY_SIZE && pNames != nullptr;

    if (!isValid) {
        cout << "\nInvalid army data; the army was not created" << endl;
    }
    else {
        Creature **ppTemp = allocatePointerArray(newSize);

        if (ppTemp != nullptr && fillCreatures(ppTemp, newSize, pNames, startIndex)) {
            releaseMemory();
            setArmy(newName, newSize, ppTemp);
            isSuccess = true;
        }
        else {
            cout << "\nCould not build the army; the previous army was left unchanged" << endl;
        }
    }

    return isSuccess;
}

void Army::resetCreatures() {
    if (ppCreatures != nullptr) {
        for (int i = 0; i < size; ++i) {
            ppCreatures[i]->reset();
        }
    }
}

string Army::getName() const {
    return name;
}

int Army::getSize() const {
    return size;
}

int Army::getTotalHealth() const {
    int total = 0;

    for (int i = 0; i < size; ++i) {
        total = total + ppCreatures[i]->getHealth();
    }

    return total;
}

Creature *Army::getCreature(int index) const {
    Creature *pFound = nullptr;

    if (ppCreatures != nullptr && index >= 0 && index < size) {
        pFound = ppCreatures[index];
    }

    return pFound;
}

void Army::print(const string &label) const {
    cout << "\n" << name << " Stats " << label << endl;

    if (ppCreatures == nullptr || size == 0) {
        cout << "This army is empty." << endl;
    }
    else {
        cout << left << setw(NAME_COL) << "Creature" << setw(TYPE_COL) << "Type" << right << setw(NUM_COL) << "Strength" << setw(NUM_COL) << "Health" << endl;

        for (int i = 0; i < size; ++i) {
            cout << ppCreatures[i]->toString() << endl;
        }

        cout << "Total health of " << name << ": " << getTotalHealth() << endl;
    }
}

Game::Game() {
}

void Game::play() {
    int nameCount = 0;
    string *pNames = loadNames(NAMES_FILE, nameCount);

    if (pNames == nullptr) {
        cout << "\nCould not read " << NAMES_FILE << "; the battle was cancelled" << endl;
    }
    else {
        runBattle(pNames, nameCount);
        delete[] pNames;
        pNames = nullptr;
    }
}

void Game::runBattle(const string *pNames, int nameCount) {
    int maxSize = nameCount / COIN_FLIP;

    if (maxSize > MAX_ARMY_SIZE) {
        maxSize = MAX_ARMY_SIZE;
    }

    if (maxSize < MIN_ARMY_SIZE) {
        cout << "\nThe name file does not hold enough names for a battle" << endl;
    }
    else {
        string name1 = readArmyName("\nEnter the name of army #1 (3+ letters): ");
        string name2 = readArmyName("Enter the name of army #2 (3+ letters): ");
        int armySize = readInt("Enter the number of creatures per army: ", MIN_ARMY_SIZE, maxSize);
        bool isReady = army1.createArmy(name1, armySize, pNames, 0);

        if (isReady) {
            isReady = army2.createArmy(name2, armySize, pNames, armySize);
        }

        if (!isReady) {
            cout << "\nThe armies could not be built; returning to the menu" << endl;
        }
        else {
            bool wantsRematch = true;

            while (wantsRematch) {
                fightRound(armySize);
                wantsRematch = (readInt("\nFight a rematch with the same creatures? 1. Yes  2. No\nEnter your choice: ", REMATCH_YES, REMATCH_NO) == REMATCH_YES);

                if (wantsRematch) {
                    army1.resetCreatures();
                    army2.resetCreatures();
                    cout << "\nBoth armies have recovered; every creature's strength and health were re-rolled" << endl;
                }
            }
        }
    }
}

void Game::fightRound(int armySize) {
    printDivider();
    cout << "NEW BATTLE" << endl;
    printDivider();

    army1.print("before the Battle");
    army2.print("before the Battle");
    printDuelHeader();

    for (int position = 0; position < armySize; ++position) {
        runDuel(position);
    }

    army1.print("after the Battle");
    army2.print("after the Battle");
    announceWinner();
}

void Game::printDuelHeader() const {
    cout << "\n" << left << setw(DUEL_NAME_COL) << "Attacker" << setw(DUEL_ARMY_COL) << "Army" << right << setw(DUEL_NUM_COL) << "Damage" << "   " << left << setw(DUEL_NAME_COL) << "Defender" << setw(DUEL_ARMY_COL) << "Army" << right << setw(DUEL_NUM_COL) << "Before" << setw(DUEL_NUM_COL) << "After" << endl;
}

void Game::runDuel(int position) {
    Creature *pFirst = army1.getCreature(position);
    Creature *pSecond = army2.getCreature(position);

    if (pFirst == nullptr || pSecond == nullptr) {
        cout << "\nDuel " << (position + 1) << " could not start; a creature was missing" << endl;
    }
    else {
        Creature *pAttacker = pFirst;
        Creature *pDefender = pSecond;
        string attackerArmy = army1.getName();
        string defenderArmy = army2.getName();

        if ((rand() % COIN_FLIP) == 0) {
            pAttacker = pSecond;
            pDefender = pFirst;
            attackerArmy = army2.getName();
            defenderArmy = army1.getName();
        }

        cout << "\n-- Duel " << (position + 1) << ": " << pFirst->getName() << " of " << army1.getName() << " vs " << pSecond->getName() << " of " << army2.getName() << " --" << endl;

        while (pFirst->getHealth() > 0 && pSecond->getHealth() > 0) {
            performStrike(pAttacker, attackerArmy, pDefender, defenderArmy);

            Creature *pSwapCreature = pAttacker;
            pAttacker = pDefender;
            pDefender = pSwapCreature;

            string swapArmy = attackerArmy;
            attackerArmy = defenderArmy;
            defenderArmy = swapArmy;
        }

        bool isFirstAlive = pFirst->getHealth() > 0;
        Creature *pWinner = pSecond;
        Creature *pLoser = pFirst;

        if (isFirstAlive) {
            pWinner = pFirst;
            pLoser = pSecond;
        }

        cout << ">> " << pWinner->getName() << " defeated " << pLoser->getName() << endl;
    }
}

void Game::performStrike(Creature *pAttacker, const string &attackerArmy, Creature *pDefender, const string &defenderArmy) const {
    int damage = pAttacker->getDamage();
    int healthBefore = pDefender->getHealth();
    int healthAfter = healthBefore - damage;

    if (healthAfter < MIN_VALID_HEALTH) {
        healthAfter = MIN_VALID_HEALTH;
    }

    pDefender->setHealth(healthAfter);

    cout << left << setw(DUEL_NAME_COL) << pAttacker->getName() << setw(DUEL_ARMY_COL) << attackerArmy << right << setw(DUEL_NUM_COL) << damage << "   " << left << setw(DUEL_NAME_COL) << pDefender->getName() << setw(DUEL_ARMY_COL) << defenderArmy << right << setw(DUEL_NUM_COL) << healthBefore << setw(DUEL_NUM_COL) << pDefender->getHealth() << endl;
}

void Game::announceWinner() const {
    int total1 = army1.getTotalHealth();
    int total2 = army2.getTotalHealth();
    string resultMessage = ">>> The battle ends in a tie! <<<";

    if (total1 > total2) {
        resultMessage = ">>> " + army1.getName() + " wins the battle! <<<";
    }
    else if (total2 > total1) {
        resultMessage = ">>> " + army2.getName() + " wins the battle! <<<";
    }

    printDivider();
    cout << resultMessage << "\n" << army1.getName() << " overall health: " << total1 << "\n" << army2.getName() << " overall health: " << total2 << endl;
    printDivider();
}

void clearCin(const string &errorMessage) {
    if (cin.fail()) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    cout << "\n" << errorMessage << endl;
}

int readInt(const string &prompt, int minVal, int maxVal) {
    int inputVal = 0;
    bool isValid = false;

    while (!isValid) {
        cout << prompt;
        cin >> inputVal;

        if (cin.fail()) {
            clearCin("Invalid input; please enter a whole number");
        }
        else if (inputVal < minVal || inputVal > maxVal) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\nPlease enter a number between " << minVal << " and " << maxVal << endl;
        }
        else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            isValid = true;
        }
    }

    return inputVal;
}

string readArmyName(const string &prompt) {
    string inputName = "";
    bool isValid = false;

    while (!isValid) {
        cout << prompt;
        getline(cin, inputName);

        if (countAlphabetic(inputName) < MIN_ARMY_NAME_ALPHA) {
            cout << "\nAn army name needs at least " << MIN_ARMY_NAME_ALPHA << " letters" << endl;
        }
        else {
            isValid = true;
        }
    }

    return inputName;
}

int countAlphabetic(const string &text) {
    int count = 0;
    int length = static_cast<int>(text.length());

    for (int i = 0; i < length; ++i) {
        char letter = text[i];

        if ((letter >= 'A' && letter <= 'Z') || (letter >= 'a' && letter <= 'z')) {
            ++count;
        }
    }

    return count;
}

string capitalizeFirst(const string &text) {
    string capitalized = text;

    if (capitalized.length() > 0 && capitalized[0] >= 'a' && capitalized[0] <= 'z') {
        capitalized[0] = static_cast<char>(capitalized[0] - LETTER_CASE_OFFSET);
    }

    return capitalized;
}

int randomStat() {
    return (rand() % (MAX_ARMY_STAT - MIN_ARMY_STAT + 1)) + MIN_ARMY_STAT;
}

CreatureType randomType() {
    return static_cast<CreatureType>(rand() % NUM_TYPES);
}

Creature *createCreature(CreatureType type, const string &creatureName, int newStrength, int newHealth) {
    Creature *pNew = nullptr;

    switch (type) {
    case DEMON:
        pNew = new Demon(creatureName, newStrength, newHealth);
        break;
    case BALROG:
        pNew = new Balrog(creatureName, newStrength, newHealth);
        break;
    case ELF:
        pNew = new Elf(creatureName, newStrength, newHealth);
        break;
    case CYBERELF:
        pNew = new Cyberelf(creatureName, newStrength, newHealth);
        break;
    }

    return pNew;
}

Creature **allocatePointerArray(int count) {
    Creature **ppTemp = nullptr;

    try {
        ppTemp = new Creature *[count] { nullptr };
    }
    catch (const bad_alloc &error) {
        cout << "\nMemory allocation failed for the array of creature pointers" << endl;
        ppTemp = nullptr;
    }

    return ppTemp;
}

void releaseArray(Creature **&ppList, int count) {
    if (ppList != nullptr) {
        for (int i = 0; i < count; ++i) {
            delete ppList[i];
            ppList[i] = nullptr;
        }

        delete[] ppList;
        ppList = nullptr;
    }
}

string *loadNames(const string &fileName, int &nameCount) {
    string *pNames = nullptr;
    string oneName = "";
    int lineTotal = 0;
    ifstream counter(fileName.c_str());

    nameCount = 0;

    if (!counter.is_open()) {
        cout << "\nCould not open " << fileName << endl;
    }
    else {
        while (getline(counter, oneName)) {
            if (countAlphabetic(oneName) > 0) {
                ++lineTotal;
            }
        }

        counter.close();

        try {
            pNames = new string[lineTotal];
        }
        catch (const bad_alloc &error) {
            cout << "\nMemory allocation failed for the array of names" << endl;
            pNames = nullptr;
        }

        if (pNames != nullptr) {
            ifstream reader(fileName.c_str());

            while (getline(reader, oneName) && nameCount < lineTotal) {
                if (countAlphabetic(oneName) > 0) {
                    pNames[nameCount] = oneName;
                    ++nameCount;
                }
            }

            reader.close();
        }
    }

    return pNames;
}

void printDivider() {
    cout << setfill(DIVIDER_CHAR) << setw(DIVIDER_WIDTH) << "" << setfill(' ') << endl;
}

/*Output
aidentsang@Aidens-MacBook-Pro CS216_L7_AT % "/Users/aidentsang/Pierce college Labs C++/CS216_L7_AT/main"


Battle Arena Menu:
1. Play Game
2. Quit
Enter your choice: 1

Enter the name of army #1 (3+ letters): A

An army name needs at least 3 letters

Enter the name of army #1 (3+ letters): ! 

An army name needs at least 3 letters

Enter the name of army #1 (3+ letters): Aid
Enter the name of army #2 (3+ letters): Sha
Enter the number of creatures per army: -1

Please enter a number between 1 and 12
Enter the number of creatures per army: !

Invalid input; please enter a whole number
Enter the number of creatures per army: 12
==============================================================================
NEW BATTLE
==============================================================================

Aid Stats before the Battle
Creature        Type          Strength    Health
Morgas          demon              101       146
Thorfin         cyberelf           239       118
Petra           elf                262        94
Karan           balrog             104       239
Seren           cyberelf            95       244
Lunara          elf                249        68
Lagnar          elf                 70        63
Orrin           demon              134       160
Quillon         elf                160       209
Morwen          balrog             145       158
Chester         demon              235       220
Ragnar          elf                 61       236
Total health of Aid: 1955

Sha Stats before the Battle
Creature        Type          Strength    Health
Kaelith         demon              224       188
Aldric          elf                217        64
Grisha          demon              198        85
Isolde          cyberelf            80        46
Dorian          elf                194       186
Hollis          cyberelf           204       118
Faelan          balrog             259       246
Cassia          balrog              53       221
Bricta          cyberelf           147        90
Varek           demon              111       197
Nimue           balrog             236        84
Osric           cyberelf           105       125
Total health of Sha: 1650

Attacker                Army           Damage   Defender                Army           Before    After

-- Duel 1: Morgas the Demon of Aid vs Kaelith the Demon of Sha --
Morgas the Demon        Aid                95   Kaelith the Demon       Sha               188       93
Kaelith the Demon       Sha               117   Morgas the Demon        Aid               146       29
Morgas the Demon        Aid                29   Kaelith the Demon       Sha                93       64
Kaelith the Demon       Sha               159   Morgas the Demon        Aid                29        0
>> Kaelith the Demon defeated Morgas the Demon

-- Duel 2: Thorfin the Cyberelf of Aid vs Aldric the Elf of Sha --
Thorfin the Cyberelf    Aid               166   Aldric the Elf          Sha                64        0
>> Thorfin the Cyberelf defeated Aldric the Elf

-- Duel 3: Petra the Elf of Aid vs Grisha the Demon of Sha --
Petra the Elf           Aid                20   Grisha the Demon        Sha                85       65
Grisha the Demon        Sha                 2   Petra the Elf           Aid                94       92
Petra the Elf           Aid                71   Grisha the Demon        Sha                65        0
>> Petra the Elf defeated Grisha the Demon

-- Duel 4: Karan the Balrog of Aid vs Isolde the Cyberelf of Sha --
Karan the Balrog        Aid                55   Isolde the Cyberelf     Sha                46        0
>> Karan the Balrog defeated Isolde the Cyberelf

-- Duel 5: Seren the Cyberelf of Aid vs Dorian the Elf of Sha --
Dorian the Elf          Sha                76   Seren the Cyberelf      Aid               244      168
Seren the Cyberelf      Aid                53   Dorian the Elf          Sha               186      133
Dorian the Elf          Sha                64   Seren the Cyberelf      Aid               168      104
Seren the Cyberelf      Aid                63   Dorian the Elf          Sha               133       70
Dorian the Elf          Sha                40   Seren the Cyberelf      Aid               104       64
Seren the Cyberelf      Aid                50   Dorian the Elf          Sha                70       20
Dorian the Elf          Sha               184   Seren the Cyberelf      Aid                64        0
>> Dorian the Elf defeated Seren the Cyberelf

-- Duel 6: Lunara the Elf of Aid vs Hollis the Cyberelf of Sha --
Hollis the Cyberelf     Sha                71   Lunara the Elf          Aid                68        0
>> Hollis the Cyberelf defeated Lunara the Elf

-- Duel 7: Lagnar the Elf of Aid vs Faelan the Balrog of Sha --
Lagnar the Elf          Aid                41   Faelan the Balrog       Sha               246      205
Faelan the Balrog       Sha               376   Lagnar the Elf          Aid                63        0
>> Faelan the Balrog defeated Lagnar the Elf

-- Duel 8: Orrin the Demon of Aid vs Cassia the Balrog of Sha --
Cassia the Balrog       Sha                35   Orrin the Demon         Aid               160      125
Orrin the Demon         Aid               166   Cassia the Balrog       Sha               221       55
Cassia the Balrog       Sha                70   Orrin the Demon         Aid               125       55
Orrin the Demon         Aid               105   Cassia the Balrog       Sha                55        0
>> Orrin the Demon defeated Cassia the Balrog

-- Duel 9: Quillon the Elf of Aid vs Bricta the Cyberelf of Sha --
Bricta the Cyberelf     Sha               113   Quillon the Elf         Aid               209       96
Quillon the Elf         Aid                76   Bricta the Cyberelf     Sha                90       14
Bricta the Cyberelf     Sha                78   Quillon the Elf         Aid                96       18
Quillon the Elf         Aid               224   Bricta the Cyberelf     Sha                14        0
>> Quillon the Elf defeated Bricta the Cyberelf

-- Duel 10: Morwen the Balrog of Aid vs Varek the Demon of Sha --
Varek the Demon         Sha                83   Morwen the Balrog       Aid               158       75
Morwen the Balrog       Aid               196   Varek the Demon         Sha               197        1
Varek the Demon         Sha                 5   Morwen the Balrog       Aid                75       70
Morwen the Balrog       Aid                95   Varek the Demon         Sha                 1        0
>> Morwen the Balrog defeated Varek the Demon

-- Duel 11: Chester the Demon of Aid vs Nimue the Balrog of Sha --
Chester the Demon       Aid                12   Nimue the Balrog        Sha                84       72
Nimue the Balrog        Sha               233   Chester the Demon       Aid               220        0
>> Nimue the Balrog defeated Chester the Demon

-- Duel 12: Ragnar the Elf of Aid vs Osric the Cyberelf of Sha --
Ragnar the Elf          Aid                55   Osric the Cyberelf      Sha               125       70
Osric the Cyberelf      Sha                91   Ragnar the Elf          Aid               236      145
Ragnar the Elf          Aid                19   Osric the Cyberelf      Sha                70       51
Osric the Cyberelf      Sha                37   Ragnar the Elf          Aid               145      108
Ragnar the Elf          Aid                18   Osric the Cyberelf      Sha                51       33
Osric the Cyberelf      Sha                98   Ragnar the Elf          Aid               108       10
Ragnar the Elf          Aid                24   Osric the Cyberelf      Sha                33        9
Osric the Cyberelf      Sha                10   Ragnar the Elf          Aid                10        0
>> Osric the Cyberelf defeated Ragnar the Elf

Aid Stats after the Battle
Creature        Type          Strength    Health
Morgas          demon              101         0
Thorfin         cyberelf           239       118
Petra           elf                262        92
Karan           balrog             104       239
Seren           cyberelf            95         0
Lunara          elf                249         0
Lagnar          elf                 70         0
Orrin           demon              134        55
Quillon         elf                160        18
Morwen          balrog             145        70
Chester         demon              235         0
Ragnar          elf                 61         0
Total health of Aid: 592

Sha Stats after the Battle
Creature        Type          Strength    Health
Kaelith         demon              224        64
Aldric          elf                217         0
Grisha          demon              198         0
Isolde          cyberelf            80         0
Dorian          elf                194        20
Hollis          cyberelf           204       118
Faelan          balrog             259       205
Cassia          balrog              53         0
Bricta          cyberelf           147         0
Varek           demon              111         0
Nimue           balrog             236        72
Osric           cyberelf           105         9
Total health of Sha: 488
==============================================================================
>>> Aid wins the battle! <<<
Aid overall health: 592
Sha overall health: 488
==============================================================================

Fight a rematch with the same creatures? 1. Yes  2. No
Enter your choice: 3

Please enter a number between 1 and 2

Fight a rematch with the same creatures? 1. Yes  2. No
Enter your choice: 1

Both armies have recovered; every creature's strength and health were re-rolled
==============================================================================
NEW BATTLE
==============================================================================

Aid Stats before the Battle
Creature        Type          Strength    Health
Morgas          demon              138        81
Thorfin         cyberelf           132        69
Petra           elf                149        69
Karan           balrog             129        50
Seren           cyberelf            50        30
Lunara          elf                 30        32
Lagnar          elf                 46       125
Orrin           demon              121        42
Quillon         elf                 99        44
Morwen          balrog             131        73
Chester         demon               77        38
Ragnar          elf                132        42
Total health of Aid: 695

Sha Stats before the Battle
Creature        Type          Strength    Health
Kaelith         demon              132        73
Aldric          elf                126        37
Grisha          demon               37        87
Isolde          cyberelf            91       133
Dorian          elf                102       110
Hollis          cyberelf            37        92
Faelan          balrog             102       138
Cassia          balrog              91        44
Bricta          cyberelf           137        38
Varek           demon               79        32
Nimue           balrog             130       139
Osric           cyberelf           100       146
Total health of Sha: 1069

Attacker                Army           Damage   Defender                Army           Before    After

-- Duel 1: Morgas the Demon of Aid vs Kaelith the Demon of Sha --
Morgas the Demon        Aid               103   Kaelith the Demon       Sha                73        0
>> Morgas the Demon defeated Kaelith the Demon

-- Duel 2: Thorfin the Cyberelf of Aid vs Aldric the Elf of Sha --
Thorfin the Cyberelf    Aid                98   Aldric the Elf          Sha                37        0
>> Thorfin the Cyberelf defeated Aldric the Elf

-- Duel 3: Petra the Elf of Aid vs Grisha the Demon of Sha --
Petra the Elf           Aid                95   Grisha the Demon        Sha                87        0
>> Petra the Elf defeated Grisha the Demon

-- Duel 4: Karan the Balrog of Aid vs Isolde the Cyberelf of Sha --
Karan the Balrog        Aid                97   Isolde the Cyberelf     Sha               133       36
Isolde the Cyberelf     Sha                30   Karan the Balrog        Aid                50       20
Karan the Balrog        Aid               177   Isolde the Cyberelf     Sha                36        0
>> Karan the Balrog defeated Isolde the Cyberelf

-- Duel 5: Seren the Cyberelf of Aid vs Dorian the Elf of Sha --
Dorian the Elf          Sha                62   Seren the Cyberelf      Aid                30        0
>> Dorian the Elf defeated Seren the Cyberelf

-- Duel 6: Lunara the Elf of Aid vs Hollis the Cyberelf of Sha --
Lunara the Elf          Aid                28   Hollis the Cyberelf     Sha                92       64
Hollis the Cyberelf     Sha               102   Lunara the Elf          Aid                32        0
>> Hollis the Cyberelf defeated Lunara the Elf

-- Duel 7: Lagnar the Elf of Aid vs Faelan the Balrog of Sha --
Lagnar the Elf          Aid                18   Faelan the Balrog       Sha               138      120
Faelan the Balrog       Sha                95   Lagnar the Elf          Aid               125       30
Lagnar the Elf          Aid                22   Faelan the Balrog       Sha               120       98
Faelan the Balrog       Sha               125   Lagnar the Elf          Aid                30        0
>> Faelan the Balrog defeated Lagnar the Elf

-- Duel 8: Orrin the Demon of Aid vs Cassia the Balrog of Sha --
Orrin the Demon         Aid               104   Cassia the Balrog       Sha                44        0
>> Orrin the Demon defeated Cassia the Balrog

-- Duel 9: Quillon the Elf of Aid vs Bricta the Cyberelf of Sha --
Quillon the Elf         Aid               182   Bricta the Cyberelf     Sha                38        0
>> Quillon the Elf defeated Bricta the Cyberelf

-- Duel 10: Morwen the Balrog of Aid vs Varek the Demon of Sha --
Varek the Demon         Sha                67   Morwen the Balrog       Aid                73        6
Morwen the Balrog       Aid               123   Varek the Demon         Sha                32        0
>> Morwen the Balrog defeated Varek the Demon

-- Duel 11: Chester the Demon of Aid vs Nimue the Balrog of Sha --
Nimue the Balrog        Sha                93   Chester the Demon       Aid                38        0
>> Nimue the Balrog defeated Chester the Demon

-- Duel 12: Ragnar the Elf of Aid vs Osric the Cyberelf of Sha --
Ragnar the Elf          Aid                44   Osric the Cyberelf      Sha               146      102
Osric the Cyberelf      Sha                24   Ragnar the Elf          Aid                42       18
Ragnar the Elf          Aid               119   Osric the Cyberelf      Sha               102        0
>> Ragnar the Elf defeated Osric the Cyberelf

Aid Stats after the Battle
Creature        Type          Strength    Health
Morgas          demon              138        81
Thorfin         cyberelf           132        69
Petra           elf                149        69
Karan           balrog             129        20
Seren           cyberelf            50         0
Lunara          elf                 30         0
Lagnar          elf                 46         0
Orrin           demon              121        42
Quillon         elf                 99        44
Morwen          balrog             131         6
Chester         demon               77         0
Ragnar          elf                132        18
Total health of Aid: 349

Sha Stats after the Battle
Creature        Type          Strength    Health
Kaelith         demon              132         0
Aldric          elf                126         0
Grisha          demon               37         0
Isolde          cyberelf            91         0
Dorian          elf                102       110
Hollis          cyberelf            37        64
Faelan          balrog             102        98
Cassia          balrog              91         0
Bricta          cyberelf           137         0
Varek           demon               79         0
Nimue           balrog             130       139
Osric           cyberelf           100         0
Total health of Sha: 411
==============================================================================
>>> Sha wins the battle! <<<
Aid overall health: 349
Sha overall health: 411
==============================================================================

Fight a rematch with the same creatures? 1. Yes  2. No
Enter your choice: !

Invalid input; please enter a whole number

Fight a rematch with the same creatures? 1. Yes  2. No
Enter your choice: 2


Battle Arena Menu:
1. Play Game
2. Quit
Enter your choice: 3

Invalid menu choice


Battle Arena Menu:
1. Play Game
2. Quit
Enter your choice: a

Invalid input; please enter a whole number


Battle Arena Menu:
1. Play Game
2. Quit
Enter your choice: 2

Thanks for playing. Goodbye! */

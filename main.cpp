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

enum MenuOption { PLAY = 1, QUIT };
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
    return name + " the " + getTypeName();
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
        int armySize = readInt("Enter the number of creatures per army between 1-12: ", MIN_ARMY_SIZE, maxSize);
        bool isReady = army1.createArmy(name1, armySize, pNames, 0);

        if (isReady) {
            isReady = army2.createArmy(name2, armySize, pNames, armySize);
        }

        if (!isReady) {
            cout << "\nThe armies could not be built; returning to the menu" << endl;
        }
        else {
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
    }
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
Enter your choice: 3

Invalid menu choice


Battle Arena Menu:
1. Play Game
2. Quit
Enter your choice: !

Invalid input; please enter a whole number


Battle Arena Menu:
1. Play Game
2. Quit
Enter your choice: a

Invalid input; please enter a whole number


Battle Arena Menu:
1. Play Game
2. Quit
Enter your choice: 1

Enter the name of army #1 (3+ letters): a

An army name needs at least 3 letters

Enter the name of army #1 (3+ letters): 2

An army name needs at least 3 letters

Enter the name of army #1 (3+ letters): !

An army name needs at least 3 letters

Enter the name of army #1 (3+ letters): Army1
Enter the name of army #2 (3+ letters): e

An army name needs at least 3 letters
Enter the name of army #2 (3+ letters): Army2
Enter the number of creatures per army: -1

Please enter a number between 1 and 12 between 1-12
Enter the number of creatures per army between 1-12: 10
==============================================================================
NEW BATTLE
==============================================================================

Army1 Stats before the Battle
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
Total health of Army1: 1499

Army2 Stats before the Battle
Creature        Type          Strength    Health
Chester         demon              235       220
Ragnar          elf                 61       236
Kaelith         demon              224       188
Aldric          elf                217        64
Grisha          demon              198        85
Isolde          cyberelf            80        46
Dorian          elf                194       186
Hollis          cyberelf           204       118
Faelan          balrog             259       246
Cassia          balrog              53       221
Total health of Army2: 1610

Attacker                Army           Damage   Defender                Army           Before    After

-- Duel 1: Morgas the demon of Army1 vs Chester the demon of Army2 --
Morgas the demon        Army1              62   Chester the demon       Army2             220      158
Chester the demon       Army2             164   Morgas the demon        Army1             146        0
>> Chester the demon defeated Morgas the demon

-- Duel 2: Thorfin the cyberelf of Army1 vs Ragnar the elf of Army2 --
Ragnar the elf          Army2              52   Thorfin the cyberelf    Army1             118       66
Thorfin the cyberelf    Army1             172   Ragnar the elf          Army2             236       64
Ragnar the elf          Army2              11   Thorfin the cyberelf    Army1              66       55
Thorfin the cyberelf    Army1              89   Ragnar the elf          Army2              64        0
>> Thorfin the cyberelf defeated Ragnar the elf

-- Duel 3: Petra the elf of Army1 vs Kaelith the demon of Army2 --
Petra the elf           Army1             127   Kaelith the demon       Army2             188       61
Kaelith the demon       Army2             159   Petra the elf           Army1              94        0
>> Kaelith the demon defeated Petra the elf

-- Duel 4: Karan the balrog of Army1 vs Aldric the elf of Army2 --
Karan the balrog        Army1              81   Aldric the elf          Army2              64        0
>> Karan the balrog defeated Aldric the elf

-- Duel 5: Seren the cyberelf of Army1 vs Grisha the demon of Army2 --
Seren the cyberelf      Army1               2   Grisha the demon        Army2              85       83
Grisha the demon        Army2               2   Seren the cyberelf      Army1             244      242
Seren the cyberelf      Army1              69   Grisha the demon        Army2              83       14
Grisha the demon        Army2              89   Seren the cyberelf      Army1             242      153
Seren the cyberelf      Army1              53   Grisha the demon        Army2              14        0
>> Seren the cyberelf defeated Grisha the demon

-- Duel 6: Lunara the elf of Army1 vs Isolde the cyberelf of Army2 --
Isolde the cyberelf     Army2              92   Lunara the elf          Army1              68        0
>> Isolde the cyberelf defeated Lunara the elf

-- Duel 7: Lagnar the elf of Army1 vs Dorian the elf of Army2 --
Dorian the elf          Army2             149   Lagnar the elf          Army1              63        0
>> Dorian the elf defeated Lagnar the elf

-- Duel 8: Orrin the demon of Army1 vs Hollis the cyberelf of Army2 --
Orrin the demon         Army1              34   Hollis the cyberelf     Army2             118       84
Hollis the cyberelf     Army2             108   Orrin the demon         Army1             160       52
Orrin the demon         Army1              56   Hollis the cyberelf     Army2              84       28
Hollis the cyberelf     Army2              13   Orrin the demon         Army1              52       39
Orrin the demon         Army1               1   Hollis the cyberelf     Army2              28       27
Hollis the cyberelf     Army2              51   Orrin the demon         Army1              39        0
>> Hollis the cyberelf defeated Orrin the demon

-- Duel 9: Quillon the elf of Army1 vs Faelan the balrog of Army2 --
Quillon the elf         Army1             145   Faelan the balrog       Army2             246      101
Faelan the balrog       Army2             265   Quillon the elf         Army1             209        0
>> Faelan the balrog defeated Quillon the elf

-- Duel 10: Morwen the balrog of Army1 vs Cassia the balrog of Army2 --
Cassia the balrog       Army2              70   Morwen the balrog       Army1             158       88
Morwen the balrog       Army1             128   Cassia the balrog       Army2             221       93
Cassia the balrog       Army2              33   Morwen the balrog       Army1              88       55
Morwen the balrog       Army1             171   Cassia the balrog       Army2              93        0
>> Morwen the balrog defeated Cassia the balrog

Army1 Stats after the Battle
Creature        Type          Strength    Health
Morgas          demon              101         0
Thorfin         cyberelf           239        55
Petra           elf                262         0
Karan           balrog             104       239
Seren           cyberelf            95       153
Lunara          elf                249         0
Lagnar          elf                 70         0
Orrin           demon              134         0
Quillon         elf                160         0
Morwen          balrog             145        55
Total health of Army1: 502

Army2 Stats after the Battle
Creature        Type          Strength    Health
Chester         demon              235       158
Ragnar          elf                 61         0
Kaelith         demon              224        61
Aldric          elf                217         0
Grisha          demon              198         0
Isolde          cyberelf            80        46
Dorian          elf                194       186
Hollis          cyberelf           204        27
Faelan          balrog             259       101
Cassia          balrog              53         0
Total health of Army2: 579
==============================================================================
>>> Army2 wins the battle! <<<
Army1 overall health: 502
Army2 overall health: 579
==============================================================================


Battle Arena Menu:
1. Play Game
2. Quit
Enter your choice: 2

Thanks for playing. Goodbye!
aidentsang@Aidens-MacBook-Pro CS216_L7_AT % */

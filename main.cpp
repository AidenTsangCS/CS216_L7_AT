// Aiden Tsang
// CS216 Lab 7 - creature inheritance and army battles

#include <cctype>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>

using namespace std;

// creature values
const string DEFAULT_NAME = "n/a";
const string INVALID_NAME = "";
const int INVALID_STAT = -1;
const int MIN_ARMY_STAT = 45;
const int MAX_ARMY_STAT = 275;
const int MIN_RESET_STAT = 30;
const int MAX_RESET_STAT = 150;
const int MIN_VALID_STRENGTH = 1;
const int MIN_VALID_HEALTH = 0;
const int DEFAULT_STAT = MIN_ARMY_STAT;
const int MIN_DAMAGE = 1;

// special attack rules
const int PERCENT_ROLL = 100;
const int DEMON_BONUS_CHANCE = 15;
const int DEMON_BONUS_DAMAGE = 40;
const int BALROG_ATTACKS = 2;
const int ELF_BONUS_CHANCE = 20;
const int ELF_MULTIPLIER = 2;
const int CYBERELF_BONUS_CHANCE = 30;
const int CYBERELF_BONUS_DAMAGE = 50;

// army values
const string DEFAULT_ARMY_NAME = "n/a";
const int DEFAULT_ARMY_SIZE = 0;
const int MIN_ARMY_SIZE = 1;
const int MAX_ARMY_SIZE = 12;
const int MIN_ARMY_NAME_LETTERS = 3;
const int MAX_ARMY_NAME_LETTERS = 9;
const int ARMIES_PER_BATTLE = 2;
const int FIRST_NAME_INDEX = 0;

// input file
const string NAMES_FILE = "in_creature_names.txt";

// screen formatting
const int ID_COL = 12;
const int TYPE_COL = 12;
const int STAT_COL = 10;
const int FIGHTER_COL = 23;
const int ARMY_COL = 10;
const int NUM_COL = 7;
const int STATS_WIDTH = ID_COL + TYPE_COL + STAT_COL + STAT_COL;
const int DUEL_WIDTH = FIGHTER_COL + ARMY_COL + NUM_COL + FIGHTER_COL + ARMY_COL + NUM_COL + NUM_COL;
const char DIVIDER_CHAR = '=';

// prompts and messages
const string MENU_PROMPT = "\n\nBattle Arena Menu:\n1. Play game\n2. Quit\nEnter your choice: ";
const string REMATCH_PROMPT = "\n\nRematch Menu:\n1. Fight again with the same creatures\n2. Back to the main menu\nEnter your choice: ";
const string SIZE_RULE = " (" + to_string(MIN_ARMY_SIZE) + " to " + to_string(MAX_ARMY_SIZE) + "): ";
const string NAME_RULE = " (" + to_string(MIN_ARMY_NAME_LETTERS) + " to " + to_string(MAX_ARMY_NAME_LETTERS) + " letters): ";
const string SIZE_PROMPT = "\nEnter the number of creatures in each army" + SIZE_RULE;
const string ARMY_ONE_PROMPT = "Enter the name of army #1" + NAME_RULE;
const string ARMY_TWO_PROMPT = "Enter the name of army #2" + NAME_RULE;
const string BEFORE_LABEL = "before the Battle";
const string AFTER_LABEL = "after the Battle";
const string INVALID_CHOICE_MESSAGE = "Invalid menu choice; please pick a listed option";

enum MenuOption
{
    PLAY = 1,
    QUIT
};

enum RematchOption
{
    REMATCH_YES = 1,
    REMATCH_NO
};

enum CreatureType
{
    CREATURE,
    DEMON,
    BALROG,
    ELF,
    CYBERELF
};

const string CREATURE_TYPE_NAMES[] = {"creature", "demon", "balrog", "elf", "cyberelf"};

class Creature
{
protected:
    string name = DEFAULT_NAME;
    int strength = DEFAULT_STAT;
    int health = DEFAULT_STAT;

private:
    void assignCreature(const string &newName, int newStrength, int newHealth);

public:
    Creature();
    Creature(const string &newName, int newStrength, int newHealth);
    virtual ~Creature();

    void setCreature(const string &newName, int newStrength, int newHealth);
    void reset();
    void takeDamage(int damageTaken);

    string getId() const;
    int getStrength() const;
    int getHealth() const;
    string getTypeName() const;
    string getName() const;
    string toString() const;

    virtual CreatureType getType() const;
    virtual int getDamage() const;
};

class Demon : public Creature
{
public:
    Demon();
    Demon(const string &newName, int newStrength, int newHealth);

    CreatureType getType() const;
    int getDamage() const;
};

class Balrog : public Creature
{
public:
    Balrog();
    Balrog(const string &newName, int newStrength, int newHealth);

    CreatureType getType() const;
    int getDamage() const;
};

class Elf : public Creature
{
public:
    Elf();
    Elf(const string &newName, int newStrength, int newHealth);

    CreatureType getType() const;
    int getDamage() const;
};

class Cyberelf : public Elf
{
public:
    Cyberelf();
    Cyberelf(const string &newName, int newStrength, int newHealth);

    CreatureType getType() const;
    int getDamage() const;
};

class Army
{
private:
    string name = DEFAULT_ARMY_NAME;
    int size = DEFAULT_ARMY_SIZE;
    Creature **ppCreatures = nullptr;

    void setArmy(const string &newName, int newSize, Creature **ppNewCreatures);
    void copyArmy(const Army &rhs);
    CreatureType randomType() const;
    Creature *createCreature(CreatureType type, const string &newName, int newStrength, int newHealth) const;
    Creature **buildCreatures(int newSize, const Army *pSource, const string *pNames, int startIndex) const;
    void releaseCreatures(Creature **&ppList, int count) const;

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

class Game
{
private:
    Army armyOne;
    Army armyTwo;

    string *loadNames(int neededCount) const;
    bool buildArmies(int armySize, const string *pNames);
    void runBattleSeries(int armySize);
    void runBattle(int armySize);
    void runDuel(int position);
    void exchangeBlows(Creature *pAttacker, const string &attackerArmy, Creature *pDefender, const string &defenderArmy) const;
    void strike(Creature *pAttacker, const string &attackerArmy, Creature *pDefender, const string &defenderArmy) const;
    void printDuelHeader() const;
    void announceDuelWinner(const Creature *pFirst, const Creature *pSecond) const;
    void announceWinner() const;

public:
    Game();

    void play();
};

void clearFailedCin(const string &errorMessage);
int readInt(const string &prompt, int minVal, int maxVal);
string readArmyName(const string &prompt);
int countAlphabetic(const string &text);
string capitalizeFirst(const string &text);
int randomInRange(int minVal, int maxVal);
bool isChanceHit(int chancePercent);
void printDivider(int width);

int main()
{
    srand(static_cast<unsigned int>(time(nullptr)));

    try
    {
        Game battleGame;
        int menuChoice = QUIT;

        do
        {
            menuChoice = readInt(MENU_PROMPT, numeric_limits<int>::min(), numeric_limits<int>::max());

            switch (menuChoice)
            {
            case PLAY:
                battleGame.play();
                break;
            case QUIT:
                cout << "\nThanks for playing. Goodbye!" << endl;
                break;
            default:
                clearFailedCin(INVALID_CHOICE_MESSAGE);
            }
        } while (menuChoice != QUIT);
    }
    catch (const bad_alloc &error)
    {
        cout << "\nThe program ran out of memory and cannot continue" << endl;
    }

    return 0;
}

// Builds a creature with default values.
// Pre: none
// Post: the creature holds the default name and stats
Creature::Creature()
{
    setCreature(DEFAULT_NAME, DEFAULT_STAT, DEFAULT_STAT);
}

// Builds a creature from the incoming values.
// Pre: none
// Post: the creature holds the incoming values when they are valid, the default values otherwise
Creature::Creature(const string &newName, int newStrength, int newHealth)
{
    setCreature(newName, newStrength, newHealth);
}

// Marks the creature as unusable when its lifetime ends.
// Pre: none
// Post: all member variables hold invalid values
Creature::~Creature()
{
    assignCreature(INVALID_NAME, INVALID_STAT, INVALID_STAT);
}

// Stores incoming values without validating them; used by the validating setter and the d'tor.
// Pre: none
// Post: all member variables hold the incoming values
void Creature::assignCreature(const string &newName, int newStrength, int newHealth)
{
    name = newName;
    strength = newStrength;
    health = newHealth;
}

// Sets every member variable of the creature.
// Pre: none
// Post: all member variables hold the incoming values, or none of them change when any value is invalid
void Creature::setCreature(const string &newName, int newStrength, int newHealth)
{
    bool isValid = countAlphabetic(newName) > 0 && newStrength >= MIN_VALID_STRENGTH && newHealth >= MIN_VALID_HEALTH;

    if (!isValid)
    {
        cout << "\nInvalid creature record; " << name << " was left unchanged" << endl;
    }
    else
    {
        assignCreature(newName, newStrength, newHealth);
    }
}

// Rolls new strength and health so the creature can fight again.
// Pre: none
// Post: strength and health hold new values between MIN_RESET_STAT and MAX_RESET_STAT
void Creature::reset()
{
    int newStrength = randomInRange(MIN_RESET_STAT, MAX_RESET_STAT);
    int newHealth = randomInRange(MIN_RESET_STAT, MAX_RESET_STAT);

    setCreature(name, newStrength, newHealth);
}

// Lowers the creature's health by the damage of one attack.
// Pre: damageTaken >= 0
// Post: health drops by damageTaken and never falls below MIN_VALID_HEALTH
void Creature::takeDamage(int damageTaken)
{
    int newHealth = health - damageTaken;

    if (newHealth < MIN_VALID_HEALTH)
    {
        newHealth = MIN_VALID_HEALTH;
    }

    setCreature(name, strength, newHealth);
}

// Returns the creature's id.
// Pre: none
// Post: the creature is unchanged
string Creature::getId() const
{
    return name;
}

// Returns the creature's strength.
// Pre: none
// Post: the creature is unchanged
int Creature::getStrength() const
{
    return strength;
}

// Returns the creature's health.
// Pre: none
// Post: the creature is unchanged
int Creature::getHealth() const
{
    return health;
}

// Returns the class name of the creature, for example "cyberelf".
// Pre: none
// Post: the creature is unchanged
string Creature::getTypeName() const
{
    return CREATURE_TYPE_NAMES[getType()];
}

// Returns the id followed by the class name, for example "Morgas the Demon".
// Pre: none
// Post: the creature is unchanged
string Creature::getName() const
{
    return name + " the " + capitalizeFirst(getTypeName());
}

// Returns one formatted table row for the creature.
// Pre: none
// Post: the creature is unchanged
string Creature::toString() const
{
    ostringstream row;

    row << left << setw(ID_COL) << name << setw(TYPE_COL) << getTypeName() << right << setw(STAT_COL) << strength << setw(STAT_COL) << health;

    return row.str();
}

// Returns the type of this object so the army can rebuild and label it.
// Pre: none
// Post: the creature is unchanged
CreatureType Creature::getType() const
{
    return CREATURE;
}

// Returns the basic damage every creature inflicts.
// Pre: none
// Post: the creature is unchanged
int Creature::getDamage() const
{
    return randomInRange(MIN_DAMAGE, strength);
}

// Builds a demon with default values.
// Pre: none
// Post: the demon holds the default name and stats
Demon::Demon() : Creature()
{
}

// Builds a demon from the incoming values.
// Pre: none
// Post: the demon holds the incoming values when they are valid, the default values otherwise
Demon::Demon(const string &newName, int newStrength, int newHealth) : Creature(newName, newStrength, newHealth)
{
}

// Returns the type of this object.
// Pre: none
// Post: the demon is unchanged
CreatureType Demon::getType() const
{
    return DEMON;
}

// Returns the basic damage plus the demonic attack bonus.
// Pre: none
// Post: the demon is unchanged
int Demon::getDamage() const
{
    int damage = Creature::getDamage();

    if (isChanceHit(DEMON_BONUS_CHANCE))
    {
        damage = damage + DEMON_BONUS_DAMAGE;
    }

    return damage;
}

// Builds a balrog with default values.
// Pre: none
// Post: the balrog holds the default name and stats
Balrog::Balrog() : Creature()
{
}

// Builds a balrog from the incoming values.
// Pre: none
// Post: the balrog holds the incoming values when they are valid, the default values otherwise
Balrog::Balrog(const string &newName, int newStrength, int newHealth) : Creature(newName, newStrength, newHealth)
{
}

// Returns the type of this object.
// Pre: none
// Post: the balrog is unchanged
CreatureType Balrog::getType() const
{
    return BALROG;
}

// Returns the damage of the two attacks a balrog lands in one turn.
// Pre: none
// Post: the balrog is unchanged
int Balrog::getDamage() const
{
    int damage = 0;

    for (int strikeCount = 0; strikeCount < BALROG_ATTACKS; ++strikeCount)
    {
        damage = damage + Creature::getDamage();
    }

    return damage;
}

// Builds an elf with default values.
// Pre: none
// Post: the elf holds the default name and stats
Elf::Elf() : Creature()
{
}

// Builds an elf from the incoming values.
// Pre: none
// Post: the elf holds the incoming values when they are valid, the default values otherwise
Elf::Elf(const string &newName, int newStrength, int newHealth) : Creature(newName, newStrength, newHealth)
{
}

// Returns the type of this object.
// Pre: none
// Post: the elf is unchanged
CreatureType Elf::getType() const
{
    return ELF;
}

// Returns the basic damage, doubled when the magical attack lands.
// Pre: none
// Post: the elf is unchanged
int Elf::getDamage() const
{
    int damage = Creature::getDamage();

    if (isChanceHit(ELF_BONUS_CHANCE))
    {
        damage = damage * ELF_MULTIPLIER;
    }

    return damage;
}

// Builds a cyberelf with default values.
// Pre: none
// Post: the cyberelf holds the default name and stats
Cyberelf::Cyberelf() : Elf()
{
}

// Builds a cyberelf from the incoming values.
// Pre: none
// Post: the cyberelf holds the incoming values when they are valid, the default values otherwise
Cyberelf::Cyberelf(const string &newName, int newStrength, int newHealth) : Elf(newName, newStrength, newHealth)
{
}

// Returns the type of this object.
// Pre: none
// Post: the cyberelf is unchanged
CreatureType Cyberelf::getType() const
{
    return CYBERELF;
}

// Returns the elf damage plus the cyber attack bonus.
// Pre: none
// Post: the cyberelf is unchanged
int Cyberelf::getDamage() const
{
    int damage = Elf::getDamage();

    if (isChanceHit(CYBERELF_BONUS_CHANCE))
    {
        damage = damage + CYBERELF_BONUS_DAMAGE;
    }

    return damage;
}

// Builds an empty army.
// Pre: none
// Post: the army holds the default name, no creatures, and a null pointer
Army::Army()
{
    setArmy(DEFAULT_ARMY_NAME, DEFAULT_ARMY_SIZE, nullptr);
}

// Builds an army that owns its own copy of every creature of the incoming army.
// Pre: none
// Post: the army is a full copy of rhs, or an empty army when memory runs out
Army::Army(const Army &rhs)
{
    copyArmy(rhs);
}

// Releases every creature the army owns.
// Pre: none
// Post: all dynamic memory is released and all member variables hold invalid values
Army::~Army()
{
    releaseCreatures(ppCreatures, size);
    setArmy(INVALID_NAME, DEFAULT_ARMY_SIZE, nullptr);
}

// Replaces this army with a copy of the incoming army.
// Pre: none
// Post: this army is a full copy of rhs, or it is left unchanged when memory runs out
Army &Army::operator=(const Army &rhs)
{
    if (this != &rhs)
    {
        copyArmy(rhs);
    }

    return *this;
}

// Stores incoming values without validating them; used by the c'tors and the builders.
// Pre: ppNewCreatures points to newSize creatures or is nullptr
// Post: all member variables hold the incoming values
void Army::setArmy(const string &newName, int newSize, Creature **ppNewCreatures)
{
    name = newName;
    size = newSize;
    ppCreatures = ppNewCreatures;
}

// Copies the incoming army; shared by the copy c'tor and the copy assignment operator.
// Pre: none
// Post: this army owns a copy of every creature of rhs, or it is left unchanged when memory runs out
void Army::copyArmy(const Army &rhs)
{
    if (rhs.ppCreatures == nullptr || rhs.size < MIN_ARMY_SIZE)
    {
        releaseCreatures(ppCreatures, size);
        setArmy(rhs.name, DEFAULT_ARMY_SIZE, nullptr);
    }
    else
    {
        Creature **ppTemp = buildCreatures(rhs.size, &rhs, nullptr, FIRST_NAME_INDEX);

        if (ppTemp == nullptr)
        {
            cout << "\nThe army could not be copied; " << name << " was left unchanged" << endl;
        }
        else
        {
            releaseCreatures(ppCreatures, size);
            setArmy(rhs.name, rhs.size, ppTemp);
        }
    }
}

// Picks the type of the next creature to create.
// Pre: none
// Post: the army is unchanged
CreatureType Army::randomType() const
{
    return static_cast<CreatureType>(randomInRange(DEMON, CYBERELF));
}

// Creates one creature of the requested type.
// Pre: none
// Post: returns the address of a new creature; throws bad_alloc when memory runs out
Creature *Army::createCreature(CreatureType type, const string &newName, int newStrength, int newHealth) const
{
    Creature *pNew = nullptr;

    switch (type)
    {
    case DEMON:
        pNew = new Demon(newName, newStrength, newHealth);
        break;
    case BALROG:
        pNew = new Balrog(newName, newStrength, newHealth);
        break;
    case ELF:
        pNew = new Elf(newName, newStrength, newHealth);
        break;
    case CYBERELF:
        pNew = new Cyberelf(newName, newStrength, newHealth);
        break;
    case CREATURE:
        pNew = new Creature(newName, newStrength, newHealth);
        break;
    }

    return pNew;
}

// Allocates the array of pointers and every creature in it, and handles its own allocation failures.
// Pre: newSize >= MIN_ARMY_SIZE; pSource holds newSize creatures, or pNames holds newSize unused names from startIndex
// Post: returns a full array of newSize creatures, or nullptr with all partial memory released
Creature **Army::buildCreatures(int newSize, const Army *pSource, const string *pNames, int startIndex) const
{
    Creature **ppTemp = nullptr;

    try
    {
        ppTemp = new Creature *[newSize]
        { nullptr };

        for (int i = 0; i < newSize; ++i)
        {
            if (pSource == nullptr)
            {
                ppTemp[i] = createCreature(randomType(), pNames[startIndex + i], randomInRange(MIN_ARMY_STAT, MAX_ARMY_STAT), randomInRange(MIN_ARMY_STAT, MAX_ARMY_STAT));
            }
            else
            {
                Creature *pOriginal = pSource->ppCreatures[i];
                ppTemp[i] = createCreature(pOriginal->getType(), pOriginal->getId(), pOriginal->getStrength(), pOriginal->getHealth());
            }
        }
    }
    catch (const bad_alloc &error)
    {
        releaseCreatures(ppTemp, newSize);
        cout << "\nMemory allocation failed while raising an army of " << newSize << " creatures" << endl;
    }

    return ppTemp;
}

// Releases an array of creatures and the array of pointers that holds them.
// Pre: ppList points to count creatures or is nullptr
// Post: all dynamic memory is released and ppList holds nullptr
void Army::releaseCreatures(Creature **&ppList, int count) const
{
    if (ppList != nullptr)
    {
        for (int i = 0; i < count; ++i)
        {
            delete ppList[i];
            ppList[i] = nullptr;
        }

        delete[] ppList;
        ppList = nullptr;
    }
}

// Raises a new army of randomly chosen creatures.
// Pre: pNames holds newSize unused names starting at startIndex
// Post: returns true and the army holds newSize new creatures, or returns false and the army is unchanged
bool Army::createArmy(const string &newName, int newSize, const string *pNames, int startIndex)
{
    bool isSuccess = false;
    bool isValid = countAlphabetic(newName) >= MIN_ARMY_NAME_LETTERS && newSize >= MIN_ARMY_SIZE && newSize <= MAX_ARMY_SIZE && pNames != nullptr && startIndex >= FIRST_NAME_INDEX;

    if (!isValid)
    {
        cout << "\nInvalid army record; the army was not created" << endl;
    }
    else
    {
        Creature **ppTemp = buildCreatures(newSize, nullptr, pNames, startIndex);

        if (ppTemp == nullptr)
        {
            cout << "\nThe army could not be raised; " << name << " was left unchanged" << endl;
        }
        else
        {
            releaseCreatures(ppCreatures, size);
            setArmy(newName, newSize, ppTemp);
            isSuccess = true;
        }
    }

    return isSuccess;
}

// Heals every creature so the same army can fight another battle.
// Pre: none
// Post: every creature holds new strength and health values
void Army::resetCreatures()
{
    if (ppCreatures != nullptr)
    {
        for (int i = 0; i < size; ++i)
        {
            ppCreatures[i]->reset();
        }
    }
}

// Returns the army's name.
// Pre: none
// Post: the army is unchanged
string Army::getName() const
{
    return name;
}

// Returns the number of creatures in the army.
// Pre: none
// Post: the army is unchanged
int Army::getSize() const
{
    return size;
}

// Returns the sum of the health of every creature in the army.
// Pre: none
// Post: the army is unchanged
int Army::getTotalHealth() const
{
    int total = 0;

    if (ppCreatures != nullptr)
    {
        for (int i = 0; i < size; ++i)
        {
            total = total + ppCreatures[i]->getHealth();
        }
    }

    return total;
}

// Returns the creature standing in the requested position.
// Pre: none
// Post: returns nullptr when the position is outside the army
Creature *Army::getCreature(int index) const
{
    Creature *pFound = nullptr;

    if (ppCreatures != nullptr && index >= FIRST_NAME_INDEX && index < size)
    {
        pFound = ppCreatures[index];
    }

    return pFound;
}

// Prints the army roster as a table.
// Pre: none
// Post: the army is unchanged
void Army::print(const string &label) const
{
    cout << "\n"
         << name << " Stats " << label << endl;

    if (ppCreatures == nullptr || size < MIN_ARMY_SIZE)
    {
        cout << "This army is empty" << endl;
    }
    else
    {
        cout << left << setw(ID_COL) << "Creature" << setw(TYPE_COL) << "Type" << right << setw(STAT_COL) << "Strength" << setw(STAT_COL) << "Health" << endl;
        printDivider(STATS_WIDTH);

        for (int i = 0; i < size; ++i)
        {
            cout << ppCreatures[i]->toString() << endl;
        }

        printDivider(STATS_WIDTH);
        cout << "Overall health of " << name << ": " << getTotalHealth() << endl;
    }
}

// Builds a game that holds two empty armies.
// Pre: none
// Post: both armies are empty
Game::Game()
{
}

// Runs one visit to the Battle menu option.
// Pre: none
// Post: the armies fight as long as the user asks for rematches
void Game::play()
{
    int armySize = readInt(SIZE_PROMPT, MIN_ARMY_SIZE, MAX_ARMY_SIZE);
    string *pNames = loadNames(armySize * ARMIES_PER_BATTLE);

    if (pNames == nullptr)
    {
        cout << "\nThe battle was cancelled" << endl;
    }
    else
    {
        bool isReady = buildArmies(armySize, pNames);

        delete[] pNames;
        pNames = nullptr;

        if (isReady)
        {
            runBattleSeries(armySize);
        }
        else
        {
            cout << "\nThe armies could not be raised; returning to the main menu" << endl;
        }
    }
}

// Reads as many creature names as the two armies need.
// Pre: neededCount > 0
// Post: returns an array of neededCount names, or nullptr when the file or the memory is unavailable
string *Game::loadNames(int neededCount) const
{
    string *pNames = nullptr;
    string oneName = DEFAULT_NAME;
    int nameCount = 0;
    ifstream namesFile(NAMES_FILE.c_str());

    if (!namesFile.is_open())
    {
        cout << "\nCould not open " << NAMES_FILE << endl;
    }
    else
    {
        try
        {
            pNames = new string[neededCount];

            while (nameCount < neededCount && getline(namesFile, oneName))
            {
                if (countAlphabetic(oneName) > 0)
                {
                    pNames[nameCount] = oneName;
                    ++nameCount;
                }
            }

            if (nameCount < neededCount)
            {
                delete[] pNames;
                pNames = nullptr;
                cout << "\n"
                     << NAMES_FILE << " holds only " << nameCount << " of the " << neededCount << " names needed" << endl;
            }
        }
        catch (const bad_alloc &error)
        {
            pNames = nullptr;
            cout << "\nMemory allocation failed while reading the creature names" << endl;
        }

        namesFile.close();
    }

    return pNames;
}

// Names and raises both armies.
// Pre: pNames holds armySize * ARMIES_PER_BATTLE names
// Post: returns true when both armies are ready to fight
bool Game::buildArmies(int armySize, const string *pNames)
{
    string nameOne = readArmyName(ARMY_ONE_PROMPT);
    string nameTwo = readArmyName(ARMY_TWO_PROMPT);
    bool isReady = armyOne.createArmy(nameOne, armySize, pNames, FIRST_NAME_INDEX);

    if (isReady)
    {
        isReady = armyTwo.createArmy(nameTwo, armySize, pNames, armySize);
    }

    return isReady;
}

// Fights battles with the same two armies until the user leaves the Rematch menu.
// Pre: both armies hold armySize creatures
// Post: the creatures of both armies hold the results of the last battle
void Game::runBattleSeries(int armySize)
{
    int rematchChoice = REMATCH_YES;

    while (rematchChoice == REMATCH_YES)
    {
        runBattle(armySize);
        rematchChoice = readInt(REMATCH_PROMPT, REMATCH_YES, REMATCH_NO);

        switch (rematchChoice)
        {
        case REMATCH_YES:
            armyOne.resetCreatures();
            armyTwo.resetCreatures();
            cout << "\nBoth armies recovered; every strength and health was rolled again" << endl;
            break;
        case REMATCH_NO:
            cout << "\nReturning to the main menu" << endl;
            break;
        }
    }
}

// Fights one full battle between the two armies.
// Pre: both armies hold armySize creatures
// Post: the roster of each army is printed before and after the duels and a winner is announced
void Game::runBattle(int armySize)
{
    cout << "\n";
    printDivider(DUEL_WIDTH);
    cout << "NEW BATTLE" << endl;
    printDivider(DUEL_WIDTH);

    armyOne.print(BEFORE_LABEL);
    armyTwo.print(BEFORE_LABEL);
    printDuelHeader();

    for (int position = 0; position < armySize; ++position)
    {
        runDuel(position);
    }

    armyOne.print(AFTER_LABEL);
    armyTwo.print(AFTER_LABEL);
    announceWinner();
}

// Prints the column headings of the duel table.
// Pre: none
// Post: the game is unchanged
void Game::printDuelHeader() const
{
    cout << "\n"
         << left << setw(FIGHTER_COL) << "Attacker" << setw(ARMY_COL) << "Army" << right << setw(NUM_COL) << "Damage" << "  " << left << setw(FIGHTER_COL) << "Defender" << setw(ARMY_COL) << "Army" << right << setw(NUM_COL) << "Before" << setw(NUM_COL) << "After" << endl;
}

// Fights the two creatures standing in the same position until one of them falls.
// Pre: none
// Post: the loser of the duel holds a health of MIN_VALID_HEALTH
void Game::runDuel(int position)
{
    Creature *pFirst = armyOne.getCreature(position);
    Creature *pSecond = armyTwo.getCreature(position);

    if (pFirst == nullptr || pSecond == nullptr)
    {
        cout << "\nDuel " << (position + 1) << " could not start; a creature is missing" << endl;
    }
    else
    {
        cout << "\n-- Duel " << (position + 1) << ": " << pFirst->getName() << " of " << armyOne.getName() << " vs " << pSecond->getName() << " of " << armyTwo.getName() << " --" << endl;

        if (isChanceHit(PERCENT_ROLL / ARMIES_PER_BATTLE))
        {
            exchangeBlows(pFirst, armyOne.getName(), pSecond, armyTwo.getName());
        }
        else
        {
            exchangeBlows(pSecond, armyTwo.getName(), pFirst, armyOne.getName());
        }

        announceDuelWinner(pFirst, pSecond);
    }
}

// Lets the two creatures attack in turn until one of them falls.
// Pre: both creatures hold a health above MIN_VALID_HEALTH
// Post: one of the creatures holds a health of MIN_VALID_HEALTH
void Game::exchangeBlows(Creature *pAttacker, const string &attackerArmy, Creature *pDefender, const string &defenderArmy) const
{
    Creature *pCurrent = pAttacker;
    Creature *pTarget = pDefender;
    string currentArmy = attackerArmy;
    string targetArmy = defenderArmy;

    while (pCurrent->getHealth() > MIN_VALID_HEALTH && pTarget->getHealth() > MIN_VALID_HEALTH)
    {
        strike(pCurrent, currentArmy, pTarget, targetArmy);

        Creature *pNextAttacker = pTarget;
        string nextArmy = targetArmy;

        pTarget = pCurrent;
        targetArmy = currentArmy;
        pCurrent = pNextAttacker;
        currentArmy = nextArmy;
    }
}

// Lands one attack and prints the result as a table row.
// Pre: none
// Post: the health of the defender drops by the damage of the attack
void Game::strike(Creature *pAttacker, const string &attackerArmy, Creature *pDefender, const string &defenderArmy) const
{
    int damage = pAttacker->getDamage();
    int healthBefore = pDefender->getHealth();

    pDefender->takeDamage(damage);

    cout << left << setw(FIGHTER_COL) << pAttacker->getName() << setw(ARMY_COL) << attackerArmy << right << setw(NUM_COL) << damage << "  " << left << setw(FIGHTER_COL) << pDefender->getName() << setw(ARMY_COL) << defenderArmy << right << setw(NUM_COL) << healthBefore << setw(NUM_COL) << pDefender->getHealth() << endl;
}

// Announces the creature that survived the duel.
// Pre: one of the creatures holds a health of MIN_VALID_HEALTH
// Post: the game is unchanged
void Game::announceDuelWinner(const Creature *pFirst, const Creature *pSecond) const
{
    const Creature *pWinner = pSecond;
    const Creature *pLoser = pFirst;

    if (pFirst->getHealth() > MIN_VALID_HEALTH)
    {
        pWinner = pFirst;
        pLoser = pSecond;
    }

    cout << ">> " << pWinner->getName() << " defeated " << pLoser->getName() << endl;
}

// Announces the army with the larger overall health.
// Pre: none
// Post: the game is unchanged
void Game::announceWinner() const
{
    int totalOne = armyOne.getTotalHealth();
    int totalTwo = armyTwo.getTotalHealth();
    string resultMessage = ">>> The battle ends in a tie <<<";

    if (totalOne > totalTwo)
    {
        resultMessage = ">>> " + armyOne.getName() + " wins the battle <<<";
    }
    else if (totalTwo > totalOne)
    {
        resultMessage = ">>> " + armyTwo.getName() + " wins the battle <<<";
    }

    printDivider(DUEL_WIDTH);
    cout << resultMessage << "\n"
         << armyOne.getName() << " overall health: " << totalOne << "\n"
         << armyTwo.getName() << " overall health: " << totalTwo << endl;
    printDivider(DUEL_WIDTH);
}

// Clears a failed input stream and explains the problem to the user.
// Pre: none
// Post: cin is ready for the next read
void clearFailedCin(const string &errorMessage)
{
    if (cin.fail())
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    cout << "\n"
         << errorMessage << endl;
}

// Reads one whole number between the two limits.
// Pre: minVal <= maxVal
// Post: returns a number between minVal and maxVal
int readInt(const string &prompt, int minVal, int maxVal)
{
    int inputVal = 0;
    bool isValid = false;

    while (!isValid)
    {
        cout << prompt;
        cin >> inputVal;

        if (cin.fail())
        {
            clearFailedCin("Invalid entry; please enter a whole number");
        }
        else if (inputVal < minVal || inputVal > maxVal)
        {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\nPlease enter a whole number from " << minVal << " to " << maxVal << endl;
        }
        else
        {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            isValid = true;
        }
    }

    return inputVal;
}

// Reads the name of one army.
// Pre: none
// Post: returns a name that holds a valid number of letters
string readArmyName(const string &prompt)
{
    string inputName = DEFAULT_NAME;
    bool isValid = false;

    while (!isValid)
    {
        cout << prompt;
        getline(cin, inputName);

        int letterCount = countAlphabetic(inputName);

        if (letterCount < MIN_ARMY_NAME_LETTERS || letterCount > MAX_ARMY_NAME_LETTERS)
        {
            cout << "\nAn army name needs from " << MIN_ARMY_NAME_LETTERS << " to " << MAX_ARMY_NAME_LETTERS << " letters" << endl;
        }
        else
        {
            isValid = true;
        }
    }

    return inputName;
}

// Counts the letters of the incoming text.
// Pre: none
// Post: returns the number of alphabetic characters
int countAlphabetic(const string &text)
{
    int count = 0;
    int length = static_cast<int>(text.length());

    for (int i = 0; i < length; ++i)
    {
        if (isalpha(static_cast<unsigned char>(text[i])))
        {
            ++count;
        }
    }

    return count;
}

// Returns the incoming text with an upper case first letter.
// Pre: none
// Post: the incoming text is unchanged
string capitalizeFirst(const string &text)
{
    string capitalized = text;

    if (capitalized.length() > 0)
    {
        capitalized[0] = static_cast<char>(toupper(static_cast<unsigned char>(capitalized[0])));
    }

    return capitalized;
}

// Returns a random whole number between the two limits.
// Pre: minVal <= maxVal
// Post: returns a number from minVal to maxVal
int randomInRange(int minVal, int maxVal)
{
    return (rand() % (maxVal - minVal + 1)) + minVal;
}

// Reports whether a percentage roll landed inside the incoming chance.
// Pre: chancePercent is from 0 to PERCENT_ROLL
// Post: returns true chancePercent times out of PERCENT_ROLL
bool isChanceHit(int chancePercent)
{
    return randomInRange(MIN_DAMAGE, PERCENT_ROLL) <= chancePercent;
}

// Prints a divider line of the incoming width.
// Pre: width > 0
// Post: cout is restored to the space fill character
void printDivider(int width)
{
    cout << setfill(DIVIDER_CHAR) << setw(width) << "" << setfill(' ') << endl;
}

/* Test run
aidentsang@Aidens-MacBook-Pro CS216_L7_AT % "/Users/aidentsang/Pierce college Labs C++/CS216_L7_AT/main"


Battle Arena Menu:
1. Play game
2. Quit
Enter your choice: 1

Enter the number of creatures in each army (1 to 12): ^C
aidentsang@Aidens-MacBook-Pro CS216_L7_AT % "/Users/aidentsang/Pierce college Labs C++/CS216_L7_AT/main"


Battle Arena Menu:
1. Play game
2. Quit
Enter your choice: !

Invalid entry; please enter a whole number


Battle Arena Menu:
1. Play game
2. Quit
Enter your choice: a

Invalid entry; please enter a whole number


Battle Arena Menu:
1. Play game
2. Quit
Enter your choice: 1

Enter the number of creatures in each army (1 to 12): -1

Please enter a whole number from 1 to 12

Enter the number of creatures in each army (1 to 12): 13

Please enter a whole number from 1 to 12

Enter the number of creatures in each army (1 to 12): 12
Enter the name of army #1 (3 to 9 letters): Hi

An army name needs from 3 to 9 letters
Enter the name of army #1 (3 to 9 letters): Aiden's
Enter the name of army #2 (3 to 9 letters): Kevin's

=======================================================================================
NEW BATTLE
=======================================================================================

Aiden's Stats before the Battle
Creature    Type          Strength    Health
============================================
Morgas      cyberelf           166       116
Thorfin     balrog              60       171
Petra       balrog             269        67
Karan       demon               93       266
Seren       demon              168       265
Lunara      balrog             205       182
Lagnar      elf                 59        60
Orrin       demon               82        92
Quillon     demon               82       137
Morwen      cyberelf           118       114
Chester     cyberelf           144        81
Ragnar      elf                263       170
============================================
Overall health of Aiden's: 1721

Kevin's Stats before the Battle
Creature    Type          Strength    Health
============================================
Kaelith     cyberelf           231       138
Aldric      elf                 85        56
Grisha      cyberelf           253       105
Isolde      balrog             121       214
Dorian      demon              134        52
Hollis      demon              171       140
Faelan      balrog             131       172
Cassia      demon               71       153
Bricta      balrog              46       238
Varek       elf                 90       151
Nimue       balrog             228       178
Osric       demon              126       197
============================================
Overall health of Kevin's: 1794

Attacker               Army       Damage  Defender               Army       Before  After

-- Duel 1: Morgas the Cyberelf of Aiden's vs Kaelith the Cyberelf of Kevin's --
Kaelith the Cyberelf   Kevin's       209  Morgas the Cyberelf    Aiden's       116      0
>> Kaelith the Cyberelf defeated Morgas the Cyberelf

-- Duel 2: Thorfin the Balrog of Aiden's vs Aldric the Elf of Kevin's --
Thorfin the Balrog     Aiden's        73  Aldric the Elf         Kevin's        56      0
>> Thorfin the Balrog defeated Aldric the Elf

-- Duel 3: Petra the Balrog of Aiden's vs Grisha the Cyberelf of Kevin's --
Petra the Balrog       Aiden's       177  Grisha the Cyberelf    Kevin's       105      0
>> Petra the Balrog defeated Grisha the Cyberelf

-- Duel 4: Karan the Demon of Aiden's vs Isolde the Balrog of Kevin's --
Karan the Demon        Aiden's        38  Isolde the Balrog      Kevin's       214    176
Isolde the Balrog      Kevin's       122  Karan the Demon        Aiden's       266    144
Karan the Demon        Aiden's        20  Isolde the Balrog      Kevin's       176    156
Isolde the Balrog      Kevin's       155  Karan the Demon        Aiden's       144      0
>> Isolde the Balrog defeated Karan the Demon

-- Duel 5: Seren the Demon of Aiden's vs Dorian the Demon of Kevin's --
Dorian the Demon       Kevin's       100  Seren the Demon        Aiden's       265    165
Seren the Demon        Aiden's        16  Dorian the Demon       Kevin's        52     36
Dorian the Demon       Kevin's        78  Seren the Demon        Aiden's       165     87
Seren the Demon        Aiden's       152  Dorian the Demon       Kevin's        36      0
>> Seren the Demon defeated Dorian the Demon

-- Duel 6: Lunara the Balrog of Aiden's vs Hollis the Demon of Kevin's --
Lunara the Balrog      Aiden's       322  Hollis the Demon       Kevin's       140      0
>> Lunara the Balrog defeated Hollis the Demon

-- Duel 7: Lagnar the Elf of Aiden's vs Faelan the Balrog of Kevin's --
Faelan the Balrog      Kevin's        63  Lagnar the Elf         Aiden's        60      0
>> Faelan the Balrog defeated Lagnar the Elf

-- Duel 8: Orrin the Demon of Aiden's vs Cassia the Demon of Kevin's --
Cassia the Demon       Kevin's        49  Orrin the Demon        Aiden's        92     43
Orrin the Demon        Aiden's        21  Cassia the Demon       Kevin's       153    132
Cassia the Demon       Kevin's        34  Orrin the Demon        Aiden's        43      9
Orrin the Demon        Aiden's        54  Cassia the Demon       Kevin's       132     78
Cassia the Demon       Kevin's        57  Orrin the Demon        Aiden's         9      0
>> Cassia the Demon defeated Orrin the Demon

-- Duel 9: Quillon the Demon of Aiden's vs Bricta the Balrog of Kevin's --
Quillon the Demon      Aiden's        50  Bricta the Balrog      Kevin's       238    188
Bricta the Balrog      Kevin's        18  Quillon the Demon      Aiden's       137    119
Quillon the Demon      Aiden's        75  Bricta the Balrog      Kevin's       188    113
Bricta the Balrog      Kevin's        36  Quillon the Demon      Aiden's       119     83
Quillon the Demon      Aiden's        82  Bricta the Balrog      Kevin's       113     31
Bricta the Balrog      Kevin's        74  Quillon the Demon      Aiden's        83      9
Quillon the Demon      Aiden's        50  Bricta the Balrog      Kevin's        31      0
>> Quillon the Demon defeated Bricta the Balrog

-- Duel 10: Morwen the Cyberelf of Aiden's vs Varek the Elf of Kevin's --
Varek the Elf          Kevin's        78  Morwen the Cyberelf    Aiden's       114     36
Morwen the Cyberelf    Aiden's        85  Varek the Elf          Kevin's       151     66
Varek the Elf          Kevin's       140  Morwen the Cyberelf    Aiden's        36      0
>> Varek the Elf defeated Morwen the Cyberelf

-- Duel 11: Chester the Cyberelf of Aiden's vs Nimue the Balrog of Kevin's --
Chester the Cyberelf   Aiden's       134  Nimue the Balrog       Kevin's       178     44
Nimue the Balrog       Kevin's       288  Chester the Cyberelf   Aiden's        81      0
>> Nimue the Balrog defeated Chester the Cyberelf

-- Duel 12: Ragnar the Elf of Aiden's vs Osric the Demon of Kevin's --
Osric the Demon        Kevin's        42  Ragnar the Elf         Aiden's       170    128
Ragnar the Elf         Aiden's       120  Osric the Demon        Kevin's       197     77
Osric the Demon        Kevin's       102  Ragnar the Elf         Aiden's       128     26
Ragnar the Elf         Aiden's       148  Osric the Demon        Kevin's        77      0
>> Ragnar the Elf defeated Osric the Demon

Aiden's Stats after the Battle
Creature    Type          Strength    Health
============================================
Morgas      cyberelf           166         0
Thorfin     balrog              60       171
Petra       balrog             269        67
Karan       demon               93         0
Seren       demon              168        87
Lunara      balrog             205       182
Lagnar      elf                 59         0
Orrin       demon               82         0
Quillon     demon               82         9
Morwen      cyberelf           118         0
Chester     cyberelf           144         0
Ragnar      elf                263        26
============================================
Overall health of Aiden's: 542

Kevin's Stats after the Battle
Creature    Type          Strength    Health
============================================
Kaelith     cyberelf           231       138
Aldric      elf                 85         0
Grisha      cyberelf           253         0
Isolde      balrog             121       156
Dorian      demon              134         0
Hollis      demon              171         0
Faelan      balrog             131       172
Cassia      demon               71        78
Bricta      balrog              46         0
Varek       elf                 90        66
Nimue       balrog             228        44
Osric       demon              126         0
============================================
Overall health of Kevin's: 654
=======================================================================================
>>> Kevin's wins the battle <<<
Aiden's overall health: 542
Kevin's overall health: 654
=======================================================================================


Rematch Menu:
1. Fight again with the same creatures
2. Back to the main menu
Enter your choice: 3

Please enter a whole number from 1 to 2


Rematch Menu:
1. Fight again with the same creatures
2. Back to the main menu
Enter your choice: !

Invalid entry; please enter a whole number


Rematch Menu:
1. Fight again with the same creatures
2. Back to the main menu
Enter your choice: 1

Both armies recovered; every strength and health was rolled again

=======================================================================================
NEW BATTLE
=======================================================================================

Aiden's Stats before the Battle
Creature    Type          Strength    Health
============================================
Morgas      cyberelf            31        76
Thorfin     balrog             110        44
Petra       balrog              53        58
Karan       demon               56        86
Seren       demon              116       111
Lunara      balrog              71       121
Lagnar      elf                134       105
Orrin       demon              114        80
Quillon     demon              130       141
Morwen      cyberelf           119        46
Chester     cyberelf           128        58
Ragnar      elf                128       120
============================================
Overall health of Aiden's: 1046

Kevin's Stats before the Battle
Creature    Type          Strength    Health
============================================
Kaelith     cyberelf           139       118
Aldric      elf                 70       149
Grisha      cyberelf           137        86
Isolde      balrog              84        54
Dorian      demon              141        84
Hollis      demon               40        86
Faelan      balrog              55        99
Cassia      demon              138        90
Bricta      balrog             112        97
Varek       elf                 37       116
Nimue       balrog              87        49
Osric       demon               76        63
============================================
Overall health of Kevin's: 1091

Attacker               Army       Damage  Defender               Army       Before  After

-- Duel 1: Morgas the Cyberelf of Aiden's vs Kaelith the Cyberelf of Kevin's --
Kaelith the Cyberelf   Kevin's       104  Morgas the Cyberelf    Aiden's        76      0
>> Kaelith the Cyberelf defeated Morgas the Cyberelf

-- Duel 2: Thorfin the Balrog of Aiden's vs Aldric the Elf of Kevin's --
Thorfin the Balrog     Aiden's       184  Aldric the Elf         Kevin's       149      0
>> Thorfin the Balrog defeated Aldric the Elf

-- Duel 3: Petra the Balrog of Aiden's vs Grisha the Cyberelf of Kevin's --
Petra the Balrog       Aiden's        57  Grisha the Cyberelf    Kevin's        86     29
Grisha the Cyberelf    Kevin's        11  Petra the Balrog       Aiden's        58     47
Petra the Balrog       Aiden's        57  Grisha the Cyberelf    Kevin's        29      0
>> Petra the Balrog defeated Grisha the Cyberelf

-- Duel 4: Karan the Demon of Aiden's vs Isolde the Balrog of Kevin's --
Isolde the Balrog      Kevin's        88  Karan the Demon        Aiden's        86      0
>> Isolde the Balrog defeated Karan the Demon

-- Duel 5: Seren the Demon of Aiden's vs Dorian the Demon of Kevin's --
Dorian the Demon       Kevin's        79  Seren the Demon        Aiden's       111     32
Seren the Demon        Aiden's        36  Dorian the Demon       Kevin's        84     48
Dorian the Demon       Kevin's        88  Seren the Demon        Aiden's        32      0
>> Dorian the Demon defeated Seren the Demon

-- Duel 6: Lunara the Balrog of Aiden's vs Hollis the Demon of Kevin's --
Lunara the Balrog      Aiden's        87  Hollis the Demon       Kevin's        86      0
>> Lunara the Balrog defeated Hollis the Demon

-- Duel 7: Lagnar the Elf of Aiden's vs Faelan the Balrog of Kevin's --
Faelan the Balrog      Kevin's        35  Lagnar the Elf         Aiden's       105     70
Lagnar the Elf         Aiden's        10  Faelan the Balrog      Kevin's        99     89
Faelan the Balrog      Kevin's        61  Lagnar the Elf         Aiden's        70      9
Lagnar the Elf         Aiden's        86  Faelan the Balrog      Kevin's        89      3
Faelan the Balrog      Kevin's        51  Lagnar the Elf         Aiden's         9      0
>> Faelan the Balrog defeated Lagnar the Elf

-- Duel 8: Orrin the Demon of Aiden's vs Cassia the Demon of Kevin's --
Cassia the Demon       Kevin's       124  Orrin the Demon        Aiden's        80      0
>> Cassia the Demon defeated Orrin the Demon

-- Duel 9: Quillon the Demon of Aiden's vs Bricta the Balrog of Kevin's --
Bricta the Balrog      Kevin's       121  Quillon the Demon      Aiden's       141     20
Quillon the Demon      Aiden's        64  Bricta the Balrog      Kevin's        97     33
Bricta the Balrog      Kevin's        77  Quillon the Demon      Aiden's        20      0
>> Bricta the Balrog defeated Quillon the Demon

-- Duel 10: Morwen the Cyberelf of Aiden's vs Varek the Elf of Kevin's --
Morwen the Cyberelf    Aiden's       230  Varek the Elf          Kevin's       116      0
>> Morwen the Cyberelf defeated Varek the Elf

-- Duel 11: Chester the Cyberelf of Aiden's vs Nimue the Balrog of Kevin's --
Chester the Cyberelf   Aiden's       186  Nimue the Balrog       Kevin's        49      0
>> Chester the Cyberelf defeated Nimue the Balrog

-- Duel 12: Ragnar the Elf of Aiden's vs Osric the Demon of Kevin's --
Osric the Demon        Kevin's        16  Ragnar the Elf         Aiden's       120    104
Ragnar the Elf         Aiden's       115  Osric the Demon        Kevin's        63      0
>> Ragnar the Elf defeated Osric the Demon

Aiden's Stats after the Battle
Creature    Type          Strength    Health
============================================
Morgas      cyberelf            31         0
Thorfin     balrog             110        44
Petra       balrog              53        47
Karan       demon               56         0
Seren       demon              116         0
Lunara      balrog              71       121
Lagnar      elf                134         0
Orrin       demon              114         0
Quillon     demon              130         0
Morwen      cyberelf           119        46
Chester     cyberelf           128        58
Ragnar      elf                128       104
============================================
Overall health of Aiden's: 420

Kevin's Stats after the Battle
Creature    Type          Strength    Health
============================================
Kaelith     cyberelf           139       118
Aldric      elf                 70         0
Grisha      cyberelf           137         0
Isolde      balrog              84        54
Dorian      demon              141        48
Hollis      demon               40         0
Faelan      balrog              55         3
Cassia      demon              138        90
Bricta      balrog             112        33
Varek       elf                 37         0
Nimue       balrog              87         0
Osric       demon               76         0
============================================
Overall health of Kevin's: 346
=======================================================================================
>>> Aiden's wins the battle <<<
Aiden's overall health: 420
Kevin's overall health: 346
=======================================================================================


Rematch Menu:
1. Fight again with the same creatures
2. Back to the main menu
Enter your choice: 2

Returning to the main menu


Battle Arena Menu:
1. Play game
2. Quit
Enter your choice: 2

Thanks for playing. Goodbye!
aidentsang@Aidens-MacBook-Pro CS216_L7_AT % 
*/

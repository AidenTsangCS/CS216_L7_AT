
* **To provide you with thorough feedback, I read your code line by line, sometimes multiple times**. Jumping from file to file is too time-consuming. Unless noted otherwise, place your class definitions above the main() and all function definitions below main(), all in one file. While the industry standard involves multi-source file programs, we will use a single file format for these labs unless instructed otherwise. Submit a fully completed lab by th due date using the correct GitHub invite link to get a second chance
* **feedback.md is for instructor use only**: please DO NOT change the feedback.md; make a copy if needed; do not add anything; if you do not understand the feedback, let me know asap (mymail.laccd.edu, grigorn@laccd.edu)
* if you do not understand feedback, let me know asap
* if any, items with (-X) - no deductions this time, serve as a warning; please ensure these errors are corrected, as repeating them in future assignments will result in X points being deducted
* in feedback, #N means line number, e.g., 
```text
* #25  global var(s); only constant may be global -10
meaning: see line #25 ; -10 deduction points for the mistake
```
* **class definition style & standards**:  first, member vars  one per line and do not forget to initialize them to state; although private is a default access specifier, still label private members  clearly for better readability; second- functions: separate group of functions  by a blank line; either matching setter/getter pairs or list all setters back to back and list all getters back to back; start with a default c’tor, followed by  copy c’tor (if any), then other c’tors in the order of increased number of arguments, d’tor- right after c’tor(s), overloaded functions- list them back to back in the order of increased number of arguments;  all accessor/getter functions, except for static function,  should be const; all functions with more than one statement should be normal (defined externally, outside of the class and below main()), no inline functions with more than one statement; initialize all member variables to appropriate default values at the time of definition; all member variables must be private; classes must provide a strong exception guarantee; must have default c’tor; implement Rule of Three when necessary;  to promote strong exception guarantee and to reduce redundancy, all setter functions, including c’tors, must call a setter function that sets all member variables
* **strong exception guarantee**: this guarantee acts as a transaction. If the operation successfully completes, all changes take effect; if it fails mid-execution, all partial changes are completely undone. If incoming values are invalid, do not change member variables; provided all member variables have been initialized to a valid state in the class definition and c’tors, the object will always be valid
* **there is no such thing as a partially valid record or set of related data** (let's say, a Creature record in an input file). If any required field (e.g., health, strength) in a record is invalid, then the entire record must be considered invalid. As a developer, your responsibility is to validate the input, not to "fix" it by replacing invalid values with defaults. Doing so changes the user's data and assumes you know what they intended to enter. In general, you should not guess what the user meant or silently modify invalid input. Instead, detect the invalid record and handle it according to the program's requirements (for example, by rejecting it or reporting an error). 
* **classes must completely manage their own failures**; classes must catch and handle internal errors locally to maintain safe, fully encapsulated invariants. In other words, classes must maintain encapsulation by catching and handling internal exceptions locally. According to the C++ Core Guidelines, an object should achieve the no-fail or strong exception guarantee by rolling back state or swallowing expected failures internally so callers aren't burdened with cleanup.
* in  CS 216, **  if a lab submission does not comply with posted instructions, it will result in a loss of an attempt; further feedback will not be provided, and additional attempts will not be granted 
* you can always access parent’s member directly by making parent’s member protected rather than private
* 79 what for?  redundant; unnecessary functions to maintain; you can always  call setCreature() -2
* A child class inherits the members of its parent class. If the child is derived using public or protected inheritance, and the parent's data member is protected, the child can access that member directly—no getter function is needed.
```text
class Parent {
protected:
int side = 10;
};

class Child : public Parent {
public:
void show() {
cout << side;      // Direct access to parent's protected member
}
};
int main() {
Child childObj;
Base baseObj;

childObj.show(); //OK
//baseObj.side=20     ERROR: Not allowed outside the classes!
//childObj.side=20     ERROR: Not allowed outside the classes!
}

```
* if new fails, who will catch the exception? This is a public function and can be called by any class user; uncaught exceptions usually result in abrupt program termination; when a C++ function throws an exception that it doesn't catch, the exception propagates backward up the function call stack, the program searches for a matching catch block while automatically cleaning up local variables in a process called stack unwinding, if it reaches the top without being caught, the program abruptly terminates; to reduce the size of exe file and other computational expenses, and to provide a strong exception guarantee; if uncaught exception occurs during c’tor execution, the object's lifetime never officially begins, the memory allocated for the object is automatically reclaimed by the runtime, and the destructor is never called, you might have a name or reference to the object but not the object itself ;  **classes must completely manage their own failures**
* remove all clone functions; unnecessary redundant functions to maintain; you can always use setCreature to copy values; let alone making them public – 5 
* may not use <new> library -5
* 218 and like **classes must completely manage their own failures** see above; if an exception is caused by an Army  class, the Army class should handle it; the Army class should not rely on others to clean up its own mess and rely on others to notify a user– 10
* 281 – use setCreature for consistency -2

* array of pointers and creature allocations should have been handled by a single function to avoid unpredictable behaviors of the program; you can always call a function to determine what creature to create; who will catch bad_alloc? -5
* Army’s copy c’tor and copy = operator have almost identical code; why not write one function and use it in both places? -2
* 453 should not be a separate function; should be done directly where  it is garanteed-2
* 459, 477 - realizing an array of pointers and creatures should be done by the same function; spreading it between multiple functions like brushing upper teeth in upstairs bathroom and bottom teeth in downstairs bathroom will likely result in a maintenance nightmare due to hard-to-follow convoluted logic; -5
* 590,858 and like:  you do not know if pointer here actually points to dynamically allocated object/variable ( string, creature, etc) and you cannot guarantee there is an object/variable ( string,  creature, etc.); if you use delete on something that was not dynamically allocated, a compiler might throw an exception; check first, then use delete; new does not return nullptr if it fails- it throws bad_alloc exception; initialize the pointer used for DMA to nullptr, then if (!p) {//deallocate};  make sure there is DMA then call delete;  -5
* 825 and like: who is catching bad_alloc if new fails? 
* 825  array of pointers and creature allocations should have been handled by a single function to avoid unpredictable behaviors of the program; you can always call a function to determine what creature to create; who will catch bad_alloc?-5
*  and if new fails? -5
* resubmit by cutoff
***



# About
This project is compiled using `g++` and `makefile`. In order to run and
compile the code you have to execute:
```sh
make run
```
The program requires `C++20` because it uses `std::format`.

The project has been setup in Visual Studio Code and supports debugging.
Note, however, that a separate compilation is generated for debugging.
If you wish that both debug and release versions match you can go ahead
and edit the makefile rules `build` and `build-debug`.

# Documentation
This project currently implements a class that interfaces with two sets
of hospitals and students that each has preferences for each other. The
class allows to find all stable matches of the given set. When invalid
data is provided an exception is thrown to make debugging easier. All
exceptions used are not custom yet, but are implemented with
`std::logic_error`.

## Class Interface
The class is nested in a namespace called `alg`. In this class to retain
more generality the more common names of "hospitals-students" or
"boys-girls" have been replaced for the more generic terms
"providers-consumers". The explanations will use the terms learned in
class "hospital-students".

### Data Members
- `mProviders`: of type `matchMap` which is an alias for a balanced
  binary tree which allows to access the "hospitals" by their name and
  their respective preferences for "students".
- `mConsumers`: of type `matchMap` which is an alias for a balanced
  binary tree which allows to store a list of students that prevent
  repeated items and access their priority by name.

### Constructors
- `StableMatch()` default constructor that initializes all data members
  to an empty map.
- `StableMatch(std::string str, bool explicitPriority = false)` allows
  to provide a neatly formatted string of hospitals and students by
  their name and their list of preferences. The class will parse the
  input and populate the data members. The format is as follows:
```
H0: s0,s1,s2,...,sm;
H1: s0,s1,s2,...,sm;
H2: s0,s1,s2,...,sm;
        ...
Hn: s0,s1,s2,...,sm;
***;
s0: H0,H1,H2,...,Hn;
s1: H0,H1,H2,...,Hn;
s2: H0,H1,H2,...,Hn;
        ...
sm: H0,H1,H2,...,Hn;
```
The `***;` separates hospitals from students and they have to be
provided in that order. The preference list by default assigns priority
in descending order from left-to-right. Note that the code assigns lower
numbers to higher priorities. If you wish to explicitly define the
priorites (and disregarding written order) then you have to include the
keyword `explicit` as the very first line in the string and then you
define each preference as `#-X` where `#` is the priority number (lower
means higher priority) and `X` is the hospital or student name.

### Methods
- `void insertProvider(strv key);`: allows inserting hospitals to the
  data structure one at a time. Their preferences are initialized empty.
- `void insertConsumer(strv key);`: allows inserting students to the
  data structure one at a time. Their preferences are initialized empty.
- `void addProviderPreference(strv key, prefPairs preference);`: allows
  inserting the student preferences to each hospital individually.
- `void addConsumerPreference(strv key, prefPairs preference);`: allows
  inserting the hospital preferences to each student individually.
- `std::string toString() const;`: prints a nicely formated list of
  entries and preferences.
- `std::map<strv, strv> findMatches() const;`: computes the stable
  matches for the provided data using the Gale-Shapley algorithm.

## Aliases
The C++ STL is sort of infamous for its very verbose types. These
aliases are used to make the code more readable and/or to allow changing
the data structures used more freely may the need arise.

- `strv` is just an alias for string. Its purpose is to allow chaning
the key type more freely.

- `matchMap` is a balanced binary tree that has **hospitals** as key and
a `prefMap` as payload.

- `prefMap` is a balanced binary tree that has **students** as key and
**priority** as payload. Priority is not used as key because we want
preferences to be unique students.

- `prefMapInv` is a hash map with separate chaining that has
**priority** as key and **students** as payload.

- `iterMapInv` is a balanced binary tree that has **hospitals** as key
and a `prefMapInv` as payload. This is used in the GS algorithm to allow
students to be iterated sorted by priority instead of sorted by name.


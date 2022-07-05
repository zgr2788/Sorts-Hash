#ifndef _HASHTABLE_H
#define _HASHTABLE_H
//HashTable header file written by Ozgur Yilmaz Beker - 28184
//This file includes the HashTable class definition used in HW3.

#include <iostream>
#include <string>
#include <vector>
#include <cmath>

using namespace std;

/*
#######################################################################
This section includes the code for structs which will be used in the
homework. Since the hash function requires the struct knowledge apriori,
they were defined here.
#######################################################################
*/


//Struct for document items (basically dictionary entries) defined below.
struct DocumentItem
{
	string docName;
	int appCount;

	//Flexible constructor
	DocumentItem(const string& name = "", const int& count = 1)
	: docName(name), appCount(count)
	{}
};


//Struct for words is defined below.
struct WordItem
{
	string word;
	vector<DocumentItem> docs; //Can use vectors here because the impact will be minimal to the speed.

	//Flexible constructor
	WordItem(const string& name = "")
	: word(name), docs(vector<DocumentItem>())
	{}

	bool operator == (const WordItem& rhs) {return word == rhs.word;}
	bool operator == (const WordItem& rhs) const {return word == rhs.word;}
	bool operator != (const WordItem& rhs) {return word != rhs.word;}
	bool operator != (const WordItem& rhs) const {return word != rhs.word;}
	bool operator > (const WordItem& rhs) {return word > rhs.word;}
	bool operator > (const WordItem& rhs) const {return word > rhs.word;}
	bool operator < (const WordItem& rhs) {return word < rhs.word;}
	bool operator < (const WordItem& rhs) const {return word < rhs.word;}
};



//Class definition
template <class T>
class HashTable
{

public:
	HashTable(T, unsigned int); //Flexible constructor
	~HashTable(); //Destructor

	//The ones below are self explanatory
	void insert(const T&);
	void remove(const T&);
	void clear();
	T& findAndGet(const T&);

	//enum types
	enum entryType {ACTIVE, EMPTY, DELETED};

	//for printing
	unsigned int getFilled() {return filledCount;}
	double getLoad() {return 1.0*filledCount/itemVector.size();}






private:
	struct hashEntry
	{
		T element;
		entryType info;

		//Flexible constructor
		hashEntry(T object = T(), entryType i = EMPTY)
		: element(object), info(i)
		{}
	};



	vector <hashEntry> itemVector;
	unsigned int filledCount;
	T ITEM_NOT_FOUND;

	bool isActive (unsigned int currentPos) {return itemVector[currentPos].info == ACTIVE;};
	unsigned int findPos(const T&);
	unsigned int nextPrime(unsigned int);
	bool isPrime(const unsigned int&);
	void rehash();







};


/*
#######################################################################
This section includes the code for nextPrime and isPrime, which will be
used during table sizing to ensure prime size. This is required as my
implementation adapts a probing rule rather than separate chaining.
#######################################################################
*/


//isPrime
template <class T>
bool HashTable<T>::isPrime(const unsigned int& num)
{
	if (num == 2 || num == 3)
		return true;

	if (num == 1 ||num % 2 == 0)
		return false;

	for (unsigned int i = 3; i * i <= num ; i += 2)
		if (num % i == 0)
			return false;

	return true;
}

//nextPrime
template <class T>
unsigned int HashTable<T>::nextPrime(unsigned int currentSize)
{
	if (currentSize % 2 == 0)
		currentSize++;

	while (!isPrime(currentSize))
		currentSize += 2;

	return currentSize;
}


/*
#######################################################################
This section includes the code for constructor and hash functions to
be used.
#######################################################################
*/


//Constructor
template <class T>
HashTable<T>::HashTable(T notFound, unsigned int size)
:ITEM_NOT_FOUND(notFound), itemVector(nextPrime(size))
{
	filledCount = 0;
}

//The function that will be used in hashing
unsigned int hashFunc(const WordItem& word, unsigned int tableSize)
{
	string key = word.word;
	unsigned int val = 0;

	for (int i = 0; i < key.length(); i++)
		val = 31 * val + key[i]; //31 is a mersenne prime, ideal for this

	val = val % tableSize; //No overflows since this is unsigned integer

	return val;
}



/*
#######################################################################
This section includes the code for more intricate functions like find
remove etc.
#######################################################################
*/


//findPos
template <class T>
unsigned int HashTable<T>::findPos(const T& key)
{
	unsigned int colNum = 0;
	unsigned int currentPos = hashFunc(key, itemVector.size());

	//Quadratic probing
	while(itemVector[currentPos].info != EMPTY && itemVector[currentPos].element != key)
	{
		currentPos += pow(++colNum, 2);
		if (currentPos >= itemVector.size())
			currentPos = currentPos % itemVector.size();
	}
	return currentPos;
}

//remove
template <class T>
void HashTable<T>::remove(const T& key)
{
	unsigned int currentPos = findPos(key);

	if (isActive(currentPos))
		itemVector[currentPos].info = DELETED;

}

//findAndGet
template <class T>
T& HashTable<T>::findAndGet(const T& key)
{
	unsigned int currentPos = findPos(key);
	if (isActive(currentPos))
		return itemVector[currentPos].element;

	return ITEM_NOT_FOUND;
}

//insert
template <class T>
void HashTable<T>::insert(const T& key)
{
	unsigned int currentPos = findPos(key);
	if (isActive(currentPos))
		return;

	itemVector[currentPos] = hashEntry(key, ACTIVE);

	//do we need to enlarge? see if load factor >= 0.66.
	//I chose this since I will be doing probing. So higher load factors impact my speed a lot.
	if (++filledCount >= itemVector.size() / 1.4)
		rehash();
}

//rehash
template <class T>
void HashTable<T>::rehash()
{
	vector<hashEntry> temp = itemVector;

	itemVector.resize(nextPrime(2*temp.size()));

	for (int j = 0; j < itemVector.size(); j++)
		itemVector[j].info = EMPTY;

	filledCount = 0;

	for (int i = 0; i < temp.size(); i++)
		if(temp[i].info == ACTIVE)
			insert(temp[i].element);


	double loadFactor = 1.0 * filledCount / itemVector.size();
	cout << "rehashed..." << endl
	<< "previous table size:" << temp.size() << ", new table size:" << itemVector.size() << ", current unique word count:" << filledCount << ", current load factor:" << loadFactor << endl;

}

//clear
template <class T>
void HashTable<T>::clear()
{itemVector.clear();}

//Destructor
template <class T>
HashTable<T>::~HashTable()
{clear();}

#endif

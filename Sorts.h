//Sorts header file used in HW4
//Written by: Ozgur Yilmaz Beker

#ifndef SORTS_H
#define SORTS_H

#include <iostream>
#include <string>
#include <vector>

using namespace std;

//Vector print for debugging
template <class T>
void printVector(vector<T>& subject)
{
	for (int i = 0; i < subject.size(); i++)
		cout << subject[i] << endl;
}

//Swap
template <class T>
inline void Swap( T & obj1, 
		T & obj2 )
{
      T tmp = obj1;
      obj1 = obj2;
      obj2 = tmp;
}



//*************************************************************************
//=========================================================================
//							SORTING ALGORITHMS
//=========================================================================
//*************************************************************************



//**********************************************
//1- Insertion Sort - Ascending
//**********************************************
//Pre: T is comparable

template <class T>
void insertSort(vector <T>& subject)
{
	for (int i = 1; i < subject.size(); i++)
	{
		int idx = i;


		while (idx > 0 && subject[idx] <= subject[idx - 1]) //Swap condition
		{
			Swap(subject[idx], subject[idx - 1]);

			idx--;
		}
	}
}



//**********************************************
//2- Heap Sort - Ascending
//Pre: T is comparable
//**********************************************


//First, percolation operation
template <class T>
void percDown(vector<T>& subject, int i, int simulatedSize)
{
	T temp = subject[i];
	int percPosition = 0;

	for(temp = subject[i] ; 2*i + 1 < simulatedSize ; i = percPosition) //Max iterations = log2(size)
	{      
		percPosition = 2*i + 1;

		if (percPosition != simulatedSize-1 && subject[percPosition] < subject[percPosition+1]) //Correct for left / right child
			percPosition++;

		if (subject[percPosition] > temp) //Swap condition
			subject[i] = subject[percPosition];
		else
			break;
	}

	subject[i] = temp; //Final replacement for Swap op
}

//Below is the rest of the code used in the sort
template <class T>
void heapSort(vector<T>& subject)
{
	//Convert vector to heap 
	//Only need to iterate over first size/2 elements, as leaf nodes are not important in heap
	for (int i = subject.size() / 2; i >= 0 ; i--)
		percDown(subject, i, subject.size());

	// sort
	for (int j = subject.size() - 1; j > 0; j--)
	{
		Swap(subject[0], subject[j]);
		  // re-form the heap
        percDown(subject, 0, j);
	}

}



//**********************************************
//3- Merge Sort (inplace algo - no temp array)
//Pre: T is comparable
//**********************************************

//Idea, shift array indices to save space, however this will make us lose time inevitably


//Merge operation
template <class T>
void merge(vector<T>& subject, int l, int r, int rend)
{
	int lend = r-1;
	int tmp = l;
	int n = rend - l + 1;

	while ( l <= lend && r <= rend )
		if ( subject[ l ] <= subject[ r ] ) //Left is smaller, just move the leftPos
			l++;
		else //Right is smaller, Swap with all elements until leftpos and move indexers accordingly
		{
				int curRight = r;
				while(curRight > l)
				{
					Swap(subject[curRight], subject[curRight-1]);
					curRight--;
				}

				l++;
				lend++;
				r++;
			
		}
}

//Mergesort recursive calls
template <class T>
void mergeSort(vector<T>& subject, int loweridx, int upperidx)
{
    if ( loweridx < upperidx ) //if equal, no need to waste time (1 element base condition)
   {
         int middleidx = ( loweridx + upperidx ) / 2;
         mergeSort( subject, loweridx, middleidx);
         mergeSort( subject, middleidx + 1, upperidx);
         merge( subject, loweridx, middleidx + 1, upperidx);
   }
}

//Mergesort initial driver
template <class T>
void mergeSort(vector<T>& subject)
{
	mergeSort(subject, 0, subject.size() - 1);
}





//**********************************************
//4- Quick Sort (median pivot, small = insertSort)
//Pre: T is comparable
//**********************************************


//Median for quicksort
template <class T>
const T& median(vector<T>& subject, int l, int r)
{
	int m = ( l + r ) / 2;

	// Place pivot at position right
	Swap( subject[ m ], subject[ r ] );
	return subject[ r ];
}

//Overloaded insertion sort
template <class T>
void insertSort(vector <T>& subject, int l, int r)
{
	for (int i = l+1; i < r+1; i++)
	{
		int idx = i;


		while (idx > l && subject[idx] <= subject[idx - 1]) //Swap condition
		{
			Swap(subject[idx], subject[idx - 1]);
			idx--;
		}
	}
}


//Quicksort recursive calls
template <class T>
void quickSort(vector<T>& subject, int l, int r)
{
	if (l+10 <= r)
	{
		T pivot = median(subject, l, r);
		int i = l, j = r-1;

		while(true)
		{
			while (subject[i] < pivot){i++;} //Find element larger than pivot from left
			while (subject[j] > pivot) {j--;} //Find element smaller than pivot from right

			if (i < j) //Still partitioning
				Swap(subject[i], subject[j]);
			else //Partition finished
				break;
		}

		Swap(subject[i], subject[r]); //Pivot to its original pos

		quickSort(subject, l, i-1);
		quickSort(subject, i+1, r);
	}

	else
		insertSort(subject, l, r);

}


//Quicksort initial driver
template <class T>
void quickSort(vector<T>& subject)
{
		quickSort(subject, 0, subject.size()-1);
}




//*************************************************************************
//=========================================================================
//							SEARCHING ALGORITHMS
//=========================================================================
//*************************************************************************


//**********************************************
//1- Binary Search
//Pre: T is comparable
//**********************************************

template <class T>
int binarySearch(vector<T>& subject, T& key)
{
    int l = 0 , r = subject.size() - 1, m; //initialize left right and middle                                          

    while (l <= r){  
		
		m = (l+r)/2; // get middle
		
        if (subject[m] == key) // key found at idx m
           return m;
        
		else if (subject[m] < key) // key is in the right half
           l = m+1;
        
		else // key is in the left half
           r = m-1;
    }
    
	
	return -1; //will be used to control if NOT_FOUND
}






//**********************************************
//2- Sequential (Linear) Search
//**********************************************

template <class T>
int linSearch(vector<T>& subject, T& key)
{
	int idx = -1;

	for (int i = 0; i < subject.size(); i++)
	{
		if(subject[i] == key)
		{
			idx = i;
			break;
		}
	}

	return idx;
}












#endif
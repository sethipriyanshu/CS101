#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

template<typename Type>
void Merge(Type array[], int low, int mid, int high) {
   int mergedSize;
   int mergeIndex;
   int leftIndex;
   int rightIndex;
   Type* mergedArray = nullptr;

   mergeIndex = 0;
   mergedSize = high - low + 1;
   leftIndex = low;
   rightIndex = mid + 1;
   mergedArray = new Type[mergedSize];

   while (leftIndex <= mid && rightIndex <= high) {
      if (array[leftIndex] < array[rightIndex]) {
         mergedArray[mergeIndex] = array[leftIndex];
         ++leftIndex;
      }
      else {
         mergedArray[mergeIndex] = array[rightIndex];
         ++rightIndex;
      }
      ++mergeIndex;
   }

   while (leftIndex <= mid) {
      mergedArray[mergeIndex] = array[leftIndex];
      ++leftIndex;
      ++mergeIndex;
   }

   while (rightIndex <= high) {
      mergedArray[mergeIndex] = array[rightIndex];
      ++rightIndex;
      ++mergeIndex;
   }

   for (mergeIndex = 0; mergeIndex < mergedSize; ++mergeIndex) {
      array[low + mergeIndex] = mergedArray[mergeIndex];
   }
   delete[] mergedArray;
}

template<typename Type>
void MergeSort(Type array[], int low, int high) {
   int mid;
   
   if (low < high) {
      mid = (low + high) / 2;
      MergeSort(array, low, mid);
      MergeSort(array, mid + 1, high);
      Merge(array, low, mid, high);
   }
}

template<typename Type>
int Partition(Type array[], int low, int high) {
   int lowBound;
   int highBound;
   int midpoint;
   Type pivot;
   Type temp;
   bool done;
   
   midpoint = low + (high - low) / 2;
   pivot = array[midpoint];
   
   done = false;
   lowBound = low;
   highBound = high;
   
   while (!done) {
      while (array[lowBound] < pivot) {
         ++lowBound;
      }
      
      while (pivot < array[highBound]) {
         --highBound;
      }
      
      if (lowBound >= highBound) {
         done = true;
      }
      else {
         temp = array[lowBound];
         array[lowBound] = array[highBound];
         array[highBound] = temp;
         
         ++lowBound;
         --highBound;
      }
   }
   
   return highBound;
}

template<typename Type>
void Quicksort(Type array[], int low, int high) {
   int mid;
   
   if (low >= high) {
      return;
   }
   
   mid = Partition(array, low, high);
   
   Quicksort(array, low, mid);
   Quicksort(array, mid + 1, high);
}

template<typename Type>
int readData(const string& filename, Type data[], char sortType) {
    ifstream file(filename);
    int count = 0;
    Type value;
    while (file >> value) {
        data[count] = value;
        count++;
    }

    
    if (sortType == 's') {
        Quicksort(data, 0, count-1);
    }
    else {
        MergeSort(data, 0, count-1);
    }
    return count;
}

template<typename Type>
void printCommonElements(Type array1[], Type array2[], int size1, int size2) {
    int i = 0, j = 0;
    int commonFound = 0;
    Type previousCommon = Type();
    while (i < size1 && j < size2) {
        if (array1[i] == array2[j]) {
            if (previousCommon != array1[i] || commonFound == 0) {
                commonFound = 1;
                cout << array1[i] << '\n';
            }
            previousCommon = array1[i];
            i++;
            j++;
        } else if (array1[i] < array2[j]) {
            i++;
        } else {
            j++;
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        cerr << "Usage: " << argv[0] << " [i|s] file1 file2\n";
        return 1;
    }
    
    if (argv[1][0] == 'i' || argv[1][0] == 's') {
        ifstream file1(argv[2]);
        ifstream file2(argv[3]);
        string firstItem1, firstItem2;
        file1 >> firstItem1;
        file2 >> firstItem2;
        file1.close();
        file2.close();

        bool isInt1 = !firstItem1.empty() && (isdigit(firstItem1[0]) || (firstItem1[0] == '-' && firstItem1.size() > 1 && isdigit(firstItem1[1])));

        if (isInt1) {
            int array1[100000];
            int array2[100000];
            int size1 = 0;
            int size2 = 0;
            size1 = readData(argv[2], array1, argv[1][0]);
            size2 = readData(argv[3], array2, argv[1][0]);
            printCommonElements(array1, array2, size1, size2);
        } else {
            string array1[100000];
            string array2[100000];
            int size1 = 0;
            int size2 = 0;
            size1 = readData(argv[2], array1, argv[1][0]);
            size2 = readData(argv[3], array2, argv[1][0]);
            printCommonElements(array1, array2, size1, size2);
        }
    } else {
        cerr << "Invalid argument: " << argv[1] << '\n';
        return 1;
    }

    return 0;
}

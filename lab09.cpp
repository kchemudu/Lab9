#include <omp.h>
#include <iostream>
#include <chrono>
using namespace std::chrono;
using namespace std;


const int cutoff = 13;//cutoff value is greater than 0, and less than size of the input array

void printArray(int *a, int size)
{
	cout << "| ";
	for (int i = 0; i < size; i++)
	{
		cout << a[i] << ", ";
	}
	cout << "| " << endl;
}

void insertionSort(int A[], int lo, int hi)
{
	int i = 0, temp = 0, j = 0;
	for (i = lo + 1; i < hi; i++)
	{
		temp = A[i];
		j = i - 1;

		while (j >= lo && A[j] > temp)
		{
			A[j + 1] = A[j];
			j = j - 1;
		}
		A[j + 1] = temp;
	}
}

//Prof Lars' code
void merge(int x[], int lo, int mid, int hi, int tmp[]) {
	int a = lo, b = mid;
	for (int k = lo; k < hi; k++) {
		if (a < mid && (b >= hi || x[a] < x[b]))
			tmp[k] = x[a++];
		else tmp[k] = x[b++];
	}
	for (int k = lo; k < hi; k++)
		x[k] = tmp[k];
}

//edited on Prof Lars' code/
void nmpInsertionSort(int A[], int lo, int hi) {
	int n = hi - lo;
	int k = n/cutoff;//This is the number of partitions i want to seperate my array into. 
	if (k >n) k = n;
	omp_set_num_threads(k);
#pragma omp parallel
	{
		int id = omp_get_thread_num(), low = id * cutoff;
		int high = 0;
		if (id != k - 1)
		{
			high = low + cutoff;
		}
		else
		{
			high = n;
		}
		insertionSort(A, low, high);
		//printArray(A, high-low);

	}
#pragma omp barrier
	int* temp = new int[hi - lo];
	int low = 0;
	int high = 2 * cutoff;
	int mid = cutoff;
	for (int i = 1; i < k; i++)
	{
		if (i == k - 1)
		{
			high = n;
		}
		merge(A, lo, mid, high, temp);
		mid = high;
		high += cutoff;
	}
}

int main()
{
	int size = 13;
	int temp = size;
	int * a = new int[size];
	for (int i = 0; i < size; i++)
	{
		a[i] = temp;
		temp--;
	}
	a[6] = 2;
	a[2] = 3;
	cout << "The cutoff value is: " << cutoff << endl;
	printArray(a, size);
	high_resolution_clock::time_point time = high_resolution_clock::now();
	nmpInsertionSort(a, 0, size);
	high_resolution_clock::time_point time2 = high_resolution_clock::now();
	duration<int, std::nano> timeTaken = time2 - time;
	printArray(a, size);
	cout << "The time taken to run the algorithm is:" << timeTaken.count() <<" nanoseconds"<< endl;
}
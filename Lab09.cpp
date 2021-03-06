// Lab09.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <omp.h>
#include <iostream>
#include <time.h>
#include <cmath>
using namespace std;


const int cutoff = 15625;//cutoff value is greater than 0, and less than size of the input array

void isitsorted(int * a, int n)
{
	bool sorted = true;
	for (int i = 1; i < n; i++)
	{
		if (a[i] < a[i - 1])
		{
			sorted = false;
		}
	}
	if (sorted)
	{
		cout << "It is sorted" << endl;
	}
	else cout << "Not sorted" << endl;
}
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
void merge(int x[], int lo, int mid, int hi) {
	int a = lo, b = mid;
	int *tmp = new int[hi - lo];
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
	int k = (int)ceil((1.0 * n) / cutoff); //This is the number of partitions i want to seperate my array into. 
	if (k >n) k = n;
	if (k < 1) k = 1;
	omp_set_num_threads(k);
	int max_threads = omp_get_max_threads();
	cout << max_threads << endl;
#pragma omp parallel
	{
		int id = omp_get_thread_num(), low = id * cutoff;
		int high = 0;
		if (id < k - 1)
		{
			high = low + cutoff;
		}
		else
		{
			high = n;
		}
		insertionSort(A, low, high);
	}

#pragma omp barrier
		int high1 = 2 * cutoff;
		int mid = cutoff;
		for (int i = 1; i < k; i++)
		{
			if (i == k - 1)
			{
				high1 = n;
			}
			merge(A, lo, mid, high1);
			mid = high1;
			high1 += cutoff;
		}
}

int main()
{
	int size =1000000;
	int temp = size;
	int * a = new int[size];
	for (int i = 0; i < size; i++)
	{
		a[i] = temp;
		temp--;
	}
	//a[6] = 2;
	//a[2] = 3;
	cout << "The cutoff value is: " << cutoff << endl;
	//printArray(a, size);
	isitsorted(a, size);
	clock_t time = clock();
	nmpInsertionSort(a, 0, size);
	time = clock() - time;
	//printArray(a, size);
	isitsorted(a, size);
	cout << "The time taken to run the algorithm is:" << (double)time/CLOCKS_PER_SEC << " seconds" << endl;
}

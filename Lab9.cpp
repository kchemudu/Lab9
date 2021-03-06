// Lab9.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <omp.h>
#include <iostream>
using namespace std;

const int CUTOFF = 3;

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
	int i =0, temp = 0, j = 0;
	for (i = lo+1; i < hi; i++)
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

//void nmpInsertionSort(int A[], int lo, int hi) {
//	if (hi - lo <= CUTOFF)
//	{
//		insertionSort(A, lo, hi);
//	}	
//	else {
//		
//#pragma omp parallel num_threads(2)
//		{
//			if (omp_get_thread_num() == 0)
//			{
//				nmpInsertionSort(A, lo, (lo + hi) / 2);
//			}			
//			nmpInsertionSort(A, (lo + hi) / 2, hi); }
//#pragma omp barrier
//			int* temp = new int[hi - lo];
//		merge(A, lo, (lo + hi) / 2, hi, temp); 
//		//printArray(A, hi - lo); 
//	}
//}

//edited on Prof Lars' code/
void nmpInsertionSort(int A[], int lo, int hi) {
	int k = 4; 
	int n = hi - lo;
	if (k >n) k = n;
	int cutoff = n / k;
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
	/*for (int sizeofMerge = cutoff; sizeofMerge <= n; sizeofMerge = sizeofMerge+cutoff)
	{
		int low = 0;
		int high = low + sizeofMerge;
		while(low< n)
			if (high >= n)
			{
				high = n;
			}
			int mid = (low + high) / 2;
			merge(A, low, mid, high, temp);
			low = low +
		}
	}*/
	int low = 0;
	int high = 2*cutoff;
	int mid = cutoff;
	for (int i = 1; i < k ; i++)
	{
		if (i == k - 1)
		{
			high = n;
		}
		merge(A, lo, mid, high, temp);
		mid = high;
		high+= cutoff;
	}
}

int main()
{
	int size = 10;
	int temp = size;
	int * a = new int[size];
	for (int i = 0; i < size; i++)
	{
		a[i] = temp;
		temp--;
	}
	a[6] = 12;
	a[2] = 13;
	printArray(a, size);
	nmpInsertionSort(a, 0, size);
	printArray(a, size);
}



//int * returnCopiedSubArray(int A[], int lo, int hi)
//{
//	int size = hi - lo;
//	int * subArray = new int[size];
//	for (int i = 0; i < size; i++)
//	{
//		subArray[i] = A[lo + i];
//	}
//	return subArray;
//} 

//#pragma omp taskwait
//return merge;
//int mergeSort(int A[], int n) {
//	int result;
//#pragma omp parallel
//#pragma omp single
//	{ result = nmpDC(A, 0, n, key); }
//	return result;
//}
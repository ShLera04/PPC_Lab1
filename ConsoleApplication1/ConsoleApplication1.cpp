#include <iostream>
#include "mpi.h"
#include <iostream>
#include <string.h>
using namespace std;
int isLexicographicStrings(char str1[], char str2[]) {
	/*std::cout << "str1 = " << str1 << std::endl;
	std::cout << "str2 = " << str2 << std::endl;*/
	if (strcmp(str1, str2)<=0)
		return 1;
	else
		return 0;
}
int main(int* argc, char** argv)
{
	int rank, size, firstlen = 0, secondlen = 0, tmp;
	setlocale(LC_ALL, "rus");
	char str1[] = "aaa";
	char str2[] = "aba";
	//cout << isLexicographicStrings(str1, str2);
	MPI_Init(argc, &argv);
	/*std::cout << strlen(str1) << endl;
	std::cout << str1 << endl;
	std::cout << strlen(str2) << endl;
	std::cout << str2 << endl;*/
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	//cout << size << endl << endl;
	int* sendcount = new int[size];
	for (int i = 0; i < size; i++)
		sendcount[i] = 0;
	/*for (int i = 0; i < size; i++)
		cout << sendcount[i] << " ";
	cout << endl;*/
	int remainder = std::max(strlen(str1), strlen(str2) )% size;
	//cout <<"remainder = "<< remainder << endl;
	int portion = std::max(strlen(str1), strlen(str2)) / size;
	//cout <<"portion"<< portion << endl;
	for (int i = 0; i < size; i++) {
		sendcount[i] = portion;
		if (i < remainder) {
			sendcount[i]++;
		}
	}
	int* displ = new int[size];
	int _displ = 0;
	for (int i = 0; i < size; i++) {
	     displ[i] = _displ;
	     _displ += sendcount[i];
	}
	for (int i = 0; i < size; i++)
		cout << sendcount[i] << " ";
	cout << endl;
	int count_local1 = sendcount[rank];
	int count_local2 = sendcount[rank];
	/*cout << rank << "count_local1 = " << count_local1 << endl;
	cout << rank << "count_local2 = " << count_local2 << endl;*/
	char* str_local1 = new char[count_local1];
	char* str_local2 = new char[count_local2];
	MPI_Scatterv(str1, sendcount, displ, MPI_CHAR, str_local1, count_local1, MPI_CHAR, 0, MPI_COMM_WORLD);
	MPI_Scatterv(str2, sendcount, displ, MPI_CHAR, str_local2, count_local2, MPI_CHAR, 0, MPI_COMM_WORLD);
	int result = isLexicographicStrings(str_local1, str_local2);
	int global_result;
	MPI_Reduce(&result, &global_result, 1, MPI_INT, MPI_LAND, 0, MPI_COMM_WORLD);
	if (rank == 0) {
		if (result) {
			std::cout << "Strings are lexicographically ordered!";
		}
		 else
		    std::cout<<"Strings aren't lexicographically ordered";
	}
	MPI_Finalize();
	delete[] sendcount;
	delete[] displ;
	delete[] str_local1;
	delete[] str_local2;
	return 0;
}
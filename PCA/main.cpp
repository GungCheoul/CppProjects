#include "Test.h"
#include <iostream>

int main()
{
	Test pca;
	pca.InputY();
	for (int i = 1; i <= 10; i++) {
		cout << "Eigen Num : " << i << endl;
		pca.Mapping(i);
	}

	return 0;
}
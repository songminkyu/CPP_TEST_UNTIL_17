#include "stdafx.h"
#include <iostream>

enum RESULT_IMG_PROGRESS {
	isRaw,    isPicture,
	isObject, isCat,
	isEYE,    isALL
};
int main()
{
	bool bpicture = true, bobject = true, bCat = true, bEye = false;
	
	RESULT_IMG_PROGRESS state = isRaw;

	bool bret = (state = isPicture, bpicture) &&
				(state = isObject, bobject) &&
				(state = isCat, bCat) &&
				(state = isEYE, bEye) &&
				(state = isALL, true);

	switch (state)
	{
	case isPicture:
		std::cout << "Picture" << std::endl;
		break;
	case isObject:
		std::cout << "Object" << std::endl;
		break;
	case isCat:
		std::cout << "Cat" << std::endl;
		break;
	case isEYE:
		std::cout << "EYE" << std::endl;
		break;
	case isALL:
		std::cout << "ALL" << std::endl;
		break;
	default:
		break;
	}
}


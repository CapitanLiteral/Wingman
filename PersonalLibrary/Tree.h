//// MathLibrary.h - Contains declaration of Function class  
//#pragma once  
//
//#ifdef PERSONALLIBRARY_EXPORTS  
//#define PERSONALLIBRARY_API __declspec(dllexport)   
//#else  
//#define MPERSONALLIBRARY_API __declspec(dllimport)   
//#endif  
//
//namespace PersonalLibrary
//{
//	// This class is exported from the MathLibrary.dll  
//	class Tree
//	{
//	public:
//		// Returns a + b  
//		static PERSONALLIBRARY_API double Add(double a, double b);
//
//		// Returns a * b  
//		static PERSONALLIBRARY_API double Multiply(double a, double b);
//
//		// Returns a + (a * b)  
//		static PERSONALLIBRARY_API double AddMultiply(double a, double b);
//	};
//}

// MathLibrary.h - Contains declaration of Function class  
#pragma once 
#include <vector> 

#ifdef PERSONALLIBRARY_EXPORTS  
#define PERSONALLIBRARY_API __declspec(dllexport)   
#else  
#define PERSONALLIBRARY_API __declspec(dllimport)
#endif  

namespace PersonalLibrary
{
	// This class is exported from the MathLibrary.dll  
	template <class type>
	class Tree
	{
	public:
		std::vector<type*> children;


		// Returns a + b  
		static PERSONALLIBRARY_API double Add(float a, float b);

		// Returns a * b  
		static PERSONALLIBRARY_API double Multiply(double a, double b);

		// Returns a + (a * b)  
		static PERSONALLIBRARY_API double AddMultiply(double a, double b);
	};
}

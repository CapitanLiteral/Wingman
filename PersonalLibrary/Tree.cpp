//#include "stdafx.h"  
//#include "Tree.h"
//
//namespace PersonalLibrary
//{
//	double Tree::Add(double a, double b)
//	{
//		return a+b;
//	}
//
//	double Tree::Multiply(double a, double b)
//	{
//		return a*b;
//	}
//
//	double Tree::AddMultiply(double a, double b)
//	{
//		return a+(a*b);
//	}
//}

// MathLibrary.cpp : Defines the exported functions for the DLL application.  
// Compile by using: cl /EHsc /DMATHLIBRARY_EXPORTS /LD MathLibrary.cpp  

#include "stdafx.h"  
#include "Tree.h"  

namespace PersonalLibrary
{

	double Tree::Add(float a, float b)
	{
		return a + b;
	}

	double Tree::Multiply(double a, double b)
	{
		return a * b;
	}

	double Tree::AddMultiply(double a, double b)
	{
		return a + (a * b);
	}
}

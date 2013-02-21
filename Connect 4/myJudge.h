/********************************************************
*	Judge.h : 用于棋局评判的辅助函数                    *	
*	张永锋                                              *
*	zhangyf07@gmail.com                                 *
*	2010.8                                              *
*********************************************************/

/*
	用于检测给定棋局是否为某一方的胜局或是否出现了平局
	这些函数你可能会使用，也可能不需要使用这些函数，这取决于你自己的策略
*/

#ifndef MY_JUDGE_H_
#define MY_JUDGE_H_

//my slightly modified version of judge functions,mainly change from int to short
int userWin(const int x, const int y, const int M, const int N, short* const* board);

int machineWin(const int x, const int y, const int M, const int N, short* const* board);

//user win3 and machine win3 are used to judge whether 3 is connected
int userWin3(const int x, const int y, const int M, const int N, short* const* board);

int machineWin3(const int x, const int y, const int M, const int N, short* const* board);

bool isTie(const int N, const short* top);

#endif
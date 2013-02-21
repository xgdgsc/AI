/********************************************************
*	Judge.h : ����������еĸ�������                    *	
*	������                                              *
*	zhangyf07@gmail.com                                 *
*	2010.8                                              *
*********************************************************/

/*
	���ڼ���������Ƿ�Ϊĳһ����ʤ�ֻ��Ƿ������ƽ��
	��Щ��������ܻ�ʹ�ã�Ҳ���ܲ���Ҫʹ����Щ��������ȡ�������Լ��Ĳ���
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
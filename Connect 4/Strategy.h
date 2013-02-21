/********************************************************
*	Strategy.h : 策略接口文件                           *
*	张永锋                                              *
*	zhangyf07@gmail.com                                 *
*	2010.8                                              *
*********************************************************/

#ifndef STRATEGY_H_
#define	STRATEGY_H_

#include "Point.h"

#include <deque>
#include <time.h>
//#include "Rand.h"
#include "myJudge.h"
//#include <omp.h>
using namespace std;

extern "C" __declspec(dllexport) Point* getPoint(const int M, const int N, const int* _top, const int* _board, const int lastX, const int lastY, const int noX, const int noY);

extern "C" __declspec(dllexport) void clearPoint(Point* p);

void clearArray(int M, int N, int* top, int** board);

//no Place point
Point noPlace(0,0);

//bool firstRun=true;
//board size
int Msize,Nsize;
//timer variables
clock_t start,finish;	double totaltime;

//Node type enum, max for machine turn and min for user turn
enum treeNodeType{
	maxNode=1,minNode=2
};

struct treeNode;

//node struct
struct treeNode 
{
	deque<treeNode*> sons;  //store its sons
	treeNodeType nodeType;  //node type
	short** board;  //board of the node
	short row,column;  //movement point
	int value;  //evaluation
	short* top;  //available movements
	treeNode* parent;  //pointer to its parent
	short level;  //depth in the tree

	//DEBUG uses
	friend void displayTree(const treeNode* node);
	friend void displayQueue(const deque<treeNode*> sonQueue);

	//calcValue of the node
	int calcValue();
	//calc offensive value
	int calcOffense();
	//calc deffensive value
	int calcDeffense();
	//calc row value
	int calcRow();
	//calc column value
	int calcColumn();
	//calc diagonal value
	int calcDiag();
	//expand current node
	void expandNode();
	
	//alpha beta Pruning
	int alphaBetaMin( int alpha, int beta, int level );
	int alphaBetaMax( int alpha, int beta, int level );

	//memory deallocation
	void clear();
};

//root node
treeNode* root=NULL;

//constants
const int DefaultAlphaValue=-100000;
const int DefaultBetaValue=100000;

//SearchDepth
const int SearchDepth=6;

//clear short array
void clearShort(short** imgboard);
//decide whether the board is empty so I can place chess without thinking
bool isEmpty(const short* top);

#endif
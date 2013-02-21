#include <iostream>
#include "Point.h"
#include "Strategy.h"
#include <conio.h>
#include <atlstr.h>
using namespace std;

/*
���Ժ����ӿ�,�ú������Կ�ƽ̨����,ÿ�δ��뵱ǰ״̬,Ҫ�����������ӵ�,�����ӵ������һ��������Ϸ��������ӵ�,��Ȼ�Կ�ƽ̨��ֱ����Ϊ��ĳ�������

input:
Ϊ�˷�ֹ�ԶԿ�ƽ̨ά����������ɸ��ģ����д���Ĳ�����Ϊconst����
M, N : ���̴�С M - ���� N - ���� ����0��ʼ�ƣ� ���Ͻ�Ϊ����ԭ�㣬����x��ǣ�����y���
_top : ��ǰ����ÿһ���ж���ʵ��λ��. e.g. ��i��Ϊ��,��_top[i] == M, ��i������,��_top[i] == 0
_board : ���̵�һά�����ʾ, Ϊ�˷���ʹ�ã��ڸú����տ�ʼ���������Ѿ�����ת��Ϊ�˶�ά����board
��ֻ��ֱ��ʹ��board���ɣ����Ͻ�Ϊ����ԭ�㣬�����[0][0]��ʼ��(����[1][1])
board[x][y]��ʾ��x�С���y�еĵ�(��0��ʼ��)
board[x][y] == 0/1/2 �ֱ��Ӧ(x,y)�� ������/���û�����/�г������,�������ӵ㴦��ֵҲΪ0
lastX, lastY : �Է���һ�����ӵ�λ��, ����ܲ���Ҫ�ò�����Ҳ������Ҫ�Ĳ������ǶԷ�һ����
����λ�ã���ʱ��������Լ��ĳ����м�¼�Է������ಽ������λ�ã�����ȫȡ�������Լ��Ĳ���
noX, noY : �����ϵĲ������ӵ�(ע:��ʵ���������top�Ѿ����㴦���˲������ӵ㣬Ҳ����˵���ĳһ��
������ӵ�����ǡ�ǲ������ӵ㣬��ôUI�����еĴ�����Ѿ������е�topֵ�ֽ�����һ�μ�һ������
��������Ĵ�����Ҳ���Ը�����ʹ��noX��noY��������������ȫ��Ϊtop������ǵ�ǰÿ�еĶ�������,
��Ȼ�������ʹ��lastX,lastY�������п��ܾ�Ҫͬʱ����noX��noY��)
���ϲ���ʵ���ϰ����˵�ǰ״̬(M N _top _board)�Լ���ʷ��Ϣ(lastX lastY),��Ҫ���ľ�������Щ��Ϣ�¸������������ǵ����ӵ�
output:
������ӵ�Point
*/
extern "C" __declspec(dllexport) Point* getPoint(const int M, const int N, const int* _top, const int* _board, const int lastX, const int lastY, const int noX, const int noY){
	/*
	��Ҫ������δ���
	*/
	int x = -1, y = -1;//���ս�������ӵ�浽x,y��
	int* top = new int[N];
	for(int i = 0; i < N; i++){
		top[i] = _top[i];
	}
	int** board = new int*[M];
	for(int i = 0; i < M; i++){
		board[i] = new int[N];
		for(int j = 0; j < N; j++){
			board[i][j] = _board[i * N + j];
		}
	}

	/*
	�������Լ��Ĳ������������ӵ�,Ҳ���Ǹ�����Ĳ�����ɶ�x,y�ĸ�ֵ
	�ò��ֶԲ���ʹ��û�����ƣ�Ϊ�˷���ʵ�֣�����Զ����Լ��µ��ࡢ.h�ļ���.cpp�ļ�
	*/
	//Add your own code below

	//timer for time control
	start=clock();

	//assign board size
	Msize=M;
	Nsize=N;
	//assign No place point
	noPlace=Point(noX,noY);
	//convert int to short to save memory
	short* rootTop=new short[Nsize];
	for(int i=0;i<Nsize;i++){
		rootTop[i]=(short)top[i];
	}

	//if my first turn, then go to the middle of the board
	if (isEmpty(rootTop))
	{
		clearArray(M, N, top, board);
		return new Point(Msize-1,Nsize/2);
	}

	//initialize root board
	short** rootBoard =new short*[Msize];
	for(int j = 0; j < Msize; j++){
		rootBoard[j] = new short[Nsize];
		for(int i=0;i<Nsize;i++){
			rootBoard[j][i]=board[j][i];
		}
	}
	//	AllocConsole();



	//initialize root node
	root=new treeNode;
	root->board=rootBoard;
	root->nodeType=maxNode;
	root->top=rootTop;
	root->parent=NULL;
	root->row=lastX;
	root->column=lastY;
	root->sons.clear();
	root->level=SearchDepth;
	//pruning
	int value=root->alphaBetaMax(DefaultAlphaValue,DefaultBetaValue,SearchDepth);
	root->value=value;

	//decide next move
	deque<treeNode*>::const_iterator  iter;
	for (deque<treeNode*>::const_iterator it=root->sons.begin();it!=root->sons.end();it++){
		if (*it!=NULL)
		{
			if (value==(*it)->value)
			{
				iter=it;break;
			}
		}
	}
	x=(*iter)->row;
	y=(*iter)->column;

	root->clear();
	delete root;

	//cout<<"oppo:"<<lastX<<' '<<lastY<<endl;
	//cout<<"me:"<<x<<' '<<y<<endl;


	//finish=clock();
	//totaltime=(double)(finish-start)/CLOCKS_PER_SEC;

	/*
	��Ҫ������δ���
	*/
	clearArray(M, N, top, board);
	return new Point(x, y);
}


/*
getPoint�������ص�Pointָ�����ڱ�dllģ���������ģ�Ϊ��������Ѵ���Ӧ���ⲿ���ñ�dll�е�
�������ͷſռ䣬����Ӧ�����ⲿֱ��delete
*/
extern "C" __declspec(dllexport) void clearPoint(Point* p){
	delete p;
	return;
}

/*
���top��board����
*/
void clearArray(int M, int N, int* top, int** board){
	delete[] top;
	for(int i = 0; i < M; i++){
		delete[] board[i];
	}
	delete[] board;
}

int treeNode::alphaBetaMax( int alpha, int beta, int level ) {
	if(level==0)
	{
		return calcValue();
	}

	if(sons.size()==0)
	{
		expandNode();
	}
	if(sons.size()==0)
	{
		return calcValue();
	}

	for (deque<treeNode*>::const_iterator it=sons.begin(); it!=sons.end(); it++)
	{
		value=(*it)->alphaBetaMin(alpha, beta, level-1);
		if( value >= beta )
			return beta;   // fail hard beta-cutoff
		if( value > alpha )
			alpha = value; // alpha acts like max in MiniMax
	}
	return alpha;
}

int treeNode::alphaBetaMin( int alpha, int beta, int level ) {
	if(level==0)
	{
		return calcValue();
	}

	if(sons.size()==0)
	{
		expandNode();
	}
	if(sons.size()==0)
	{
		return calcValue();
	}
	for (deque<treeNode*>::const_iterator it=sons.begin(); it!=sons.end(); it++)
	{
		value=(*it)->alphaBetaMax(alpha, beta, level-1);
		if( value <= alpha )
			return alpha; // fail hard alpha-cutoff
		if( value < beta )
			beta = value; // beta acts like min in MiniMax
	}
	return beta;
}

int treeNode::calcValue()
{
	//value=calcRow()+calcColumn()+calcDiag()+calcOffense()+calcDeffense();
	value=0;
	if (parent!=root)
	{
		value+=parent->calcValue();
	}
	//value+=(level+0.5)*calcOffense()+(level)*calcDeffense();
	value+=calcOffense()+calcDeffense();
	if (nodeType==maxNode)
	{
		value-=(2*calcRow()+calcColumn() +calcDiag());
	}else{
		value+=(2*calcRow()+calcColumn() +calcDiag());
	}
	return value;
}

int treeNode::calcOffense()
{
	int total=0;
	int offense=0;
	if (nodeType==minNode&&machineWin(row,column,Msize,Nsize,board))
	{
		return 1500;
	}else if(nodeType==maxNode&& userWin(row,column,Msize,Nsize,board))
	{
		return -1500;
	}

	if (nodeType==minNode)
	{
		total=machineWin3(row,column,Msize,Nsize,board);
		switch (total)
		{
		case 0:offense=0;break;
		case 1:offense=256;break;
		case 2:offense=512;break;
		case 3:offense=1024;break;
		case 4:offense=2048;break;
		default:offense=0;
		}
		return offense;
	}else if(nodeType==maxNode)
	{
		total=userWin3(row,column,Msize,Nsize,board);
		switch (total)
		{
		case 0:offense=0;break;
		case 1:offense=256;break;
		case 2:offense=512;break;
		case 3:offense=1024;break;
		case 4:offense=2048;break;
		default:offense=0;
		}
		return -offense;
	}
	else{
		return 0;
	}
}

int treeNode::calcDeffense(){

	short** imgboard	=new short*[Msize];
	 
	for(int j = 0; j < Msize; j++){
		imgboard[j] = new short[Nsize];
		memcpy(imgboard[j],board[j],Nsize*sizeof(short));
	}

	if (nodeType==minNode)
	{
		imgboard[row][column]=maxNode;
	}else if (nodeType==maxNode)
	{
		imgboard[row][column]=minNode;
	}

	if (nodeType==minNode&&userWin(row,column,Msize,Nsize,imgboard))
	{
		clearShort(imgboard);
		return 1500;
	}else if (nodeType==maxNode&&machineWin(row,column,Msize,Nsize,imgboard))
	{
		clearShort(imgboard);
		return -1500;
	}else if (nodeType==minNode&&userWin3(row,column,Msize,Nsize,imgboard))
	{
		clearShort(imgboard);
		return 512;
	}else if (nodeType==maxNode&&machineWin3(row,column,Msize,Nsize,imgboard))
	{
		clearShort(imgboard);
		return -512;
	}
	clearShort(imgboard);
	return 0;
}

void clearShort(short** imgboard){
	 
	for (int i=0;i<Msize;i++)
	{
		delete[] imgboard[i];
	}
	delete[] imgboard;
}

int treeNode::calcRow()
{
	int sum=0;
	 
	for (int i=((column-4<0) ? 0 :column-4);i<(column>Nsize-4?Nsize-4:column);i++)
	{
		int mine=0;
		for (int j=i;j<i+4;j++)
		{
			if (board[row][j]==nodeType)
			{
				mine++;
			}
			else if (((board[row][j]==nodeType+1||board[row][j]==nodeType-1)&&board[row][j]!=0)||(row==noPlace.x&&j==noPlace.y))
			{
				mine=0;
				break;
			}
		}
		switch(mine){
		case 0:sum+=0;break;
		case 1:sum+=1;break;
		case 2:sum+=4;break;
		case 3:sum+=8;break;
		case 4:sum+=32;break;
		default:sum+=0;break;
		}
	}
	return sum;
}

int treeNode::calcColumn()
{
	int sum=0;
	int mine=1;
	int oppo=0;
	if (row+1<Msize)
	{
		for (int j=row+1;j<Msize;j++)
		{
			if (board[j][column]==nodeType)
			{
				mine++;
			}
			else{
				break;
			}
		}

		if (board[row+1][column]==nodeType+1||board[row+1][column]==nodeType-1)
		{
			for (int j=row+1;j<Msize;j++)
			{
				if (board[j][column]==nodeType+1||board[j][column]==nodeType-1&&board[j][column]!=0){
					oppo++;
				}
				else{
					break;
				}
			}
		}
	}
	if (row+mine<4)
	{
		mine=0;
	}
	switch(mine){
	case 0:sum+=0;break;
	case 1:sum+=0;break;
	case 2:sum+=4;break;
	case 3:sum+=8;break;
	case 4:sum+=32;break;
	default:sum+=0;break;
	}
	switch(oppo){
	case 0:sum+=0;break;
	case 1:sum+=0;break;
	case 2:sum+=4;break;
	case 3:sum+=8;break;
	default:sum+=0;break;
	}
	return sum;
}

int treeNode::calcDiag()
{
	int sum=0;
	int sum1=0,sum2=0;
	int mine=1;
	int leftBlock=-1,rightBlock=-1;
	for (int j=1;j<4;j++)
	{
		if(row+j>Msize-1||column+j>Nsize-1){
			rightBlock=j;
			break;
		}

		if (board[row+j][column+j]==nodeType)
		{
			mine++;
		}else if ((board[row+j][column+j]==nodeType+1||board[row+j][column+j]==nodeType-1&&board[row+j][column+j]!=0)||(row+j==noPlace.x&&column+j==noPlace.y))
		{
			rightBlock=j;
			break;
		}else{
			rightBlock=min(Msize-row,Nsize-column);
		}
	}

	for (int j=1;j<4;j++)
	{
		if(row-j<0||column-j<0){
			leftBlock=j;
			break;
		}

		if (board[row-j][column-j]==nodeType)
		{
			mine++;
		}else if ((board[row-j][column-j]==nodeType+1||board[row-j][column-j]==nodeType-1&&board[row-j][column-j]!=0)||(row-j==noPlace.x&&column-j==noPlace.y))
		{
			leftBlock=j;
			break;
		}
		else{
			leftBlock=min(row,column);
			break;
		}
	}
	if (leftBlock+rightBlock<4)
	{
		mine=0;
	}
	switch(mine){
	case 0:sum1+=0;break;
	case 1:sum1+=0;break;
	case 2:sum1+=4;break;
	case 3:sum1+=8;break;
	case 4:sum1+=32;break;
	default:sum1+=0;break;
	}


	mine=1;
	for (int j=1;j<4;j++)
	{
		if(row+j>Msize-1||column-j<0){
			leftBlock=j;
			break;
		}

		if (board[row+j][column-j]==nodeType)
		{
			mine++;
		}else if ((board[row+j][column-j]==nodeType+1||board[row+j][column-j]==nodeType-1&&board[row+j][column-j]!=0)||(row+j==noPlace.x&&column-j==noPlace.y))
		{
			leftBlock=j;
			break;
		}else{
			leftBlock=min(Msize-row,column);
			break;
		}
	}

	for (int j=1;j<4;j++)
	{
		if(row-j<0||column+j>Nsize-1){
			break;
		}

		if (board[row-j][column+j]==nodeType)
		{
			mine++;
		}else if ((board[row-j][column+j]==nodeType+1||board[row-j][column+j]==nodeType-1&&board[row-j][column+j]!=0)||(row-j==noPlace.x&&column+j==noPlace.y))
		{
			leftBlock=j;
			break;
		}else{
			leftBlock=min(row,Nsize-column);
			break;
		}
	}
	if (leftBlock+rightBlock<4)
	{
		mine=0;
	}
	switch(mine){
	case 0:sum2+=0;break;
	case 1:sum2+=1;break;
	case 2:sum2+=4;break;
	case 3:sum2+=8;break;
	case 4:sum2+=32;break;
	default:sum2+=0;break;
	}
	sum=max(sum1,sum2);
	return sum;
}


void treeNode::clear(){
	for (deque<treeNode*>::const_iterator it=sons.begin(); it!=sons.end(); it++)
	{
		(*it)->clear();
		delete *it;
	}
	sons.clear();
	 
	for(int i = 0; i < Msize; i++){
		delete[] board[i];
	}
	delete[] board;
	delete[] top;
}

void displayQueue(const deque<treeNode*> sonQueue)
{
	deque<treeNode*> myqueue;
	if (sonQueue.empty())
	{
		return ;
	}
	for (deque<treeNode*>::const_iterator it=sonQueue.begin(); it!=sonQueue.end(); it++)
	{
		cout<<"value:"<<(*it)->value;
		cout<<"\tlevel:"<<(*it)->level<<"("<<"NodeType: "<<(*it)->nodeType<<"\tsons size():"<<(*it)->sons.size()<<") "<<"\n";

		myqueue.insert(myqueue.end(), (*it)->sons.begin(), (*it)->sons.end());
	}
	cout<<"\n";
	displayQueue(myqueue);
}

void displayTree(const treeNode* node)
{
	cout<<"value:"<<node->value;
	cout<<"\tlevel:"<<node->level<<"("<<"NodeType: "<<node->nodeType<<"\tsons size():"<<node->sons.size()<<") "<<"\n";
	displayQueue(node->sons);
}

void treeNode::expandNode()
{
	totaltime=(double)(clock()-start)/CLOCKS_PER_SEC;
	//time control
	if (totaltime>4.5)
	{
		return;
	}
	treeNode* tn=this;
	treeNodeType sonType;
	//if the node is in win or tie state, it's a terminal leaf node,avoid expanding
	if (tn->nodeType==maxNode)
	{
		if (tn!=root)
		{
			if (userWin(row,column,Msize,Nsize,tn->board)||isTie(Nsize,tn->top))
			{
				return;
			}
		}
		sonType=minNode;
	}
	else if (tn->nodeType==minNode)
	{
		if (tn!=root)
		{
			if (machineWin(row,column,Msize,Nsize,tn->board)||isTie(Nsize,tn->top))
			{
				return;
			}
		}
		sonType=maxNode;
	}			

	//expand the node
	for(int i=0;i<Nsize;i++)
	{
		if(tn->top[i]!=0){
			treeNode* son=new treeNode;	
			son->column=i;
			son->row=tn->top[i]-1;	

			son->top=new short[Nsize];
			 
			for (int k=0;k<Nsize;k++)
			{
				son->top[k]=tn->top[k];
			}
			son->top[i]=son->row;

			if (i==noPlace.y &&son->top[i]-1==noPlace.x)
			{
				son->top[i]=son->top[i]-1;
			}
			//cout<<"i:"<<i<<endl;
			son->board =new short*[Msize];
			 
			for(int j = 0; j < Msize; j++){
				son->board[j] = new short[Nsize];
				//memcpy(son->board[j],tn->board[j],Nsize*sizeof(short));
				 
				for(int k=0;k<Nsize;k++){
					son->board[j][k]=tn->board[j][k];
				}
			}

			son->board[son->row][son->column]=sonType;
			son->nodeType=sonType;
			son->parent=tn;
			son->sons.clear();
			son->level=tn->level-1;
			tn->sons.push_back(son);
		}
		else{
			continue;
		}
	}
}

bool isEmpty(const short* top){
	bool empty=true;
	for (int i=0;i<Nsize;i++)
	{
		if (top[i]!=Msize)
		{
			empty=false;
			break;
		}
	}
	return empty;
}
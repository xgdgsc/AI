#include <iostream>
#include <cstdlib>
#include<fstream>
#include<string.h>

using namespace std;

int N,k;
int c=1;
bool path[20][20][2];

struct MCNode   //linked list Node
{
    int m , c , b ;   //m for Missionaries ,c for Cannibals, b for boat
    MCNode *next;
};

MCNode *MChead; //head of list

//function to insert to the result list
void insertMC( int missionary,int cannibal,int boat)
{
    MCNode *newMCNode=new MCNode();
    newMCNode->m = missionary;
    newMCNode->c=cannibal;
    newMCNode->b=boat;
    newMCNode->next=MChead->next;
    MChead->next=newMCNode;
}

//recursive function to search for one available answer
bool search(int missionary,int cannibal,int boat)
{
    //the finish condition
    if(missionary==0&&cannibal==0&&boat==0)
    {
        insertMC(0,0,0);
        return true;
    }

    //missionary must < cannibal anywhere
    if((missionary<0||missionary>N||cannibal<0||cannibal>N)
            ||(missionary>0&&missionary<cannibal)
            ||(N-missionary>0&&N-missionary<N-cannibal))
        return false;

    //if the condition has been reached don't need to reach again
    if(path[missionary][cannibal][boat]) return false;
    else path[missionary][cannibal][boat]=true;

    //boatM is for Missionarys on boat,boatC is for Cannibals on boat
    //searching for viable solutions
    for(int boatM=k;boatM>=0;boatM--)
        for(int boatC=k-boatM;boatC>=0;boatC--)
        {
            //the boat shouldn't be empty or boatM shouldn't be less than boatC
            if(boatM+boatC==0||(boatM>0&&boatM<boatC)) continue;

            //boat is on the left
            if(boat==1)
            {
                if(search(missionary-boatM,cannibal-boatC,0))
                {
                    insertMC(missionary,cannibal,boat);//record path
                    return true;
                }
            }
            else
            {
                if(search(missionary+boatM,cannibal+boatC,1))
                {
                    insertMC(missionary,cannibal,boat);//record path
                    return true;
                }
            }
        }

    //record failure
    path[missionary][cannibal][(boat+1)/2]=false;
    return false;
}

int main(int argc, char *argv[])
{
    char* input;
    char* output;
    ifstream infile;
    ofstream outfile;

    //initialize list
    MChead=new MCNode();
    MChead->next=NULL;
    memset(path,false,sizeof(path));

    //make sure the argument is adequate
    if(argc<3)
    {
        cout << "No Input||Output File Specified!" << endl;
        exit(1);
    }
    else
    {
        input=argv[1];
        output=argv[2];
        infile.open(input,ios::in);
        outfile.open(output,ios::out);
        infile>>N>>k;           //get key value
    }

    if(search(N,N,c)==true)     //solution found
    {
        MCNode *p=MChead->next;
        while(p)                                    //traverse the linked list to output result to file
        {
            outfile<<"("<<p->m<<","<<p->c<<","<<p->b<<")";
            if(p->next!=NULL)
                outfile<<endl;
            p=p->next;
        }
        outfile<<endl;
    }
    else outfile<<"no solution\n";
    infile.close();
    outfile.close();
    return 0;
}


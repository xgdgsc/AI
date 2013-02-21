#include<cstdlib>
#include<iostream>
#include<string>
#include<queue>
#include<vector>
#include<map>
#include<fstream>
using namespace std;

priority_queue <pair<int,int>,vector<pair<int,int> >,greater<pair<int,int> > > pq;   //priority queue to store nodes
map <string,int> hash;     //hash table to store whether accessed before
int lp;            //line pointer
const string target="123804765";   //final state

struct node
{
    string state;                       //state is a string similar to target
    int zeroPos,g,pParent;                  //zero Position,g function,pointer to parent node
}line[362880];

//move right/left/down/up
const int w[4]={1,-1,3,-3};

//h为当前状态与目标状态不一致点的个数，g为已走过的步数
int f(string now,int index)
{
    int ans=line[index].g;
    for (int i=0; i<9; i++)
        if (now[i]!=target[i]) ans++;
    return ans;
}

//check whether would be out of border
//i is the move,x is the current state
bool checkBorder(int i,int x)
{
    int j=line[x].zeroPos;
    if ((j % 3==0 && w[i]==-1)    //if 0 is on the left side of the matrix, cannot move left
            ||(j % 3==2 && w[i]==1)  //............ right................  ............right
            ||(j<3 && w[i]==-3)        //............ top...............................up
            ||(j>5 && w[i]==3))         //............bottom...........................down
        return false;
    else
        return true;
}

//A* Algorithm
int A_star()
{
    while (!pq.empty())
    {
        int x=pq.top().second;
        pq.pop();

        for (int i=0; i<4; i++)
            if (checkBorder(i,x))  //valid move?
            {
                //perform the move
                string now=line[x].state;
                int j=line[x].zeroPos;
                now[j]=now[j+w[i]];
                now[j+w[i]]='0';

                //if not existed
                if (hash.find(now)==hash.end())
                {
                    lp++;
                    hash.insert(make_pair(now,lp));        //insert move
                    line[lp].state=now;
                    line[lp].zeroPos=j+w[i];
                    line[lp].g=line[x].g+1;
                    line[lp].pParent=x;
                    pq.push(make_pair(f(now,lp),lp));
//                    cout<<line[lp].g<<endl;
                }
                if (now==target)  return line[x].g+1;
            }
    }
    return 0;                                                                   //no solution
}


int main(int argc,char* argv[])
{
    ifstream infile;
    ofstream outfile;

    //make sure the argument is adequate
    if(argc<3)
    {
        cout << "No Input||Output File Specified!" << endl;
        exit(1);
    }
    else
    {
        infile.open(argv[1],ios::in);
        outfile.open(argv[2],ios::out);
    }

    string initial="123456789";
    for(int i=0;i<9;i++)
    {
        infile>>initial[i];
    }

    if(initial==target)
    {
        outfile<<'0'<<endl;
    }
    else
    {
        lp=0;
        line[lp].state=initial;
        line[lp].g=0;
        line[lp].zeroPos=initial.find('0');          //zeroPos 0~8
        line[lp].pParent=-1;
        pq.push(make_pair(f(initial,lp),lp));

        hash.insert(make_pair(initial,lp));

        int ans=A_star();
        if(ans==0){outfile<<"no solution"<<endl;}
        else
        {
            outfile<<ans<<endl;
            vector<string> states;
            while(line[lp].pParent!=0)
            {
                //outfile<<line[line[lp].pParent].state<<endl;
                states.push_back(line[line[lp].pParent].state);
                lp=line[lp].pParent;
            }
            for(int i=states.size()-1;i>=0;i--)
            {
                outfile<<endl;
                for(int j=0;j<9;j++)
                {
                    outfile<<states[i][j]<<' ';
                    if(j%3==2)outfile<<endl;
                }
//                outfile<<states[i]<<endl;
            }
            outfile<<endl;
            for(int j=0;j<9;j++)
            {
                outfile<<target[j]<<' ';
                if(j%3==2)outfile<<endl;
            }
        }
    }

    infile.close();
    outfile.close();
    return 0;
}

#include <iostream>
#include<string>
#include<fstream>
#include<stdlib.h>
#include<vector>
#include<algorithm>
#include<cmath>
#include<map>
#include <boost/numeric/ublas/matrix.hpp>
//#include"matrix.hpp"
#include"Rand.h"

using namespace std;
using namespace boost::numeric::ublas;
int cityNum;
//按照书上举的例子选取的初始温度、冷却率、停止阈值
double t0=280,t,coolingRate=0.92,Tmin=0.01;
int k=0;
RandNumInteger rd;
//City Class
struct City{
    string cityName;
    int cityN;
    double posX;
    double posY;
};

//计算城市间距离
double cityDist(City A,City B){
    return sqrt(pow((A.posX-B.posX),2)+pow((A.posY-B.posY),2));
}

//总距离计算函数
double func(std::vector<City> state,matrix<double> disTable){
    double sum=0;
    for(int i=0;i<cityNum-1;i++){
        sum+=disTable(state[i].cityN,state[i+1].cityN);
    }
    sum+=disTable(state[cityNum-1].cityN,state[0].cityN);
    return sum;
}

//产生状态的临域状态
std::vector<City> getNextState(std::vector<City> current){
    int firstPos=rd.RandomInteger(0,cityNum-1);
    int secondPos;
    do{
        secondPos=rd.RandomInteger(0,cityNum-1);
    }while(firstPos==secondPos+1);
   // std::vector<City> nextState=curr;

    reverse(current.begin()+firstPos,current.begin()+secondPos+1);

    return current;
}


int main(int argc,char* argv[]){
    ifstream infile;
    ofstream outfile;

    //make sure the argument is adequate
    if(argc<3){
        cout << "No Input||Output File Specified!" << endl;
        exit(1);
    }
    else{
        infile.open(argv[1],ios::in);
        outfile.open(argv[2],ios::out);
    }


    infile>>cityNum;
    City tmpCity;
    std::vector<City> cities,currentState,nextState;

    //读入文件
    for(int i=0;i<cityNum;i++){
        infile>>tmpCity.cityName;
        infile>>tmpCity.posX;
        infile>>tmpCity.posY;
        tmpCity.cityN=i;
        cities.push_back(tmpCity);
    }

    //用于存储距离的表，查表减少运算量
    matrix<double> disTable (cityNum, cityNum);
    //Mat disTable[cityNum][cityNum];

    //计算用于存储距离的表
    for(int i=0;i<cityNum;i++){
        for(int j=i;j<cityNum;j++){
            disTable(i,j)=disTable(j,i)=cityDist(cities[i],cities[j]);
        }
    }

    double currentDistance=func(cities,disTable);
    double nextDistance,deltaDistance;
    currentState=cities;
    t=t0;

    //随机数类
    RandNumReal rnr;
    //算法在降温充分后结束
    while(t>Tmin){
        //同一温度内计算结束的条件为计算进行了城市数的100倍
        for(int i=0;i<100*cityNum;i++){
            nextState=getNextState(currentState);
            nextDistance=func(nextState,disTable);
            deltaDistance=nextDistance-currentDistance;
            if(deltaDistance<0||((deltaDistance>0)&&(exp(-deltaDistance/t)>rnr.RandomReal(0,1)))){
                currentState=nextState;
                currentDistance=nextDistance;
            }
        }
        //输出序列
        for(int i=0;i<cityNum;i++){
            outfile<<currentState[i].cityName<<' ';
        }
        outfile<<currentDistance<<endl;
        t=t*coolingRate;//按等比率降温
    }



//    for(int i=0;i<cityNum;i++){
//        outfile<<currentState[i].cityName<<'\t';
//    }
//    outfile<<currentDistance<<endl;

    //outfile<<func(cities,disTable);
   // outfile<<disTable(0,3)+disTable(3,4)+disTable(4,5)+disTable(5,6)+disTable(6,7)+disTable(7,8)+disTable(8,9)+disTable(9,1)+disTable(1,2)+disTable(2,0)<<endl;
    return 0;
}

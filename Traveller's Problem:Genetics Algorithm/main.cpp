#include <iostream>
#include<string>
#include<fstream>
#include<stdlib.h>
#include<vector>
#include<algorithm>
#include<cmath>
#include<map>
#include <boost/numeric/ublas/matrix.hpp>
#include"Rand.h"
#include<time.h>

using namespace std;
//using namespace boost::numeric::ublas;
int cityNum;

//just a random number generator class
RandNumReal rd;

//store the distances between two cities to eliminate calculation
boost::numeric::ublas::matrix<double> disTable ;

//City Class
struct City{
    string cityName;
    int cityN;
    double posX;
    double posY;
};

//city Number to City class map
map<int,City> cityTable;

const int MaxGenerations=20000;       //Max generations number
const double popMulti=2;//40;
const int mutaRate=0.1;                     //probability of mutation
//constrain run time to within 5 min
clock_t start,finish;	double totaltime;

//Chroma struct
struct Chroma
{
    std::vector<int> series;    //a vector of int to store the city number order
    double fitness,selectProb;  //fitness=distance,selectProb: the probability of the Chroma to be chosen to mate(), less fitness ~ more selectProb
    int inverseProb;        //used to generate random mate pairs
    int t;                          //generation
    Chroma(){
        fitness = 10000000;
        t=0;
    }

    //initialize random series
    void RandSeries(){
        series.clear();
        for(int i=0;i<cityNum;i++){
            int tmp;
            do{
                tmp=rd.RandomInteger(0,cityNum-1);
            }while(find(series.begin(),series.end(),tmp)!=series.end());
            series.push_back(tmp);
        }
    }

    //calc total distance of series
    int calcFitness(){
        fitness=0;
        for(int i=0;i<cityNum-1;i++){
            fitness+=disTable(series[i],series[i+1]);
        }
        fitness+=disTable(series[cityNum-1],series[0]);
        return fitness;
    }

    //mutate by swapping two positions
    Chroma mutate(){
        int first=rd.RandomInteger(0,cityNum-1);
        int second;
        do{
            second=rd.RandomInteger(0,cityNum-1);
        }while(second==first);
        vector<int> mutated=series;
        Chroma tmp;
        swap(mutated[first],mutated[second]);
        tmp.series=mutated;
        return tmp;
    }

    //Chromas are equal if their series are exactly the same
    bool operator == (const Chroma &B) {
        bool equal;
        for(int i=0;i<cityNum;i++){
            if(series[i]!=B.series[i]){
                equal=false;
                break;
            }
        }
        return equal;
    }

    //output ,used for debugging purposes
    friend ostream& operator <<(ostream& os,const Chroma & chr){
        for(uint i=0;i<chr.series.size();i++){
            cout<<cityTable[chr.series[i]].cityName<<' ';
        }
        // cout<<"dist:"<<fitness<<endl;
        cout<<endl;
        return os;
    }

    //reload function, used by std::sort
    bool operator < (const Chroma& B) const{
        if(fitness<B.fitness)
            return true;
        else
            return false;
    }

    //mate process presented as the operation "+"
    //mate: generate one son by generating a cut point and use A/B's part prior to the point and select unused cities from the other
    Chroma operator + (const Chroma&B)const{
        Chroma son;
        int cut=rd.RandomInteger(1,cityNum-1);
        int which=rd.RandomInteger(0,1);
        switch (which){
        case 0:
            for(int i=0;i<cut;i++){
                son.series.push_back(series[i]);
            }
            for(int i=0;i<cityNum-1;i++){
                if(find(son.series.begin(),son.series.end(),B.series[i])==son.series.end()){
                    son.series.push_back(B.series[i]);
                }
                if(son.series.size()==(uint)cityNum) break;
            }
            break;
        case 1:
            for(int i=0;i<cut;i++){
                son.series.push_back(B.series[i]);
            }
            for(int i=0;i<cityNum-1;i++){
                if(find(son.series.begin(),son.series.end(),series[i])==son.series.end()){
                    son.series.push_back(series[i]);
                }
                if(son.series.size()==(uint)cityNum) break;
            }
            break;
        default:
            break;
        }
        //cout<<son.series.size()<<endl;
        if(son.series.size()<(uint)cityNum){
            for(int i=0;i<cityNum;i++){
                if(find(son.series.begin(),son.series.end(),i)==son.series.end()){
                    son.series.push_back(i);
                }
                if(son.series.size()==(uint)cityNum) break;
            }
        }
        return son;
    }

};

//store best result in generations
Chroma currentOptimal;

//the Population of Chromas
struct Population{
    std::vector<Chroma> pop,newPop,merge;   //pop:current population
    int popSize,newPopSize;
    double pm;
    double t;       //generation
    std::vector<int> selectTable;   //for select() random Chroma
    Population(){
        pm=mutaRate;
        t=0;
    }

    //initialize Population
    void initialize(){
        popSize=pow((double)cityNum,popMulti);//popMulti*cityNum;
        newPopSize=5*popSize;
        for(int i=0;i<popSize;i++){
            Chroma tmpChroma;
            do{
                tmpChroma.RandSeries();
            }while(find(pop.begin(),pop.end(),tmpChroma)!=pop.end());
            pop.push_back(tmpChroma);
        }
    }

    //output,DEBUG uses
    friend ostream& operator <<(ostream& os,const Population & ppl){
        for(int i=0;i<ppl.popSize;i++){
            cout<<ppl.pop[i];
        }
        return os;
    }

    //calc fitnesses of Chromas
    void calcFitnesses(){
        for(int i=0;i<popSize;i++){
            pop[i].calcFitness();
        }
    }

    //calc fitnesses of newPop Chromas
    void calcNewFitnesses(){
        for(uint i=0;i<newPop.size();i++){
            newPop[i].calcFitness();
        }
    }

    //calc Probability of selected for mate()
    void calcProbs(){
        for(int i=0;i<popSize;i++){
            pop[popSize-i-1].selectProb=(2.0*(i+1.0))/(popSize*(popSize+1));
            pop[popSize-i-1].inverseProb=1.0/pop[popSize-i-1].selectProb;
        }
        selectTable.clear();
        for(int i=0;i<popSize;i++){
            for(int j=0;j<pop[i].inverseProb*(popSize-i);j++){
                selectTable.push_back(i);
            }
        }
    }

    //return one selected Chroma for mate
    int select(){
        int select=rd.RandomInteger(0,selectTable.size()-1);
        return selectTable[select];
    }

    //mate two Chromas
    void mate(){
        newPop.clear();
        newPopSize=rd.RandomInteger(5,25)*popSize;
        Chroma tmpChroma;
        while(newPop.size()<(uint)newPopSize){
            do{
                tmpChroma=pop[select()]+pop[select()];
            }while(find(newPop.begin(),newPop.end(),tmpChroma)!=newPop.end());/*||tmpChroma.calcFitness()>pop[popSize-1].fitness*/
            newPop.push_back(tmpChroma);
        }
    }

    //mutate by the Probaility designated by mutaRate
    void mutate(){
        for(uint i=0;i<newPop.size();i++){
            double r=rd.RandomReal(0,1);
            if(r<pm){
                int mutateOne=rd.RandomInteger(0,newPop.size()-1);
                Chroma tmp;
                do{
                    tmp=newPop[mutateOne].mutate();
                }while(find(newPop.begin(),newPop.end(),tmp)!=newPop.end()||find(pop.begin(),pop.end(),tmp)!=pop.end());
                newPop[mutateOne]=tmp;
            }
        }
    }

    void sortPop(){
        sort(pop.begin(),pop.end());
    }

    //Main process of the Genetic Algorithm
    void genetic(){

        initialize();
        //control time to 4 min 40s
        for(t=0;t<MaxGenerations;t++){
            totaltime=(double)(clock()-start)/CLOCKS_PER_SEC;
            if(totaltime>280){//||t-currentOptimal.t>1000){
                break;
            }
            cout<<"t:"<<t<<endl;
            if(t==0){
                calcFitnesses();
            }

            sortPop();

            //store current Optimal
            if(t==0){
                currentOptimal=pop[0];
            }else{
                if(pop[0].fitness<currentOptimal.fitness){
                    currentOptimal=pop[0];
                    currentOptimal.t=t;
                }
            }

           calcProbs();
           //popSize += rd.RandomInteger((int) (-0.02*popSize),(int)(0.02*popSize));  //make population size slightly mutable to emulate natural process
            mate();
            mutate();
            calcNewFitnesses();

            //store the top Chromas of pop and newPop to merge
            merge.reserve(pop.size()+newPop.size());
            merge.insert(merge.end(),pop.begin(),pop.end());
            merge.insert(merge.end(),newPop.begin(),newPop.end());
            sort(merge.begin(),merge.end());

            pop.clear();
            pop.insert(pop.end(),merge.begin(),merge.begin()+popSize);
            merge.clear();
            newPop.clear();
        }
    }

};

//计算城市间距离
double cityDist(City A,City B){
    return sqrt(pow((A.posX-B.posX),2)+pow((A.posY-B.posY),2));
}




int main(int argc,char* argv[]){
    ifstream infile;
    ofstream outfile;

    start=clock();
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
    std::vector<City> cities;

    //读入文件
    for(int i=0;i<cityNum;i++){
        infile>>tmpCity.cityName;
        infile>>tmpCity.posX;
        infile>>tmpCity.posY;
        tmpCity.cityN=i;
        cities.push_back(tmpCity);
        cityTable.insert(std::pair<int,City>(tmpCity.cityN,cities[i]));
    }

    //用于存储距离的表，查表减少运算量
    disTable= boost::numeric::ublas::matrix<double>(cityNum, cityNum);
    //Mat disTable[cityNum][cityNum];

    //计算用于存储距离的表
    for(int i=0;i<cityNum;i++){
        for(int j=i;j<cityNum;j++){
            disTable(i,j)=disTable(j,i)=cityDist(cities[i],cities[j]);
        }
    }

    Population ppl;
    ppl.genetic();
    // cout<<ppl<<endl;
    //ppl.calcFitnesses();
    //sort(ppl.pop.begin(),ppl.pop.end());
    // cout<<ppl;
    cout<<currentOptimal<<endl;
    cout<<currentOptimal.fitness<<endl;
    cout<<totaltime<<endl;
    for(int i=0;i<cityNum;i++){
        outfile<<cityTable[currentOptimal.series[i]].cityName<<' ';
    }
    outfile<<currentOptimal.fitness<<endl;
    //pause();
    return 0;
}

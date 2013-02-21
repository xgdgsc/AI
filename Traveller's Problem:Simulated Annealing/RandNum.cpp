#include<iostream>
#include<cstdlib>
#include<ctime>
#include"Rand.h"
using namespace std;

Rand::Rand()
  {
  mrand=rand;//default
  srand(time(NULL));//initialize
  }
  Rand::Rand(int(*p)())
  {
    mrand=p;
    srand(time(NULL));//initialize
  }
int RandNumInteger::RandomInteger(int min,int max)
  {
    if(min>max)
      {
	cerr<<"min>max,Please verify input!"<<endl;
	exit(1);
      }
  return mrand()%(max-min+1)+min;
  //double _d = (double)rand() / ((double)RAND_MAX + 1.0);
  //return (min+(int)(_d*(max-min+1)));
  }
int RandNumInteger::RandomInteger(int max)
  {
    if(0>max)
      {
	cerr<<"0>max,Please verify input!"<<endl;
	exit(1);
      }
  return mrand()%(max+1);
  }
int RandNumInteger::RandomInteger()
  {
    return mrand()%2;
  }
int* RandArrayInteger::RandomInteger(int min,int max,int n)
  {
    if(min>max)
      {
	cerr<<"min>max,Please verify input!"<<endl;
	exit(1);
      }
    int* rna=new int[n];//create array
    for(int i=0;i<n;i++)
      {
	rna[i]=mrand()%(max-min+1)+min;
      }
    return rna;
  }
int** RandArrayInteger::RandomInteger(int min,int max,int m,int n)
  {
    if(min>max)
      {
	cerr<<"min>max,Please verify input!"<<endl;
	exit(1);
      }
    int** rna=new int*[m];
    for(int i=0;i<m;i++)
      {
	rna[i]=new int[n];
      }
    for(int i=0;i<m;i++)
      for(int j=0;j<n;j++)
	rna[i][j]=mrand()%(max-min+1)+min;
    return rna;
  }
double RandNumReal::RandomReal()//0~1
  {
    return (double)mrand()/((double)RAND_MAX);
  }

double RandNumReal::RandomReal(double min,double max)
{
      if(min>max)
      {
	cerr<<"min>max,Please verify input!"<<endl;
	exit(1);
      }
      return min+(max-min)*RandomReal();
}
  double* RandArrayReal::RandomReal(int n)
  {
    double * rnra=new double[n];
    for(int i=0;i<n;i++)
    {
      rnra[i]=(double)mrand()/((double)RAND_MAX);
    }
    return rnra;
  }
 double ** RandArrayReal::RandomReal(int m,int n)
  {
     double** rna=new double*[m];
    for(int i=0;i<m;i++)
      {
	rna[i]=new double[n];//allocation
      }
     for(int i=0;i<m;i++)
      for(int j=0;j<n;j++)
	rna[i][j]=(double)mrand()/((double)RAND_MAX);//store
      return rna;
  }
double* RandArrayReal::RandomReal(double min,double max,int n)
{
     if(min>max)
      {
	cerr<<"min>max,Please verify input!"<<endl;
	exit(1);
      }
    double* rna=RandomReal(n);
     for(int i=0;i<n;i++)
     {
       rna[i]=rna[i]*(max-min)+min;
     }
    return rna;
}
double **RandArrayReal::RandomReal(double min,double max,int m,int n)
{
      if(min>max)
      {
	cerr<<"min>max,Please verify input!"<<endl;
	exit(1);
      }
      double** rna=RandomReal(m,n);
     for(int i=0;i<m;i++)
      for(int j=0;j<n;j++)
	rna[i][j]=rna[i][j]*(max-min)+min;
      return rna;
}
DistributionFunction::DistributionFunction()
{
  PI=3.14159265358979323846264;
}
double DistributionFunction::Normal(double x,double miu,double sigma)
{
        return 1.0/(sqrt(2*PI)*sigma) * exp(-1*(x-miu)*(x-miu)/(2*sigma*sigma));
}
double DistributionFunction::ExpDistr(double x,double lamda)
{
  return lamda*exp(-lamda*x);
}
double RandNumDistribution::NormalRandom(double miu,double sigma,double min,double max)
{
        do
        {
        x =RandomReal(min,max);
        y = Normal(x, miu, sigma);
        dScope = RandomReal(0, Normal(miu,miu,sigma));//highest point
        }while( dScope > y);
        return x;
}
double* RandNumDistribution::NormalRandom(double miu, double sigma, double min, double max, int n)
{
  double* RNDA=new double[n];
  for(int i=0;i<n;i++)
  {
    RNDA[i]=NormalRandom(miu,sigma,min,max);
  }
  return RNDA;
}

double RandNumDistribution::ExpRandom(double lamda,double min,double max)
{
  do
  {
    x=RandomReal(min,max);
    y=ExpDistr(x,lamda);
    dScope=RandomReal(0,ExpDistr(min,lamda));
  }while(dScope>y);
  return x;
}
double *RandNumDistribution::ExpRandom(double lamda,double min,double max,int n)
{
    double* RNDA=new double[n];
  for(int i=0;i<n;i++)
  {
    RNDA[i]=ExpRandom(lamda,min,max);
  }
  return RNDA;
}

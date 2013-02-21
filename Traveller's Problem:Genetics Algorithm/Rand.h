#ifndef RAND_H
#define RAND_H
#include<iostream>
#include<cstdlib>
#include<ctime>
#include<cmath>
using namespace std;
//以前写的随机数的类，在此只是用于产生随机数
class Rand
{
 public:
   Rand();//default constructor
   Rand(int(*p)());//constructor for user update
 protected:
  int (* mrand)(void);//pointer to the rand() function
};
class RandNumInteger:public Rand
{
public:
 RandNumInteger():Rand(){};
    RandNumInteger(int(*p)() ):Rand(p){};
 int RandomInteger(int min,int max);//generates int between min and max
 int RandomInteger(int max);//generate int between 0 and max
 int RandomInteger();//generate 0 or 1
};
class RandArrayInteger:public RandNumInteger
{
public:
    RandArrayInteger():RandNumInteger(){};
    RandArrayInteger(int(*p) ()):RandNumInteger(p){};
 int* RandomInteger(int min,int max,int n);//generate array[n] between min and max
 int** RandomInteger(int min,int max,int m,int n);//()generate matrix m*n,between min and max
};
class RandNumReal:public RandNumInteger
{
 public:
    RandNumReal():RandNumInteger(){};
    RandNumReal(int(*p)() ):RandNumInteger(p){};
double RandomReal();//generate real number between 0~1
double RandomReal(double min,double max);//generate real number between min and max
};
class RandArrayReal:public RandArrayInteger
{
public:
    RandArrayReal():RandArrayInteger(){};
    RandArrayReal(int(*p) ()):RandArrayInteger(p){};
  double* RandomReal(int n);//generate array[n] of 0~1
  double** RandomReal(int m,int n);//generate matrix[n] of 0~1
  double* RandomReal(double min,double max,int n);//generate array[n] of min~max
  double** RandomReal(double min,double max,int m,int n);//generate matrix[m][n] of min~max
};
class DistributionFunction
{
public:
  DistributionFunction();
  double Normal(double x,double miu,double sigma); //normal distribution
  double ExpDistr(double x,double lamda);//exp distribution
private:
  double PI;//const PI
};
class RandNumDistribution:public RandNumReal,public DistributionFunction
{
public:
    RandNumDistribution():RandNumReal(),DistributionFunction(){};
    RandNumDistribution(int(*p)() ):RandNumReal(p){};
  double NormalRandom(double miu,double sigma,double min,double max);//generate a number of Normal distribution
  double* NormalRandom(double miu,double sigma,double min,double max,int n);//generate array[n] of normal distribution
  double ExpRandom(double lamda,double min,double max);//generate a number of exp distribution
  double* ExpRandom(double lamda,double min,double max,int n);//generate array[n] of exp distribution
private:
  double x,y,dScope;//used in generating number according to distribution
};
#endif

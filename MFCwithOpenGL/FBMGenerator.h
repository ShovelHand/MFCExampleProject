#pragma once
#include <math.h>
#include <stdlib.h>     /* srand, rand */
#include <stdio.h>
#include <Eigen/Dense>

#define FBM_SIZE 512
#define MAX_OCTAVES 10  

typedef Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> GreyLevelImage;
typedef Eigen::Matrix<Eigen::Vector3f, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> RGBImage;

class FBMGenerator
{
public:
	FBMGenerator(int randSeed);
	~FBMGenerator();

	//float GetArray(){ return noiseArray;}
	RGBImage BuildNoiseImage(int width, int height);

	int m_iPeriod;
	float m_fH;
	float m_fLac;
	int m_iOctaves;
	float m_fOffset;

	bool m_bDistort;
	float m_fDistort;
	int m_iRandSeed;

	bool m_bHills;
	bool m_bRidges;


private:
	float noiseArray[FBM_SIZE][FBM_SIZE];

	float Mix(float x, float y, float alpha);
	float F(float t) { return 6 * pow(t, 5) - 15 * pow(t, 4) + 10 * pow(t, 3); }
	float Rand0_1() { return ((float)std::rand()) / ((float)RAND_MAX); }


	


	float FBM(Eigen::Vector3f point, float H, float lacunarity, int octaves, float offset);
	float FBMHills(Eigen::Vector3f point, float H, float lacunarity, int octaves, float offset);
	float FBMRidges(Eigen::Vector3f point, float H, float lacunarity, int octaves, float offset);

	float Distort(Eigen::Vector3f point, float H, float Lacunarity, int octaves, float distort);
};








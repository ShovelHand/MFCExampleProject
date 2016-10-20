#pragma once
#include <math.h>
#include <stdlib.h>     /* srand, rand */
#include <stdio.h>
#include <Eigen/Dense>

#define FBM_SIZE 512

typedef Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> GreyLevelImage;
typedef Eigen::Matrix<Eigen::Vector3f, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> RGBImage;

class FBMGenerator
{
public:
	FBMGenerator();
	~FBMGenerator();

	//float GetArray(){ return noiseArray;}
	RGBImage BuildNoiseImage(int width, int height);

private:
	float noiseArray[FBM_SIZE][FBM_SIZE];

	float Mix(float x, float y, float alpha);
	float F(float t) { return 6 * pow(t, 5) - 15 * pow(t, 4) + 10 * pow(t, 3); }
	float Rand0_1() { return ((float)std::rand()) / ((float)RAND_MAX); }

	

	float FBM(Eigen::Vector3f point, float H, float lacunarity, int octaves, float offset);
};








#pragma once
#include "stdafx.h"
#include "FBMGenerator.h"

//using namespace glm;

FBMGenerator::FBMGenerator(int randSeed) :
	m_fLac(2.0f)
	, m_fH(0.2f)
	, m_fOffset(0.7f)
	, m_iOctaves(7)
	, m_iPeriod(128)
	, m_bDistort(false)
	, m_fDistort(0.1f)
	, m_bHills(false)
	, m_bRidges(false)
{
	m_iRandSeed = randSeed;
}


FBMGenerator::~FBMGenerator()
{
}

float FBMGenerator::Mix(float x, float y, float alpha)
{
	return y * alpha + x * (1.0f - alpha);
}

RGBImage FBMGenerator::BuildNoiseImage(int width, int height)
{
	RGBImage base(width, height);
	RGBImage result(width, height);
	//set vertex heights using Perlin noise
//	int period = 128;
	float frequency = 1.0f / m_iPeriod;
	std::srand(m_iRandSeed);
	Eigen::Vector3f randGradientVec;

	for (int i = 0; i < width; ++i)
		for (int j = 0; j < height; ++j)
		{
			//we use vec3 instead of vec2 but set the last component to zero
			randGradientVec.x() = cos(2 * M_PI * Rand0_1());
			randGradientVec.y() = sin(2 * M_PI * Rand0_1());
			randGradientVec.z() = 0;
			base(i, j) = randGradientVec; //make base noise image
		}

	for (int i = 0; i < width; ++i)
	{
		for (int j = 0; j < height; ++j)
		{
			int left = (i / m_iPeriod) *  m_iPeriod;
			int right = (left + m_iPeriod) % width;
			float dx = (i - left) * frequency;

			int top = (j / m_iPeriod) *  m_iPeriod;
			int bottom = (top + m_iPeriod) % height;
			float dy = (j - top) * frequency;

			Eigen::Vector2f a(dx, -dy);
			Eigen::Vector2f b(dx - 1, -dy);
			Eigen::Vector2f c(dx - 1, 1 - dy);
			Eigen::Vector2f d(dx, 1 - dy);

			Eigen::Vector3f topleft = base(left, top);
			float s = topleft(0) * a(0) + topleft(1) * a(1);
			Eigen::Vector3f topright = base(right, top);
			float t = topright(0) * b(0) + topright(1) * b(1);
			Eigen::Vector3f bottomleft = base(left, bottom);
			float u = bottomleft(0) * d(0) + bottomleft(1) * d(1);
			Eigen::Vector3f bottomright = base(right, bottom);
			float v = bottomright(0) * c(0) + bottomright(1) * c(1);

			float fx = F(dx);
			float fy = F(dy);

			float st = Mix(s, t, fx);
			float uv = Mix(u, v, fx);
			float noise = Mix(st, uv, fy);

			//	result(i,j) = Eigen::Vector3f(noise, noise, noise);
			noiseArray[i][j] = noise;
		}
	}

	//Build FBM image based on perlin noise.
	for (int i = 0; i < base.rows(); i++)
		for (int j = 0; j < base.cols(); j++)
		{
			float value;
			if(!m_bDistort)
			{
				if(m_bHills)
					value = FBMHills(Eigen::Vector3f(j, i, 0), m_fH, m_fLac, m_iOctaves, m_fOffset);
				else if(m_bRidges)
					value = FBMRidges(Eigen::Vector3f(j, i, 0), m_fH, m_fLac, m_iOctaves, m_fOffset);
				else
					 value = FBM(Eigen::Vector3f(j, i, 0), m_fH, m_fLac, m_iOctaves, m_fOffset);
				result(j, i) = Eigen::Vector3f(value, value, value);
			}
			else
			{
				value = Distort(Eigen::Vector3f(j, i, 0), m_fH, m_fLac, m_iOctaves, m_fDistort);
				result(j, i) = Eigen::Vector3f(value, value, value);
			}
		}

	return result;
}




float FBMGenerator::FBM(Eigen::Vector3f point, float H, float lacunarity, int octaves, float offset)
{
    
	float value, frequency, remainder, signal, weight;
	int i;
	static bool first = true;
	static float exp_array[MAX_OCTAVES];

	if (first)
	{
		frequency = 1.0f;

		for (i = 0; i< MAX_OCTAVES; i++)
		{ /* compute weight for each frequency */
			exp_array[i] = pow(frequency, -H);
			frequency *= lacunarity;
		}

		first = false;
	}
	
	value = 0.0f;
	
	for (int i = 0; i < MAX_OCTAVES; ++i)
	{
		value += noiseArray[int(point.x() + offset) % FBM_SIZE][int(point.y() + offset) % FBM_SIZE] * exp_array[i];
		point.x() *= lacunarity; point.y() *= lacunarity;// point.z() *= lacunarity;
	}

	remainder = octaves - (int)octaves;
	if (remainder) /* add in "octaves" remainder */ /* "i" and spatial freq. are preset in loop above */
		value += remainder * noiseArray[int(point.x()) / FBM_SIZE][int(point.y()) / FBM_SIZE] * exp_array[i];

	return value;
}

float FBMGenerator::FBMHills(Eigen::Vector3f point, float H, float lacunarity, int octaves, float offset)
{

	float value, frequency, remainder, signal, weight;
	int i = 0;
	static bool first = true;
	static float exp_array[MAX_OCTAVES];

	if (first)
	{
		frequency = 1.0f;

		for (i = 0; i< MAX_OCTAVES; i++)
		{ /* compute weight for each frequency */
			exp_array[i] = pow(frequency, -H);
			frequency *= lacunarity;
		}

		first = false;
	}
	value = noiseArray[int(point.x() + offset) % FBM_SIZE][int(point.y() + offset) % FBM_SIZE] * exp_array[i];
	weight = value;
	point.x() *= lacunarity; point.y() *= lacunarity;// point.z() *= lacunarity;

	/* inner loop of spectral construction */
	for (i = 0; i < octaves; i++)
	{
		if (weight > 1.0) weight = 1.0;

		signal = noiseArray[int(point.x() + offset) % FBM_SIZE][int(point.y() + offset) % FBM_SIZE] * exp_array[i];
		value += weight * signal;
		weight *= signal;
		point.x() *= lacunarity; point.y() *= lacunarity;// point.z() *= lacunarity;
	} /* for */

	remainder = octaves - (int)octaves;
	if (remainder) /* add in "octaves" remainder */ /* "i" and spatial freq. are preset in loop above */
		value += remainder * noiseArray[int(point.x()) / FBM_SIZE][int(point.y()) / FBM_SIZE] * exp_array[i];

	return value;
}

float FBMGenerator::FBMRidges(Eigen::Vector3f point, float H, float lacunarity, int octaves, float offset)
{

	float value, frequency, remainder, signal, weight;
	int i;
	static bool first = true;
	static float exp_array[MAX_OCTAVES];

	if (first)
	{
		frequency = 1;

		for (i = 0; i< MAX_OCTAVES; i++)
		{ /* compute weight for each frequency */
			exp_array[i] = pow(frequency, -H);
			frequency *= lacunarity;
		}

		first = false;
	}
	1 - abs(value = noiseArray[int(point.x() + offset) % FBM_SIZE][int(point.y() + offset) % FBM_SIZE] * exp_array[0]);
	weight = value;
	/* inner loop of spectral construction */
	for (i = 0; i < octaves; i++)
	{
		if (weight > 1.0) weight = 1.0;
		signal = 1 - abs(noiseArray[int(point.x() + offset) % FBM_SIZE][int(point.y() + offset) % FBM_SIZE] * exp_array[i]);
		value += weight * signal;
		weight *= signal;
		point.x() *= lacunarity; point.y() *= lacunarity;// point.z() *= lacunarity;
	} /* for */

	remainder = octaves - (int)octaves;
	if (remainder) /* add in "octaves" remainder */ /* "i" and spatial freq. are preset in loop above */
		value += remainder * noiseArray[int(point.x()) / FBM_SIZE][int(point.y()) / FBM_SIZE] * exp_array[i];

	return value;
}

float FBMGenerator::Distort(Eigen::Vector3f point, float H, float Lacunarity, int octaves, float distortion)
{
	Eigen::Vector3f distort;
	Eigen::Vector3f tmp = point;
	if (m_bHills)
	{
		distort.x() = FBMHills(tmp, m_fH, m_fLac, m_iOctaves, m_fOffset);
		tmp.x() += 10.5f;
		distort.y() = FBMHills(tmp, m_fH, m_fLac, m_iOctaves, m_fOffset);
		tmp.x() += 10.5f;
		distort.z() = FBMHills(tmp, m_fH, m_fLac, m_iOctaves, m_fOffset);
	}
	else if (m_bRidges) 
	{
		distort.x() = FBMRidges(tmp, m_fH, m_fLac, m_iOctaves, m_fOffset);
		tmp.x() += 10.5f;
		distort.y() = FBMRidges(tmp, m_fH, m_fLac, m_iOctaves, m_fOffset);
		tmp.x() += 10.5f;
		distort.z() = FBMRidges(tmp, m_fH, m_fLac, m_iOctaves, m_fOffset);
	}

	else
	{
		distort.x() = FBM(tmp, m_fH, m_fLac, m_iOctaves, m_fOffset);
		tmp.x() += 10.5f;
		distort.y() = FBM(tmp, m_fH, m_fLac, m_iOctaves, m_fOffset);
		tmp.x() += 10.5f;
		distort.z() = FBM(tmp, m_fH, m_fLac, m_iOctaves, m_fOffset);
	}

	point.x() =  distort.x() * distortion;
	point.y() = distortion * distort.y();
	point.z() = distortion * distort.z();

	if (m_bHills)
	{
		return FBMHills(point, m_fH, m_fLac, m_iOctaves, m_fOffset) /100.0f;
	}
	else if (m_bRidges)
	{
		return FBMRidges(point, m_fH, m_fLac, m_iOctaves, m_fOffset) / 100.0f;
	}

	else
	{
		return FBM(point, m_fH, m_fLac, m_iOctaves, m_fOffset) / 100.0f;
	}
		
}
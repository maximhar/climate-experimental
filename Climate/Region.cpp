#include "Region.h"
#include <cmath>
#include <algorithm>

const float fertilityRateMin = 1; // per woman
const float fertilityRateMax = 8; // per woman

float startF = 2.05;

const float baseEnergyEfficiency = 1.0;

const int   adultAge = 18;
const int	elderAge = 65;

const int   fertilityMin = 15;
const int   fertilityMax = 49;

std::vector<uint64_t> sPrevDeadD;
uint64_t sPrevTotalDead;


float DeathCause::GetMortalityAtAge(int age) const
{
	float rate = 0;

	switch (model)
	{
	case ModelConstant:
		rate = (age >= offset) ? std::max(.0f, linearK) : .0f;
		break;
	case ModelConstantBounded:
		rate = (age >= offset && age < bound) ? std::max(.0f, linearK) : .0f;
		break;
	case ModelExponential:
		rate = std::max(.0f, linearK) * std::expf(expK * (age - offset));
		break;
	}

	return isnormal(rate) ? rate : .0f;
}


static std::vector<DeathCause> Diseases = 
{
	{"Cancer", 0.105f, 0.08f, 100.5f, ModelExponential},
	{"Heart disease", 0.093f, 0.12f, 97.0f, ModelExponential },
	{"Respiratory", 0.055f, 0.09f, 105.0f, ModelExponential },
	{"Nervous", 0.031f, 0.10f, 105.0f, ModelExponential },
	{"Road deaths", 0.00004f, .0f, 18.f, ModelConstant },
	{"Workplace", 0.000005f, .0f, 18.f, ModelConstantBounded, (float)elderAge },
	{"Early childhood", 0.005f, 0.0f, .0f, ModelConstantBounded, 5.f},
	{"Crime", 0.00001f, .0f, 18.f }
};


Region::Region(std::string name, int currentYear, PopulationUnitList population, float gdp, 
	float education, float resourceCapacity, float energyEfficiency, float healthcareEfficiency)
	: 
	mName(name), 
	mCurrentYear(currentYear), 
	mLivingGenerations(population), 
	mGdp(gdp), 
	mEducation(education), 
	mResourceCapacity(resourceCapacity), 
	mEnergyEfficiency(energyEfficiency),
	mHealthcareEfficiency(healthcareEfficiency)
{
	uint64_t tpopulation = 0;

	for (PopulationUnit& pop : mLivingGenerations)
	{
		tpopulation += pop.Count;
	}

	mPopulation = tpopulation;
}

void Region::Evaluate()
{
	// calculate deaths
	std::vector<PopulationUnit> newGenerations(mLivingGenerations);
	uint64_t totalDead = 0;
	uint64_t totalDeadW = 0;
	int ccause = 0;
	std::vector<uint64_t> deadD;
	deadD.resize(Diseases.size());
	std::vector<uint64_t> deadW;
	deadW.resize(Diseases.size());
	sPrevDeadD.resize(Diseases.size());
	for (DeathCause& cause : Diseases)
	{
		for (int i = 0; i < newGenerations.size(); i++)
		{
			const PopulationUnit& oldPop = mLivingGenerations[i];
			PopulationUnit&		  newPop = newGenerations[i];

			uint64_t dead = (float)oldPop.Count * 
							cause.GetMortalityAtAge(oldPop.GetAge(mCurrentYear));

			dead = std::min(dead, newPop.Count);
			newPop.Count -= dead;
			totalDead += dead;
			totalDeadW += dead * oldPop.GetAge(mCurrentYear);
			deadD[ccause] += dead;
			deadW[ccause] += dead * oldPop.GetAge(mCurrentYear);
		}
		ccause++;
	}

	Diseases[0].linearK *= 0.98;
	Diseases[1].linearK *= 0.96;
	Diseases[2].linearK *= 0.99;
	Diseases[3].linearK *= 0.999;
	Diseases[3].offset += 0.05;
	Diseases[6].linearK *= 0.9991;
	Diseases[7].linearK *= 1.001;

	// calculate newly born
	uint64_t children = 0, adults = 0, elderly = 0, fertile = 0;
	for (const PopulationUnit& pop : mLivingGenerations)
	{
		if (pop.YearBorn > mCurrentYear - adultAge)
		{
			children += pop.Count;
		}
		else if (pop.InAgeRange(adultAge, elderAge, mCurrentYear))
		{
			adults += pop.Count;
		}
		else
		{
			elderly += pop.Count;
		}

		if (pop.InAgeRange(fertilityMin, fertilityMax, mCurrentYear))
		{
			fertile += pop.Count;
		}
	}

	uint64_t women = fertile / 2;
	float    fertilityAnnual = startF / (fertilityMax - fertilityMin);
	uint64_t newGeneration = women * fertilityAnnual;

	newGenerations.push_back({ mCurrentYear, newGeneration });

	uint64_t population = 0;
	mLivingGenerations.clear();


	for (PopulationUnit& pop : newGenerations)
	{
		population += pop.Count;
		if (pop.Count > 0)
			mLivingGenerations.push_back(pop);
	}

	mPopulation = population;


	mCurrentYear++;

	float avgExpectancy = float(totalDeadW) / float(totalDead);

	printf("[%d, %s]   Population: %6.2f million \n", 
		mCurrentYear, mName.c_str(), (float)mPopulation / 1000000.f);
	printf("  Fertility    : %6.2f/woman, Newborns: %d\n", startF, newGenerations.back().Count);
	printf("  Avg. lifespan: %6.2f,       Deaths:   %lld, oldest: %3d\n",
		avgExpectancy, totalDead, mLivingGenerations.front().GetAge(mCurrentYear));
	ccause = 0;
	printf("   %-15s  %-5s   %-5s   %-5s  %s\n", "Disease:", "% of all:", "Change:", "Avg. age:", "Deaths:");
	for (DeathCause& cause : Diseases)
	{
		float perc = 100 * ((deadD[ccause]>0) ? ((float)deadD[ccause] / (float)totalDead) : 0.f);
		float prev = 100 * ((sPrevDeadD[ccause]>0) ? ((float)sPrevDeadD[ccause] / (float)sPrevTotalDead) : 0.f);
		avgExpectancy = (deadD[ccause] > 0) ? float(deadW[ccause]) / float(deadD[ccause])
										  : 0.f;
		printf("   %-15s  %5.2f%%      %+7.3f  %6.2f      %d\n", 
			cause.Name.c_str(), perc, perc - prev, avgExpectancy, deadD[ccause]);
		ccause++;
	}

	sPrevDeadD = deadD;
	sPrevTotalDead = totalDead;
}

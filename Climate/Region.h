#pragma once
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>

enum MortalityModel
{
	ModelConstant,
	ModelConstantBounded,
	ModelExponential
};

struct DeathCause
{
	std::string Name;
	float linearK;
	float expK;
	float offset;
	MortalityModel model;
	float bound;

	float		GetMortalityAtAge(int age) const
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
};

struct PopulationUnit
{
	int			YearBorn;
	uint64_t    Count;
	int			GetAge(int currentYear) const { return currentYear - YearBorn; }
	bool        InAgeRange(int min, int max, int currentYear) const
	{
		return GetAge(currentYear) >= min && GetAge(currentYear) < max;
	}
};

typedef std::vector<PopulationUnit> PopulationUnitList;

class Region
{
public:
					Region(std::string name, 
						int currentYear,
						PopulationUnitList population,
						float gdp,
						float education, 
						float resourceCapacity, 
						float energyEfficiency,
						float healthcareEfficiency);

	void			Evaluate();

	std::string			mName;
	int					mCurrentYear;
	float				mEmissions; // GT/y
	float				mGdp; // B USD/y
	uint64_t			mPopulation; // people
	float				mEducation; // 0.00 - 1.00
	float				mEnergyUse; // GW/y
	float				mResourceUse; // GT/y
	float				mResourceProduction; // GT/y
	float				mResourceCapacity; // GT
	float				mEnergyEfficiency; // GT/GW
	float				mHealthcareSpending; // USD/pp
	float				mHealthcareEfficiency; // unitless
	PopulationUnitList  mLivingGenerations; // list of PopulationUnits
};
#pragma once
#include <vector>
#include "Region.h"


typedef std::vector<Region*> RegionList;

class WorldContext
{
public:
					WorldContext(RegionList regions);
	void			Evaluate();

	int				mTurn;
	float			mPpm;
	float			mTemperature;
	float			mSeaLevel;
	uint64_t		mPopulation;
	RegionList		mRegions;
};
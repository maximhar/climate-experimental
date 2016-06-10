#include "WorldContext.h"

WorldContext::WorldContext(RegionList regions) :
	mTurn(0),
	mRegions(regions)
{
}

void WorldContext::Evaluate()
{
	uint64_t pop = 0;
	for (Region* region : mRegions)
	{
		region->Evaluate();
		pop += region->mPopulation;
	}
	mPopulation = pop;
	mTurn++;
}

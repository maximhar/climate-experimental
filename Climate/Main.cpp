#include "WorldContext.h"
#include <stdio.h>

int main(int argc, char** argv)
{
	RegionList regions
	{
		new Region("Europe", 2015, 
			{
				{ 1920, 1000000 },
				{ 1921, 1000000 },
				{ 1922, 1000000 },
				{ 1923, 1000000 },
				{ 1924, 1000000 },
				{ 1925, 1000000 },
				{ 1926, 1000000 },
				{ 1927, 1000000 },
				{ 1928, 1000000 },
				{ 1929, 1000000 },

				{ 1930, 2000000 },
				{ 1931, 2000000 },
				{ 1932, 2000000 },
				{ 1933, 2000000 },
				{ 1934, 2000000 },
				{ 1935, 2000000 },
				{ 1936, 2000000 },
				{ 1937, 2000000 },
				{ 1938, 2000000 },
				{ 1939, 2000000 },

				{ 1940, 2000000 },
				{ 1941, 2000000 },
				{ 1942, 2000000 },
				{ 1943, 2000000 },
				{ 1944, 2000000 },
				{ 1945, 2000000 },
				{ 1946, 2000000 },
				{ 1947, 2000000 },
				{ 1948, 2000000 },
				{ 1949, 2000000 },

				{ 1950, 2000000 },
				{ 1951, 2000000 },
				{ 1952, 2000000 },
				{ 1953, 2000000 },
				{ 1954, 2000000 },
				{ 1955, 2000000 },
				{ 1956, 2000000 },
				{ 1957, 2000000 },
				{ 1958, 2000000 },
				{ 1959, 2000000 },

				{ 1960, 2000000 },
				{ 1961, 2000000 },
				{ 1962, 2000000 },
				{ 1963, 2000000 },
				{ 1964, 2000000 },
				{ 1965, 2000000 },
				{ 1966, 2000000 },
				{ 1967, 2000000 },
				{ 1968, 2000000 },
				{ 1969, 2000000 },

				{ 1970, 2000000 },
				{ 1971, 2000000 },
				{ 1972, 2000000 },
				{ 1973, 2000000 },
				{ 1974, 2000000 },
				{ 1975, 2000000 },
				{ 1976, 2000000 },
				{ 1977, 2000000 },
				{ 1978, 2000000 },
				{ 1979, 2000000 },

				{ 1980, 2000000 },
				{ 1981, 2000000 },
				{ 1982, 2000000 },
				{ 1983, 2000000 },
				{ 1984, 2000000 },
				{ 1985, 2000000 },
				{ 1986, 2000000 },
				{ 1987, 2000000 },
				{ 1988, 2000000 },
				{ 1989, 2000000 },

				{ 1990, 2000000 },
				{ 1991, 2000000 },
				{ 1992, 2000000 },
				{ 1993, 2000000 },
				{ 1994, 2000000 },
				{ 1995, 2000000 },
				{ 1996, 2000000 },
				{ 1997, 2000000 },
				{ 1998, 2000000 },
				{ 1999, 2000000 },

				{ 2000, 2000000 },
				{ 2001, 2000000 },
				{ 2002, 2000000 },
				{ 2003, 2000000 },
				{ 2004, 2000000 },
				{ 2005, 2000000 },
				{ 2006, 2000000 },
				{ 2007, 2000000 },
				{ 2008, 2000000 },
				{ 2009, 2000000 },

				{ 2010, 2000000 },
				{ 2011, 2000000 },
				{ 2012, 2000000 },
				{ 2013, 2000000 },
				{ 2014, 2000000 },
			},
			500000, 1.0, 1.0, 1.0, 1.0)
	};
	WorldContext con (regions);

	while (true)
	{
		con.Evaluate();
		fgetchar();
	}
}
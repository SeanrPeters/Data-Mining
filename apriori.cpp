//by Sean Peters
//10/16/2022


#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <algorithm>
#include <deque>
#include <vector>
#include <map>
#include <cmath>



void reading_theFile(char*, std::deque<std::vector<int>>&);
void printing_ints(const std::deque<std::vector<int>>&);


std::deque<std::vector<int>> gen_candidate(const std::deque<std::vector<int>>&, const std::deque<std::vector<int>>&);
std::deque<std::vector<int>> gen_new(const std::deque<std::vector<int>>&);
std::deque<std::vector<int>> get_freq(const std::deque<std::vector<int>>&, const std::deque<std::vector<int>>&, double);
std::deque<std::vector<int>> apriori(const std::deque<std::vector<int>>&, double);




int main(int argc, char** argv) 
{

	if (argc < 3) 
	{
		std::cerr << "need more arguments " << '\n';
		exit(1);
	}

	// this gets the minnimum support

	int theNum = 0;

	double min_sup = 0;
	for (int i = 0; argv[2][i] != '\0'; ++i) 
		++theNum;
	
	for (int i = 0; argv[2][i] != '\0'; ++i) 
		min_sup += ((argv[2][i] - '0') * pow(10, theNum - i - 1));
	

	
	std::cout << "The min support is: " << min_sup << '\n';

	//this will make the data set
	std::deque<std::vector<int>> int_item_sets;
	reading_theFile(argv[1], int_item_sets);

	//this will find the frequency
	std::deque<std::vector<int>> frequent_sets = apriori(int_item_sets, min_sup);

	std::cout << "Printing Results;" << '\n';
	std::cout << "Frequent Item Sets:" << '\n';
	
	printing_ints(frequent_sets);
	return 0;
}
//the implimentation of the functions
void reading_theFile(char* file_name, std::deque<std::vector<int>>& int_sets) 
{
	std::ifstream in_file(file_name);
	std::vector<int> candidateSet;
	int_sets.push_back(std::vector<int>());

	std::string line;
	while (!in_file.eof())
	{
		std::getline(in_file, line);
		for (int i = 0; line[i] != '\0'; ++i) 
		{
			if (line[i] != ' ') 
			{
				int num = line[i] - '0';

				(int_sets.end() - 1) -> push_back(num);
				std::vector<int>::iterator itr = std::find(candidateSet.begin(), candidateSet.end(), num);

				if (itr == candidateSet.end()) 
					candidateSet.push_back(num);
			}
		}
		int_sets.push_back(std::vector<int>());
	}

//sorting the candidates
	std::sort(candidateSet.begin(), candidateSet.end());

	//erasing the sets with nohing in them
	for (std::deque<std::vector<int>>::iterator theIT = int_sets.begin(); theIT != int_sets.end(); ++theIT) 
	{
		if (theIT -> empty())
		{
			theIT = int_sets.erase(theIT);
			--theIT;
		}
	}


	in_file.close();
}

void printing_ints(const std::deque<std::vector<int>>& item_sets) 
{
	for (const std::vector<int>& vec : item_sets) 
	{
		for (int item : vec) 
			std::cout << item << " ";
		
		std::cout << '\n';
	}
}

std::deque<std::vector<int>> gen_candidate(const std::deque<std::vector<int>>& previous, const std::deque<std::vector<int>>& first_set) 
{
	std::deque<std::vector<int>> result;
	std::vector<int> cur_set;

	for (const std::vector<int>& vec : previous) 
	{
		for (std::vector<int> theElem : first_set)
		{
			if (std::find(vec.begin(), vec.end(), theElem[0]) == vec.end()) 
			{
				std::vector<int> theTMP_vect(vec);

				for (std::vector<int>::iterator itr = theTMP_vect.begin(); itr != theTMP_vect.end(); ++itr)
				{
					if (theElem[0] < *itr || itr == theTMP_vect.end() - 1)
					{
						if (itr == theTMP_vect.end() - 1 && !(theElem[0] < *itr)) 
							theTMP_vect.push_back(theElem[0]);
						
						else 
							theTMP_vect.insert(itr, theElem[0]);
						

						if (std::find(result.begin(), result.end(), theTMP_vect) == result.end()) 
							result.push_back(theTMP_vect);
						
						break;
					}
				}
			}
		}
	}

	return result;
}

std::deque<std::vector<int>> gen_new(const std::deque<std::vector<int>>& item_sets) 
{
	std::deque<std::vector<int>> candidate;
	for (const std::vector<int>& v : item_sets) 
		for (int x : v) 
		{
			std::vector<int> temp; temp.push_back(x);
			if (std::find(candidate.begin(), candidate.end(), temp) == candidate.end()) 
				candidate.push_back(temp);
			
		}

	return candidate;
}

std::deque<std::vector<int>> get_freq(const std::deque<std::vector<int>>& elem_set, const std::deque<std::vector<int>>& candidate_set, double min_support)
{
	std::deque<std::vector<int>> frequent_set;
	std::map<std::vector<int>, int> frequency_map;
	for (std::vector<int> v : candidate_set) 
		frequency_map.emplace(v, 0);
	

	for (const std::vector<int>& c_set : candidate_set) 
	{
		for (const std::vector<int>& set : elem_set) 
		{
			std::vector<int>::const_iterator candidate_itr = c_set.begin();
			std::vector<int>::const_iterator set_itr = set.begin();
			while (candidate_itr != c_set.end() && set_itr != set.end())
			{
				if (*candidate_itr == *set_itr) 
					++candidate_itr;
				
				++set_itr;
			}
			if (candidate_itr == c_set.end()) 
				frequency_map.find(c_set)->second += 1;
			
		}
	}

	for (auto pair : frequency_map)
	{
		double theFreq = ((double)pair.second / elem_set.size()) * 100;
		if (theFreq >= min_support) 
			frequent_set.push_back(pair.first);
		
	}

	return frequent_set;
}

std::deque<std::vector<int>> apriori(const std::deque<std::vector<int>>& elem_set, double min_support) 
{

	std::deque<std::vector<int>> theResult;
	std::deque<std::vector<int>> theCandidat;
	std::deque<std::vector<int>> freq;
	std::deque<std::vector<int>> freq2;

	int set_len = 1;
	int max_len;

	theCandidat = gen_new(elem_set);
	freq = get_freq(elem_set, theCandidat, min_support);
	freq2 = freq;

	for (std::vector<int> vec : freq2) 
		theResult.push_back(vec); 
	++max_len;

	max_len = theCandidat.size();

	while (set_len <= max_len) 
	{
		theCandidat = gen_candidate(freq, freq2);
		if (theCandidat.size() == 0) 
		 break; 
		freq = get_freq(elem_set, theCandidat, min_support);
		if (freq.size() == 0) 
			break; 
		for (std::vector<int> v : freq) 
			theResult.push_back(v); 
		++max_len;
	}

	return theResult;
}


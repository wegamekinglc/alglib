#include "utilities.hpp"
#include <iostream>

real_2d_array readVarianceMatrix(const std::string& path) {
    std::ifstream in(path.c_str());

    if (!in.is_open())
    {
        throw std::runtime_error("No file: " + path);
    }

    typedef boost::tokenizer<boost::escaped_list_separator<char> > Tokenizer;
    std::vector<std::string> vec;
    std::string line;

    std::vector<double> allData;

    int rows = 0;
    {
        while (std::getline(in, line)) {
            Tokenizer tok(line);
            vec.assign(tok.begin(), tok.end());
            for (size_t i = 0; i != vec.size(); ++i)
                allData.push_back(boost::lexical_cast<double>(vec[i]));
            rows += 1;
        }
    }

    if(allData.size() / rows != rows) 
    {
        throw std::runtime_error("Variance matrix is not a rectangle matrix");
    }

    real_2d_array res;
    res.setcontent(rows, rows, &allData[0]);

    return res;
}


double min(const real_1d_array& array, int n)
{
	double minimum = 1e308;

	if (n == 0)
		n = array.length();

	for (int i = 0; i != n; ++i)
	{
		if (array[i] < minimum)
			minimum = array[i];
	}
	return minimum;
}


double max(const real_1d_array& array, int n)
{
	double maximum = 1e-308;

	if (n == 0)
		n = array.length();

	for (int i = 0; i != n; ++i)
	{
		if (array[i] > maximum)
			maximum = array[i];
	}
	return maximum;
}


double sum(const real_1d_array& array, int n)
{
	double sumValue = 0.0;

	if (n == 0)
		n = array.length();

	for (int i = 0; i != n; ++i)
	{
		sumValue += array[i];
	}
	return sumValue;
}

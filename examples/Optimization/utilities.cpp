#include "utilities.hpp"

real_2d_array readVarianceMatrix(const std::string& path, int length) {
    std::ifstream in(path.c_str());
    typedef boost::tokenizer<boost::escaped_list_separator<char> > Tokenizer;
    std::vector<std::string> vec;
    std::string line;

    real_2d_array res;
    res.setlength(length, length);

    size_t count = 0;
    while (std::getline(in, line)) {
        Tokenizer tok(line);
        vec.assign(tok.begin(), tok.end());
        vec.resize(length);

        for (size_t i = 0; i != vec.size(); ++i)
            res[count][i] = boost::lexical_cast<double>(vec[i]);

        ++count;

        if (count == length)
            break;
    }

    return res;
}


double min(const real_1d_array& array)
{
    double minimum = 1e308;
    for(int i=0;i!=array.length();++i)
    {
        if(array[i] < minimum)
            minimum = array[i];
    }
    return minimum;
}


double max(const real_1d_array& array)
{
    double maximum = 1e-308;
    for(int i=0;i!=array.length();++i)
    {
        if(array[i] > maximum)
            maximum = array[i];
    }
    return maximum;
}


double sum(const real_1d_array& array)
{
    double sumValue = 0.0;
    for(int i=0;i!=array.length();++i)
    {
        sumValue += array[i];
    }
    return sumValue;
}

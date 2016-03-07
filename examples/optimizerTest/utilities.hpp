#ifndef ALGLIB_OPTIMIZER_TEST_UTILITIES_HPP
#define ALGLIB_OPTIMIZER_TEST_UTILITIES_HPP

#include "types.hpp"
#include <fstream>
#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>

real_2d_array readVarianceMatrix(const std::string& path);

double min(const real_1d_array& array);


double max(const real_1d_array& array);


double sum(const real_1d_array& array);

#endif //ALGLIB_UTILITIES_HPP

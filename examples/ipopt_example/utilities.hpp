#ifndef ALGLIB_OPTIMIZATION_UTILITIES_HPP
#define ALGLIB_OPTIMIZATION_UTILITIES_HPP

#include "types.hpp"
#include <fstream>
#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/limits.hpp>
#include <boost/shared_ptr.hpp>
#include <cmath>
#include <exception>
#include <sstream>
#include <string>

#define MAX_FUNCTION_EVALUATIONS 100

typedef double Real;
typedef size_t Size;
typedef int Integer;

#define QL_EPSILON ((std::numeric_limits<Real>::epsilon)())


inline bool close(Real x, Real y, Size n=42) {
	// Deals with +infinity and -infinity representations etc.
	if (x == y)
		return true;

	Real diff = std::fabs(x - y);
	Real tolerance = n * QL_EPSILON;

	if (x * y == 0.0) // x or y = 0.0
		return diff < (tolerance * tolerance);

	return diff <= tolerance*std::fabs(x) &&
		diff <= tolerance*std::fabs(y);
}


#if defined(_MSC_VER) || defined(__BORLANDC__)
// allow Visual Studio integration
inline std::string format(
#ifdef QL_ERROR_LINES
	const std::string& file, long line,
#else
	const std::string&, long,
#endif
#ifdef QL_ERROR_FUNCTIONS
	const std::string& function,
#else
	const std::string&,
#endif
	const std::string& message) {
	std::ostringstream msg;
#ifdef QL_ERROR_FUNCTIONS
	if (function != "(unknown)")
		msg << function << ": ";
#endif
#ifdef QL_ERROR_LINES
	msg << "\n  " << file << "(" << line << "): \n";
#endif
	msg << message;
	return msg.str();
}
#else
// use gcc format (e.g. for integration with Emacs)
inline std::string format(const std::string& file, long line,
	const std::string& function,
	const std::string& message) {
	std::ostringstream msg;
#ifdef QL_ERROR_LINES
	msg << "\n" << file << ":" << line << ": ";
#endif
#ifdef QL_ERROR_FUNCTIONS
	if (function != "(unknown)")
		msg << "In function `" << function << "': \n";
#endif
	msg << message;
	return msg.str();
}
#endif


class Error : public std::exception {
public:
	Error(const std::string& file, long line,
		const std::string& function,
		const std::string& message) {
		message_ = boost::shared_ptr<std::string>(new std::string(
			format(file, line, function, message)));
	}
	~Error() throw() {}
	const char* what() const throw () {
		return message_->c_str();
	}
private:
	boost::shared_ptr<std::string> message_;
};

#define MULTILINE_MACRO_BEGIN do {

#if defined(BOOST_MSVC) && BOOST_MSVC >= 1500
/* __pragma is available from VC++9 */
#define MULTILINE_MACRO_END \
        __pragma(warning(push)) \
        __pragma(warning(disable:4127)) \
        } while(false) \
        __pragma(warning(pop))
#else
#define MULTILINE_MACRO_END } while(false)
#endif

#define QL_FAIL(message) \
MULTILINE_MACRO_BEGIN \
    std::ostringstream _ql_msg_stream; \
    _ql_msg_stream << message; \
    throw Error(__FILE__,__LINE__, \
                          BOOST_CURRENT_FUNCTION,_ql_msg_stream.str()); \
MULTILINE_MACRO_END


/*! \def QL_ASSERT
\brief throw an error if the given condition is not verified
*/
#define QL_ASSERT(condition,message) \
if (!(condition)) { \
    std::ostringstream _ql_msg_stream; \
    _ql_msg_stream << message; \
    throw Error(__FILE__,__LINE__, \
                          BOOST_CURRENT_FUNCTION,_ql_msg_stream.str()); \
 } else 


/*! \def QL_REQUIRE
\brief throw an error if the given pre-condition is not verified
*/
#define QL_REQUIRE(condition,message) \
if (!(condition)) { \
    std::ostringstream _ql_msg_stream; \
    _ql_msg_stream << message; \
    throw Error(__FILE__,__LINE__, \
                          BOOST_CURRENT_FUNCTION,_ql_msg_stream.str()); \
 } else 


/*! \def QL_ENSURE
\brief throw an error if the given post-condition is not verified
*/
#define QL_ENSURE(condition,message) \
if (!(condition)) { \
    std::ostringstream _ql_msg_stream; \
    _ql_msg_stream << message; \
    throw Error(__FILE__,__LINE__, \
                          BOOST_CURRENT_FUNCTION,_ql_msg_stream.str()); \
 } else 

/*
	useful stuff above
*/


real_2d_array readVarianceMatrix(const std::string& path);


double min(const real_1d_array& array, int n = 0);


double max(const real_1d_array& array, int n = 0);


double sum(const real_1d_array& array, int n = 0);


template <class Impl>
class CuriouslyRecurringTemplate {
protected:
	// not meant to be instantiated as such
	CuriouslyRecurringTemplate() {}
	~CuriouslyRecurringTemplate() {}
	// support for the curiously recurring template pattern
	Impl& impl() {
		return static_cast<Impl&>(*this);
	}
	const Impl& impl() const {
		return static_cast<const Impl&>(*this);
	}
};


template <class Impl>
class Solver1D : public CuriouslyRecurringTemplate<Impl> {
public:
	Solver1D()
		: maxEvaluations_(MAX_FUNCTION_EVALUATIONS),
		lowerBoundEnforced_(false), upperBoundEnforced_(false) {}
	//! \name Modifiers
	//@{
	/*! This method returns the zero of the function \f$ f \f$,
	determined with the given accuracy \f$ \epsilon \f$;
	depending on the particular solver, this might mean that
	the returned \f$ x \f$ is such that \f$ |f(x)| < \epsilon
	\f$, or that \f$ |x-\xi| < \epsilon \f$ where \f$ \xi \f$
	is the real zero.

	This method contains a bracketing routine to which an
	initial guess must be supplied as well as a step used to
	scan the range of the possible bracketing values.
	*/
	template <class F>
	Real solve(const F& f,
		Real accuracy,
		Real guess,
		Real step) const {

		QL_REQUIRE(accuracy>0.0,
			"accuracy (" << accuracy << ") must be positive");
		// check whether we really want to use epsilon
		accuracy = std::max(accuracy, QL_EPSILON);

		const Real growthFactor = 1.6;
		Integer flipflop = -1;

		root_ = guess;
		fxMax_ = f(root_);

		// monotonically crescent bias, as in optionValue(volatility)
		if (close(fxMax_, 0.0))
			return root_;
		else if (fxMax_ > 0.0) {
			xMin_ = enforceBounds_(root_ - step);
			fxMin_ = f(xMin_);
			xMax_ = root_;
		}
		else {
			xMin_ = root_;
			fxMin_ = fxMax_;
			xMax_ = enforceBounds_(root_ + step);
			fxMax_ = f(xMax_);
		}

		evaluationNumber_ = 2;
		while (evaluationNumber_ <= maxEvaluations_) {
			if (fxMin_*fxMax_ <= 0.0) {
				if (close(fxMin_, 0.0))
					return xMin_;
				if (close(fxMax_, 0.0))
					return xMax_;
				root_ = (xMax_ + xMin_) / 2.0;
				return this->impl().solveImpl(f, accuracy);
			}
			if (std::fabs(fxMin_) < std::fabs(fxMax_)) {
				xMin_ = enforceBounds_(xMin_ + growthFactor*(xMin_ - xMax_));
				fxMin_ = f(xMin_);
			}
			else if (std::fabs(fxMin_) > std::fabs(fxMax_)) {
				xMax_ = enforceBounds_(xMax_ + growthFactor*(xMax_ - xMin_));
				fxMax_ = f(xMax_);
			}
			else if (flipflop == -1) {
				xMin_ = enforceBounds_(xMin_ + growthFactor*(xMin_ - xMax_));
				fxMin_ = f(xMin_);
				evaluationNumber_++;
				flipflop = 1;
			}
			else if (flipflop == 1) {
				xMax_ = enforceBounds_(xMax_ + growthFactor*(xMax_ - xMin_));
				fxMax_ = f(xMax_);
				flipflop = -1;
			}
			evaluationNumber_++;
		}

		QL_FAIL("unable to bracket root in " << maxEvaluations_
			<< " function evaluations (last bracket attempt: "
			<< "f[" << xMin_ << "," << xMax_ << "] "
			<< "-> [" << fxMin_ << "," << fxMax_ << "])");
	}
	/*! This method returns the zero of the function \f$ f \f$,
	determined with the given accuracy \f$ \epsilon \f$;
	depending on the particular solver, this might mean that
	the returned \f$ x \f$ is such that \f$ |f(x)| < \epsilon
	\f$, or that \f$ |x-\xi| < \epsilon \f$ where \f$ \xi \f$
	is the real zero.

	An initial guess must be supplied, as well as two values
	\f$ x_\mathrm{min} \f$ and \f$ x_\mathrm{max} \f$ which
	must bracket the zero (i.e., either \f$ f(x_\mathrm{min})
	\leq 0 \leq f(x_\mathrm{max}) \f$, or \f$
	f(x_\mathrm{max}) \leq 0 \leq f(x_\mathrm{min}) \f$ must
	be true).
	*/
	template <class F>
	Real solve(const F& f,
		Real accuracy,
		Real guess,
		Real xMin,
		Real xMax) const {

		QL_REQUIRE(accuracy>0.0,
			"accuracy (" << accuracy << ") must be positive");
		// check whether we really want to use epsilon
		accuracy = std::max(accuracy, QL_EPSILON);

		xMin_ = xMin;
		xMax_ = xMax;

		QL_REQUIRE(xMin_ < xMax_,
			"invalid range: xMin_ (" << xMin_
			<< ") >= xMax_ (" << xMax_ << ")");
		QL_REQUIRE(!lowerBoundEnforced_ || xMin_ >= lowerBound_,
			"xMin_ (" << xMin_
			<< ") < enforced low bound (" << lowerBound_ << ")");
		QL_REQUIRE(!upperBoundEnforced_ || xMax_ <= upperBound_,
			"xMax_ (" << xMax_
			<< ") > enforced hi bound (" << upperBound_ << ")");

		fxMin_ = f(xMin_);
		if (close(fxMin_, 0.0))
			return xMin_;

		fxMax_ = f(xMax_);
		if (close(fxMax_, 0.0))
			return xMax_;

		evaluationNumber_ = 2;

		QL_REQUIRE(fxMin_*fxMax_ < 0.0,
			"root not bracketed: f["
			<< xMin_ << "," << xMax_ << "] -> ["
			<< std::scientific
			<< fxMin_ << "," << fxMax_ << "]");

		QL_REQUIRE(guess > xMin_,
			"guess (" << guess << ") < xMin_ (" << xMin_ << ")");
		QL_REQUIRE(guess < xMax_,
			"guess (" << guess << ") > xMax_ (" << xMax_ << ")");

		root_ = guess;

		return this->impl().solveImpl(f, accuracy);
	}

	/*! This method sets the maximum number of function
	evaluations for the bracketing routine. An error is thrown
	if a bracket is not found after this number of
	evaluations.
	*/
	void setMaxEvaluations(Size evaluations);
	//! sets the lower bound for the function domain
	void setLowerBound(Real lowerBound);
	//! sets the upper bound for the function domain
	void setUpperBound(Real upperBound);
	//@}
protected:
	mutable Real root_, xMin_, xMax_, fxMin_, fxMax_;
	Size maxEvaluations_;
	mutable Size evaluationNumber_;
private:
	Real enforceBounds_(Real x) const;
	Real lowerBound_, upperBound_;
	bool lowerBoundEnforced_, upperBoundEnforced_;
};


// inline definitions

template <class T>
inline void Solver1D<T>::setMaxEvaluations(Size evaluations) {
	maxEvaluations_ = evaluations;
}

template <class T>
inline void Solver1D<T>::setLowerBound(Real lowerBound) {
	lowerBound_ = lowerBound;
	lowerBoundEnforced_ = true;
}

template <class T>
inline void Solver1D<T>::setUpperBound(Real upperBound) {
	upperBound_ = upperBound;
	upperBoundEnforced_ = true;
}

template <class T>
inline Real Solver1D<T>::enforceBounds_(Real x) const {
	if (lowerBoundEnforced_ && x < lowerBound_)
		return lowerBound_;
	if (upperBoundEnforced_ && x > upperBound_)
		return upperBound_;
	return x;
}


class Bisection : public Solver1D<Bisection> {
public:
	template <class F>
	Real solveImpl(const F& f,
		Real xAccuracy) const {

		/* The implementation of the algorithm was inspired by
		Press, Teukolsky, Vetterling, and Flannery,
		"Numerical Recipes in C", 2nd edition, Cambridge
		University Press
		*/

		Real dx, xMid, fMid;

		// Orient the search so that f>0 lies at root_+dx
		if (fxMin_ < 0.0) {
			dx = xMax_ - xMin_;
			root_ = xMin_;
		}
		else {
			dx = xMin_ - xMax_;
			root_ = xMax_;
		}

		while (evaluationNumber_ <= maxEvaluations_) {
			dx /= 2.0;
			xMid = root_ + dx;
			fMid = f(xMid);
			++evaluationNumber_;
			if (fMid <= 0.0)
				root_ = xMid;
			if (std::fabs(dx) < xAccuracy || (close(fMid, 0.0))) {
				f(root_);
				++evaluationNumber_;
				return root_;
			}
		}
		QL_FAIL("maximum number of function evaluations ("
			<< maxEvaluations_ << ") exceeded");
	}
};


class Brent : public Solver1D<Brent> {
public:
	template <class F>
	Real solveImpl(const F& f,
		Real xAccuracy) const {

		/* The implementation of the algorithm was inspired by
		Press, Teukolsky, Vetterling, and Flannery,
		"Numerical Recipes in C", 2nd edition, Cambridge
		University Press
		*/

		Real min1, min2;
		Real froot, p, q, r, s, xAcc1, xMid;

		// we want to start with root_ (which equals the guess) on
		// one side of the bracket and both xMin_ and xMax_ on the
		// other.
		froot = f(root_);
		++evaluationNumber_;
		if (froot * fxMin_ < 0) {
			xMax_ = xMin_;
			fxMax_ = fxMin_;
		}
		else {
			xMin_ = xMax_;
			fxMin_ = fxMax_;
		}
		Real d = root_ - xMax_;
		Real e = d;

		while (evaluationNumber_ <= maxEvaluations_) {
			if ((froot > 0.0 && fxMax_ > 0.0) ||
				(froot < 0.0 && fxMax_ < 0.0)) {

				// Rename xMin_, root_, xMax_ and adjust bounds
				xMax_ = xMin_;
				fxMax_ = fxMin_;
				e = d = root_ - xMin_;
			}
			if (std::fabs(fxMax_) < std::fabs(froot)) {
				xMin_ = root_;
				root_ = xMax_;
				xMax_ = xMin_;
				fxMin_ = froot;
				froot = fxMax_;
				fxMax_ = fxMin_;
			}
			// Convergence check
			xAcc1 = 2.0*QL_EPSILON*std::fabs(root_) + 0.5*xAccuracy;
			xMid = (xMax_ - root_) / 2.0;
			if (std::fabs(xMid) <= xAcc1 || (close(froot, 0.0))) {
				f(root_);
				++evaluationNumber_;
				return root_;
			}
			if (std::fabs(e) >= xAcc1 &&
				std::fabs(fxMin_) > std::fabs(froot)) {

				// Attempt inverse quadratic interpolation
				s = froot / fxMin_;
				if (close(xMin_, xMax_)) {
					p = 2.0*xMid*s;
					q = 1.0 - s;
				}
				else {
					q = fxMin_ / fxMax_;
					r = froot / fxMax_;
					p = s*(2.0*xMid*q*(q - r) - (root_ - xMin_)*(r - 1.0));
					q = (q - 1.0)*(r - 1.0)*(s - 1.0);
				}
				if (p > 0.0) q = -q;  // Check whether in bounds
				p = std::fabs(p);
				min1 = 3.0*xMid*q - std::fabs(xAcc1*q);
				min2 = std::fabs(e*q);
				if (2.0*p < (min1 < min2 ? min1 : min2)) {
					e = d;                // Accept interpolation
					d = p / q;
				}
				else {
					d = xMid;  // Interpolation failed, use bisection
					e = d;
				}
			}
			else {
				// Bounds decreasing too slowly, use bisection
				d = xMid;
				e = d;
			}
			xMin_ = root_;
			fxMin_ = froot;
			if (std::fabs(d) > xAcc1)
				root_ += d;
			else
				root_ += sign(xAcc1, xMid);
			froot = f(root_);
			++evaluationNumber_;
		}
		QL_FAIL("maximum number of function evaluations ("
			<< maxEvaluations_ << ") exceeded");
	}
private:
	Real sign(Real a, Real b) const {
		return b >= 0.0 ? std::fabs(a) : -std::fabs(a);
	}
};


#endif //ALGLIB_UTILITIES_HPP

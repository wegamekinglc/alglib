/* config.h.  Generated from config.h.in by configure.  */
/* config.h.in.  Generated from configure.ac by autoheader.  */

/* C++ compiler */
#define CXX "g++"

/* Flags passed to C++ compiler */
#define CXXFLAGS "-g -O2"

/* Define to 1 if you have the <adolc/adolc.h> header file. */
/* #undef HAVE_ADOLC_ADOLC_H */

/* Define to 1 if you have the <cppad/cppad.hpp> header file. */
/* #undef HAVE_CPPAD_CPPAD_HPP */

/* Define to 1 if you have the <dlfcn.h> header file. */
#define HAVE_DLFCN_H 1

/* Define to 1 if you have the `gettimeofday' function. */
#define HAVE_GETTIMEOFDAY 1

/* Define to 1 if you have the <inttypes.h> header file. */
#define HAVE_INTTYPES_H 1

/* Define to 1 if you have the `adolc' library (-ladolc). */
/* #undef HAVE_LIBADOLC */

/* Define to 1 if you have the `gsl' library (-lgsl). */
/* #undef HAVE_LIBGSL */

/* Define to 1 if you have the `m' library (-lm). */
#define HAVE_LIBM 1

/* Define to 1 if you have the <memory.h> header file. */
#define HAVE_MEMORY_H 1

/* Define to 1 if you have the `pow' function. */
#define HAVE_POW 1

/* Is the Sacado library working? */
/* #undef HAVE_SACADO */

/* Define to 1 if you have the `sqrt' function. */
#define HAVE_SQRT 1

/* Define to 1 if you have the <stdint.h> header file. */
#define HAVE_STDINT_H 1

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define to 1 if you have the <strings.h> header file. */
#define HAVE_STRINGS_H 1

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H 1

/* Define to 1 if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H 1

/* Define to 1 if you have the <sys/time.h> header file. */
#define HAVE_SYS_TIME_H 1

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 1

/* Define to 1 if you have the <unistd.h> header file. */
#define HAVE_UNISTD_H 1

/* Define to the sub-directory where libtool stores uninstalled libraries. */
#define LT_OBJDIR ".libs/"

/* If CppAD is being used by the benchmarking program then it is much faster
   with debugging disabled */
/* #undef NDEBUG */

/* Name of package */
#define PACKAGE "adept"

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT "r.j.hogan@reading.ac.uk"

/* Define to the full name of this package. */
#define PACKAGE_NAME "adept"

/* Define to the full name and version of this package. */
#define PACKAGE_STRING "adept 1.1"

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME "adept"

/* Define to the home page for this package. */
#define PACKAGE_URL ""

/* Define to the version of this package. */
#define PACKAGE_VERSION "1.1"

/* Define to 1 if you have the ANSI C header files. */
#define STDC_HEADERS 1

/* Version number of package */
#define VERSION "1.1"

/* Define to `unsigned int' if <sys/types.h> does not define. */
/* #undef size_t */

/* Use ADOLC only if both the library and the header files are available */
#if defined( HAVE_LIBADOLC ) && defined( HAVE_ADOLC_ADOLC_H )
#define HAVE_ADOLC 1
#endif

/* Use CPPAD if the header files are available */
#if defined( HAVE_CPPAD_CPPAD_HPP )
#define HAVE_CPPAD 1
#endif

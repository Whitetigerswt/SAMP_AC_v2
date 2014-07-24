//  boost/system/config.hpp  -----------------------------------------------------------//

//  Copyright Beman Dawes 2003, 2006

//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/system for documentation.

#ifndef BOOST_SYSTEM_CONFIG_HPP                  
#define BOOST_SYSTEM_CONFIG_HPP

#include <boost/config.hpp>

// These #defines added by the separate Boost.Asio package.
#if !defined(BOOST_SYMBOL_IMPORT)
# if defined(BOOST_HAS_DECLSPEC)
#  define BOOST_SYMBOL_IMPORT __declspec(dllimport)
# else // defined(BOOST_HAS_DECLSPEC)
#  define BOOST_SYMBOL_IMPORT
# endif // defined(BOOST_HAS_DECLSPEC)
#endif // !defined(BOOST_SYMBOL_IMPORT)
#if !defined(BOOST_SYMBOL_EXPORT)
# if defined(BOOST_HAS_DECLSPEC)
#  define BOOST_SYMBOL_EXPORT __declspec(dllexport)
# else // defined(BOOST_HAS_DECLSPEC)
#  define BOOST_SYMBOL_EXPORT
# endif // defined(BOOST_HAS_DECLSPEC)
#endif // !defined(BOOST_SYMBOL_EXPORT)
#if !defined(BOOST_SYMBOL_VISIBLE)
# define BOOST_SYMBOL_VISIBLE
#endif // !defined(BOOST_SYMBOL_VISIBLE)

#include <boost/system/api_config.hpp>  // for BOOST_POSIX_API or BOOST_WINDOWS_API

// This header implements separate compilation features as described in
// http://www.boost.org/more/separate_compilation.html

//  normalize macros  ------------------------------------------------------------------//

#if !defined(BOOST_SYSTEM_DYN_LINK) && !defined(BOOST_SYSTEM_STATIC_LINK) \
  && !defined(BOOST_ALL_DYN_LINK) && !defined(BOOST_ALL_STATIC_LINK)
# define BOOST_SYSTEM_STATIC_LINK
#endif

#if defined(BOOST_ALL_DYN_LINK) && !defined(BOOST_SYSTEM_DYN_LINK)
# define BOOST_SYSTEM_DYN_LINK 
#elif defined(BOOST_ALL_STATIC_LINK) && !defined(BOOST_SYSTEM_STATIC_LINK)
# define BOOST_SYSTEM_STATIC_LINK 
#endif

#if defined(BOOST_SYSTEM_DYN_LINK) && defined(BOOST_SYSTEM_STATIC_LINK)
# error Must not define both BOOST_SYSTEM_DYN_LINK and BOOST_SYSTEM_STATIC_LINK
#endif

//  enable dynamic or static linking as requested --------------------------------------//

#if defined(BOOST_ALL_DYN_LINK) || defined(BOOST_SYSTEM_DYN_LINK)
# if defined(BOOST_SYSTEM_SOURCE)
#   define BOOST_SYSTEM_DECL BOOST_SYMBOL_EXPORT
# else 
#   define BOOST_SYSTEM_DECL BOOST_SYMBOL_IMPORT
# endif
#else
# define BOOST_SYSTEM_DECL
#endif

//  enable automatic library variant selection  ----------------------------------------// 

#if !defined(BOOST_SYSTEM_SOURCE) && !defined(BOOST_ALL_NO_LIB) && !defined(BOOST_SYSTEM_NO_LIB)
//
// Set the name of our library, this will get undef'ed by auto_link.hpp
// once it's done with it:
//
#define BOOST_LIB_NAME boost_system
//
// If we're importing code from a dll, then tell auto_link.hpp about it:
//
#if defined(BOOST_ALL_DYN_LINK) || defined(BOOST_SYSTEM_DYN_LINK)
#  define BOOST_DYN_LINK
#endif
//
// And include the header that does the work:
//
#include <boost/config/auto_link.hpp>
#endif  // auto-linking disabled

#endif // BOOST_SYSTEM_CONFIG_HPP


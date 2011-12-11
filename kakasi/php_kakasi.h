/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2011 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:                                                              |
  +----------------------------------------------------------------------+
*/

/* $Id: header 310447 2011-04-23 21:14:10Z bjori $ */

#ifndef PHP_KAKASI_H
#define PHP_KAKASI_H

extern zend_module_entry kakasi_module_entry;
#define phpext_kakasi_ptr &kakasi_module_entry

#ifdef PHP_WIN32
#	define PHP_KAKASI_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_KAKASI_API __attribute__ ((visibility("default")))
#else
#	define PHP_KAKASI_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

PHP_MINIT_FUNCTION(kakasi);
PHP_MSHUTDOWN_FUNCTION(kakasi);
PHP_RINIT_FUNCTION(kakasi);
PHP_RSHUTDOWN_FUNCTION(kakasi);
PHP_MINFO_FUNCTION(kakasi);

PHP_FUNCTION(confirm_kakasi_compiled);	/* For testing, remove later. */

PHP_FUNCTION(kakasi);
PHP_METHOD(kakasi, reproc);
PHP_METHOD(kakasi, morpheme);
PHP_FUNCTION(KAKASI_MORPHEME);
PHP_FUNCTION(KAKASI_CONVERT);
//PHP_METHOD(kakasi, hira);
//PHP_METHOD(kakasi, alph);
//PHP_METHOD(kakasi, kata);
static zend_class_entry *kakasi_ce;

//PHP_FUNCTION(kakasi);

/* 
  	Declare any global variables you may need between the BEGIN
	and END macros here:     

ZEND_BEGIN_MODULE_GLOBALS(kakasi)
	long  global_value;
	char *global_string;
ZEND_END_MODULE_GLOBALS(kakasi)
*/

/* In every utility function you add that needs to use variables 
   in php_kakasi_globals, call TSRMLS_FETCH(); after declaring other 
   variables used by that function, or better yet, pass in TSRMLS_CC
   after the last function argument and declare your utility function
   with TSRMLS_DC after the last declared argument.  Always refer to
   the globals in your function as KAKASI_G(variable).  You are 
   encouraged to rename these macros something shorter, see
   examples in any other php module directory.
*/

#ifdef ZTS
#define KAKASI_G(v) TSRMG(kakasi_globals_id, zend_kakasi_globals *, v)
#else
#define KAKASI_G(v) (kakasi_globals.v)
#endif

#endif	/* PHP_KAKASI_H */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */

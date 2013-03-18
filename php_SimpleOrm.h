/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2013 The PHP Group                                |
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

/* $Id$ */

#ifndef PHP_SIMPLEORM_H
#define PHP_SIMPLEORM_H

extern zend_module_entry SimpleOrm_module_entry;
#define phpext_SimpleOrm_ptr &SimpleOrm_module_entry

#ifdef PHP_WIN32
#	define PHP_SIMPLEORM_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_SIMPLEORM_API __attribute__ ((visibility("default")))
#else
#	define PHP_SIMPLEORM_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

PHP_MINIT_FUNCTION(SimpleOrm);
PHP_MSHUTDOWN_FUNCTION(SimpleOrm);
PHP_RINIT_FUNCTION(SimpleOrm);
PHP_RSHUTDOWN_FUNCTION(SimpleOrm);
PHP_MINFO_FUNCTION(SimpleOrm);

PHP_METHOD(SimpleOrm, __construct);
PHP_METHOD(SimpleOrm, getInstance);
PHP_METHOD(SimpleOrm, query);
PHP_METHOD(SimpleOrm, exec);
PHP_METHOD(SimpleOrm, select);
PHP_METHOD(SimpleOrm, find);
PHP_METHOD(SimpleOrm, field);
PHP_METHOD(SimpleOrm, where);
PHP_METHOD(SimpleOrm, order);
PHP_METHOD(SimpleOrm, limit);
PHP_METHOD(SimpleOrm, insert);
PHP_METHOD(SimpleOrm, insertBatch);
PHP_METHOD(SimpleOrm, update);
PHP_METHOD(SimpleOrm, delete);
PHP_METHOD(SimpleOrm, top);
PHP_METHOD(SimpleOrm, end);

PHP_METHOD(SimpleOrm, begin);
PHP_METHOD(SimpleOrm, commit);
PHP_METHOD(SimpleOrm, rollBack);
PHP_METHOD(SimpleOrm, setAttribute);

PHP_SIMPLEORM_API zval * pdo_query(char * query TSRMLS_DC);
PHP_SIMPLEORM_API zval * pdo_exec(char * query TSRMLS_DC);
PHP_SIMPLEORM_API zval * pdo_errorCode();
PHP_SIMPLEORM_API zval * pdo_errorInfo();

PHP_SIMPLEORM_API zval * get_array_keys(zval * array TSRMLS_DC);
PHP_SIMPLEORM_API zval * join(char *delim, zval *arr, int type TSRMLS_DC);

#define CALL_PDO_METHOD(method_name) 	\
	zval *pdo, *success, *instance;		\
	instance = zend_read_static_property(SimpleOrm_ce, ZEND_STRL("instance"), 1 TSRMLS_CC);			\
	pdo = zend_read_property(SimpleOrm_ce, instance, ZEND_STRL("pdo"), 0 TSRMLS_CC);				\
	zend_call_method(&pdo, NULL, NULL, ZEND_STRL(method_name), &success, NULL TSRMLS_DC);	\
																									\
	if (!success || EG(exception)) {                                           \
		zval_ptr_dtor(&success);                                              	   \
		php_error_docref(NULL TSRMLS_CC, E_ERROR, "PDO method method_name fiald!");	\
	}                                                                          \
	convert_to_boolean(success); 											   \
	if (!success){                                                             \
		RETURN_FALSE;                                                          \
	}                                                                          \
	if (Z_BVAL_P(success) == 1) {                                              \
		zval_ptr_dtor(&success);                                               \
		RETURN_TRUE;                                                           \
	}else{                                                                     \
		zval_ptr_dtor(&success);                                               \
		RETURN_FALSE;                                                          \
	}																		   \

	
#define THIS(method_name, res, ...)	\
	zval *this=NULL;				\
	this=getThis();					\
	zend_call_method(&this, Z_OBJCE_P(this), NULL, ZEND_STRL(method_name), res, ##__VA_ARGS__, NULL TSRMLS_CC); \
	
/* 
  	Declare any global variables you may need between the BEGIN
	and END macros here:     

ZEND_BEGIN_MODULE_GLOBALS(SimpleOrm)
	long  global_value;
	char *global_string;
ZEND_END_MODULE_GLOBALS(SimpleOrm)
*/

/* In every utility function you add that needs to use variables 
   in php_SimpleOrm_globals, call TSRMLS_FETCH(); after declaring other 
   variables used by that function, or better yet, pass in TSRMLS_CC
   after the last function argument and declare your utility function
   with TSRMLS_DC after the last declared argument.  Always refer to
   the globals in your function as SIMPLEORM_G(variable).  You are 
   encouraged to rename these macros something shorter, see
   examples in any other php module directory.
*/

#ifdef ZTS
#define SIMPLEORM_G(v) TSRMG(SimpleOrm_globals_id, zend_SimpleOrm_globals *, v)
#else
#define SIMPLEORM_G(v) (SimpleOrm_globals.v)
#endif

#endif	/* PHP_SIMPLEORM_H */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */

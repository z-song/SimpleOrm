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
  | Author:    merlinal                                                  |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "ext/standard/php_smart_str.h"
#include "php_SimpleOrm.h"

/* If you declare any globals in php_SimpleOrm.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(SimpleOrm)
*/

/* True global resources - no need for thread safety here */
static int le_SimpleOrm;

ZEND_BEGIN_ARG_INFO_EX(arginfo_no_parameters, 0, 0, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_construct, 0, 0, 1)
	ZEND_ARG_INFO(0, pdo)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_getInstance, 0, 0, 1)
	ZEND_ARG_INFO(0, pdo)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_query, 0, 0, 1)
	ZEND_ARG_INFO(0, query)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_exec, 0, 0, 1)
	ZEND_ARG_INFO(0, query)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_find, 0, 0, 1)
	ZEND_ARG_INFO(0, query)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_select, 0, 0, 1)
	ZEND_ARG_INFO(0, table)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_string, 0, 0, 1)
	ZEND_ARG_INFO(0, string)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_int, 0, 0, 1)
	ZEND_ARG_INFO(0, _int)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_insert, 0, 0, 1)
	ZEND_ARG_INFO(0, string)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_update, 0, 0, 3)
	ZEND_ARG_INFO(0, table)
	ZEND_ARG_INFO(0, data)
	ZEND_ARG_INFO(0, condition)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_delete, 0, 0, 2)
	ZEND_ARG_INFO(0, table)
	ZEND_ARG_INFO(0, condition)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_no_setAttribute, 0, 0, 2)
	ZEND_ARG_INFO(0, attribute)
	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_explain, 0, 0, 1)
	ZEND_ARG_INFO(0, explain)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_page, 0, 0, 2)
	ZEND_ARG_INFO(0, page)
	ZEND_ARG_INFO(0, pagenum)
ZEND_END_ARG_INFO()


/* {{{ SimpleOrm_functions[]
 *
 * Every user visible function must have an entry in SimpleOrm_functions[].
 */
const zend_function_entry SimpleOrm_methods[] = {
	PHP_ME(SimpleOrm, __construct, 	arginfo_construct, 		ZEND_ACC_PRIVATE|ZEND_ACC_CTOR)
	PHP_ME(SimpleOrm, getInstance, 	arginfo_getInstance, 	ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(SimpleOrm, query,	 	arginfo_query, 			ZEND_ACC_PUBLIC)
	PHP_ME(SimpleOrm, exec,	 		arginfo_exec, 			ZEND_ACC_PUBLIC)
	PHP_ME(SimpleOrm, select,	 	arginfo_select, 		ZEND_ACC_PUBLIC)
	PHP_ME(SimpleOrm, find,	 		arginfo_find, 			ZEND_ACC_PUBLIC)
	PHP_ME(SimpleOrm, field,	 	arginfo_string, 		ZEND_ACC_PUBLIC)
	PHP_ME(SimpleOrm, where,	 	arginfo_string, 		ZEND_ACC_PUBLIC)
	PHP_ME(SimpleOrm, order,	 	arginfo_string, 		ZEND_ACC_PUBLIC)
	PHP_ME(SimpleOrm, limit,	 	arginfo_string, 		ZEND_ACC_PUBLIC)
	PHP_ME(SimpleOrm, top,	 		arginfo_int, 			ZEND_ACC_PUBLIC)
	PHP_ME(SimpleOrm, end,	 		arginfo_int, 			ZEND_ACC_PUBLIC)

	PHP_ME(SimpleOrm, insert,	 	arginfo_insert, 		ZEND_ACC_PUBLIC)
	PHP_ME(SimpleOrm, insertBatch,	arginfo_insert, 		ZEND_ACC_PUBLIC)
	PHP_ME(SimpleOrm, update,		arginfo_update, 		ZEND_ACC_PUBLIC)
	
	PHP_ME(SimpleOrm, tableInfo,	arginfo_no_parameters, 	ZEND_ACC_PUBLIC)
	PHP_ME(SimpleOrm, explain,		arginfo_explain, 		ZEND_ACC_PUBLIC)
	PHP_ME(SimpleOrm, total,		arginfo_no_parameters, 	ZEND_ACC_PUBLIC)
	PHP_ME(SimpleOrm, page,			arginfo_page, 			ZEND_ACC_PUBLIC)
	
	PHP_ME(SimpleOrm, begin, 		arginfo_no_parameters, 	ZEND_ACC_PUBLIC)
	PHP_ME(SimpleOrm, commit,  		arginfo_no_parameters , ZEND_ACC_PUBLIC)
	PHP_ME(SimpleOrm, rollBack, 	arginfo_no_parameters , ZEND_ACC_PUBLIC)
	PHP_ME(SimpleOrm, setAttribute, arginfo_no_setAttribute,ZEND_ACC_PUBLIC)
	{NULL, NULL, NULL}
};
/* }}} */

/* {{{ SimpleOrm_module_entry
 */
zend_module_entry SimpleOrm_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"SimpleOrm",
	NULL,
	PHP_MINIT(SimpleOrm),
	PHP_MSHUTDOWN(SimpleOrm),
	PHP_RINIT(SimpleOrm),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(SimpleOrm),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(SimpleOrm),
#if ZEND_MODULE_API_NO >= 20010901
	"0.1", /* Replace with version number for your extension */
#endif
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_SIMPLEORM
ZEND_GET_MODULE(SimpleOrm)
#endif

zend_class_entry *SimpleOrm_ce;

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(SimpleOrm)
{
	zend_class_entry ce;
	
	INIT_CLASS_ENTRY(ce, "SimpleOrm", SimpleOrm_methods);
	SimpleOrm_ce = zend_register_internal_class_ex(&ce, NULL, NULL TSRMLS_CC);
	
	zend_declare_property_null(SimpleOrm_ce, ZEND_STRL("pdo"), ZEND_ACC_PUBLIC TSRMLS_CC);
	zend_declare_property_null(SimpleOrm_ce, ZEND_STRL("stmt"), ZEND_ACC_PUBLIC TSRMLS_CC);
	zend_declare_property_null(SimpleOrm_ce, ZEND_STRL("instance"), ZEND_ACC_PUBLIC|ZEND_ACC_STATIC TSRMLS_CC);
	
	zend_declare_property_stringl(SimpleOrm_ce, ZEND_STRL("primary_key"), ZEND_STRL("id"), ZEND_ACC_PUBLIC TSRMLS_DC);
	zend_declare_property_stringl(SimpleOrm_ce, ZEND_STRL("field"), ZEND_STRL("*"), ZEND_ACC_PUBLIC TSRMLS_DC);
	zend_declare_property_stringl(SimpleOrm_ce, ZEND_STRL("action"), ZEND_STRL("SELECT"), ZEND_ACC_PUBLIC TSRMLS_DC);
	zend_declare_property_null(SimpleOrm_ce, ZEND_STRL("table"), ZEND_ACC_PUBLIC TSRMLS_DC);
	zend_declare_property_null(SimpleOrm_ce, ZEND_STRL("where"), ZEND_ACC_PUBLIC TSRMLS_DC);
	zend_declare_property_null(SimpleOrm_ce, ZEND_STRL("order"), ZEND_ACC_PUBLIC TSRMLS_DC);
	zend_declare_property_null(SimpleOrm_ce, ZEND_STRL("limit"), ZEND_ACC_PUBLIC TSRMLS_DC);
	
	zend_declare_property_null(SimpleOrm_ce, ZEND_STRL("sql"), ZEND_ACC_PUBLIC TSRMLS_DC);
	
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(SimpleOrm)
{
	/* uncomment this line if you have INI entries
	UNREGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(SimpleOrm)
{
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(SimpleOrm)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(SimpleOrm){
	php_info_print_table_start();
	php_info_print_table_header(2, "SimpleOrm support", "enabled");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */

/* {{{ proto private SimpleOrm::__construct(PDO pdo)
*/
PHP_METHOD(SimpleOrm, __construct){
	zval *pdo;
	zend_class_entry **obj;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &pdo) == FAILURE) {
		return;
	}
	if(zend_lookup_class(ZEND_STRL("PDO"), &obj TSRMLS_CC)!=SUCCESS){
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "this extension is based on PDO!");
	}
	if(Z_TYPE_P(pdo) == IS_OBJECT && instanceof_function(Z_OBJCE_P(pdo), *obj TSRMLS_CC)) {
		zend_update_property(SimpleOrm_ce, getThis(), ZEND_STRL("pdo"), pdo TSRMLS_CC);
	}else{
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "input parameter must be a pdo object");
	}
	return;
}
/* }}} */

/* {{{ proto public SimpleOrm::getInstance(PDO pdo)
*/
PHP_METHOD(SimpleOrm, getInstance){
	zval *pdo, *instance;
	zend_class_entry **obj;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &pdo) == FAILURE) {
		return;
	}

	instance = zend_read_static_property(SimpleOrm_ce, ZEND_STRL("instance"), 0 TSRMLS_CC);
	if(Z_TYPE_P(instance) != IS_OBJECT) {
		zend_function *constructor;
		zval ** retval_ptr_ptr;
		if(zend_lookup_class(ZEND_STRL("SimpleOrm"), &obj TSRMLS_CC)==SUCCESS){
			MAKE_STD_ZVAL(instance);
			object_init_ex(instance, *obj);
			constructor = Z_OBJ_HT_P(instance)->get_constructor(instance TSRMLS_CC);
			if (constructor != NULL){	
				zend_call_method(&instance, *obj, &constructor, ZEND_STRL("__construct"), retval_ptr_ptr, 1, pdo, NULL TSRMLS_DC);
			}
			zend_update_static_property(Z_OBJCE_P(instance), ZEND_STRL("instance"), instance TSRMLS_CC);
			RETURN_ZVAL(instance, 1, 0);
		}
	}else{
		RETURN_ZVAL(instance, 1, 0);
	}
	return;
}
/* }}} */

/* {{{ proto public SimpleOrm::query(string query)
*/
PHP_METHOD(SimpleOrm, query){
	char *query;
	zval *stmt;
	int query_len;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &query, &query_len) == FAILURE) {
		return;
	}
	stmt = pdo_query(query);
	RETURN_ZVAL(stmt, 1, 0);
}
/* }}} */

/* {{{ proto public SimpleOrm::exec(string query)
*/
PHP_METHOD(SimpleOrm, exec){
	char *query;
	int query_len;
	zval *rows;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &query, &query_len) == FAILURE) {
		return;
	}
	rows = pdo_exec(query);
	RETURN_ZVAL(rows, 1, 0);
}
/* }}} */

/* {{{ proto public SimpleOrm::select([string table])
*/
PHP_METHOD(SimpleOrm, select){
	char *table;
	int table_len;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|s", &table, &table_len) == FAILURE) {
		return;
	}
	
	zval *self=getThis();
	zend_update_property_string(Z_OBJCE_P(self), self, ZEND_STRL("action"), "SELECT" TSRMLS_DC);
	if (ZEND_NUM_ARGS() == 1) {
		zend_update_property_string(Z_OBJCE_P(self), self, ZEND_STRL("table"), table TSRMLS_DC);
	}
	RETURN_ZVAL(self, 1, 0);
}
/* }}} */

/* {{{ proto public SimpleOrm::select(string fields)
*/
PHP_METHOD(SimpleOrm, field){
	zval *self;
	char *field;
	int field_len;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &field, &field_len) == FAILURE) {
		return;
	}
	self=getThis();
	if(ZEND_NUM_ARGS()==1){
		zend_update_property_string(Z_OBJCE_P(self), self, ZEND_STRL("field"), field TSRMLS_DC);
	}
	
	RETURN_ZVAL(self, 1, 0);
}
/* }}} */

/* {{{ proto public SimpleOrm::where([string where])
*/
PHP_METHOD(SimpleOrm, where){
	zval *self;
	char *where, *sql;
	int where_len;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|s", &where, &where_len) == FAILURE) {
		return;
	}

	self=getThis();
	if(ZEND_NUM_ARGS()==1){
		spprintf(&sql, 0, "WHERE %s", where);
		zend_update_property_string(Z_OBJCE_P(self), self, ZEND_STRL("where"), sql TSRMLS_DC);
	}
	
	RETURN_ZVAL(self, 1, 0);
}
/* }}} */

/* {{{ proto public SimpleOrm::order(string order)
*/
PHP_METHOD(SimpleOrm, order){
	zval *self;
	char *order, *sql;
	int order_len;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &order, &order_len) == FAILURE) {
		return;
	}
	
	spprintf(&sql, 0, "ORDER BY %s", order);
	self=getThis();
	zend_update_property_string(Z_OBJCE_P(self), self, ZEND_STRL("order"), sql TSRMLS_DC);
	
	RETURN_ZVAL(self, 1, 0);
}
/* }}} */

/* {{{ proto public SimpleOrm::limit(int offset[, int length])
*/
PHP_METHOD(SimpleOrm, limit){
	long offset, length;
	int str_len;
	char *sql;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l|l", &offset, &length) == FAILURE) {
		return;
	}
	
	if (ZEND_NUM_ARGS()==1){
		spprintf(&sql, 0, "LIMIT %d", offset);
	}else if(ZEND_NUM_ARGS()==2){
		spprintf(&sql, 0, "LIMIT %d,%d", offset, length);
	}

	zend_update_property_string(Z_OBJCE_P(getThis()), getThis(), ZEND_STRL("limit"), sql TSRMLS_DC);
	
	RETURN_ZVAL(getThis(), 1, 0);
}
/* }}} */

/* {{{ proto public SimpleOrm::find([string query])
*/
PHP_METHOD(SimpleOrm, find){
	zval *stmt, *query, *action, *field, *table, *primary_key, *self=NULL, *where, *order, *limit;
	char *sql;
	int query_len, rows;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|z", &query) == FAILURE) {
		return;
	}
	
	self=getThis();
	GET_THIS_PROPERTY(self, "action", action);
	GET_THIS_PROPERTY(self, "field", field);
	GET_THIS_PROPERTY(self, "primary_key", primary_key);
	GET_THIS_PROPERTY(self, "table", table);
	GET_THIS_PROPERTY(self, "where", where);
	GET_THIS_PROPERTY(self, "order", order);
	GET_THIS_PROPERTY(self, "limit", limit);
	
	Z_TYPE_P(where)==IS_NULL && (Z_STRVAL_P(where) = "");
	Z_TYPE_P(order)==IS_NULL && (Z_STRVAL_P(order) = "");
	Z_TYPE_P(limit)==IS_NULL && (Z_STRVAL_P(limit) = "");
	
	if(ZEND_NUM_ARGS()==1){
		if(Z_TYPE_P(query)==IS_LONG){
			spprintf(&sql, 0, "%s %s FROM `%s` WHERE %s=%d;",Z_STRVAL_P(action),Z_STRVAL_P(field), Z_STRVAL_P(table), Z_STRVAL_P(primary_key), Z_LVAL_P(query));
		}else if(Z_TYPE_P(query)==IS_ARRAY){
			zval *Pkeys;
			Pkeys=join(",", query, 0);
			spprintf(&sql, 0, "%s %s FROM `%s` WHERE `%s` IN (%s);",Z_STRVAL_P(action), Z_STRVAL_P(field), Z_STRVAL_P(table), Z_STRVAL_P(primary_key), Z_STRVAL_P(Pkeys));
		}
	}else{
		spprintf(&sql, 0, "%s %s FROM `%s` %s %s %s;",Z_STRVAL_P(action), Z_STRVAL_P(field), Z_STRVAL_P(table), Z_STRVAL_P(where), Z_STRVAL_P(order), Z_STRVAL_P(limit));
	}
	
	stmt = pdo_query(sql);
	RETURN_ZVAL(stmt, 0, 0);
}
/* }}} */

/* {{{ proto public SimpleOrm::tatal()
*/
PHP_METHOD(SimpleOrm, total){
	zval *stmt, *table, *self=NULL, *where, *order, *limit, *res, **count;
	char *sql;

	self=getThis();
	GET_THIS_PROPERTY(self, "table", table);
	GET_THIS_PROPERTY(self, "where", where);
	GET_THIS_PROPERTY(self, "order", order);
	GET_THIS_PROPERTY(self, "limit", limit);

	
	Z_TYPE_P(where)==IS_NULL && (Z_STRVAL_P(where) = "");
	Z_TYPE_P(order)==IS_NULL && (Z_STRVAL_P(order) = "");
	Z_TYPE_P(limit)==IS_NULL && (Z_STRVAL_P(limit) = "");
		
	spprintf(&sql, 0, "SELECT COUNT(1) AS COUNT FROM `%s` %s %s %s;", Z_STRVAL_P(table), Z_STRVAL_P(where), Z_STRVAL_P(order), Z_STRVAL_P(limit));

	stmt = pdo_query(sql);
	zend_update_property(SimpleOrm_ce, self, ZEND_STRL("stmt"), stmt TSRMLS_CC);
	CALL_PDO_STMT_METHOD("fetch", res);

	zend_hash_index_find(Z_ARRVAL_P(res), 0, (void **)&count);
	convert_to_long_ex(count);
	RETURN_LONG(Z_LVAL_PP(count));
}
/* }}} */

/* {{{ proto public SimpleOrm::page(int page[, int pagenum])
*/
PHP_METHOD(SimpleOrm, page){
	zval *stmt, *table, *self=NULL, *where, *order, *action, *field, *primary_key, *total, *page_info;
	char *sql, *limit;
	ulong page, pagenum=10;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l|l", &page, &pagenum)==FAILURE){
		return;
	}
	self=getThis();
	THIS_METHOD("total", &total);
	spprintf(&limit, 0 ,"LIMIT %d,%d", (page-1)*pagenum, pagenum);
	zend_update_property_string(Z_OBJCE_P(self), self, ZEND_STRL("limit"), limit TSRMLS_DC);

	GET_THIS_PROPERTY(self, "action", action);
	GET_THIS_PROPERTY(self, "field", field);
	GET_THIS_PROPERTY(self, "primary_key", primary_key);
	GET_THIS_PROPERTY(self, "table", table);
	GET_THIS_PROPERTY(self, "where", where);
	GET_THIS_PROPERTY(self, "order", order);

	Z_TYPE_P(where)==IS_NULL && (Z_STRVAL_P(where) = "");
	Z_TYPE_P(order)==IS_NULL && (Z_STRVAL_P(order) = "");

	spprintf(&sql, 0, "%s %s FROM `%s` %s %s %s;",Z_STRVAL_P(action), Z_STRVAL_P(field), Z_STRVAL_P(table), Z_STRVAL_P(where), Z_STRVAL_P(order), limit);

	stmt = pdo_query(sql);
	zend_update_property(SimpleOrm_ce, self, ZEND_STRL("stmt"), stmt TSRMLS_CC);

	MAKE_STD_ZVAL(page_info);
	array_init(page_info);
	add_assoc_long(page_info, "total", Z_LVAL_P(total));
	add_assoc_long(page_info, "current", page);
	add_assoc_long(page_info, "total_page", ceil(Z_LVAL_P(total)/pagenum));
	zend_update_property(SimpleOrm_ce, self, ZEND_STRL("page_info"), page_info TSRMLS_CC);

	RETURN_ZVAL(stmt, 0, 0);
}
/* }}} */

/* {{{ proto public SimpleOrm::explain([string query])
*/
PHP_METHOD(SimpleOrm, explain){
	zval *stmt, *action, *field, *table, *self=NULL, *where, *order, *limit, *_parm, *res;
	char *sql, *explain;
	int explain_len;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|s", &explain, &explain_len) == FAILURE) {
		return;
	}

	self=getThis();
	if (ZEND_NUM_ARGS()>=1)
	{
		spprintf(&sql, 0, "EXPLAIN %s;", explain);
	}else{
		GET_THIS_PROPERTY(self, "action", action);
		GET_THIS_PROPERTY(self, "field", field);
		GET_THIS_PROPERTY(self, "table", table);
		GET_THIS_PROPERTY(self, "where", where);
		GET_THIS_PROPERTY(self, "order", order);
		GET_THIS_PROPERTY(self, "limit", limit);
		
		Z_TYPE_P(where)==IS_NULL && (Z_STRVAL_P(where) = "");
		Z_TYPE_P(order)==IS_NULL && (Z_STRVAL_P(order) = "");
		Z_TYPE_P(limit)==IS_NULL && (Z_STRVAL_P(limit) = "");
		
		spprintf(&sql, 0, "EXPLAIN %s %s FROM `%s` %s %s %s;", Z_STRVAL_P(action), Z_STRVAL_P(field), Z_STRVAL_P(table), Z_STRVAL_P(where), Z_STRVAL_P(order), Z_STRVAL_P(limit));
	}

	stmt = pdo_query(sql);
	zend_update_property(SimpleOrm_ce, self, ZEND_STRL("stmt"), stmt TSRMLS_CC);

	MAKE_STD_ZVAL(_parm);
	ZVAL_LONG(_parm, 2);
	CALL_PDO_STMT_METHOD("fetch", res, 1, _parm);

	RETURN_ZVAL(res, 0, 0);
}
/* }}} */

/* {{{ proto public SimpleOrm::top(int num)
*/
PHP_METHOD(SimpleOrm, top){
	zval *self, *res;
	int num;
	char *limit;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &num) == FAILURE) {
		return;
	}
	self=getThis();
	spprintf(&limit, 0 ,"LIMIT %d", num);
	zend_update_property_string(Z_OBJCE_P(self), self, ZEND_STRL("order"), "" TSRMLS_DC);
	zend_update_property_string(Z_OBJCE_P(self), self, ZEND_STRL("limit"), limit TSRMLS_DC);
	
	THIS_METHOD("find", &res);
	
	if(Z_TYPE_P(res)!=IS_BOOL){
		RETVAL_ZVAL(res, 0, 0);
	} else {
		 RETVAL_NULL();
	}
}
/* }}} */

/* {{{ proto public SimpleOrm::end(int num)
*/
PHP_METHOD(SimpleOrm, end){
	zval *self, *res, *P_key;
	int num;
	char *limit, *order;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &num) == FAILURE) {
		return;
	}
	self=getThis();

	P_key = zend_read_property(Z_OBJCE_P(self), self, ZEND_STRL("primary_key"), 0 TSRMLS_CC);
	spprintf(&order, 0 ,"ORDER BY %s DESC", Z_STRVAL_P(P_key));
	zend_update_property_string(Z_OBJCE_P(self), self, ZEND_STRL("order"), order TSRMLS_DC);
	
	spprintf(&limit, 0 ,"LIMIT %d", num);
	zend_update_property_string(Z_OBJCE_P(self), self, ZEND_STRL("limit"), limit TSRMLS_DC);
	
	THIS_METHOD("find", &res);
	
	if(Z_TYPE_P(res)!=IS_BOOL){
		RETVAL_ZVAL(res, 0, 0);
	} else {
		RETVAL_NULL();
	}
}
/* }}} */

/* {{{ proto public SimpleOrm::insert(string table_name, array data)
*/
PHP_METHOD(SimpleOrm, insert){
	zval *action, *fields, *values, *self=NULL, *rows=NULL;
	char *table;
	char *sql=NULL;
	int table_len;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sz", &table, &table_len, &values) == FAILURE) {
		return;
	}

	self=getThis();
	zend_update_property_string(Z_OBJCE_P(self), self, ZEND_STRL("action"), "INSERT" TSRMLS_DC);
	
	if(Z_TYPE_P(values)==IS_ARRAY){
		zval *fields, *value;
		
		MAKE_STD_ZVAL(rows);
		fields=join(",", get_array_keys(values), 0);
		value=join(",", values, 1);
		spprintf(&sql, 0, "INSERT INTO `%s`(%s) VALUES (%s);", table, Z_STRVAL_P(fields), Z_STRVAL_P(value));
		rows=pdo_exec(sql);
	}
	
	RETVAL_ZVAL(rows, 0, 0);
}
/* }}} */

/* {{{ proto public SimpleOrm::insertBatch(string table_name, multi_array data)
*/
PHP_METHOD(SimpleOrm, insertBatch){
	zval *values, *self=NULL, **element, *stmt, *table_name;
	char *table;
	int table_len, rows=0;
	HashTable *value_hash;
	HashPosition pos;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sz", &table, &table_len, &values) == FAILURE) {
		return;
	}
	
	MAKE_STD_ZVAL(table_name);
	ZVAL_STRING(table_name, table, 0);
	
	self=getThis();
	value_hash=Z_ARRVAL_P(values);
	for (zend_hash_internal_pointer_reset_ex(value_hash, &pos);
		zend_hash_get_current_data_ex(value_hash, (void **)&element, &pos) == SUCCESS;
		zend_hash_move_forward_ex(value_hash, &pos)
	) {
		THIS_METHOD("insert", &stmt, 2, table_name, *element);
		if(Z_TYPE_P(stmt)!=IS_BOOL)
			rows++;
	}
	RETVAL_LONG(rows);
}
/* }}} */

/* {{{ proto public SimpleOrm::update(string table_name, array data[, string where])
*/
PHP_METHOD(SimpleOrm, update){
	zval *data, *where, *instance, **element, *stmt, *set;
	char *table, *condition, *sql;
	int table_len, condition_len;
	char *buff;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sz|s", &table, &table_len, &data, &condition, &condition_len) == FAILURE) {
		return;
	}
	
	if(ZEND_NUM_ARGS()>=3){
		ssprintf(buff, 0, "WHERE %s", condition);
		MAKE_STD_ZVAL(where);
		ZVAL_STRING(where, buff, 0);
	}else{
		instance = zend_read_static_property(SimpleOrm_ce, ZEND_STRL("instance"), 1 TSRMLS_CC);
		where = zend_read_property(SimpleOrm_ce, instance, ZEND_STRL("where"), 0 TSRMLS_CC);
	}
	buff=NULL;
	
	if(Z_TYPE_P(where)!=IS_NULL){
		zval *piece=NULL;
		HashTable *data_hash;
		char *key;
		int index;
		
		data_hash=Z_ARRVAL_P(data);
		int nums = zend_hash_num_elements(data_hash);
		
		MAKE_STD_ZVAL(piece);
		array_init_size(piece, nums);
		
		for (zend_hash_internal_pointer_reset(data_hash);
			zend_hash_get_current_data(data_hash, (void **) &element) == SUCCESS;
			zend_hash_move_forward(data_hash)
		) {
			if(zend_hash_get_current_key(data_hash, &key, &index, 0)==HASH_KEY_IS_STRING){
				if (Z_TYPE_PP(element)==IS_STRING){
					spprintf(&buff, 0, "%s='%s'", key, Z_STRVAL_PP(element));
				}else{
					spprintf(&buff, 0, "%s=%d", key, Z_LVAL_PP(element));
				}
				add_next_index_string(piece, buff, 1);
			}
		}
		
		set=join(",", piece, 0);
		spprintf(&sql, 0, "UPDATE `%s` SET %s %s;", table, Z_STRVAL_P(set), Z_STRVAL_P(where));
		stmt = pdo_exec(sql);
	} else {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "dangerous for update opration without condition!");
		RETVAL_NULL();
	}
	
	RETURN_ZVAL(stmt, 1, 0);
}
/* }}} */

/* {{{ proto public SimpleOrm::delete(string table_name[, array condition])
*/
PHP_METHOD(SimpleOrm, delete){
	zval *condition, *where, *instance, *p_key, *stmt;
	char *sql, *buff, *table;
	int table_len;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|z", &table, &table_len, &condition) == FAILURE) {
		return;
	}
	
	instance = zend_read_static_property(SimpleOrm_ce, ZEND_STRL("instance"), 1 TSRMLS_CC);
	p_key=zend_read_property(SimpleOrm_ce, instance, ZEND_STRL("primary_key"), 0 TSRMLS_CC);
	if (ZEND_NUM_ARGS()==1){
		where = zend_read_property(SimpleOrm_ce, instance, ZEND_STRL("where"), 0 TSRMLS_CC);
	} else {
		switch(Z_TYPE_P(condition)){
			case IS_LONG:
				spprintf(&buff, 0, "WHERE %s=%d", Z_STRVAL_P(p_key), Z_LVAL_P(condition));
				break;
				
			case IS_ARRAY:
				condition=join(",", condition, 0);
				spprintf(&buff, 0, "WHERE %s IN(%s)", Z_STRVAL_P(p_key), Z_STRVAL_P(condition));
				break;
				
			case IS_STRING:
				spprintf(&buff, 0, "WHERE %s", Z_STRVAL_P(condition));
				break;
				
			default:
				php_error_docref(NULL TSRMLS_CC, E_WARNING, "condition error!");
				RETVAL_NULL();
		}
		MAKE_STD_ZVAL(where);
		ZVAL_STRING(where, buff, 0);
	}
	
	if (Z_TYPE_P(where)==IS_NULL){
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "dangerous for delete opration without condition!");
		RETVAL_NULL();
	}
	
	spprintf(&sql, 0, "DELETE FROM `%s` %s;", table, Z_STRVAL_P(where));
	stmt = pdo_exec(sql);
	
	RETVAL_ZVAL(stmt, 0, 0);
}
/* }}} */

/* {{{ proto public SimpleOrm::delete(string table_name, array condition)
*/
PHP_METHOD(SimpleOrm, setAttribute){
	zval *value, *instance=NULL, *pdo=NULL, *code, *attribute;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "lz", &attribute, &value)==FAILURE) {
		return;
	}
	
	instance = zend_read_static_property(SimpleOrm_ce, ZEND_STRL("instance"), 1 TSRMLS_CC);
	pdo = zend_read_property(SimpleOrm_ce, instance, ZEND_STRL("pdo"), 0 TSRMLS_CC);
	zend_call_method(&pdo, NULL, NULL, ZEND_STRL("setAttribute"), &code, 2, attribute, value, NULL TSRMLS_DC);
	convert_to_boolean(code); 
	if (!code){
		RETVAL_FALSE;
	} else {
		RETVAL_TRUE;
	}
}
/* }}} */

/* {{{ proto public SimpleOrm::beginTransaction(voidn)
*/
PHP_METHOD(SimpleOrm, begin){
	CALL_PDO_METHOD("beginTransaction");
}
/* }}} */

/* {{{ proto public SimpleOrm::commit(void)
*/
PHP_METHOD(SimpleOrm, commit){
	CALL_PDO_METHOD("commit");
}
/* }}} */

/* {{{ proto public SimpleOrm::rollBack(void)
*/
PHP_METHOD(SimpleOrm, rollBack){
	CALL_PDO_METHOD("rollBack");
}
/* }}} */

/* {{{ proto public SimpleOrm::rollBack(void)
*/
PHP_METHOD(SimpleOrm, demo){
	CALL_PDO_METHOD("rollBack");
}
/* }}} */

/* {{{ proto public SimpleOrm::rollBack(void)
*/
PHP_METHOD(SimpleOrm, tableInfo){
	zval *self, *table_name, *stmt, *res, *_parm;
	char *sql;

	self =getThis();
	table_name=zend_read_property(Z_OBJCE_P(self), self, ZEND_STRL("table"), 0 TSRMLS_CC);
	spprintf(&sql, 0, "DESC `%s`;", Z_STRVAL_P(table_name));
	stmt = pdo_query(sql);
	zend_update_property(SimpleOrm_ce, self, ZEND_STRL("stmt"), stmt TSRMLS_CC);

	MAKE_STD_ZVAL(_parm);
	ZVAL_LONG(_parm, 2);
	CALL_PDO_STMT_METHOD("fetchAll", res, 1, _parm);

	RETVAL_ZVAL(res, 0, 0);
}
/* }}} */

PHP_SIMPLEORM_API zval * get_array_keys(zval * array TSRMLS_DC){
	zval *keys, **element;
	HashTable *arr_hash;
	HashPosition pos;
	char *string_key;
	uint string_key_len;
	ulong num_key;
	
	ALLOC_INIT_ZVAL(keys);
	array_init(keys);
	
	arr_hash=Z_ARRVAL_P(array);
	for (zend_hash_internal_pointer_reset_ex(arr_hash, &pos);
		zend_hash_get_current_data_ex(arr_hash, (void **) &element, &pos) == SUCCESS;
		zend_hash_move_forward_ex(arr_hash, &pos)
	) {
		switch (zend_hash_get_current_key_ex(arr_hash, &string_key, &string_key_len, &num_key, 0, &pos)) {
			case HASH_KEY_IS_STRING:
				add_next_index_stringl(keys, string_key, string_key_len - 1, 1);
				break;
			case HASH_KEY_IS_LONG:
				add_next_index_long(keys, num_key);
		}
	}
	
	return keys;
}


PHP_SIMPLEORM_API zval * join(char *delim, zval *arr, int type TSRMLS_DC){
	zval         **tmp;
	HashPosition   pos;
	smart_str      implstr = {0};
	int            numelems, i = 0;
	zval tmp_val;
	int str_len;
	zval *return_val;

	numelems = zend_hash_num_elements(Z_ARRVAL_P(arr));
	if (numelems == 0) {
		return return_val;
	}

	zend_hash_internal_pointer_reset_ex(Z_ARRVAL_P(arr), &pos);
	while (zend_hash_get_current_data_ex(Z_ARRVAL_P(arr), (void **) &tmp, &pos) == SUCCESS) {
		switch ((*tmp)->type) {
			case IS_STRING:{
					if(type==1){
						char buff[MAX_LENGTH_OF_LONG + 1];
						int len;
						len=sprintf(buff, "'%s'", Z_STRVAL_PP(tmp));
						smart_str_appendl(&implstr, buff, len);
					}else{
						smart_str_appendl(&implstr, Z_STRVAL_PP(tmp), Z_STRLEN_PP(tmp));
					}
				}
				break;
			case IS_LONG: {
					char stmp[MAX_LENGTH_OF_LONG + 1];
					str_len = slprintf(stmp, sizeof(stmp), "%ld", Z_LVAL_PP(tmp));
					smart_str_appendl(&implstr, stmp, str_len);
				}
				break;
			case IS_BOOL:
				if (Z_LVAL_PP(tmp) == 1) {
					smart_str_appendl(&implstr, "1", sizeof("1")-1);
				}
				break;
			case IS_NULL:
				break;
			case IS_DOUBLE: {
					char *stmp;
					str_len = spprintf(&stmp, 0, "%.*G", (int) EG(precision), Z_DVAL_PP(tmp));
					smart_str_appendl(&implstr, stmp, str_len);
					efree(stmp);
				}
				break;
			case IS_OBJECT: {
					int copy;
					zval expr;
					zend_make_printable_zval(*tmp, &expr, &copy);
					smart_str_appendl(&implstr, Z_STRVAL(expr), Z_STRLEN(expr));
					if (copy) {
						zval_dtor(&expr);
					}
				}
				break;
			default:
				tmp_val = **tmp;
				zval_copy_ctor(&tmp_val);
				convert_to_string(&tmp_val);
				smart_str_appendl(&implstr, Z_STRVAL(tmp_val), Z_STRLEN(tmp_val));
				zval_dtor(&tmp_val);
				break;
		}
		if (++i != numelems) {
			smart_str_appendl(&implstr, delim, strlen(delim));
		}
		zend_hash_move_forward_ex(Z_ARRVAL_P(arr), &pos);
	}

	smart_str_0(&implstr);
	if (implstr.len) {
		MAKE_STD_ZVAL(return_val);
		ZVAL_STRING(return_val, implstr.c, 0);
	} else {
		smart_str_free(&implstr);
	}
	
	return return_val;
}

PHP_SIMPLEORM_API zval * pdo_query(char *query TSRMLS_DC){
	zval *pdo, *stmt, *instance, *sql;
	
	MAKE_STD_ZVAL(sql);
	ZVAL_STRING(sql, query, 0);

	instance = zend_read_static_property(SimpleOrm_ce, ZEND_STRL("instance"), 1 TSRMLS_CC);
	zend_update_property_string(Z_OBJCE_P(instance), instance, ZEND_STRL("sql"), query TSRMLS_DC);
	pdo = zend_read_property(SimpleOrm_ce, instance, ZEND_STRL("pdo"), 0 TSRMLS_CC);
	zend_call_method(&pdo, Z_OBJCE_P(pdo), NULL, ZEND_STRL("query"), &stmt, 1, sql, NULL TSRMLS_CC);
	zval_ptr_dtor(&sql);
	
	if(Z_TYPE_P(stmt)==IS_BOOL){
		stmt=pdo_errorInfo();
	}
	return stmt;
}

PHP_SIMPLEORM_API zval * pdo_exec(char * query TSRMLS_DC){
	zval *pdo, *stmt, *instance, *sql, *rows;

	MAKE_STD_ZVAL(sql);
	ZVAL_STRING(sql, query, 0);
	instance = zend_read_static_property(SimpleOrm_ce, ZEND_STRL("instance"), 1 TSRMLS_CC);
	pdo = zend_read_property(SimpleOrm_ce, instance, ZEND_STRL("pdo"), 0 TSRMLS_CC);
	zend_call_method(&pdo, Z_OBJCE_P(pdo), NULL, ZEND_STRL("exec"), &rows, 1, sql, NULL TSRMLS_CC);
	zval_ptr_dtor(&sql);
	
	if(Z_TYPE_P(rows)==IS_BOOL){
		rows=pdo_errorInfo();
	}
	
	return rows;
}

PHP_SIMPLEORM_API zval * pdo_errorCode(){
	zval *pdo, *code, *instance;
	instance = zend_read_static_property(SimpleOrm_ce, ZEND_STRL("instance"), 1 TSRMLS_CC);
	pdo = zend_read_property(SimpleOrm_ce, instance, ZEND_STRL("pdo"), 0 TSRMLS_CC);
	zend_call_method(&pdo, NULL, NULL, ZEND_STRL("errorCode"), &code, NULL TSRMLS_DC);
	
	return code;
}

PHP_SIMPLEORM_API zval * pdo_errorInfo(){
	zval *pdo, *info, *instance;
	zend_function ** fn;
	instance = zend_read_static_property(SimpleOrm_ce, ZEND_STRL("instance"), 1 TSRMLS_CC);
	pdo = zend_read_property(SimpleOrm_ce, instance, ZEND_STRL("pdo"), 0 TSRMLS_CC);
	zend_call_method(&pdo, NULL, NULL, ZEND_STRL("errorInfo"), &info, NULL TSRMLS_DC);
	
	return info;
}

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */

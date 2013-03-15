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
  | Author:    merlinal                                                          |
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
	zend_declare_property_null(SimpleOrm_ce, ZEND_STRL("instance"), ZEND_ACC_PUBLIC|ZEND_ACC_STATIC TSRMLS_CC);
	
	zend_declare_property_null(SimpleOrm_ce, ZEND_STRL("action"), ZEND_ACC_PUBLIC TSRMLS_DC);
	zend_declare_property_stringl(SimpleOrm_ce, ZEND_STRL("field"), ZEND_STRL("*"), ZEND_ACC_PUBLIC TSRMLS_DC);
	zend_declare_property_null(SimpleOrm_ce, ZEND_STRL("table"), ZEND_ACC_PUBLIC TSRMLS_DC);
	zend_declare_property_stringl(SimpleOrm_ce, ZEND_STRL("primary_key"), ZEND_STRL("id"), ZEND_ACC_PUBLIC TSRMLS_DC);
	zend_declare_property_null(SimpleOrm_ce, ZEND_STRL("where"), ZEND_ACC_PUBLIC TSRMLS_DC);
	zend_declare_property_null(SimpleOrm_ce, ZEND_STRL("order"), ZEND_ACC_PUBLIC TSRMLS_DC);
	zend_declare_property_null(SimpleOrm_ce, ZEND_STRL("limit"), ZEND_ACC_PUBLIC TSRMLS_DC);
	
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
PHP_MINFO_FUNCTION(SimpleOrm)
{
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
PHP_METHOD(SimpleOrm, __construct)
{
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
PHP_METHOD(SimpleOrm, getInstance)
{
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
PHP_METHOD(SimpleOrm, query)
{
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
PHP_METHOD(SimpleOrm, exec)
{
	char *query;
	int query_len, rows;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &query, &query_len) == FAILURE) {
		return;
	}
	rows = pdo_exec(query);
	RETURN_LONG(rows);
}
/* }}} */

/* {{{ proto public SimpleOrm::select(string table)
*/
PHP_METHOD(SimpleOrm, select)
{
	char *table;
	int table_len;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &table, &table_len) == FAILURE) {
		return;
	}
	
	zval *self=getThis();
	zend_update_property_string(Z_OBJCE_P(self), self, ZEND_STRL("action"), "SELECT" TSRMLS_DC);
	zend_update_property_string(Z_OBJCE_P(self), self, ZEND_STRL("table"), table TSRMLS_DC);
	
	RETURN_ZVAL(self, 1, 0);
}
/* }}} */

/* {{{ proto public SimpleOrm::select(string fields)
*/
PHP_METHOD(SimpleOrm, field)
{
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

/* {{{ proto public SimpleOrm::where(string where)
*/
PHP_METHOD(SimpleOrm, where)
{
	zval *self;
	char *where;
	int where_len;
	smart_str implstr = {0};
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|s", &where, &where_len) == FAILURE) {
		return;
	}

	self=getThis();
	if(ZEND_NUM_ARGS()==1){
		smart_str_appendl(&implstr, "WHERE ", sizeof("WHERE ")-1);
		smart_str_appendl(&implstr, where, sizeof(where));
		zend_update_property_string(Z_OBJCE_P(self), self, ZEND_STRL("where"), implstr.c TSRMLS_DC);
	}
	
	RETURN_ZVAL(self, 1, 0);
}
/* }}} */

/* {{{ proto public SimpleOrm::order(string order)
*/
PHP_METHOD(SimpleOrm, order)
{
	zval *self;
	char *order;
	int order_len;
	smart_str implstr = {0};
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &order, &order_len) == FAILURE) {
		return;
	}
	
	smart_str_appendl(&implstr, "ORDER BY ", sizeof("ORDER BY ")-1);
	smart_str_appendl(&implstr, order, sizeof(order));
	
	self=getThis();
	zend_update_property_string(Z_OBJCE_P(self), self, ZEND_STRL("order"), implstr.c TSRMLS_DC);
	
	RETURN_ZVAL(self, 1, 0);
}
/* }}} */

/* {{{ proto public SimpleOrm::limit(int offset, int length)
*/
PHP_METHOD(SimpleOrm, limit)
{
	smart_str implstr = {0};
	long offset, length;
	int str_len;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l|l", &offset, &length) == FAILURE) {
		return;
	}
	
	char stmp[MAX_LENGTH_OF_LONG + 1];
	if (ZEND_NUM_ARGS()==1){
		str_len = slprintf(stmp, sizeof(stmp), "LIMIT %d", offset);
	}else if(ZEND_NUM_ARGS()==2){
		str_len = slprintf(stmp, sizeof(stmp), "LIMIT %d,%d", offset, length);
	}
	smart_str_appendl(&implstr, stmp, str_len);
	smart_str_0(&implstr);
	zend_update_property_string(Z_OBJCE_P(getThis()), getThis(), ZEND_STRL("limit"), implstr.c TSRMLS_DC);
	
	RETURN_ZVAL(getThis(), 1, 0);
}
/* }}} */

/* {{{ proto public SimpleOrm::find(string query)
*/
PHP_METHOD(SimpleOrm, find)
{
	zval *stmt, *query, *action, *field, *table, *primary_key, *self=NULL, *where, *order, *limit;
	char sql[30];
	int query_len, rows;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|z", &query) == FAILURE) {
		return;
	}
	
	self=getThis();
	action=zend_read_property(Z_OBJCE_P(self), self, ZEND_STRL("action"), 0 TSRMLS_CC);
	field=zend_read_property(Z_OBJCE_P(self), self, ZEND_STRL("field"), 0 TSRMLS_CC);
	primary_key=zend_read_property(Z_OBJCE_P(self), self, ZEND_STRL("primary_key"), 0 TSRMLS_CC);
	table=zend_read_property(Z_OBJCE_P(self), self, ZEND_STRL("table"), 0 TSRMLS_CC);
	where=zend_read_property(Z_OBJCE_P(self), self, ZEND_STRL("where"), 0 TSRMLS_CC);
	order=zend_read_property(Z_OBJCE_P(self), self, ZEND_STRL("order"), 0 TSRMLS_CC);
	limit=zend_read_property(Z_OBJCE_P(self), self, ZEND_STRL("limit"), 0 TSRMLS_CC);
	
	Z_TYPE_P(where)==IS_NULL && (Z_STRVAL_P(where) = "");
	Z_TYPE_P(order)==IS_NULL && (Z_STRVAL_P(order) = "");
	Z_TYPE_P(limit)==IS_NULL && (Z_STRVAL_P(limit) = "");
	
	if(ZEND_NUM_ARGS()==1){
		if(Z_TYPE_P(query)==IS_LONG){
			sprintf(sql, "%s %s FROM `%s` WHERE %s=%d;",Z_STRVAL_P(action),Z_STRVAL_P(field), Z_STRVAL_P(table), Z_STRVAL_P(primary_key), Z_LVAL_P(query));
		}else if(Z_TYPE_P(query)==IS_ARRAY){
			HashTable *query_hash;
			HashPosition pos;
			zval **element;
			smart_str implstr = {0};
			int i=0;
			
			query_hash=Z_ARRVAL_P(query);
			int numelems = zend_hash_num_elements(query_hash);
			for (zend_hash_internal_pointer_reset_ex(query_hash, &pos);
				zend_hash_get_current_data_ex(query_hash, (void **) &element, &pos) == SUCCESS;
				zend_hash_move_forward_ex(query_hash, &pos)
			) {
				if(Z_TYPE_PP(element)==IS_LONG){
					char stmp[MAX_LENGTH_OF_LONG + 1];
					int str_len = slprintf(stmp, sizeof(stmp), "%ld", Z_LVAL_PP(element));
					smart_str_appendl(&implstr, stmp, str_len);
				}
				if(++i != numelems)
					smart_str_appendl(&implstr, ",", sizeof(",")-1);
			}
			smart_str_0(&implstr);
			sprintf(sql, "%s %s FROM `%s` WHERE `%s` IN (%s);",Z_STRVAL_P(action), Z_STRVAL_P(field), Z_STRVAL_P(table), Z_STRVAL_P(primary_key), implstr.c);
		}
	}else{
		sprintf(sql, "%s %s FROM `%s` %s %s %s;",Z_STRVAL_P(action), Z_STRVAL_P(field), Z_STRVAL_P(table), Z_STRVAL_P(where), Z_STRVAL_P(order), Z_STRVAL_P(limit));
	}
	stmt = pdo_query(sql);
	
	RETURN_ZVAL(stmt, 1, 0);
}
/* }}} */


PHP_METHOD(SimpleOrm, insert)
{
	zval *stmt, *query, *action, *field, *table, *primary_key, *self=NULL, *where, *order, *limit;
	char sql[30];
	int query_len, rows;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|z", &query) == FAILURE) {
		return;
	}
}


PHP_SIMPLEORM_API zval * pdo_query(char *query TSRMLS_DC){
	zval *pdo, *stmt, *instance, *sql;
	
	MAKE_STD_ZVAL(sql);
	ZVAL_STRING(sql, query, 0);

	instance = zend_read_static_property(SimpleOrm_ce, ZEND_STRL("instance"), 1 TSRMLS_CC);
	pdo = zend_read_property(SimpleOrm_ce, instance, ZEND_STRL("pdo"), 0 TSRMLS_CC);
	zend_call_method(&pdo, Z_OBJCE_P(pdo), NULL, ZEND_STRL("query"), &stmt, 1, sql, NULL TSRMLS_CC);
	zval_ptr_dtor(sql);
	
	if(Z_TYPE_P(stmt)==IS_BOOL){
		stmt=pdo_errorInfo();
		//php_error_docref(NULL TSRMLS_CC, E_WARNING, "query error!");
		return stmt;
	}
	pdo_errorCode();
	return stmt;
}

PHP_SIMPLEORM_API int pdo_exec(char * query TSRMLS_DC){
	zval *pdo, *stmt, *instance, *sql, *rows;

	MAKE_STD_ZVAL(sql);
	ZVAL_STRING(sql, query, 0);
	instance = zend_read_static_property(SimpleOrm_ce, ZEND_STRL("instance"), 1 TSRMLS_CC);
	pdo = zend_read_property(SimpleOrm_ce, instance, ZEND_STRL("pdo"), 0 TSRMLS_CC);
	zend_call_method(&pdo, Z_OBJCE_P(pdo), NULL, ZEND_STRL("exec"), &rows, 1, sql, NULL TSRMLS_CC);
	zval_ptr_dtor(sql);
	
	return Z_LVAL_P(rows);
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

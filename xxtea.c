
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_xxtea.h"
#include "libxxtea.h"
#include <memory.h>

/* If you declare any globals in php_xxtea.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(xxtea)
*/

/* True global resources - no need for thread safety here */
static int le_xxtea;

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("xxtea.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_xxtea_globals, xxtea_globals)
    STD_PHP_INI_ENTRY("xxtea.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_xxtea_globals, xxtea_globals)
PHP_INI_END()
*/
/* }}} */

/* Remove the following function when you have successfully modified config.m4
   so that your module can be compiled into PHP, it exists only for testing
   purposes. */
 

/* Every user-visible function in PHP should document itself in the source */
/* {{{ proto string confirm_xxtea_compiled(string arg)
   Return a string to confirm that the module is compiled in */
/* XXTEA ENCODE */
PHP_FUNCTION(xxtea_encode)
{
	char *text = NULL;
	char *key = NULL;
	int t_len, k_len, s_len;
	char *secure_text;
	char *result;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ss", &text, &t_len, &key, &k_len) == FAILURE) {
		return;
	}
	
	// php_printf("Decode String: %s  Length:%d\n", text, t_len);
	s_len = xxtea_bytes_encode(text, t_len, key, k_len, &secure_text);
	s_len = spprintf(&result, s_len, "%s", secure_text);
	free(secure_text);
	// php_printf("Encode String: %s  Length:%d\n", secure_text, r_len);
	RETURN_STRINGL(result, s_len, 0);
}

/* XXTEA DECODE */
PHP_FUNCTION(xxtea_decode)
{
	char *secure_text = NULL;
	char *key = NULL;
	char *text, *result;
	int s_len, k_len, r_len;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ss", &secure_text, &s_len, &key, &k_len) == FAILURE) {
		return;
	}
	
	// php_printf("Encode String: %s  Length:%d\n", secure_text, s_len);
	r_len = xxtea_bytes_decode(secure_text, s_len, key, k_len, &text);
	r_len = spprintf(&result, r_len, "%s", text);
	free(text);
	// php_printf("Decode String: %s  Length:%d\n", text, r_len);
	RETURN_STRINGL(result, r_len, 0);
}

/* }}} */
/* The previous line is meant for vim and emacs, so it can correctly fold and 
   unfold functions in source code. See the corresponding marks just before 
   function definition, where the functions purpose is also documented. Please 
   follow this convention for the convenience of others editing your code.
*/


/* {{{ php_xxtea_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_xxtea_init_globals(zend_xxtea_globals *xxtea_globals)
{
	xxtea_globals->global_value = 0;
	xxtea_globals->global_string = NULL;
}
*/
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(xxtea)
{
	/* If you have INI entries, uncomment these lines 
	REGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(xxtea)
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
PHP_RINIT_FUNCTION(xxtea)
{
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(xxtea)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(xxtea)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "xxtea support", "enabled");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */

/* {{{ xxtea_functions[]
 *
 * Every user visible function must have an entry in xxtea_functions[].
 */
const zend_function_entry xxtea_functions[] = {
	PHP_FE(xxtea_encode,	NULL)		/* For testing, remove later. */
	PHP_FE(xxtea_decode,	NULL)
	PHP_FE_END	/* Must be the last line in xxtea_functions[] */
};
/* }}} */

/* {{{ xxtea_module_entry
 */
zend_module_entry xxtea_module_entry = {
	STANDARD_MODULE_HEADER,
	"xxtea",
	xxtea_functions,
	PHP_MINIT(xxtea),
	PHP_MSHUTDOWN(xxtea),
	PHP_RINIT(xxtea),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(xxtea),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(xxtea),
	PHP_XXTEA_VERSION,
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_XXTEA
ZEND_GET_MODULE(xxtea)
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */

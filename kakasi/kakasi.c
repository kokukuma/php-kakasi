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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_kakasi.h"

#include <libkakasi.h>
#include <iconv.h>

#define MYBUFSZ 1024

//int UtfToEuc(char *str);
//int EucToUtf(char *str);
int convert(char const *src,
			char const *dest,
			char const *text,
			char *buf,
			size_t bufsize);
//char* ExeKakasi(char *srcStr, char *option[]);
int
ExeKakasi(char  *srcStr,
	  	  char  *option[],
	   	  char *dstStr,
	   	  size_t dststrsize);

int ToHira(char *word, char *hira);
int ToKata(char *word, char *kata);
int ToAlph(char *word, char *alph);

/* If you declare any globals in php_kakasi.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(kakasi)
*/

/* True global resources - no need for thread safety here */
static int le_kakasi;

/* {{{ kakasi_functions[]
 *
 * Every user visible function must have an entry in kakasi_functions[].
 */
const zend_function_entry kakasi_functions[] = {
	PHP_FE(confirm_kakasi_compiled,	NULL)		/* For testing, remove later. */
	PHP_FE(kakasi, NULL) 
	PHP_ME(kakasi, reproc ,NULL,0)
	//PHP_ME(kakasi, hira   ,NULL,0)
	//PHP_ME(kakasi, alph   ,NULL,0)
	//PHP_ME(kakasi, kata   ,NULL,0)
	PHP_FE_END	/* Must be the last line in kakasi_functions[] */
};
/* }}} */

/* {{{ kakasi_module_entry
 */
zend_module_entry kakasi_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"kakasi",
	kakasi_functions,
	PHP_MINIT(kakasi),
	PHP_MSHUTDOWN(kakasi),
	PHP_RINIT(kakasi),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(kakasi),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(kakasi),
#if ZEND_MODULE_API_NO >= 20010901
	"0.1", /* Replace with version number for your extension */
#endif
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_KAKASI
ZEND_GET_MODULE(kakasi)
#endif

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("kakasi.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_kakasi_globals, kakasi_globals)
    STD_PHP_INI_ENTRY("kakasi.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_kakasi_globals, kakasi_globals)
PHP_INI_END()
*/
/* }}} */

/* {{{ php_kakasi_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_kakasi_init_globals(zend_kakasi_globals *kakasi_globals)
{
	kakasi_globals->global_value = 0;
	kakasi_globals->global_string = NULL;
}
*/
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(kakasi)
{
	/* If you have INI entries, uncomment these lines 
	REGISTER_INI_ENTRIES();
	*/

	zend_class_entry ce;
	INIT_CLASS_ENTRY(ce, "KAKASI", kakasi_functions);
	kakasi_ce = zend_register_internal_class(&ce TSRMLS_CC);
	zend_declare_property_string(kakasi_ce,
						 					"word", strlen("word"),
											"test", ZEND_ACC_PUBLIC);
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(kakasi)
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
PHP_RINIT_FUNCTION(kakasi)
{
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(kakasi)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(kakasi)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "kakasi support", "enabled");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */


/* Remove the following function when you have succesfully modified config.m4
   so that your module can be compiled into PHP, it exists only for testing
   purposes. */

/* Every user-visible function in PHP should document itself in the source */
/* {{{ proto string confirm_kakasi_compiled(string arg)
   Return a string to confirm that the module is compiled in */
PHP_FUNCTION(confirm_kakasi_compiled)
{
	char *arg = NULL;
	int arg_len, len;
	char *strg;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &arg, &arg_len) == FAILURE) {
		return;
	}

	len = spprintf(&strg, 0, "Congratulations! You have successfully modified ext/%.78s/config.m4. Module %.78s is now compiled into PHP.", "kakasi", arg);
	RETURN_STRINGL(strg, len, 0);
}
/* }}} */
/* The previous line is meant for vim and emacs, so it can correctly fold and 
   unfold functions in source code. See the corresponding marks just before 
   function definition, where the functions purpose is also documented. Please 
   follow this convention for the convenience of others editing your code.
*/


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
PHP_FUNCTION(kakasi){
	return;
}

PHP_METHOD(kakasi, reproc){

	zval *obj;
	zval *word;

	//char *hira;
	//char *kata;
	//char *alph;
	char hira[MYBUFSZ];
	char kata[MYBUFSZ];
	char alph[MYBUFSZ];
	
	// wordを取得
	obj = getThis();
	word = zend_read_property(Z_OBJCE_P(obj),obj, "word",strlen("word"), 1 TSRMLS_CC);

	// ひらがな
	ToHira(word->value.str.val, hira);

	// カタカナ 
	ToKata(word->value.str.val, kata);

	// alphabet
	ToAlph(word->value.str.val, alph);


	// 返却オブジェクトの生成
	zval *wordset;
	MAKE_STD_ZVAL(wordset);
	if(object_init(wordset) != SUCCESS){}

	add_property_stringl(wordset,"base",word->value.str.val,strlen(word->value.str.val),1);
	add_property_stringl(wordset,"hira",hira,strlen(hira),1);
	add_property_stringl(wordset,"kata",kata,strlen(kata),1);
	add_property_stringl(wordset,"alph",alph,strlen(alph),1);

	// オブジェクトの返却
	*return_value = *wordset;
	zval_copy_ctor(return_value);

	return;
}

int
ToHira(char *WORD, char *RETURN){

	char *optionJH[] = { "kakasi", "-JH"};
	char *optionKH[] = { "kakasi", "-KH"};

	char resJH[MYBUFSZ];
	char resKH[MYBUFSZ];
	char resHK[MYBUFSZ];


	// J -> H
	ExeKakasi(WORD, optionJH, resJH, MYBUFSZ);   


	// H -> K 
	ExeKakasi(resJH, optionKH, RETURN, MYBUFSZ);   

	return ;

}

int
ToKata(char *WORD, char *RETURN){

	char *optionJH[] = { "kakasi", "-JH"};
	char *optionHK[] = { "kakasi", "-HK"};

	char resJH[MYBUFSZ];
	char resKH[MYBUFSZ];
	char resHK[MYBUFSZ];


	// J -> H
	ExeKakasi(WORD, optionJH, resJH, MYBUFSZ);   

	// H -> K 
	ExeKakasi(resJH, optionHK, RETURN, MYBUFSZ);   

	return ;

}

int
ToAlph(char *WORD, char *RETURN){

	char *optionJH[] = { "kakasi", "-JH"};
	char *optionKH[] = { "kakasi", "-KH"};
	char *optionHa[] = { "kakasi", "-Ha"};

	char resJH[MYBUFSZ];
	char resKH[MYBUFSZ];
	char resHK[MYBUFSZ];


	// J -> H
	ExeKakasi(WORD, optionJH, resJH, MYBUFSZ);   

	// H -> K 
	ExeKakasi(resJH, optionKH, resKH, MYBUFSZ);   

	// H -> K 
	ExeKakasi(resKH, optionHa, RETURN, MYBUFSZ);   

	return ;

}


//PHP_METHOD(kakasi, hira){
//	// define	
//	zval *objh;
//	zval *word;
//	char *base;
//
//	char *optionJH[] = { "kakasi", "-JH"};
//	char *optionKH[] = { "kakasi", "-KH"};
//	char *optionHK[] = { "kakasi", "-HK"};
//
//	char resJH[MYBUFSZ];
//	char resKH[MYBUFSZ];
//	char resHK[MYBUFSZ];
//
//    //char str[MYBUFSZ+1];
//    //char *ptrin = str;
//
//	// get filter string
//	objh = getThis();
//	word = zend_read_property(Z_OBJCE_P(objh),objh, "word",strlen("word"), 1 TSRMLS_CC);
//
//	
//	// J -> H
//	base = word->value.str.val;
//
//	// J -> H
//	ExeKakasi(base, optionJH, resJH, MYBUFSZ);   
//
//	// H -> K 
//	ExeKakasi(resJH, optionHK, resHK, MYBUFSZ);   
//
//	// H -> K 
//	ExeKakasi(resHK, optionKH, resKH, MYBUFSZ);   
//
//
//	// retrun
//	RETURN_STRINGL(resKH, strlen(resKH), 0);   	
//
//	return ;
//
//}
//PHP_METHOD(kakasi, alph){
//	// define	
//	zval *objh;
//	zval *word;
//	char *base;
//
//	char *optionJH[] = { "kakasi", "-JH"};
//	char *optionKH[] = { "kakasi", "-KH"};
//	char *optionHa[] = { "kakasi", "-Ha"};
//
//	char resJH[MYBUFSZ];
//	char resKH[MYBUFSZ];
//	char resHa[MYBUFSZ];
//
//    //char str[MYBUFSZ+1];
//    //char *ptrin = str;
//
//	// get filter string
//	objh = getThis();
//	word = zend_read_property(Z_OBJCE_P(objh),objh, "word",strlen("word"), 1 TSRMLS_CC);
//
//	
//	// J -> H
//	base = word->value.str.val;
//
//	// J -> H
//	ExeKakasi(base, optionJH, resJH, MYBUFSZ);   
//
//	// H -> K 
//	ExeKakasi(resJH, optionKH, resKH, MYBUFSZ);   
//
//	// H -> a 
//	ExeKakasi(resKH, optionHa, resHa, MYBUFSZ);   
//
//	// retrun
//	RETURN_STRINGL(resHa, strlen(resHa), 0);   	
//
//	return ;
//
//}
//PHP_METHOD(kakasi, kata){
//	// define	
//	zval *objh;
//	zval *word;
//	char *base;
//
//	char *optionJH[] = { "kakasi", "-JH"};
//	char *optionKH[] = { "kakasi", "-KH"};
//	char *optionHK[] = { "kakasi", "-HK"};
//
//	char resJH[MYBUFSZ];
//	char resKH[MYBUFSZ];
//	char resHK[MYBUFSZ];
//
//    //char str[MYBUFSZ+1];
//    //char *ptrin = str;
//
//	// get filter string
//	objh = getThis();
//	word = zend_read_property(Z_OBJCE_P(objh),objh, "word",strlen("word"), 1 TSRMLS_CC);
//
//	
//	// J -> H
//	base = word->value.str.val;
//
//	// J -> H
//	ExeKakasi(base, optionJH, resJH, MYBUFSZ);   
//
//	// H -> K 
//	ExeKakasi(resJH, optionHK, resHK, MYBUFSZ);   
//
//	// H -> K 
//	ExeKakasi(resHK, optionKH, resKH, MYBUFSZ);   
//
//
//	// retrun
//	RETURN_STRINGL(resHK, strlen(resHK), 0);   	
//
//}

int
ExeKakasi(char  *srcStr,
	   		char  *option[],
		   	char *dstStr,
		   	size_t dststrsize)
{

    char buf[MYBUFSZ];
    char *res;
	int ret;

	// EUCに変換
	convert("UTF-8","EUC-JP",srcStr,buf, sizeof(buf));

	// kakasiのオプションを設定
	ret = kakasi_getopt_argv(2, option);

	if(ret == 0)
	{
		// kakasiを実行
		res = kakasi_do(&buf);
		if(buf)
		{            
			convert("EUC-JP","UTF-8",res,dstStr,dststrsize);
			return 0;
		}
	}
}


int convert(char const *src,
			char const *dest,
			char const *text,
			char *buf,
			size_t bufsize)
{
	iconv_t cd;
	size_t srclen, destlen;
	size_t ret;

	cd = iconv_open(dest, src);
	if(cd == (iconv_t)-1){
		perror("iconv open");
		return 0;
	}
	srclen = strlen(text);
	destlen = bufsize -1;
	memset(buf, '\0', bufsize);

	ret = iconv(cd, &text, &srclen, &buf, &destlen);
	if(ret == -1){
		perror("iconv");
		return 0;
	}
	iconv_close(cd);
	return 1;
}

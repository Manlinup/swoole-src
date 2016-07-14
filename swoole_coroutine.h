/*
 +----------------------------------------------------------------------+
 | Swoole                                                               |
 +----------------------------------------------------------------------+
 | Copyright (c) 2012-2015 The Swoole Group                             |
 +----------------------------------------------------------------------+
 | This source file is subject to version 2.0 of the Apache license,    |
 | that is bundled with this package in the file LICENSE, and is        |
 | available through the world-wide-web at the following url:           |
 | http://www.apache.org/licenses/LICENSE-2.0.html                      |
 | If you did not receive a copy of the Apache2.0 license and are unable|
 | to obtain it through the world-wide-web, please send a note to       |
 | license@swoole.com so we can mail you a copy immediately.            |
 +----------------------------------------------------------------------+
 | Author: Tianfeng Han  <mikan.tenny@gmail.com>                        |
 +----------------------------------------------------------------------+
 */

#ifndef SWOOLE_COROUTINE_H_
#define SWOOLE_COROUTINE_H_

#define coro_global _coro_global

#define CORO_END 0
#define CORO_YIELD 1
#define CORO_LIMIT 2
#define CORO_SAVE 3

typedef struct _php_context php_context;

struct _php_context
{
    zval **current_coro_return_value_ptr_ptr;
    zval *current_coro_return_value_ptr;
    void *coro_params;
    int coro_params_cnt;
    void (*onTimeout)(struct _php_context *cxt);
    zval **current_eg_return_value_ptr_ptr;
    zend_execute_data *current_execute_data;
    zend_op **current_opline_ptr;
    zend_op *current_opline;
    zend_op_array *current_active_op_array;
    HashTable *current_active_symbol_table;
    zval *current_this;
    zend_class_entry *current_scope;
    zend_class_entry *current_called_scope;
    zend_vm_stack current_vm_stack;
    void *parent;
};

typedef struct
{
    int coro_num;
    int max_coro_num;
    zend_vm_stack origin_vm_stack;
    zend_execute_data *origin_ex;
} _coro_global;


coro_global COROG;

int coro_init();
int coro_create(zend_fcall_info_cache *op_array, zval **argv, int argc, zval **retval);
void coro_close();
php_context *coro_save(zval *return_value, zval **return_value_ptr, php_context *sw_php_context);
int coro_resume(php_context *sw_current_context, zval *retval, zval **coro_retval);
void coro_yield();
void coro_handle_timeout();
long php_swoole_add_timer_coro(int ms, int cli_fd, void* param TSRMLS_DC);
int php_swoole_clear_timer_coro(long id TSRMLS_DC);
#endif

#include "leptjson.h"
#include <assert.h>  /* assert() */
#include <stdlib.h>  /* NULL */

#define EXPECT(c, ch)       do { assert(*c->json == (ch)); c->json++; } while(0)

typedef struct {
    const char* json;
}lept_context;

static void lept_parse_whitespace(lept_context* c) {	//传入lept_context的地址
    const char *p = c->json;	//将结构体中的指针json赋给p
    while (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r')
        p++;	//如果指针指向的值为空白，则指针向后移动一位
    c->json = p;	//将p指针赋给结构体中的指针json
}

static int lept_parse_null(lept_context* c, lept_value* v) {
    EXPECT(c, 'n');	//检查结构体中常量指针json指向的值第一位是否为n，如果为n则指针向后移动一位
    if (c->json[0] != 'u' || c->json[1] != 'l' || c->json[2] != 'l')
        return LEPT_PARSE_INVALID_VALUE;	//如果常量指针指向的值不为null，则返回LEPT_PARSE_INVALID_VALUE
    c->json += 3;	//json指针向后移动三位
    v->type = LEPT_NULL;	
	if (c->json[1] != '\0')
		return LEPT_PARSE_ROOT_NOT_SINGULAR;
    return LEPT_PARSE_OK;
}

static int lept_parse_true(lept_context* c, lept_value* v) {
	EXPECT(c, 't');	
	if (c->json[0] != 'r' || c->json[1] != 'u' || c->json[2] != 'e')
		return LEPT_PARSE_INVALID_VALUE;	
	c->json += 3;	//json指针向后移动三位
	v->type = LEPT_TRUE;
	if (c->json[1] != '\0')
		return LEPT_PARSE_ROOT_NOT_SINGULAR;
	return LEPT_PARSE_OK;
}

static int lept_parse_false(lept_context* c, lept_value* v) {
	EXPECT(c, 'f');
	if (c->json[0] != 'a' || c->json[1] != 'l' || c->json[2] != 's'||c->json[3]!='e')
		return LEPT_PARSE_INVALID_VALUE;
	c->json += 4;	//json指针向后移动三位
	v->type = LEPT_FALSE;
	if (c->json[1] != '\0')
		return LEPT_PARSE_ROOT_NOT_SINGULAR;
	return LEPT_PARSE_OK;
}

static int lept_parse_value(lept_context* c, lept_value* v) {	//传入存放常量字符指针结构体的地址和指向json结构体的指针
    switch (*(c->json)) {	//读取结构体中字符指针的值
        case 'n':  return lept_parse_null(c, v);	//如果是字符n，即null的开头
		case 't':  return lept_parse_true(c, v);
		case 'f':  return lept_parse_false(c, v);
        case '\0': return LEPT_PARSE_EXPECT_VALUE;	//如果是c风格字符串的结束符，则返回LEPT_PARSE_EXPECT_VALUE(json只含有空白)
        default:   return LEPT_PARSE_INVALID_VALUE;
    }
}

int lept_parse(lept_value* v, const char* json) { //v是指向一个json结构体的指针，json是一个指向常量字符的指针
    lept_context c;	//c是一个结构体，内部存放了一个指向常量字符的指针
    assert(v != NULL);	//检查v是否存在
    c.json = json;	//将传入的常量字符指针赋给c中的常量字符指针
    v->type = LEPT_NULL;	//将v的值改为NULL
    lept_parse_whitespace(&c);	//对c中的常量字符指针进行操作，使其跳过空白
    return lept_parse_value(&c, v);	//将c中解析出来的根值写入v
}

lept_type lept_get_type(const lept_value* v) {
    assert(v != NULL);
    return v->type;
}

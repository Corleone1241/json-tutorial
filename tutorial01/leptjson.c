#include "leptjson.h"
#include <assert.h>  /* assert() */
#include <stdlib.h>  /* NULL */

#define EXPECT(c, ch)       do { assert(*c->json == (ch)); c->json++; } while(0)

typedef struct {
    const char* json;
}lept_context;

static void lept_parse_whitespace(lept_context* c) {	//����lept_context�ĵ�ַ
    const char *p = c->json;	//���ṹ���е�ָ��json����p
    while (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r')
        p++;	//���ָ��ָ���ֵΪ�հף���ָ������ƶ�һλ
    c->json = p;	//��pָ�븳���ṹ���е�ָ��json
}

static int lept_parse_null(lept_context* c, lept_value* v) {
    EXPECT(c, 'n');	//���ṹ���г���ָ��jsonָ���ֵ��һλ�Ƿ�Ϊn�����Ϊn��ָ������ƶ�һλ
    if (c->json[0] != 'u' || c->json[1] != 'l' || c->json[2] != 'l')
        return LEPT_PARSE_INVALID_VALUE;	//�������ָ��ָ���ֵ��Ϊnull���򷵻�LEPT_PARSE_INVALID_VALUE
    c->json += 3;	//jsonָ������ƶ���λ
    v->type = LEPT_NULL;	
	if (c->json[1] != '\0')
		return LEPT_PARSE_ROOT_NOT_SINGULAR;
    return LEPT_PARSE_OK;
}

static int lept_parse_true(lept_context* c, lept_value* v) {
	EXPECT(c, 't');	
	if (c->json[0] != 'r' || c->json[1] != 'u' || c->json[2] != 'e')
		return LEPT_PARSE_INVALID_VALUE;	
	c->json += 3;	//jsonָ������ƶ���λ
	v->type = LEPT_TRUE;
	if (c->json[1] != '\0')
		return LEPT_PARSE_ROOT_NOT_SINGULAR;
	return LEPT_PARSE_OK;
}

static int lept_parse_false(lept_context* c, lept_value* v) {
	EXPECT(c, 'f');
	if (c->json[0] != 'a' || c->json[1] != 'l' || c->json[2] != 's'||c->json[3]!='e')
		return LEPT_PARSE_INVALID_VALUE;
	c->json += 4;	//jsonָ������ƶ���λ
	v->type = LEPT_FALSE;
	if (c->json[1] != '\0')
		return LEPT_PARSE_ROOT_NOT_SINGULAR;
	return LEPT_PARSE_OK;
}

static int lept_parse_value(lept_context* c, lept_value* v) {	//�����ų����ַ�ָ��ṹ��ĵ�ַ��ָ��json�ṹ���ָ��
    switch (*(c->json)) {	//��ȡ�ṹ�����ַ�ָ���ֵ
        case 'n':  return lept_parse_null(c, v);	//������ַ�n����null�Ŀ�ͷ
		case 't':  return lept_parse_true(c, v);
		case 'f':  return lept_parse_false(c, v);
        case '\0': return LEPT_PARSE_EXPECT_VALUE;	//�����c����ַ����Ľ��������򷵻�LEPT_PARSE_EXPECT_VALUE(jsonֻ���пհ�)
        default:   return LEPT_PARSE_INVALID_VALUE;
    }
}

int lept_parse(lept_value* v, const char* json) { //v��ָ��һ��json�ṹ���ָ�룬json��һ��ָ�����ַ���ָ��
    lept_context c;	//c��һ���ṹ�壬�ڲ������һ��ָ�����ַ���ָ��
    assert(v != NULL);	//���v�Ƿ����
    c.json = json;	//������ĳ����ַ�ָ�븳��c�еĳ����ַ�ָ��
    v->type = LEPT_NULL;	//��v��ֵ��ΪNULL
    lept_parse_whitespace(&c);	//��c�еĳ����ַ�ָ����в�����ʹ�������հ�
    return lept_parse_value(&c, v);	//��c�н��������ĸ�ֵд��v
}

lept_type lept_get_type(const lept_value* v) {
    assert(v != NULL);
    return v->type;
}

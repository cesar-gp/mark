#include "out.h"
#include "assert.h"
#include "parser.h"

void test_consumestr() {
	out_start("consumestr");

	char* in = "[#][#][#]Consumer[#][#]Test[#]";
	char* inorigin = in;

	assert_uintequals(consumestr(&in, "[#]", 1, REPEATER), 1);
	assert_strequals(in, inorigin + 3);
	assert_uintequals(consumestr(&in, "[#]", 0, REPEATER), 2);
	assert_strequals(in, inorigin + 9);
	assert_uintequals(consumestr(&in, "[#]", 20, REPEATER), 0);
	assert_strequals(in, inorigin + 9);
	assert_uintequals(consumestr(&in, "[#]", 1, DELIMITER), 1);
	assert_strequals(in, inorigin + 20);
	assert_uintequals(consumestr(&in, "[#]", 0, DELIMITER), 2);
	assert_strequals(in, "");

	in = "REPREPR1RREP222REPREP";
	inorigin = in;

	assert_uintequals(consumestr(&in, "REP", 2, REPEATER), 2);
	assert_strequals(in, inorigin + 6);
	assert_uintequals(consumestr(&in, "1", 1, DELIMITER), 1);
	assert_strequals(in, inorigin + 8);
	assert_uintequals(consumestr(&in, "REP", 0, REPEATER), 0);
	assert_strequals(in, inorigin + 8);
	assert_uintequals(consumestr(&in, "REP", 1, REPEATER), 0);
	assert_strequals(in, inorigin + 8);
	assert_uintequals(consumestr(&in, "REP", 20, REPEATER), 0);
	assert_strequals(in, inorigin + 8);
	assert_uintequals(consumestr(&in, "2", 3, DELIMITER), 3);
	assert_strequals(in, inorigin + 15);
	assert_uintequals(consumestr(&in, "REP", 0, REPEATER), 2);
	assert_strequals(in, "");

	in = "1LLIMIT2LILIMIT3LIMLIMIT4LIMILIMIT5LIMITLIMIT";
	inorigin = in;

	assert_uintequals(consumestr(&in, "LIMIT", 1, DELIMITER), 1);
	assert_strequals(in, inorigin + 7);
	assert_uintequals(consumestr(&in, "LIMIT", 0, DELIMITER), 5);
	assert_strequals(in, "");

	in = "🃛理Ͷ|Uno🃛理Ͷ🃛理Ͷ|Dos🃛理Ͷ|Tres🃛理🃛理Ͷ|🃛理Ͷ|Cuatro🃛理Ͷ|Cinco🃛🃛理Ͷ|";
	inorigin = in;

	assert_uintequals(consumestr(&in, "🃛理Ͷ|", 2, DELIMITER), 2);
	assert_strequals(in, inorigin + 32);
	assert_uintequals(consumestr(&in, "Tres", 1, DELIMITER), 1);
	assert_strequals(in, inorigin + 49);
	assert_uintequals(consumestr(&in, "🃛理Ͷ|", 0, REPEATER), 0);
	assert_strequals(in, inorigin + 49);
	assert_uintequals(consumestr(&in, "🃛理", 1, REPEATER), 1);
	assert_strequals(in, inorigin + 56);
	assert_uintequals(consumestr(&in, "🃛理Ͷ|", 0, REPEATER), 2);
	assert_strequals(in, inorigin + 76);
	assert_uintequals(consumestr(&in, "🃛理Ͷ|", 0, DELIMITER), 2);
	assert_strequals(in, "");

	out_ok();
}
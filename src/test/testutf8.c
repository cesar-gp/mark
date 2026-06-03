#include "utf8.h"
#include "assert.h"
#include "out.h"

void test_utf8charlen() {
	out_start("utf8charlen");

	assert_uintequals(utf8charlen(""), 1);
	assert_uintequals(utf8charlen("a"), 1);
	assert_uintequals(utf8charlen("á"), 2);
	assert_uintequals(utf8charlen("鿿"), 3);
	assert_uintequals(utf8charlen("🇦"), 4);
	assert_uintequals(utf8charlen("\x88"), 0);
	assert_uintequals(utf8charlen("\xF8"), 0);
	assert_uintequals(utf8charlen("\xFF"), 0);

	out_ok();
}

void test_utf8chars() {
	out_start("utf8chars");

	uint32_t bytes;

	assert_uintequals(utf8chars("", &bytes), 0);
	assert_uintequals(bytes, 0);

	assert_uintequals(utf8chars("Test", &bytes), 4);
	assert_uintequals(bytes, 4);

	assert_uintequals(utf8chars("Árbol", &bytes), 5);
	assert_uintequals(bytes, 6);

	assert_uintequals(utf8chars("字test汉test", &bytes), 10);
	assert_uintequals(bytes, 14);

	assert_uintequals(utf8chars("\xC0\x80τ\0ξ", &bytes), 2);
	assert_uintequals(bytes, 4);

	assert_uintequals(utf8chars("\x95स्\xA0न\xB8", &bytes), 6);
	assert_uintequals(bytes, 12);

	assert_uintequals(utf8chars("Continuation\x80\0!", &bytes), 13);
	assert_uintequals(bytes, 13);

	assert_uintequals(utf8chars("First null test\xC0\0!", &bytes), 16);
	assert_uintequals(bytes, 16);

	assert_uintequals(utf8chars("Second null test\xE0\0!", &bytes), 17);
	assert_uintequals(bytes, 17);

	assert_uintequals(utf8chars("Third and last null test\xF0\0!", &bytes), 25);
	assert_uintequals(bytes, 25);

	out_ok();
}

void test_utf8decode() {
	out_start("utf8decode");

	char* str;
	uint8_t ret;
	uint32_t enc;
	uint8_t len;

	uint32_t rets[] = {	0, 1, 2, 3,
						1, 4, 4, 4,
						4, 5, 6, 0	};

	str =	"0", "\xFA", "\xC4", "\xC5\x80",
			"\x81", "\xC1\x80", "\xC0\x80", "\xE0\x80\x81",
			"\xF0\x80\x81\x82", "\xED\xA1", "\xF7\xBF\xBF\xBF", "𝄞";
	
	for(int i = 0; *str; str += len, i++) {
		ret = utf8decode(str, &enc, &len);
		assert_uintequals(ret, rets[i]);
	}

	uint32_t encs[] = {	0x0001F592, 0x000027A4,
						0x00000194, 0x00000024,
						0x00000020, 0x0000058D,
						0x00002928, 0x00012230	};

	str = "🖒➤Ɣ$ ֍⤨𒈰";
	for(int i = 0; *str; str += len, i++) {
		ret = utf8decode(str, &enc, &len);
		assert_uintequals(enc, encs[i]);
		assert_uintequals(ret, 0);
	}

	char lens[] = {	1, 2, 3, 4,
					2, 1, 4, 3,
					1, 3, 2, 4,
					3, 1, 4, 2,
					1, 4, 4, 1	};

	str = "Pπ✨🩷čf𓀅ࠄ?⊕˜🂡ও=𓇤ʘ.🌀🍣,";
	for(int i = 0; *str; str += len, i++) {
		ret = utf8decode(str, &enc, &len);
		assert_uintequals(len, lens[i]);
		assert_uintequals(ret, 0);
	}

	out_ok();
}
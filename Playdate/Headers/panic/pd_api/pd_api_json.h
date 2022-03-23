//
//  pdext_json.h
//  Playdate Simulator
//
//  Created by Dave Hayden on 10/6/17.
//  Copyright © 2017 Panic, Inc. All rights reserved.
//

#ifndef pdext_json_h
#define pdext_json_h

#if TARGET_EXTENSION

#include <stdlib.h>
#include <string.h>

typedef enum
{
	kJSONNull,
	kJSONTrue,
	kJSONFalse,
	kJSONInteger,
	kJSONFloat,
	kJSONString,
	kJSONArray,
	kJSONTable
} json_value_type;

typedef struct
{
	char type;
	
	union
	{
		int intval;
		float floatval;
		char* stringval;
		void* arrayval;
		void* tableval;
	} data;
} json_value;

static inline int json_intValue(json_value value) {
	switch (value.type) {
		case kJSONInteger: return value.data.intval;
		case kJSONFloat:   return (int)value.data.floatval;
		case kJSONString:  return (int)strtol(value.data.stringval, NULL, 10);
		case kJSONTrue:    return 1;
		default:           return 0;
	}
}

static inline float json_floatValue(json_value value) {
	switch (value.type) {
		case kJSONInteger: return (float)value.data.intval;
		case kJSONFloat:   return value.data.floatval;
		case kJSONString:  return 0; //strtof(value.data.stringval, NULL); XXX - required _strbk
		case kJSONTrue:    return 1.0;
		default:           return 0.0;
	}
}

static inline int json_boolValue(json_value value) {
	return value.type == kJSONString ? strcmp(value.data.stringval,"") != 0 : json_intValue(value);
}

static inline char* json_stringValue(json_value value) {
	return value.type == kJSONString ? value.data.stringval : NULL;
}


// decoder

typedef struct json_decoder json_decoder;

struct json_decoder
{
	void (*decodeError)(json_decoder* decoder, const char* error, int linenum);
	
	// the following functions are each optional
	void (*willDecodeSublist)(json_decoder* decoder, const char* name, json_value_type type);
	int (*shouldDecodeTableValueForKey)(json_decoder* decoder, const char* key);
	void (*didDecodeTableValue)(json_decoder* decoder, const char* key, json_value value);
	int (*shouldDecodeArrayValueAtIndex)(json_decoder* decoder, int pos);
	void (*didDecodeArrayValue)(json_decoder* decoder, int pos, json_value value); // if pos==0, this was a bare value at the root of the file
	void* (*didDecodeSublist)(json_decoder* decoder, const char* name, json_value_type type);
	
	void* userdata;
	int returnString; // when set, the decoder skips parsing and returns the current subtree as a string
	const char* path; // updated during parsing, reflects current position in tree
};

// convenience functions for setting up a table-only or array-only decoder

static inline void json_setTableDecode(json_decoder* decoder,
									   void (*willDecodeSublist)(json_decoder* decoder, const char* name, json_value_type type),
									   void (*didDecodeTableValue)(json_decoder* decoder, const char* key, json_value value),
									   void* (*didDecodeSublist)(json_decoder* decoder, const char* name, json_value_type type))
{
	decoder->didDecodeTableValue = didDecodeTableValue;
	decoder->didDecodeArrayValue = NULL;
	decoder->willDecodeSublist = willDecodeSublist;
	decoder->didDecodeSublist = didDecodeSublist;
}

static inline void json_setArrayDecode(json_decoder* decoder,
									   void (*willDecodeSublist)(json_decoder* decoder, const char* name, json_value_type type),
									   void (*didDecodeArrayValue)(json_decoder* decoder, int pos, json_value value),
									   void* (*didDecodeSublist)(json_decoder* decoder, const char* name, json_value_type type))
{
	decoder->didDecodeTableValue = NULL;
	decoder->didDecodeArrayValue = didDecodeArrayValue;
	decoder->willDecodeSublist = willDecodeSublist;
	decoder->didDecodeSublist = didDecodeSublist;
}

typedef struct
{
	int (*read)(void* userdata, uint8_t* buf, int bufsize); // fill buffer, return bytes written or -1 on end of data
	void* userdata; // passed back to the read function above
} json_reader;


// encoder

typedef void (writeFunc)(void* userdata, const char* str, int len);

typedef struct json_encoder
{
	writeFunc* writeStringFunc;
	void* userdata;
	
	int pretty : 1;
	int startedTable : 1;
	int startedArray : 1;
	int depth : 29;
	
	void (*startArray)(struct json_encoder* encoder);
	void (*addArrayMember)(struct json_encoder* encoder);
	void (*endArray)(struct json_encoder* encoder);
	void (*startTable)(struct json_encoder* encoder);
	void (*addTableMember)(struct json_encoder* encoder, const char* name, int len);
	void (*endTable)(struct json_encoder* encoder);
	void (*writeNull)(struct json_encoder* encoder);
	void (*writeFalse)(struct json_encoder* encoder);
	void (*writeTrue)(struct json_encoder* encoder);
	void (*writeInt)(struct json_encoder* encoder, int num);
	void (*writeDouble)(struct json_encoder* encoder, double num);
	void (*writeString)(struct json_encoder* encoder, const char* str, int len);
	
} json_encoder;

#endif

struct playdate_json
{
	void (*initEncoder)(json_encoder* encoder, writeFunc* write, void* userdata, int pretty);

	int (*decode)(struct json_decoder* functions, json_reader reader, json_value* outval);
	int (*decodeString)(struct json_decoder* functions, const char* jsonString, json_value* outval);
};

#endif /* pdext_json_h */

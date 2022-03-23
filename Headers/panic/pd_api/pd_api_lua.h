//
//  pdext_lua.h
//  Playdate Simulator
//
//  Created by Dave Hayden on 10/6/17.
//  Copyright © 2017 Panic, Inc. All rights reserved.
//

#ifndef pdext_lua_h
#define pdext_lua_h

#if TARGET_EXTENSION

typedef void* lua_State;
typedef int (*lua_CFunction)(lua_State* L);

typedef struct luaL_Val
{
	const char *name;
	enum { kInt, kFloat, kStr } type;
	union
	{
		unsigned int intval;
		float floatval;
		const char* strval;
	} v;
} luaL_Val;

typedef struct LuaUDObject LuaUDObject;
typedef struct LCDSprite LCDSprite;

#endif

typedef struct
{
	const char *name;
	lua_CFunction func;
} lua_reg;

enum LuaType
{
	kTypeNil,
	kTypeBool,
	kTypeInt,
	kTypeFloat,
	kTypeString,
	kTypeTable,
	kTypeFunction,
	kTypeThread,
	kTypeObject
};

struct playdate_lua
{
	// these two return 1 on success, else 0 with an error message in outErr
	int (*addFunction)(lua_CFunction f, const char* name, const char** outErr);
	int (*registerClass)(const char* name, const lua_reg* reg, const luaL_Val* vals, int isstatic, const char** outErr);

	void (*pushFunction)(lua_CFunction f);
	int (*indexMetatable)(void);

	void (*stop)(void);
	void (*start)(void);
	
	// stack operations
	int (*getArgCount)(void);
	enum LuaType (*getArgType)(int pos, const char** outClass);

	int (*argIsNil)(int pos);
	int (*getArgBool)(int pos);
	int (*getArgInt)(int pos);
	float (*getArgFloat)(int pos);
	const char* (*getArgString)(int pos);
	const char* (*getArgBytes)(int pos, size_t* outlen);
	void* (*getArgObject)(int pos, char* type, LuaUDObject** outud);
	
	LCDBitmap* (*getBitmap)(int pos);
	LCDSprite* (*getSprite)(int pos);

	// for returning values back to Lua
	void (*pushNil)(void);
	void (*pushBool)(int val);
	void (*pushInt)(int val);
	void (*pushFloat)(float val);
	void (*pushString)(const char* str);
	void (*pushBytes)(const char* str, size_t len);
	void (*pushBitmap)(LCDBitmap* bitmap);
	void (*pushSprite)(LCDSprite* sprite);
	
	LuaUDObject* (*pushObject)(void* obj, char* type, int nValues);
	LuaUDObject* (*retainObject)(LuaUDObject* obj);
	void (*releaseObject)(LuaUDObject* obj);
	
	void (*setObjectValue)(LuaUDObject* obj, int slot); // sets item on top of stack and pops it
	int (*getObjectValue)(LuaUDObject* obj, int slot); // pushes item at slot to top of stack, returns stack position

	// calling lua from C has some overhead. use sparingly!
	void (*callFunction_deprecated)(const char* name, int nargs);
	int (*callFunction)(const char* name, int nargs, const char** outerr);
};

#endif /* pdext_lua_h */

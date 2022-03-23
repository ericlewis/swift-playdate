//
//  pd_api.h
//  Playdate C API
//
//  Created by Dave Hayden on 7/30/14.
//  Copyright (c) 2014 Panic, Inc. All rights reserved.
//

#ifndef PLAYDATEAPI_H
#define PLAYDATEAPI_H

#include <stdint.h>
#include <stdlib.h>

#include "pd_api/pd_api_gfx.h"
#include "pd_api/pd_api_sys.h"
#include "pd_api/pd_api_lua.h"
#include "pd_api/pd_api_json.h"
#include "pd_api/pd_api_file.h"
#include "pd_api/pd_api_sprite.h"
#include "pd_api/pd_api_sound.h"
#include "pd_api/pd_api_display.h"
#include "pd_api/pd_api_scoreboards.h"

typedef struct PlaydateAPI PlaydateAPI;

struct PlaydateAPI
{
	const struct playdate_sys* system;
	const struct playdate_file* file;
	const struct playdate_graphics* graphics;
	const struct playdate_sprite* sprite;
	const struct playdate_display* display;
	const struct playdate_sound* sound;
	const struct playdate_lua* lua;
	const struct playdate_json* json;
	const struct playdate_scoreboards* scoreboards;
};

#if TARGET_EXTENSION
typedef enum
{
	kEventInit,
	kEventInitLua,
	kEventLock,
	kEventUnlock,
	kEventPause,
	kEventResume,
	kEventTerminate,
	kEventKeyPressed, // arg is keycode
	kEventKeyReleased,
	kEventLowPower
} PDSystemEvent;
#endif

// main entry point defined in game code, called for the above events

extern PlaydateAPI* pd;

#ifdef _WINDLL
__declspec(dllexport)
#endif
int eventHandler(PlaydateAPI* playdate, PDSystemEvent event, uint32_t arg);

#endif // PLAYDATEAPI_H

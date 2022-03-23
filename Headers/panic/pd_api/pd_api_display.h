//
//  pdext_display.h
//  Playdate Simulator
//
//  Created by Dave Hayden on 10/6/17.
//  Copyright © 2017 Panic, Inc. All rights reserved.
//

#ifndef pdext_display_h
#define pdext_display_h

struct playdate_display
{
	int (*getWidth)(void);
	int (*getHeight)(void);
	
	void (*setRefreshRate)(float rate);

	void (*setInverted)(int flag);
	void (*setScale)(unsigned int s);
	void (*setMosaic)(unsigned int x, unsigned int y);
	void (*setFlipped)(int x, int y);
	void (*setOffset)(int x, int y);
};

#endif /* pdext_display_h */

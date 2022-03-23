//
//  pdext_gfx.h
//  Playdate Simulator
//
//  Created by Dave Hayden on 10/6/17.
//  Copyright © 2017 Panic, Inc. All rights reserved.
//

#ifndef pdext_sprite_h
#define pdext_sprite_h

#if TARGET_EXTENSION

typedef enum {kCollisionTypeSlide, kCollisionTypeFreeze, kCollisionTypeOverlap, kCollisionTypeBounce} SpriteCollisionResponseType;

typedef struct
{
	float x;
	float y;
	float width;
	float height;
	
} PDRect;

static inline PDRect PDRectMake(float x, float y, float width, float height)
{
	return (PDRect){ .x = x, .y = y, .width = width, .height = height };
}

#endif

typedef struct {
	float x;
	float y;
} CollisionPoint;

typedef struct {
	int x;
	int y;
} CollisionVector;

typedef struct SpriteCollisionInfo SpriteCollisionInfo;
struct SpriteCollisionInfo
{
	LCDSprite *sprite;		// The sprite being moved
	LCDSprite *other;		// The sprite colliding with the sprite being moved
	SpriteCollisionResponseType responseType;	// The result of collisionResponse
	uint8_t overlaps;		// True if the sprite was overlapping other when the collision started. False if it didn’t overlap but tunneled through other.
	float ti;				// A number between 0 and 1 indicating how far along the movement to the goal the collision occurred
	CollisionPoint move;	// The difference between the original coordinates and the actual ones when the collision happened
	CollisionVector normal;	// The collision normal; usually -1, 0, or 1 in x and y. Use this value to determine things like if your character is touching the ground.
	CollisionPoint touch;	// The coordinates where the sprite started touching other
	PDRect spriteRect;		// The rectangle the sprite occupied when the touch happened
	PDRect otherRect; 		// The rectangle the sprite being collided with occupied when the touch happened
};

typedef struct SpriteQueryInfo SpriteQueryInfo;
struct SpriteQueryInfo
{
	LCDSprite *sprite;			// The sprite being intersected by the segment
								// ti1 and ti2 are numbers between 0 and 1 which indicate how far from the starting point of the line segment the collision happened
	float ti1;					// entry point
	float ti2;					// exit point
	CollisionPoint entryPoint;	// The coordinates of the first intersection between sprite and the line segment
	CollisionPoint exitPoint;	// The coordinates of the second intersection between sprite and the line segment
};

typedef struct LCDSprite LCDSprite;
typedef struct CWCollisionInfo CWCollisionInfo;
typedef struct CWItemInfo CWItemInfo;
typedef void LCDSpriteDrawFunction(LCDSprite* sprite, PDRect bounds, PDRect drawrect);
typedef void LCDSpriteUpdateFunction(LCDSprite* sprite);
typedef SpriteCollisionResponseType LCDSpriteCollisionFilterProc(LCDSprite* sprite, LCDSprite* other);

struct playdate_sprite
{
	void (*setAlwaysRedraw)(int flag);
	void (*addDirtyRect)(LCDRect dirtyRect);
	void (*drawSprites)(void);
	void (*updateAndDrawSprites)(void);

	LCDSprite* (*newSprite)(void);
	void (*freeSprite)(LCDSprite *sprite);
	LCDSprite* (*copy)(LCDSprite *sprite);
	
	void (*addSprite)(LCDSprite *sprite);
	void (*removeSprite)(LCDSprite *sprite);
	void (*removeSprites)(LCDSprite **sprites, int count);
	void (*removeAllSprites)(void);
	int (*getSpriteCount)(void);
	
	void (*setBounds)(LCDSprite *sprite, PDRect bounds);
	PDRect (*getBounds)(LCDSprite *sprite);
	void (*moveTo)(LCDSprite *sprite, float x, float y);
	void (*moveBy)(LCDSprite *sprite, float dx, float dy);

	void (*setImage)(LCDSprite *sprite, LCDBitmap *image, LCDBitmapFlip flip);
	LCDBitmap* (*getImage)(LCDSprite *sprite);
	void (*setSize)(LCDSprite *s, float width, float height);
	void (*setZIndex)(LCDSprite *sprite, int16_t zIndex);
	int16_t (*getZIndex)(LCDSprite *sprite);
	
	void (*setDrawMode)(LCDSprite *sprite, LCDBitmapDrawMode mode);
	void (*setImageFlip)(LCDSprite *sprite, LCDBitmapFlip flip);
	LCDBitmapFlip (*getImageFlip)(LCDSprite *sprite);
	void (*setStencil)(LCDSprite *sprite, LCDBitmap* stencil);
	
	void (*setClipRect)(LCDSprite *sprite, LCDRect clipRect);
	void (*clearClipRect)(LCDSprite *sprite);
	void (*setClipRectsInRange)(LCDRect clipRect, int startZ, int endZ);
	void (*clearClipRectsInRange)(int startZ, int endZ);
	
	void (*setUpdatesEnabled)(LCDSprite *sprite, int flag);
	int (*updatesEnabled)(LCDSprite *sprite);
	void (*setCollisionsEnabled)(LCDSprite *sprite, int flag);
	int (*collisionsEnabled)(LCDSprite *sprite);
	void (*setVisible)(LCDSprite *sprite, int flag);
	int (*isVisible)(LCDSprite *sprite);
	void (*setOpaque)(LCDSprite *sprite, int flag);
	void (*markDirty)(LCDSprite *sprite);
	
	void (*setTag)(LCDSprite *sprite, uint8_t tag);
	uint8_t (*getTag)(LCDSprite *sprite);
	
	void (*setIgnoresDrawOffset)(LCDSprite *sprite, int flag);
	
	void (*setUpdateFunction)(LCDSprite *sprite, LCDSpriteUpdateFunction *func);
	void (*setDrawFunction)(LCDSprite *sprite, LCDSpriteDrawFunction *func);
	
	void (*getPosition)(LCDSprite *sprite, float *x, float *y);

	// Collisions
	void (*resetCollisionWorld)(void);
	
	void (*setCollideRect)(LCDSprite *sprite, PDRect collideRect);
	PDRect (*getCollideRect)(LCDSprite *sprite);
	void (*clearCollideRect)(LCDSprite *sprite);
	
	// caller is responsible for freeing the returned array for all collision methods
	void (*setCollisionResponseFunction)(LCDSprite *sprite, LCDSpriteCollisionFilterProc *func);
	SpriteCollisionInfo* (*checkCollisions)(LCDSprite *sprite, float goalX, float goalY, float *actualX, float *actualY, int *len);			// access results using SpriteCollisionInfo *info = &results[i];
	SpriteCollisionInfo* (*moveWithCollisions)(LCDSprite *sprite, float goalX, float goalY, float *actualX, float *actualY, int *len);
	LCDSprite** (*querySpritesAtPoint)(float x, float y, int *len);
	LCDSprite** (*querySpritesInRect)(float x, float y, float width, float height, int *len);
	LCDSprite** (*querySpritesAlongLine)(float x1, float y1, float x2, float y2, int *len);
	SpriteQueryInfo* (*querySpriteInfoAlongLine)(float x1, float y1, float x2, float y2, int *len);											// access results using SpriteQueryInfo *info = &results[i];
	LCDSprite** (*overlappingSprites)(LCDSprite *sprite, int *len);
	LCDSprite** (*allOverlappingSprites)(int *len);

	// added in 1.7
	void (*setStencilPattern)(LCDSprite* sprite, uint8_t pattern[8]);
	void (*clearStencil)(LCDSprite* sprite);

	void (*setUserdata)(LCDSprite* sprite, void* userdata);
	void* (*getUserdata)(LCDSprite* sprite);
};

#endif /* pdext_sprite_h */

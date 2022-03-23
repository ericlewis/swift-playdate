//
//  pdext_gfx.h
//  Playdate Simulator
//
//  Created by Dave Hayden on 10/6/17.
//  Copyright © 2017 Panic, Inc. All rights reserved.
//

#ifndef pdext_gfx_h
#define pdext_gfx_h

#if TARGET_EXTENSION

typedef struct
{
	int left;
	int right; // not inclusive
	int top;
	int bottom; // not inclusive
} LCDRect;

static inline LCDRect LCDMakeRect(int x, int y, int width, int height)
{
	// XXX - assumes width and height are positive
	LCDRect r = { .left = x, .right = x + width, .top = y, .bottom = y + height };
	return r;
}

static inline LCDRect LCDRect_translate(LCDRect r, int dx, int dy)
{
	return (LCDRect){ .left = r.left + dx, .right = r.right + dx, .top = r.top + dy, .bottom = r.bottom + dy };
}

#define LCD_COLUMNS	400
#define LCD_ROWS	240
#define LCD_ROWSIZE 52
#define LCD_SCREEN_RECT LCDMakeRect(0,0,LCD_COLUMNS,LCD_ROWS)

typedef enum
{
	kDrawModeCopy,
	kDrawModeWhiteTransparent,
	kDrawModeBlackTransparent,
	kDrawModeFillWhite,
	kDrawModeFillBlack,
	kDrawModeXOR,
	kDrawModeNXOR,
	kDrawModeInverted
} LCDBitmapDrawMode;

typedef enum
{
	kBitmapUnflipped,
	kBitmapFlippedX,
	kBitmapFlippedY,
	kBitmapFlippedXY
} LCDBitmapFlip;

typedef enum
{
	kColorBlack,
	kColorWhite,
	kColorClear,
	kColorXOR
} LCDSolidColor;

typedef enum
{
	kLineCapStyleButt,
	kLineCapStyleSquare,
	kLineCapStyleRound
} LCDLineCapStyle;

typedef enum
{
	kLCDFontLanguageEnglish,
	kLCDFontLanguageJapanese,
	kLCDFontLanguageUnknown,
} LCDFontLanguage;

typedef enum
{
	kASCIIEncoding,
	kUTF8Encoding,
	k16BitLEEncoding
} PDStringEncoding;

typedef uint8_t LCDPattern[16]; // 8x8 pattern: 8 rows image data, 8 rows mask
typedef uintptr_t LCDColor; // LCDSolidColor or pointer to LCDPattern

typedef enum
{
	kPolygonFillNonZero,
	kPolygonFillEvenOdd
} LCDPolygonFillRule;

#endif

typedef struct LCDBitmap LCDBitmap;
typedef struct LCDBitmapTable LCDBitmapTable;
typedef struct LCDFont LCDFont;
typedef struct LCDFontPage LCDFontPage;
typedef struct LCDFontGlyph LCDFontGlyph;
typedef struct LCDVideoPlayer LCDVideoPlayer;

struct playdate_video
{
	LCDVideoPlayer* (*loadVideo)(const char* path);
	void (*freePlayer)(LCDVideoPlayer* p);
	int (*setContext)(LCDVideoPlayer* p, LCDBitmap* context);
	void (*useScreenContext)(LCDVideoPlayer* p);
	int (*renderFrame)(LCDVideoPlayer* p, int n);
	const char* (*getError)(LCDVideoPlayer* p);
	void (*getInfo)(LCDVideoPlayer* p, int* outWidth, int* outHeight, float* outFrameRate, int* outFrameCount, int* outCurrentFrame);
	LCDBitmap* (*getContext)(LCDVideoPlayer *p);
};

struct playdate_graphics
{
	const struct playdate_video* video;

	// Drawing Functions
	void (*clear)(LCDColor color);
	void (*setBackgroundColor)(LCDSolidColor color);
	void (*setStencil)(LCDBitmap* stencil);
	void (*setDrawMode)(LCDBitmapDrawMode mode);
	void (*setDrawOffset)(int dx, int dy);
	void (*setClipRect)(int x, int y, int width, int height);
	void (*clearClipRect)(void);
	void (*setLineCapStyle)(LCDLineCapStyle endCapStyle);
	void (*setFont)(LCDFont* font);
	void (*setTextTracking)(int tracking);
	void (*pushContext)(LCDBitmap* target);
	void (*popContext)(void);
	
	void (*drawBitmap)(LCDBitmap* bitmap, int x, int y, LCDBitmapFlip flip);
	void (*tileBitmap)(LCDBitmap* bitmap, int x, int y, int width, int height, LCDBitmapFlip flip);
	void (*drawLine)(int x1, int y1, int x2, int y2, int width, LCDColor color);
	void (*fillTriangle)(int x1, int y1, int x2, int y2, int x3, int y3, LCDColor color);
	void (*drawRect)(int x, int y, int width, int height, LCDColor color);
	void (*fillRect)(int x, int y, int width, int height, LCDColor color);
	void (*drawEllipse)(int x, int y, int width, int height, int lineWidth, float startAngle, float endAngle, LCDColor color); // stroked inside the rect
	void (*fillEllipse)(int x, int y, int width, int height, float startAngle, float endAngle, LCDColor color);
	void (*drawScaledBitmap)(LCDBitmap* bitmap, int x, int y, float xscale, float yscale);
	int  (*drawText)(const void* text, size_t len, PDStringEncoding encoding, int x, int y);

	// LCDBitmap
	LCDBitmap* (*newBitmap)(int width, int height, LCDColor bgcolor);
	void (*freeBitmap)(LCDBitmap*);
	LCDBitmap* (*loadBitmap)(const char* path, const char** outerr);
	LCDBitmap* (*copyBitmap)(LCDBitmap* bitmap);
	void (*loadIntoBitmap)(const char* path, LCDBitmap* bitmap, const char** outerr);
	void (*getBitmapData)(LCDBitmap* bitmap, int* width, int* height, int* rowbytes, int* hasmask, uint8_t** data);
	void (*clearBitmap)(LCDBitmap* bitmap, LCDColor bgcolor);
	LCDBitmap* (*rotatedBitmap)(LCDBitmap* bitmap, float rotation, float xscale, float yscale, int* allocedSize);

	// LCDBitmapTable
	LCDBitmapTable* (*newBitmapTable)(int count, int width, int height);
	void (*freeBitmapTable)(LCDBitmapTable* table);
	LCDBitmapTable* (*loadBitmapTable)(const char* path, const char** outerr);
	void (*loadIntoBitmapTable)(const char* path, LCDBitmapTable* table, const char** outerr);
	LCDBitmap* (*getTableBitmap)(LCDBitmapTable* table, int idx);

	// LCDFont
	LCDFont* (*loadFont)(const char* path, const char** outErr);
	LCDFontPage* (*getFontPage)(LCDFont* font, uint32_t c);
	LCDFontGlyph* (*getPageGlyph)(LCDFontPage* page, uint32_t c, LCDBitmap** bitmap, int* advance);
	int (*getGlyphKerning)(LCDFontGlyph* glyph, uint32_t glyphcode, uint32_t nextcode);
	int (*getTextWidth)(LCDFont* font, const void* text, size_t len, PDStringEncoding encoding, int tracking);

	// raw framebuffer access
	uint8_t* (*getFrame)(void); // row stride = LCD_ROWSIZE
	uint8_t* (*getDisplayFrame)(void); // row stride = LCD_ROWSIZE
	LCDBitmap* (*getDebugBitmap)(void); // valid in simulator only, function is NULL on device
	LCDBitmap* (*copyFrameBufferBitmap)(void);
	void (*markUpdatedRows)(int start, int end);
	void (*display)(void);

	// misc util.
	void (*setColorToPattern)(LCDColor* color, LCDBitmap* bitmap, int x, int y);
	int (*checkMaskCollision)(LCDBitmap* bitmap1, int x1, int y1, LCDBitmapFlip flip1, LCDBitmap* bitmap2, int x2, int y2, LCDBitmapFlip flip2, LCDRect rect);
	
	// 1.1
	void (*setScreenClipRect)(int x, int y, int width, int height);
	
	// 1.1.1
	void (*fillPolygon)(int nPoints, int* coords, LCDColor color, LCDPolygonFillRule fillrule);
	uint8_t (*getFontHeight)(LCDFont* font);
	
	// 1.7
	LCDBitmap* (*getDisplayBufferBitmap)(void);
	void (*drawRotatedBitmap)(LCDBitmap* bitmap, int x, int y, float rotation, float centerx, float centery, float xscale, float yscale);
	void (*setTextLeading)(int lineHeightAdustment);

	// 1.8
	int (*setBitmapMask)(LCDBitmap* bitmap, LCDBitmap* mask);
	LCDBitmap* (*getBitmapMask)(LCDBitmap* bitmap);
};

#endif /* pdext_gfx_h */


#ifndef pdext_scoreboards_h
#define pdext_scoreboards_h

#include <stdint.h>

typedef struct {
	uint32_t rank;
	uint32_t value;
	char *player;
} PDScore;

typedef struct {
	char *boardID;
	unsigned int count;
	uint32_t lastUpdated;
	int playerIncluded;
	unsigned int limit;
	PDScore *scores;
} PDScoresList;

typedef struct {
	char *boardID;
	char *name;
} PDBoard;

typedef struct {
	unsigned int count;
	uint32_t lastUpdated;
	PDBoard *boards;
} PDBoardsList;

typedef void (*AddScoreCallback)(PDScore *score, const char* errorMessage);
typedef void (*PersonalBestCallback)(PDScore *score, const char *errorMessage);
typedef void (*BoardsListCallback)(PDBoardsList *boards, const char *errorMessage);
typedef void (*ScoresCallback)(PDScoresList *scores, const char *errorMessage);

struct playdate_scoreboards
{
	int (*addScore)(const char *boardId, uint32_t value, AddScoreCallback callback);
	int (*getPersonalBest)(const char *boardId, PersonalBestCallback callback);
	void (*freeScore)(PDScore *score);

	int (*getScoreboards)(BoardsListCallback callback);
	void (*freeBoardsList)(PDBoardsList *boardsList);

	int (*getScores)(const char *boardId, ScoresCallback callback);
	void (*freeScoresList)(PDScoresList *scoresList);
};

#endif /* pdext_scoreboards_h */

#include "mineshaft.h"

#include <stdio.h>
#include <string.h>

#include "piece.h"

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

static int isUnderwaterCaveBiome(int biome) {
    return isDeepOcean(biome) || biome == frozen_ocean;
}

typedef enum {
    NORTH = 0,
    EAST  = 1,
    SOUTH = 2,
    WEST  = 3
} Direction;

STRUCT(MineshaftPieceEnv) {
    int mc;
    Piece *list;
    int *n;
    uint64_t *rng;
    int nmax;
};

static const char* dirName(Direction d) {
    switch (d) {
    case NORTH: return "NORTH";
    case EAST:  return "EAST";
    case SOUTH: return "SOUTH";
    case WEST:  return "WEST";
    default:    return "UNKNOWN";
    }
}

static inline Piece* hasCollision(MineshaftPieceEnv *env, Pos3 b0, Pos3 b1) {
    int i, n = *env->n;
    for (i = 0; i < n; i++) {
        Piece *q = env->list + i;
        if (hasIntersection(q->bb0, q->bb1, b0, b1)) {
            return q;
        }
    }
    return NULL;
}

static void extendMineshaftPiece(MineshaftPieceEnv *env, Piece *piece);

static void extendMineshaft(MineshaftPieceEnv *env, int x, int y, int z, Direction facing, int depth) {
    if (depth > 8) return;
    depth += 1;

    if (IABS(x - env->list->bb0.x) > 80 || IABS(z - env->list->bb0.z) > 80) return;

    int randomSelection = nextInt(env->rng, 100);
    if (randomSelection >= 80) {
        int y1 = 2 + 4 * (nextInt(env->rng, 4) == 0);
        Pos3 bb0, bb1;
        switch (facing) {
        case NORTH: bb0 = (Pos3) {x + -1, y, z + -4}; bb1 = (Pos3) {x + 3, y + y1, z + 0}; break;
        case EAST:  bb0 = (Pos3) {x + 0, y, z + -1}; bb1 = (Pos3) {x + 4, y + y1, z + 3}; break;
        case SOUTH: bb0 = (Pos3) {x + -1, y, z + 0}; bb1 = (Pos3) {x + 3, y + y1, z + 4}; break;
        case WEST:  bb0 = (Pos3) {x + -4, y, z + -1}; bb1 = (Pos3) {x + 0, y + y1, z + 3}; break;
        default: UNREACHABLE();
        }
        if (!hasCollision(env, bb0, bb1)) {
            Piece *p = env->list + (*env->n)++;
            p->name = "MSCrossing";
            p->pos = bb0;
            p->bb0 = bb0;
            p->bb1 = bb1;
            p->rot = facing;
            p->depth = depth;
            p->type = MS_CROSSING;
            p->next = NULL;
            extendMineshaftPiece(env, p);
            return;
        }
    } else if (randomSelection >= 70) {
        Pos3 bb0, bb1;
        switch (facing) {
        case NORTH: bb0 = (Pos3) {x + 0, y + -5, z + -8}; bb1 = (Pos3) {x + 2, y + 2, z + 0}; break;
        case EAST:  bb0 = (Pos3) {x + 0, y + -5, z + 0}; bb1 = (Pos3) {x + 8, y + 2, z + 2}; break;
        case SOUTH: bb0 = (Pos3) {x + 0, y + -5, z + 0}; bb1 = (Pos3) {x + 2, y + 2, z + 8}; break;
        case WEST:  bb0 = (Pos3) {x + -8, y + -5, z + 0}; bb1 = (Pos3) {x + 0, y + 2, z + 2}; break;
        default: UNREACHABLE();
        }
        if (!hasCollision(env, bb0, bb1)) {
            Piece *p = env->list + (*env->n)++;
            p->name = "MSStairs";
            p->pos = bb0;
            p->bb0 = bb0;
            p->bb1 = bb1;
            p->rot = facing;
            p->depth = depth;
            p->type = MS_STAIRS;
            p->next = NULL;
            extendMineshaftPiece(env, p);
            return;
        }
    } else {
        for (int corridorLength = nextInt(env->rng, 3) + 2; corridorLength > 0; corridorLength--) {
            int blockLength = corridorLength * 5;

            Pos3 bb0, bb1;
            switch (facing) {
            case NORTH: bb0 = (Pos3) {x + 0, y, z + - (blockLength - 1)}; bb1 = (Pos3) {x + 2, y + 2, z + 0}; break;
            case EAST:  bb0 = (Pos3) {x + 0, y, z + 0}; bb1 = (Pos3) {x + blockLength - 1, y + 2, z + 2}; break;
            case SOUTH: bb0 = (Pos3) {x + 0, y, z + 0}; bb1 = (Pos3) {x + 2, y + 2, z + blockLength - 1}; break;
            case WEST:  bb0 = (Pos3) {x + - (blockLength - 1), y, z + 0}; bb1 = (Pos3) {x + 0, y + 2, z + 2}; break;
            default: UNREACHABLE();
            }
            if (!hasCollision(env, bb0, bb1)) {
                Piece *p = env->list + (*env->n)++;
                p->name = "MSCorridor";
                p->pos = bb0;
                p->bb0 = bb0;
                p->bb1 = bb1;
                p->rot = facing;
                p->depth = depth;
                p->type = MS_CORRIDOR;
                p->next = NULL;

                int hasRails = nextInt(env->rng, 3) == 0;
                p->additionalData |= hasRails << 0;
                p->additionalData |= (!hasRails && nextInt(env->rng, 23) == 0) << 1; // spiderCorridor

                extendMineshaftPiece(env, p);
                return;
            }
        } // TODO check corridorLength > 0 check in older versions
    }
}

static void extendMineshaftPiece(MineshaftPieceEnv *env, Piece *piece) { //add children
    if (*env->n >= env->nmax) {
        return;
    }
    switch (piece->type) {
    case MS_CORRIDOR: {
        int endSelection = nextInt(env->rng, 4);
        Direction rot = piece->rot;
        switch (rot) {
        case NORTH:
            if (endSelection <= 1) {
                extendMineshaft(env, piece->bb0.x, piece->bb0.y - 1 + nextInt(env->rng, 3), piece->bb0.z - 1, NORTH, piece->depth);
            } else if (endSelection == 2) {
                extendMineshaft(env, piece->bb0.x - 1, piece->bb0.y - 1 + nextInt(env->rng, 3), piece->bb0.z, WEST, piece->depth);
            } else {
                extendMineshaft(env, piece->bb1.x + 1, piece->bb0.y - 1 + nextInt(env->rng, 3), piece->bb0.z, EAST, piece->depth);
            }
            break;
        case EAST:
            if (endSelection <= 1) {
                extendMineshaft(env, piece->bb1.x + 1, piece->bb0.y - 1 + nextInt(env->rng, 3), piece->bb0.z, EAST, piece->depth);
            } else if (endSelection == 2) {
                extendMineshaft(env, piece->bb1.x - 3, piece->bb0.y - 1 + nextInt(env->rng, 3), piece->bb0.z - 1, NORTH, piece->depth);
            } else {
                extendMineshaft(env, piece->bb1.x - 3, piece->bb0.y - 1 + nextInt(env->rng, 3), piece->bb1.z + 1, SOUTH, piece->depth);
            }
            break;
        case SOUTH:
            if (endSelection <= 1) {
                extendMineshaft(env, piece->bb0.x, piece->bb0.y - 1 + nextInt(env->rng, 3), piece->bb1.z + 1, SOUTH, piece->depth);
            } else if (endSelection == 2) {
                extendMineshaft(env, piece->bb0.x - 1, piece->bb0.y - 1 + nextInt(env->rng, 3), piece->bb1.z - 3, WEST, piece->depth);
            } else {
                extendMineshaft(env, piece->bb1.x + 1, piece->bb0.y - 1 + nextInt(env->rng, 3), piece->bb1.z - 3, EAST, piece->depth);
            }
            break;
        case WEST:
            if (endSelection <= 1) {
                extendMineshaft(env, piece->bb0.x - 1, piece->bb0.y - 1 + nextInt(env->rng, 3), piece->bb0.z, WEST, piece->depth);
            } else if (endSelection == 2) {
                extendMineshaft(env, piece->bb0.x, piece->bb0.y - 1 + nextInt(env->rng, 3), piece->bb0.z - 1, NORTH, piece->depth);
            } else {
                extendMineshaft(env, piece->bb0.x, piece->bb0.y - 1 + nextInt(env->rng, 3), piece->bb1.z + 1, SOUTH, piece->depth);
            }
            break;
        default: UNREACHABLE();
        }

        if (piece->depth >= 8) {
            break;
        }
        if (rot != NORTH && rot != SOUTH) {
            for (int x = piece->bb0.x + 3; x + 3 <= piece->bb1.x; x += 5) {
                int selection = nextInt(env->rng, 5);
                if (selection == 0) {
                    extendMineshaft(env, x, piece->bb0.y, piece->bb0.z - 1, NORTH, piece->depth + 1);
                } else if (selection == 1) {
                    extendMineshaft(env, x, piece->bb0.y, piece->bb1.z + 1, SOUTH, piece->depth + 1);
                }
            }
        } else {
            for (int z = piece->bb0.z + 3; z + 3 <= piece->bb1.z; z += 5) {
                int selection = nextInt(env->rng, 5);
                if (selection == 0) {
                    extendMineshaft(env, piece->bb0.x - 1, piece->bb0.y, z, WEST, piece->depth + 1);
                } else if (selection == 1) {
                    extendMineshaft(env, piece->bb1.x + 1, piece->bb0.y, z, EAST, piece->depth + 1);
                }
            }
        }
        break;
    }
    case MS_CROSSING: {
        switch ((Direction)piece->rot) {
        case NORTH:
            extendMineshaft(env, piece->bb0.x + 1, piece->bb0.y, piece->bb0.z - 1, NORTH, piece->depth);
            extendMineshaft(env, piece->bb0.x - 1, piece->bb0.y, piece->bb0.z + 1, WEST,  piece->depth);
            extendMineshaft(env, piece->bb1.x + 1, piece->bb0.y, piece->bb0.z + 1, EAST,  piece->depth);
            break;
        case EAST:
            extendMineshaft(env, piece->bb0.x + 1, piece->bb0.y, piece->bb0.z - 1, NORTH, piece->depth);
            extendMineshaft(env, piece->bb0.x + 1, piece->bb0.y, piece->bb1.z + 1, SOUTH, piece->depth);
            extendMineshaft(env, piece->bb1.x + 1, piece->bb0.y, piece->bb0.z + 1, EAST,  piece->depth);
            break;
        case SOUTH:
            extendMineshaft(env, piece->bb0.x + 1, piece->bb0.y, piece->bb1.z + 1, SOUTH, piece->depth);
            extendMineshaft(env, piece->bb0.x - 1, piece->bb0.y, piece->bb0.z + 1, WEST,  piece->depth);
            extendMineshaft(env, piece->bb1.x + 1, piece->bb0.y, piece->bb0.z + 1, EAST,  piece->depth);
            break;
        case WEST:
            extendMineshaft(env, piece->bb0.x + 1, piece->bb0.y, piece->bb0.z - 1, NORTH, piece->depth);
            extendMineshaft(env, piece->bb0.x + 1, piece->bb0.y, piece->bb1.z + 1, SOUTH, piece->depth);
            extendMineshaft(env, piece->bb0.x - 1, piece->bb0.y, piece->bb0.z + 1, WEST,  piece->depth);
            break;
        default: UNREACHABLE();
        }

        int isTwoFloored = piece->bb1.y - piece->bb0.y + 1 > 3;
        if (!isTwoFloored) {
            break;
        }
        if (next(env->rng, 1)) {
            extendMineshaft(env, piece->bb0.x + 1, piece->bb0.y + 3 + 1, piece->bb0.z - 1, NORTH, piece->depth);
        }
        if (next(env->rng, 1)) {
            extendMineshaft(env, piece->bb0.x - 1, piece->bb0.y + 3 + 1, piece->bb0.z + 1, WEST,  piece->depth);
        }
        if (next(env->rng, 1)) {
            extendMineshaft(env, piece->bb1.x + 1, piece->bb0.y + 3 + 1, piece->bb0.z + 1, EAST,  piece->depth);
        }
        if (next(env->rng, 1)) {
            extendMineshaft(env, piece->bb0.x + 1, piece->bb0.y + 3 + 1, piece->bb1.z + 1, SOUTH, piece->depth);
        }
        break;
    }
    case MS_ROOM: {
        int heightSpace = (piece->bb1.y - piece->bb0.y + 1) - 3 - 1;
        if (heightSpace <= 0) {
            heightSpace = 1;
        }

        int xSpan = piece->bb1.x - piece->bb0.x + 1;

        for (int pos = 0; pos < xSpan; pos += 4) {
            pos += nextInt(env->rng, xSpan);
            if (pos + 3 > xSpan) {
                break;
            }
            extendMineshaft(env, piece->bb0.x + pos, piece->bb0.y + nextInt(env->rng, heightSpace) + 1, piece->bb0.z - 1, NORTH, piece->depth);
        }

        for (int pos = 0; pos < xSpan; pos += 4) {
            pos += nextInt(env->rng, xSpan);
            if (pos + 3 > xSpan) {
                break;
            }
            extendMineshaft(env, piece->bb0.x + pos, piece->bb0.y + nextInt(env->rng, heightSpace) + 1, piece->bb1.z + 1, SOUTH, piece->depth);
        }

        int zSpan = piece->bb1.z - piece->bb0.z + 1;
        for (int pos = 0; pos < zSpan; pos += 4) {
            pos += nextInt(env->rng, zSpan);
            if (pos + 3 > zSpan) {
                break;
            }
            extendMineshaft(env, piece->bb0.x - 1, piece->bb0.y + nextInt(env->rng, heightSpace) + 1, piece->bb0.z + pos, WEST, piece->depth);
        }

        for (int pos = 0; pos < zSpan; pos += 4) {
            pos += nextInt(env->rng, zSpan);
            if (pos + 3 > zSpan) {
                break;
            }
            extendMineshaft(env, piece->bb1.x + 1, piece->bb0.y + nextInt(env->rng, heightSpace) + 1, piece->bb0.z + pos, EAST, piece->depth);
        }
        break;
    }
    case MS_STAIRS: {
        switch ((Direction)piece->rot) {
        case NORTH:
            extendMineshaft(env, piece->bb0.x, piece->bb0.y, piece->bb0.z - 1, NORTH, piece->depth);
            break;
        case EAST:
            extendMineshaft(env, piece->bb1.x + 1, piece->bb0.y, piece->bb0.z, EAST, piece->depth);
            break;
        case SOUTH:
            extendMineshaft(env, piece->bb0.x, piece->bb0.y, piece->bb1.z + 1, SOUTH, piece->depth);
            break;
        case WEST:
            extendMineshaft(env, piece->bb0.x - 1, piece->bb0.y, piece->bb0.z, WEST, piece->depth);
            break;
        default: UNREACHABLE();
        }
        break;
    }
    default: UNREACHABLE();
    }
}

int getMineshaftPieces(Generator *g, Piece *list, int n, int mc, uint64_t seed, int chunkX, int chunkZ) {
    int x = (chunkX << 4) + 2;
    int z = (chunkZ << 4) + 2;

    uint64_t rng = chunkGenerateRnd(seed, chunkX, chunkZ);
    if (mc >= MC_1_18) nextDouble(&rng);

    int count = 1;

    MineshaftPieceEnv env;
    memset(&env, 0, sizeof(env));
    env.mc = mc;
    env.list = list;
    env.n = &count;
    env.rng = &rng;
    env.nmax = n;

    Piece *p = list;
    p->type = MS_ROOM;
    p->name = "MSRoom";

    p->bb0 = p->bb1 = p->pos = (Pos3) {x, 50, z};
    p->bb1.x += 7 + nextInt(&rng, 6);
    p->bb1.y += 4 + nextInt(&rng, 6);
    p->bb1.z += 7 + nextInt(&rng, 6);
    p->depth = 0;
    p->next = NULL;

    extendMineshaftPiece(&env, p);

    int minX = list->bb0.x;
    int minY = list->bb0.y;
    int minZ = list->bb0.z;
    int maxX = list->bb1.x;
    int maxY = list->bb1.y;
    int maxZ = list->bb1.z;
    for (int i = 0; i < count; ++i) {
        Piece *p = &list[i];
        minX = MIN(minX, p->bb0.x);
        minY = MIN(minY, p->bb0.y);
        minZ = MIN(minZ, p->bb0.z);
        maxX = MAX(maxX, p->bb1.x);
        maxY = MAX(maxY, p->bb1.y);
        maxZ = MAX(maxZ, p->bb1.z);
    }

    int vertShift = 0;
    int biome = getBiomeAt(g, 4, chunkX << 2, 0, chunkZ << 2); 
    if (biome == badlands || biome == badlands_plateau || biome == wooded_badlands || biome == wooded_badlands_plateau || biome == eroded_badlands || biome == modified_badlands_plateau || biome == modified_wooded_badlands_plateau) {
        vertShift = 63 - maxY + (maxY - minY + 1) / 2 + 5;
    } else {
        int k = 53;
        int l = maxY - minY + 2; 
        if (l < k) l += nextInt(&rng, (k - l));
        vertShift = l - maxY;
    }    

    minY += vertShift;
    maxY += vertShift;

    for (int i = 0; i < count; ++i) {
        Piece *p = &list[i];
        p->pos.y += vertShift;
        p->bb0.y += vertShift;
        p->bb1.y += vertShift;
    }

    return count;
}

static int isPosCarved(Pos3List *list, int x, int y, int z) {
    for (int i = 0; i < list->size; i++) {
        if (list->pos3s[i].x == x && list->pos3s[i].y == y && list->pos3s[i].z == z)
            return 1;
    }
    return 0;
}

static int isSupportingBox(Piece *p, int cx, int cz, int x0, int x1, int z0,
                           Pos3List *carvedCaves, Pos3List *carvedCanyons,
                           Piece *allPieces, int allCount) {
    int ceilY = p->bb0.y + 3;
    for (int x = x0; x <= x1; x++) {
        int tx = x, tz = z0;
        rotPos(p->bb0, p->bb1, &tx, &tz, p->rot);
        if (tx < cx || tx >= cx + 16 || tz < cz || tz >= cz + 16) {
            return 0;
        }
        if (isPosCarved(carvedCaves, tx, ceilY, tz) || isPosCarved(carvedCanyons, tx, ceilY, tz)) {
            return 0;
        }
        for (int k = 0; k < allCount; k++) {
            Piece *p2 = &allPieces[k];
            if (p2 == p || p2->waterSkipped) continue;
            if (tx >= p2->bb0.x && tx <= p2->bb1.x &&
                ceilY >= p2->bb0.y && ceilY <= p2->bb1.y &&
                tz >= p2->bb0.z && tz <= p2->bb1.z) {
                return 0;
            }
        }
    }
    return 1;
}

static void placeSupport(Piece *p, int cx, int cz, int x0, int z, int x1, RandomSource rnd,
                         Pos3List *carvedCaves, Pos3List *carvedCanyons,
                         Piece *allPieces, int allCount) {
    if (isSupportingBox(p, cx, cz, x0, x1, z, carvedCaves, carvedCanyons, allPieces, allCount)) {
        if (rnd.nextInt(rnd.state, 4) != 0) {
            maybeGenerateBlock(rnd);
            maybeGenerateBlock(rnd);
        }
    }
}

static void maybePlaceCobWeb(Piece *p, int cx, int cz, RandomSource rnd, int x, int z) {
    rotPos(p->bb0, p->bb1, &x, &z, p->rot);
    if (x >= cx && x < cx + 16 && z >= cz && z < cz + 16) {
        rnd.nextFloat(rnd.state);
    }
}

int getMineshaftLoot(Generator *g, Piece *list, int n, StructureSaltConfig ssconf, int mc, uint64_t seed, int chunkX, int chunkZ) {
    int count = getMineshaftPieces(g, list, n, mc, seed, chunkX, chunkZ);

    const int legacy = mc <= MC_1_17;
    int minX = list->bb0.x;
    int minY = list->bb0.y;
    int minZ = list->bb0.z;
    int maxX = list->bb1.x;
    int maxY = list->bb1.y;
    int maxZ = list->bb1.z;
    for (int i = 0; i < count; ++i) {
        Piece *p = &list[i];
        minX = MIN(minX, p->bb0.x);
        minY = MIN(minY, p->bb0.y);
        minZ = MIN(minZ, p->bb0.z);
        maxX = MAX(maxX, p->bb1.x);
        maxY = MAX(maxY, p->bb1.y);
        maxZ = MAX(maxZ, p->bb1.z);
    }
    int cMinX = minX & ~15;
    int cMinZ = minZ & ~15;
    int cMaxX = maxX & ~15;
    int cMaxZ = maxZ & ~15;

    for (int i = 0; i < count; ++i) list[i].chestCount = 0;

    // slow code ahead
    for (int cx = cMinX; cx <= cMaxX; cx += 16) {
        for (int cz = cMinZ; cz <= cMaxZ; cz += 16) {
            CREATE_RANDOM_SOURCE(rnd, legacy);
            uint64_t populationSeed = getPopulationSeed(mc, seed, cx, cz);
            rnd.setSeed(rnd.state, populationSeed + ssconf.generationStep * 10000 + ssconf.decoratorIndex);
            for (int i = 0; i < count; ++i) {
                Piece *p = &list[i];
                if (p->waterSkipped) continue;
                if (!(p->bb1.x >= cx && p->bb0.x <= cx + 15 &&
                      p->bb1.z >= cz && p->bb0.z <= cz + 15)) {
                    continue;
                }

                CaveCarverConfig caveCarverConfig;
                CanyonCarverConfig canyonCarverConfig;
                Pos3List carvedCaveList;
                Pos3List carvedCanyonList;
                Pos3List waterCaves;
                Pos3List waterCanyons;
                Pos3List tempList;
                int touchesWater = 0;
                
                int biome = getBiomeAt(g, 4, cx >> 2, 0, cz >> 2); 

                createPos3List(&carvedCaveList, 1);
                createPos3List(&carvedCanyonList, 1);
                createPos3List(&waterCaves, 1);
                createPos3List(&waterCanyons, 1);

                Range r = {16, (cx >> 4) - 8, (cz >> 4) - 8, 17, 17, 0, 0};
                int *cache = allocCache(g, r);
                genBiomes(g, cache, r);

                for (int caveCarverType = 0; caveCarverType < 4; caveCarverType++) {
                    if (!getCaveCarverConfig(caveCarverType, mc, biome, &caveCarverConfig)) continue;
                    if (caveCarverType <= 1 && !isViableCaveBiome(caveCarverType, biome)) continue;
                    
                    createPos3List(&tempList, 1);

                    carveCave(seed, mc, cx >> 4, cz >> 4, caveCarverConfig, caveCarverType, (int (*)[17]) cache, &tempList);
                    
                    
                    for (int i = 0; i < tempList.size; i++) {
                        appendPos3List(&carvedCaveList, tempList.pos3s[i]);
                        if (caveCarverType == 2 || caveCarverType == 3) appendPos3List(&waterCaves, tempList.pos3s[i]);
                    }
                    
                    freePos3List(&tempList);
                }

                for (int canyonCarverType = 0; canyonCarverType < 2; canyonCarverType++) {
                    if (!getCanyonCarverConfig(canyonCarverType, mc, &canyonCarverConfig)) continue;
                    if (canyonCarverType == 0 && !isViableCanyonBiome(0, biome)) continue;

                    createPos3List(&tempList, 1);

                    carveCanyon(seed, mc, cx >> 4, cz >> 4, canyonCarverConfig, canyonCarverType, (int (*)[17]) cache, &tempList);

                    for (int i = 0; i < tempList.size; i++) {
                        appendPos3List(&carvedCanyonList, tempList.pos3s[i]);
                        if (canyonCarverType == 1) appendPos3List(&waterCanyons, tempList.pos3s[i]);
                    }

                    freePos3List(&tempList);
                }

                free(cache);

                for (int i = 0; i < waterCaves.size; i++) {
                    Pos3 pos = waterCaves.pos3s[i];
                    if (pos.x >= p->bb0.x - 1 && pos.x <= p->bb1.x + 1 &&
                        pos.y >= p->bb0.y - 1 && pos.y <= p->bb1.y + 1 &&
                        pos.z >= p->bb0.z - 1 && pos.z <= p->bb1.z + 1) {
                        touchesWater = 1;
                        break;
                    }
                }

                if (touchesWater) {
                    p->waterSkipped = 1;
                    freePos3List(&carvedCaveList);
                    freePos3List(&carvedCanyonList);
                    freePos3List(&waterCaves);
                    freePos3List(&waterCanyons);
                    continue;
                }

                for (int i = 0; i < waterCanyons.size; i++) {
                    Pos3 pos = waterCanyons.pos3s[i];
                    if (pos.x >= p->bb0.x - 1 && pos.x <= p->bb1.x + 1 &&
                        pos.y >= p->bb0.y - 1 && pos.y <= p->bb1.y + 1 &&
                        pos.z >= p->bb0.z - 1 && pos.z <= p->bb1.z + 1) {
                        touchesWater = 1;
                        break;
                    }
                }

                if (touchesWater) {
                    p->waterSkipped = 1;
                    freePos3List(&carvedCaveList);
                    freePos3List(&carvedCanyonList);
                    freePos3List(&waterCaves);
                    freePos3List(&waterCanyons);
                    continue;
                }

                switch (p->type) {
                case MS_CORRIDOR: {

                    int numSections; // TODO cache?
                    if (p->rot == NORTH || p->rot == SOUTH) {
                        numSections = (p->bb1.z - p->bb0.z + 1) / 5;
                    } else {
                        numSections = (p->bb1.x - p->bb0.x + 1) / 5;
                    }
                    int length = numSections * 5 - 1;
                    generateMaybeBox(0, 2, 0, 2, 2, length, rnd);
                    if ((p->additionalData >> 1) & 1) {
                        generateMaybeBox(0, 0, 0, 2, 1, length, rnd);
                    }

                    for (int section = 0; section < numSections; section++) {
                        int z = 2 + section * 5;
                        placeSupport(p, cx, cz, 0, z, 2, rnd, &carvedCaveList, &carvedCanyonList, list, count);
                        maybePlaceCobWeb(p, cx, cz, rnd, 0, z - 1);
                        maybePlaceCobWeb(p, cx, cz, rnd, 2, z - 1);
                        maybePlaceCobWeb(p, cx, cz, rnd, 0, z + 1);
                        maybePlaceCobWeb(p, cx, cz, rnd, 2, z + 1);
                        maybePlaceCobWeb(p, cx, cz, rnd, 0, z - 2);
                        maybePlaceCobWeb(p, cx, cz, rnd, 2, z - 2);
                        maybePlaceCobWeb(p, cx, cz, rnd, 0, z + 2);
                        maybePlaceCobWeb(p, cx, cz, rnd, 2, z + 2);

                        if (rnd.nextInt(rnd.state, 100) == 0) {
                            int chestPosX = 2, chestPosZ = z - 1;
                            rotPos(p->bb0, p->bb1, &chestPosX, &chestPosZ, p->rot);
                            if (chestPosX >= cx && chestPosX < cx + 16 && chestPosZ >= cz && chestPosZ < cz + 16) {
                                rnd.nextBoolean(rnd.state);
                                p->chestPoses[p->chestCount] = (Pos) {chestPosX, chestPosZ};
                                p->lootTables[p->chestCount] = "abandoned_mineshaft";
                                p->lootSeeds[p->chestCount] = rnd.nextLong(rnd.state);
                                p->chestCount++;
                            }
                        }

                        if (rnd.nextInt(rnd.state, 100) == 0) {
                            int chestPosX = 0, chestPosZ = z + 1;
                            rotPos(p->bb0, p->bb1, &chestPosX, &chestPosZ, p->rot);
                            if (chestPosX >= cx && chestPosX < cx + 16 && chestPosZ >= cz && chestPosZ < cz + 16) {
                                rnd.nextBoolean(rnd.state);
                                p->chestPoses[p->chestCount] = (Pos) {chestPosX, chestPosZ};
                                p->lootTables[p->chestCount] = "abandoned_mineshaft";
                                p->lootSeeds[p->chestCount] = rnd.nextLong(rnd.state);
                                p->chestCount++;
                            }
                        }

                        // this.spiderCorridor && !this.hasPlacedSpider
                        // spiderCorridor is 0b_X_, hasPlacedSpider is 0bX__
                        if (((p->additionalData >> 1) & 0b11) == 0b01) {
                            int spiderRoll = rnd.nextInt(rnd.state, 3);
                            int newX = 1;
                            int newZ = z - 1 + spiderRoll;
                            rotPos(p->bb0, p->bb1, &newX, &newZ, p->rot);
                            if (newX >= cx && newX < cx + 16 && newZ >= cz && newZ < cz + 16) {
                                p->additionalData |= 1 << 2;
                            }
                        }
                    }

                    if ((p->additionalData >> 0) & 1) {
                        for (int zx = 0; zx <= length; zx++) {
                            int tx = 1, tz = zx;
                            rotPos(p->bb0, p->bb1, &tx, &tz, p->rot);
                            if (tx >= cx && tx < cx + 16 && tz >= cz && tz < cz + 16) {
                                maybeGenerateBlock(rnd);
                            }
                        }
                    }
                    freePos3List(&carvedCaveList);
                    freePos3List(&carvedCanyonList);
                    freePos3List(&waterCaves);
                    freePos3List(&waterCanyons);
                    break;
                }
                case MS_CROSSING:
                case MS_ROOM:
                case MS_STAIRS:
                    break;
                default: UNREACHABLE();
                }
            }
        }
    }

    return count;
}
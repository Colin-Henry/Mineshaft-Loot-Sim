#include "cubiomes.h"
#include "cubiomes/loot/loot_tables.h"
#include <stdio.h>

int main()
{
    int mc = MC_1_16_1;
    int64_t seed = 3055141959546LL;

    Generator g;
    setupGenerator(&g, mc, 0);
    applySeed(&g, DIM_OVERWORLD, seed);
    int max = 400;
    Piece *pieceList = malloc(400 * sizeof(Piece));
    StructureSaltConfig ssconf;
    Pos pos;

    for (int regX = -20; regX < 20; regX++) {
        for (int regZ = -20; regZ < 20; regZ++) {
            if (!getStructurePos(Mineshaft, mc, seed, regX, regZ, &pos)) // reg size for mineshafts is 1 chunk
                continue;
            if (isViableStructurePos(Mineshaft, &g, pos.x, pos.z, 0)) {
                printf("Structure pos: %d %d\n", pos.x, pos.z);
                
                getStructureSaltConfig(Mineshaft, mc, getBiomeAt(&g, 4, pos.x, 64, pos.z), &ssconf);
                int mineshaftPieceCount = getMineshaftLoot(&g, pieceList, max, ssconf, mc, seed, regX, regZ);

                for (int i = 0; i < mineshaftPieceCount; ++i) {
                    Piece *piece = &pieceList[i];
                    for (int j = 0; j < piece->chestCount; ++j) {
                        Pos chestPos = piece->chestPoses[j];
                        printf("finalized chestpos %d %d\n", chestPos.x, chestPos.z);
                        printf("lootseed: %ld\n\n", piece->lootSeeds[j]);
                    }
                }
            }
        }
    }
    
    return 0;
}
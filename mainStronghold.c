#include "cubiomes.h"
#include "cubiomes/loot/loot_tables.h"
#include <stdio.h>

static void print_loot(LootTableContext* ctx) {
    for (int i = 0; i < ctx->generated_item_count; i++) {
        ItemStack* item_stack = &(ctx->generated_items[i]);
        printf("%s x %d\n", get_item_name(ctx, item_stack->item), item_stack->count);

        for (int j = 0; j < item_stack->enchantment_count; j++) {
            EnchantInstance* ench = &(item_stack->enchantments[j]);
            printf("    %s %d\n", get_enchantment_name(ench->enchantment), ench->level);
        }
    }
}

int main()
{
    FILE *inputFile = fopen("inputSeeds.txt", "r");
    if (!inputFile)
    {
        perror("Error opening input file");
        return 1;
    }

    FILE *outputFile = fopen("outputSeeds.txt", "w");
    if (!outputFile)
    {
        perror("Error opening output file");
        fclose(inputFile);
        return 1;
    }
    

    int mc = MC_1_16_1;
    StrongholdIter sh;
    int64_t seed;
    Generator g;
    setupGenerator(&g, mc, 0);
    int max = 400;
    Piece *pieceList = malloc(400 * sizeof(Piece)); // Should be fine to be out here (?)
    StructureSaltConfig ssconf;

    //while (fscanf(inputFile, "%" SCNd64, &seed) == 1) {
        seed = 3055141959546LL;
        Pos pos = initFirstStronghold(&sh, mc, seed);
        applySeed(&g, DIM_OVERWORLD, seed);

        for (int i = 1; i <= 3; i++) {

            if (nextStronghold(&sh, &g) <= 0)
                break;

            getStructureSaltConfig(Stronghold, mc, -1, &ssconf);
            int shPieces = getStrongholdLoot(&g, pieceList, max, ssconf, mc, seed, sh.pos.x >> 4, sh.pos.z >> 4);
            printf("Stronghold at %d ~ %d\n\n", sh.pos.x, sh.pos.z);

            for (int i = 0; i < shPieces; ++i) {
                Piece *piece = &pieceList[i];
                for (int j = 0; j < piece->chestCount; ++j) {

                    LootTableContext *ctx;
                    init_loot_table_name(&ctx, piece->lootTables[j], mc);
                    set_loot_seed(ctx, piece->lootSeeds[j]);
                    generate_loot(ctx);
                    Pos chestPos = piece->chestPoses[j];
                    printf("Chest loot '%s' at %d ~ %d\n", piece->lootTables[j], chestPos.x, chestPos.z);
                    print_loot(ctx);
                }
            }
        }
    //}
    return 0;
}
#ifndef SG_ASSETMANAGER_H
#define SG_ASSETMANAGER_H

#include "common.h"

typedef struct {
    GHashTable* textures; // <char*, Texture2D>
    // ...
} SG_AssetManager;

SG_AssetManager SG_AssetManagerNew();
void            SG_AssetManagerLoadTexture(const char* filepath);
void            SG_AssetManagerUnloadTexture(const char* filepath);
void            SG_AssetManagerFree(SG_AssetManager* am);

#endif // SG_ASSETMANAGER_H
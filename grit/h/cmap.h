
//{{BLOCK(cmap)

//======================================================================
//
//	cmap, 256x512@8, 
//	Transparent color : FF,00,FF
//	+ palette 256 entries, not compressed
//	+ 136 tiles (t|f reduced) not compressed
//	+ regular map (in SBBs), not compressed, 32x64 
//	Total size: 512 + 8704 + 4096 = 13312
//
//	Time-stamp: 2014-05-24, 15:41:11
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.8
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_CMAP_H
#define GRIT_CMAP_H

#define cmapTilesLen 8704
extern const unsigned int cmapTiles[2176];

#define cmapMapLen 4096
extern const unsigned short cmapMap[2048];

#define cmapPalLen 512
extern const unsigned short cmapPal[256];

#endif // GRIT_CMAP_H

//}}BLOCK(cmap)

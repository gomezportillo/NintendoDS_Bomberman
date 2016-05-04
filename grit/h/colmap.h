
//{{BLOCK(colmap)

//======================================================================
//
//	colmap, 256x256@8, 
//	Transparent color : FF,00,FF
//	+ palette 256 entries, not compressed
//	+ 135 tiles (t|f reduced) not compressed
//	+ regular map (in SBBs), not compressed, 32x32 
//	Total size: 512 + 8640 + 2048 = 11200
//
//	Time-stamp: 2014-05-24, 15:41:11
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.8
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_COLMAP_H
#define GRIT_COLMAP_H

#define colmapTilesLen 8640
extern const unsigned int colmapTiles[2160];

#define colmapMapLen 2048
extern const unsigned short colmapMap[1024];

#define colmapPalLen 512
extern const unsigned short colmapPal[256];

#endif // GRIT_COLMAP_H

//}}BLOCK(colmap)

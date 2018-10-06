#pragma once

#pragma pack(push, 1)
typedef struct tagBio2EffectSprite {
	std::vector <UCHAR> Id;
	std::vector <ULONG> Pointer;
	std::vector <ULONG> Size;
	std::vector <UCHAR*> Data;
	std::vector <SonyTexture*> Tim;
} Bio2EffectSprite;
#pragma pack(pop)
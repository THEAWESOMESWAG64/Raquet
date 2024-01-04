#include "ppf_functions.h"

// PPF HEADER V1.0
const unsigned char PPFHEADER[8] =
{
    0x50, 0x50, 0x46, 0x76, 0x01, 0x00, 0x00, 0x00
};

const unsigned int ppfbitmask[8] =
{
	0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01 
};

int LoadPPFData(const char* dir)
{
	
	SDL_RWops* ppfdata = SDL_RWFromFile(dir, "rb");

	// check if ppf data is a valid directory
	if (ppfdata != NULL) 
	{
		
		SDL_RWread(ppfdata, CHARDATASET, 8, 1024);

		for (int i = 0; i < 8; i++)
		{
			if (CHARDATASET[i] != PPFHEADER[i])
			{
				printf("WARNING HEADER DATA DOES NOT MATCH\n");
			}
			
		} 

		SDL_RWclose(ppfdata);
		return 1;
		
	} 
	else 
	{
		printf("Failed to load PPF");
		return 0;
	}

}

SDL_Texture* LoadCHR(int id, NES palette[3])
{

	SDL_Texture* tex = SDL_CreateTexture(
		gRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, 8, 8);

	Uint32 pixels[64];
	
	for (int y = 0; y < 8; y++)
	{
		for (int x = 0; x < 8; x++)
		{
			int dest = x + (y * 8);
			int index = y + 8 + (8 * id);
			int index2 = y + 16 + (16 * id);

			int check1 = fsign(CHARDATASET[index] & ppfbitmask[x]);
			int check2 = fsign(CHARDATASET[index2] & ppfbitmask[x]);
			int place =  check1 +  check2;

			switch (place)
			{
				case 0:
					pixels[dest] = NES_PAL0F.color;
				break;

				case 1:
					if (check1)
					{
						pixels[dest] = palette[0].color;
					} 
					else {
						pixels[dest] = palette[1].color;
					}
				break;

				case 2:
					pixels[dest] = palette[2].color;
				break;
			}

		}
		
	}

	SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);
	SDL_UpdateTexture(tex, NULL, pixels, 8 * sizeof(Uint32));
	
	return tex;
}

void PlaceCHR(SDL_Texture* tex, int x, int y) {
	SDL_Rect dstrect;
		dstrect.x = x;
		dstrect.y = y;
		dstrect.w = 8;
		dstrect.h = 8;

	SDL_RenderCopy(gRenderer, tex, NULL, &dstrect);
}

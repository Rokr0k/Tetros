#include <SDL.h>
#include <SDL_net.h>
#include "multiServerScene.h"
#include "game.h"
#include "font.h"
#include "multiAddress.h"

static UDPsocket socket;
static UDPpacket* packet;

static SDL_Texture* textTexture;

void MultiServerScene_Init(SDL_Renderer* renderer)
{
	SDLNet_Init();
	socket = SDLNet_UDP_Open(37646);
	packet = SDLNet_AllocPacket(5);
	textTexture = Font_GetTexture(renderer, "Please wait", 0xFF, 0x00, 0x00, SDL_ALPHA_OPAQUE);
}

void MultiServerScene_Event(SDL_Event* event)
{
	switch (event->type)
	{
	case SDL_KEYDOWN:
		switch (event->key.keysym.sym)
		{
		case SDLK_ESCAPE:
			Game_ChangeScene(MULTI_CONNECT);
			break;
		}
		break;
	}
}

void MultiServerScene_Update(double delta)
{
	if (SDLNet_UDP_Recv(socket, packet) > 0)
	{
		if (packet->data[0] == 'T' && packet->data[1] == 'o' && packet->data[2] == 'c')
		{
			switch (packet->data[3])
			{
			case 'g':
				packet->data[2] = 's';
				SDLNet_UDP_Send(socket, -1, packet);
				break;
			case 'e':
				packet->data[2] = 's';
				SDLNet_UDP_Send(socket, -1, packet);
				SDLNet_Write16(37645, &packet->address.port);
				MultiPlayScene_SetPeer(packet->address);
				MultiPlayScene_IsClient(0);
				Game_ChangeScene(MULTI_PLAY);
				break;
			}
		}
	}
}

void MultiServerScene_Render(SDL_Renderer* renderer)
{
	int w, h;
	SDL_GetRendererOutputSize(renderer, &w, &h);

	SDL_Rect rect;
	rect.w = w / 2;
	rect.h = h / 2;
	rect.x = w / 2 - rect.w / 2;
	rect.y = h / 2 - rect.h / 2;
	SDL_RenderCopy(renderer, textTexture, NULL, &rect);
}

void MultiServerScene_Quit()
{
	SDLNet_FreePacket(packet);
	SDLNet_UDP_Close(socket);
	SDLNet_Quit();
	SDL_DestroyTexture(textTexture);
}
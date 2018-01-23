#include <draw_game.h>
#include <platform/generic_renderer.h>
#include <platform/platform.h>

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#include <debug.h>
#include <dma.h>
#include <dma_tags.h>
#include <draw.h>
#include <gif_tags.h>
#include <graph.h>
#include <gs_psm.h>
#include <kernel.h>
#include <packet.h>
#include <sifrpc.h>
#include <tamtypes.h>

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 512

struct PS2Platform {
	bool is_running;
	framebuffer_t framebuffer;
	texbuffer_t texturebuffer;
	zbuffer_t zbuffer;
	packet_t* packet;
} ps2_platform;

static void setMode(framebuffer_t* framebuffer, texbuffer_t* texturebuffer,
                    zbuffer_t* zbuffer, int width, int height)
{
	framebuffer->width = width;
	framebuffer->height = height;
	framebuffer->mask = 0;
	framebuffer->psm = GS_PSM_32;
	framebuffer->address =
	    graph_vram_allocate(width, height, framebuffer->psm, GRAPH_ALIGN_PAGE);
	zbuffer->enable = 0;
	zbuffer->address = 0;
	zbuffer->mask = 0;
	zbuffer->zsm = 0;

	texturebuffer->width = 512;
	texturebuffer->psm = GS_PSM_24;
	texturebuffer->address =
	    graph_vram_allocate(512, 512, GS_PSM_24, GRAPH_ALIGN_BLOCK);

	graph_initialize(framebuffer->address, width, height, framebuffer->psm, 0,
	                 0);
}

static void setDrawingEnviroment(packet_t* packet, framebuffer_t* framebuffer,
                                 zbuffer_t* zbuffer)
{
	qword_t* q = packet->data;

	// This will setup a default drawing environment.
	q = draw_setup_environment(q, 0, framebuffer, zbuffer);

	// q = draw_primitive_xyoffset(q, 0, 1024, 1024);

	// Finish setting up the environment.
	q = draw_finish(q);

	// Now send the packet, no need to wait since it's the first.
	dma_channel_send_normal(DMA_CHANNEL_GIF, packet->data, q - packet->data, 0,
	                        0);
	dma_wait_fast();
}

static void loadTexture(packet_t* packet, framebuffer_t* framebuffer,
                        texbuffer_t* texturebuffer)
{
	qword_t* q = packet->data;

	q = draw_texture_transfer(q, render_buffer, 320, 240, GS_PSM_24,
	                          texturebuffer->address, texturebuffer->width);
	q = draw_texture_flush(q);

	dma_channel_send_chain(DMA_CHANNEL_GIF, packet->data, q - packet->data, 0,
	                       0);
	dma_wait_fast();
}

static void setupTexture(packet_t* packet, texbuffer_t* texturebuffer)
{
	qword_t* q = packet->data;

	// Using a texture involves setting up a lot of information.
	clutbuffer_t clut;

	texturebuffer->info.width = draw_log2(320);
	texturebuffer->info.height = draw_log2(240);
	texturebuffer->info.components = TEXTURE_COMPONENTS_RGB;
	texturebuffer->info.function = TEXTURE_FUNCTION_DECAL;

	clut.storage_mode = CLUT_STORAGE_MODE1;
	clut.start = 0;
	clut.psm = 0;
	clut.load_method = CLUT_NO_LOAD;
	clut.address = 0;

	q = draw_texturebuffer(q, 0, texturebuffer, &clut);

	// Now send the packet, no need to wait since it's the first.
	dma_channel_send_normal(DMA_CHANNEL_GIF, packet->data, q - packet->data, 0,
	                        0);
	dma_wait_fast();
}

void platformInit()
{
	srand(0);
	SifInitRpc(0);
	dma_channel_initialize(DMA_CHANNEL_GIF, NULL, 0);
	dma_channel_fast_waits(DMA_CHANNEL_GIF);
	setMode(&ps2_platform.framebuffer, &ps2_platform.texturebuffer,
	        &ps2_platform.zbuffer, WINDOW_WIDTH, WINDOW_HEIGHT);

	ps2_platform.packet = packet_init(50, PACKET_NORMAL);
	setDrawingEnviroment(ps2_platform.packet, &ps2_platform.framebuffer,
	                     &ps2_platform.zbuffer);
	loadTexture(ps2_platform.packet, &ps2_platform.framebuffer,
	            &ps2_platform.texturebuffer);
	setupTexture(ps2_platform.packet, &ps2_platform.texturebuffer);

	ps2_platform.is_running = true;
	// clear(0xFF000000);
}
static void processInput(struct Game* game)
{
	return;
}
static void render()
{
	// clear(0xFF000000);

	/*loadTexture(ps2_platform.packet, &ps2_platform.framebuffer,
	            &ps2_platform.texturebuffer);

	setupTexture(ps2_platform.packet, &ps2_platform.texturebuffer);
	*/

	dma_wait_fast();

	qword_t* q = ps2_platform.packet->data;
	q = draw_clear(q, 0, 0, 0, ps2_platform.framebuffer.width,
	               ps2_platform.framebuffer.height, 0x0, 0x0, 0xFF);
	q = draw_finish(q);

	dma_channel_send_normal(DMA_CHANNEL_GIF, ps2_platform.packet->data,
	                        q - ps2_platform.packet->data, 0, 0);
	// graph_wait_vsync();

	/* q = ps2_platform.packet->data;
	texrect_t rect;
	rect.v0.x = 0.5;
	rect.v0.y = 0.5;
	rect.v0.z = 0;
	rect.t0.u = 0.0;
	rect.t0.v = 0.0;
	rect.v1.x = 640.5;
	rect.v1.y = 480.5;
	rect.v1.z = 0;
	rect.t1.u = 1.0;
	rect.t1.v = 1.0;
	rect.color.r = 0xFF;
	rect.color.g = 0xFF;
	rect.color.b = 0xFF;
	rect.color.q = 1.0;
	q = draw_rect_textured(q, 0, &rect);
	*/

	rect_t rect;
	rect.v0.x = 100.0;
	rect.v0.y = 100.0;
	rect.v0.z = 0;
	rect.v1.x = 200.0;
	rect.v1.y = 200.0;
	rect.v1.z = 0;
	rect.color.r = 0xFF;
	rect.color.g = 0x0;
	rect.color.b = 0x0;
	rect.color.a = 0xFF;
	rect.color.q = 1.0;
	draw_wait_finish();
	q = draw_rect_filled(q, 0, &rect);
	q = draw_finish(q);
	dma_channel_send_normal(DMA_CHANNEL_GIF, ps2_platform.packet->data,
	                        q - ps2_platform.packet->data, 0, 0);

	draw_wait_finish();
	graph_wait_vsync();
};

void platformLoop(struct Game* game)
{
	while (ps2_platform.is_running) {
		// processInput(game);
		// gameUpdate(game);
		// gameDraw(game);
		render();
	}
}

void platformExit()
{
	exit(0);
}

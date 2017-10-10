#include <SDL.h>
#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "simple_logger.h"
#include "tilemap.h"
#include "priority_queue.h"
#include "pathfinding.h"

typedef struct
{
	int width;
}Brick;

void draw_stack(Sprite *brick, Vector2D start, Brick *bricklist, unsigned int count)
{
	unsigned int i, j;
	int brickheight = 32;
	int brickwidth = 32;
	Vector2D drawPosition;
	if (!brick)return;
	if (!bricklist)return;
	for (i = 0; i < count; i++)
	{
		//vertical draw
		drawPosition.x = start.x - ((bricklist[i].width * brickwidth) / 2);
		drawPosition.y = start.y - ((i + 1) * brickheight);
		for (j = 0; j < bricklist[i].width; j++)
		{
			//horizontal draw
			drawPosition.x += brickwidth;
			gf2d_sprite_draw(
				brick,
				drawPosition,
				NULL,
				NULL,
				NULL,
				NULL,
				NULL,
				0);
		}
	}
}

int main(int argc, char * argv[])
{
    /*variable declarations*/
    int done = 0;
    const Uint8 * keys;
    Sprite *sprite;

	PriorityQueueList *pqlist;
	int i;
	PF_Graph *graph;

	static Brick bricklist[] =
	{
		{ 2 },
		{ 7 },
		{ 1 },
		{ 5 },
		{ 14 },
		{ 9 },
		{ 13 },
		{ 24 },
		{ 16 },
		{ 22 }
	};
	Sprite *brick;

	Brick bricklist2[10];

	Vector2D testPath[2];
	PF_Path *p;
	PF_PathArray *pa;
    
    int mx,my;
    float mf = 0;
    Sprite *mouse;
    TileMap *map;
    Vector4D mouseColor = {0,0,255,200};
    static Vector2D path[2];
   
    /*program initializtion*/
    init_logger("gf2d.log");
    slog("---==== BEGIN ====---");
    gf2d_graphics_initialize(
        "gf2d",
        1200,
        720,
        1200,
        720,
        vector4d(0,0,0,255),
        0);
    gf2d_graphics_set_frame_delay(17);
    gf2d_sprite_init(1024);
    SDL_ShowCursor(SDL_DISABLE);
    
    /*demo setup*/

    sprite = gf2d_sprite_load_image("images/backgrounds/bg_flat.png");
    mouse = gf2d_sprite_load_all("images/pointer.png",32,32,16); 
	brick = gf2d_sprite_load_all("images/brick.png", 32, 32, 16);
    
    map = tilemap_load("levels/tilemap.map");
    vector2d_copy(path[0],map->start);
    vector2d_copy(path[1],map->end);

	pqlist = pqlist_new();
	for (i = 0; i < 10; i++)
	{
		pqlist_insert(pqlist, &bricklist[i], bricklist[i].width);
	}
	for (i = 0; i < 10; i++)
	{
		bricklist2[i] = *(Brick *)pqlist_delete_max(pqlist);
	}

	slog("calling graph generator..");
	graph = pathfinding_generate_graph_from_tilemap(map);
	p = pathfinding_get_path(graph, path[0], path[1]);
	pa = convert_path_to_vector2d_array(p);
	slog("%i", graph->nodes[0]->num_con);
	/*main game loop*/
    while(!done)
    {
        SDL_PumpEvents();   // update SDL's internal event structures
        keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
        /*update things here*/
        SDL_GetMouseState(&mx,&my);
        mf+=0.1;
        if (mf >= 16.0)mf = 0;
        
        gf2d_graphics_clear_screen();// clears drawing buffers
        // all drawing should happen betweem clear_screen and next_frame
            //backgrounds drawn first
            gf2d_sprite_draw_image(sprite,vector2d(0,0));

			//draw_stack(brick, vector2d(600, 700), bricklist2, 10);
       
            tilemap_draw(map,vector2d(86,24));
            tilemap_draw_path(pa->path,pa->count, map,vector2d(86,24));
            //UI elements last
            gf2d_sprite_draw(
                mouse,
                vector2d(mx,my),
                NULL,
                NULL,
                NULL,
                NULL,
                &mouseColor,
                (int)mf);
        gf2d_grahics_next_frame();// render current draw frame and skip to the next frame
        
        if (keys[SDL_SCANCODE_ESCAPE])done = 1; // exit condition
   //     slog("Rendering at %f FPS",gf2d_graphics_get_frames_per_second());
    }
    slog("---==== END ====---");
    return 0;
}
/*eol@eof*/

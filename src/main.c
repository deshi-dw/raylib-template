#include <stdio.h>
#include <raylib.h>
#include <rlgl.h>

#include "cimgui_impl_raylib.h"

#define WIDTH 1024
#define HEIGHT 768

int main(void) {
	// setup window.
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE);
	SetTraceLogLevel(LOG_ALL);
	InitWindow(WIDTH, HEIGHT, "Room");
	// SetWindowIcon();

	// initialize imgui.
	igCreateContext(NULL);
	igStyleColorsDark(NULL);

	ImGui_ImplRaylib_Init();

	struct ImGuiIO* io = igGetIO();

	// Build and load the texture atlas into a texture
	// (In the examples/ app this is usually done within the
	// ImGui_ImplXXX_Init() function from one of the demo Renderer)
	unsigned char* pixels = NULL;

	int width  = WIDTH;
	int height = HEIGHT;
	ImFontAtlas_GetTexDataAsRGBA32(io->Fonts, &pixels, &width, &height, NULL);
	// At this point you've got the texture data and you need to upload that
	// your your graphic system: After we have created the texture, store its
	// pointer/identifier (_in whichever format your engine uses_) in
	// 'io.Fonts->TexID'. This will be passed back to your via the renderer.
	// Basically ImTextureID == void*. Read FAQ for details about ImTextureID.
	Image	  image	  = LoadImageEx((Color*) pixels, width, height);
	Texture2D texture = LoadTextureFromImage(
		image); // MyEngine::CreateTextureFromMemoryPixels(pixels,
				// width, height, TEXTURE_TYPE_RGBA32)
	io->Fonts->TexID = (void*) &texture.id;

	SetTargetFPS(60);

	// setup camera.
	Camera3D camera = {0};
	camera.target	= (Vector3){.0f, .0f, .0f};
	camera.position = (Vector3){0.0f, 0.0f, 10.0f};
	camera.up		= (Vector3){0.0f, 1.0f, 0.0f};
	camera.type		= CAMERA_ORTHOGRAPHIC;

	ImDrawData* imgui_data;

	while(! WindowShouldClose()) {
		// process imgui events and get ready to draw a new frame.
		ImGui_ImplRaylib_NewFrame();
		ImGui_ImplRaylib_ProcessEvent();
		igNewFrame();

		// update the camera.
		UpdateCamera(&camera);

		BeginDrawing();

		ClearBackground(WHITE);

		// TODO remove this.
		// temporarily show the demo imgui window to know it works.
		igShowDemoWindow(NULL);

		// render imgui frame and draw it to the screen.
		igRender();
		imgui_data = igGetDrawData();
		raylib_render_cimgui(imgui_data);

		EndDrawing();
	}

	CloseWindow();

	return 0;
}

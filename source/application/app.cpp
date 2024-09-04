#include <window.h>
#include <imgui_layer.h>
#include <render.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

void APIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	std::cerr << "OpenGL Debug Message: " << message << std::endl;
}

auto main() -> int {
	init_window();
	
	init_render();

	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(MessageCallback, 0);

	SwapChain* swapchain;
	add_swapchain(true, &swapchain);

	// add just vertices for Triangle
	float vertices[] = {
    	-0.5f, -0.5f, 0.0f,
    	 0.5f, -0.5f, 0.0f,
    	 0.5f,  0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f,
	}; 

	uint32_t indexes[] =
	{
		0, 1, 2,
		2, 3, 0
	};

	float rgb[4] = { 0.5f, 1.0f, 0.0f, 0.0f };

	BufferDesc buffer_desc;
	buffer_desc.size = sizeof(vertices);
	buffer_desc.flags = BufferFlag::kBufferFlagMapWrite;
	Buffer* vbo = nullptr;
	add_buffer(&buffer_desc, &vbo);

	buffer_desc.size = sizeof(indexes);
	Buffer* ebo = nullptr;
	add_buffer(&buffer_desc, &ebo);

	buffer_desc.size = sizeof(rgb);
	buffer_desc.flags |= kBufferFlagDynamic;
	Buffer* ubo = nullptr;
	add_buffer(&buffer_desc, &ubo);

	ShaderLoadDesc shader_load_desc = {};
	shader_load_desc.stages[0] = { "../source/shaders/base_vert.hlsl", "main", ShaderStage::kShaderStageVert };
	shader_load_desc.stages[1] = { "../source/shaders/base_frag.hlsl", "main", ShaderStage::kShaderStageFrag };
	ShaderDesc* shader_desc = nullptr;
	load_shader(&shader_load_desc, &shader_desc);
	Shader* shader;
	add_shader(shader_desc, &shader);

	{ // update buffer data
		void* buffer = map_buffer(vbo);
		std::memcpy(buffer, vertices, sizeof(vertices));
		unmap_buffer(vbo);

		buffer = map_buffer(ebo);
		std::memcpy(buffer, indexes, sizeof(indexes));
		unmap_buffer(ebo);

		buffer = map_buffer(ubo);
		std::memcpy(buffer, rgb, sizeof(rgb));
		unmap_buffer(ubo);
	}

	VertexLayout layout = {0};
	layout.attrib_count = 1;
	layout.attribs[0].size = 3;
	layout.attribs[0].format = GL_FLOAT;
	layout.attribs[0].offset = 0;

	PipelineDesc pipeline_desc = { 0 };
	pipeline_desc.shader = shader;
	pipeline_desc.vertex_layout = &layout;
	pipeline_desc.ubo = ubo->id;

	Pipeline* graphics_pipeline;
	add_pipeline(&pipeline_desc, &graphics_pipeline);

	CmdQueueDesc queue_desc;
	CmdQueue* queue;
	add_queue(&queue_desc, &queue);

	CmdBufferDesc cmd_desc;
	cmd_desc.current_queue = queue;
	CmdBuffer* cmd;
	add_cmd(&cmd_desc, &cmd);

	while(update_window())
	{
		rgb[0] = sin(glfwGetTime()) *0.5 + 0.5;
		{ // update buffer data
			void* buffer = buffer = map_buffer(ubo);
			std::memcpy(buffer, rgb, sizeof(rgb));
			unmap_buffer(ubo);
		}


        imgui_begin_frame();

        glClearColor(gBackGroundColor[0], gBackGroundColor[1], gBackGroundColor[2], 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

		cmd_bind_pipeline(cmd, graphics_pipeline);
		cmd_bind_vertex_buffer(cmd, vbo, 0, sizeof(float) * 3);
		cmd_bind_index_buffer(cmd, ebo);
		cmd_draw_indexed(cmd, 6, 0, 0);
		//cmd_draw(cmd, 0, 3);
		queue_submit(queue);

        imgui_render();
        imgui_end_frame();

        swapchain->swap_buffers(swapchain->window);
        glfwPollEvents();
    
	}

	terminate_window();
}
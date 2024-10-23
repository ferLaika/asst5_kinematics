#include "Renderer.h"

Camera* Renderer::m_camera = new Camera();

Lighting* Renderer::m_lightings = new Lighting();

Bone_Animation* Renderer::m_bone_animation = new Bone_Animation();

nanogui::Screen* Renderer::m_nanogui_screen = nullptr;

bool Renderer::keys[1024];

Renderer::Renderer()
{
}


Renderer::~Renderer()
{	
}

void Renderer::nanogui_init(GLFWwindow* window)
{
    m_nanogui_screen = new nanogui::Screen();
    m_nanogui_screen->initialize(window, true);

    glViewport(0, 0, m_camera->width, m_camera->height);

    // Create nanogui gui
    nanogui::FormHelper *gui_1 = new nanogui::FormHelper(m_nanogui_screen);
    nanogui::ref<nanogui::Window> nanoguiWindow_1 = gui_1->addWindow(Eigen::Vector2i(0, 0), "Nanogui control bar_1");

    // Add camera position controls
    gui_1->addGroup("Camera Position");
    static auto camera_x_widget = gui_1->addVariable("X", m_camera->position[0]);
    static auto camera_y_widget = gui_1->addVariable("Y", m_camera->position[1]);
    static auto camera_z_widget = gui_1->addVariable("Z", m_camera->position[2]);

    // Add Reset Camera button
    gui_1->addButton("Reset Camera", []() {
        m_camera->reset();
        camera_x_widget->setValue(m_camera->position[0]);
        camera_y_widget->setValue(m_camera->position[1]);
        camera_z_widget->setValue(m_camera->position[2]);
    });

    // Adding the bone rotation controls just below the 'Reset Camera' button
    gui_1->addGroup("Bone Rotation");

    // Bone rotation on first axis (X1, Y1, Z1)
    static float rotateX1 = 0.0f;
    static float rotateY1 = 0.0f;
    static float rotateZ1 = 30.0f;  // Z1 is set to 30 degrees
    gui_1->addVariable("Rotate X1", rotateX1);
    gui_1->addVariable("Rotate Y1", rotateY1);
    gui_1->addVariable("Rotate Z1", rotateZ1);

    // Bone rotation on second axis (X2, Y2, Z2)
    static float rotateX2 = 0.0f;
    static float rotateY2 = 0.0f;
    static float rotateZ2 = 30.0f;  // Z2 is set to 30 degrees
    gui_1->addVariable("Rotate X2", rotateX2);
    gui_1->addVariable("Rotate Y2", rotateY2);
    gui_1->addVariable("Rotate Z2", rotateZ2);

    // Bone rotation on third axis (X3, Y3, Z3)
    static float rotateX3 = 0.0f;
    static float rotateY3 = 0.0f;
    static float rotateZ3 = 30.0f;  // Z3 is set to 30 degrees
    gui_1->addVariable("Rotate X3", rotateX3);
    gui_1->addVariable("Rotate Y3", rotateY3);
    gui_1->addVariable("Rotate Z3", rotateZ3);

    // Reset Bone Rotation Button
    gui_1->addButton("Reset Bone Rotation", [&]() {
        rotateX1 = rotateY1 = rotateX2 = rotateY2 = rotateX3 = rotateY3 = 0.0f;
        rotateZ1 = 30.0f; rotateZ2 = 30.0f; rotateZ3 = 30.0f;
    });

    // Set the screen visible and perform layout
    m_nanogui_screen->setVisible(true);
    m_nanogui_screen->performLayout();

    // Nanogui callbacks for various input events
    glfwSetCursorPosCallback(window,
        [](GLFWwindow *window, double x, double y) {
            m_nanogui_screen->cursorPosCallbackEvent(x, y);
        }
    );

    glfwSetMouseButtonCallback(window,
        [](GLFWwindow *, int button, int action, int modifiers) {
            m_nanogui_screen->mouseButtonCallbackEvent(button, action, modifiers);
        }
    );

    glfwSetKeyCallback(window,
        [](GLFWwindow *window, int key, int scancode, int action, int mods) {
            if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
                glfwSetWindowShouldClose(window, GL_TRUE);
            if (key >= 0 && key < 1024)
            {
                if (action == GLFW_PRESS)
                    keys[key] = true;
                else if (action == GLFW_RELEASE)
                    keys[key] = false;
            }
            camera_x_widget->setValue(m_camera->position[0]);
            camera_y_widget->setValue(m_camera->position[1]);
            camera_z_widget->setValue(m_camera->position[2]);
        }
    );

    glfwSetCharCallback(window,
        [](GLFWwindow *, unsigned int codepoint) {
            m_nanogui_screen->charCallbackEvent(codepoint);
        }
    );

    glfwSetDropCallback(window,
        [](GLFWwindow *, int count, const char **filenames) {
            m_nanogui_screen->dropCallbackEvent(count, filenames);
        }
    );

    glfwSetScrollCallback(window,
        [](GLFWwindow *, double x, double y) {
            m_nanogui_screen->scrollCallbackEvent(x, y);
        }
    );

    glfwSetFramebufferSizeCallback(window,
        [](GLFWwindow *, int width, int height) {
            m_nanogui_screen->resizeCallbackEvent(width, height);
        }
    );
}


void Renderer::init()
{
	glfwInit();
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

#if defined(__APPLE__)
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	m_camera->init();

	// Create a GLFWwindow object that we can use for GLFW's functions
	this->m_window = glfwCreateWindow(m_camera->width, m_camera->height, "Assignment 5", nullptr, nullptr);
	glfwMakeContextCurrent(this->m_window);

	glewExperimental = GL_TRUE;
	glewInit();

	m_lightings->init();
	m_bone_animation->init();
	nanogui_init(this->m_window);
}

void Renderer::display(GLFWwindow* window)
{
	Shader m_shader = Shader("../src/shader/basic.vert", "../src/shader/basic.frag");

	// Main frame while loop
	while (!glfwWindowShouldClose(window))
	{

		glfwPollEvents();

		if (is_scene_reset) {
			scene_reset();
			is_scene_reset = false;
		}

		camera_move();

		m_shader.use();
			
		setup_uniform_values(m_shader);

		draw_scene(m_shader);

		m_nanogui_screen->drawWidgets();

		// Swap the screen buffers
		glfwSwapBuffers(window);
	}

	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
	return;
}

void Renderer::run()
{
	init();
	display(this->m_window);
}

void Renderer::load_models()
{
	obj_list.clear();
	Object cube_object("../src/objs/cube.obj");
	cube_object.obj_color = glm::vec4(1.0, 1.0, 0.0, 1.0);
	cube_object.obj_name = "cube";

	Object plane_object("../src/objs/plane.obj");
	plane_object.obj_color = glm::vec4(0.5, 0.5, 0.5, 1.0);
	plane_object.obj_name = "plane";

	Object arrow_object("../src/objs/arrow.obj");
	arrow_object.obj_name = "axis_arrow";


	bind_vaovbo(cube_object);
	bind_vaovbo(plane_object);
	bind_vaovbo(arrow_object);
	
	// Here we only load one model
	obj_list.push_back(cube_object);
	obj_list.push_back(plane_object);
	obj_list.push_back(arrow_object);
}

void Renderer::draw_scene(Shader& shader)
{
	// Set up some basic parameters
	glClearColor(background_color[0], background_color[1], background_color[2], background_color[3]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glFrontFace(GL_CW);

	glm::mat4 world_identity_obj_mat = glm::mat4(1.0f);
	draw_axis(shader, world_identity_obj_mat);
	draw_plane(shader);

	draw_bones(shader, m_bone_animation);
}

void Renderer::camera_move()
{
	float current_frame = glfwGetTime();
	delta_time = current_frame - last_frame;
	last_frame = current_frame;
	// Camera controls
	if (keys[GLFW_KEY_W])
		m_camera->process_keyboard(FORWARD, delta_time);
	if (keys[GLFW_KEY_S])
		m_camera->process_keyboard(BACKWARD, delta_time);
	if (keys[GLFW_KEY_A])
		m_camera->process_keyboard(LEFT, delta_time);
	if (keys[GLFW_KEY_D])
		m_camera->process_keyboard(RIGHT, delta_time);
	if (keys[GLFW_KEY_Q])
		m_camera->process_keyboard(UP, delta_time);
	if (keys[GLFW_KEY_E])
		m_camera->process_keyboard(DOWN, delta_time);
	if (keys[GLFW_KEY_I])
		m_camera->process_keyboard(ROTATE_X_UP, delta_time);
	if (keys[GLFW_KEY_K])
		m_camera->process_keyboard(ROTATE_X_DOWN, delta_time);
	if (keys[GLFW_KEY_J])
		m_camera->process_keyboard(ROTATE_Y_UP, delta_time);
	if (keys[GLFW_KEY_L])
		m_camera->process_keyboard(ROTATE_Y_DOWN, delta_time);
	if (keys[GLFW_KEY_U])
		m_camera->process_keyboard(ROTATE_Z_UP, delta_time);
	if (keys[GLFW_KEY_O])
		m_camera->process_keyboard(ROTATE_Z_DOWN, delta_time);

}

void Renderer::draw_object(Shader& shader, Object& object)
{
	glBindVertexArray(object.vao);

	glUniform3f(glGetUniformLocation(shader.program, "m_object.object_color"), object.obj_color[0], object.obj_color[1], object.obj_color[2]);
	glUniform1f(glGetUniformLocation(shader.program, "m_object.shininess"), object.shininess);

	if (object.m_render_type == RENDER_TRIANGLES)
	{
		if (object.m_obj_type == OBJ_POINTS)
		{
			std::cout << "Error: Cannot render triangles if input obj type is point\n";
			return;
		}
		if (object.m_obj_type == OBJ_TRIANGLES)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glDrawArrays(GL_TRIANGLES, 0, object.vao_vertices.size());
		}
	}

	if (object.m_render_type == RENDER_LINES)
	{
		glLineWidth(10.0);
		if (object.m_obj_type == OBJ_POINTS)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glDrawArrays(GL_LINE_LOOP, 0, object.vao_vertices.size());
		}
		if (object.m_obj_type == OBJ_TRIANGLES)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glDrawArrays(GL_TRIANGLES, 0, object.vao_vertices.size());
		}
	}

	if (object.m_obj_type == OBJ_POINTS)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINTS);
		glDrawArrays(GL_POINTS, 0, object.vao_vertices.size());
	}
	glBindVertexArray(0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Renderer::draw_axis(Shader& shader, const glm::mat4 axis_obj_mat)
{
	// You can always see the arrow
	glDepthFunc(GL_ALWAYS);
	// Get arrow obj
	Object *arrow_obj = nullptr;
	for (unsigned int i = 0; i < obj_list.size(); i++)
	{
		if (obj_list[i].obj_name == "axis_arrow") {
			arrow_obj = &obj_list[i];
		}
	}

	if (arrow_obj == nullptr)
		return;

	// Draw main axis
	arrow_obj->obj_mat = axis_obj_mat;
	glUniformMatrix4fv(glGetUniformLocation(shader.program, "model"), 1, GL_FALSE, glm::value_ptr(arrow_obj->obj_mat));
	arrow_obj->obj_color = glm::vec4(1, 0, 0, 1);
	draw_object(shader, *arrow_obj);

	arrow_obj->obj_mat = axis_obj_mat;
	arrow_obj->obj_mat = glm::rotate(arrow_obj->obj_mat, glm::radians(90.0f), glm::vec3(0, 0, 1));
	glUniformMatrix4fv(glGetUniformLocation(shader.program, "model"), 1, GL_FALSE, glm::value_ptr(arrow_obj->obj_mat));
	arrow_obj->obj_color = glm::vec4(0, 1, 0, 1);
	draw_object(shader, *arrow_obj);

	arrow_obj->obj_mat = axis_obj_mat;
	arrow_obj->obj_mat = glm::rotate(arrow_obj->obj_mat, glm::radians(-90.0f), glm::vec3(0, 1, 0));
	glUniformMatrix4fv(glGetUniformLocation(shader.program, "model"), 1, GL_FALSE, glm::value_ptr(arrow_obj->obj_mat));
	arrow_obj->obj_color = glm::vec4(0, 0, 1, 1);
	draw_object(shader, *arrow_obj);
	glDepthFunc(GL_LESS);
}

void Renderer::draw_plane(Shader& shader)
{
	Object *plane_obj = nullptr;
	for (unsigned int i = 0; i < obj_list.size(); i++)
	{
		if (obj_list[i].obj_name == "plane") {
			plane_obj = &obj_list[i];
		}
	}
	if (plane_obj == nullptr)
		return;

	plane_obj->obj_mat =  glm::mat4(1.0f);
	plane_obj->obj_mat = glm::scale(plane_obj->obj_mat, glm::vec3(10, 10, 10));
	glUniformMatrix4fv(glGetUniformLocation(shader.program, "model"), 1, GL_FALSE, glm::value_ptr(plane_obj->obj_mat));
	draw_object(shader, *plane_obj);
}

void Renderer::draw_bones(Shader& shader, Bone_Animation* m_bone_animation)
{
    Object *bone_obj = nullptr;
    for (unsigned int i = 0; i < obj_list.size(); i++)
    {
        if (obj_list[i].obj_name == "cube") {
            bone_obj = &obj_list[i];
        }
    }
    if (bone_obj == nullptr)
        return;
    
    m_bone_animation->update(delta_time);

    // Draw root bone (red)
    glm::mat4 root_bone_mat = glm::mat4(1.0f);
    root_bone_mat = glm::translate(root_bone_mat, m_bone_animation->root_position);
    glUniformMatrix4fv(glGetUniformLocation(shader.program, "model"), 1, GL_FALSE, glm::value_ptr(root_bone_mat));
    bone_obj->obj_color = m_bone_animation->colors[0]; // Red
    draw_object(shader, *bone_obj);

    // Draw first bone (yellow) - Should start at top of red cube
    glm::mat4 yellow_bone_mat = root_bone_mat;
    yellow_bone_mat = glm::translate(yellow_bone_mat, glm::vec3(0.0f, 0.5f, 0.0f)); // Move to top of red cube
    yellow_bone_mat = glm::translate(yellow_bone_mat, glm::vec3(0.0f, 2.0f, 0.0f)); // Move up by half of yellow bone's height
    yellow_bone_mat = glm::scale(yellow_bone_mat, glm::vec3(0.5f, 4.0f, 0.5f));
    glUniformMatrix4fv(glGetUniformLocation(shader.program, "model"), 1, GL_FALSE, glm::value_ptr(yellow_bone_mat));
    bone_obj->obj_color = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f); // Yellow
    draw_object(shader, *bone_obj);

    // Draw second bone (magenta) - Should start at top of yellow bone
    glm::mat4 magenta_bone_mat = root_bone_mat;
    magenta_bone_mat = glm::translate(magenta_bone_mat, glm::vec3(0.0f, 4.5f, 0.0f)); // Move to top of yellow bone
    magenta_bone_mat = glm::translate(magenta_bone_mat, glm::vec3(0.0f, 1.5f, 0.0f)); // Move up by half of magenta bone's height
    magenta_bone_mat = glm::scale(magenta_bone_mat, glm::vec3(0.5f, 3.0f, 0.5f));
    glUniformMatrix4fv(glGetUniformLocation(shader.program, "model"), 1, GL_FALSE, glm::value_ptr(magenta_bone_mat));
    bone_obj->obj_color = glm::vec4(1.0f, 0.0f, 1.0f, 1.0f); // Magenta
    draw_object(shader, *bone_obj);

    // Draw third bone (cyan) - Should start at top of magenta bone
    glm::mat4 cyan_bone_mat = root_bone_mat;
    cyan_bone_mat = glm::translate(cyan_bone_mat, glm::vec3(0.0f, 7.5f, 0.0f)); // Move to top of magenta bone
    cyan_bone_mat = glm::translate(cyan_bone_mat, glm::vec3(0.0f, 1.0f, 0.0f)); // Move up by half of cyan bone's height
    cyan_bone_mat = glm::scale(cyan_bone_mat, glm::vec3(0.5f, 2.0f, 0.5f));
    glUniformMatrix4fv(glGetUniformLocation(shader.program, "model"), 1, GL_FALSE, glm::value_ptr(cyan_bone_mat));
    bone_obj->obj_color = glm::vec4(0.0f, 1.0f, 1.0f, 1.0f); // Cyan
    draw_object(shader, *bone_obj);
}


void Renderer::bind_vaovbo(Object &cur_obj)
{
	glGenVertexArrays(1, &cur_obj.vao);
	glGenBuffers(1, &cur_obj.vbo);

	glBindVertexArray(cur_obj.vao);

	glBindBuffer(GL_ARRAY_BUFFER, cur_obj.vbo);
	glBufferData(GL_ARRAY_BUFFER, cur_obj.vao_vertices.size() * sizeof(Object::Vertex), &cur_obj.vao_vertices[0], GL_STATIC_DRAW);

	// Vertex Positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Object::Vertex), (GLvoid*)0);
	// Vertex Normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Object::Vertex), (GLvoid*)offsetof(Object::Vertex, Normal));
	// Vertex Texture Coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Object::Vertex), (GLvoid*)offsetof(Object::Vertex, TexCoords));

	glBindVertexArray(0);
}

void Renderer::setup_uniform_values(Shader& shader)
{
	// Camera uniform values
	glUniform3f(glGetUniformLocation(shader.program, "camera_pos"), m_camera->position.x, m_camera->position.y, m_camera->position.z);

	glUniformMatrix4fv(glGetUniformLocation(shader.program, "projection"), 1, GL_FALSE, glm::value_ptr(m_camera->get_projection_mat()));
	glUniformMatrix4fv(glGetUniformLocation(shader.program, "view"), 1, GL_FALSE, glm::value_ptr(m_camera->get_view_mat()));

	// Light uniform values
	glUniform1i(glGetUniformLocation(shader.program, "dir_light.status"), m_lightings->direction_light.status);
	glUniform3f(glGetUniformLocation(shader.program, "dir_light.direction"), m_lightings->direction_light.direction[0], m_lightings->direction_light.direction[1], m_lightings->direction_light.direction[2]);
	glUniform3f(glGetUniformLocation(shader.program, "dir_light.ambient"), m_lightings->direction_light.ambient[0], m_lightings->direction_light.ambient[1], m_lightings->direction_light.ambient[2]);
	glUniform3f(glGetUniformLocation(shader.program, "dir_light.diffuse"), m_lightings->direction_light.diffuse[0], m_lightings->direction_light.diffuse[1], m_lightings->direction_light.diffuse[2]);
	glUniform3f(glGetUniformLocation(shader.program, "dir_light.specular"), m_lightings->direction_light.specular[0], m_lightings->direction_light.specular[1], m_lightings->direction_light.specular[2]);

	// Set current point light as camera's position
	m_lightings->point_light.position = m_camera->position;
	glUniform1i(glGetUniformLocation(shader.program, "point_light.status"), m_lightings->point_light.status);
	glUniform3f(glGetUniformLocation(shader.program, "point_light.position"), m_lightings->point_light.position[0], m_lightings->point_light.position[1], m_lightings->point_light.position[2]);
	glUniform3f(glGetUniformLocation(shader.program, "point_light.ambient"), m_lightings->point_light.ambient[0], m_lightings->point_light.ambient[1], m_lightings->point_light.ambient[2]);
	glUniform3f(glGetUniformLocation(shader.program, "point_light.diffuse"), m_lightings->point_light.diffuse[0], m_lightings->point_light.diffuse[1], m_lightings->point_light.diffuse[2]);
	glUniform3f(glGetUniformLocation(shader.program, "point_light.specular"), m_lightings->point_light.specular[0], m_lightings->point_light.specular[1], m_lightings->point_light.specular[2]);
	glUniform1f(glGetUniformLocation(shader.program, "point_light.constant"), m_lightings->point_light.constant);
	glUniform1f(glGetUniformLocation(shader.program, "point_light.linear"), m_lightings->point_light.linear);
	glUniform1f(glGetUniformLocation(shader.program, "point_light.quadratic"), m_lightings->point_light.quadratic);
}

void Renderer::scene_reset()
{
	load_models();
	m_camera->reset();
}
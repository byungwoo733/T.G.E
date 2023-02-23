/* Here's an example of using EFL with OpenGL ES for rendering */
#include <Ecore_Evas.h>
#include <Evas_GL.h>

int main(int argc, char **argv)
{
    Ecore_Evas *ee;
    Evas *evas;
    Evas_GL_Config *gl_config;
    Evas_Object *bg, *rect;
    GLuint program, vertex_shader, fragment_shader;
    GLint position_location, color_location;

    ecore_evas_init();
    ecore_evas_app_name_set("EFL with OpenGL ES");
    ecore_evas_size_hint_min_set(NULL, EINA_SIZE2D(400, 400));
    ee = ecore_evas_new(NULL, 0, 0, 400, 400, NULL);
    evas = ecore_evas_get(ee);

    gl_config = evas_gl_config_new();
    evas_gl_config_set(gl_config, EVAS_GL_DEPTH_BIT, EVAS_GL_TRUE);
    evas_gl_config_set(gl_config, EVAS_GL_STENCIL_BIT, EVAS_GL_TRUE);
    evas_gl_config_set(gl_config, EVAS_GL_COLOR_FORMAT_RGBA_8888, EVAS_GL_TRUE);
    evas_gl_config_set(gl_config, EVAS_GL_OPTIONS_NONE, EVAS_GL_TRUE);

    evas_object_hide(ecore_evas_add(evas_engine_info_get(), NULL, NULL, 0, 0, 400, 400));

    Evas_GL_Context_Version gl_version;
    gl_version.api = EVAS_GL_GLES_API;
    gl_version.version = EVAS_GL_GLES_2_X;
    Evas_GL_Context *gl_context = evas_gl_context_create(evas, NULL);
    evas_gl_context_version_set(gl_context, &gl_version);

    evas_object_show(ee->evas);

    bg = evas_object_rectangle_add(ee->evas);
    evas_object_color_set(bg, 255, 255, 255, 255);
    evas_object_resize(bg, 400, 400);
    evas_object_show(bg);

    const char *vertex_shader_source =
        "attribute vec4 a_Position;\n"
        "attribute vec4 a_Color;\n"
        "varying vec4 v_Color;\n"
        "void main() {\n"
        "  v_Color = a_Color;\n"
        "  gl_Position = a_Position;\n"
        "}\n";

    const char *fragment_shader_source =
        "precision mediump float;\n"
        "varying vec4 v_Color;\n"
        "void main() {\n"
        "  gl_FragColor = v_Color;\n"
        "}\n";

    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
    glCompileShader(vertex_shader);

    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
    glCompileShader(fragment_shader);

    program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    position_location = glGetAttribLocation(program, "a_Position");
    color_location = glGetAttribLocation(program, "a_Color");

    glViewport(0, 0, 400, 400);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    rect = evas_object_rectangle_add(ee->evas);
    evas_object_color_set(rect, 0, 0, 255, 255);
        evas_object_resize(rect, 200, 200);
    evas_object_move(rect, 100, 100);
    evas_object_show(rect);

    GLfloat vertices[] = {
        -0.5f, -0.5f, 0.0f, 1.0f,  // position 1
         0.0f,  0.0f, 1.0f, 1.0f,  // color 1
         0.5f, -0.5f, 0.0f, 1.0f,  // position 2
         0.0f,  1.0f, 0.0f, 1.0f,  // color 2
         0.0f,  0.5f, 0.0f, 1.0f,  // position 3
         1.0f,  0.0f, 0.0f, 1.0f   // color 3
    };

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(position_location);
    glVertexAttribPointer(position_location, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0);

    glEnableVertexAttribArray(color_location);
    glVertexAttribPointer(color_location, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(4 * sizeof(GLfloat)));

    while (ecore_main_loop_iterate())
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        glUseProgram(program);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        evas_gl_swap_buffers(gl_context);
        ecore_evas_manual_render(ee);
    }

    evas_gl_context_destroy(evas, gl_context);
    evas_gl_config_free(gl_config);
    ecore_evas_free(ee);
    ecore_evas_shutdown();

    return 0;
}
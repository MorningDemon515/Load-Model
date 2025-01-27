#define SDL_MAIN_HANDLED
#define STB_IMAGE_IMPLEMENTATION
#include "Model.h"
#include "Camera.h"

#define WIDTH 800
#define HEIGHT 600

SDL_Window* window;
SDL_GLContext glc;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, FPS);

float deltaTime = 0.0f;
float lastFrame = 0.0f;

glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

int main()
{
    SDL_Init(SDL_INIT_EVERYTHING);

    window = SDL_CreateWindow(
        " ",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        WIDTH,HEIGHT,SDL_WINDOW_SHOWN|SDL_WINDOW_OPENGL
    );

    SDL_SetRelativeMouseMode(SDL_TRUE);

    glc = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window,glc);

    gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);

    SDL_Event event;
    bool run = true;

    //stbi_set_flip_vertically_on_load(true);

    ShaderFromFile* shader = new ShaderFromFile("glsl/vertex.txt","glsl/fragment.txt");
    ShaderFromFile* shader2 = new ShaderFromFile("glsl/vertex.txt","glsl/fragment.txt");
    Model* m = new Model("model/yinlang.fbx");

    Model* m2 = new Model("model/173.fbx");

    while (run)
    {
        float currentFrame = SDL_GetTicks() / 1000.0f;
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        SDL_PollEvent(&event);

        if(event.type == SDL_QUIT)
        {
            run = false;
        }

        const Uint8* state = SDL_GetKeyboardState(NULL);
        if (state[SDL_SCANCODE_W] )
        {
            camera.processKeyboard(static_cast<SDL_Scancode>(SDL_SCANCODE_W), deltaTime);
        }

        if(state[SDL_SCANCODE_S])
        {
            camera.processKeyboard(static_cast<SDL_Scancode>(SDL_SCANCODE_S), deltaTime);
        }

        if(state[SDL_SCANCODE_A] )
        {
            camera.processKeyboard(static_cast<SDL_Scancode>(SDL_SCANCODE_A), deltaTime);
        }

        if(state[SDL_SCANCODE_D])
        {
            camera.processKeyboard(static_cast<SDL_Scancode>(SDL_SCANCODE_D), deltaTime);
        }

        if(state[SDL_SCANCODE_ESCAPE])
        {
            run = false;
        }

        if (event.type == SDL_MOUSEMOTION) {
                camera.processMouseMovement(event.motion.xrel, -event.motion.yrel);
        }

        glClearColor(0.0f,0.0f,0.0f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        shader->Use();

        glUniform3f(shader->GetShaderSourceUniform("light.direction"), -0.2f, -1.0f, -0.3f);
        glUniform3fv(shader->GetShaderSourceUniform("viewPos"),1,&camera.getPosition()[0]);

        // light properties
        glUniform3f(shader->GetShaderSourceUniform("light.ambient"), 0.2f, 0.2f, 0.2f);
        glUniform3f(shader->GetShaderSourceUniform("light.diffuse"), 0.5f, 0.5f, 0.5f);
        glUniform3f(shader->GetShaderSourceUniform("light.specular"), 1.0f, 1.0f, 1.0f);

        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.getViewMatrix();
        glUniformMatrix4fv(shader->GetShaderSourceUniform("projection"),1,GL_FALSE,&projection[0][0]);
        glUniformMatrix4fv(shader->GetShaderSourceUniform("view"),1,GL_FALSE,&view[0][0]);
        
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); 
        model = glm::rotate(model,glm::radians(360.0f),glm::vec3(1.0f,0.0f,0.0f));
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	
        glUniformMatrix4fv(shader->GetShaderSourceUniform("model"),1,GL_FALSE,&model[0][0]);
        
        m->Draw(*shader);

        shader2->Use();

        glUniform3f(shader2->GetShaderSourceUniform("light.direction"), -0.2f, -1.0f, -0.3f);
        glUniform3fv(shader2->GetShaderSourceUniform("viewPos"),1,&camera.getPosition()[0]);

        // light properties
        glUniform3f(shader2->GetShaderSourceUniform("light.ambient"), 0.2f, 0.2f, 0.2f);
        glUniform3f(shader2->GetShaderSourceUniform("light.diffuse"), 0.5f, 0.5f, 0.5f);
        glUniform3f(shader2->GetShaderSourceUniform("light.specular"), 1.0f, 1.0f, 1.0f);

        glUniformMatrix4fv(shader2->GetShaderSourceUniform("projection"),1,GL_FALSE,&projection[0][0]);
        glUniformMatrix4fv(shader2->GetShaderSourceUniform("view"),1,GL_FALSE,&view[0][0]);
        
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 10.0f)); 
        model = glm::rotate(model,glm::radians(270.0f),glm::vec3(1.0f,0.0f,0.0f));
        model = glm::rotate(model,glm::radians(180.0f),glm::vec3(0.0f,0.0f,1.0f));
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	
        glUniformMatrix4fv(shader2->GetShaderSourceUniform("model"),1,GL_FALSE,&model[0][0]);
        
        m2->Draw(*shader2);

        SDL_GL_SwapWindow(window);
    }
    
    delete shader;
    delete shader2;
    delete m;

    SDL_GL_DeleteContext(glc);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
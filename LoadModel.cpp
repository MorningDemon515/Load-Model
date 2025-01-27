#include "LoadModel.h"

#include "Model.h"
#include "Camera.h"

#define WIDTH 800
#define HEIGHT 600

extern int run;
extern SDL_Event event;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, FPS);

float deltaTime = 0.0f;
float lastFrame = 0.0f;
ShaderFromFile* shader;
ShaderFromFile* shader2;
Model_MD* m;
Model_MD* m2;

void InputModel()
{
    float currentFrame = SDL_GetTicks() / 1000.0f;
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

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
            run = 0;
        }

        if (event.type == SDL_MOUSEMOTION) {
                camera.processMouseMovement(event.motion.xrel, -event.motion.yrel);
        }
}

void LoadModel()
{

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
        model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));	
        glUniformMatrix4fv(shader->GetShaderSourceUniform("model"),1,GL_FALSE,&model[0][0]);
        
        m->Draw(*shader);
}

void InitModel()
{
    
    shader = new ShaderFromFile("glsl/vertex.txt","glsl/fragment.txt");
    shader2 = new ShaderFromFile("glsl/vertex.txt","glsl/fragment.txt");
   // Model* m = new Model("model/yinlang.fbx");
   m = new Model_MD("model/dancing_vampire.dae");

    m2 = new Model_MD("model/173.fbx");
}
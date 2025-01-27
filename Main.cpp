#define SDL_MAIN_HANDLED
#define STB_IMAGE_IMPLEMENTATION

#include "learnopengl/shader_m.h"
//#include "learnopengl/camera.h"
#include "learnopengl/animator.h"
#include "learnopengl/model_animation.h"

#include "Camera.h"
//#include "LoadModel.h"

#define WIDTH 800
#define HEIGHT 600

SDL_Window* window;
SDL_GLContext glc;
SDL_Event event;

glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

int run = 1;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, FPS);

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

    stbi_set_flip_vertically_on_load(true);
   // InitModel();
	
	Shader ourShader("anim_model.vs", "anim_model.fs");

	// load models
	// -----------
	Model ourModel("model/dancing_vampire.dae");
	Animation danceAnimation("model/dancing_vampire.dae",&ourModel);
	Animator animator(&danceAnimation);

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    while (run)
    {

        SDL_PollEvent(&event);

        if(event.type == SDL_QUIT)
        {
            run = 0;
        }

        float currentFrame = SDL_GetTicks() / 1000.0f;
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        animator.UpdateAnimation(deltaTime);

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

       // InputModel();

        glClearColor(0.0f,0.0f,0.0f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        
        
        /*
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
        
        m2->Draw(*shader2);*/
        //LoadModel();
        ourShader.use();

		glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.getViewMatrix();
		ourShader.setMat4("projection", projection);
		ourShader.setMat4("view", view);

        auto transforms = animator.GetFinalBoneMatrices();
		for (int i = 0; i < transforms.size(); ++i)
			ourShader.setMat4("finalBonesMatrices[" + std::to_string(i) + "]", transforms[i]);


		// render the loaded model
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -0.4f, 0.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));	// it's a bit too big for our scene, so scale it down
		ourShader.setMat4("model", model);
		ourModel.Draw(ourShader);

        SDL_GL_SwapWindow(window);
    }
    
    //delete shader;
    //delete shader2;
    //delete m;

    SDL_GL_DeleteContext(glc);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
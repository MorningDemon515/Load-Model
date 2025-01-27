#define SDL_MAIN_HANDLED
#define STB_IMAGE_IMPLEMENTATION
#include "Model.h"
#include "Camera.h"

#define WIDTH 800
#define HEIGHT 600

SDL_Window* window;
SDL_GLContext glc;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = WIDTH / 2.0f;
float lastY = HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// 假设有一个加载纹理的实现
unsigned int loadTextureFromFile(const char* path) {
    // 示例：加载纹理用 stb_image.h
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrChannels;
    unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
    if (data) {
        GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(data);
    } else {
        std::cerr << "Failed to load texture: " << path << std::endl;
    }

    return textureID;
}

int main()
{
    SDL_Init(SDL_INIT_EVERYTHING);

    window = SDL_CreateWindow(
        " ",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        WIDTH,HEIGHT,SDL_WINDOW_SHOWN|SDL_WINDOW_OPENGL
    );

    glc = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window,glc);

    gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);

    SDL_Event event;
    bool run = true;

    //stbi_set_flip_vertically_on_load(true);

    ShaderFromFile* shader = new ShaderFromFile("vertex.txt","fragment.txt");
    Model* m = new Model("model/173_2.obj");

    while (run)
    {
        float currentFrame = static_cast<float>(SDL_GetTicks());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        SDL_PollEvent(&event);

        if(event.type == SDL_QUIT)
        {
            run = false;
        }

        if(event.type == SDL_KEYDOWN)
        {
            if(event.key.keysym.sym == SDLK_w)
            {
                camera.ProcessKeyboard(FORWARD, deltaTime);
            }

            if(event.key.keysym.sym == SDLK_a)
            {
                camera.ProcessKeyboard(LEFT, deltaTime);
            }

            if(event.key.keysym.sym == SDLK_s)
            {
                camera.ProcessKeyboard(BACKWARD, deltaTime);
            }

            if(event.key.keysym.sym == SDLK_d)
            {
                camera.ProcessKeyboard(RIGHT, deltaTime);
            }
        }

        if(event.type == SDL_MOUSEMOTION)
        {
            int px = event.motion.x;
            int py = event.motion.y;

            float xpos = static_cast<float>(px);
            float ypos = static_cast<float>(py);

            if (firstMouse)
            {
                lastX = xpos;
                lastY = ypos;
                firstMouse = false;
            }

            float xoffset = xpos - lastX;
            float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

            lastX = xpos;
            lastY = ypos;

            camera.ProcessMouseMovement(xoffset, yoffset);
        }

        glClearColor(1.0f,0.0f,0.0f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        shader->Use();

        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glUniformMatrix4fv(shader->GetShaderSourceUniform("projection"),1,GL_FALSE,&projection[0][0]);
        glUniformMatrix4fv(shader->GetShaderSourceUniform("view"),1,GL_FALSE,&view[0][0]);
        
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); 
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	
        glUniformMatrix4fv(shader->GetShaderSourceUniform("model"),1,GL_FALSE,&model[0][0]);
        
        m->Draw(*shader);

        SDL_GL_SwapWindow(window);
    }
    
    delete shader;
    delete m;
    SDL_GL_DeleteContext(glc);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
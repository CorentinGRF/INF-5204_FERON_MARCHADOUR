#include <glimac/SDLWindowManager.hpp>
#include <GL/glew.h>
#include <iostream>
#include <glimac/Program.hpp>
#include <glimac/FilePath.hpp>
#include <glimac/glm.hpp>
#include <glimac/Image.hpp>
#include <glimac/Sphere.hpp>
#include <glimac/Rectangle.hpp>
#include <glimac/FreeFlyCamera.hpp>


using namespace glimac;

GLuint loadTexture (std::string filepath, GLuint* texture){
    std::unique_ptr<Image> texturemap = loadImage(filepath);
    if(texturemap == NULL) std::cout << "La texture est nulle : " <<  filepath << std::endl;
    glGenTextures(1,texture);
    glBindTexture(GL_TEXTURE_2D,*texture);
    glTexImage2D(GL_TEXTURE_2D,0, GL_RGBA,texturemap->getWidth(),texturemap->getHeight(),0,GL_RGBA,GL_FLOAT,texturemap->getPixels());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D,0);
}

struct ShaderProgram{
    Program m_Program;
    GLuint uMVPMatrix;
    GLuint uMVMatrix;
    GLuint uNormalMatrix;

    GLuint uTexture;
    GLuint uKdTexture;
    GLuint uKsTexture;

    GLuint uShininess;

    GLuint uLightPos_vs;
    GLuint uLightIntensity;

    GLuint uLightPos_vs2;
    GLuint uLightIntensity2;

    ShaderProgram(const FilePath& applicationPath):
        m_Program(loadProgram(applicationPath.dirPath() + "shaders/shader.vs.glsl",
                              applicationPath.dirPath() + "shaders/shader.fs.glsl")) {
        uMVPMatrix = glGetUniformLocation(m_Program.getGLId(), "uMVPMatrix");
        uMVMatrix = glGetUniformLocation(m_Program.getGLId(), "uMVMatrix");
        uNormalMatrix = glGetUniformLocation(m_Program.getGLId(), "uNormalMatrix");

        uTexture = glGetUniformLocation(m_Program.getGLId(), "uTexture");
        uKdTexture = glGetUniformLocation(m_Program.getGLId(), "uKdTexture");
        uKsTexture = glGetUniformLocation(m_Program.getGLId(), "uKsTexture");

        uShininess = glGetUniformLocation(m_Program.getGLId(), "uShininess");

        uLightPos_vs = glGetUniformLocation(m_Program.getGLId(), "uLightPos_vs");
        uLightIntensity = glGetUniformLocation(m_Program.getGLId(), "uLightIntensity");

        uLightPos_vs2 = glGetUniformLocation(m_Program.getGLId(), "uLightPos_vs2");
        uLightIntensity2 = glGetUniformLocation(m_Program.getGLId(), "uLightIntensity2");
    }
};

int main(int argc, char** argv) {
    // Initialize SDL and open a window
    SDLWindowManager windowManager(800, 600, "GLImac");

    glewExperimental = GL_TRUE;

    // Initialize glew for OpenGL3+ support
    GLenum glewInitError = glewInit();
    if(GLEW_OK != glewInitError) {
        std::cerr << glewGetErrorString(glewInitError) << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "OpenGL Version : " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLEW Version : " << glewGetString(GLEW_VERSION) << std::endl;

    /*********************************
     * HERE SHOULD COME THE INITIALIZATION CODE
     *********************************/
    
    //SHADER
    FilePath applicationPath(argv[0]);
    ShaderProgram shaderProgram(applicationPath);
    shaderProgram.m_Program.use();
    glEnable(GL_DEPTH_TEST);

    //TEXTURE
    GLuint panorama_nord;
    GLuint panorama_est;
    GLuint panorama_sud;
    GLuint panorama_ouest;
    GLuint panorama_haut;
    GLuint panorama_bas;
    GLuint texSphereStone;
    GLuint texSphereDirt;
    GLuint texSphereBricks;
	
    loadTexture("../GLImac-Template/assets/textures/corentin/panorama_0.png",&panorama_nord);
    loadTexture("../GLImac-Template/assets/textures/corentin/panorama_1.png",&panorama_est);
    loadTexture("../GLImac-Template/assets/textures/corentin/panorama_2.png",&panorama_sud);
    loadTexture("../GLImac-Template/assets/textures/corentin/panorama_3.png",&panorama_ouest);
    loadTexture("../GLImac-Template/assets/textures/corentin/panorama_4.png",&panorama_haut);
    loadTexture("../GLImac-Template/assets/textures/corentin/panorama_5.png",&panorama_bas);
    loadTexture("../GLImac-Template/assets/textures/corentin/stone.png",&texSphereStone);
    loadTexture("../GLImac-Template/assets/textures/corentin/dirt.png",&texSphereDirt);
    loadTexture("../GLImac-Template/assets/textures/corentin/bricks.png",&texSphereBricks);

    //SPHERE
    Sphere sphere(1,32,16);

    //RECTANGLE
    float squareSize = 10;
    float halfSquare = squareSize/2; 
    Rectangle rectangle(glm::vec3(-halfSquare,-halfSquare,0),glm::vec3(squareSize,0,0),glm::vec3(0,squareSize,0));

    FreeFlyCamera freeflyCamera = FreeFlyCamera(halfSquare*0.95);

    //VBO
    GLuint vboRectangle,vboSphere;

    glGenBuffers(1, &vboRectangle);
    glBindBuffer(GL_ARRAY_BUFFER, vboRectangle);
    glBufferData(GL_ARRAY_BUFFER, rectangle.getVertexCount()*sizeof(ShapeVertex), rectangle.getDataPointer(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1, &vboSphere);
    glBindBuffer(GL_ARRAY_BUFFER, vboSphere);
    glBufferData(GL_ARRAY_BUFFER, sphere.getVertexCount()*sizeof(ShapeVertex), sphere.getDataPointer(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //VAO
    GLuint vaoRectangle,vaoSphere;

    const GLuint VERTEX_ATTR_POSITION = 0;
    const GLuint VERTEX_ATTR_NORMAL = 1;
    const GLuint VERTEX_ATTR_TEXTURE = 2;
    
    

    //VAORECTANGLE
    glGenVertexArrays(1, &vaoRectangle);
    glBindVertexArray(vaoRectangle);
    
    glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
    glEnableVertexAttribArray(VERTEX_ATTR_NORMAL);
    glEnableVertexAttribArray(VERTEX_ATTR_TEXTURE);
    glBindBuffer(GL_ARRAY_BUFFER, vboRectangle);
    glVertexAttribPointer(VERTEX_ATTR_POSITION, 3, GL_FLOAT, GL_FALSE,sizeof(ShapeVertex),(const GLvoid*) (offsetof(ShapeVertex, position)));
    glVertexAttribPointer(VERTEX_ATTR_NORMAL, 3, GL_FLOAT, GL_FALSE,sizeof(ShapeVertex),(const GLvoid*) (offsetof(ShapeVertex, normal)));                     
    glVertexAttribPointer(VERTEX_ATTR_TEXTURE, 2, GL_FLOAT, GL_FALSE,sizeof(ShapeVertex),(const GLvoid*) (offsetof(ShapeVertex, texCoords)));   
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    //VAOSPHERE
    glGenVertexArrays(1, &vaoSphere);
    glBindVertexArray(vaoSphere);

    glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
    glEnableVertexAttribArray(VERTEX_ATTR_NORMAL);
    glEnableVertexAttribArray(VERTEX_ATTR_TEXTURE);
    glBindBuffer(GL_ARRAY_BUFFER, vboSphere);
    glVertexAttribPointer(VERTEX_ATTR_POSITION, 3, GL_FLOAT, GL_FALSE,sizeof(ShapeVertex),(const GLvoid*) (offsetof(ShapeVertex, position)));
    glVertexAttribPointer(VERTEX_ATTR_NORMAL, 3, GL_FLOAT, GL_FALSE,sizeof(ShapeVertex),(const GLvoid*) (offsetof(ShapeVertex, normal)));                     
    glVertexAttribPointer(VERTEX_ATTR_TEXTURE, 2, GL_FLOAT, GL_FALSE,sizeof(ShapeVertex),(const GLvoid*) (offsetof(ShapeVertex, texCoords)));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Application loop:
    bool done = false;
    while(!done) {
        // Event loop:
        SDL_Event e;
        while(windowManager.pollEvent(e)) {
            if(e.type == SDL_QUIT) {
                done = true; // Leave the loop after this iteration
            }
        }

        /*********************************
         * HERE SHOULD COME THE RENDERING CODE
         *********************************/
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        

         //CAMERA
        
        if(windowManager.isKeyPressed(SDLK_z)) freeflyCamera.moveFront(0.1);
        if(windowManager.isKeyPressed(SDLK_s)) freeflyCamera.moveFront(-0.1);
        if(windowManager.isKeyPressed(SDLK_q)) freeflyCamera.moveLeft(0.1);
        if(windowManager.isKeyPressed(SDLK_d)) freeflyCamera.moveLeft(-0.1);
        if(windowManager.isKeyPressed(SDLK_i)) freeflyCamera.rotateUp(2);
        if(windowManager.isKeyPressed(SDLK_k)) freeflyCamera.rotateUp(-2);
        if(windowManager.isKeyPressed(SDLK_j)) freeflyCamera.rotateLeft(2);
        if(windowManager.isKeyPressed(SDLK_l)) freeflyCamera.rotateLeft(-2);

         //MATRIX
        glm::mat4 ProjMatrix = glm::perspective(glm::radians(70.f),800.f/600.f,0.1f,100.f);
        glm::mat4 ViewMatrix = freeflyCamera.getViewMatrix();

        glBindVertexArray(vaoRectangle);
        
        glUniform1i(shaderProgram.uTexture, 0);
        glUniform1i(shaderProgram.uKdTexture, 1);
        glUniform1i(shaderProgram.uKsTexture, 2);
        glUniform1f(shaderProgram.uShininess, 32.0);

        glm::vec4 LightPos = glm::vec4(0, 0, 0, 1.0);
        glm::vec4 LightPos2 = glm::vec4(0.75*halfSquare, 0, 0.75*halfSquare, 1.0);

        LightPos = ViewMatrix*LightPos;
        LightPos2 = ViewMatrix*LightPos2;

        glUniform3f(shaderProgram.uLightPos_vs, LightPos.x, LightPos.y, LightPos.z);
        glUniform3f(shaderProgram.uLightPos_vs2, LightPos2.x, LightPos2.y, LightPos2.z);

        glUniform3f(shaderProgram.uLightIntensity, 10.0, 10.0,10.0);
        glUniform3f(shaderProgram.uLightIntensity2, 5, 5, 5);
        
        //TEXTURE&DRAWING
		
		//North
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,panorama_nord);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, panorama_nord);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, panorama_nord);

        glm::mat4 panoramaMVMatrixNorth = glm::translate(ViewMatrix,glm::vec3(0,0,-halfSquare));
        glm::mat4 panoramaNormalMatrixNorth = glm::transpose(glm::inverse(panoramaMVMatrixNorth)) ;
        glm::mat4 panoramaMVPMatrixNorth = ProjMatrix * panoramaMVMatrixNorth;

        glUniformMatrix4fv(shaderProgram.uMVPMatrix,1,GL_FALSE,glm::value_ptr(panoramaMVPMatrixNorth));
        glUniformMatrix4fv(shaderProgram.uMVMatrix,1,GL_FALSE,glm::value_ptr(panoramaMVMatrixNorth));
        glUniformMatrix4fv(shaderProgram.uNormalMatrix,1,GL_FALSE,glm::value_ptr(panoramaNormalMatrixNorth));

        glDrawArrays(GL_TRIANGLES,0,rectangle.getVertexCount());
		
		glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D,0);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D,0);

        //East
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,panorama_est);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, panorama_est);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, panorama_est);
		
        glm::mat4 panoramaMVMatrixEast = glm::translate(ViewMatrix,glm::vec3(halfSquare,0,0));
        panoramaMVMatrixEast = glm::rotate(panoramaMVMatrixEast,glm::radians(-90.f),glm::vec3(0,1,0));
        glm::mat4 panoramaNormalMatrixEast = glm::transpose(glm::inverse(panoramaMVMatrixEast)) ;
        glm::mat4 panoramaMVPMatrixEast = ProjMatrix * panoramaMVMatrixEast;

        glUniformMatrix4fv(shaderProgram.uMVPMatrix,1,GL_FALSE,glm::value_ptr(panoramaMVPMatrixEast));
        glUniformMatrix4fv(shaderProgram.uMVMatrix,1,GL_FALSE,glm::value_ptr(panoramaMVMatrixEast));
        glUniformMatrix4fv(shaderProgram.uNormalMatrix,1,GL_FALSE,glm::value_ptr(panoramaNormalMatrixEast));

        glDrawArrays(GL_TRIANGLES,0,rectangle.getVertexCount());
		
		glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D,0);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D,0);

        //South
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,panorama_sud);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, panorama_sud);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, panorama_sud);
		
        glm::mat4 panoramaMVMatrixSouth = glm::translate(ViewMatrix,glm::vec3(0,0,halfSquare));
        panoramaMVMatrixSouth = glm::rotate(panoramaMVMatrixSouth,glm::radians(180.f),glm::vec3(0,1,0));
        glm::mat4 panoramaNormalMatrixSouth = glm::transpose(glm::inverse(panoramaMVMatrixSouth)) ;
        glm::mat4 panoramaMVPMatrixSouth = ProjMatrix * panoramaMVMatrixSouth;

        glUniformMatrix4fv(shaderProgram.uMVPMatrix,1,GL_FALSE,glm::value_ptr(panoramaMVPMatrixSouth));
        glUniformMatrix4fv(shaderProgram.uMVMatrix,1,GL_FALSE,glm::value_ptr(panoramaMVMatrixSouth));
        glUniformMatrix4fv(shaderProgram.uNormalMatrix,1,GL_FALSE,glm::value_ptr(panoramaNormalMatrixSouth));

        glDrawArrays(GL_TRIANGLES,0,rectangle.getVertexCount());
		
		glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D,0);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D,0);

        //West
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,panorama_ouest);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, panorama_ouest);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, panorama_ouest);
		
        glm::mat4 panoramaMVMatrixWest = glm::translate(ViewMatrix,glm::vec3(-halfSquare,0,0));
        panoramaMVMatrixWest = glm::rotate(panoramaMVMatrixWest,glm::radians(90.f),glm::vec3(0,1,0));
        glm::mat4 panoramaNormalMatrixWest = glm::transpose(glm::inverse(panoramaMVMatrixWest)) ;
        glm::mat4 panoramaMVPMatrixWest = ProjMatrix * panoramaMVMatrixWest;

        glUniformMatrix4fv(shaderProgram.uMVPMatrix,1,GL_FALSE,glm::value_ptr(panoramaMVPMatrixWest));
        glUniformMatrix4fv(shaderProgram.uMVMatrix,1,GL_FALSE,glm::value_ptr(panoramaMVMatrixWest));
        glUniformMatrix4fv(shaderProgram.uNormalMatrix,1,GL_FALSE,glm::value_ptr(panoramaNormalMatrixWest));

        glDrawArrays(GL_TRIANGLES,0,rectangle.getVertexCount());

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D,0);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D,0);

        //Down
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,panorama_bas);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, panorama_bas);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, panorama_bas);

        glm::mat4 panoramaMVMatrixDown = glm::translate(ViewMatrix,glm::vec3(0,-halfSquare,0));
        panoramaMVMatrixDown = glm::rotate(panoramaMVMatrixDown,glm::radians(-90.f),glm::vec3(1,0,0));
        glm::mat4 panoramaNormalMatrixDown = glm::transpose(glm::inverse(panoramaMVMatrixDown)) ;
        glm::mat4 panoramaMVPMatrixDown = ProjMatrix * panoramaMVMatrixDown;

        glUniformMatrix4fv(shaderProgram.uMVPMatrix,1,GL_FALSE,glm::value_ptr(panoramaMVPMatrixDown));
        glUniformMatrix4fv(shaderProgram.uMVMatrix,1,GL_FALSE,glm::value_ptr(panoramaMVMatrixDown));
        glUniformMatrix4fv(shaderProgram.uNormalMatrix,1,GL_FALSE,glm::value_ptr(panoramaNormalMatrixDown));

        glDrawArrays(GL_TRIANGLES,0,rectangle.getVertexCount());

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D,0);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D,0);

        //Top
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,panorama_haut);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, panorama_haut);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, panorama_haut);

        glm::mat4 panoramaMVMatrixUp = glm::translate(ViewMatrix,glm::vec3(0,halfSquare,0));
        panoramaMVMatrixUp = glm::rotate(panoramaMVMatrixUp,glm::radians(90.f),glm::vec3(1,0,0));
        glm::mat4 panoramaNormalMatrixUp = glm::transpose(glm::inverse(panoramaMVMatrixUp)) ;
        glm::mat4 panoramaMVPMatrixUp = ProjMatrix * panoramaMVMatrixUp;

        glUniformMatrix4fv(shaderProgram.uMVPMatrix,1,GL_FALSE,glm::value_ptr(panoramaMVPMatrixUp));
        glUniformMatrix4fv(shaderProgram.uMVMatrix,1,GL_FALSE,glm::value_ptr(panoramaMVMatrixUp));
        glUniformMatrix4fv(shaderProgram.uNormalMatrix,1,GL_FALSE,glm::value_ptr(panoramaNormalMatrixUp));

        glDrawArrays(GL_TRIANGLES,0,rectangle.getVertexCount());

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D,0);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D,0);


        glBindVertexArray(0);

        //Sphere
        glBindVertexArray(vaoSphere);
        
        glUniform1i(shaderProgram.uTexture, 0);
        glUniform1i(shaderProgram.uKdTexture, 1);
        glUniform1i(shaderProgram.uKsTexture, 2);
        glUniform1f(shaderProgram.uShininess, 32.0);

        glUniform3f(shaderProgram.uLightPos_vs, LightPos.x, LightPos.y, LightPos.z);
        glUniform3f(shaderProgram.uLightPos_vs2, LightPos2.x, LightPos2.y, LightPos2.z);

        glUniform3f(shaderProgram.uLightIntensity, 10.0, 10.0,10.0);
        glUniform3f(shaderProgram.uLightIntensity2, 5, 5, 5);

        //Stone
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,texSphereStone);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texSphereStone);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, texSphereStone);

        glm::mat4 stoneMVMatrix = glm::translate(ViewMatrix,glm::vec3(0.75*halfSquare,-0.75*halfSquare,0.75*halfSquare));
        glm::mat4 stoneNormalMatrix = glm::transpose(glm::inverse(stoneMVMatrix)) ;
        glm::mat4 stoneMVPMatrix = ProjMatrix * stoneMVMatrix;

        glUniformMatrix4fv(shaderProgram.uMVPMatrix,1,GL_FALSE,glm::value_ptr(stoneMVPMatrix));
        glUniformMatrix4fv(shaderProgram.uMVMatrix,1,GL_FALSE,glm::value_ptr(stoneMVMatrix));
        glUniformMatrix4fv(shaderProgram.uNormalMatrix,1,GL_FALSE,glm::value_ptr(stoneNormalMatrix));

        glDrawArrays(GL_TRIANGLES,0,sphere.getVertexCount());

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D,0);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D,0);
		
		//Dirt
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,texSphereDirt);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texSphereDirt);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, texSphereDirt);

        glm::mat4 dirtMVMatrix = glm::translate(ViewMatrix,glm::vec3(0.25*halfSquare,-0.75*halfSquare,0.75*halfSquare));
        glm::mat4 dirtNormalMatrix = glm::transpose(glm::inverse(dirtMVMatrix)) ;
        glm::mat4 dirtMVPMatrix = ProjMatrix * dirtMVMatrix;

        glUniformMatrix4fv(shaderProgram.uMVPMatrix,1,GL_FALSE,glm::value_ptr(dirtMVPMatrix));
        glUniformMatrix4fv(shaderProgram.uMVMatrix,1,GL_FALSE,glm::value_ptr(dirtMVMatrix));
        glUniformMatrix4fv(shaderProgram.uNormalMatrix,1,GL_FALSE,glm::value_ptr(dirtNormalMatrix));

        glDrawArrays(GL_TRIANGLES,0,sphere.getVertexCount());

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D,0);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D,0);

		//Briks
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,texSphereBricks);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texSphereBricks);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, texSphereBricks);

        glm::mat4 bricksMVMatrix = glm::translate(ViewMatrix,glm::vec3(0.75*halfSquare,-0.75*halfSquare,0.25*halfSquare));
        glm::mat4 bricksNormalMatrix = glm::transpose(glm::inverse(bricksMVMatrix)) ;
        glm::mat4 bricksMVPMatrix = ProjMatrix * bricksMVMatrix;

        glUniformMatrix4fv(shaderProgram.uMVPMatrix,1,GL_FALSE,glm::value_ptr(bricksMVPMatrix));
        glUniformMatrix4fv(shaderProgram.uMVMatrix,1,GL_FALSE,glm::value_ptr(bricksMVMatrix));
        glUniformMatrix4fv(shaderProgram.uNormalMatrix,1,GL_FALSE,glm::value_ptr(bricksNormalMatrix));

        glDrawArrays(GL_TRIANGLES,0,sphere.getVertexCount());

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D,0);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D,0);

        glBindVertexArray(0);
        // Update the display
        windowManager.swapBuffers();
    }

    return EXIT_SUCCESS;
}

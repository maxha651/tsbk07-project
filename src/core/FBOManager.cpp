//
// Created by max on 2015-05-11.
//

#include <FBOManager.h>
#include <assert.h>
#include <GL/freeglut_std.h>

FBOManager::FBOManager() {

}

unsigned int FBOManager::AddFBO(unsigned int width, unsigned int height) {
    struct FBOData newData;

    newData.width = width;
    newData.height = height;

    // Generate an FBO and bind it to the pipeline
    glGenFramebuffers(1, &newData.fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, newData.fbo);

    newData.colorTex = GenerateColorTexture(width, height);
    newData.depthTex = GenerateDepthTexture(width, height);

    // Bind textures to pipeline. texture_depth is optional
    // 0 is the mipmap level. 0 is the heightest

    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, newData.colorTex, 0);
    glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, newData.depthTex);

    //drawbuffer.push_back(GL_COLOR_ATTACHMENT0);
    //glDrawBuffers(drawbuffer.size(), &drawbuffer[0]);

    // Check for FBO completeness
    if (glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
        std::cerr << "FBOManager: Error! FrameBuffer is not complete" << std::endl;
        assert(false);
    }

    // Unbind buffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    fboMap.insert(std::pair<unsigned int, struct FBOData>(newData.fbo, newData));

    return newData.fbo;
}

bool FBOManager::ActivateFBO(unsigned int fbo) {
    try {
        FBOData data = fboMap.find(fbo)->second;
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, data.colorTex);
        //glEnable(GL_TEXTURE_2D);
        //glBindTexture(GL_TEXTURE_2D, 0);
        glBindFramebuffer(GL_FRAMEBUFFER, data.fbo);

        glClearColor(0.0,0.0,0.0,0.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glEnable(GL_DEPTH_TEST);

        // TODO This should be handled better
        GLuint dBuffer[] = { GL_COLOR_ATTACHMENT0, GL_DEPTH_ATTACHMENT };
        glDrawBuffers(2, dBuffer);
        glViewport(0, 0, data.width, data.height);

        current_fbo = data.fbo;

        // Check for FBO completeness
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            std::cerr << "FBOManager: Error! FrameBuffer is not complete" << std::endl;
            assert(false);
        }
        return true;
    }
    catch (std::exception& e) {
        std::cout << e.what() << std::endl;
        assert(false);
    }
}

void FBOManager::DeactivateFBO() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClearColor(.5,.5,.5,1.0);
    glViewport(0, 0, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
    current_fbo = 0;
}

unsigned int FBOManager::getColorTexture() {
    return getColorTexture(current_fbo);
}

unsigned int FBOManager::getDepthTexture() {
    return getDepthTexture(current_fbo);
}

unsigned int FBOManager::getColorTexture(unsigned int fbo) {
    try {
        return fboMap.find(current_fbo)->second.colorTex;
    } catch  (std::exception& e) {
        std::cout << e.what() << std::endl;
        assert(false);
    }
}

unsigned int FBOManager::getDepthTexture(unsigned int fbo) {
    try {
        return fboMap.find(fbo)->second.depthTex;
    } catch  (std::exception& e) {
        std::cout << e.what() << std::endl;
        assert(false);
    }
}

GLuint FBOManager::GenerateColorTexture(unsigned int width, unsigned int height) {
    GLuint tex;

    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

    return tex;
}

GLuint FBOManager::GenerateDepthTexture(unsigned int width, unsigned int height) {
    GLuint depth;

    glGenRenderbuffers(1, &depth);
    glBindRenderbuffer(GL_RENDERBUFFER, depth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);

    return depth;
}
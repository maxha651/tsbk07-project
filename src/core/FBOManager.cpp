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

    // Attach textures
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, newData.colorTex, 0);
    glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, newData.depthTex);

    // Check for FBO completeness
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
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
        FBOData data = fboMap[fbo];
        // Bind us
        glBindFramebuffer(GL_FRAMEBUFFER, data.fbo);

        // Rendering to draw buffers ? I think this is location in shader. Not sure
        glViewport(0, 0, data.width, data.height);

        // Check for FBO completeness
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            DeactivateFBO();
            std::cerr << "FBOManager: Error! FrameBuffer is not complete" << std::endl;
            assert(false);
        }
        current_fbo = data.fbo;
        return true;
    }
    catch (std::exception& e) {
        std::cout << e.what() << std::endl;
        assert(false);
    }
}

void FBOManager::DeactivateFBO() {
    if (current_fbo != 0) {
        glBindTexture(GL_TEXTURE_2D, fboMap[current_fbo].colorTex);
    }
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
        return fboMap[fbo].colorTex;
    } catch  (std::exception& e) {
        std::cout << e.what() << std::endl;
        assert(false);
    }
}

unsigned int FBOManager::getDepthTexture(unsigned int fbo) {
    try {
        return fboMap[fbo].depthTex;
    } catch  (std::exception& e) {
        std::cout << e.what() << std::endl;
        assert(false);
    }
}

GLuint FBOManager::GenerateColorTexture(unsigned int width, unsigned int height) {
    GLuint tex;

    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    // Specify image (most importantly, set mipmap level)
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, NULL);
    // Generate (this allocates memory depending on glTexImage2D info, otherwise use glTexStorage)
    glGenerateMipmap(GL_TEXTURE_2D);
    // Set sensible paramters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    return tex;
}

GLuint FBOManager::GenerateDepthTexture(unsigned int width, unsigned int height) {
    GLuint depth;

    glGenRenderbuffers(1, &depth);
    glBindRenderbuffer(GL_RENDERBUFFER, depth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);

    return depth;
}
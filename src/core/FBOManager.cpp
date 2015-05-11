//
// Created by max on 2015-05-11.
//

#include <FBOManager.h>

FBOManager::FBOManager() {

}

unsigned int FBOManager::AddFBO(unsigned int width, unsigned int height) {
    struct FBOData newData;

    // Generate an FBO and bind it to the pipeline
    glGenFramebuffers(1, &newData.fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, newData.fbo);

    newData.colorTex = GenerateColorTexture(width, height);
    newData.depthTex = GenerateDepthTexture(width, height);

    // Bind textures to pipeline. texture_depth is optional
    // 0 is the mipmap level. 0 is the heightest

    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, newData.colorTex, 0);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, newData.depthTex, 0);

    // I don't understand this
    //drawbuffer.push_back(GL_COLOR_ATTACHMENT0);
    //glDrawBuffers(drawbuffer.size(), &drawbuffer[0]);

    // Check for FBO completeness
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
        std::cout << "FBOManager: Error! FrameBuffer is not complete" << std::endl;
        std::cin.get();
        std::terminate();
    }

    // Unbind buffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    fboMap.insert(std::pair<unsigned int, struct FBOData>(newData.fbo, newData));
}

bool FBOManager::ActivateFBO(unsigned int fbo) {
    try {
        FBOData data = fboMap.at(fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, data.fbo);
        current_fbo = data.fbo;
        return true;
    }
    catch (...) {
        return false;
    }
}

void FBOManager::DeactivateFBO() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
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
        return fboMap.at(current_fbo).colorTex;
    } catch  (...) {
        return 0;
    }
}

unsigned int FBOManager::getDepthTexture(unsigned int fbo) {
    try {
        return fboMap.at(fbo).depthTex;
    } catch  (...) {
        return 0;
    }
}

GLuint FBOManager::GenerateColorTexture(unsigned int width, unsigned int height) {
    GLuint tex;

    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

    return tex;
}

GLuint FBOManager::GenerateDepthTexture(unsigned int width, unsigned int height) {
    GLuint tex;

    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);

    return tex;
}
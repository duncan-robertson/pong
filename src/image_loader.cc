#include "image_loader.hh"

GLuint loadBMP_custom(const char* imagepath) {
    unsigned char header[54];
    unsigned int dataPos;
    unsigned int width, height;
    unsigned int imageSize;
    unsigned char* data;

    FILE* file = fopen(imagepath, "rb");
    if (!file) {
        printf("Image could not be opened\n");
        return 0;

    }

    if (fread(header, 1, 54, file)!=54) {
        printf("BMP verification failed\n");
        return 0;
    }

    if (header[0] != 'B' || header[1] != 'M') {
        printf("BMP verification failed\n");
        return 0;
    }

    dataPos = *(int*)&(header[0x0A]);
    imageSize = *(int*)&(header[0x22]);
    width = *(int*)&(header[0x12]);
    height = *(int*)&(header[0x16]);

    //Bad image size, calculate it with other values
    if (imageSize==0) {
        imageSize = width*height*3;
    }
    //Bad dataPos, set it to the end of the header
    if (dataPos==0) {
        dataPos = 54;
    }

    data = new unsigned char [imageSize];
    fread(data,1,imageSize,file);
    fclose(file);

    GLuint textureID;
    glGenTextures(1,&textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

    //Linear filtering when maginfying
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //Linear mipmap filtering when minifying
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    //Generate mipmaps
    glGenerateMipmap(GL_TEXTURE_2D);
    
    return textureID;
}

GLuint loadBMP_SDL(const char* imagepath) {
    SDL_Surface* textureImage;
    textureImage = SDL_LoadBMP(imagepath);
    if(!textureImage) {
        printf("Failed to load image\n");
        SDL_FreeSurface(textureImage);
        
        return 0;
    }
    else {
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureImage->w, textureImage->h, 0, GL_RGB, GL_UNSIGNED_BYTE, textureImage->pixels);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glGenerateMipmap(GL_TEXTURE_2D);

        SDL_FreeSurface(textureImage);

        return texture;
    }
}

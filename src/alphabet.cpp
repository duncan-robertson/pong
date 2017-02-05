#include <stdio.h>

#include "alphabet.h"

GLuint loadAlphabet(void) {
    FILE *fp;

    fp = fopen("textures/alphabet.data", "rb");
    if(fp) {
        //Loading a known quantity of image data from memory
        //width 50 height 28 RGB
        GLubyte data[5600];
        GLubyte transparency_match[3] = {0xff, 0xff, 0xff};
        GLubyte color_match[3] = {0x00, 0x00, 0x00};
        GLubyte color_replace[3] = {0xff, 0xff, 0xff};

        //load rgb image into data
        for(unsigned int i=0; i<sizeof(data); i+=4) {
            fread(data+i, 1, 3, fp);

            //Replace background color with transparency
            if(data[i] == transparency_match[0] && 
                    data[i+1] == transparency_match[1] && 
                    data[i+2] == transparency_match[2]) {

                data[i] = 0x00;
                data[i+1] = 0x00;
                data[i+2] = 0x00;
                data[i+3] = 0x00;
            }
            //Replace foreground color with desired foreground color
            else if(data[i] == color_match[0] &&
                    data[i+1] == color_match[1] &&
                    data[i+2] == color_match[2]) {

                data[i] = color_replace[0];
                data[i+1] = color_replace[1];
                data[i+2] = color_replace[2];
                data[i+3] = 0xff;
            }
            //Touch nothing else but make sure it's visible
            else {
                data[i+3] = 0xff;
            }
        }
        fclose(fp);

        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 50, 28, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        return texture;
    }
    else {
        return 0;
    }
}

#define STB_IMAGE_IMPLEMENTATION
#include "stbImage.hpp"

const GLenum targets[6] =
	{
			GL_TEXTURE_CUBE_MAP_POSITIVE_X,
			GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
			GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
			GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
			GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
			GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
	};

class Texture {
    public: 
        GLuint id;
        char const* path;
        int width, height;
        int channels;
        GLenum target;

        Texture(char const* path); 
        Texture(const char* files[6]);
};

Texture::Texture(char const* path) {
    this->target = GL_TEXTURE_2D;

    stbi_set_flip_vertically_on_load(1);

    unsigned char* data = stbi_load(path, &(this->width), &(this->height),
                                 &(this->channels), 0);

    glGenTextures(1, &(this->id));
    glBindTexture(GL_TEXTURE_2D, this->id);

    if(channels == 3) //RGB
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    else if(channels == 4) //RGBA
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);
};

//constructor for implementing skybox texture
Texture::Texture(const char* files[6]) {
    this->target = GL_TEXTURE_CUBE_MAP;
    
    glGenTextures(1, &(this->id));
    glBindTexture(GL_TEXTURE_CUBE_MAP, this->id);

    for(int i=0; i<6; i++) {
        stbi_set_flip_vertically_on_load(0);
        unsigned char* data = stbi_load(files[i], &(this->width), &(this->height),
                                &(this->channels), 0);
        if(channels == 3) //RGB
            glTexImage2D(targets[i], 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        else if(channels == 4) //RGBA
            glTexImage2D(targets[i], 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    
    stbi_image_free(data);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_REPEAT);
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
}




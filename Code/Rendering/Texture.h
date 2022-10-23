#pragma once

class Texture
{
protected:
    unsigned int width;
    unsigned int height;
    unsigned int channels;
    unsigned int handle;
private:
    Texture(const Texture& other);
    Texture& operator=(const Texture& other);
public:
    Texture();
    Texture(const char* path);
    ~Texture();

    void Load(const char* path);

    void Set(unsigned int uniformIndex, unsigned int textureIndex);
    void UnSet(unsigned int textureIndex);
    unsigned int GetHandle();
};

#pragma once

#include <array>
#include <fstream>
#include <sstream>
#include <streambuf>
#include <string>
#include <vector>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stb_image.h>

#include "GLUtils.h"
#include "Input.h"

extern std::string
ReadFile(const char* _path);

class Texture
{
private:
  Texture(const Texture&) = delete;
  Texture& operator=(const Texture&) = delete;

public:
  Texture() = default;

  Texture(Texture&& _texture)
    : m_Texture{ _texture.m_Texture }
  {
    _texture.m_Texture = 0;
  }

  ~Texture()
  {
    if (m_Texture)
      glDeleteTextures(1, &m_Texture);
  }

  bool init(const char* _path)
  {
    int width, height, nrChannels;
    unsigned char* data = stbi_load(_path, &width, &height, &nrChannels, 3);

    glGenTextures(1, &m_Texture);
    glBindTexture(GL_TEXTURE_2D, m_Texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(
      GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RGB,
                 width,
                 height,
                 0,
                 GL_RGB,
                 GL_UNSIGNED_BYTE,
                 data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);
    return true;
  }

  void bind(GLenum _slot)
  {
    glActiveTexture(_slot);
    glBindTexture(GL_TEXTURE_2D, m_Texture);
  }

private:
  GLuint m_Texture{ 0 };
};

class Program
{
public:
  ~Program()
  {
    if (m_Program != 0) {
      glDeleteShader(m_Program);
    }
  }

  bool init(const char* _vs, const char* _fs)
  {
    GLuint vs;
    GLuint fs;

    if (compile(_vs, GL_VERTEX_SHADER, vs) &&
        compile(_fs, GL_FRAGMENT_SHADER, fs)) {

      GLuint programHandle = glCreateProgram();
      glAttachShader(programHandle, vs);
      glAttachShader(programHandle, fs);
      glLinkProgram(programHandle);

      GLint infoLogLength = 0;
      glGetProgramiv(programHandle, GL_INFO_LOG_LENGTH, &infoLogLength);

      if (infoLogLength) {
        GLchar infoLog[512] = { 0 };

        glGetProgramInfoLog(programHandle, 512, nullptr, infoLog);
        infoLog[511] = 0;

        LOG_INFO(infoLog);
      }

      glDetachShader(programHandle, vs);
      glDeleteShader(vs);

      glDetachShader(programHandle, fs);
      glDeleteShader(fs);

      GLint linkStatus = GL_FALSE;
      glGetProgramiv(programHandle, GL_LINK_STATUS, &linkStatus);

      if (linkStatus == GL_FALSE) {
        glDeleteProgram(programHandle);
      } else {
        m_Program = programHandle;
      }
    }

    return m_Program != 0;
  }

  void bind() { glUseProgram(m_Program); }

  GLuint getHandle() const { return m_Program; }

private:
  bool compile(const char* _shader, GLenum _type, GLuint& _handle)
  {
    _handle = glCreateShader(_type);
    glShaderSource(_handle, 1, &_shader, nullptr);
    glCompileShader(_handle);

    GLint infoLogLength = 0;
    glGetShaderiv(_handle, GL_INFO_LOG_LENGTH, &infoLogLength);

    if (infoLogLength) {
      GLchar infoLog[512] = { 0 };

      glGetShaderInfoLog(_handle, 512, nullptr, infoLog);
      infoLog[511] = 0;

      LOG_INFO(infoLog);
    }

    GLint compileStatus = GL_FALSE;
    glGetShaderiv(_handle, GL_COMPILE_STATUS, &compileStatus);

    if (compileStatus == GL_FALSE) {
      glDeleteShader(_handle);
      _handle = 0;

      return false;
    }

    return true;
  }

private:
  GLuint m_Program{ 0 };
};

class Camera
{
public:
  Camera()
  {
    m_Pos = glm::vec3{ 0.0f, 50.0f, 50.0f };
    m_Forward = glm::vec3{ 0.0f, 0.0f, 0.0f } - m_Pos;
    m_Up = glm::vec3{ 0.0f, 1.0f, 0.0f };
  }

  ~Camera() {}

  void update(float _delta)
  {
    m_Forward = glm::normalize(glm::vec3{ 0.0f, 0.0f, 0.0f } - m_Pos);
    glm::vec3 forwardDisplacement = glm::vec3{ 0.0f, 0.0f, 0.0f };

    if (Input::isKeyPressed(GLFW_KEY_UP))
      forwardDisplacement = glm::normalize(m_Forward) * 10.0f * _delta;

    if (Input::isKeyPressed(GLFW_KEY_DOWN))
      forwardDisplacement = glm::normalize(m_Forward) * -10.0f * _delta;

    glm::vec3 sideDisplacement = glm::vec3{ 0.0f, 0.0f, 0.0f };
    glm::vec3 sideDisplacementDirection =
      glm::normalize(glm::cross(m_Forward, glm::vec3{ 0.0f, 1.0f, 0.0f }));

    if (Input::isKeyPressed(GLFW_KEY_LEFT))
      ;

    if (Input::isKeyPressed(GLFW_KEY_RIGHT))
      ;

    if (glm::length(forwardDisplacement) > 0.1f) {
      m_Pos += forwardDisplacement;
    }
  }

  glm::mat4 getVPMatrix() const
  {
    glm::mat4 view = glm::lookAt(m_Pos, m_Pos + m_Forward, m_Up);

    glm::mat4 projection =
      glm::perspective(glm::radians(60.0f), 800.0f / 600.0f, 0.1f, 1000.0f);

    return projection * view;
  }

private:
  glm::vec3 m_Pos;
  glm::vec3 m_Forward;
  glm::vec3 m_Up;
};

class Terrain
{
  enum Layer
  {
    DIRT = 0,
    GRASS,
    ROCK,
    SNOW,

    COUNT
  };

  struct TextureLayer
  {
    Texture texture;

    float minHeight;
    float maxHeight;
  };

  struct VtxAttributes
  {
    float pos[3];
    float normal[3];
    float texCoord[2];
  };

public:
  struct InitInfo
  {
    int gridCountX{ 0 };
    int gridCountZ{ 0 };

    float gridSizeX{ 0.0f };
    float gridSizeZ{ 0.0f };
  };

public:
  ~Terrain()
  {
    if (m_VAO)
      glDeleteVertexArrays(1, &m_VAO);

    if (m_VBO)
      glDeleteBuffers(1, &m_VBO);

    if (m_EBO)
      glDeleteBuffers(1, &m_EBO);
  }

  bool init(const InitInfo& _info)
  {
    std::string vs = ReadFile("data/shaders/layered_terrain.vs");
    std::string fs = ReadFile("data/shaders/layered_terrain.fs");

    if (!m_Program.init(vs.c_str(), fs.c_str()))
      return false;

    // === Generate terrain data

    glm::vec3 startPos;
    startPos.x = -_info.gridSizeX / 2.0f;
    startPos.y = 0.0f;
    startPos.z = _info.gridSizeZ / 2.0f;

    glm::vec3 step;
    step.x = _info.gridSizeX / static_cast<float>(_info.gridCountX);
    step.y = 0.0f;
    step.z = -_info.gridSizeZ / static_cast<float>(_info.gridCountZ);

    std::vector<VtxAttributes> vtx;
    vtx.reserve(_info.gridCountX * _info.gridCountZ);

    for (int z = 0; z < _info.gridCountZ; ++z) {
      for (int x = 0; x < _info.gridCountX; ++x) {
        VtxAttributes& att = vtx.emplace_back();

        att.pos[0] = (startPos.x + step.x * static_cast<float>(x));
        att.pos[1] = (0.0f);
        att.pos[2] = (startPos.z + step.z * static_cast<float>(z));

        att.texCoord[0] =
          (static_cast<float>(x) / static_cast<float>(_info.gridCountX));
        att.texCoord[1] =
          (static_cast<float>(z) / static_cast<float>(_info.gridCountZ));
      }
    }

    int zs = _info.gridCountZ - 1;
    int xs = _info.gridCountX - 1;
    int ts = zs * xs;
    int ti = ts * 6;

    std::vector<unsigned> idx;
    idx.reserve(ti);

    for (int zS = 0; zS < zs; ++zS) {
      for (int xS = 0; xS < xs; ++xS) {
        unsigned t11 = zS * _info.gridCountX + xS;
        unsigned t12 = zS * _info.gridCountX + xS + 1;
        unsigned t13 = (zS + 1) * _info.gridCountX + xS;

        unsigned t21 = zS * _info.gridCountX + xS + 1;
        unsigned t22 = (zS + 1) * _info.gridCountX + xS + 1;
        unsigned t23 = (zS + 1) * _info.gridCountX + xS;

        idx.push_back(t11);
        idx.push_back(t12);
        idx.push_back(t13);

        idx.push_back(t21);
        idx.push_back(t22);
        idx.push_back(t23);
      }
    }

    m_IndicesCount = idx.size();

    // Create GL objects

    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    glGenBuffers(1, &m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(VtxAttributes) * vtx.size(),
                 vtx.data(),
                 GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(VtxAttributes),
                          (void*)offsetof(VtxAttributes, pos));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(VtxAttributes),
                          (void*)offsetof(VtxAttributes, normal));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2,
                          2,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(VtxAttributes),
                          (void*)offsetof(VtxAttributes, texCoord));

    glGenBuffers(1, &m_EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 sizeof(unsigned) * idx.size(),
                 idx.data(),
                 GL_STATIC_DRAW);

    glBindVertexArray(m_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);

    // Create layers

    if (!m_Heightmap.init("data/textures/perlin_512x512.jpg") ||
        !m_Layers[DIRT].texture.init("data/textures/dirt_512x512.jpg") ||
        !m_Layers[GRASS].texture.init("data/textures/grass_512x512.png") ||
        !m_Layers[ROCK].texture.init("data/textures/rock_512x512.jpg") ||
        !m_Layers[SNOW].texture.init("data/textures/snow_512x512.jpg"))
      return false;

    m_Layers[DIRT].minHeight = 0.0f;
    m_Layers[DIRT].maxHeight = 0.3f;

    m_Layers[GRASS].minHeight = 0.1f;
    m_Layers[GRASS].maxHeight = 0.4f;

    m_Layers[ROCK].minHeight = 0.3f;
    m_Layers[ROCK].maxHeight = 0.7f;

    m_Layers[SNOW].minHeight = 0.6f;
    m_Layers[SNOW].maxHeight = 1.0f;

    return true;
  }

  void update(float _delta, const Camera& _camera)
  {
    m_Program.bind();

    glm::mat4 mMtx = glm::mat4(1.0f);
    glm::mat4 vpMtx = _camera.getVPMatrix();

    glUniformMatrix4fv(glGetUniformLocation(m_Program.getHandle(), "uM"),
                       1,
                       GL_FALSE,
                       &mMtx[0][0]);

    glUniformMatrix4fv(glGetUniformLocation(m_Program.getHandle(), "uVP"),
                       1,
                       GL_FALSE,
                       &vpMtx[0][0]);

    m_Heightmap.bind(GL_TEXTURE0);
    glUniform1i(glGetUniformLocation(m_Program.getHandle(), "uHeightmap"), 0);

    for (int i = 0; i < m_Layers.size(); ++i) {
      TextureLayer& layer = m_Layers[i];
      layer.texture.bind(GL_TEXTURE1 + i);

      std::ostringstream ss;
      ss << "uLayers[" << i << "].layerTexture";

      glUniform1i(glGetUniformLocation(m_Program.getHandle(), ss.str().c_str()),
                  i + 1);

      ss.str("");
      ss.clear();
      ss << "uLayers[" << i << "].minHeight";

      glUniform1f(glGetUniformLocation(m_Program.getHandle(), ss.str().c_str()),
                  layer.minHeight);

      ss.str("");
      ss.clear();
      ss << "uLayers[" << i << "].maxHeight";

      glUniform1f(glGetUniformLocation(m_Program.getHandle(), ss.str().c_str()),
                  layer.maxHeight);
    }

    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, m_IndicesCount, GL_UNSIGNED_INT, nullptr);
  }

private:
  Texture m_Heightmap;
  std::array<TextureLayer, COUNT> m_Layers;

  Program m_Program;

  GLuint m_VAO{ 0 };
  GLuint m_VBO{ 0 };
  GLuint m_EBO{ 0 };

  unsigned m_IndicesCount{ 0 };
};

class Game
{
public:
  bool init()
  {
    glViewport(0, 0, 800, 600);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    Terrain::InitInfo info;
    info.gridCountX = 128;
    info.gridCountZ = 128;
    info.gridSizeX = 128.0f;
    info.gridSizeZ = 128.0f;

    return m_Terrain.init(info);
  }

  void update(float _delta)
  {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_Camera.update(_delta);
    m_Terrain.update(_delta, m_Camera);
  }

private:
  Camera m_Camera;
  Terrain m_Terrain;
};
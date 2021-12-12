// Project includes.
#include "Graphics/Buffers.h"

// OpenGL includes.
#include "glad/glad.h"

namespace Strontium
{
  //----------------------------------------------------------------------------
  // Vertex buffer here.
  //----------------------------------------------------------------------------
  VertexBuffer::VertexBuffer(const void* bufferData, const unsigned &dataSize,
                             BufferType bufferType)
    : hasData(true)
    , type(bufferType)
    , bufferData(bufferData)
    , dataSize(dataSize)
  {
    glGenBuffers(1, &this->bufferID);
    glBindBuffer(GL_ARRAY_BUFFER, this->bufferID);
    glBufferData(GL_ARRAY_BUFFER, dataSize, bufferData, static_cast<GLenum>(bufferType));
  }

  VertexBuffer::~VertexBuffer()
  {
    glDeleteBuffers(1, &this->bufferID);
  }

  // Bind the buffer.
  void
  VertexBuffer::bind()
  {
    glBindBuffer(GL_ARRAY_BUFFER, this->bufferID);
  }

  // Unbind the buffer.
  void
  VertexBuffer::unbind()
  {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }

  //----------------------------------------------------------------------------
  // Index buffer here.
  //----------------------------------------------------------------------------
  IndexBuffer::IndexBuffer(const uint* bufferData,
                           unsigned numIndices,
                           BufferType bufferType)
    : hasData(true)
    , type(bufferType)
    , bufferData(bufferData)
    , count(numIndices)
  {
    glGenBuffers(1, &this->bufferID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->bufferID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * numIndices, bufferData,
                 static_cast<GLenum>(bufferType));
  }

  IndexBuffer::~IndexBuffer()
  {
    glDeleteBuffers(1, &this->bufferID);
  }

  // Bind the buffer.
  void
  IndexBuffer::bind()
  {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->bufferID);
  }

  // Unbind the buffer.
  void
  IndexBuffer::unbind()
  {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  }

  // Get the number of stored vertices.
  unsigned
  IndexBuffer::getCount()
  {
    return this->count;
  }

  //----------------------------------------------------------------------------
  // Uniform buffer here.
  //----------------------------------------------------------------------------
  UniformBuffer::UniformBuffer(const unsigned &bufferSize, BufferType bufferType)
    : type(bufferType)
    , dataSize(bufferSize)
    , filled(false)
  {
    glGenBuffers(1, &this->bufferID);
    glBindBuffer(GL_UNIFORM_BUFFER, this->bufferID);
    glBufferData(GL_UNIFORM_BUFFER, bufferSize, nullptr, static_cast<GLenum>(bufferType));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
  }

  UniformBuffer::UniformBuffer(const void* bufferData, const unsigned &dataSize,
                               BufferType bufferType)
    : type(bufferType)
    , dataSize(dataSize)
    , filled(true)
  {
    glGenBuffers(1, &this->bufferID);
    glBindBuffer(GL_UNIFORM_BUFFER, this->bufferID);
    glBufferData(GL_UNIFORM_BUFFER, dataSize, nullptr, static_cast<GLenum>(bufferType));
    glBufferSubData(GL_UNIFORM_BUFFER, 0, dataSize, bufferData);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
  }

  UniformBuffer::UniformBuffer()
    : type(BufferType::Static)
    , dataSize(0)
    , filled(false)
  {
    glGenBuffers(1, &this->bufferID);
  }

  UniformBuffer::~UniformBuffer()
  {
    glDeleteBuffers(1, &this->bufferID);
  }

  // Bind/unbind the buffer.
  void
  UniformBuffer::bindToPoint(const uint bindPoint)
  {
    glBindBuffer(GL_UNIFORM_BUFFER, this->bufferID);
    glBindBufferBase(GL_UNIFORM_BUFFER, bindPoint, this->bufferID);
  }

  void
  UniformBuffer::bind()
  {
    glBindBuffer(GL_UNIFORM_BUFFER, this->bufferID);
  }

  void
  UniformBuffer::unbind()
  {
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
  }

  // Set a specific part of the buffer data.
  void
  UniformBuffer::setData(uint start, uint newDataSize, const void* newData)
  {
    assert(("New data exceeds buffer size.", !(start + newDataSize > this->dataSize)));

    this->bind();
    glBufferSubData(GL_UNIFORM_BUFFER, start, newDataSize, newData);
    this->unbind();

    this->filled = true;
  }

  RenderBuffer::RenderBuffer()
    : format(RBOInternalFormat::DepthStencil)
    , width(0)
    , height(0)
  {
    glGenRenderbuffers(1, &this->bufferID);
  }

  RenderBuffer::RenderBuffer(uint width, uint height)
    : format(RBOInternalFormat::DepthStencil)
    , width(width)
    , height(height)
  {
    glGenRenderbuffers(1, &this->bufferID);
    glBindRenderbuffer(GL_RENDERBUFFER, this->bufferID);

    glRenderbufferStorage(GL_RENDERBUFFER,
                          static_cast<GLenum>(RBOInternalFormat::DepthStencil),
                          width, height);
  }

  RenderBuffer::RenderBuffer(uint width, uint height,
                             const RBOInternalFormat &format)
    : format(format)
    , width(width)
    , height(height)
  {
    glGenRenderbuffers(1, &this->bufferID);
    glBindRenderbuffer(GL_RENDERBUFFER, this->bufferID);

    glRenderbufferStorage(GL_RENDERBUFFER, static_cast<GLenum>(format),
                          width, height);
  }

  RenderBuffer::~RenderBuffer()
  {
    glDeleteRenderbuffers(1, &this->bufferID);
  }

  void 
  RenderBuffer::reset(uint newWidth, uint newHeight)
  {
    this->width = newWidth;
    this->height = newHeight;
    glBindRenderbuffer(GL_RENDERBUFFER, this->bufferID);

    glRenderbufferStorage(GL_RENDERBUFFER, static_cast<GLenum>(this->format),
                          this->width, this->height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
  }

  void 
  RenderBuffer::reset(uint newWidth, uint newHeight, const RBOInternalFormat& newFormat)
  {
    this->format = newFormat;
    this->width = newWidth;
    this->height = newHeight;
    glBindRenderbuffer(GL_RENDERBUFFER, this->bufferID);

    glRenderbufferStorage(GL_RENDERBUFFER, static_cast<GLenum>(this->format),
                          this->width, this->height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
  }

  void
  RenderBuffer::bind()
  {
    glBindRenderbuffer(GL_RENDERBUFFER, this->bufferID);
  }

  void
  RenderBuffer::unbind()
  {
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
  }

  //----------------------------------------------------------------------------
  // Shader storage buffer here.
  //----------------------------------------------------------------------------
  ShaderStorageBuffer::ShaderStorageBuffer(const void* bufferData,
                                           const unsigned &dataSize,
                                           BufferType bufferType)
    : filled(true)
    , type(bufferType)
    , dataSize(dataSize)
  {
    glGenBuffers(1, &this->bufferID);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, this->bufferID);
    glBufferData(GL_SHADER_STORAGE_BUFFER, dataSize, bufferData,
                 static_cast<GLenum>(bufferType));
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
  }

  ShaderStorageBuffer::ShaderStorageBuffer(const unsigned &bufferSize,
                                           BufferType bufferType)
    : filled(false)
    , type(bufferType)
    , dataSize(bufferSize)
  {
    glGenBuffers(1, &this->bufferID);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, this->bufferID);
    glBufferData(GL_SHADER_STORAGE_BUFFER, bufferSize, nullptr, static_cast<GLenum>(bufferType));
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
  }

  ShaderStorageBuffer::~ShaderStorageBuffer()
  {
    glDeleteBuffers(1, &this->bufferID);
  }

  void
  ShaderStorageBuffer::bind()
  {
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, this->bufferID);
  }

  void
  ShaderStorageBuffer::bindToPoint(const uint bindPoint)
  {
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, this->bufferID);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, bindPoint, this->bufferID);
  }

  void
  ShaderStorageBuffer::unbind()
  {
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
  }

  void
  ShaderStorageBuffer::setData(uint start, uint newDataSize,
                               const void* newData)
  {
    assert(("New data exceeds buffer size.", !(start + newDataSize > this->dataSize)));

    this->bind();
    glBufferSubData(GL_SHADER_STORAGE_BUFFER, start, newDataSize, newData);
    this->unbind();

    this->filled = true;
  }
}

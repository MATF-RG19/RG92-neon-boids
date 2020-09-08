//
// Created by rentorious on 08/09/2020.
//

#ifndef NEONBOIDS_VERTEXARRAY_H
#define NEONBOIDS_VERTEXARRAY_H

#endif //NEONBOIDS_VERTEXARRAY_H

#pragma once

#include "VertexBuffer.h"

class VertexBufferLayout;

class VertexArray {
private:
    unsigned int m_RendererID;
public:
    VertexArray();

    ~VertexArray();

    void AddBuffer(const VertexBuffer &vb, const VertexBufferLayout layout);

    void Bind() const;
    void Unbind() const;
};
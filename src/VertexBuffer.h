//
// Created by rentorious on 07/09/2020.
//

#ifndef NEONBOIDS_VERTEXBUFFER_H
#define NEONBOIDS_VERTEXBUFFER_H

#endif //NEONBOIDS_VERTEXBUFFER_H

#pragma once

class VertexBuffer {
private:
    unsigned int m_RendererID;
public:
    VertexBuffer(const void *data, unsigned  int size);
    ~VertexBuffer();

    void Bind() const;
    void Unbind() const;
};

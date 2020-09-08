//
// Created by rentorious on 07/09/2020.
//

#ifndef NEONBOIDS_VERTEXBUFFER_H
#define NEONBOIDS_VERTEXBUFFER_H

#endif //NEONBOIDS_VERTEXBUFFER_H

#pragma once

class IndexBuffer {
private:
    unsigned int m_RendererID;
    unsigned int m_Count;
public:
    IndexBuffer(const unsigned int *data, unsigned int count);

    ~IndexBuffer();

    void Bind() const;

    void Unbind() const;


    inline unsigned int GetCount() const {return m_Count;}


};
